#include "pch.h"
#include "CTexture.h"

#include "CDevice.h"


CTexture::CTexture()
	: CRes(RES_TYPE::TEXTURE)
	, m_Desc{}
{
}

CTexture::~CTexture()
{
}

void CTexture::UpdateData()
{
}

void CTexture::UpdateData(int _iRegisterNum)
{
	CONTEXT->PSSetShaderResources(_iRegisterNum, 1, m_SRV.GetAddressOf());

}

int CTexture::Load(const wstring& _strFilePath)
{
	wchar_t szExt[50] = L"";
	// #1 : 파일 전체 경로,  #2 : 드라이브 경로 (C: D:) , #3 : 최종 디렉터리 폴더명
	// #4 : 파일 이름, #5 : 확장자 (.bmp, .jpg)
	_wsplitpath_s(_strFilePath.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExt, 50);
	wstring strExt = szExt;
	
	HRESULT hr = S_OK;

	// dds, DDS
	if (L".dds" == strExt || L".DDS" == strExt)
	{
		hr = LoadFromDDSFile(_strFilePath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, m_Image);
	}

	// tga, TGA
	else if (L".tga" == strExt || L".TGA" == strExt)
	{
		hr = LoadFromTGAFile(_strFilePath.c_str(), nullptr, m_Image);
	}

	// png, jpg, jpeg, bmp
	else 
	{
		hr = LoadFromWICFile(_strFilePath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, m_Image);
	}

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"리소스 로딩 실패", L"텍스쳐 로딩 실패", MB_OK);
		return E_FAIL;
	}

	// Image를 -> 시스템 메모리 -> Desc 작성 -> Tex2D 크리에이트 -> Tex2D로 SRV(Shader Resource View) 로 가야되는데
	// 해당 과정을 함수하나로 처리해준다. 따라서, View 에서 역순으로 Desc, Tex2D 를 알아온다.
	hr = CreateShaderResourceView(DEVICE
		, m_Image.GetImages()
		, m_Image.GetImageCount()
		, m_Image.GetMetadata()
		, m_SRV.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ShaderResourceView 생성 실패", L"텍스쳐 로딩 실패", MB_OK);
		return E_FAIL;
	}

	// View에서 Tex2D 정보를 얻어온다.
	m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());
	m_Tex2D->GetDesc(&m_Desc);	// Desc를 Tex2D에서 얻어온다.


	return S_OK;
}

int CTexture::Save(const wstring& _strRelativePath)
{
	return S_OK;
}
