#include "reverse.h"
#include "rat.h"
#include "Print.hpp"
#include "d3d9_x.h"
#include "xor.hpp"
#include <dwmapi.h>
#include <vector>
#include "Keybind.h"

#include "offsets.h"
#include "xstring"
#include "icons1.h"
#include "Imgui\imgui.h"
#include "ac_spoof.h"
bool holdingsniper;
DWORD_PTR closestPawnbullet = NULL;

Vector3 Ping;
#define FNOffset_UWORLD 0xe9ea3f8
#define FNOffset_GameInstance 0x1b8
#define FNOffset_LocalPlayersInGame 0x38
#define FNOffset_PlayerController  0x30
#define FNOffset_PlayerCameraManager 0x340
#define FNOffset_LocalPawn  0x330
#define FNOffset_PlayerState 0x2a8
#define FNOffset_RootComponet  0x190
#define FNOffset_LocalRelativeLocation  0x128
#define FNOffset_RelativeLocation  0x128
#define FNOffset_RelativeRotation  0x140
#define FNOffset_PersistentLevel  0x30
#define FNOffset_ActorCount  0xA0
#define FNOffset_AACTORS  0X98
#define FNOffset_LocalPlayerID 0x18
#define FNOffset_WorldSettings 0x298
#define FNOffset_DBNOCheck 0x4388
#define FNOffset_Mesh 0x310
#define FNOffset_BulletReduceLag 0x6a0
#define FNOffset_CurrentVehicle 0x23d8
#define FNOffset_Levels 0x170
#define FNOffset_CurrentWeapon 0x8d8
#define FNOffset_WeaponData 0x3f0
#define FNOffset_TimeDilation 0x64
#define FNOffset_KillScore 0x10fc
#define FNOffset_CachedFuelComponent 0x10a0
#define FNOffset_ServerFuel 0xf0
#define FNOffset_bADSWhileNotOnGround 0x4cd6
#define FNOffset_TeamIndex 0x50e
#define FNOffset_DefaultFOV 0x29c
#define FNOffset_AmmoCount 0xb64
#define FNOffset_SeasonLevelUIDisplay 0x1100
#define FNOffset_bIsReloadingWeapon 0x329

//APlayerCameraManager
#define FNOffset_ViewPitchMin  0x3314 //float
#define FNOffset_ViewPitchMax 0x3318 //float
#define FNOffset_ViewYawMin  0x331c //float
#define FNOffset_ViewYawMax  0x3320 //float
#define color1 (WORD)(0x0001 | 0x0000)
#define color2 (WORD)(0x0002 | 0x0000)
#define color3 (WORD)(0x0003 | 0x0000)
#define color4 (WORD)(0x0004 | 0x0000)
#define color5 (WORD)(0x0005 | 0x0000)
#define color6 (WORD)(0x0006 | 0x0000)
#define color7 (WORD)(0x0007 | 0x0000)
#define color8 (WORD)(0x0008 | 0x0000)
#define color9 (WORD)(0x0008 | 0x0000)
#define COLOR(h, c) SetConsoleTextAttribute(h, c);
#include <winuser.h>
bool fovcircle = true;
bool targetlines = false;
bool ShowMenu = true;
bool Esp = true;
bool Esp_box = true;
bool Esp_fbox = false;
bool cornerbox = false;
bool Esp_info = false;
bool cartp = false;
bool Esp_line = false;
bool Aimbot = false;
bool Prediction = false;
bool Skeleton = false;
bool slefESP = false;
bool square_fov = false;
bool silentaim = false;
bool ammoESP = false;
bool AimWhileJumping = false;
bool Esp_Distance = false;
bool threeeeeed = false;
bool playerbot = false;
bool weaponesp = false;
bool carFly = false;
bool niggerfovchanger = false;
bool RapidFire = false;
bool instareload = false;
bool spinbot = false;
bool Safemode = false;
bool reloadcheck = false;
bool fillbox = false;
bool crosshair = true;

bool Esp_Skeleton = false;
bool lineheadesp = false;
bool cornerline = false;
float BoxWidthValue = 0.550;
bool lobby = false;
bool triggerbot = false;
bool guntracers = false;
bool nigger_detector = false;
float FOVChangerValue = 10;
bool ThreeDBox = false;
bool DrawEnemyFov = false;

float ChangerFOV = 300;




ImFont* m_pFont;
static int smooth = 3;
static int VisDist = 50;
float AimFOV = 150;
static int aimkey;
static int hitbox = 98;
static int aimkeypos = 1;
static int hitboxpos = 0;





DWORD_PTR Uworld;
DWORD_PTR LocalPawn;
DWORD_PTR PlayerState;
DWORD_PTR Localplayer;
DWORD_PTR Rootcomp;
DWORD_PTR PlayerController;
DWORD_PTR Persistentlevel;
uintptr_t PlayerCameraManager;
Vector3 localactorpos;

uint64_t TargetPawn;
int localplayerID;

RECT GameRect = { NULL };
D3DPRESENT_PARAMETERS d3dpp;
DWORD ScreenCenterX;
DWORD ScreenCenterY;
Vector3 LocalRelativeLocation; struct FBoxSphereBounds
{
	struct Vector3                                     Origin;                                                   // 0x0000(0x0018) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
	struct Vector3                                     BoxExtent;                                                // 0x0018(0x0018) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
	double                                             SphereRadius;                                             // 0x0030(0x0008) (Edit, BlueprintVisible, ZeroConstructor, SaveGame, IsPlainOldData)
};
static void CreateWindows();
static void xInitD3d();
static void mainloop();
static void Shutdown();
extern LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

static HWND Window = NULL;
IDirect3D9Ex* p_Object = NULL;
static LPDIRECT3DDEVICE9 D3dDevice = NULL;
static LPDIRECT3DVERTEXBUFFER9 TriBuf = NULL;
LRESULT CALLBACK WinProc(HWND hWnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	if (ImGui_ImplWin32_WndProcHandler(hWnd, Message, wParam, lParam))
		return true;

	switch (Message)
	{
	case WM_DESTROY:
		Shutdown();
		PostQuitMessage(0);
		exit(4);
		break;
	case WM_SIZE:
		if (D3dDevice != NULL && wParam != SIZE_MINIMIZED)
		{
			ImGui_ImplDX9_InvalidateDeviceObjects();
			d3dpp.BackBufferWidth = LOWORD(lParam);
			d3dpp.BackBufferHeight = HIWORD(lParam);
			HRESULT hr = D3dDevice->Reset(&d3dpp);
			if (hr == D3DERR_INVALIDCALL)
				IM_ASSERT(0);
			ImGui_ImplDX9_CreateDeviceObjects();
		}
		break;
	default:
		return DefWindowProc(hWnd, Message, wParam, lParam);
		break;
	}
	return 0;
}
void Shutdown()
{
	TriBuf->Release();
	D3dDevice->Release();
	p_Object->Release();

	DestroyWindow(Window);
	UnregisterClass("Discord", NULL);
}
typedef struct {
	float X, Y, Z;
} FVector;

typedef struct {
	float X, Y;
} FVector2D;

typedef struct {
	float Pitch;
	float Yaw;
	float Roll;
} FRotator;

typedef struct {
	FVector Location;
	FRotator Rotation;
	float FOV;
	float OrthoWidth;
	float OrthoNearClipPlane;
	float OrthoFarClipPlane;
	float AspectRatio;
} FMinimalViewInfo;

typedef struct {
	float M[4][4];
} FMatrix;

typedef struct {
	FVector ViewOrigin;
	char _padding_0[4];
	FMatrix ViewRotationMatrix;
	FMatrix ProjectionMatrix;
} FSceneViewProjectionData;




class UClass {
public:
	BYTE _padding_0[0x40];
	UClass* SuperClass;
};

class UObject {
public:
	PVOID VTableObject;
	DWORD ObjectFlags;
	DWORD InternalIndex;
	UClass* Class;
	BYTE _padding_0[0x8];
	UObject* Outer;

	inline BOOLEAN IsA(PVOID parentClass) {
		for (auto super = this->Class; super; super = super->SuperClass) {
			if (super == parentClass) {
				return TRUE;
			}
		}

		return FALSE;
	}
};

class FUObjectItem {
public:
	UObject* Object;
	DWORD Flags;
	DWORD ClusterIndex;
	DWORD SerialNumber;
	DWORD SerialNumber2;
};

class TUObjectArray {
public:
	FUObjectItem* Objects[9];
};

class GObjects {
public:
	TUObjectArray* ObjectArray;
	BYTE _padding_0[0xC];
	DWORD ObjectCount;
};


template<class T>
struct TArray {
	friend struct FString;

public:
	inline TArray() {
		Data = nullptr;
		Count = Max = 0;
	};

	inline INT Num() const {
		return Count;
	};

	inline T& operator[](INT i) {
		return Data[i];
	};

