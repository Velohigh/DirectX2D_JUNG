#pragma once

typedef void (*EXIT)(void);

template<typename T>
class CSingleton
{
private:
	static T* m_Inst;

public:
	static T* GetInst();
	static void Destroy();

protected:
	CSingleton() { }
	virtual ~CSingleton() { }
};

template<typename T>
T* CSingleton<T>::m_Inst = nullptr;

template<typename T>
inline T* CSingleton<T>::GetInst()
{
	if (nullptr == m_Inst)
	{
		m_Inst = new T;
		atexit( (EXIT) &CSingleton<T>::Destroy);	// atexit : 인자로 함수 포인터를 받아서, 메인 함수가 끝날때 해당 함수를 호출해준다.
	}

	return m_Inst;
}

template<typename T>
inline void CSingleton<T>::Destroy()
{
	if (nullptr != m_Inst)
	{
		delete m_Inst;
		m_Inst = nullptr;
	}
}