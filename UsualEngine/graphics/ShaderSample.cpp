#include "PreCompile.h"
#include "ShaderSample.h"


namespace UsualEngine
{
	Shader ShaderSample::VS_Model;
	Shader ShaderSample::VS_Model_Skin;
	Shader ShaderSample::PS_Model;
	Shader ShaderSample::PS_AlphaModel;
	Shader ShaderSample::PS_AlphaModel_Specular;

	Shader ShaderSample::VS_Sprite;
	Shader ShaderSample::PS_Sprite;

	Shader ShaderSample::VS_Copy;
	Shader ShaderSample::PS_Copy;
	Shader ShaderSample::CS_Copy_1;

	void ShaderSample::Init()
	{
		VS_Model.Load("Assets/shader/model.fx", "VSMain", Shader::EnType::VS);
		VS_Model_Skin.Load("Assets/shader/model.fx", "VSMainSkin", Shader::EnType::VS);
		PS_Model.Load("Assets/shader/model.fx", "PSMain", Shader::EnType::PS);
		PS_AlphaModel.Load("Assets/shader/AlphaModel.fx", "PSMain_Alpha", Shader::EnType::PS);
		PS_AlphaModel_Specular.Load("Assets/shader/AlphaModel.fx", "PSMain_Alpha_Specular", Shader::EnType::PS);

		VS_Sprite.Load("Assets/shader/sprite.fx", "VSMain", Shader::EnType::VS);
		PS_Sprite.Load("Assets/shader/sprite.fx", "PSMain", Shader::EnType::PS);

		VS_Copy.Load("Assets/shader/Copy.fx", "VSMain", Shader::EnType::VS);
		PS_Copy.Load("Assets/shader/Copy.fx", "PSMain", Shader::EnType::PS);
		CS_Copy_1.Load("Assets/shader/Copy.fx", "CSMain_Copy", Shader::EnType::CS);
	}
}