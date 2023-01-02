#pragma once
#include "CSingleton.h"

class CLayer;
class CCollider2D;


union CollisionID
{
	struct
	{
		UINT LeftID;
		UINT RightID;
	};

	UINT_PTR id;
};


class CCollisionMgr :
	public CSingleton<CCollisionMgr>
{
	SINGLE(CCollisionMgr);
private:
	UINT					m_matrix[MAX_LAYER];
	map<UINT_PTR, bool>		m_mapColID;

public:
	void LayerCheck(UINT _left, UINT _right);
	void LayerCheck(const wstring& _strLeftLayer, const wstring& _strRightLayer);

	void Clear()
	{
		memset(m_matrix, 0, sizeof(UINT) * MAX_LAYER);
	}

public:
	void tick();

private:
	void CollisionBtwLayer(CLayer* _LeftLayer, CLayer* _RightLayer);	// 어떤 레이어, 어떤레이어가 충돌할것인지
	void CollisionBtwObject(CGameObject* _LeftObject, CGameObject* _RightObject);
	bool CollisionBtwCollider(CCollider2D* _pLeft, CCollider2D* _pRight);	// 두 충돌체가 충돌하는지 true,false
};

