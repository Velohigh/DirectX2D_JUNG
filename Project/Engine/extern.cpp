#include "pch.h"
#include "global.h"

tTransform g_transform = {};
tGlobal GlobalData = {};



extern const char* RES_TYPE_STR[(UINT)RES_TYPE::END] =
{
	"MESHDATA",
	"MATERIAL",
	"PREFAB",
	"MESH",
	"TEXTURE",
	"SOUND",
	"GRAPHICS_SHADER",
	"COMPUTE_SHADER"
};

extern const wchar_t* RES_TYPE_WSTR[(UINT)RES_TYPE::END] =
{
	L"MESHDATA",
	L"MATERIAL",
	L"PREFAB",
	L"MESH",
	L"TEXTURE",
	L"SOUND",
	L"GRAPHICS_SHADER",
	L"COMPUTE_SHADER"
};