/*//////////////////////////////////////////
    パーリンノイズ
*///////////////////////////////////////////

//パーリン用2Dランダム関数。
float2 PerlinRandom2D(float2 v)
{
    v = float2(dot(v,float2(349.7f,761.3f)),
                dot(v,float2(277.1f,409.5f)));
    return -1.f+2.f*frac(sin(v)*19990.623f);
}

//パーリン用3Dランダム関数。
float3 PerlinRandom3D(float3 v)
{
    v = float3(dot(v,float3(349.7f,761.3f,1061.1f)),
                dot(v,float3(277.1f,409.5f,971.7f)),
                dot(v,float3(101.3f,881.1f,523.3f)));
    return -1.f+2.f*frac(sin(v)*19990.623f);
}

//2Dパーリンノイズ
float PerlinNoise2D(float2 pos)
{
    float2 i = floor(pos);
    float2 f = frac(pos);

    float2 u = f*f*(3.f-2.f*f);

    float mixX1 = lerp(dot(PerlinRandom2D(i+float2(0.f,0.f)),f-float2(0.f,0.f)),
                        dot(PerlinRandom2D(i+float2(1.f,0.f)),f-float2(1.f,0.f)),u.x);
    float mixX2 = lerp(dot(PerlinRandom2D(i+float2(0.f,1.f)),f-float2(0.f,1.f)),
                        dot(PerlinRandom2D(i+float2(1.f,1.f)),f-float2(1.f,1.f)),u.x);
    return lerp(mixX1,mixX2,u.y);
}

//3Dパーリンノイズ
float PerlinNoise3D(float3 pos)
{
    float3 i = floor(pos);
    float3 f = frac(pos);

    float3 u = f*f*(3.f-2.f*f);
    
    float mixX1 = lerp(dot(PerlinRandom3D(i+float3(0.f,0.f,0.f)),f-float3(0.f,0.f,0.f)),
                        dot(PerlinRandom3D(i+float3(1.f,0.f,0.f)),f-float3(1.f,0.f,0.f)),u.x);
    float mixX2 = lerp(dot(PerlinRandom3D(i+float3(0.f,1.f,0.f)),f-float3(0.f,1.f,0.f)),
                        dot(PerlinRandom3D(i+float3(1.f,1.f,0.f)),f-float3(1.f,1.f,0.f)),u.x);

    float mixX3 = lerp(dot(PerlinRandom3D(i+float3(0.f,0.f,1.f)),f-float3(0.f,0.f,1.f)),
                        dot(PerlinRandom3D(i+float3(1.f,0.f,1.f)),f-float3(1.f,0.f,1.f)),u.x);
    float mixX4 = lerp(dot(PerlinRandom3D(i+float3(0.f,1.f,1.f)),f-float3(0.f,1.f,1.f)),
                        dot(PerlinRandom3D(i+float3(1.f,1.f,1.f)),f-float3(1.f,1.f,1.f)),u.x);

    float mixY1 = lerp(mixX1,mixX2,u.y);
    float mixY2 = lerp(mixX3,mixX4,u.y);
    return lerp(mixY1,mixY2,u.z);
}