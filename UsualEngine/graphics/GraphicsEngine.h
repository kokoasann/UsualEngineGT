#pragma once

#include "RenderTarget.h"
#include "LightManager.h"
#include "ShadowMap.h"
#include "PostEffect/PostEffect.h"
#include "PreRender.h"

namespace UsualEngine
{
	/// <summary>
	/// 
	/// </summary>
	enum EnRenderMode
	{
		enRenderMode_ShadowMap,		//�V���h�E�}�b�v�֕`��
		enRenderMode_3DModel,			//3D���f����`��
	};

	/*!
	 *@brief	�O���t�B�b�N�X�G���W���B
	 */
	class GraphicsEngine
	{
	public:
		GraphicsEngine();
		~GraphicsEngine();
		/*!
		 *@brief	�������B
		 *@param[in]	hWnd		�E�B���h�E�n���h���B
		 */
		void Init(HWND hWnd);
		void InitBackBuffer();
		/*!
		 *@brief	����B
		 */
		void Release();
		/*!
		 *@brief	D3D11�f�o�C�X���擾�B
		 */
		ID3D11Device* GetD3DDevice()
		{
			return m_pd3dDevice;
		}
		/*!
		 *@brief	D3D11�f�o�C�X�R���e�L�X�g���擾�B
		 */
		ID3D11DeviceContext* GetD3DDeviceContext()
		{
			return m_pd3dDeviceContext;
		}

		LightManager& GetLightManager()
		{
			return m_lightManager;
		}

		/// <summary>
		/// �����_�[�^�[�Q�b�g���Z�b�g����
		/// </summary>
		/// <param name="targetCount">�^�[�Q�b�g�̐�</param>
		/// <param name="rtlist">�����_�[�^�[�Q�b�g�z��</param>
		void OMSetRenderTarget(int targetCount, RenderTarget* rtlist[]);

		/// <summary>
		/// ���݂̃����_�[�^�[�Q�b�g���Q�b�g����
		/// </summary>
		/// <param name="targetCount">�^�[�Q�b�g�̐�������</param>
		/// <param name="rtlist">�����_�[�^�[�Q�b�g������</param>
		void OMGetRenderTargets(int& targetCount, RenderTarget* rtlist[]);
		/*!
		 *@brief	�`��J�n�B
		 */
		void BegineRender();
		/*!
		 *@brief	�`��I���B
		 */
		void EndRender();

		void EndPostEffect();

		void PreRenderDraw();

		ShadowMap& GetShadowMap()
		{
			return m_shadowMap;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rm"></param>
		void SetRenderMode(EnRenderMode rm)
		{
			m_renderMode = rm;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		EnRenderMode GetRenderMode()
		{
			return m_renderMode;
		}

		/// <summary>
		/// ���C�������_�[�^�[�Q�b�g��Ԃ�
		/// </summary>
		/// <returns></returns>
		RenderTarget* GetMainRenderTarget()
		{
			return &m_mainRenderTarget;
		}

		/// <summary>
		/// �|�X�g�G�b�t�F�N�g�̃C���b�X�^���X�B
		/// </summary>
		/// <returns></returns>
		PostEffect& GetPostEffect()
		{
			return m_postEffect;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		PreRender& GetPreRender()
		{
			return m_preRender;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		DirectX::SpriteBatch* GetSpriteBatch()
		{
			return m_spriteBatch;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		DirectX::SpriteFont* GetSpriteFont()
		{
			return m_spriteFont;
		}
	private:
		D3D_FEATURE_LEVEL		m_featureLevel;				//Direct3D �f�o�C�X�̃^�[�Q�b�g�ƂȂ�@�\�Z�b�g�B
		ID3D11Device* m_pd3dDevice = NULL;					//D3D11�f�o�C�X�B
		IDXGISwapChain* m_pSwapChain = NULL;				//�X���b�v�`�F�C���B
		ID3D11DeviceContext* m_pd3dDeviceContext = NULL;	//D3D11�f�o�C�X�R���e�L�X�g�B
		RenderTarget m_mainRenderTarget;
		ID3D11RenderTargetView* m_backBuffer = NULL;		//�o�b�N�o�b�t�@�B
		ID3D11RasterizerState* m_rasterizerState = NULL;	//���X�^���C�U�X�e�[�g�B
		ID3D11Texture2D* m_depthStencil = NULL;				//�f�v�X�X�e���V���B
		ID3D11DepthStencilView* m_depthStencilView = NULL;	//�f�v�X�X�e���V���r���[�B

		Shader m_vsCopy;									//�R�s�[�p���_�V�F�[�_
		Shader m_psCopy;									//�R�s�[�p�s�N�Z���V�F�[�_
		Shader m_vsDefferd;								//�f�t�@�[�h�p���_�V�F�[�_
		Shader m_psDefferd;								//�f�t�@�[�h�p�s�N�Z���V�F�[�_
#if DEBUG_FUNC
		Shader m_psDefferd_Diffuse;						//�f�t�@�[�h�p�s�N�Z���V�F�[�_(�f�o�b�N�p)
		Shader m_psDefferd_Normal;						//�f�t�@�[�h�p�s�N�Z���V�F�[�_(�f�o�b�N�p)
		Shader m_psDefferd_Depth;						//�f�t�@�[�h�p�s�N�Z���V�F�[�_(�f�o�b�N�p)
		Shader m_psDefferd_Shadow;						//�f�t�@�[�h�p�s�N�Z���V�F�[�_(�f�o�b�N�p)
		Shader m_psDefferd_Specular;					//�f�t�@�[�h�p�s�N�Z���V�F�[�_(�f�o�b�N�p)
#endif
		
		ShadowMap m_shadowMap;								//�V���h�E�}�b�v
		LightManager m_lightManager;							//���C�g�̃}�l�[�W���[
		RenderTarget* m_nowRenderTargets[RTV_MAX] = { 0 };	//���̃����_�[�^�[�Q�b�g
		int m_renderTargetCount = 1;						//���̃����_�[�^�[�Q�b�g�̐�

		PreRender m_preRender;							//�v�������_�[
		PostEffect m_postEffect;							//�|�X�g�G�b�t�F�N�g
		
		EnRenderMode m_renderMode = enRenderMode_3DModel;	//���ǂ̍H���H

		ID3D11ShaderResourceView* m_speculaGradation=nullptr;		//�g�D�[���V�F�[�h�p�̃O���f�[�V�����}�b�v
		DirectX::SpriteBatch* m_spriteBatch = nullptr;							//�X�v���C�g�o�b�`
		DirectX::SpriteFont* m_spriteFont = nullptr;								//�X�v���C�g�t�H���g
	};
}