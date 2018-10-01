#pragma once
class cSpere
{
private:

	LPD3DXMESH			m_pSpere;

	SYNTHESIZE(D3DXMATRIX, m_matWorld, World);
	D3DXVECTOR3				 m_vPosition;
	SYNTHESIZE(float, m_fRadius, Radius);

public:
	cSpere();
	~cSpere();

	void Setup(D3DXVECTOR3 pos, float r);
	void Update();
	void Render();

	D3DXVECTOR3 GetPosition()
	{
		D3DXVECTOR3 pos(0, 0, 0);
		D3DXVec3TransformCoord(&pos, &pos, &m_matWorld);
		return pos;
	}

	void SetPosition(D3DXVECTOR3 pos)
	{
		m_vPosition = pos;
	}
};

