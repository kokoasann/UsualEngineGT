#pragma once

#include "RenderTarget.h"
#include "LightManager.h"

namespace UsualEngine
{
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
			return mLightManager;
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
	private:
		D3D_FEATURE_LEVEL		m_featureLevel;				//Direct3D �f�o�C�X�̃^�[�Q�b�g�ƂȂ�@�\�Z�b�g�B
		ID3D11Device* m_pd3dDevice = NULL;		//D3D11�f�o�C�X�B
		IDXGISwapChain* m_pSwapChain = NULL;		//�X���b�v�`�F�C���B
		ID3D11DeviceContext* m_pd3dDeviceContext = NULL;	//D3D11�f�o�C�X�R���e�L�X�g�B
		RenderTarget m_renderTarget;
		ID3D11RenderTargetView* m_backBuffer = NULL;		//�o�b�N�o�b�t�@�B
		ID3D11RasterizerState* m_rasterizerState = NULL;	//���X�^���C�U�X�e�[�g�B
		ID3D11Texture2D* m_depthStencil = NULL;		//�f�v�X�X�e���V���B
		ID3D11DepthStencilView* m_depthStencilView = NULL;	//�f�v�X�X�e���V���r���[�B
		

		LightManager mLightManager;						//���C�g�̃}�l�[�W���[
		RenderTarget* m_nowRenderTargets[RTV_MAX];		//���̃����_�[�^�[�Q�b�g
		int m_renderTargetCount = 1;									//���̃����_�[�^�[�Q�b�g�̐�
	};

	//extern GraphicsEngine* g_graphicsEngine;			//�O���t�B�b�N�X�G���W��
}