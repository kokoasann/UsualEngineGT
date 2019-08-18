#pragma once


namespace UsualEngine
{
	/// <summary>
	/// directXのViewPortをラッピングしたものがこちらになります。
	/// </summary>
	class ViewPort
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="vp"></param>
		void operator=(ViewPort vp)
		{
			m_viewPort = vp.Get();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		D3D11_VIEWPORT& Get()
		{
			return m_viewPort;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="vp"></param>
		void Set(D3D11_VIEWPORT& vp)
		{
			m_viewPort = vp;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="vp"></param>
		void Set(ViewPort& vp)
		{
			m_viewPort = vp.Get();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="width"></param>
		/// <param name="height"></param>
		/// <param name="topLeftX"></param>
		/// <param name="topLeftY"></param>
		/// <param name="minDepth"></param>
		/// <param name="maxDepth"></param>
		void Set(float width, float height, float topLeftX = 0, float topLeftY = 0, float minDepth = 0, float maxDepth = 1)
		{
			m_viewPort.Width = width;
			m_viewPort.Height = height;
			m_viewPort.TopLeftX = topLeftX;
			m_viewPort.TopLeftY = topLeftY;
			m_viewPort.MinDepth = minDepth;
			m_viewPort.MaxDepth = maxDepth;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		float GetWidth()
		{
			return m_viewPort.Width;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="v"></param>
		void SetWidth(float v)
		{
			m_viewPort.Width = v;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		float GetHeight()
		{
			return m_viewPort.Height;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="v"></param>
		void SetHeight(float v)
		{
			m_viewPort.Height = v;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		float GetTopLeftX()
		{
			return m_viewPort.TopLeftX;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="v"></param>
		void SetTopLeftX(float v)
		{
			m_viewPort.TopLeftX = v;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		float GetTopLeftY()
		{
			return m_viewPort.TopLeftY;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="v"></param>
		void SetTopLeftY(float v)
		{
			m_viewPort.TopLeftY = v;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		float GetMinDepth()
		{
			return m_viewPort.MinDepth;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="v"></param>
		void SetMinDepth(float v)
		{
			m_viewPort.MinDepth = v;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		float GetMaxDepth()
		{
			return m_viewPort.MaxDepth;
		}
		/// <summary>
		/// 
		/// </summary>
		/// <param name="v"></param>
		void SetMaxDepth(float v)
		{
			m_viewPort.MaxDepth = v;
		}
	private:
		D3D11_VIEWPORT m_viewPort = {0,0,0,0,0,0};	//ものほん
	};
}