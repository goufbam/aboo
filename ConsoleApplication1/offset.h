#pragma once
#include <Windows.h>
#include "Drv.h"
#include "Math.h"
ULONG64 offset_GWorld = 0x9D7FF48;
ULONG64 offset_GNames = 0xA1A7FC0;
ULONG64 offset_GObjects = 0xA1C22B8;

ULONG64 offset_OwningGameInstance = 0x180; // World -> OwningGameInstance
ULONG64 offset_PersistentLevel = 0x30; // World  -> PersistentLevel
ULONG64 offset_ActorsOffset = 0x98; // Ulevel -> Actors
ULONG64 offset_ActorSize = 0xA0;
ULONG64 offset_LocalPlayers = 0x38; // GameInstance -> LocalPlayers
ULONG64 offset_PlayerController = 0x30; // Player -> PlayerController
ULONG64 offset_AcknowledgedPawn = 0x390; //PlayerController -> AcknowledgedPawn
ULONG64 offset_CameraManager = 0x3A8; // PlayerController -> PlayerCameraManager
ULONG64 offset_CameraCacheOffset = 0x1E90 + 16; // PlayerCameraManager -> CameraCachePrivate
ULONG64 offset_PlayerState = 0x340; // Pawn -> PlayerState
ULONG64 offset_Controller = 0x358; // Pawn -> Controller
ULONG64 offset_RootComponent = 0x168; // Actor -> RootComponent
ULONG64 offset_RelativeLocation = 0x16C; // SceneComponent -> RelativeLocation
ULONG64 offset_Mesh = 0x380; // Character -> Mesh
ULONG64 offset_BoneArrayOne = 0x658;
ULONG64 offset_BoneArrayTwo = offset_BoneArrayOne + 0x10;
ULONG64 offset_ComponentToWorld = 0x210; //Handle as ptr
ULONG64 offset_PlayerNamePrivate = 0x3F0; // PlayerState->PlayerNamePrivate(class FString)  相机
ULONG64 offset_DeathComponent = 0x15B0; // SGFramework.SGCharacter->DeathComponent(class USGCharacterDeathComponent*)
ULONG64 offset_DeathInfo = 0x240; // SGCharacterDeathComponent->DeathInfo(struct FCharacterDeathInfo)
ULONG64 offset_TeamId = 0x4D0; // PlayerState->TeamIndex(int32)
ULONG64 offset_WeaponManagerComponent = 0x15A8; //Player -> WeaponManagerComponent
ULONG64 offset_CurrentWeapon = 0x150; //WeaponManagerComponent -> CurrentWeapon
ULONG64 offset_WeaponZoomComponent = 0xA38; //CurrentWeapon -> WeaponZoomComponent
ULONG64 offset_ZoomProgressRateCheck = 0x3AC; //WeaponZoomComponent -> ZoomProgressRate  this check if is scope or not return only 1 or 0 
ULONG64 offset_ZoomProgressRate = 0X3C0;//WeaponZoomComponent -> offset_ZoomProgressRate not in sdk check your scope if is 4x or 6x return 1,4,6,8 then you can customise your fov in w2s when is 1 or 4 etc..
ULONG64 offset_last_submit_time = 0X3DC;
ULONG64 offset_last_render_time = 0X3E0;
ULONG64 f_Pitch = 0x372;// PlayerController -> Pitch   useless
ULONG64 f_Yaw = 0x376;// PlayerController -> Yaw   useless


ULONG64 GWorld;
ULONG64 ULevel;
ULONG64 ActorsArray;
ULONG32 ActorSize;
ULONG64 OwningGameInstance;
ULONG64 LocalPlayers;
ULONG64 LocalPlayersData;
ULONG64 PlayerController;
ULONG64 AcknowledgedPawn;//本人地址
ULONG64 PlayerCameraManager;
ULONG64 CameraCache;
ULONG64 RootComponent;
Vector3 本人坐标;
ULONG32 ActorID;
ULONG64 本人状态;
ULONG32 本人队伍;
ULONG64 本人Mesh;

bool 瞄准目标 = false;
float 自瞄范围 = 60;
float 当前距离;
ULONG64	AimPlayer;
bool 正在瞄准 = false;
DWORD LastTimePoint1 = 0;
