#include "PreCompile.h"
#include "SkinModel.h"
#include "SkinModelDataManager.h"

namespace UsualEngine
{
	SkinModel::~SkinModel()
	{
		Release();
	}
	void SkinModel::Release()
	{
		if (m_cb != nullptr) {
			//�萔�o�b�t�@������B
			m_cb->Release();
		}
		if (m_samplerState != nullptr) {
			//�T���v���X�e�[�g������B
			m_samplerState->Release();
		}
		if (m_isInstancing)
		{
			delete[] m_instancingData;
			m_instancingDataSB.Release();
		}
		m_skeleton.Release();
	}
	void SkinModel::Init(const wchar_t* filePath, EnFbxUpAxis enFbxUpAxis)
	{
		//�X�P���g���̃f�[�^��ǂݍ��ށB
		InitSkeleton(filePath);

		//�萔�o�b�t�@�̍쐬�B
		InitConstantBuffer();

		//�T���v���X�e�[�g�̏������B
		InitSamplerState();

		//SkinModelDataManager���g�p����CMO�t�@�C���̃��[�h�B
		m_modelDx = g_skinModelDataManager.Load(filePath, m_skeleton);

		m_enFbxUpAxis = enFbxUpAxis;
	}
	void SkinModel::InitInstancing(unsigned int maxInstance)
	{
		m_instancingData = new CMatrix[maxInstance];
		D3D11_BUFFER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.ByteWidth = sizeof(CMatrix) * maxInstance;
		desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		desc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		desc.StructureByteStride = sizeof(CMatrix);
		m_instancingDataSB.Create(m_instancingData, desc);
		m_maxInstance = maxInstance;

		for (auto& meshs : m_modelDx->meshes)
		{
			for (std::unique_ptr<DirectX::ModelMeshPart>& mesh : meshs->meshParts)
			{
				ModelEffect* eff = reinterpret_cast<ModelEffect*>(mesh->effect.get());
				eff->Instancing();
			}
		}
	}
	void SkinModel::InitSkeleton(const wchar_t* filePath)
	{
		//�X�P���g���̃f�[�^��ǂݍ��ށB
		//cmo�t�@�C���̊g���q��tks�ɕύX����B
		std::wstring skeletonFilePath = filePath;
		//�����񂩂�.cmo�t�@�C���n�܂�ꏊ�������B
		int pos = (int)skeletonFilePath.find(L".cmo");
		//.cmo�t�@�C����.tks�ɒu��������B
		skeletonFilePath.replace(pos, 4, L".tks");
		//tks�t�@�C�������[�h����B
		bool result = m_skeleton.Load(skeletonFilePath.c_str());
		if (result == false) {
			//�X�P���g�����ǂݍ��݂Ɏ��s�����B
			//�A�j���[�V�������Ȃ����f���́A�X�P���g�����s�v�Ȃ̂�
			//�ǂݍ��݂Ɏ��s���邱�Ƃ͂���̂ŁA���O�o�͂����ɂ��Ă����B
#ifdef _DEBUG
			char message[256];
			sprintf(message, "tks�t�@�C���̓ǂݍ��݂Ɏ��s���܂����B%ls\n", skeletonFilePath.c_str());
			OutputDebugStringA(message);
#endif
		}
	}
	void SkinModel::InitConstantBuffer()
	{
		if (m_cb != nullptr)
			return;
		//�쐬����o�b�t�@�̃T�C�Y��sizeof���Z�q�ŋ��߂�B
		int bufferSize = sizeof(SVSConstantBuffer);
		//�ǂ�ȃo�b�t�@���쐬����̂�������bufferDesc�ɐݒ肷��B
		D3D11_BUFFER_DESC bufferDesc;
		ZeroMemory(&bufferDesc, sizeof(bufferDesc));				//�O�ŃN���A�B
		bufferDesc.Usage = D3D11_USAGE_DEFAULT;						//�o�b�t�@�őz�肳��Ă���A�ǂݍ��݂���я������ݕ��@�B
		bufferDesc.ByteWidth = (((bufferSize - 1) / 16) + 1) * 16;	//�o�b�t�@��16�o�C�g�A���C�����g�ɂȂ��Ă���K�v������B
																	//�A���C�����g���ā��o�b�t�@�̃T�C�Y��16�̔{���Ƃ������Ƃł��B
		bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;			//�o�b�t�@���ǂ̂悤�ȃp�C�v���C���Ƀo�C���h���邩���w�肷��B
																	//�萔�o�b�t�@�Ƀo�C���h����̂ŁAD3D11_BIND_CONSTANT_BUFFER���w�肷��B
		bufferDesc.CPUAccessFlags = 0;								//CPU �A�N�Z�X�̃t���O�ł��B
																	//CPU�A�N�Z�X���s�v�ȏꍇ��0�B
		//�쐬�B
		usualEngine()->GetGraphicsEngine()->GetD3DDevice()->CreateBuffer(&bufferDesc, NULL, &m_cb);
	}


