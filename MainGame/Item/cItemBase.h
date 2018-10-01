#pragma once
#include "MainGame/cGameObject.h"

class cXMesh;
class cBoundingBox;

class cItemBase : public cGameObject
{
protected:

	SYNTHESIZE(D3DXMATRIX, m_matWorld, World);

	char *			m_sName;
	cXMesh*			m_pBody;

	ST_PCT_VERTEX		m_arrPos[20];
	int					m_nIndex;
	float				m_fTime;


	LPDIRECT3DTEXTURE9	m_pSlashEffectTexture;

	D3DXVECTOR3			m_vOnPos;
	D3DXVECTOR3			m_vDownPos;

	vector<ST_PCT_VERTEX>	m_vecVertex;

public:
	cItemBase();
	virtual~cItemBase();

	virtual void Setup();
	virtual void Update();
	virtual void Render();

};


/*for (int i = 0; i < 18; i += 2)
		{
			m_arrPos[i + 0].t = D3DXVECTOR2(0, 0);
			m_vecVertex.push_back(m_arrPos[i + 0]);
			m_arrPos[i + 2].t = D3DXVECTOR2(0, 1);
			m_vecVertex.push_back(m_arrPos[i + 2]);
			m_arrPos[i + 1].t = D3DXVECTOR2(1, 0);
			m_vecVertex.push_back(m_arrPos[i + 1]);

			m_arrPos[i + 2].t = D3DXVECTOR2(0, 1);
			m_vecVertex.push_back(m_arrPos[i + 2]);
			m_arrPos[i + 3].t = D3DXVECTOR2(1, 1);
			m_vecVertex.push_back(m_arrPos[i + 3]);
			m_arrPos[i + 1].t = D3DXVECTOR2(1, 0);
			m_vecVertex.push_back(m_arrPos[i + 1]);
			}*/