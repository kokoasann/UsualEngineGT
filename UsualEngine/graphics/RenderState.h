#pragma once

namespace UsualEngine
{
	class RenderState
	{
	private:
		RenderState();
		~RenderState();
	public:
		/// <summary>
		/// ���̃N���X�̃C���X�^���X��������
		/// </summary>
		/// <returns></returns>
		static RenderState& Get()
		{
			static RenderState rs;
			return rs;
		}

		/*///////////////////////////////////////////////////////////////////////////////////////////////
			BLEND STATE
		*//////////////////////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// blendstate�̎��
		/// </summary>
		enum BlendStateKind
		{
			bsDisable,
			bsTrans,
			bsAdd,
		};
		/// <summary>
		/// blendstate���W�߂��\����
		/// </summary>
		struct BlendState
		{
			ID3D11BlendState* disable = nullptr;	//����
			ID3D11BlendState* trans = nullptr;		//������
			ID3D11BlendState* add = nullptr;		//���Z
		};
		/// <summary>
		/// blendstate�̏�����
		/// </summary>
		void InitBlendState();
		/// <summary>
		/// 
		/// </summary>
		void ReleaseBlendState();
		/// <summary>
		/// blendstate��������
		/// ������blendstate��kind�Ɉˑ�����
		/// </summary>
		/// <param name="kind">enum BlendStateKind�̂����ꂩ�̒l</param>
		/// <returns></returns>
		ID3D11BlendState* GetBlendState(int kind);


		/*////////////////////////////////////////////////////////////////////////////////////////
			SAMPLER STATE
		*///////////////////////////////////////////////////////////////////////////////////////

		/// <summary>
		/// 
		/// </summary>
		enum SamplerStateKind
		{
			ssPoint,
			ssLiner,
		};
		/// <summary>
		/// 
		/// </summary>
		struct SamplerState
		{
			ID3D11SamplerState* pointSampling = 0;
			ID3D11SamplerState* linerSampling = 0;
		};
		/// <summary>
		/// 
		/// </summary>
		void InitSamplerState();
		/// <summary>
		/// 
		/// </summary>
		void ReleaseSamplerState();
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kind"></param>
		/// <returns></returns>
		ID3D11SamplerState* GetSamplerState(int kind);


		/*////////////////////////////////////////////////////////////////////////////////////////
			DEPTH STENCIL STATE
		*///////////////////////////////////////////////////////////////////////////////////////

		enum DepthStencilStateKind
		{
			dssSceneRender,
			dssDeferredRender,
			dssSpriteRender,
		};
		struct DepthStencilState
		{
			ID3D11DepthStencilState* sceneRender=0;
			ID3D11DepthStencilState* deferredRender = 0;
			ID3D11DepthStencilState* spriteRender = 0;
		};
		void InitDepthStencilState();
		void ReleaseDepthStencilState();
		ID3D11DepthStencilState* GetDepthStencilState(DepthStencilStateKind kind);
	private:
		BlendState m_blendStatePreset;				//blendstate
		SamplerState m_samplerStatePreset;
		DepthStencilState m_depthStencilStatePreset;
	};

	/// <summary>
		/// blendstate���W�߂��\����
		/// </summary>
	struct BlendState
	{
		static ID3D11BlendState* disable;	//����
		static ID3D11BlendState* trans;		//������
		static ID3D11BlendState* add;		//���Z
		
		static void Init();
		static void Release();
	};

	struct SamplerState
	{
		static ID3D11SamplerState* pointSampling;
		static ID3D11SamplerState* linerSampling;

		static void Init();
		static void Release();
	};

	struct DepthStencilState
	{
		static ID3D11DepthStencilState* sceneRender;
		static ID3D11DepthStencilState* deferredRender;
		static ID3D11DepthStencilState* spriteRender;

		static void Init();
		static void Release();
	};

	struct RasterizerState
	{
		static ID3D11RasterizerState* sceneRender;
		static ID3D11RasterizerState* spriteRender;

		static void Init();
		static void Release();
	};

	/// <summary>
	/// �A���t�@�u�����h������BlendState��������
	/// </summary>
	/// <returns></returns>
	static ID3D11BlendState* BlendState_Disable()
	{
		//return RenderState::Get().GetBlendState(RenderState::bsDisable);
		return BlendState::disable;
	}
	/// <summary>
	/// ������������BlendState��������
	/// </summary>
	/// <returns></returns>
	static ID3D11BlendState* BlendState_Trans()
	{
		//return RenderState::Get().GetBlendState(RenderState::bsTrans);
		return BlendState::trans;
	}
	/// <summary>
	/// ���Z������BlendState��������
	/// </summary>
	/// <returns></returns>
	static ID3D11BlendState* BlendState_Add()
	{
		//return RenderState::Get().GetBlendState(RenderState::bsAdd);
		return BlendState::add;
	}


	/// <summary>
	/// �|�C���g�T���v�����O
	/// </summary>
	/// <returns></returns>
	static ID3D11SamplerState* SamplerState_Point()
	{
		//return RenderState::Get().GetSamplerState(RenderState::ssPoint);
		return SamplerState::pointSampling;
	}
	/// <summary>
	/// ���`�⊮�̃T���v���X�e�[�g
	/// </summary>
	/// <returns></returns>
	static ID3D11SamplerState* SamplerState_Liner()
	{
		//return RenderState::Get().GetSamplerState(RenderState::ssLiner);
		return SamplerState::linerSampling;
	}
}