#pragma once
class cBoundingObject
{
protected:
	SYNTHESIZE_REF(cSpere*, m_pSpere, Spere);
	SYNTHESIZE_REF(cBoundingBox*, m_pBoundingBox, BoundingBox);
	SYNTHESIZE_REF(vector<cBoundingBox*>, m_vecBBoxGroup, vecBBoxGroup); // 하나의 Object에 여러개의 충돌box

	SYNTHESIZE(bool, m_bSightBox, SightBox);
	SYNTHESIZE(bool, m_bSightSpere, SightSpere);


public:
	cBoundingObject();
	virtual~cBoundingObject();

	void Setup();
	void Update();
	void Render();

	void Destroy_g();
};

