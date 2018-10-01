#pragma once
#include "MainGame\cGameObject.h"

class iMap;
class NpcShop : public cGameObject
{
private :
	enum STATE
	{
		IDLE
	};

	float				m_fRotY;
	D3DXVECTOR3			m_vDirection;
	D3DXMATRIX			m_matWorld;
	SYNTHESIZE(iMap*, m_pMap, Map);
	float				m_fCosVal; // ī�޶� ���� ����

	cSkinnedMesh*		m_pPoalong;
	ST_BONE*			m_pDummyRoot;

	STATE				m_state;
	STATE				m_currState;

	float				m_fCurAnimTime;
	float				m_fAnimTime;

	float				m_fTime;

	D3DXVECTOR3			m_vWhere_To_Stay;
	float DialogZone;
	//LPD3DXEFFECT		m_pRimLight;
public:
	NpcShop();
	~NpcShop();

	void Setup(D3DXVECTOR3 v);
	void Update();
	void Render();

	//���̸� ����
	void PlusMapHeight();
};

