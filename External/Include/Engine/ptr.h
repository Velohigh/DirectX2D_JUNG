#pragma once

// T 타입을 가리키는 스마트 포인터
template<typename T>
class Ptr
{
private:
	T*	m_Res;

public:
	T* Get() const { return m_Res; }

	T* operator -> ()
	{
		return m_Res;
	}

	void operator = (T* _Res)
	{
		if (nullptr != m_Res)
			m_Res->Release();

		m_Res = _Res;
		
		if (nullptr != m_Res)
			m_Res->AddRef();
	}

	void operator = (const Ptr<T>& _Res)	// 다른 스마트 포인터가 들어온경우
	{
		if (nullptr != m_Res)
			m_Res->Release();

		m_Res = _Res.m_Res;

		if (nullptr != m_Res)
			m_Res->AddRef();
	}

	bool operator == (T* _Other)
	{
		return m_Res == _Other;
	}

	bool operator != (T* _Other)
	{
		return m_Res != _Other;
	}

	bool operator == (const Ptr<T>& _Other)
	{
		return m_Res == _Other.m_Res;
	}

	bool operator != (const Ptr<T>& _Other)
	{
		return m_Res != _Other.m_Res;
	}



public:
	Ptr()
		: m_Res(nullptr)
	{

	}

	Ptr(T* _Res)
		: m_Res(_Res)
	{
		if (nullptr != m_Res)
			m_Res->AddRef();
	}

	Ptr(const Ptr<T>& _Res)
		: m_Res(_Res.m_Res)
	{
		if (nullptr != m_Res)
			m_Res->AddRef();
	} 
	
	~Ptr()
	{
		if (nullptr != m_Res)
		{
			m_Res->Release();
		}
	}


};

// 상수 == Ptr<T> 비교 가능하도록 템플릿 전역 operator 오버로딩 추가. (ex. nullptr == Ptr<CMesh> )
template <typename T>
bool operator == (void* _Res, const Ptr<T>& _Ptr)
{
	return _Res == _Ptr.Get();
}



template <typename T>
bool operator != (void* _Res, const Ptr<T>& _Ptr)
{
	return _Res != _Ptr.Get();
}