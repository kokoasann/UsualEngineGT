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
		/// このクラスのインスタンスをかえす
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
		/// blendstateの種類
		/// </summary>
		enum BlendStateKind
		{
			bsDisable,
			bsTrans,
			bsAdd,
		};
		/// <summary>
		/// blendstateを集めた構造体
		/// </summary>
		struct BlendState
		{
			ID3D11BlendState* disable = nullptr;	//無効
			ID3D11BlendState* trans = nullptr;		//半透明
			ID3D11BlendState* add = nullptr;		//加算
		};
		/// <summary>
		/// blendstateの初期化
		/// </summary>
		void InitBlendState();
		/// <summary>
		/// 
		/// </summary>
		void ReleaseBlendState();
		/// <summary>
		/// blendstateをかえす
		/// かえすblendstateはkindに依存する
		/// </summary>
		/// <param name="kind">enum BlendStateKindのいずれかの値</param>
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
		/// blendstateを集めた構造体
		/// </summary>
	struct BlendState
	{
		static ID3D11BlendState* disable;	//無効
		static ID3D11BlendState* trans;		//半透明
		static ID3D11BlendState* add;		//加算
		
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
	/// アルファブレンド無効のBlendStateをかえす
	/// </summary>
	/// <returns></returns>
	static ID3D11BlendState* BlendState_Disable()
	{
		//return RenderState::Get().GetBlendState(RenderState::bsDisable);
		return BlendState::disable;
	}
	/// <summary>
	/// 半透明合成のBlendStateをかえす
	/// </summary>
	/// <returns></returns>
	static ID3D11BlendState* BlendState_Trans()
	{
		//return RenderState::Get().GetBlendState(RenderState::bsTrans);
		return BlendState::trans;
	}
	/// <summary>
	/// 加算合成のBlendStateをかえす
	/// </summary>
	/// <returns></returns>
	static ID3D11BlendState* BlendState_Add()
	{
		//return RenderState::Get().GetBlendState(RenderState::bsAdd);
		return BlendState::add;
	}


	/// <summary>
	/// ポイントサンプリング
	/// </summary>
	/// <returns></returns>
	static ID3D11SamplerState* SamplerState_Point()
	{
		//return RenderState::Get().GetSamplerState(RenderState::ssPoint);
		return SamplerState::pointSampling;
	}
	/// <summary>
	/// 線形補完のサンプラステート
	/// </summary>
	/// <returns></returns>
	static ID3D11SamplerState* SamplerState_Liner()
	{
		//return RenderState::Get().GetSamplerState(RenderState::ssLiner);
		return SamplerState::linerSampling;
	}
}