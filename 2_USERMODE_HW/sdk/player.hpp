#include "../Driver/API/Driver.h"
#include "../utils/pimraryXor.h"
#include "data/weapon_info.h"

#include "../utils/vector.h"

#include <stdio.h>

struct Bones3D {
	Vector3 Ahead;
	Vector3 Acou;
	Vector3 AshoulderR;
	Vector3 AshoulderL;
	Vector3 AbrasR;
	Vector3 AbrasL;
	Vector3 AhandR;
	Vector3 AhandL;
	Vector3 Acock;
	Vector3 AkneesR;
	Vector3 AkneesL;
	Vector3 AfeetR;
	Vector3 AfeetL;
};

struct C_UTL_VECTOR
{
	DWORD_PTR count = 0;
	DWORD_PTR data = 0;
};

class LocalPlayer {
private:

public:
	uint64_t LocalPlayerPtr = NULL;
	uint64_t modBase = NULL;

	int m_iHealth = 0;
	int m_iTeamNum = 0;
	int m_iShotsFired = 0;

	C_UTL_VECTOR aimPunchCache;
	Vector2 aimPunchAngle;
	Vector3 viewAngle;
	Vector3 Localorigin;
	uintptr_t BoneArray = NULL;
	uintptr_t CGameSceneNode = NULL;

	LocalPlayer() {
		this->modBase = Driver->ClientDLL;
		this->LocalPlayerPtr = Driver->RPM<uint64_t>(this->modBase + offsets::client_dll::dwLocalPlayerPawn);
		if (this->LocalPlayerPtr == NULL) return;
		this->m_iTeamNum = Driver->RPM<int>(this->LocalPlayerPtr + schemas::client_dll::C_BaseEntity::m_iTeamNum);
		this->m_iShotsFired = Driver->RPM<int>(this->LocalPlayerPtr + schemas::client_dll::C_CSPlayerPawn::m_iShotsFired);

		this->aimPunchCache = Driver->RPM<C_UTL_VECTOR>(this->LocalPlayerPtr + schemas::client_dll::C_CSPlayerPawn::m_aimPunchCache);
		this->aimPunchAngle = Driver->RPM<Vector2>(this->LocalPlayerPtr + schemas::client_dll::C_CSPlayerPawn::m_aimPunchAngle);
		this->viewAngle = Driver->RPM<Vector3>(this->LocalPlayerPtr + schemas::client_dll::C_BasePlayerPawn::v_angle);

		this->CGameSceneNode = Driver->RPM<uintptr_t>(this->LocalPlayerPtr + schemas::client_dll::C_BaseEntity::m_pGameSceneNode);
		this->BoneArray = Driver->RPM<uintptr_t>(this->CGameSceneNode + schemas::client_dll::CSkeletonInstance::m_modelState + 0x80);
		this->Localorigin = Driver->RPM<Vector3>(this->BoneArray + 6 * 32);
	}

};

struct CSEntity {
	char name[128];
};

class Player {
private:

public:
	int IndexID = 0;
	uint64_t modBase = NULL;
	uintptr_t entity_list = NULL;
	uintptr_t listEntry = NULL;
	uintptr_t entityController = NULL;
	uintptr_t entityControllerPawn = NULL;

	uintptr_t entity = NULL;
	uintptr_t CGameSceneNode = NULL;
	uintptr_t BoneArray = NULL;
	int m_iHealth = 0;
	int m_iTeamNum = 0;
	bool m_iSpotted = false;
	
	Vector3 origin;
	std::string name;

	Bones3D Bones2D;
	Bones3D Bones3D;

