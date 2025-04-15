#include <iostream>
#include "Drv.h"
#include <winsvc.h>
#include <fileapi.h>
#include "offset.h"
#include <chrono>
#include "Math.h"

#pragma comment(lib,"winmm.lib")

float MatrixData[4][4]{};
driver_manager* drv;
ULONG64 模块地址;
static void fname_decrypt(char* inputBuf, int namelength)
{
    const char xor_key = drv->RPM<BYTE>(模块地址 + 0xA1A7C08);//32 15 ? ? ? ? 0F B6 CA
    __int64 v3; // r9
    char v4; // cl
    char result; // al

    if (namelength)
    {
        v3 = namelength;
        do
        {
            ++inputBuf;
            v4 = xor_key ^ ((unsigned __int8)xor_key >> 1) & 8 ^ (2
                * ((xor_key ^ ((unsigned __int8)xor_key >> 1) & 8) & 8));
            result = v4 ^ *(BYTE*)(inputBuf - 1) ^ ((unsigned __int8)(v4 & 0x10 ^ 0xE) >> 1);
            *(BYTE*)(inputBuf - 1) = result;
            --v3;
        } while (v3);
    }
}

std::string GetNameFromFName(int key)
{
	auto chunkOffset = (UINT)((int)(key) >> 16);
	auto nameOffset = (USHORT)key;

	auto namePoolChunk = drv->RPM<ULONG64>(模块地址 + offset_GNames + ((chunkOffset + 2) * 8));
	auto entryOffset = namePoolChunk + (ULONG)(2 * nameOffset);
	auto nameEntry = drv->RPM<INT16>(entryOffset);

	auto nameLength = nameEntry >> 6;
	char buff[1028]{};
	if ((DWORD)nameLength && nameLength > 0)
	{
		drv->ReadChar(entryOffset + 2, buff, nameLength);
		buff[nameLength] = '\0';
		fname_decrypt(buff, nameLength);	
		return std::string(buff);
	}
	else return "";
}

std::string GetExplanationName(std::string ObjectClassName)
{

	if (ObjectClassName.find("BP_Weapon_AKM_Uam_C") != std::string::npos)
	{
		return std::string("AKM");
		
	}
	else if (ObjectClassName.find("BP_Weapon_M9_Bayonet_Uam_C") != std::string::npos)
	{
		return std::string("M9_Knife");
	
	}
	else if (ObjectClassName.find("BP_UamAICharacter_C") != std::string::npos)
	{
		return std::string("Bot");
	
	}
	else if (ObjectClassName.find("BP_UamRangeCharacter_C") != std::string::npos)
	{
		return std::string("Player");
	}
	else if (ObjectClassName.find("BP_UamCharacter_C") != std::string::npos)
	{
		return std::string("Player");
	}
	return ObjectClassName;
}


FTransform GetPos(ULONG64 Ptr)
{
	FTransform Result = drv->RPM<FTransform>(Ptr);
	return Result;
}

void GetBonePos(ULONG64 pObj, Vector3& Result, int BoneIndex)//传入对象的Mesh
{
	ULONG64 Bone_Array = drv->RPM<ULONG64>(pObj + offset_BoneArrayOne);
	FTransform Bone = GetPos(Bone_Array + BoneIndex * 48);
	ULONG64 ComponentToWorld = drv->RPM<ULONG64>(pObj + offset_ComponentToWorld);
	FTransform Actor = GetPos(ComponentToWorld);
	//FTransform Actor = GetPos(pObj + offset_ComponentToWorld);
	D3DMATRIX BoneMatrix = D3DMatrixTransformation(Bone.Rotation, Bone.Translation, Bone.Scale3D);
	D3DMATRIX ComponentToWorldMatrix = D3DMatrixTransformation(Actor.Rotation, Actor.Translation, Actor.Scale3D);
	D3DMATRIX NewMatrix = MatrixMultiply(BoneMatrix, ComponentToWorldMatrix);
	Result.x = NewMatrix._41;
	Result.y = NewMatrix._42;
	Result.z = NewMatrix._43;
}

void 计算方框大小(ULONG64 Mesh,ImVec4& 返回屏幕, 相机信息 相机)
{
	Vector2 相对坐标;
	Vector2 坐标位置;
	Vector3	敌人头;
	Vector3	敌人脚;
	GetBonePos(Mesh, 敌人头, 16);
	敌人头.z = 敌人头.z + 25;
	GetBonePos(Mesh, 敌人脚, 0);
	敌人脚.z = 敌人脚.z - 13;
	运算_矩阵转换(敌人头, 相对坐标, 相机);
	运算_矩阵转换(敌人脚, 坐标位置, 相机);
	
		返回屏幕.z = 坐标位置.y - 相对坐标.y;
		返回屏幕.x = 相对坐标.x - 返回屏幕.z / 4;
		返回屏幕.y = 相对坐标.y;
		返回屏幕.w = 返回屏幕.z / 2;
	
}

