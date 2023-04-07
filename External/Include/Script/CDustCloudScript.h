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
	ObjDir		m_PreDir;	// ������ �ٶ󺸰��ִ� ����
	ObjDir		m_CurDir;	// ���� �ٶ󺸴� ����

	CLevel*		m_Level;	// ���� ����


	float		m_MoveSpeed;	// �̵��ӵ�
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

