#pragma once
class MainGame
{
private :

	LPDIRECT3DSURFACE9		surfaceCursor;
	LPDIRECT3DTEXTURE9		m_pCursorArrow;
	LPDIRECT3DTEXTURE9		m_pCursorEmpty;

public:
	MainGame();
	~MainGame();

	void Setup();
	void Update();
	void Render();

	void SetLight();
	void SetCursor();

};

