#pragma once


#define DEVICE  CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetDeviceContext()

// return new(*this) 는 복사 생성자를 호출한다.
#define CLONE(type) public: virtual type* Clone() { return new type(*this); }
#define CLONE_DISABLE(type) public: virtual type* Clone() { return nullptr; assert(nullptr); }

#define KEY_TAP(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::TAP
#define KEY_RELEASE(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::RELEASE
#define KEY_PRESSED(Key) CKeyMgr::GetInst()->GetKeyState(Key) == KEY_STATE::PRESSED

#define DT CTimeMgr::GetInst()->GetDeltaTime()

#define MAX_LAYER 32

#define SINGLE(type) private: type(); ~type(); friend class CSingleton<type>;






enum class COMPONENT_TYPE
{
	// update
	TRANSFORM,		// 위치, 크기, 회전
	COLLIDER2D,		// 2차원 충돌
	COLLIDER3D,		// 3차원 충돌
	ANIMATOR2D,		// Sprite Animation
	ANIMATOR3D,		// Bone Sknning Animation ( 수학적인 연산으로 물체를 비틀어서)
	LIGHT2D,		// 2차원 광원
	LIGHT3D,		// 3차원 광원
	CAMERA,			// Camera

	// render
	MESHRENDER,		// 기본적인 렌더링
	PARTICLESYSTEM,	// 입자 렌더링
	TILEMAP,		// 2차원 타일
	LANDSCAPE,		// 3차원 지형
	DECAL,			// 내부 렌더링

	END,

	// custom
	SCRIPT,			// 프로그래머가 직접 만드는 기능. (ex. PlayerScript, MonsterScript)
	
};


enum class RES_TYPE
{
	MESHDATA,	// 나중에 3D때..
	MATERIAL,	// 재질
	PREFAB,		// 나중에

	MESH,		// 형태, 버텍스 버퍼와 인덱스 버퍼를 묶어서 메쉬
	TEXTURE,	// 이미지
	SOUND,

	GRAPHICS_SHADER,	// 렌더링 파이프 라인 셰이더
	COMPUTE_SHADER,		// 일반적인 목적으로 쓸때, ex) 비트코인

	END,
};

enum class CB_TYPE
{
	TRANSFORM,	// b0
	MATERIAL,	// b1

	END,
};


enum SCALAR_PARAM
{
	INT_0,
	INT_1,
	INT_2,
	INT_3,

	FLOAT_0,
	FLOAT_1,
	FLOAT_2,
	FLOAT_3,

	VEC2_0,
	VEC2_1,
	VEC2_2,
	VEC2_3,

	VEC4_0,
	VEC4_1,
	VEC4_2,
	VEC4_3,

	MAT_0,
	MAT_1,
	MAT_2,
	MAT_3,
};

enum TEX_PARAM
{
	TEX_0,
	TEX_1,
	TEX_2,
	TEX_3,
	TEX_4,
	TEX_5,
	TEX_6,
	TEX_7,

	TEX_END,
};

enum PIPELINE_STAGE
{
	PS_VERTEX = 0x00,
	PS_HULL = 0x01,
	PS_DOMAIN = 0x02,
	PS_GEOMETRY = 0x04,
	PS_PIXEL = 0x08,

	PS_ALL = PS_VERTEX | PS_HULL | PS_DOMAIN | PS_GEOMETRY | PS_PIXEL,
};

enum class RS_TYPE
{
	CULL_BACK,		// 
	CULL_FRONT,		// 시계 방향을 컬링, 반시계를 보겠다. 하늘 같은, 맵안에서 밖을 바라볼 때 사용
	CULL_NONE,		// 앞면이든 뒷면이든 컬링하지 않겠다.
	WIRE_FRAME,		// 텍스쳐가 아닌 뼈대만 확인하기 위한 옵션
	END,
};

enum class DS_TYPE
{
	LESS,				// 디폴트 옵션, 깊이 값이 낮은 걸 출력, // 이전에 누가 세팅해둔게 남아 있을 수 있으므로, nullptr 로 다시 세팅해줘야한다.
	LESS_EQUAL,			
	GREATER,			// 깊이 값이 높은 걸 출력
	GREATER_EQUAL,
	NO_WRITE,			// LESS, DepthWrite X   출력후 깊이값 저장 X
	NO_TEST_NO_WRITE,	// Test X, DepthWrite X	깊이 상관없이 무조건 출력, 저장 X
	END,
};


enum class BS_TYPE
{
	DEFAULT,		// No Blending, 출력 색상을 그대로 출력
	MASK,			// Alpha Coverage
	ALPHA_BLEND,	// Alpha 계수에 따라 출력색상 * 알파값 + 기존색상 * (1-알파값)
	ONE_ONE,		// 1:1 혼합, 보통 검은색 배경은 알파값이 없기 때문에, 검은 부분을 빼는 작업에 사용함
	END,
};





enum class DIR_TYPE
{
	RIGHT,
	UP,
	FRONT,
};

enum class PROJ_TYPE
{
	ORTHOGRAPHIC,
	PERSPECTIVE,
};

enum class SHADER_DOMAIN
{
	DOMAIN_OPAQUE,		// 불투명 오브젝트
	DOMAIN_MASK,		// 불투명, 투명
	DOMAIN_TRANSPARENT,	// 반투명
	DOMAIN_POSTPROCESS, // 후 처리
	DOMAIN_UI,
	DOMAIN_UNDEFINED,	// 미정, Domain은 무조건 분류되어야 하므로, assert로 처리해준다.
};

enum class EVENT_TYPE
{
	CREATE_OBJECT,	// wParam : GameObject, lParam : Layer Index
	DELETE_OBJECT,

	ADD_CHILD,

	DELETE_RESOURCE,

	LEVEL_CHANGE,
};


enum class SHAPE_TYPE
{
	RECT,
	CIRCLE,
	CUBE,
	SPHERE,
	END,
};


enum class COLLIDER2D_TYPE
{
	RECT,
	CIRCLE,
};