bool isVisible(ULONG64 mesh)//Error occurs when the distance is below 60    But this is the best external choice
{
	float fLastSubmitTime = drv->RPM<float>(mesh + offset_last_submit_time);
	float fLastRenderTimeOnScreen = drv->RPM<float>(mesh + offset_last_render_time);
	const float fVisionTick = 0.06f;
	bool bVisible = fLastRenderTimeOnScreen + fVisionTick >= fLastSubmitTime;
	return bVisible;
}
 
void move_to(float x, float y)
{
	float center_x = 2560 / 2;
	float center_y = 1440 / 2;

	float smooth = 3.f;

	float target_x = 0.f;
	float target_y = 0.f;

	if (x != 0.f)
	{
		if (x > center_x)
		{
			target_x = -(center_x - x);
			target_x /= smooth;
			if (target_x + center_x > center_x * 2.f) target_x = 0.f;
		}

		if (x < center_x)
		{
			target_x = x - center_x;
			target_x /= smooth;
			if (target_x + center_x < 0.f) target_x = 0.f;
		}
	}

	if (y != 0.f)
	{
		if (y > center_y)
		{
			target_y = -(center_y - y);
			target_y /= smooth;
			if (target_y + center_y > center_y * 2.f) target_y = 0.f;
		}

		if (y < center_y)
		{
			target_y = y - center_y;
			target_y /= smooth;
			if (target_y + center_y < 0.f) target_y = 0.f;
		}
	}

	mouse_event(MOUSEEVENTF_MOVE, static_cast<DWORD>(target_x), static_cast<DWORD>(target_y), 0, 0);
}

void Menu()
{
	static bool IsMenuInit = false;
	if (!IsMenuInit)
	{
		ImGui::GetStyle().Colors[ImGuiCol_WindowBg].w = 0.75;
		IsMenuInit = true;
	}

	ImGui::Begin("Arena Breakout Infinite", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse);
	{
		ImGui::BeginTabBar("ChumChum");
		// esp menu
		if (ImGui::BeginTabItem("ESP"))
		{
			Gui.MyCheckBox("BoxESP", &MenuConfig::ShowBoxESP);
			ImGui::SameLine();
			ImGui::ColorEdit4("##BoxColor", reinterpret_cast<float*>(&MenuConfig::BoxColor), ImGuiColorEditFlags_NoInputs);

			Gui.MyCheckBox("FovLine", &MenuConfig::ShowFovLine);
			ImGui::SameLine();
			ImGui::ColorEdit4("##EyeRay", reinterpret_cast<float*>(&MenuConfig::EyeRayColor), ImGuiColorEditFlags_NoInputs);

			Gui.MyCheckBox("LineToEnemy", &MenuConfig::ShowLineToEnemy);
			ImGui::SameLine();
			ImGui::ColorEdit4("##LineToEnemyColor", reinterpret_cast<float*>(&MenuConfig::LineToEnemyColor), ImGuiColorEditFlags_NoInputs);


			ImGui::EndTabItem();
		}



		ImGui::Text("Show/Hide----InSert");

		ImGui::EndTabBar();
	}ImGui::End();
}



typedef struct _EntityList
{
	uintptr_t actor_pawn;
	uintptr_t actor_mesh;
	uintptr_t actor_state;
	std::string acotr_name;
	int actor_id;
}EntityList;

std::vector<EntityList> entityList;

