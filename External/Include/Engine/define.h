#pragma once

#define DEVICE	CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetDeviceContext()

#define CLONE(type) public: virtual type* Clone() { return new type(*this); }

#define KEY_TAP(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::TAP
#define KEY_RELEASE(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::RELEASE
#define KEY_PRESSED(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::PRESSED

#define DT CTimeMgr::GetInst()->GetDeltaTime()


enum class COMPONENT_TYPE
{
	TRANSFORM,

	COLLIDER2D,
	COLLIDER3D,
	ANIMATOR2D,
	ANIMATOR3D,
	LIGHT2D,
	LIGHT3D,
	CAMERA,

	// render
	MESHRENDER,
	PARTICLESYSTEM,
	TILEMAP,
	LANDSCAPE,	// 3D 지형
	DECAL,

	// custom
	SCRIPT,
	
	END,
};

enum class RES_TYPE
{
	MESHDATA,	// 나중에 3D때..
	MATERIAL,	// 재질
	PREFAB,		// 나중에

	MESH,		// 형태, 버텍스 버퍼와 인덱스 버퍼를 묶어서 메쉬
	TEXTURE,	// 이미지
	SOUND,

	GRAPHICS_SHADER,
	COMPUTE_SHADER,		// 일반적인 목적으로 쓸때, ex) 비트코인

	END,
};