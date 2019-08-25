static const float PI = 3.1415926535f;

float Rad2Deg(float rad)
{
	return rad / (PI / 180.0f);
}

float Deg2Rad(float deg)
{
	return deg * (PI / 180.0f);
}