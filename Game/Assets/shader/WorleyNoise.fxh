/*//////////////////////////////////////////////
    ウォーリーノイズ
*///////////////////////////////////////////////

//
float3 WorleyRandom3D(float3 v)
{
    return frac(sin(float3(dot(v,float3(.1,.7,.1)),
                          dot(v,float3(.5,.3,.5)),
                          dot(v,float3(.7,.2,.1))))*43758.5453);
}

//3x3x3なのでちょっと重い。
float WorleyNoise3D(float3 pos)
{
    float3 i_st = floor(pos);
    float3 f_st = frac(pos);

    float m_dist = 10.;  // minimum distance
    float3 m_point;        // minimum point
	
    [unroll(3)]
    for (int j=-1; j<=1; j++ )
    {
        [unroll(3)]
        for (int i=-1; i<=1; i++ )
        {
            [unroll(3)]
            for (int k=-1; k<=1; k++ )
            {
                float3 neighbor = float3(float(k),float(i),float(j));
                float3 p = WorleyRandom3D(i_st + neighbor);
                
                //動かすくねくね。
                //point = 0.5 + 0.5*sin(u_time + 6.2831*point);
                
                float3 diff = neighbor + p - f_st;
                
                float dist = length(diff);

                float isMinDist = dist < m_dist;
                m_dist = dist*isMinDist + m_dist*(1.f-isMinDist);
                m_point = p*isMinDist + m_point*(1.f-isMinDist);
                //上の要約↓
                /*
                if( dist < m_dist ) {
                    m_dist = dist;
                    m_point = p;
                }
                */
            }
        }
    }

    return m_dist;
}