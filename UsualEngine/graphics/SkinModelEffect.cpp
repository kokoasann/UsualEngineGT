#include "PreCompile.h"
#include "graphics/SkinModelEffect.h"
#include "graphics/SkinModelShaderConst.h"

namespace UsualEngine
{
	void __cdecl ModelEffect::Apply(ID3D11DeviceContext* deviceContext)
	{
		EnRenderMode renderMode = usualEngine()->GetGraphicsEngine()->GetRenderMode();
		switch (renderMode)
		{
		case enRenderMode_3DModel:
			deviceContext->VSSetShader((ID3D11VertexShader*)m_pVSShader->GetBody(), NULL, 0);
			deviceContext->PSSetShader((ID3D11PixelShader*)m_pPSShader->GetBody(), NULL, 0);
			break;
		case enRenderMode_ShadowMap:
			deviceContext->VSSetShader((ID3D11VertexShader*)m_vsToDepth.GetBody(), NULL, 0);
			deviceContext->PSSetShader((ID3D11PixelShader*)m_psToDepth.GetBody(), NULL, 0);
			break;
		}
		
		deviceContext->PSSetShaderResources(enSkinModelSRVReg_AlbedoTexture, 1, &m_albedoTex);

	}
}