	inline BOOLEAN IsValidIndex(INT i) {
		return i < Num();
	}

private:
	T* Data;
	INT Count;
	INT Max;
};
#include <intrin.h>
#include <list>
float powf_(float _X, float _Y) {
	return (_mm_cvtss_f32(_mm_pow_ps(_mm_set_ss(_X), _mm_set_ss(_Y))));
}
float sqrtf_(float _X) {
	return (_mm_cvtss_f32(_mm_sqrt_ps(_mm_set_ss(_X))));
}
double GetDistance(double x1, double y1, double z1, double x2, double y2) {
	//return sqrtf(powf_((x2 - x1), 2) + powf_((y2 - y1), 2));
	return sqrtf(powf((x2 - x1), 2) + powf_((y2 - y1), 2));
}
bool memoryaim = false;
struct FString : private TArray<WCHAR> {
	FString() {
		Data = nullptr;
		Max = Count = 0;
	}

	FString(LPCWSTR other) {
		Max = Count = static_cast<INT>(wcslen(other));

		if (Count) {
			Data = const_cast<PWCHAR>(other);
		}
	};

	inline BOOLEAN IsValid() {
		return Data != nullptr;
	}

	inline PWCHAR c_str() {
		return Data;
	}
};

VOID(*FreeInternal)(PVOID) = nullptr;

VOID Free(PVOID buffer) {
	FreeInternal(buffer);
}


#pragma once
uintptr_t virtualaddy;

#define code_rw CTL_CODE(FILE_DEVICE_UNKNOWN, 0x71, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_ba CTL_CODE(FILE_DEVICE_UNKNOWN, 0x72, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_get_guarded_region CTL_CODE(FILE_DEVICE_UNKNOWN, 0x73, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define code_security 0x85b3e12

typedef struct _rw {
	INT32 security;
	INT32 process_id;
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
	BOOLEAN write;
} rw, * prw;

typedef struct _ba {
	INT32 security;
	INT32 process_id;
	ULONGLONG* address;
} ba, * pba;

typedef struct _ga {
	INT32 security;
	ULONGLONG* address;
} ga, * pga;

namespace mem {
	HANDLE driver_handle;
	INT32 process_id;

