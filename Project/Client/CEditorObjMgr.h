#pragma once

class CGameObjectEx;

class CEditorObjMgr
	: public CSingleton<CEditorObjMgr>
{
	SINGLE(CEditorObjMgr);
private:
	CGameObjectEx*			m_DebugShape[(UINT)SHAPE_TYPE::END];
	vector<tDebugShapeInfo> m_DebugShapeInfo;



public:
	void init();
	void progress();

private:
	void tick();
	void render();
};

