#pragma once
class cGrid
{
private:
	vector<ST_PC_VERTEX>	m_vecVertex;
	D3DXVECTOR3				m_vAxisX;
	D3DXVECTOR3				m_vAxisZ;

	D3DXMATRIX				m_matWorld;
public:
	cGrid();
	~cGrid();

	void Setup();
	void Render();
};


