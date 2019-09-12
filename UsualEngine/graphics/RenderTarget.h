#pragma once

#include "Wraped/ViewPort.h"
namespace UsualEngine
{
	/// <summary>
	/// 
	/// </summary>
	class RenderTarget
	{
	public:
		RenderTarget();
		~RenderTarget();

		void Release();

		/// <summary>
		/// �����_�����O�^�[�Q�b�g�̍쐬
		/// </summary>
		/// <param name="w">��</param>
		/// <param name="h">����</param>
		/// <param name="mipLevel">�~�b�v�}�b�v���x��</param>
		/// <param name="arraySize">�z��̃T�C�Y</param>
		/// <param name="colformat">�J���[�o�b�t�@�̃t�H�[�}�b�g</param>
		/// <param name="dsFormat">�[�x�X�e���V���̃t�H�[�}�b�g</param>
		/// <param name="msDesc">�}���`�T���v�����O�̒�`</param>
		/// <param name="renderTarget">�����_�[�^�[�Q�b�g�̃e�N�X�`���BNULL�Ȃ珟��ɍ��</param>
		/// <param name="depthStencil">�f�v�X�X�e���V���̃e�N�X�`���BNULL�Ȃ珟��ɍ��</param>
		/// <returns>����������true��Ԃ�</returns>
		bool Create(int w, int h, int mipLevel, int arraySize, DXGI_FORMAT colformat, DXGI_FORMAT dsFormat,
			DXGI_SAMPLE_DESC msDesc, ID3D11Texture2D* renderTarget = nullptr, ID3D11Texture2D* depthStencil = nullptr, bool RTVDisNone =false);

		/// <summary>
		/// renderTargetView��Ԃ�
		/// </summary>
		/// <returns></returns>
		ID3D11RenderTargetView* GetRTV()
		{
			return m_renderTargetView;
		}

		/// <summary>
		/// depthStencilView��Ԃ�
		/// </summary>
		/// <returns></returns>
		ID3D11DepthStencilView* GetDSV()
		{
			return m_depthStencilView;
		}

		/// <summary>
		/// shaderResourceView��Ԃ�
		/// </summary>
		/// <returns></returns>
		ID3D11ShaderResourceView*& GetSRV()
		{
			return m_renderTargetSRV;
		}

		/// <summary>
		/// ����Ԃ�
		/// </summary>
		/// <returns></returns>
		int GetWidth()
		{
			return m_width;
		}

		/// <summary>
		/// �c��Ԃ�
		/// </summary>
		/// <returns></returns>
		int GetHeight()
		{
			return m_height;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		ViewPort GetViewPort()
		{
			return m_viewport;
		}
	private:
		ID3D11Texture2D* m_renderTarget = NULL;
		ID3D11RenderTargetView* m_renderTargetView = NULL;
		ID3D11Texture2D* m_depthStencil = NULL;
		ID3D11DepthStencilView* m_depthStencilView = NULL;
		ID3D11ShaderResourceView* m_renderTargetSRV = NULL;
		ID3D11UnorderedAccessView* m_renderTargetUAV = NULL;
		ID3D11Texture2D* m_resolveTextureMSAA = NULL;

		DXGI_FORMAT m_colFormat = DXGI_FORMAT_UNKNOWN;
		DXGI_FORMAT m_dsFormat = DXGI_FORMAT_UNKNOWN;
		ViewPort m_viewport;
		int m_width = 0;
		int m_height = 0;
		bool m_isMSAA = false;
	};
}