void loopData()
{
	模块地址 = drv->GeModule();
	
	while (true)
	{
		std::vector<EntityList> tmpList;

		GWorld = drv->RPM<ULONG64>(模块地址 + offset_GWorld);
		ULevel = drv->RPM<ULONG64>(GWorld + offset_PersistentLevel);
		OwningGameInstance = drv->RPM<ULONG64>(GWorld + offset_OwningGameInstance);
		LocalPlayers = drv->RPM<ULONG64>(OwningGameInstance + offset_LocalPlayers);
		LocalPlayersData = drv->RPM<ULONG64>(LocalPlayers);
		PlayerController = drv->RPM<ULONG64>(LocalPlayersData + offset_PlayerController);
		AcknowledgedPawn = drv->RPM<ULONG64>(PlayerController + offset_AcknowledgedPawn);//本人地址
		PlayerCameraManager = drv->RPM<ULONG64>(PlayerController + offset_CameraManager);
		CameraCache = PlayerCameraManager + offset_CameraCacheOffset;
		RootComponent = drv->RPM<ULONG64>(AcknowledgedPawn + offset_RootComponent);
		本人坐标 = drv->RPM<Vector3>(RootComponent + offset_RelativeLocation);
		本人状态 = drv->RPM<ULONG64>(AcknowledgedPawn + offset_PlayerState);
		本人队伍 = drv->RPM<ULONG32>(本人状态 + offset_TeamId);
		ULONG64 ActorsArray = drv->RPM<ULONG64>(ULevel + offset_ActorsOffset);
		ULONG32 ActorSize = drv->RPM<ULONG32>(ULevel + offset_ActorSize);

		for (int i = 0; i < ActorSize; i++)
		{
			ULONG64 对象数组 = drv->RPM<ULONG64>(ActorsArray + i * 8);
			if (对象数组 == AcknowledgedPawn) { continue; }
			ULONG32 对象ID = drv->RPM<ULONG32>(对象数组 + 24);
			auto 对象名称 = GetNameFromFName(对象ID);

			

			if (对象名称.find("BP_UamRangeCharacter_C") != std::string::npos ||
				对象名称.find("BP_UamAICharacter_C") != std::string::npos || 
				对象名称.find("Character") != std::string::npos)
			{
				ULONG64 敌人Mesh = drv->RPM<ULONG64>(对象数组 + offset_Mesh);
				ULONG64 敌人状态 = drv->RPM<ULONG64>(对象数组 + offset_PlayerState);

				EntityList Entity{ };
				Entity.actor_pawn = 对象数组;
				Entity.actor_id = 对象ID;
				Entity.actor_state = 敌人状态;
				Entity.actor_mesh = 敌人Mesh;
				Entity.acotr_name = 对象名称;
				tmpList.push_back(Entity);
			}



		}
		entityList = tmpList;
		Sleep(200);
		//_Thrd_sleep_for(1000);


	}

}


