#include<fstream>
#include<cmath>
#include<string.h>
#include"vector.h"
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
using namespace std;
int w, h, nchannel;

vec2 uv0 = { 0,0 };
vec2 uv1 = { 0,1 };
vec2 uv2 = { 1,1 };
vec2 uv3 = { 1,0 };
vec2 uv4 = { 1,1 };

vec3 v0 = { -48,  -48, 0 };
vec3 v1 = { -48, 48,  0 };
vec3 v2 = { 48, 48,  0 };
vec3 v3 = { 48,  -48, 0 };
vec3 v4 = { 48, -48, 96 };

float edgeFun(const vec3 &a, const vec3 &b, const vec3 &c)
{
	return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]);
}

vec3 value(float u, float v, unsigned char *data)
{
	int i = u * w;
	int j = (1 - v) * h - 0.001f;
	if (i < 0)i = 0;
	if (j < 0)j = 0;
	if (i > w - 1)i = w - 1;
	if (j > h - 1)j = h - 1;
	float r = int(data[3 * i + j * w * 3]) / 255.0;
	float g = int(data[3 * i + j * w * 3 + 1]) / 255.0;
	float b = int(data[3 * i + j * w * 3 + 2]) / 255.0;
	return vec3(r, g, b);
}

tra trangle(vec3 v0, vec3 v1, vec3 v2,vec2 uv0,vec2 uv1,vec2 uv2, int nx, int ny)
{
	//透视裁剪
	v0[0] /= v0[2];
	v0[1] /= v0[2];

	v1[0] /= v1[2];
	v1[1] /= v1[2];

	v2[0] /= v2[2];
	v2[1] /= v2[2];

	v0[0] = (1 + v0[0])*0.5*nx;	v0[1] = (1 + v0[1])*0.5*ny;
	v1[0] = (1 + v1[0])*0.5*nx;	v1[1] = (1 + v1[1])*0.5*ny;
	v2[0] = (1 + v2[0])*0.5*nx;	v2[1] = (1 + v2[1])*0.5*ny;

	v0[2] = 1 /v0[2]; v1[2] = 1 / v1[2]; v2[2] = 1 / v2[2];

	return tra(v0, v1, v2, uv0, uv1, uv2);
}
vec4 Color(vec3 &p, tra &tr, float area,unsigned char *data)
{
	
	float r = 0, g = 0, b = 0, z = 0, zz = 0;
	float w0 = edgeFun(tr.v1, tr.v2, p);	
	float w1 = edgeFun(tr.v2, tr.v0, p);
	float w2 = edgeFun(tr.v0, tr.v1, p);
	if (w0 >= 0 && w1 >= 0 && w2 >= 0) {

		w0 /= area;
		w1 /= area;
		w2 /= area;

		float xxx = w0 * tr.uv0[0]* tr.v0.z + w1 * tr.uv1[0] * tr.v1.z + w2 * tr.uv2[0] * tr.v2.z;
		float yyy = w0 * tr.uv0[1] * tr.v0.z + w1 * tr.uv1[1] * tr.v1.z + w2 * tr.uv2[1] * tr.v2.z;

		z = 1 / (w0*tr.v0[2] + w1 * tr.v1[2] + w2 * tr.v2[2]);

		xxx *= z; yyy *= z;

		r = ((int)data[3 * int(xxx*w) + 3 * int(yyy*h)*w]) / 255.0;
		g = ((int)data[3 * int(xxx*w) + 3 * int(yyy*h)*w+1])/255.0;
		b = ((int)data[3 * int(xxx*w) + 3 * int(yyy*h)*w+2])/255.0;
	}
	else
	{		
		r = 0; g = 0; b = 0;
	}
	return vec4(r, g, b, z);
}
int main()
{
	vec3 lightpos = { 0,0,100 };
	vec3 viewpos = { 0,0,0 };
	vec3 lightcolor = { 1,1,1 };

	unsigned const int nx = 512, ny = 512;

	v0 = translate(rotate(v0, vec3(0, 45, 0)), vec3(0, 0, 150));
	v1 = translate(rotate(v1, vec3(0, 45, 0)), vec3(0, 0, 150));
	v2 = translate(rotate(v2, vec3(0, 45, 0)), vec3(0, 0, 150));
	v3 = translate(rotate(v3, vec3(0, 45, 0)), vec3(0, 0, 150));
	v4 = translate(rotate(v4, vec3(0, 45, 0)), vec3(0, 0, 150));	

	vec3 normal;

	tra tr1 = trangle(v3, v0, v2, uv3,uv0,uv2,nx, ny);
	float area1 = edgeFun(tr1.v0, tr1.v1, tr1.v2);

	tra tr2 = trangle(v0, v1, v2,uv0,uv1,uv2, nx, ny);
	float area2 = edgeFun(tr2.v0, tr2.v1, tr2.v2);

	tra tr3 = trangle(v3, v0, v4,uv3,uv0,uv4, nx, ny);
	float area3 = edgeFun(tr3.v0, tr3.v1, tr3.v2);

	float zBuffer[nx][ny] ;
	for (int i = 0; i < nx; i++)
	{
		for (int j = 0; j < ny; j++)
		{
			zBuffer[i][j] = FLT_MAX;
		}
	}

	ofstream outfile("光栅化.ppm");
	outfile << "P3" << "\n" << nx << " " << ny << "\n" << "255" << "\n";

	ofstream zfile("zbuffer.ppm");
	zfile << "P3" << "\n" << nx << " " << ny << "\n" << "255" << "\n";

	unsigned char *data = stbi_load("container.jpg", &w, &h, &nchannel, 0);

	for (int j = ny-1; j>=0; j--)
	{
		for (int i = 0; i < nx; i++)
		{
			float rr=0, gg=0, bb=0,z=0;

			float xx, yy, zz;
			vec3 p = { (float)i , (float)j  ,0 };

			vec4 pix1 = Color(p, tr2, area2, data);
			vec4 pix2 = Color(p, tr1, area1, data);
			vec4 pix3 = Color(p, tr3, area3, data);

			if (pix1.w < zBuffer[i][j] && pix1.w)
			{
				rr = pix1.x;
				gg = pix1.y;
				bb = pix1.z;
				zBuffer[i][j] = pix1.w;

				normal = cross(v0 - v1, v0 - v2);

				zz = pix1.w;
				xx = ((2 * p.x) / nx - 1)*zz;
				yy = ((2 * p.y) / ny - 1)*zz;
			}
			if (pix2.w < zBuffer[i][j] && pix2.w)
			{
				rr = pix2.x;
				gg = pix2.y;
				bb = pix2.z;
				zBuffer[i][j] = pix2.w;

				vec3 normal = cross(v3 - v0, v3 - v2);
				zz = pix2.w;
				xx = ((2 * p.x) / nx - 1)*zz;
				yy = ((2 * p.y) / ny - 1)*zz;
			}
			if (pix3.w < zBuffer[i][j] && pix3.w)
			{
				rr = pix3.x;
				gg = pix3.y;
				bb = pix3.z;
				zBuffer[i][j] = pix3.w;

				vec3 normal = vec3(0, 1, 0);
				zz = pix3.w;
				xx = ((2 * p.x) / nx - 1)*zz;
				yy = ((2 * p.y) / ny - 1)*zz;
			}

			if (zBuffer[i][j] == FLT_MAX)
			{
				zBuffer[i][j] = 1;
			}
			else
			{
				z = zBuffer[i][j];

				vec3 fragpos = vec3(xx, yy, zz);
				vec3 lightdir = lightpos - fragpos;
				vec3 viewdir = viewpos - fragpos;

				normal = normalize(normal);				
				lightdir = normalize(lightdir);				
				viewdir = normalize(viewdir);

				float diff = max(dot(lightdir, normal), 0);

				vec3 rec = reflect(lightdir, normal);
				rec = normalize(rec);

				float spec = pow(max(dot(rec, viewdir), 0), 16);

				vec3 result = lightcolor * (0.1 + spec + diff);

				vec3 col = result * vec3(rr, gg, bb);

				rr = col.x;
				gg = col.y;
				bb = col.z;
			}

			rr = rr > 1 ? 1 : rr;
			gg = gg > 1 ? 1 : gg;
			bb = bb > 1 ? 1 : bb;

			outfile << " " << int(rr*255.99) << " " << int(gg*255.99) << " " << int(bb*255.99);
			zfile << " " << int(z *255.99) << " " << int(z*255.99) << " " << int(z*255.99);
		}
		outfile << "\n";
	}
}
