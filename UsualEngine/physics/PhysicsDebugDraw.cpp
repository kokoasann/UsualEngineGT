#include "PreCompile.h"
#include "Physics/PhysicsDebugDraw.h"

namespace UsualEngine
{
	void PhysicsDebugDraw::Init()
	{
		m_primi.Cteate(D3D_PRIMITIVE_TOPOLOGY_LINELIST, m_vertexBuffer.size(), sizeof(CVector4), &m_vertexBuffer[0], 0, IndexBuffer::e32bit, nullptr);
		m_vs.Load("Assets/shader/linePrimitive.fx", "VSMain", Shader::EnType::VS);
		m_ps.Load("Assets/shader/linePrimitive.fx", "PSMain", Shader::EnType::PS);
		m_constbuff.Create(nullptr, sizeof(SConstantBuffer));
	}

	void PhysicsDebugDraw::BeginRender()
	{
		m_numLine = 0;
	}

	void PhysicsDebugDraw::EndRender()
	{
		SConstantBuffer cb;
		cb.mView = usualEngine()->GetMainCamera().GetViewMatrix();
		cb.mProj = usualEngine()->GetMainCamera().GetProjectionMatrix();
		
		ID3D11DeviceContext* dc = usualEngine()->GetGraphicsEngine()->GetD3DDeviceContext();
		dc->UpdateSubresource(m_constbuff.GetBody(), 0, NULL, &cb, 0, 0);

		dc->IASetInputLayout(m_vs.GetInputLayout());
		dc->VSSetShader((ID3D11VertexShader*)m_vs.GetBody(), 0, 0);
		dc->PSSetShader((ID3D11PixelShader*)m_ps.GetBody(), 0, 0);
		dc->VSSetConstantBuffers(0, 1, &m_constbuff.GetBody());

		dc->UpdateSubresource(m_primi.GetVertexBuffer().GetBody(), 0, 0, &m_vertexBuffer[0], 0, 0);

		m_primi.Draw(dc,m_numLine*2);
	}

	void PhysicsDebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		int base = m_numLine * 2;
		m_vertexBuffer[base].Set(CVector3{ from.x(),from.y(),from.z() });
		m_vertexBuffer[base + 1].Set(CVector3{ to.x(),to.y(),to.z() });
		m_numLine++;
	}

}