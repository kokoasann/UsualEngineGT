static const float PI = 3.1415926535f;

float Rad2Deg(float rad)
{
	return rad / (PI / 180.0f);
}

float Deg2Rad(float deg)
{
	return deg * (PI / 180.0f);
}

//uvと深度値とビュープロジェクション逆行列からワールド座標を求める。
float3 GetWorldPosition(float2 uv,float depth,float4x4 mVPI)
{
    float4 projPos = float4(uv*float2(2.0f,-2.0f)+float2(-1.0f,1.0f),depth,1.0f);
    float4 pos = mul(mVPI,projPos);
    return pos.xyz / pos.w;
}

//ビュー空間でのZに変換。
float GetViewZ(float depth,float camFar,float camNear)
{
    return (camFar*camNear)/((camFar-camNear)*depth-camFar);
}