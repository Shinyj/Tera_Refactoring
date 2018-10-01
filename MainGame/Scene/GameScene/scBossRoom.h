#pragma once

class cBossRoom_Wall;
class cBossRoom_Road;
class cBossRoom_Doll;
class Kelsaik;
class Player;

class scBossRoom : public cScene
{
private :
	vector<cXMesh*>		m_vBossRoom_SingObj;
	cBossRoom_Wall*		m_pBossRoom_Wall;
	cBossRoom_Road*		m_pBossRoom_Road;
	cBossRoom_Doll*		m_pBossRoom_Doll;

	cSprite*			m_pBackGroundBlack;
	int					m_nBGBlackAlpha;
	Player*				m_pPlayer;
	Kelsaik*			m_pKelsaik;

public:
	scBossRoom();
	virtual~scBossRoom();

	void Setup();
	void Release();
	void Update();
	void Render();

	void Render_Wall();
	void Roader_WallGate();
	void Destroy();
};