	bool find_driver() {
		driver_handle = CreateFileW((L"\\\\.\\\msmodule"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

		if (!driver_handle || (driver_handle == INVALID_HANDLE_VALUE))
			return false;

		return true;
	}

	void read_physical(PVOID address, PVOID buffer, DWORD size) {
		_rw arguments = { 0 };

		arguments.security = code_security;
		arguments.address = (ULONGLONG)address;
		arguments.buffer = (ULONGLONG)buffer;
		arguments.size = size;
		arguments.process_id = process_id;
		arguments.write = FALSE;

		DeviceIoControl(driver_handle, code_rw, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
	}

	void write_physical(PVOID address, PVOID buffer, DWORD size) {
		_rw arguments = { 0 };

		arguments.security = code_security;
		arguments.address = (ULONGLONG)address;
		arguments.buffer = (ULONGLONG)buffer;
		arguments.size = size;
		arguments.process_id = process_id;
		arguments.write = TRUE;

		DeviceIoControl(driver_handle, code_rw, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
	}

	uintptr_t find_image() {
		uintptr_t image_address = { NULL };
		_ba arguments = { NULL };

		arguments.security = code_security;
		arguments.process_id = process_id;
		arguments.address = (ULONGLONG*)&image_address;

		DeviceIoControl(driver_handle, code_ba, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);

		return image_address;
	}

	uintptr_t get_guarded_region() {
		uintptr_t guarded_region_address = { NULL };
		_ga arguments = { NULL };

		arguments.security = code_security;
		arguments.address = (ULONGLONG*)&guarded_region_address;

		DeviceIoControl(driver_handle, code_get_guarded_region, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);

		return guarded_region_address;
	}

	INT32 find_process(LPCTSTR process_name) {
		PROCESSENTRY32 pt;
		HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		pt.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hsnap, &pt)) {
			do {
				if (!lstrcmpi(pt.szExeFile, process_name)) {
					CloseHandle(hsnap);
					process_id = pt.th32ProcessID;
					return pt.th32ProcessID;
				}
			} while (Process32Next(hsnap, &pt));
		}
		CloseHandle(hsnap);

		return { NULL };
	}
}

template <typename T>
T read(uint64_t address) {
	T buffer{ };
	mem::read_physical((PVOID)address, &buffer, sizeof(T));
	return buffer;
}

template <typename T>
T write(uint64_t address, T buffer) {

	mem::write_physical((PVOID)address, &buffer, sizeof(T));
	return buffer;
}
#define BONE_HEAD_ID (68)
#define BONE_NECK_ID (67)
#define BONE_CHEST_ID (36)
#define BONE_PELVIS_ID (2)

FTransform GetBoneIndex(DWORD_PTR mesh, int index)
{
	DWORD_PTR bonearray = read<DWORD_PTR>(mesh + 0x5d0);

	if (!bonearray)
	{
		bonearray = read<DWORD_PTR>(mesh + 0x5d0 + 0x10);
	}
	return read<FTransform>(bonearray + (index * 0x60));
}

Vector3 GetBoneWithRotation(DWORD_PTR mesh, int id)
{
	FTransform bone = GetBoneIndex(mesh, id);
	FTransform ComponentToWorld = read<FTransform>(mesh + 0x240);

	D3DMATRIX Matrix;
	Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

	return Vector3(Matrix._41, Matrix._42, Matrix._43);
}

D3DXMATRIX Matrix(Vector3 rot, Vector3 origin = Vector3(0, 0, 0))
{
	float radPitch = (rot.x * float(M_PI) / 180.f);
	float radYaw = (rot.y * float(M_PI) / 180.f);
	float radRoll = (rot.z * float(M_PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	D3DMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;

	return matrix;
}

Vector3 Location;

//CamewaDescwipsion
struct Camera
{
	float FieldOfView;
	Vector3 Rotation;
	Vector3 Location;
};

Camera GetCamera(__int64 a1)
{
	Camera camera;
	__int64 v1;

	v1 = read<__int64>(Localplayer + 0xd0);
	__int64 v9 = read<__int64>(v1 + 0x8);
	camera.FieldOfView = 80.f / (read<double>(v9 + 0x7F0) / 1.19f);
	camera.Rotation.x = read<double>(v9 + 0x9C0);
	camera.Rotation.y = read<double>(a1 + 0x148);

	uint64_t PointerLocation = read<uint64_t>(Uworld + 0x110);
	camera.Location = read<Vector3>(PointerLocation);
	return camera;
}

Vector3 ProjectWorldToScreen(Vector3 WorldLocation)
{
	Camera vCamera = GetCamera(Rootcomp);
	vCamera.Rotation.x = (asin(vCamera.Rotation.x)) * (180.0 / M_PI);


	D3DMATRIX MatrixTemp = Matrix(vCamera.Rotation);

	Vector3 vAxisX = Vector3(MatrixTemp.m[0][0], MatrixTemp.m[0][1], MatrixTemp.m[0][2]);
	Vector3 vAxisY = Vector3(MatrixTemp.m[1][0], MatrixTemp.m[1][1], MatrixTemp.m[1][2]);
	Vector3 vAxisZ = Vector3(MatrixTemp.m[2][0], MatrixTemp.m[2][1], MatrixTemp.m[2][2]);

	Vector3 vDelta = WorldLocation - vCamera.Location;
	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	return Vector3((Width / 2.0f) + vTransformed.x * (((Width / 2.0f) / tanf(vCamera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, (Height / 2.0f) - vTransformed.y * (((Width / 2.0f) / tanf(vCamera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, 0);
}

static std::string ReadGetNameFromFName(int key)
{
	uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
	uint16_t NameOffset = (uint16_t)key;

	uint64_t NamePoolChunk = read<uint64_t>(base_address + 0xEE7C240 + (8 * ChunkOffset) + 16) + (unsigned int)(4 * NameOffset); // FNamePool
	uint16_t nameEntry = read<uint16_t>(NamePoolChunk);

	int nameLength = nameEntry >> 6;
	char buff[1024];
	if ((uint32_t)nameLength)
	{
		for (int x = 0; x < nameLength; ++x)
		{
			buff[x] = read<char>(NamePoolChunk + 4 + x);
		}
		char* v2 = buff; // rdi
		int v4 = nameLength; // ebx
		__int64 result; // rax
		int v6; // r8d
		unsigned int v7; // ecx
		char v8; // dl

		result = read<int>(base_address + 0xED83F44) >> 5;
		v6 = 0;

		if (v4)
		{
			do
			{
				v7 = *v2++;
				v8 = result ^ (16 * v7) ^ (result ^ (v7 >> 4)) & 0xF;
				result = (unsigned int)(result + 2 * v6++);
				*(v2 - 1) = v8;
			} while (v6 < v4);
		}
		buff[nameLength] = '\0';
		return std::string(buff);
	}
	else
	{
		return ("");
	}
}
void DrawESP();

void DrawLine(int x1, int y1, int x2, int y2, ImU32 color, float thickness)
{
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(x1, y1), ImVec2(x2, y2), ImGui::GetColorU32(color), thickness);
}

void LobbyLine(int x1, int y1, int x2, int y2, ImU32 color, int thickness)
{
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(959, 1100), ImVec2(960, 540), ImGui::GetColorU32(color), 1.0f);
}

void LobbyBox(float X, float Y, float W, float H, ImU32 color)
{
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(815 + 1, 967 + 1), ImVec2(((830 + 275) - 1), ((851 - 625) - 1)), ImGui::GetColorU32(color));
	//ImGui::GetOverlayDrawList()->AddRect(ImVec2(150, 60), ImVec2(150 + 63, 60 + 125), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
}

void LobbyFilled(float X, float Y, float W, float H, ImU32 color)
{
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(815 + 1, 967 + 1), ImVec2(((830 + 275) - 1), ((851 - 625) - 1)), ImGui::GetColorU32(color), 0, 0);
	//ImGui::GetOverlayDrawList()->AddRect(ImVec2(150, 60), ImVec2(150 + 63, 60 + 125), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
}


void LobbyCornerBox2(float X, float Y, float W, float H, const ImU32& color, int thickness) {
	float lineW = (W / 3);
	float lineH = (H / 3);

	//black outlines
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);

	//corners
	//ImGui::GetOverlayDrawList()->AddRect(ImVec2(815 + 1, 967 + 1), ImVec2(((830 + 275) - 1), ((851 - 625) - 1)), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)));
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
}

void DrawStrokeText(int x, int y, const char* str)
{
	ImFont a;
	std::string utf_8_1 = std::string(str);
	std::string utf_8_2 = string_To_UTF8(utf_8_1);

	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x - 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x + 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(255, 255, 255, 255)), utf_8_2.c_str());
}
void Deliration(DWORD x, DWORD y)
{
	INPUT input = { 0 };
	input.type = INPUT_MOUSE;
	input.mi.dwFlags = MOUSEEVENTF_MOVE;
	input.mi.dx = x;
	input.mi.dy = y;
	DWORD oldTime = GetTickCount();
	Spoofed_keybd_event(VK_RETURN, 0, 0, 0);
	Spoofed_mouse_event(MOUSEEVENTF_MOVE, x, y, 0, 0);
	DWORD newTime = GetTickCount();
	if ((newTime - oldTime) > 2000)
	{
		Spoofed_mouse_event(MOUSEEVENTF_MOVE, x, y, 0, 0);
	}
}

int random(int min, int max) {
	return min + (rand() % (int)(max - min + 1));
}
// MADE BY BAS AND AC //
enum InjectedInputMouseOptions
{
	Absolute = 32768,
	HWheel = 4096,
	LeftDown = 2,
	LeftUp = 4,
	MiddleDown = 32,
	MiddleUp = 64,
	Move = 1,
	MoveNoCoalesce = 8192,
	None = 0,
	RightDown = 8,
	RightUp = 16,
	VirtualDesk = 16384,
	Wheel = 2048,
	XDown = 128,
	XUp = 256
};

typedef struct _InjectedInputMouseInfo
{
	int DeltaX;
	int DeltaY;
	unsigned int MouseData;
	InjectedInputMouseOptions MouseOptions;
	unsigned int TimeOffsetInMilliseconds;
	void* ExtraInfo;
} InjectedInputMouseInfo;

typedef bool (WINAPI* InjectMouseInput_t)(InjectedInputMouseInfo* inputs, int count);

InjectMouseInput_t InjectMouseInput;

//void aimbot(float x, float y)
//{
//	float ScreenCenterX = (Width / 2);
//	float ScreenCenterY = (Height / 2);
//	int AimSpeed = smooth;
//	float TargetX = 0;
//	float TargetY = 0;
//
//	if (x != 0)
//	{
//		if (x > ScreenCenterX)
//		{
//			TargetX = -(ScreenCenterX - x);
//			TargetX /= AimSpeed;
//			if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
//		}
//
//		if (x < ScreenCenterX)
//		{
//			TargetX = x - ScreenCenterX;
//			TargetX /= AimSpeed;
//			if (TargetX + ScreenCenterX < 0) TargetX = 0;
//		}
//	}
//
//	if (y != 0)
//	{
//		if (y > ScreenCenterY)
//		{
//			TargetY = -(ScreenCenterY - y);
//			TargetY /= AimSpeed;
//			if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
//		}
//
//		if (y < ScreenCenterY)
//		{
//			TargetY = y - ScreenCenterY;
//			TargetY /= AimSpeed;
//			if (TargetY + ScreenCenterY < 0) TargetY = 0;
//		}
//	}
//
//	InjectedInputMouseInfo info = { 0 };
//	info.DeltaX = TargetX;
//	info.DeltaY = TargetY;
//	InjectMouseInput(&info, 1);
//
//
//
//
//	return;
//}
void aimbot(float x, float y)
{
	// Calculate the center of the screen 
	float screenCenterX = (Width / 2);
	float screenCenterY = (Height / 2);

	// The aim speed 
	int AimSpeed = smooth;

	// Target X and Y position 
	float TargetX = 0;
	float TargetY = 0;

	// Check if the target is to the right or left of the screen center 
	if (x != 0)
	{
		// If the target is to the right of the center 
		if (x > screenCenterX)
		{
			// Calculate the distance between the center and the target 
			TargetX = -(screenCenterX - x);
			TargetX /= AimSpeed;

			// Check if the target is out of range and reset if necessary 
			if (TargetX + screenCenterX > screenCenterX * 2)
				TargetX = 0;
		}

		// If the target is to the left of the center 
		if (x < screenCenterX)
		{
			// Calculate the distance between the center and the target 
			TargetX = x - screenCenterX;
			TargetX /= AimSpeed;

			// Check if the target is out of range and reset if necessary 
			if (TargetX + screenCenterX < 0)
				TargetX = 0;
		}
	}

	// Check if the target is above or below the screen center 
	if (y != 0)
	{
		// If the target is below the center 
		if (y > screenCenterY)
		{
			// Calculate the distance between the center and the target 
			TargetY = -(screenCenterY - y);
			TargetY /= AimSpeed;

			// Check if the target is out of range and reset if necessary 
			if (TargetY + screenCenterY > screenCenterY * 2)
				TargetY = 0;
		}

		// If the target is above the center 
		if (y < screenCenterY)
		{
			// Calculate the distance between the center and the target 
			TargetY = y - screenCenterY;
			TargetY /= AimSpeed;

			// Check if the target is out of range and reset if necessary 
			if (TargetY + screenCenterY < 0)
				TargetY = 0;
		}
	}

	// Inject user-defined mouse input 
	InjectedInputMouseInfo info = { 0 };
	info.DeltaX = TargetX;
	info.DeltaY = TargetY;
	InjectMouseInput(&info, 1);

	return;
}
float DrawOutlinedText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center)
{
	std::stringstream stream(text);
	std::string line;

	float y = 0.0f;
	int i = 0;

	ImVec2 screenSize = ImGui::GetIO().DisplaySize;

	while (std::getline(stream, line))
	{
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());

		if (center)
		{
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((screenSize.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((screenSize.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((screenSize.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((screenSize.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());

			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2(screenSize.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
		}
		else
		{
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());

			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
		}

		y = pos.y + textSize.y * (i + 1);
		i++;
	}
	return y;
}

void DrawText1(int x, int y, const char* str, RGBA* color)
{
	ImFont a;
	std::string utf_8_1 = std::string(str);
	std::string utf_8_2 = string_To_UTF8(utf_8_1);
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), utf_8_2.c_str());
}

void DrawCircle(int x, int y, int radius, RGBA* color, int segments)
{
	ImGui::GetOverlayDrawList()->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(ImVec4(color->R / 255.0, color->G / 255.0, color->B / 255.0, color->A / 255.0)), segments);
}
void DrawBox(float X, float Y, float W, float H, const ImU32& color, float thickness)
{
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(X + 1, Y + 1), ImVec2(((X + W) - 1), ((Y + H) - 1)), ImGui::GetColorU32(color), 0, 0, thickness);
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(X, Y), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), 0, 0, thickness);
}
void DrawFilledRect(int x, int y, int w, int h, ImU32 color)
{
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x, y), ImVec2(x + w, y + h), color, 0, 0);
}
void DrawNormalBox(int x, int y, int w, int h, int borderPx, ImU32 color)
{

	DrawFilledRect(x + borderPx, y, w, borderPx, color); //top 
	DrawFilledRect(x + w - w + borderPx, y, w, borderPx, color); //top 
	DrawFilledRect(x, y, borderPx, h, color); //left 
	DrawFilledRect(x, y + h - h + borderPx * 2, borderPx, h, color); //left 
	DrawFilledRect(x + borderPx, y + h + borderPx, w, borderPx, color); //bottom 
	DrawFilledRect(x + w - w + borderPx, y + h + borderPx, w, borderPx, color); //bottom 
	DrawFilledRect(x + w + borderPx, y, borderPx, h, color);//right 
	DrawFilledRect(x + w + borderPx, y + h - h + borderPx * 2, borderPx, h, color);//right 
}
void DrawCorneredBox(int X, int Y, int W, int H, const ImU32& color, int thickness) {
	float lineW = (W / 3);
	float lineH = (H / 3);

	//black outlines
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0, 1 / 255.0, 1 / 255.0, 255 / 255.0)), 3);

	//corners
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), ImGui::GetColorU32(color), thickness);
}
struct HandleDisposer
{
	using pointer = HANDLE;
	void operator()(HANDLE handle) const
	{
		if (handle != NULL || handle != INVALID_HANDLE_VALUE)
		{
			CloseHandle(handle);
		}
	}
};
using unique_handle = std::unique_ptr<HANDLE, HandleDisposer>;
DWORD Menuthread(LPVOID in)
{
	while (1)
	{
		if (Spoofed_GetAsyncKeyState_Spoofed(VK_F1) & 1) { 
			ShowMenu = !ShowMenu;
		}
		Sleep(2);
		if (Spoofed_GetAsyncKeyState_Spoofed(VK_INSERT) & 1) {
			ShowMenu = !ShowMenu;
		}
		Sleep(2);
	}
}
static std::uint32_t _GetProcessId(std::string process_name) {
	PROCESSENTRY32 processentry;
	const unique_handle snapshot_handle(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0));

	if (snapshot_handle.get() == INVALID_HANDLE_VALUE)
		return 0;

	processentry.dwSize = sizeof(MODULEENTRY32);

	while (Process32Next(snapshot_handle.get(), &processentry) == TRUE) {
		if (process_name.compare(processentry.szExeFile) == 0)
			return processentry.th32ProcessID;
	}
	return 0;
}
const int MEM_EXECUTE_OPTION_DISABLE = 0x1;
bool abed()
{

	DWORD ProcessExecuteFlags = MEM_EXECUTE_OPTION_DISABLE;

	typedef LONG NTSTATUS;
	typedef NTSTATUS(WINAPI* PNtSetInformationProcess)(HANDLE, UINT, PVOID, ULONG);

	HMODULE NtdllHandle = GetModuleHandleA("ntdll.dll");
	PNtSetInformationProcess NtSetInformationProcess = (PNtSetInformationProcess)GetProcAddress(NtdllHandle, "NtSetInformationProcess");

	NTSTATUS NtStatus = NtSetInformationProcess(GetCurrentProcess(), 29, &ProcessExecuteFlags, sizeof(DWORD));
	return (NtStatus == 0);
}
void SetWindowToTarget()
{
	while (true)
	{
		
		if (hwnd)
		{
			ZeroMemory(&GameRect, sizeof(GameRect));
			GetWindowRect(hwnd, &GameRect);
			Width = GameRect.right - GameRect.left;
			Height = GameRect.bottom - GameRect.top;
			DWORD dwStyle = GetWindowLong(hwnd, GWL_STYLE);

			if (dwStyle & WS_BORDER)
			{
				GameRect.top += 32;
				Height -= 39;
			}
			ScreenCenterX = Width / 2;
			ScreenCenterY = Height / 2;
			MoveWindow(Window, GameRect.left, GameRect.top, Width, Height, true);

		}
		else
		{
			exit(0);
		}
	}
}

