#pragma once

#define TEXTURE cTextureManager::GetInstance()

#pragma once

// 텍스쳐 매니져에서 정보를 받아서 사용해야함

class cSprite
{
public:

	LPDIRECT3DTEXTURE9	texture;		// 텍스쳐
	D3DXIMAGE_INFO		textureInfo;	// 텍스쳐 정보
										//int					x;				// 시작 x 좌표
										//int					y;				// 시작 y 좌표

	LPD3DXSPRITE			m_pSprite;
public:
	cSprite();
	~cSprite();

	void Setup();
	void Render();

	void Render(D3DXVECTOR3 location);
	void Render(D3DXVECTOR3 center, D3DXVECTOR3 location);
	void Render(RECT rc, D3DXVECTOR3 center, D3DXVECTOR3 location);
	void Render(int x, int y);
	void AlphaRender(D3DXVECTOR3 center, D3DXVECTOR3 location, int alpha);
	void AlphaRender(RECT rc, D3DXVECTOR3 center, D3DXVECTOR3 location, int alpha);
	void AlphaRenderWinSize(D3DXVECTOR3 center, D3DXVECTOR3 location, int alpha);
};

class cTextureManager
{
private:
	SINGLETONE(cTextureManager);
	map<string, LPDIRECT3DTEXTURE9>	m_mapTexture;
	map<string, D3DXIMAGE_INFO>		m_mapImageInfo;



public:
	LPDIRECT3DTEXTURE9 GetTexture(const char* szFullPath);
	LPDIRECT3DTEXTURE9 GetTexture(const char* szFullPath, D3DXIMAGE_INFO* pImageInfo);

	LPDIRECT3DTEXTURE9 GetTexture(wstring szFullPath);
	LPDIRECT3DTEXTURE9 GetTexture(wstring szFullPath, D3DXIMAGE_INFO* pImageInfo);

	cSprite * GetSprite(string szFullPath);
	cSprite * GetSprite(wstring szFullPath);

	void AddTexture(const char * szFullPath);

	void Destroy();
};

