#pragma once
#include "..\Cube\cCube.h"

class cBoundingBox : public cCube
{
private :

	
	SYNTHESIZE_REF(D3DXMATRIX,m_matWorld, World);
	SYNTHESIZE_REF(cCube*, m_pBoundBox, BoundingBox);

public:
	cBoundingBox();
	~cBoundingBox();

	D3DXVECTOR3					m_vMinPos; 
	D3DXVECTOR3					m_vMaxPos;

	D3DXVECTOR3					m_vPosition;  //lsc
	D3DXVECTOR3					m_vRoatation;

	bool			m_bIsPicked; // 선택된Box 시각효과 표시.

	void Setup(D3DXVECTOR3 min, D3DXVECTOR3 max);
	void Update();
	void Render();

	ST_OBB GetOBB();

};

