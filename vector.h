#pragma once
#include <iostream>
#include<math.h>
using namespace std;

struct vec2
{
	float x, y;
	vec2(){}
	vec2(float x, float y) :x(x), y(y) {}
	float operator[](int i)const
	{
		if (i == 0)
			return x;
		else if (i == 1)
			return y;
	}
	float &operator[](int i)
	{
		if (i == 0)
			return x;
		else if (i == 1)
			return y;
	}
};
struct vec3
{
	float x, y, z;
	vec3(){}
	vec3(float x, float y, float z) :x(x), y(y), z(z) {}
	float operator[](int i)const
	{
		if (i == 0)
			return x;
		else if (i == 1)
			return y;
		else if (i == 2)
			return z;
	}
	float &operator[](int i)
	{
		if (i == 0)
			return x;
		else if (i == 1)
			return y;
		else if (i == 2)
			return z;
	}
	vec3 operator*(vec3 temp)
	{
		return vec3(x*temp.x, y*temp.y, z*temp.z);
	}
	vec3 operator*(float temp)
	{
		return vec3(x*temp, y*temp, z*temp);
	}
	vec3 operator-(vec3 temp)
	{
		return vec3(x-temp.x, y-temp.y, z-temp.z);
	}
	vec3 operator+(vec3 temp)
	{
		return vec3(temp.x + x, temp.y + y, temp.z + z);
	}
	vec3 operator/(float a)
	{
		return vec3(x / a, y / a, z / a);
	}
};
float length(vec3 a)
{
	return sqrt(a.x*a.x + a.y*a.y + a.z*a.z);
}
vec3 normalize(vec3 a)
{
	float len = length(a);
	return a/len;
}
vec3 cross(vec3 a, vec3 b)
{
	return vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}
float dot(vec3 a, vec3 b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}
vec3 reflect(vec3 a, vec3 n)
{
	n = normalize(n);
	vec3 temp = a-n*dot(n, a) / length(n);
	return  temp*(-2) + a;
}

float max(float a, float b)
{
	return a > b ? a : b;
}
struct  vec4
{
	float x, y, z, w;
	vec4(float x, float y, float z, float w) :x(x), y(y), z(z), w(w) {}
	float operator[](int i)const
	{
		if (i == 0)
			return x;
		else if (i == 1)
			return y;
		else if (i == 2)
			return z;
		else if (i == 3)
			return w;
	}
	float &operator[](int i)
	{
		if (i == 0)
			return x;
		else if (i == 1)
			return y;
		else if (i == 2)
			return z;
		else if (i == 3)
			return w;
	}
};

struct  tra
{
	vec3 v0, v1, v2;
	vec2 uv0, uv1,uv2;
	tra(vec3 v0, vec3 v1, vec3 v2, vec2 uv0,vec2 uv1, vec2 uv2) :v0(v0), v1(v1), v2(v2),uv0(uv0),uv1(uv1),uv2(uv2){}

};

vec3 rotate_z(vec3 vec,float theta)
{
	float xx, yy;
	theta = (theta / 180)*3.1415926535;
	xx = vec.x*cos(theta) - vec.y*sin(theta);
	yy=  vec.x*sin(theta) + vec.y*cos(theta);
	return vec3(xx, yy, vec.z);
}
vec3 rotate_y(vec3 vec, float theta)
{
	float xx, zz;
	theta = (theta / 180)*3.1415926535;
	xx = vec.x*cos(theta) + vec.z*sin(theta);
	zz = -vec.x*sin(theta) + vec.z*cos(theta);
	return vec3(xx, vec.y, zz);
}
vec3 rotate_x(vec3 vec, float theta)
{
	float yy, zz;
	theta = (theta / 180)*3.1415926535;
	yy = vec.y*cos(theta) - vec.z*sin(theta);
	zz = vec.y*sin(theta) + vec.z*cos(theta);
	return vec3(vec.x, yy, zz);
}
vec3 rotate(vec3 vec, vec3 rot)
{
	return rotate_z(rotate_y(rotate_x(vec, rot.x),rot.y),rot.z);
}
vec3 translate(vec3 vec, vec3 offset)
{
	return vec + offset;
}
