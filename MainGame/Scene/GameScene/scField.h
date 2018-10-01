#pragma once

class Beast;
class GateKeeper;
class NpcShop;
class FieldMap;
class Portal;
class House;
class Player;

class scField : public cScene
{
private:
	cSprite*			m_pBackGroundBlack;
	int					m_nBGBlackAlpha;
	Player*				m_pPlayer;

	Portal*				m_pPortal;

	NpcShop*			m_pPoalong;

	House*				m_pTown_House;
	FieldMap*			m_pMap;
	cBoundingBoxLoader* m_pBoxLoader;

	bool				m_isGoBoss;
public:
	scField();
	~scField();

	void Setup();
	void Release();
	void Update();
	void Render();
};

