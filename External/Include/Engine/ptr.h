#pragma once

// T 타입을 가리키는 스마트 포인터
template<typename T>
class Ptr
{
private:
	T*	m_Res;

public:
	T* Get() { return m_Res; }

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

		m_Res = _Res;

		if (nullptr != m_Res)
			m_Res->AddRef();
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
		// == 이라고 되있는 부분 != 이렇게 고쳐야하지 않을까? 싶어서 고침
		if (nullptr != m_Res)
		{
			m_Res->Release();
		}
	}


};