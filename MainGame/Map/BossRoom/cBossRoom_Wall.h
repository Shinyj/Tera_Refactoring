#pragma once
#include "MainGame\Map\iMap.h"

class cBossRoom_Wall : public iMap
{
private :
	D3DXMATRIX				m_matWorld;
	ST_PNT_VERTEX*			m_pVertex;		// 버텍스 버퍼
	WORD*					m_pIndex;		// 인덱스 버퍼
	DWORD					numOfVertex;	// 버텍스 개수
	DWORD					numOfIndex;		// 인덱스 개수 


public:
	cBossRoom_Wall();
	~cBossRoom_Wall();

	cXMesh*					m_pFloor;

	vector<cBossRoom_Wall> m_vecBossRoom_Wall;
	vector<cXMesh*> m_vecWallParts;
	D3DXMATRIX matWorld;

	float rotY;

	void Setup();
	void Render();
	void Destroy();

	virtual bool GetHeight(IN float x, OUT float& y, IN float z);

};

