#pragma once

namespace UsualEngine
{
	static constexpr int MAX_SHADOW_MAP = 3;
}
const float FRAME_BUFFER_W = 1280.0f;				//フレームバッファの幅。
const float FRAME_BUFFER_H = 720.0f;				//フレームバッファの高さ。
const float SPRITE_SPACE_SCREEN_W = 1280.0f;
const float SPRITE_SPACE_SCREEN_H = 720.0f;

static const int MAX_BONE = 512;	//!<ボーンの最大数。

static const int RTV_MAX = 8;
#define DEBUG_FUNC 1

#include <stdio.h>
#include <stdlib.h>
#include <memory>
#include <math.h>
#include <vector>
#include <list>
#include <array>
#include <D3Dcompiler.h>
#include <Windows.h>
#include <d3d11.h>
#include <map>
#include <functional>
#include <algorithm>

#include "btBulletDynamicsCommon.h"
#include "BulletCollision\CollisionDispatch\btGhostObject.h"

//DirectXTKのインクルードファイル。
#include "DirectXTK/Inc/Model.h"
#include "DirectXTK/Inc/Effects.h"	
#include "DirectXTK/Inc/CommonStates.h"
#include "DirectXTK/Inc/SpriteFont.h"
#include "DirectXTK/Inc/DDSTextureLoader.h"

#include "HID/Pad.h"




#include "Physics/CollisionAttr.h"
//#include "physics/Physics.h"
//#include "physics/CapsuleCollider.h"
//#include "physics/PhysicsStaticObject.h"



#include "util/Util.h"

#include "math/Vector.h"
#include "math/Matrix.h"
#include "graphics/GraphicsEngine.h"
#include "graphics/Wraped/ViewPort.h"
#include "graphics/RenderTarget.h"
#include "graphics/ConstantBuffer.h"
#include "graphics/StructuredBuffer.h"


#include "graphics/Primitive.h"
#include "graphics/Shader.h"

#include "graphics/SpriteDataManager.h"
#include "graphics/Sprite.h"
#include "graphics/Font/Font.h"

#include "graphics/SkinModelShaderConst.h"
#include "graphics/SkinModel.h"

#include "graphics/Skeleton.h"
#include "graphics/ShadowMap.h"

#include "graphics/animation/AnimationClip.h"


#include "graphics/animation/Animation.h"

#include "graphics/Camera.h"

#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Timer/GameTime.h"
#include "Timer/Stopwatch.h"

#include "UsualEngine.h"


#include "graphics/SpriteRender.h"
#include "graphics/LightBase.h"
#include "graphics/LightDirection.h"
#include "graphics/LightPoint.h"

#include "character/CharacterController.h"

#include "graphics/SkinModelRender.h"
#include "Prefab/SkinModelRender4Ground.h"
#include "level/Level.h"
#include "graphics/ShaderSample.h"

#if DEBUG_FUNC
//#include "Debug/DebugLog.h"

#include "Debug/Debug.h"
#include "Debug/DebugSwitch.h"
#endif
namespace UsualEngine
{
	using SMR4Ground = SkinModelRender4Ground;
}

