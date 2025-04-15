#include "Math.h"
#include <corecrt_math.h>
#define M_PI        3.14159265358979323846264338327950288  
float Width = 2560;//屏幕宽
float Height = 1440;//屏幕高
float CenterX = Width / 2;
float CenterY = Height / 2;


Vector3 运算_3D差坐标(Vector3 参_运算坐标A, Vector3 参_运算坐标B)
{
	Vector3 局_返回值;
	局_返回值.x = 参_运算坐标A.x - 参_运算坐标B.x;
	局_返回值.y = 参_运算坐标A.y - 参_运算坐标B.y;
	局_返回值.z = 参_运算坐标A.z - 参_运算坐标B.z;
	return 局_返回值;
}

float 运算坐标3D积(Vector3 参_运算坐标A, Vector3 参_运算坐标B)
{
	float 局_返回值 = 参_运算坐标A.x * 参_运算坐标B.x + 参_运算坐标A.y * 参_运算坐标B.y + 参_运算坐标A.z * 参_运算坐标B.z;
	return 局_返回值;
}


void 运算坐标3D轴(Vector3 参_运算坐标A, Vector3& 参_运算坐标B_X, Vector3& 参_运算坐标B_Y, Vector3& 参_运算坐标B_Z)
{

	float 局_俯仰角弧度 = 参_运算坐标A.x * M_PI / 180;
	float 局_偏航角弧度 = 参_运算坐标A.y * M_PI / 180;
	float 局_翻滚角弧度 = 参_运算坐标A.z * M_PI / 180;
	float 局_运算变量SP = sin(局_俯仰角弧度);
	float 局_运算变量CP = cos(局_俯仰角弧度);
	float 局_运算变量SY = sin(局_偏航角弧度);
	float 局_运算变量CY = cos(局_偏航角弧度);
	float 局_运算变量SR = sin(局_翻滚角弧度);
	float 局_运算变量CR = cos(局_翻滚角弧度);

	参_运算坐标B_X.x = 局_运算变量CP * 局_运算变量CY;
	参_运算坐标B_X.y = 局_运算变量CP * 局_运算变量SY;
	参_运算坐标B_X.z = 局_运算变量SP;

	参_运算坐标B_Y.x = 局_运算变量SR * 局_运算变量SP * 局_运算变量CY - 局_运算变量CR * 局_运算变量SY;
	参_运算坐标B_Y.y = 局_运算变量SR * 局_运算变量SP * 局_运算变量SY + 局_运算变量CR * 局_运算变量CY;
	参_运算坐标B_Y.z = -局_运算变量SR * 局_运算变量CP;

	参_运算坐标B_Z.x = -(局_运算变量CR * 局_运算变量SP * 局_运算变量CY + 局_运算变量SR * 局_运算变量SY);
	参_运算坐标B_Z.y = 局_运算变量CY * 局_运算变量SR - 局_运算变量CR * 局_运算变量SP * 局_运算变量SY;
	参_运算坐标B_Z.z = 局_运算变量CR * 局_运算变量CP;
}

bool 运算_矩阵转换(Vector3 参_对象坐标, Vector2& 参_屏幕坐标,相机信息 相机)
{
	Vector3 局_转换坐标;
	Vector3 局_相机坐标 = 运算_3D差坐标(参_对象坐标, 相机.相机坐标);
	局_转换坐标.x = 运算坐标3D积(局_相机坐标, 相机.转换坐标Y);
	局_转换坐标.y = 运算坐标3D积(局_相机坐标, 相机.转换坐标Z);
	局_转换坐标.z = 运算坐标3D积(局_相机坐标, 相机.转换坐标X);
	int 游戏中心X = Width / 2;
	int 游戏中心Y = Height / 2;
	int	参_距离 = 局_转换坐标.z / 100;
	if (局_转换坐标.z >= 1)
	{
		float 局_视角角度 = 相机.fov;
		参_屏幕坐标.x = 游戏中心X + 局_转换坐标.x * 游戏中心X / tan(局_视角角度 * M_PI / 360) / 局_转换坐标.z;
		参_屏幕坐标.y = 游戏中心Y - 局_转换坐标.y * 游戏中心X / tan(局_视角角度 * M_PI / 360) / 局_转换坐标.z;
		if (参_屏幕坐标.x > 0 && 参_屏幕坐标.y > 0)
		{
			if (参_屏幕坐标.x <= Width && 参_屏幕坐标.y <= Height)
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
	Vector3 差距;
	差距.x = My.x - Taget.x;
	差距.y = My.y - Taget.y;
	差距.z = My.z - Taget.z;
	float  局_返回值 = sqrt(差距.x * 差距.x + 差距.y * 差距.y + 差距.z * 差距.z) / div;
	return 局_返回值;

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

float 取_准星距离(Vector2 准星A, Vector2 准星B)
{
	Vector2 距离;
	距离.x = 准星A.x - 准星B.x;
	距离.y = 准星A.y - 准星B.y;
	return sqrt(距离.x * 距离.x + 距离.y * 距离.y);
}

