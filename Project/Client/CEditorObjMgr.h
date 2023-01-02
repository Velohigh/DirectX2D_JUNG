#pragma once
// 에디터 용으로 만들어진 오브젝트들을 관리하는 클래스

class CGameObjectEx;

class CEditorObjMgr
	: public CSingleton<CEditorObjMgr>
{
	SINGLE(CEditorObjMgr);
private:
	vector<CGameObjectEx*> m_vecDebugShape;

public:
	void init();
	void progress();

private:
	void tick();
	void render();
};