	void SkinModel::SetSpecularMap(const wchar_t* path)
	{
		//std::wstring p = path;
		m_specularMap = SpriteDataManager::Get()->Load(path);
	}
	void SkinModel::InitSamplerState()
	{
		if (m_samplerState != nullptr)
			return;
		//�e�N�X�`���̃T���v�����O���@���w�肷�邽�߂̃T���v���X�e�[�g���쐬�B
		D3D11_SAMPLER_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
		desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		usualEngine()->GetGraphicsEngine()->GetD3DDevice()->CreateSamplerState(&desc, &m_samplerState);
	}
	void SkinModel::UpdateWorldMatrix(const CVector3& position, const CQuaternion& rotation, const CVector3& scal)
	{
		CVector3 scale = scal;
		//3dsMax�Ǝ������킹�邽�߂̃o�C�A�X�B
		CMatrix mBias = CMatrix::Identity();
		if (m_enFbxUpAxis == enFbxUpAxisZ) {
			//Z-up
			mBias.MakeRotationX(CMath::PI * -0.5f);
			float z = scale.z;
			scale.z = scale.y;
			scale.y = z;
		}
		CMatrix transMatrix, rotMatrix, scaleMatrix;
		//���s�ړ��s����쐬����B
		transMatrix.MakeTranslation(position);
		//��]�s����쐬����B
		rotMatrix.MakeRotationFromQuaternion(rotation);
		rotMatrix.Mul(mBias, rotMatrix);
		//�g��s����쐬����B
		scaleMatrix.MakeScaling(scale);
		//���[���h�s����쐬����B
		//�g��~��]�~���s�ړ��̏��Ԃŏ�Z����悤�ɁI
		//���Ԃ��ԈႦ���猋�ʂ��ς���B
		m_worldMatrix.Mul(scaleMatrix, rotMatrix);
		m_worldMatrix.Mul(m_worldMatrix, transMatrix);

		//�X�P���g���̍X�V�B
		m_skeleton.Update(m_worldMatrix);

		if(m_isShadowCaster)
			usualEngine()->GetGraphicsEngine()->GetShadowMap().AddShadowCaster(this);
	}
	void SkinModel::UpdateWorldMatrix(const CMatrix& wmat)
	{
		m_skeleton.Update(wmat);
		if (m_isShadowCaster)//�V���h�E�A���Ƃ��܂����Hnj
			usualEngine()->GetGraphicsEngine()->GetShadowMap().AddShadowCaster(this);
	}
	void SkinModel::Draw(CMatrix viewMatrix, CMatrix projMatrix)
	{
		DirectX::CommonStates state(usualEngine()->GetGraphicsEngine()->GetD3DDevice());

		ID3D11DeviceContext* d3dDeviceContext = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();

		//�萔�o�b�t�@�̓��e���X�V�B
		SVSConstantBuffer vsCb;
		vsCb.mWorld = m_worldMatrix;
		vsCb.mProj = projMatrix;
		vsCb.mView = viewMatrix;
		auto& maincam = usualEngine()->GetMainCamera();
		CVector3 camdir = maincam.GetTarget() - maincam.GetPosition();
		camdir.Normalize();
		vsCb.camDir =camdir;
		vsCb.isShadowReciever = m_isShadowReciever;
		vsCb.isUseSpecularMap = m_specularMap != nullptr;

		d3dDeviceContext->UpdateSubresource(m_cb, 0, nullptr, &vsCb, 0, 0);
		//�萔�o�b�t�@��GPU�ɓ]���B
		d3dDeviceContext->VSSetConstantBuffers(enSkinModelCBReg_VSPS, 1, &m_cb);
		d3dDeviceContext->PSSetConstantBuffers(enSkinModelCBReg_VSPS, 1, &m_cb);

		//d3dDeviceContext->PSSetShaderResources(enSkinModelSRVReg_Specular_1, 1, &m_specularMap);
		//�T���v���X�e�[�g��ݒ�B
		d3dDeviceContext->PSSetSamplers(0, 1, &m_samplerState);
		//�{�[���s���GPU�ɓ]���B
		m_skeleton.SendBoneMatrixArrayToGPU();

		if (m_maxInstance > 0)
		{
			d3dDeviceContext->UpdateSubresource(m_instancingDataSB.GetBody(),0,0, m_instancingData,0,0);
			d3dDeviceContext->VSSetShaderResources(enSkinModelSRVReg_InstancingData, 1, &m_instancingDataSB.GetSRV());
		}

		//�`��B
		m_modelDx->Draw(
			d3dDeviceContext,
			state,
			m_worldMatrix,
			viewMatrix,
			projMatrix,
			0,
			0,
			m_maxInstance > 0 ? m_maxInstance : 1
		);
	}
}