bool isVisible(uint64_t mesh)
{
	float LastSumbitTime = read<float>(mesh + 0x338);
	float LastRenderTimeOnScreen = read<float>(mesh + 0x340);

	bool Visible = LastRenderTimeOnScreen + 0.06f >= LastSumbitTime;
	return Visible;
}

const MARGINS Margin = { -1 };
void CreateWindows()
{

	//HWND hwnd = CreateWindowA("STATIC", NULL, WS_CHILD | WS_VISIBLE | SS_BITMAP, 0, 0, 0, 0, hwnd, (HMENU)NULL, NULL, NULL);
	HWND targetWindow = FindWindow(NULL, "Discord.exe"); // targetwindow: Discord.


	SetWindowLong(targetWindow, GWL_STYLE, WS_VISIBLE | WS_POPUP);
	SetWindowPos(targetWindow, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	SpoofShowWindow(targetWindow, SW_SHOW);
	UpdateWindow(targetWindow);


	SetWindowLong(Window, NULL, (LONG)targetWindow);
	SpoofSetWindowPos(Window, NULL, 0, 0, 0, 0, SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);

	SpoofCreateThread(0, 0, (LPTHREAD_START_ROUTINE)SetWindowToTarget, (LPVOID)targetWindow, 0, 0);

	WNDCLASS windowClass = { 0 };
	windowClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hInstance = NULL;
	windowClass.lpfnWndProc = WinProc;
	windowClass.lpszClassName = "Discord";
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	if (!RegisterClass(&windowClass))
		return;

	Window = CreateWindowEx(WS_EX_LAYERED | WS_EX_TRANSPARENT, "Discord",
		NULL,
		WS_POPUP,
		0,
		0,
		SpoofGetSystemMetrics(SM_CXSCREEN),
		SpoofGetSystemMetrics(SM_CYSCREEN),
		NULL,
		NULL,
		NULL,
		NULL);

	SpoofSetLayeredWindowAttributes(Window, 0, 255, LWA_ALPHA);

	SpoofShowWindow(Window, SW_SHOW);

	DwmExtendFrameIntoClientArea(Window, &Margin);
	SpoofUpdateWindow(Window);
}


void xInitD3d()
{

	if (FAILED(Direct3DCreate9Ex(D3D_SDK_VERSION, &p_Object)))
		exit(3);

	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = Width;
	d3dpp.BackBufferHeight = Height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.hDeviceWindow = Window;
	d3dpp.Windowed = TRUE;

	p_Object->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, Window, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &D3dDevice);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui_ImplWin32_Init(Window);
	ImGui_ImplDX9_Init(D3dDevice);

	auto& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0 };
	ImFontConfig icons_config;
	icons_config.MergeMode = true;
	icons_config.PixelSnapH = true;
	icons_config.OversampleH = 3;
	icons_config.OversampleV = 3;

	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\CONSOLA.ttf", 13.f);
	io.Fonts->AddFontFromMemoryCompressedTTF(font_awesome_data, font_awesome_size, 13.0f, &icons_config, icons_ranges);

	p_Object->Release();
}

typedef struct _FNlEntity
{
	uint64_t Actor;
	int ID;
	uint64_t mesh;
}FNlEntity;
Vector3 GetLoc(Vector3 Hitbone)
{
	Vector3 LOC = Vector3(Hitbone.x, Hitbone.y, Hitbone.z);

	LOC.x += ((float)rand() / RAND_MAX) * 0.01f;
	LOC.y += ((float)rand() / RAND_MAX) * 0.01f;
	LOC.z += ((float)rand() / RAND_MAX) * 0.01f;

	return LOC;
}
Vector3 AimbotCorrection(float bulletVelocity, float bulletGravity, float targetDistance, Vector3 targetPosition, Vector3 targetVelocity) {
	Vector3 recalculated = targetPosition;
	float gravity = fabs(bulletGravity);
	float time = targetDistance / fabs(bulletVelocity);
	float bulletDrop = (gravity / 250) * time * time;
	recalculated.z += bulletDrop * 120;
	recalculated.x += time * (targetVelocity.x);
	recalculated.y += time * (targetVelocity.y);
	recalculated.z += time * (targetVelocity.z);

	recalculated.x += ((float)rand() / RAND_MAX) * 0.01f;
	recalculated.y += ((float)rand() / RAND_MAX) * 0.01f;
	recalculated.z += ((float)rand() / RAND_MAX) * 0.01f;

	return recalculated;
}

Vector3 CalculateNewRotation(Vector3& zaz, Vector3& daz) {
	Vector3 dalte = zaz - daz;
	Vector3 ongle;
	float hpm = sqrtf(dalte.x * dalte.x + dalte.y * dalte.y);
	ongle.y = atan(dalte.y / dalte.x) * 57.295779513082f;
	ongle.x = (atan(dalte.z / hpm) * 57.295779513082f) * -1.f;
	if (dalte.x >= 0.f) ongle.y += 180.f;

	ongle.x += ((float)rand() / RAND_MAX) * 0.01f;
	ongle.y += ((float)rand() / RAND_MAX) * 0.01f;

	return ongle;
}

