#pragma once



namespace UsualEngine
{
	class PhysicsDebugDraw:public btIDebugDraw
	{
	private:
		struct SConstantBuffer
		{
			CMatrix mView;
			CMatrix mProj;
		};
	public:
		PhysicsDebugDraw() {}
		~PhysicsDebugDraw(){}

		void Init();
		void BeginRender();
		void EndRender();

		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
		void drawContactPoint(const btVector3&, const btVector3&, btScalar, int, const btVector3&) override
		{
		}
		void reportErrorWarning(const char*) override
		{
		}
		void draw3dText(const btVector3&, const char*) override
		{
		}
		void setDebugMode(int debugMode) override
		{
			m_debugMode = debugMode;
		}
		int	getDebugMode() const override
		{
			return m_debugMode;
		}

	private:
		ConstantBuffer m_constbuff;
		Primitive m_primi;
		Shader m_vs;
		Shader m_ps;
		static const unsigned int MAX_VERTEX = 100000;
		std::array<CVector4, MAX_VERTEX> m_vertexBuffer;
		int m_numLine = 0;

		int m_debugMode = btIDebugDraw::DBG_DrawWireframe;
	};
}