	Player(int IndexID) {
		this->IndexID = IndexID;	
		this->modBase = Driver->ClientDLL;

		if (this->modBase == NULL) return;

		this->entity_list = Driver->RPM<uintptr_t>(this->modBase + offsets::client_dll::dwEntityList);
		if (this->entity_list == NULL) return;
		// Entity Initialization
		this->listEntry = Driver->RPM<uintptr_t>(this->entity_list + ((8 * (IndexID & 0x7ff) >> 9) + 16));
		if (this->listEntry == NULL) return;
		this->entityController = Driver->RPM<uintptr_t>(this->listEntry + 120 * (this->IndexID & 0x1ff));
		if (this->entityController == NULL) return;
		this->entityControllerPawn = Driver->RPM<uintptr_t>(this->entityController + schemas::client_dll::CCSPlayerController::m_hPlayerPawn);
		if (this->entityControllerPawn == NULL) return;

		this->entity = Driver->RPM<uintptr_t>(this->listEntry + 120 * (this->entityControllerPawn & 0x1ff));
		if (this->entity == NULL) return;
		// Initialize Basic Values
		this->m_iHealth = Driver->RPM<int>(this->entity + schemas::client_dll::C_BaseEntity::m_iHealth);
		this->m_iTeamNum = Driver->RPM<int>(this->entity + schemas::client_dll::C_BaseEntity::m_iTeamNum);

		this->CGameSceneNode = Driver->RPM<uintptr_t>(this->entity + schemas::client_dll::C_BaseEntity::m_pGameSceneNode);
		this->BoneArray = Driver->RPM<uintptr_t>(this->CGameSceneNode + schemas::client_dll::CSkeletonInstance::m_modelState + 0x80);

		this->m_iSpotted = Driver->RPM<bool>(this->entity + schemas::client_dll::C_CSPlayerPawn::m_entitySpottedState + schemas::client_dll::EntitySpottedState_t::m_bSpotted);
		
		this->origin = Driver->RPM<Vector3>(this->BoneArray + 6 * 32);

		uintptr_t entityNameAddress = Driver->RPM<uintptr_t>(this->entityController + schemas::client_dll::CCSPlayerController::m_sSanitizedPlayerName);
		if (entityNameAddress) {
			char buf[MAX_PATH] = {};
			Driver->readArray(entityNameAddress, buf, MAX_PATH);
			this->name = std::string(buf);
		}
		else {
			this->name = "Unknown";
		}
	}

	Vector3 GetBone3D(int BoneIndex) {
		Vector3 BoneI;
		if (this->BoneArray == NULL) return BoneI;
		BoneI = Driver->RPM<Vector3>(this->BoneArray + BoneIndex * 32);
		return BoneI;
	}

	void Cache3DBones() {
		struct Quaternion { float x, y, z, w; };
		struct CBoneData
		{
			Vector3 location;
			float scale;
			Quaternion rotation;
		};

		std::array<CBoneData, 30> boneArray;
		Driver->ReadProcessMemory((PVOID)this->BoneArray, boneArray.data(), sizeof(CBoneData) * 30);
		this->Bones3D.Ahead = { boneArray[6].location.x, boneArray[6].location.y, boneArray[6].location.z };
		this->Bones3D.Acou = { boneArray[5].location.x, boneArray[5].location.y, boneArray[5].location.z };
		this->Bones3D.AshoulderR = { boneArray[8].location.x, boneArray[8].location.y, boneArray[8].location.z };
		this->Bones3D.AshoulderL = { boneArray[13].location.x, boneArray[13].location.y, boneArray[13].location.z };
		this->Bones3D.AbrasR = { boneArray[9].location.x, boneArray[9].location.y, boneArray[9].location.z };
		this->Bones3D.AbrasL = { boneArray[14].location.x, boneArray[14].location.y, boneArray[14].location.z };
		this->Bones3D.AhandR = { boneArray[11].location.x, boneArray[11].location.y, boneArray[11].location.z };
		this->Bones3D.AhandL = { boneArray[16].location.x, boneArray[16].location.y, boneArray[16].location.z };
		this->Bones3D.Acock = { boneArray[0].location.x, boneArray[0].location.y, boneArray[0].location.z };
		this->Bones3D.AkneesR = { boneArray[23].location.x, boneArray[23].location.y, boneArray[23].location.z };
		this->Bones3D.AkneesL = { boneArray[26].location.x, boneArray[26].location.y, boneArray[26].location.z };
		this->Bones3D.AfeetR = { boneArray[24].location.x, boneArray[24].location.y, boneArray[24].location.z };
		this->Bones3D.AfeetL = { boneArray[27].location.x, boneArray[27].location.y, boneArray[27].location.z };


		// if (this->BoneArray == NULL) return;
		// if (this->BoneArray == NULL) return; this->Bones3D.Ahead = Driver->RPM<Vector3>(this->BoneArray + 6 * 32);
		// if (this->BoneArray == NULL) return; this->Bones3D.Acou = Driver->RPM<Vector3>(this->BoneArray + 5 * 32);
		// if (this->BoneArray == NULL) return; this->Bones3D.AshoulderR = Driver->RPM<Vector3>(this->BoneArray + 8 * 32);
		// if (this->BoneArray == NULL) return; this->Bones3D.AshoulderL = Driver->RPM<Vector3>(this->BoneArray + 13 * 32);
		// if (this->BoneArray == NULL) return; this->Bones3D.AbrasR = Driver->RPM<Vector3>(this->BoneArray + 9 * 32);
		// if (this->BoneArray == NULL) return; this->Bones3D.AbrasL = Driver->RPM<Vector3>(this->BoneArray + 14 * 32);
		// if (this->BoneArray == NULL) return; this->Bones3D.AhandR = Driver->RPM<Vector3>(this->BoneArray + 11 * 32);
		// if (this->BoneArray == NULL) return; this->Bones3D.AhandL = Driver->RPM<Vector3>(this->BoneArray + 16 * 32);
		// if (this->BoneArray == NULL) return; this->Bones3D.Acock = Driver->RPM<Vector3>(this->BoneArray + 0 * 32);
		// if (this->BoneArray == NULL) return; this->Bones3D.AkneesR = Driver->RPM<Vector3>(this->BoneArray + 23 * 32);
		// if (this->BoneArray == NULL) return; this->Bones3D.AkneesL = Driver->RPM<Vector3>(this->BoneArray + 26 * 32);
		// if (this->BoneArray == NULL) return; this->Bones3D.AfeetR = Driver->RPM<Vector3>(this->BoneArray + 24 * 32);
		// if (this->BoneArray == NULL) return; this->Bones3D.AfeetL = Driver->RPM<Vector3>(this->BoneArray + 27 * 32);
	}

