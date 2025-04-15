#pragma once

struct Vector4
{
	float x;
	float y;
	float z;
	float w;
};


struct ��Ļ��Χ
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

struct �����Ϣ {
	Vector3 �������;
	Vector3 �ӽ�����;
	Vector3 ת������X;
	Vector3 ת������Y;
	Vector3 ת������Z;
	float fov;
};






Vector3 ����_3D������(Vector3 ��_��������A, Vector3 ��_��������B);
float ��������3D��(Vector3 ��_��������A, Vector3 ��_��������B);
void ��������3D��(Vector3 ��_��������A, Vector3& ��_��������B_Y, Vector3& ��_��������B_X, Vector3& ��_��������B_Z);
bool ����_����ת��(Vector3 ��_��������, Vector2& ��_��Ļ����, �����Ϣ ���);
bool w2c(float viewworld[4][4], Vector3 ��������, Vector2& ��_��Ļ����);
float GetDistance(Vector3 My, Vector3 Taget, int div);
D3DMATRIX D3DMatrixTransformation(Vector4 Rotation, Vector3 Translation, Vector3 Scale3D);
D3DMATRIX MatrixMultiply(D3DMATRIX pM1, D3DMATRIX pM2);
float ȡ_׼�Ǿ���(Vector2 ׼��A, Vector2 ׼��B);
