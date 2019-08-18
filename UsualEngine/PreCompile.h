#pragma once

namespace UsualEngine
{
	static constexpr int MAX_SHADOW_MAP = 1;
}
const float FRAME_BUFFER_W = 1280.0f;				//フレームバッファの幅。
const float FRAME_BUFFER_H = 720.0f;				//フレームバッファの高さ。

static const int MAX_BONE = 512;	//!<ボーンの最大数。

static const int RTV_MAX = 8;

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

#include "UsualEngine.h"

#include "util/Util.h"

#include "physics/Physics.h"
#include "HID/Pad.h"

#include "math/Vector.h"
#include "math/Matrix.h"
#include "graphics/GraphicsEngine.h"
#include "graphics/Wraped/ViewPort.h"
#include "graphics/RenderTarget.h"
#include "graphics/ConstantBuffer.h"
#include "graphics/StructuredBuffer.h"

#include "graphics/Primitive.h"
#include "graphics/Shader.h"

#include "graphics/Sprite.h"

#include "graphics/SkinModelShaderConst.h"
#include "graphics/SkinModel.h"
#include "graphics/Skeleton.h"
#include "graphics/ShadowMap.h"

#include "graphics/animation/Animation.h"
#include "graphics/animation/AnimationClip.h"
#include "graphics/Camera.h"

#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "Timer/GameTime.h"
#include "Timer/Stopwatch.h"

#include "graphics/SkinModelRender.h"
#include "graphics/SpriteRender.h"
#include "graphics/LightBase.h"
#include "graphics/LightDirection.h"