std::vector<FNlEntity> entityList;
void DrawESP() {
	if (fovcircle)
	{
		ImGui::GetOverlayDrawList()->AddCircle(ImVec2(Width / 2, Height / 2), AimFOV, ImColor(255, 165, 0, 255), 2000, 2.0f);
	}
	if (crosshair)
	{

		ImDrawList* drawList = ImGui::GetOverlayDrawList();


		drawList->AddText(NULL, 16.0f, ImVec2(ImGui::GetIO().DisplaySize.x / 2, ImGui::GetIO().DisplaySize.y / 2 - 10), ImColor(0, 0, 255, 255), "+");
	}
	float closestDistance = FLT_MAX;
	DWORD_PTR closestPawn = NULL;
	Uworld = read<DWORD_PTR>(base_address + 0xEE5A148);
	//std::cout << "Uworld:" << std::hex << Uworld << std::endl;

	DWORD_PTR Gameinstance = read<DWORD_PTR>(Uworld + 0x1b8);
	//std::cout << "Gameinstance:" << std::hex << Gameinstance << std::endl;

	DWORD_PTR LocalPlayers = read<DWORD_PTR>(Gameinstance + OFFSETS::LocalPlayers);
	//	std::cout << "LocalPlayers:" << std::hex << LocalPlayers << std::endl;

	Localplayer = read<DWORD_PTR>(LocalPlayers);
	PlayerController = read<DWORD_PTR>(Localplayer + 0x30);
	LocalPawn = read<DWORD_PTR>(PlayerController + 0x330);
	PlayerState = read<DWORD_PTR>(LocalPawn + OFFSETS::PlayerState);
	Rootcomp = read<DWORD_PTR>(LocalPawn + OFFSETS::RootComponet);
	Persistentlevel = read<DWORD_PTR>(Uworld + OFFSETS::PersistentLevel);
	uintptr_t Crrneytwep = read<uintptr_t>(LocalPawn + 0x8d8);
	DWORD ActorCount = read<DWORD>(Persistentlevel + OFFSETS::ActorCount);
	DWORD_PTR AActors = read<DWORD_PTR>(Persistentlevel + OFFSETS::AActor);
	auto bIsReloadingWeapon = read<bool>(Crrneytwep + 0x329);
	DWORD_PTR GameState = read<DWORD_PTR>(Uworld + 0x158);//gamestate
	DWORD_PTR PlayerArray = read<DWORD_PTR>(GameState + 0x2A0);//playerarray
	//std::cout << "ActorCount:" << std::hex << ActorCount << std::endl;
	//std::cout << "AActors:" << std::hex << AActors << std::endl;
	//std::cout << "Crrneytwep:" << std::hex << Crrneytwep << std::endl;
	//std::cout << "GameState:" << std::hex << GameState << std::endl;
	//std::cout << "PlayerArray:" << std::hex << PlayerArray << std::endl;

	for (int i = 0; i < ActorCount; ++i)
	{
		bool rotating = 0;
		int value;

		bool lobby;

		DWORD_PTR CurrentActor = read<DWORD_PTR>(AActors + i * 0x8);
		int CurrentActorId = read<int>(CurrentActor + 0x18);
		//std::cout << "CurrentActorId:" << std::hex << CurrentActorId << std::endl;
//
		float player_check = read<float>(CurrentActor + 0x4438);
		//std::cout << "player_check:" << std::hex << player_check << std::endl;

		if (player_check == 10)
		{
			uint64_t CurrentActorMesh = read<uint64_t>(CurrentActor + 0x310);
			int curactorid = read<int>(CurrentActor + 0x18);

			auto player = read<uintptr_t>(PlayerArray + i * 0x8);
			//std::cout << "player:" << std::hex << player << std::endl;

			int CurrentActorId = read<int>(CurrentActor + 0x18);

			if (!LocalPawn) lobby = true;
			else lobby = false;

			uintptr_t Dying = read<char>(CurrentActor + 0x708);
			//	uint64_t CurrentActorMesh = read<uint64_t>(CurrentActor + OFFSETS::Mesh);
			int MyTeamId = read<int>(PlayerState + OFFSETS::TeamId);
			//std::cout << "0x" << std::hex << MyTeamId << std::endl;
			DWORD64 otherPlayerState = read<uint64_t>(CurrentActor + 0x290);
			int ActorTeamId = read<int>(otherPlayerState + OFFSETS::TeamId);
			uintptr_t currentgun = read<uintptr_t>(CurrentActor + 0x8d8);

			auto entityListCopy = entityList;
			//if (MyTeamId == ActorTeamId) continue;
			if (CurrentActor == LocalPawn) continue;
			Vector3 Headpos = GetBoneWithRotation(CurrentActorMesh, 106);
			Vector3 footpos = GetBoneWithRotation(CurrentActorMesh, 0);
			localactorpos = read<Vector3>(Rootcomp + 0x128);
			auto normal_head = ProjectWorldToScreen(Headpos);
			float distance = localactorpos.Distance(Headpos) / ChangerFOV;

			Vector3 bone0 = GetBoneWithRotation(CurrentActorMesh, 0);
			Vector3 bottom = ProjectWorldToScreen(bone0);
			Vector3 Headbox = ProjectWorldToScreen(Vector3(Headpos.x, Headpos.y, Headpos.z + 15));
			Vector3 Headbox2 = ProjectWorldToScreen(Vector3(Headpos.x, Headpos.y, Headpos.z + 20));
			Vector3 HeadElvar = ProjectWorldToScreen(Vector3(Headpos.x, Headpos.y, Headpos.z));
			Vector3 FeetElvar = ProjectWorldToScreen(Vector3(footpos.x, footpos.y, footpos.z));
			Vector3 w2shead = ProjectWorldToScreen(Headpos);
			Vector3 vHeadBone = GetBoneWithRotation(CurrentActorMesh, 106);
			Vector3 vRootBone = GetBoneWithRotation(CurrentActorMesh, 0);
			Vector3 vHeadBoneOut = ProjectWorldToScreen(Vector3(vHeadBone.x, vHeadBone.y, vHeadBone.z + 15));
			Vector3 vRootBoneOut = ProjectWorldToScreen(vRootBone);

			//	std::cout << "inside";

			float BoxHeight = (float)(Headbox.y - bottom.y);
			float BoxWidth = BoxHeight * 0.50f;

			float LeftX = (float)Headbox.x - (BoxWidth / 1);
			float LeftY = (float)bottom.y;

			float CornerHeight = abs(Headbox.y - bottom.y);
			float CornerWidth = CornerHeight * BoxWidthValue;

			bool rotating = 0;
			int value;

			auto fname = ReadGetNameFromFName(CurrentActorId);
			FString PlayerName = read<FString>(player + 0x2F0);
			//FString PlayerName = read<FString>(player + 0x2F0);
			if (spinbot)
			{
				ImDrawList* overlay_draw_list = ImGui::GetOverlayDrawList();
				overlay_draw_list->AddText(ImVec2(0.0f, 120.0f), ImColor(255, 0, 0, 255), "Spinbot");

				if (!rotating)
				{
					uint64_t PlayerMesh = read<uint64_t>(LocalPawn + OFFSETS::Mesh);
					value = value + 100;
					write<Vector3>(PlayerMesh + 0x140, Vector3(1, value, 1));
					rotating = 1;
				}
				else if (rotating)
				{
					uint64_t PlayerMesh = read<uint64_t>(LocalPawn + 0x2F0);
					write<Vector3>(PlayerMesh + 0x140, Vector3(0, -90, 0));
					rotating = 0;
				}
			}


			if (distance < VisDist)
			{
				if (Esp)
				{

					if (!lobby)
					{

						if (DrawEnemyFov)
						{
							ImU32 FovVisCheck;
							bool TooClose = false;
							float ranged = localactorpos.Distance(vHeadBone) / 10.f;
							FovVisCheck = ImGui::GetColorU32({ 0, 0, 0, 255 });

							int VisualSize;
							VisualSize = distance;

							uintptr_t ClosestActor = closestPawn + CurrentActorMesh;
							Vector3 HitBox = GetBoneWithRotation(ClosestActor, 106);
							Vector3 HitBoxPos = ProjectWorldToScreen(Vector3(HitBox.x, HitBox.y, HitBox.z));
							ImGui::GetOverlayDrawList()->AddCircle(ImVec2(HitBoxPos.x, HitBoxPos.y), VisualSize, FovVisCheck, 200, 1.0f);
						}

						if (ThreeDBox) {

							ImU32 ESPSkeleton = ImColor(0, 0, 0, 255);

							Vector3 bottom1 = ProjectWorldToScreen(Vector3(vRootBone.x + 40, vRootBone.y - 40, vRootBone.z));
							Vector3 bottom2 = ProjectWorldToScreen(Vector3(vRootBone.x - 40, vRootBone.y - 40, vRootBone.z));
							Vector3 bottom3 = ProjectWorldToScreen(Vector3(vRootBone.x - 40, vRootBone.y + 40, vRootBone.z));
							Vector3 bottom4 = ProjectWorldToScreen(Vector3(vRootBone.x + 40, vRootBone.y + 40, vRootBone.z));

							Vector3 top1 = ProjectWorldToScreen(Vector3(vHeadBone.x + 40, vHeadBone.y - 40, vHeadBone.z + 15));
							Vector3 top2 = ProjectWorldToScreen(Vector3(vHeadBone.x - 40, vHeadBone.y - 40, vHeadBone.z + 15));
							Vector3 top3 = ProjectWorldToScreen(Vector3(vHeadBone.x - 40, vHeadBone.y + 40, vHeadBone.z + 15));
							Vector3 top4 = ProjectWorldToScreen(Vector3(vHeadBone.x + 40, vHeadBone.y + 40, vHeadBone.z + 15));

							ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom1.x, bottom1.y), ImVec2(top1.x, top1.y), ESPSkeleton, 2.0f);
							ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom2.x, bottom2.y), ImVec2(top2.x, top2.y), ESPSkeleton, 2.0f);
							ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom3.x, bottom3.y), ImVec2(top3.x, top3.y), ESPSkeleton, 2.0f);
							ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom4.x, bottom4.y), ImVec2(top4.x, top4.y), ESPSkeleton, 2.0f);

							ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom1.x, bottom1.y), ImVec2(bottom2.x, bottom2.y), ESPSkeleton, 2.0f);
							ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom2.x, bottom2.y), ImVec2(bottom3.x, bottom3.y), ESPSkeleton, 2.0f);
							ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom3.x, bottom3.y), ImVec2(bottom4.x, bottom4.y), ESPSkeleton, 2.0f);
							ImGui::GetOverlayDrawList()->AddLine(ImVec2(bottom4.x, bottom4.y), ImVec2(bottom1.x, bottom1.y), ESPSkeleton, 2.0f);

							ImGui::GetOverlayDrawList()->AddLine(ImVec2(top1.x, top1.y), ImVec2(top2.x, top2.y), ESPSkeleton, 2.0f);
							ImGui::GetOverlayDrawList()->AddLine(ImVec2(top2.x, top2.y), ImVec2(top3.x, top3.y), ESPSkeleton, 2.0f);
							ImGui::GetOverlayDrawList()->AddLine(ImVec2(top3.x, top3.y), ImVec2(top4.x, top4.y), ESPSkeleton, 2.0f);
							ImGui::GetOverlayDrawList()->AddLine(ImVec2(top4.x, top4.y), ImVec2(top1.x, top1.y), ESPSkeleton, 2.0f);
						}

						/*
						if (dbno_check != 0.f or Dying)
						{
							char name[64];
							sprintf_s(name, "");
							DrawString(20, Headbox.x, Headbox.y - 40, &Col.red, true, true, name);
						}
						*/








						auto LAmmoCount = read<int>(currentgun + 0xb64);



						if (fillbox) { DrawFilledRect(Headbox.x - (CornerWidth / 2), Headbox.y, CornerWidth, CornerHeight, ImColor(0, 0, 0, 150)); }
						if (Esp_box) { DrawCorneredBox(Headbox.x - (CornerWidth / 2), Headbox.y, CornerWidth, CornerHeight, ImColor(0, 0, 0, 255), 1.5f); }
						if (Esp_fbox) { DrawBox(Headbox.x - (CornerWidth / 2), Headbox.y, CornerWidth, CornerHeight, ImColor(0, 0, 0, 255), 1.5f); }

						if (Esp_line)
						{
							Vector3 SnaplinePos = GetBoneWithRotation(CurrentActorMesh, hitbox);
							Vector3 W2SMOMENTO = ProjectWorldToScreen(SnaplinePos);
							DrawLine(Width / 2, Height / 2 - 2, W2SMOMENTO.x, W2SMOMENTO.y, ImColor(255, 255, 255, 255), 1.5f);
						}

						if (Esp_Skeleton)
						{
							Vector3 Head = GetBoneWithRotation(CurrentActorMesh, 106);
							Vector3 Chest = GetBoneWithRotation(CurrentActorMesh, 7);
							Vector3 Pelvis = GetBoneWithRotation(CurrentActorMesh, 2);
							Vector3 vHead = ProjectWorldToScreen(GetLoc(Head));
							Vector3 vChest = ProjectWorldToScreen(GetLoc(Chest));
							Vector3 vPelvis = ProjectWorldToScreen(GetLoc(Pelvis));

							Vector3 LeftHIP = GetBoneWithRotation(CurrentActorMesh, 78);
							Vector3 LeftLeg1 = GetBoneWithRotation(CurrentActorMesh, 84);
							Vector3 LeftLeg2 = GetBoneWithRotation(CurrentActorMesh, 79);
							Vector3 LeftLeg3 = GetBoneWithRotation(CurrentActorMesh, 81);
							Vector3 LeftLeg4 = GetBoneWithRotation(CurrentActorMesh, 80);
							Vector3 vLeftHIP = ProjectWorldToScreen(GetLoc(LeftHIP));
							Vector3 vLeftLeg1 = ProjectWorldToScreen(GetLoc(LeftLeg1));
							Vector3 vLeftLeg2 = ProjectWorldToScreen(GetLoc(LeftLeg2));
							Vector3 vLeftLeg3 = ProjectWorldToScreen(GetLoc(LeftLeg3));
							Vector3 vLeftLeg4 = ProjectWorldToScreen(GetLoc(LeftLeg4));

							Vector3 RightHIP = GetBoneWithRotation(CurrentActorMesh, 71);
							Vector3 RightLeg1 = GetBoneWithRotation(CurrentActorMesh, 77);
							Vector3 RightLeg2 = GetBoneWithRotation(CurrentActorMesh, 72);
							Vector3 RightLeg3 = GetBoneWithRotation(CurrentActorMesh, 74);
							Vector3 RightLeg4 = GetBoneWithRotation(CurrentActorMesh, 73);
							Vector3 vRightHIP = ProjectWorldToScreen(GetLoc(RightHIP));
							Vector3 vRightLeg1 = ProjectWorldToScreen(GetLoc(RightLeg1));
							Vector3 vRightLeg2 = ProjectWorldToScreen(GetLoc(RightLeg2));
							Vector3 vRightLeg3 = ProjectWorldToScreen(GetLoc(RightLeg3));
							Vector3 vRightLeg4 = ProjectWorldToScreen(GetLoc(RightLeg4));

							Vector3 RightArm1 = GetBoneWithRotation(CurrentActorMesh, 95);
							Vector3 RightArm2 = GetBoneWithRotation(CurrentActorMesh, 36);
							Vector3 RightArm3 = GetBoneWithRotation(CurrentActorMesh, 10);
							Vector3 RightArm4 = GetBoneWithRotation(CurrentActorMesh, 34);
							Vector3 vRightArm1 = ProjectWorldToScreen(GetLoc(RightArm1));
							Vector3 vRightArm2 = ProjectWorldToScreen(GetLoc(RightArm2));
							Vector3 vRightArm3 = ProjectWorldToScreen(GetLoc(RightArm3));
							Vector3 vRightArm4 = ProjectWorldToScreen(GetLoc(RightArm4));

							Vector3 LeftArm1 = GetBoneWithRotation(CurrentActorMesh, 60);
							Vector3 LeftArm2 = GetBoneWithRotation(CurrentActorMesh, 65);
							Vector3 LeftArm3 = GetBoneWithRotation(CurrentActorMesh, 39);
							Vector3 LeftArm4 = GetBoneWithRotation(CurrentActorMesh, 63);
							Vector3 vLeftArm1 = ProjectWorldToScreen(GetLoc(LeftArm1));
							Vector3 vLeftArm2 = ProjectWorldToScreen(GetLoc(LeftArm2));
							Vector3 vLeftArm3 = ProjectWorldToScreen(GetLoc(LeftArm3));
							Vector3 vLeftArm4 = ProjectWorldToScreen(GetLoc(LeftArm4));

							ImU32 Skeleton = ImColor(0, 0, 0, 255);

							DrawLine(vHead.x, vHead.y, vPelvis.x, vPelvis.y, Skeleton, 2.5f);
							DrawLine(vPelvis.x, vPelvis.y, vLeftHIP.x, vLeftHIP.y, Skeleton, 2.5f);
							DrawLine(vLeftHIP.x, vLeftHIP.y, vLeftLeg1.x, vLeftLeg1.y, Skeleton, 2.5f);
							DrawLine(vLeftLeg1.x, vLeftLeg1.y, vLeftLeg2.x, vLeftLeg2.y, Skeleton, 2.5f);
							DrawLine(vLeftLeg2.x, vLeftLeg2.y, vLeftLeg3.x, vLeftLeg3.y, Skeleton, 2.5f);
							DrawLine(vLeftLeg3.x, vLeftLeg3.y, vLeftLeg4.x, vLeftLeg4.y, Skeleton, 2.5f);

							DrawLine(vPelvis.x, vPelvis.y, vRightHIP.x, vRightHIP.y, Skeleton, 2.5f);
							DrawLine(vRightHIP.x, vRightHIP.y, vRightLeg1.x, vRightLeg1.y, Skeleton, 2.5f);
							DrawLine(vRightLeg1.x, vRightLeg1.y, vRightLeg2.x, vRightLeg2.y, Skeleton, 2.5f);
							DrawLine(vRightLeg2.x, vRightLeg2.y, vRightLeg3.x, vRightLeg3.y, Skeleton, 2.5f);
							DrawLine(vRightLeg3.x, vRightLeg3.y, vRightLeg4.x, vRightLeg4.y, Skeleton, 2.5f);

							DrawLine(vChest.x, vChest.y, vLeftArm1.x, vLeftArm1.y, Skeleton, 2.5f);
							DrawLine(vLeftArm1.x, vLeftArm1.y, vLeftArm2.x - 1, vLeftArm2.y - 1, Skeleton, 2.5f);
							DrawLine(vLeftArm2.x, vLeftArm2.y, vLeftArm3.x - 1, vLeftArm3.y - 1, Skeleton, 2.5f);
							DrawLine(vLeftArm3.x, vLeftArm3.y, vLeftArm4.x - 1, vLeftArm4.y - 1, Skeleton, 2.5f);

							//DrawLine(vChest.x, vChest.y, vRightArm1.x, vRightArm1.y, Skeleton, 2.5f);
							//DrawLine(vRightArm1.x, vRightArm1.y, vRightArm2.x - 1, vRightArm2.y - 1, Skeleton, 2.5f);
							//DrawLine(vRightArm2.x, vRightArm2.y, vRightArm3.x - 1, vRightArm3.y - 1, Skeleton, 2.5f);
							//DrawLine(vRightArm3.x, vRightArm3.y, vRightArm4.x - 1, vRightArm4.y - 1, Skeleton, 2.5f);
						}
					}
				}
			}

			if (instareload)
			{
				ImDrawList* overlay_draw_list = ImGui::GetOverlayDrawList();
				overlay_draw_list->AddText(ImVec2(0.0f, 105.0f), ImColor(255, 0, 0, 255), "Instant Reload");
				uintptr_t worldsettings = read<uint64_t>(Persistentlevel + 0x298);
				uintptr_t CurrentWeapon = read<uintptr_t>(LocalPawn + 0x8d8);
				uintptr_t SimcraftsTwoPoint5Hours1 = read<uintptr_t>(CurrentWeapon + 0xc41);
				uintptr_t SimcraftsTwoPoint5Hours2 = read<uintptr_t>(SimcraftsTwoPoint5Hours1 + 0x1678);
				uintptr_t SimcraftsTwoPoint5Hours3 = read<uintptr_t>(SimcraftsTwoPoint5Hours2 + 0x6233);
				uintptr_t SimcraftsTwoPoint5Hours4 = read<uintptr_t>(SimcraftsTwoPoint5Hours3 + 0xc87);
				uintptr_t SimcraftsTwoPoint5Hours5 = read<uintptr_t>(SimcraftsTwoPoint5Hours4 + 0xb39);
				uintptr_t SimcraftsTwoPoint5Hours6 = read<uintptr_t>(SimcraftsTwoPoint5Hours5 + 0x267);
				uintptr_t SimcraftsTwoPoint5Hours7 = read<uintptr_t>(SimcraftsTwoPoint5Hours6 + 0x5cc);
				uintptr_t SimcraftsTwoPoint5Hours8 = read<uintptr_t>(SimcraftsTwoPoint5Hours7 + 0xc82 + 0x8 + 0x18);

				write<char>(SimcraftsTwoPoint5Hours8 + 0x9c8, 0);
				write<float>(SimcraftsTwoPoint5Hours8 + 0x928, 0.01);

				bool cum = read<bool>(CurrentWeapon + 0x329);

				if (cum) {
					write<float>(worldsettings + 0x3C0, 70);
				}
				else {
					write<float>(worldsettings + 0x3C0, 1);
				}
			}

			if (Skeleton)
			{
				float a = 0;
				float b = 0;
				uintptr_t CurrentWeapon = read<uintptr_t>(PlayerState + 0x8d8);
				if (CurrentWeapon) {
					a = read<float>(CurrentWeapon + 0xab0); //LastFireTime Offset
					b = read<float>(CurrentWeapon + 0xab4); //LastFireTimeVerified Offset
					write<float>(CurrentWeapon + 0xab0, a + b - 0.003f); //LastFireTime Offset
				}
			}

			if (RapidFire)
			{
				uintptr_t DMR = read<uintptr_t>(base_address + 0x2883A10);
				uintptr_t DMRBuff1 = read<uintptr_t>(DMR + 0xDA);
				uintptr_t DMRBuff2 = read<uintptr_t>(DMRBuff1 + 0x3A);
				uintptr_t DMRBuff3 = read<uintptr_t>(DMRBuff2 + 0x67);
				uintptr_t DMRBuff4 = read<uintptr_t>(DMRBuff3 + 0x26);
				uintptr_t DMRBuff5 = read<uintptr_t>(DMRBuff4 + 0x7EA);

				write<float>(DMRBuff5 + 0x26, 0.003f);//troppo veloce
			}






			if (guntracers)
			{
				ImDrawList* overlay_draw_list = ImGui::GetOverlayDrawList();
				overlay_draw_list->AddText(ImVec2(0.0f, 75.0f), ImColor(255, 0, 0, 255), "Guntracers");
				uintptr_t CurrentWeapon = read<uintptr_t>(LocalPawn + 0x8d8);
				uintptr_t EquitmentAnimitititi = read<uintptr_t>(CurrentWeapon + 0x328);

				if (bIsReloadingWeapon) {
					write<bool>(CurrentWeapon + 0x329, false);
				}

				if (EquitmentAnimitititi) {
					write<bool>(CurrentWeapon + 0x328, false);
					write<bool>(CurrentWeapon + 0x32b, true);
				}

				if (Spoofed_GetAsyncKeyState_Spoofed(VK_LBUTTON)) {
					write<float>(CurrentWeapon + 0x64, 3.402823466e+38F);
					write<float>(CurrentWeapon + 0x64, 99);
				}
				else
					write<float>(CurrentWeapon + 0x64, 1);
			}

			if (triggerbot)
			{
				ImDrawList* overlay_draw_list = ImGui::GetOverlayDrawList();
				overlay_draw_list->AddText(ImVec2(0.0f, 60.0f), ImColor(255, 0, 0, 255), "Triggerbot");
				uint64_t currentactormesh = read<uint64_t>(closestPawn + OFFSETS::Mesh);
				auto hitbone = GetBoneWithRotation(currentactormesh, hitbox);
				uint64_t currentaimbotmesh = read<uint64_t>(currentactormesh + hitbox);

				if (distance < AimFOV)
				{
					if (closestPawn != 0)
					{
						if (hitbone.x != 0 || hitbone.y != 0 || hitbone.z != 0)
						{
							PostMessage(hwnd, VK_LBUTTON, 0x45, KEYEVENTF_EXTENDEDKEY);
						}
						PostMessage(hwnd, VK_LBUTTON, 0x45, KEYEVENTF_KEYUP);
					}
					PostMessage(hwnd, VK_LBUTTON, 0x45, KEYEVENTF_KEYUP);
				}
				PostMessage(hwnd, VK_LBUTTON, 0x45, KEYEVENTF_KEYUP);
			}

			auto dx = w2shead.x - (Width / 2);
			auto dy = w2shead.y - (Height / 2);
			auto dist = sqrtf(dx * dx + dy * dy);
			if (isVisible(CurrentActorMesh)) {
				if (dist < AimFOV && dist < closestDistance) {
					closestDistance = dist;
					closestPawn = CurrentActor;
				}
			}
		}

		if (Prediction) {
			ImDrawList* overlay_draw_list = ImGui::GetOverlayDrawList();
			overlay_draw_list->AddText(ImVec2(0.0f, 45.0f), ImColor(255, 0, 0, 255), "Prediction");
			uint64_t currentactormesh = read<uint64_t>(closestPawn + OFFSETS::Mesh);
			auto hitbone = GetBoneWithRotation(currentactormesh, hitbox);
			Vector3 HitboneOut = ProjectWorldToScreen(Vector3(hitbone.x, hitbone.y, hitbone.z));

			if (closestPawn != 0 && Spoofed_GetAsyncKeyState_Spoofed(VK_RBUTTON))
			{
				if (hitbone.x != 0 || hitbone.y != 0 || hitbone.z != 0)
				{
					float distance = localactorpos.Distance(hitbone) / 200;
					uint64_t CurrentActorRootComponent = read<uint64_t>(OFFSETS::CurrentActor + 0x190);
					Vector3 vellocity = read<Vector3>(CurrentActorRootComponent + OFFSETS::Velocity);
					Vector3 Predicted = AimbotCorrection(-504, 35, distance, HitboneOut, vellocity);
					Vector3 rootHeadOut = ProjectWorldToScreen(Predicted);

					aimbot(rootHeadOut.x, rootHeadOut.y);
				}
				else if (GetBoneWithRotation(currentactormesh, 0).y != 0) {
					aimbot(HitboneOut.x, HitboneOut.y);
				}
			}
		}


		float timer = 0.0f;

		if (Aimbot)
		{
			ImDrawList* overlay_draw_list = ImGui::GetOverlayDrawList();
			overlay_draw_list->AddText(ImVec2(0.0f, 30.0f), ImColor(255, 0, 0, 255), "Aimbot");

			uint64_t currentactormesh = read<uint64_t>(closestPawn + OFFSETS::Mesh);
			auto hitbone = GetBoneWithRotation(currentactormesh, hitbox);
			Vector3 HitboneOut = ProjectWorldToScreen(hitbone);
			uint64_t CLOSESTATTACK = read<uint64_t>(closestPawn + HitboneOut.x);

			if (closestPawn != 0 && Spoofed_GetAsyncKeyState_Spoofed(VK_RBUTTON))
			{
				if (HitboneOut.x != 0 || HitboneOut.y != 0 || HitboneOut.z != 0)
				{
					if (targetlines) {
						DrawLine(Width / 2, Height / 2, HitboneOut.x, HitboneOut.y, ImColor(255, 255, 255, 255), 0.5f);
					}
					aimbot(HitboneOut.x, HitboneOut.y);


				}
			}
		}


		if (lobby)
		{
			if (Esp_box or Esp_fbox) {
			}
			if (fillbox)
			{
				LobbyFilled(847, 60, 63, 125, ImColor(0, 0, 0, 150));
			}

			if (Esp_line) {
				LobbyLine(1920, 1080, 1920, 1080, ImColor(0, 0, 0, 255), 25);
			}
		}
	}
}
MSG Message = { NULL };
int Loop = 0;
void render() {
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	if (ShowMenu)
	{
		XorS(box_esp, "Box");
		XorS(snapline, "Snapline");
		XorS(es5, "Max Visuals Distance");
		XorS(aim1, "Aimbot");
		XorS(aim2, "Aimbot Key");
		XorS(aim3, "Aimbot Bone");
		XorS(aim6, "Aimbot FOV");
		XorS(smoothh, "Smooth"); // daca le scoti iti da crash 
		static ImVec4 inactive = ImColor(0, 136, 255, 255);
		static ImVec4 symbol = ImColor(0.150f, 0.361f, 0.696f, 1.000f);

		static int tabs = 0;
		ImVec2 cursor_pos = ImGui::GetCursorScreenPos();


		ImGui::SetNextWindowPos(cursor_pos);
		ImGui::SetNextWindowSize({ 450, 520 });
		ImGui::Begin(("pro menu!!!!"), 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar);
		ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeNS);
		ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 128));
		ImGui::Checkbox("Aimbot", &Aimbot);

		ImGui::Spacing();
		ImGui::Checkbox("Corner Box", &Esp_box);
		ImGui::Checkbox("Outlined Box", &Esp_fbox);
		ImGui::Checkbox("3D Box", &ThreeDBox);
		ImGui::Checkbox("Filled box", &fillbox);
		ImGui::Checkbox("Snaplines", &Esp_line);
		ImGui::Checkbox("Skeleton Esp", &Esp_Skeleton);
		ImGui::Checkbox("Distance Esp", &Esp_Distance);
		ImGui::Spacing();
		ImGui::Checkbox("BulletTP", &cartp);
		ImGui::SliderInt("Fov Size", &smooth, 3, 10);
		ImGui::PopStyleColor();
		ImGui::End();
	}
	DrawESP();

	ImGui::EndFrame();
	D3dDevice->SetRenderState(D3DRS_ZENABLE, false);
	D3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	D3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, false);
	D3dDevice->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

	if (D3dDevice->BeginScene() >= 0)
	{
		ImGui::Render();
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		D3dDevice->EndScene();
	}
	HRESULT result = D3dDevice->Present(NULL, NULL, NULL, NULL);

	if (result == D3DERR_DEVICELOST && D3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
	{
		ImGui_ImplDX9_InvalidateDeviceObjects();
		D3dDevice->Reset(&d3dpp);
		ImGui_ImplDX9_CreateDeviceObjects();
	}
}