	void Cache2DBones(Camera CameraObject) {
		if (this->BoneArray == NULL) return;
		// I'll fix this later
		Vector3 Head2D; CameraObject.w2s(this->Bones3D.Ahead, Head2D, CameraObject.ViewMatrix);
		Vector3 Cou2D; CameraObject.w2s(this->Bones3D.Acou, Cou2D, CameraObject.ViewMatrix);

		Vector3 AShoulderL; CameraObject.w2s(this->Bones3D.AshoulderL, AShoulderL, CameraObject.ViewMatrix);
		Vector3 AShoulderR; CameraObject.w2s(this->Bones3D.AshoulderR, AShoulderR, CameraObject.ViewMatrix);

		Vector3 ABrasR; CameraObject.w2s(this->Bones3D.AbrasR, ABrasR, CameraObject.ViewMatrix);
		Vector3 ABrasL; CameraObject.w2s(this->Bones3D.AbrasL, ABrasL, CameraObject.ViewMatrix);

		Vector3 AHandR; CameraObject.w2s(this->Bones3D.AhandR, AHandR, CameraObject.ViewMatrix);
		Vector3 AHandL; CameraObject.w2s(this->Bones3D.AhandL, AHandL, CameraObject.ViewMatrix);

		Vector3 ACock; CameraObject.w2s(this->Bones3D.Acock, ACock, CameraObject.ViewMatrix);

		Vector3 AKneesR; CameraObject.w2s(this->Bones3D.AkneesR, AKneesR, CameraObject.ViewMatrix);
		Vector3 AKneesL; CameraObject.w2s(this->Bones3D.AkneesL, AKneesL, CameraObject.ViewMatrix);

		Vector3 AFeetL; CameraObject.w2s(this->Bones3D.AfeetL, AFeetL, CameraObject.ViewMatrix);
		Vector3 AFeetR; CameraObject.w2s(this->Bones3D.AfeetR, AFeetR, CameraObject.ViewMatrix);

		this->Bones2D.Ahead = Head2D;
		this->Bones2D.Acou = Cou2D;
		this->Bones2D.AshoulderL = AShoulderL;
		this->Bones2D.AshoulderR = AShoulderR;
		this->Bones2D.AbrasR = ABrasR;
		this->Bones2D.AbrasL = ABrasL;
		this->Bones2D.AhandR = AHandR;
		this->Bones2D.AhandL = AHandL;
		this->Bones2D.Acock = ACock;
		this->Bones2D.AkneesR = AKneesR;
		this->Bones2D.AkneesL = AKneesL;
		this->Bones2D.AfeetR = AFeetR;
		this->Bones2D.AfeetL = AFeetL;
	}
};