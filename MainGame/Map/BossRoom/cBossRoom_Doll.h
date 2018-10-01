#pragma once

class cXMesh;

class cBossRoom_Doll
{
public:
	cBossRoom_Doll();
	~cBossRoom_Doll();

	vector<cBossRoom_Doll> m_vecBossRoom_Doll;
	vector<cXMesh*> m_vecDollParts;
	D3DXMATRIX matWorld;

	float rotY;

	void Setup();
	void Render();
	void Destroy();
};

