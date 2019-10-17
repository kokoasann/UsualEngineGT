#include "PreCompile.h"
#include "SkinModel.h"
#include "SkinModelDataManager.h"

namespace UsualEngine
{
	SkinModel::~SkinModel()
	{
		if (m_cb != nullptr) {
			//定数バッファを解放。
			m_cb->Release();
		}
		if (m_samplerState != nullptr) {
			//サンプラステートを解放。
			m_samplerState->Release();
		}
	}
	void SkinModel::Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis)
	{
		//スケルトンのデータを読み込む。
		InitSkeleton(filePath);

		//定数バッファの作成。
		InitConstantBuffer();

		//サンプラステートの初期化。
		InitSamplerState();

		//SkinModelDataManagerを使用してCMOファイルのロード。
		m_modelDx = g_skinModelDataManager.Load(filePath, m_skeleton);

		m_enFbxUpAxis = enFbxUpAxis;
	}
	void SkinModel::InitSkeleton(const wchar_t* filePath)
	{
		//スケルトンのデータを読み込む。
		//cmoファイルの拡張子をtksに変更する。
		std::wstring skeletonFilePath = filePath;
		//文字列から.cmoファイル始まる場所を検索。
		int pos = (int)skeletonFilePath.find(L".cmo");
		//.cmoファイルを.tksに置き換える。
		skeletonFilePath.replace(pos, 4, L".tks");
		//tksファイルをロードする。
		bool result = m_skeleton.Load(skeletonFilePath.c_str());
		if (result == false) {
			//スケルトンが読み込みに失敗した。
			//アニメーションしないモデルは、スケルトンが不要なので
			//読み込みに失敗することはあるので、ログ出力だけにしておく。
#ifdef _DEBUG
			char message[256];
			sprintf(message, "tksファイルの読み込みに失敗しました。%ls\n", skeletonFilePath.c_str());
			OutputDebugStringA(message);
#endif
		}
	}
	void SkinModel::InitConstantBuffer()
	{
		if (m_cb != nullptr)
			return;
		//作成するバッファのサイズをsizeof演算子で求める。
		int bufferSize = sizeof(SVSConstantBuffer);
		//どんなバッファを作成するのかをせてbufferDescに設定する。
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));				//０でクリア。
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//バッファで想定されている、読み込みおよび書き込み方法。
		bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//バッファは16バイトアライメントになっている必要がある。
																	//アライメントって→バッファのサイズが16の倍数ということです。
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//バッファをどのようなパイプラインにバインドするかを指定する。
																	//定数バッファにバインドするので、D3D11_BIND_CONSTANT_BUFFERを指定する。
		bufferDesc.CPUAccessFlags = 0;								//CPU アクセスのフラグです。
																	//CPUアクセスが不要な場合は0。
		//作成。
		usualEngine()->GetGraphicsEngine()->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_cb);
	}
	void SkinModel::InitSamplerState()
	{
		if (m_samplerState != nullptr)
			return;
		//テクスチャのサンプリング方法を指定するためのサンプラステートを作成。
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		usualEngine()->GetGraphicsEngine()->GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
	}
	void SkinModel::UpdateWorldMatrix(CVector3 position, CQuaternion rotation, CVector3 scale)
	{
		//3dsMaxと軸を合わせるためのバイアス。
		CMatrix mBias = CMatrix::Identity();
		if (m_enFbxUpAxis == enFbxUpAxisZ) {
			//Z-up
			mBias.MakeRotationX(CMath::PI * -0.5f);
			float z = scale.z;
			scale.z = scale.y;
			scale.y = z;
		}
		CMatrix transMatrix, rotMatrix, scaleMatrix;
		//平行移動行列を作成する。
		transMatrix.MakeTranslation(position);
		//回転行列を作成する。
		rotMatrix.MakeRotationFromQuaternion(rotation);
		rotMatrix.Mul(mBias, rotMatrix);
		//拡大行列を作成する。
		scaleMatrix.MakeScaling(scale);
		//ワールド行列を作成する。
		//拡大×回転×平行移動の順番で乗算するように！
		//順番を間違えたら結果が変わるよ。
		m_worldMatrix.Mul(scaleMatrix, rotMatrix);
		m_worldMatrix.Mul(m_worldMatrix, transMatrix);

		//スケルトンの更新。
		m_skeleton.Update(m_worldMatrix);

		if(m_isShadowCaster)
			usualEngine()->GetGraphicsEngine()->GetShadowMap().AddShadowCaster(this);
	}
	void SkinModel::Draw(CMatrix viewMatrix, CMatrix projMatrix)
	{
		DirectX::CommonStates state(usualEngine()->GetGraphicsEngine()->GetD3DDevice());

		ID3D11DeviceContext* d3dDeviceContext = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();

		//定数バッファの内容を更新。
		SVSConstantBuffer vsCb;
		vsCb.mWorld = m_worldMatrix;
		vsCb.mProj = projMatrix;
		vsCb.mView = viewMatrix;
		auto& maincam = usualEngine()->GetMainCamera();
		CVector3 camdir = maincam.GetTarget() - maincam.GetPosition();
		camdir.Normalize();
		vsCb.camDir =camdir;
		vsCb.isShadowReciever = m_isShadowReciever;

		d3dDeviceContext->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);
		//定数バッファをGPUに転送。
		d3dDeviceContext->VSSetConstantBuffers(enSkinModelCBReg_VSPS, 1, &m_cb);
		d3dDeviceContext->PSSetConstantBuffers(enSkinModelCBReg_VSPS, 1, &m_cb);
		//サンプラステートを設定。
		d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
		//ボーン行列をGPUに転送。
		m_skeleton.SendBoneMatrixArrayToGPU();


		//描画。
		m_modelDx->Draw(
			d3dDeviceContext,
			state,
			m_worldMatrix,
			viewMatrix,
			projMatrix
		);
	}
}