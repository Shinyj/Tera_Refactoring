#pragma once
#include "cGameObject.h"

class Portal : public cGameObject
{
	SYNTHESIZE(D3DXMATRIX, m_matWorld, World);
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);

public:
	Portal();
	~Portal();

	void Setup(D3DXVECTOR3	pos);
	void Update();
	void Render();

	bool IsInRange();
};

