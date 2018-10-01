#pragma once
class House
{
private:
	SYNTHESIZE(D3DXMATRIX, m_matWorld, World);
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);

	cParticleSet*				m_pParticleWarp;

public :
	vector<cXMesh*>				m_vHouse;
public:
	House();
	virtual~House();

	void Setup();
	void Update();
	void Render();

	void Destroy();
};

