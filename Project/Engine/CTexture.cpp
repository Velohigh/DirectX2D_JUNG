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
	// #1 : ���� ��ü ���,  #2 : ����̺� ��� (C: D:) , #3 : ���� ���͸� ������
	// #4 : ���� �̸�, #5 : Ȯ���� (.bmp, .jpg)
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
		MessageBox(nullptr, L"���ҽ� �ε� ����", L"�ؽ��� �ε� ����", MB_OK);
		return E_FAIL;
	}

	// Image�� -> �ý��� �޸� -> Desc �ۼ� -> Tex2D ũ������Ʈ -> Tex2D�� SRV(Shader Resource View) �� ���ߵǴµ�
	// �ش� ������ �Լ��ϳ��� ó�����ش�. ����, View ���� �������� Desc, Tex2D �� �˾ƿ´�.
	hr = CreateShaderResourceView(DEVICE
		, m_Image.GetImages()
		, m_Image.GetImageCount()
		, m_Image.GetMetadata()
		, m_SRV.GetAddressOf());

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"ShaderResourceView ���� ����", L"�ؽ��� �ε� ����", MB_OK);
		return E_FAIL;
	}

	// View���� Tex2D ������ ���´�.
	m_SRV->GetResource((ID3D11Resource**)m_Tex2D.GetAddressOf());
	m_Tex2D->GetDesc(&m_Desc);	// Desc�� Tex2D���� ���´�.


	return S_OK;
}

int CTexture::Save(const wstring& _strRelativePath)
{
	return S_OK;
}
