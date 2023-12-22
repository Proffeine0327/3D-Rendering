#define PI 3.1415926f
#define RADIAN PI / 180 

#include "olcConsoleGameEngine.h"
using namespace std;

struct Matrix4x4
{
	float m[4][4] = { 0 };
};

struct Vector3
{
	float x, y, z;

	Vector3& operator*(const Matrix4x4& rhs)
	{
		Vector3 vec;
		vec.x =
			rhs.m[0][0] * x +
			rhs.m[1][0] * y +
			rhs.m[2][0] * z +
			rhs.m[3][0];

		vec.y = 
			rhs.m[0][1] * x +
			rhs.m[1][1] * y +
			rhs.m[2][1] * z +
			rhs.m[3][1];

		vec.z =
			rhs.m[0][2] * x +
			rhs.m[1][2] * y +
			rhs.m[2][2] * z +
			rhs.m[3][2];

		float w = 
			rhs.m[0][3] * x +
			rhs.m[1][3] * y +
			rhs.m[2][3] * z +
			rhs.m[3][3];

		if (w != 0.0f)
		{
			vec.x /= w;
			vec.y /= w;
			vec.z /= w;
		}

		return vec;
	}
};

struct Triangle
{
	Vector3 points[3];
};

struct Mesh
{
	vector<Triangle> tris;
};

class ConsoleGameEngine : public olcConsoleGameEngine
{
private:
	Mesh meshCube;
	Matrix4x4 mat;
	float fTheta;

public:
	ConsoleGameEngine()
	{

	}

	bool OnUserCreate() override
	{
		meshCube.tris =
		{
			// SOUTH
			{ 0.0f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 0.0f, 0.0f },

			// EAST                                                      
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 0.0f, 1.0f },

			// NORTH                                                     
			{ 1.0f, 0.0f, 1.0f,    1.0f, 1.0f, 1.0f,    0.0f, 1.0f, 1.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 0.0f, 1.0f },

			// WEST                                                      
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 1.0f,    0.0f, 1.0f, 0.0f },
			{ 0.0f, 0.0f, 1.0f,    0.0f, 1.0f, 0.0f,    0.0f, 0.0f, 0.0f },

			// TOP                                                       
			{ 0.0f, 1.0f, 0.0f,    0.0f, 1.0f, 1.0f,    1.0f, 1.0f, 1.0f },
			{ 0.0f, 1.0f, 0.0f,    1.0f, 1.0f, 1.0f,    1.0f, 1.0f, 0.0f },

			// BOTTOM                                                    
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f },
			{ 1.0f, 0.0f, 1.0f,    0.0f, 0.0f, 0.0f,    1.0f, 0.0f, 0.0f },

		};

		float fNear = 0.1f;
		float fFar = 1000;
		float fFov = 120;
		float fAspectRatio = (float)ScreenHeight() / (float)ScreenWidth();
		float fFovTan = 1.0f / tanf(fFov / 2 * RADIAN);

		mat.m[0][0] = fAspectRatio * fFovTan;
		mat.m[1][1] = fFovTan;
		mat.m[2][2] = fFar / (fFar - fNear);
		mat.m[3][2] = (-fFar * fNear) / (fFar - fNear);
		mat.m[2][3] = 1.0f;
		mat.m[3][3] = 0;

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Fill(0, 0, ScreenWidth(), ScreenHeight(), PIXEL_SOLID, FG_BLACK);

		Matrix4x4 rotz, rotx;
		fTheta += fElapsedTime;

		rotx.m[0][0] = 1;
		rotx.m[1][1] = cosf(fTheta);
		rotx.m[1][2] = -sinf(fTheta);
		rotx.m[2][1] = sinf(fTheta);
		rotx.m[2][2] = cosf(fTheta);

		rotz.m[0][0] = cosf(fTheta);
		rotz.m[0][1] = -sinf(fTheta);
		rotz.m[1][0] = sinf(fTheta);
		rotz.m[1][1] = cosf(fTheta);
		rotz.m[2][2] = 1;
		

		for (auto tri : meshCube.tris)
		{
			Triangle display, translate, rotateZ, rotateZX;

			rotateZ.points[0] = tri.points[0] * rotz;
			rotateZ.points[1] = tri.points[1] * rotz;
			rotateZ.points[2] = tri.points[2] * rotz;

			rotateZX.points[0] = rotateZ.points[0] * rotx;
			rotateZX.points[1] = rotateZ.points[1] * rotx;
			rotateZX.points[2] = rotateZ.points[2] * rotx;

			translate = rotateZX;
			translate.points[0].z = rotateZX.points[0].z + 3.0f;
			translate.points[1].z = rotateZX.points[1].z + 3.0f;
			translate.points[2].z = rotateZX.points[2].z + 3.0f;

			display.points[0] = translate.points[0] * mat;
			display.points[1] = translate.points[1] * mat;
			display.points[2] = translate.points[2] * mat;

			display.points[0].x += 1.0f; display.points[0].y += 1.0f;
			display.points[1].x += 1.0f; display.points[1].y += 1.0f;
			display.points[2].x += 1.0f; display.points[2].y += 1.0f;

			display.points[0].x *= 0.5f * ScreenWidth(); display.points[0].y *= 0.5f * ScreenHeight();
			display.points[1].x *= 0.5f * ScreenWidth(); display.points[1].y *= 0.5f * ScreenHeight();
			display.points[2].x *= 0.5f * ScreenWidth(); display.points[2].y *= 0.5f * ScreenHeight();

			DrawTriangle(
				display.points[0].x, display.points[0].y,
				display.points[1].x, display.points[1].y,
				display.points[2].x, display.points[2].y,
				PIXEL_SOLID,
				FG_WHITE
			);
		}

		return true;
	}
};

int main()
{
	ConsoleGameEngine engine;
	if (engine.ConstructConsole(256, 240, 4, 4))
		engine.Start();
	return 0;
}