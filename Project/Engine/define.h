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
	LANDSCAPE,	// 3D ����
	DECAL,

	// custom
	SCRIPT,
	
	END,
};

enum class RES_TYPE
{
	MESHDATA,	// ���߿� 3D��..
	MATERIAL,	// ����
	PREFAB,		// ���߿�

	MESH,		// ����, ���ؽ� ���ۿ� �ε��� ���۸� ��� �޽�
	TEXTURE,	// �̹���
	SOUND,

	GRAPHICS_SHADER,
	COMPUTE_SHADER,		// �Ϲ����� �������� ����, ex) ��Ʈ����

	END,
};