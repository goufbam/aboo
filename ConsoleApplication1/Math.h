#pragma once

struct Vector4
{
	float x;
	float y;
	float z;
	float w;
};


struct 屏幕范围
{
	float x;
	float y;
	float h;
	float w;
};

struct Vector3
{
	float x;
	float y;
	float z;
};


struct Vector2
{
	float x;
	float y;
};

struct FTransform
{
	Vector4 Rotation;
	Vector3 Translation;
	Vector3 Scale3D;
};

typedef struct _D3DMATRIX {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		};
		float m[4][4];
	};
} D3DMATRIX;

struct 相机信息 {
	Vector3 相机坐标;
	Vector3 视角坐标;
	Vector3 转换坐标X;
	Vector3 转换坐标Y;
	Vector3 转换坐标Z;
	float fov;
};






Vector3 运算_3D差坐标(Vector3 参_运算坐标A, Vector3 参_运算坐标B);
float 运算坐标3D积(Vector3 参_运算坐标A, Vector3 参_运算坐标B);
void 运算坐标3D轴(Vector3 参_运算坐标A, Vector3& 参_运算坐标B_Y, Vector3& 参_运算坐标B_X, Vector3& 参_运算坐标B_Z);
bool 运算_矩阵转换(Vector3 参_对象坐标, Vector2& 参_屏幕坐标, 相机信息 相机);
bool w2c(float viewworld[4][4], Vector3 对象坐标, Vector2& 参_屏幕坐标);
float GetDistance(Vector3 My, Vector3 Taget, int div);
D3DMATRIX D3DMatrixTransformation(Vector4 Rotation, Vector3 Translation, Vector3 Scale3D);
D3DMATRIX MatrixMultiply(D3DMATRIX pM1, D3DMATRIX pM2);
float 取_准星距离(Vector2 准星A, Vector2 准星B);
