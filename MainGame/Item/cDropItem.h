#pragma once
#include "../cGameObject.h"

class cSprite;
class iMap;
class cItemInfo;
class cCube;

class cDropItem : public cGameObject
{

private :
	
	cCube*					m_body;

	LPDIRECT3DTEXTURE9		m_pTexture;

	D3DXMATRIX				m_matWorld;
	//D3DXVECTOR3				m_vPosition;

	SYNTHESIZE(iMap*, m_pMap, Map);

	float					m_fRotY;
	float					m_fOverLand;


	//cSprite*		m_pBodySprite;
	D3DMATERIAL9			m_stMtl;
	tagItemInfo				m_stItemInfo;

public:
	cDropItem();
	~cDropItem();

	void Setup(const char * name, D3DXVECTOR3 pos);
	void Update();
	void Render();
	
	void PlusMapHeight();

	void SetMaterial();
	void SetItemInfo(const char * name);
	void PickUp();

	
};