void mainloop()
{
	static RECT old_rc;
	ZeroMemory(&Message, sizeof(MSG));

	while (Message.message != WM_QUIT)
	{
		if (PeekMessage(&Message, Window, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&Message);
			DispatchMessage(&Message);
		}

		HWND hwnd_active = GetForegroundWindow();

		if (hwnd_active == hwnd) {
			HWND hwndtest = GetWindow(hwnd_active, GW_HWNDPREV);
			SetWindowPos(Window, hwndtest, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		}

		if (Spoofed_GetAsyncKeyState_Spoofed(0x23) & 1)
			exit(8);

		RECT rc;
		POINT xy;

		ZeroMemory(&rc, sizeof(RECT));
		ZeroMemory(&xy, sizeof(POINT));
		GetClientRect(hwnd, &rc);
		ClientToScreen(hwnd, &xy);
		rc.left = xy.x;
		rc.top = xy.y;

		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = NULL;
		io.ImeWindowHandle = hwnd;
		io.DeltaTime = 1.0f / 60.0f;

		POINT p;
		GetCursorPos(&p);
		io.MousePos.x = p.x - xy.x;
		io.MousePos.y = p.y - xy.y;

		if (Spoofed_GetAsyncKeyState_Spoofed(VK_LBUTTON)) {
			io.MouseDown[0] = true;
			io.MouseClicked[0] = true;
			io.MouseClickedPos[0].x = io.MousePos.x;
			io.MouseClickedPos[0].x = io.MousePos.y;
		}
		else
			io.MouseDown[0] = false;

		if (rc.left != old_rc.left || rc.right != old_rc.right || rc.top != old_rc.top || rc.bottom != old_rc.bottom)
		{
			old_rc = rc;

			Width = rc.right;
			Height = rc.bottom;

			d3dpp.BackBufferWidth = Width;
			d3dpp.BackBufferHeight = Height;
			SetWindowPos(Window, (HWND)0, xy.x, xy.y, Width, Height, SWP_NOREDRAW);
			D3dDevice->Reset(&d3dpp);
		}
		render();
		if (Loop == 0) {
			XorS(base, "Process base address: %p.\n");
			//printf(base.decrypt(), (void*)base_address);
		}
		Loop = Loop + 1;
	}
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	DestroyWindow(Window);
}
int main(int argc, const char* argv[])
{
	
	HANDLE hpStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	//SetConsoleTitleA("You can close notepad!");
	CreateThread(NULL, NULL, Menuthread, NULL, NULL, NULL);
	//std::rename(path().c_str(), random_string(15).c_str());
	COLOR(hpStdout, color4);
	//system(skCrypt("taskkill /f /im EpicGamesLauncher.exe >nul 2>&1"));
	//system(skCrypt("taskkill /f /im FortniteClient-Win64-Shipping.exe >nul 2>&1"));
	HANDLE iqvw64e_device_handle;



	system("cls");
	//	BlockInput(true);

		//TODO: download in bytes, map it.

	//BlockInput(false);
	Sleep(500);
	system("cls");
	Sleep(500);
	time_t t = time(NULL);
	struct tm tm = *localtime(&t);
	printf("[%d-%d-%d] - Waiting for Fortnite...\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
	while (hwnd == NULL)
	{
		XorS(wind, "Fortnite  ");
		hwnd = FindWindowA(0, wind.decrypt());

		Sleep(100);
	}
	system("cls");
     //Here you just startt initiating the driver.
	if (!mem::find_driver())
	{
		std::cout << ("Driver not Found, please restart PC/MAP again.\n");
		abed();
	}
	else if (mem::find_driver()) {
		std::cout << ("Driver Loaded\n");
	}
	processID = _GetProcessId("FortniteClient-Win64-Shipping.exe");
	if (!mem::find_process(("FortniteClient-Win64-Shipping.exe"))) {
		system(("cls"));
		MessageBoxA(NULL, ("failed to load, please contact the owner of the cheat (the cheat will continue but will not work)."), NULL, NULL);
	}
	base_address = mem::find_image();

	std::cout << "[+] Base Address: " << std::hex << base_address << std::endl;

	InjectMouseInput = reinterpret_cast<InjectMouseInput_t>(GetProcAddress(LoadLibraryA("user32.dll"), "InjectMouseInput"));

	CreateWindows();
	xInitD3d();

	mainloop();
	Shutdown();

	return 0;
}