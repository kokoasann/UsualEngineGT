#include "PreCompile.h"
#include "Bloom.h"


namespace UsualEngine
{
	void Bloom::Init()
	{
		m_vs.Load("Assets/shader/bloom.fx","VSMain", Shader::EnType::VS);
		m_ps.Load("Assets/shader/bloom.fx","PSMain_SamplingLuminance", Shader::EnType::PS);
		m_psCombine.Load("Assets/shader/bloom.fx","PSMain_Cambine", Shader::EnType::PS);
		m_psCopy.Load("Assets/shader/copy.fx", "PSMain", Shader::EnType::PS);
		m_vsCopy.Load("Assets/shader/copy.fx", "VSMain", Shader::EnType::VS);

		int w = FRAME_BUFFER_W;
		int h = FRAME_BUFFER_H;
		for(int i=0;i<BLOOM_NUM_DWNSMP;i++)
		{
			int sft = i;
			m_gausBlur[i].Init(w >> sft, h >> sft);
		}
		DXGI_SAMPLE_DESC desc;
		ZeroMemory(&desc, sizeof(desc));
		desc.Count = 1;
		desc.Quality = 0;
		m_combineRT.Create(w >> 2, h >> 2, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, desc);
		m_luminanceRT.Create(w, h, 1, 1, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT_UNKNOWN, desc);

	}
	void Bloom::Render(PostEffect* pe)
	{

		
	}
}