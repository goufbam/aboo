#include "Math.h"
#include <corecrt_math.h>
#define M_PI        3.14159265358979323846264338327950288  
float Width = 2560;//��Ļ��
float Height = 1440;//��Ļ��
float CenterX = Width / 2;
float CenterY = Height / 2;


Vector3 ����_3D������(Vector3 ��_��������A, Vector3 ��_��������B)
{
	Vector3 ��_����ֵ;
	��_����ֵ.x = ��_��������A.x - ��_��������B.x;
	��_����ֵ.y = ��_��������A.y - ��_��������B.y;
	��_����ֵ.z = ��_��������A.z - ��_��������B.z;
	return ��_����ֵ;
}

float ��������3D��(Vector3 ��_��������A, Vector3 ��_��������B)
{
	float ��_����ֵ = ��_��������A.x * ��_��������B.x + ��_��������A.y * ��_��������B.y + ��_��������A.z * ��_��������B.z;
	return ��_����ֵ;
}


void ��������3D��(Vector3 ��_��������A, Vector3& ��_��������B_X, Vector3& ��_��������B_Y, Vector3& ��_��������B_Z)
{

	float ��_�����ǻ��� = ��_��������A.x * M_PI / 180;
	float ��_ƫ���ǻ��� = ��_��������A.y * M_PI / 180;
	float ��_�����ǻ��� = ��_��������A.z * M_PI / 180;
	float ��_�������SP = sin(��_�����ǻ���);
	float ��_�������CP = cos(��_�����ǻ���);
	float ��_�������SY = sin(��_ƫ���ǻ���);
	float ��_�������CY = cos(��_ƫ���ǻ���);
	float ��_�������SR = sin(��_�����ǻ���);
	float ��_�������CR = cos(��_�����ǻ���);

	��_��������B_X.x = ��_�������CP * ��_�������CY;
	��_��������B_X.y = ��_�������CP * ��_�������SY;
	��_��������B_X.z = ��_�������SP;

	��_��������B_Y.x = ��_�������SR * ��_�������SP * ��_�������CY - ��_�������CR * ��_�������SY;
	��_��������B_Y.y = ��_�������SR * ��_�������SP * ��_�������SY + ��_�������CR * ��_�������CY;
	��_��������B_Y.z = -��_�������SR * ��_�������CP;

	��_��������B_Z.x = -(��_�������CR * ��_�������SP * ��_�������CY + ��_�������SR * ��_�������SY);
	��_��������B_Z.y = ��_�������CY * ��_�������SR - ��_�������CR * ��_�������SP * ��_�������SY;
	��_��������B_Z.z = ��_�������CR * ��_�������CP;
}

bool ����_����ת��(Vector3 ��_��������, Vector2& ��_��Ļ����,�����Ϣ ���)
{
	Vector3 ��_ת������;
	Vector3 ��_������� = ����_3D������(��_��������, ���.�������);
	��_ת������.x = ��������3D��(��_�������, ���.ת������Y);
	��_ת������.y = ��������3D��(��_�������, ���.ת������Z);
	��_ת������.z = ��������3D��(��_�������, ���.ת������X);
	int ��Ϸ����X = Width / 2;
	int ��Ϸ����Y = Height / 2;
	int	��_���� = ��_ת������.z / 100;
	if (��_ת������.z >= 1)
	{
		float ��_�ӽǽǶ� = ���.fov;
		��_��Ļ����.x = ��Ϸ����X + ��_ת������.x * ��Ϸ����X / tan(��_�ӽǽǶ� * M_PI / 360) / ��_ת������.z;
		��_��Ļ����.y = ��Ϸ����Y - ��_ת������.y * ��Ϸ����X / tan(��_�ӽǽǶ� * M_PI / 360) / ��_ת������.z;
		if (��_��Ļ����.x > 0 && ��_��Ļ����.y > 0)
		{
			if (��_��Ļ����.x <= Width && ��_��Ļ����.y <= Height)
			{
				return true;
			}
			return false;
		}

	}
	return false;

}

float GetDistance(Vector3 My, Vector3 Taget, int div)
{
	Vector3 ���;
	���.x = My.x - Taget.x;
	���.y = My.y - Taget.y;
	���.z = My.z - Taget.z;
	float  ��_����ֵ = sqrt(���.x * ���.x + ���.y * ���.y + ���.z * ���.z) / div;
	return ��_����ֵ;

}

D3DMATRIX D3DMatrixTransformation(Vector4 Rotation, Vector3 Translation, Vector3 Scale3D)
{
	D3DMATRIX m;
	m._41 = Translation.x;
	m._42 = Translation.y;
	m._43 = Translation.z;
	float x2 = Rotation.x + Rotation.x;
	float y2 = Rotation.y + Rotation.y;
	float z2 = Rotation.z + Rotation.z;

	float xx2 = Rotation.x * x2;
	float yy2 = Rotation.y * y2;
	float zz2 = Rotation.z * z2;
	m._11 = (1 -(yy2 + zz2)) * Scale3D.x;
	m._22 = (1 -(xx2 + zz2)) * Scale3D.y;
	m._33 = (1 -(xx2 + yy2)) * Scale3D.z;

	float yz2 = Rotation.y * z2;
	float	wx2 = Rotation.w * x2;
	m._32 = (yz2 - wx2) * Scale3D.z;
	m._23 = (yz2 + wx2) * Scale3D.y;

	float	xy2 = Rotation.x * y2;
	float wz2 = Rotation.w * z2;
	m._21 = (xy2 - wz2) * Scale3D.y;
	m._12 = (xy2 + wz2) * Scale3D.x;

	float xz2 = Rotation.x * z2;
	float wy2 = Rotation.w * y2;
	m._31 = (xz2 + wy2) * Scale3D.z;
	m._13 = (xz2 - wy2) * Scale3D.x;

	m._14 = 0;
	m._24 = 0;
	m._34 = 0;
	m._44 = 1;
	return m;
}


D3DMATRIX MatrixMultiply(D3DMATRIX pM1, D3DMATRIX pM2)
{
	D3DMATRIX pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;
	return pOut;
}

float ȡ_׼�Ǿ���(Vector2 ׼��A, Vector2 ׼��B)
{
	Vector2 ����;
	����.x = ׼��A.x - ׼��B.x;
	����.y = ׼��A.y - ׼��B.y;
	return sqrt(����.x * ����.x + ����.y * ����.y);
}

