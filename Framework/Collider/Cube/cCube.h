#pragma once
class cCube
{
protected	:

	//vector<ST_PC_VERTEX>		m_vecVertex;
	SYNTHESIZE(vector<ST_PCT_VERTEX>, m_vecVertex, Vertex); //lsc
	SYNTHESIZE(vector<ST_PT_VERTEX>, m_vecPtVertex, PtVertex); //lsc

public:
	cCube();
	~cCube();

	virtual void Setup();
	virtual void Setup(D3DXVECTOR3 min, D3DXVECTOR3 max);
	
	virtual void Update();
			void Update(D3DXVECTOR3 min, D3DXVECTOR3 max);
	
	virtual void Render();
	virtual void PTRender();
};

