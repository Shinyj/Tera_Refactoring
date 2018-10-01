#pragma once
#include "iMap.h"

class FieldMap : public iMap
{
private :
	D3DXMATRIX			m_matWorld;
	cXMesh*				m_pGround;

	ST_PNT_VERTEX*		m_pVertex;
	WORD*				m_pIndex;
	
	DWORD				numOfVertex;
	DWORD				numOfIndex;

public:
	FieldMap();
	~FieldMap();

	void Setup();
	void Update();
	void Render();

	virtual bool GetHeight(IN float x, OUT float& y, IN float z);
	void IntersectTri(OUT D3DXVECTOR3 & vPickedPosition, OUT float & ray_dis);

	//몬스터 경사로 지점에서의 기울기 처리
	void GetTerrainSlope(OUT D3DXMATRIX& out, IN float x, IN float z);
	D3DXVECTOR3 GetNormal(IN float x, IN float z);
};

