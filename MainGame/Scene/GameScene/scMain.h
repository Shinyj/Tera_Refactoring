#pragma once
class scMain : public cScene
{
private :

	cSprite*			m_pBackGround;
	cSprite*			m_pBackGroundBlack;
	cSprite*			m_pBackGroundWhite;

	ProgressBar*		m_pLoadingBar;
	int					m_nBGBlackAlpha;
	bool				m_isStart;


	RECT rc[5];

	D3DCOLOR			m_colorOver;
	D3DCOLOR			m_colorDefault;

	string				m_sNextScene;

public:
	scMain();
	virtual~scMain();

	void Setup();
	void Release();
	void Update();
	void Render();
};

