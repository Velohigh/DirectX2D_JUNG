#pragma once


#define DEVICE  CDevice::GetInst()->GetDevice()
#define CONTEXT CDevice::GetInst()->GetDeviceContext()

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
	COLLIDER2D_View,		// 2차원 충돌_시야
	COLLIDER2D_Attack,		// 2차원 충돌_공격
	
	COLLIDER3D,		// 3차원 충돌
	ANIMATOR2D,		// Sprite Animation
	ANIMATOR3D,		// Bone Sknning Animation
	LIGHT2D,		// 2차원 광원
	LIGHT3D,		// 3차원 광원
	CAMERA,			// Camera

	// render
	MESHRENDER,		// 기본적인 렌더링
	PARTICLESYSTEM, // 입자 렌더링
	TILEMAP,		// 2차원 타일
	LANDSCAPE,		// 3차원 지형
	DECAL,			// 내부 렌더링

	END,

	// custom
	SCRIPT,
};

extern const char* COMPONENT_TYPE_STR[(UINT)COMPONENT_TYPE::END];
extern const wchar_t* COMPONENT_TYPE_WSTR[(UINT)COMPONENT_TYPE::END];


enum class RES_TYPE
{
	MESHDATA,
	MATERIAL,
	PREFAB,

	MESH,			// 형태
	TEXTURE,		// 이미지
	SOUND,

	GRAPHICS_SHADER,
	COMPUTE_SHADER,

	END,
};

extern const char* RES_TYPE_STR[(UINT)RES_TYPE::END];
extern const wchar_t* RES_TYPE_WSTR[(UINT)RES_TYPE::END];




enum class CB_TYPE
{
	TRANSFORM,	// b0
	MATERIAL,	// b1
	GLOBAL,		// b2
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

	TEX_CUBE_0,
	TEX_CUBE_1,

	TEX_ARR_0,
	TEX_ARR_1,

	TEX_END,
};

enum PIPELINE_STAGE
{
	PS_VERTEX = 0x01,
	PS_HULL = 0x02,
	PS_DOMAIN = 0x04,
	PS_GEOMETRY = 0x08,
	PS_PIXEL = 0x10,

	PS_ALL = PS_VERTEX | PS_HULL | PS_DOMAIN | PS_GEOMETRY | PS_PIXEL,
};

enum class RS_TYPE
{
	CULL_BACK,
	CULL_FRONT,
	CULL_NONE,
	WIRE_FRAME,
	END,
};

enum class DS_TYPE
{
	LESS,
	LESS_EQUAL,
	GREATER,
	GREATER_EQUAL,
	NO_WRITE,			// LESS, DepthWrite X
	NO_TEST_NO_WRITE,	// Test X, DepthWrite X
	END,
};


enum class BS_TYPE
{
	DEFAULT,		// No Blending
	MASK,			// Alpha Coverage
	ALPHA_BLEND,	// Alpha 계수 
	ONE_ONE,		// 1:1 혼합
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
	DOMAIN_UNDEFINED,	// 미정
};


enum class EVENT_TYPE
{
	CREATE_OBJECT,	// wParam : GameObject, lParam : Layer Index
	DELETE_OBJECT,  // wParam : GameObject

	ADD_CHILD,

	DELETE_RESOURCE,	// wParam : RES_TYPE, lParam : Resource Adress

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


enum class LIGHT_TYPE
{
	DIRECTIONAL,
	POINT,
	SPOT,
};

enum class LEVEL_STATE
{
	PLAY,
	PAUSE,
	STOP,
};

enum class SB_TYPE
{
	READ_ONLY,
	READ_WRITE,
};

enum class PARTICLE_MODULE
{
	PARTICLE_SPAWN,
	COLOR_CHANGE,
	SCALE_CHANGE,
	ADD_VELOCITY,

	DRAG,
	NOISE_FORCE,
	RENDER,
	DUMMY_3,

	END,
};

enum class ObjDir
{
	Left,
	Right,
	End
};

enum class ObjState
{
	Idle,
	Walk,
	Turn,
	Run,
	Attack,
	HurtGround,
	HurtFly,
	KnockDown,
	Dead,
	END
};

enum class BossState
{
	Idle,			// 대기
	Takeoutgun,		// 권총 꺼내는 모션
	Putbackgun,		// 권총 넣는 모션
	Shoot,			// 권총 쏘는 모션

	Takeoutrifle,	// 라이플 꺼내는 모션
	Putbackrifle,	// 라이플 넣는 모션
	Aim_rifle,		// 라이플 조준하는 모션
	
	Hurt,		// 한대맞고 한바퀴 돌면서 날아가는 모션
	Recover,	// 한대맞고나서 그로기상태에서, 사라지는 모습.
	
	Wallidle,	// 벽에 매달려서 대기
	Walljump,	// 벽에서 점프하여 지상으로 착지하는 모션
	Walljump_land,	// 착지모션

	Predash,				// 칼 꺼내서 대쉬하기직전 모션
	Dash,					// 칼꺼내고 대쉬하는 모션
	Dash_end_ground,		// 대쉬 끝나고 베는 모션

	Prejump,				// 점프 진입모션
	Jump,					// 점프

	Teleport_in,			// 라이플 아래향한채로, 나타나는 모션
	Teleport_out,			// 라이플 아래향한채로, 사라지는 모션
	Teleport_in_ground,		// 땅에서 라이플 앞 향한채로, 나타나는 모션
	Teleport_out_ground,	// 땅에서 라이플 앞 향한채로, 사라지는 모션

	Teleport_in_sweep,		// sweep 하기전에 공중에 나타나는 모션
	Sweep,					// 공중에서 총을 한바퀴 회전하면서, 총알을 흩뿌리는 모션
	Teleport_out_sweep,		// sweep 후, 공중에서 사라지는 모션

	Dodgeroll,				// 굴러서 회피 모션

	Hurtfly,				// 막타 맞았을때 날아가는 모션
	Hurtground,				// 막타 맞고 날아가다 땅에 닿았을때 모션


	End,
};