void Run()
{
	static std::chrono::time_point LastTimePoint = std::chrono::steady_clock::now();
	auto CurTimePoint = std::chrono::steady_clock::now();
	if (GetAsyncKeyState(VK_INSERT)
		&& CurTimePoint - LastTimePoint >= std::chrono::milliseconds(200))
	{
		MenuConfig::ShowMenu = !MenuConfig::ShowMenu;
		LastTimePoint = CurTimePoint;
	}

	if (MenuConfig::ShowMenu)
		Menu();
	
	if (MenuConfig::ShowFovLine)
	{
		Gui.Circle({ 1280,720 }, 自瞄范围, MenuConfig::BoxColor, 1);
	}

	ULONG64 ActorsArray = drv->RPM<ULONG64>(ULevel + offset_ActorsOffset);
	ULONG32 ActorSize = drv->RPM<ULONG32>(ULevel + offset_ActorSize);
	
	ULONG64 WeaponManagerComponent = drv->RPM<ULONG64>(AcknowledgedPawn + offset_WeaponManagerComponent);
	ULONG64 CurrentWeapon = drv->RPM<ULONG64>(WeaponManagerComponent + offset_CurrentWeapon);
	ULONG64 WeaponZoomComponent = drv->RPM<ULONG64>(CurrentWeapon + offset_WeaponZoomComponent);
	//float ZoomProgressRateCheck = drv->RPM<float>(WeaponZoomComponent + offset_ZoomProgressRateCheck);//1是开镜 0是没开
	float ZoomProgressRate = drv->RPM<float>(WeaponZoomComponent + offset_ZoomProgressRate);//倍镜的倍数

	ULONG32 WeaponID = drv->RPM<ULONG32>(CurrentWeapon + 24);
	auto Weaponname = GetNameFromFName(WeaponID);



	相机信息 相机;
	相机.相机坐标 = drv->RPM<Vector3>(CameraCache);
	相机.视角坐标 = drv->RPM<Vector3>(CameraCache + 12);
	相机.fov = 90 / ZoomProgressRate;

	运算坐标3D轴(相机.视角坐标, 相机.转换坐标X, 相机.转换坐标Y, 相机.转换坐标Z);



	if (正在瞄准 == true)
	{
		if (AimPlayer > 0)
		{
			瞄准目标 = true;
		}
		else
		{
			瞄准目标 = false;
			当前距离 = 99999;
		}

	}
	else
	{
		AimPlayer = 0;
		当前距离 = 99999;
	}



	auto EntityList_Copy = entityList;
	for (int i = 0; i < EntityList_Copy.size(); i++)
	{
		auto 当前对象 = EntityList_Copy[i];
		auto 绘制名称 = GetExplanationName(当前对象.acotr_name);
		ULONG64 敌人RootComponent = drv->RPM<ULONG64>(当前对象.actor_pawn + offset_RootComponent);
		Vector3 敌人坐标 = drv->RPM<Vector3>(敌人RootComponent + offset_RelativeLocation);
		ULONG32 敌人队伍 = drv->RPM<ULONG32>(当前对象.actor_state + offset_TeamId);
		ULONG64 DeathComponent = drv->RPM<ULONG64>(当前对象.actor_pawn + offset_DeathComponent);
		ULONG64 IsDead = drv->RPM<ULONG64>(DeathComponent + offset_DeathInfo);
		bool vis = isVisible(当前对象.actor_mesh);
		if (IsDead != 0) { continue; }//0代表存活
		if (敌人队伍 == 本人队伍) { continue; }
		
		ULONG64 EnemyWeaponManagerComponent = drv->RPM<ULONG64>(当前对象.actor_pawn + offset_WeaponManagerComponent);
		ULONG64 EnemyCurrentWeapon = drv->RPM<ULONG64>(EnemyWeaponManagerComponent + offset_CurrentWeapon);
		ULONG32 EnemyWeaponID = drv->RPM<ULONG32>(EnemyCurrentWeapon + 24);
		auto EnemyWeaponname = GetNameFromFName(WeaponID);


		Vec2 屏幕坐标{};
		Vector2 屏幕坐标1;
		ImVec4 Rect;
		if (运算_矩阵转换(敌人坐标, 屏幕坐标1, 相机) == true)
		{

			int 距离 = GetDistance(本人坐标, 敌人坐标, 100);
			屏幕坐标.x = 屏幕坐标1.x - 20;
			屏幕坐标.y = 屏幕坐标1.y + 30;

			Vec2 屏幕坐标2 = 屏幕坐标;

			

			if (距离 < 50 && MenuConfig::ShowLineToEnemy)
			{
				Gui.Line({ 1280,720 }, { 屏幕坐标1.x,屏幕坐标1.y }, MenuConfig::BoxColor, 1.2);
			}
			if (MenuConfig::ShowBoxESP)
			{
				计算方框大小(当前对象.actor_mesh, Rect, 相机);
				Gui.Rectangle({ Rect.x,Rect.y }, { Rect.w,Rect.z }, vis ? MenuConfig::Boneviscolor : MenuConfig::BoxColor, 1.3);
			}
			if (MenuConfig::ShowDistance)
			{
				屏幕坐标2.y -= 15;
				Gui.Text(绘制名称, 屏幕坐标, MenuConfig::BoxColor);
				Gui.Text(std::to_string(距离), 屏幕坐标2, MenuConfig::BoxColor);
				屏幕坐标2.y -= 15;
				Gui.Text(std::to_string(敌人队伍), 屏幕坐标2, MenuConfig::BoxColor);
				屏幕坐标2.y -= 15;
				Gui.Text(EnemyWeaponname, 屏幕坐标2, MenuConfig::BoxColor);

				
			}



			if (瞄准目标 == false)
			{
				Vector2 准星 = { 1280.f,720.f };//Screen center

				float 敌人准星距离 = 取_准星距离(准星, 屏幕坐标1);
				if (敌人准星距离 < 自瞄范围 && 距离 > 5)
				{

					if (敌人准星距离 == 0)
					{
						当前距离 = 敌人准星距离;
						AimPlayer = 当前对象.actor_pawn;
					}
					else if (敌人准星距离 < 当前距离)
					{
						当前距离 = 敌人准星距离;
						AimPlayer = 当前对象.actor_pawn;
						
					}
				}
			}


		}

	}
		


		DWORD CurTimePoint1 = timeGetTime();
		
		if (GetAsyncKeyState(2) && CurTimePoint1 - LastTimePoint1 >= 1 )
		{
			Vector3 敌人头;
			Vector2 屏幕坐标;
			Vec2 屏幕坐标1;

			ULONG64 Mesh = drv->RPM<ULONG64>(AimPlayer + offset_Mesh);
			GetBonePos(Mesh, 敌人头, 15);//15是脖子 16是头
			if (运算_矩阵转换(敌人头, 屏幕坐标, 相机) == true)
			{
				屏幕坐标1.x = 屏幕坐标.x;
				屏幕坐标1.y = 屏幕坐标.y;

				bool vis = isVisible(Mesh);
				if (vis)
				{
					move_to(屏幕坐标.x, 屏幕坐标.y);
				}
				
				//Gui.Text("Aim", 屏幕坐标1, MenuConfig::BoxColor);
				//正在瞄准 = true;
			}
			;
			LastTimePoint1 = CurTimePoint1;
		}
		else
		{
			
			//正在瞄准 = false;
		}
		

	//Sleep(1);
}




int main()
{
	BOOL d = LoadDriver("use youself driver");
	if (d)
	{

		DWORD pid = GetProcessID("UAGame.exe");


		HANDLE Thread = CreateThread(nullptr, NULL, reinterpret_cast<LPTHREAD_START_ROUTINE>(loopData), nullptr, NULL, nullptr);
		//CloseHandle(Thread);
/*

		Gui.AttachAnotherWindow("Arena Breakout Infinite  ", "UnrealWindow", Run);




	}

	system("pause");

}