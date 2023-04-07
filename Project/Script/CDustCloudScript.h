#pragma once
#include <Engine\CScript.h>



class CDustCloudScript :
	public CScript
{
	enum class ObjDir
	{
		Left,
		Right,
		End
	};

private:
	ObjDir		m_PreDir;	// 이전에 바라보고있던 방향
	ObjDir		m_CurDir;	// 현재 바라보는 방향

	CLevel*		m_Level;	// 속한 레벨


	float		m_MoveSpeed;	// 이동속도
	Vec2		m_MoveDir;
	Vec2		RandomValueX = {};


public:
	virtual void begin() override;
	virtual void tick() override;
	
private:
	void MoveDir(const Vec2& Dir);
	void MoveValue(const Vector2& MoveValue);  //Move();

public:
	void SetSpeed(float _Speed) { m_MoveSpeed = _Speed; }
	void SetDir(int _Dir) { m_CurDir = (ObjDir)_Dir; }

private:
	CLONE(CDustCloudScript);
public:
	CDustCloudScript();
	~CDustCloudScript();


private:
};

