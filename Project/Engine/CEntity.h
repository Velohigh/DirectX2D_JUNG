#pragma once

class CEntity
{
private:
	static UINT	g_iNextID;

private:
	wstring		m_strName;		// 이름
	const UINT	m_iID;			// 객체 고유 번호


public:
	void SetName(const wstring& _strName) {	m_strName = _strName; }
	const wstring& GetName() { return m_strName; }
	UINT GetID() { return m_iID; }

public:
	virtual CEntity* Clone() = 0;

public:
	CEntity();
	CEntity(const CEntity& _other);
	virtual ~CEntity();

};

