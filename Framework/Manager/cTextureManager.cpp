#include "stdafx.h"
#include "cTextureManager.h"

cSprite::cSprite()
	: texture(NULL)
	, m_pSprite(NULL)
{
	ZeroMemory(&textureInfo, sizeof(D3DXIMAGE_INFO));

	HRESULT hr;
	hr = D3DXCreateSprite(DEVICE, &m_pSprite);
	assert(SUCCEEDED(hr));
}


cSprite::~cSprite()
{
	SAFE_RELEASE(m_pSprite);
	//SAFE_RELEASE(texture);
}

void cSprite::Setup()
{
}

void cSprite::Render()
{
}

void cSprite::Render(D3DXVECTOR3 location)
{
	RECT rc;
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	{

		SetRect(&rc, 0, 0,
			textureInfo.Width, textureInfo.Height); // 시작점과 너비
													// 드로우
		m_pSprite->Draw
		(
			texture,
			&rc,
			&D3DXVECTOR3(textureInfo.Width / 2, 0, 0),
			&D3DXVECTOR3(WINSIZEX / 2, 0, 0),
			D3DCOLOR_ARGB(255, 255, 255, 255)
		);
	}
	m_pSprite->End();

}

void cSprite::Render(D3DXVECTOR3 center, D3DXVECTOR3 location)
{
	RECT rc;
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	{

		SetRect(&rc, 0, 0,
			textureInfo.Width, textureInfo.Height); // 시작점과 너비
													// 드로우
		m_pSprite->Draw
		(
			texture,
			&rc,
			&center,
			&location,
			D3DCOLOR_ARGB(255, 255, 255, 255)
		);
	}
	m_pSprite->End();
}

void cSprite::Render(RECT rc, D3DXVECTOR3 center, D3DXVECTOR3 location)
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	{
		m_pSprite->Draw
		(
			texture,
			&rc,
			&center,
			&location,
			D3DCOLOR_ARGB(255, 255, 255, 255)
		);
	}
	m_pSprite->End();
}

void cSprite::Render(int x, int y)
{
	if (x != 0)
	{
		int a = 10;
	}
	RECT rc;
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	{

		SetRect(&rc, 0, 0, textureInfo.Width, textureInfo.Height);

		m_pSprite->Draw
		(
			texture,
			&rc,
			&D3DXVECTOR3(0, 0, 0),
			&D3DXVECTOR3(x, y, 0),
			D3DCOLOR_ARGB(255, 255, 255, 255)
		);
	}
	m_pSprite->End();
}

void cSprite::AlphaRender(D3DXVECTOR3 center, D3DXVECTOR3 location, int alpha)
{
	RECT rc;
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	{

		SetRect(&rc, 0, 0,
			textureInfo.Width, textureInfo.Height); // 시작점과 너비
													// 드로우
		m_pSprite->Draw
		(
			texture,
			&rc,
			&center,
			&location,
			D3DCOLOR_ARGB(alpha, 255, 255, 255)
		);
	}
	m_pSprite->End();
}

void cSprite::AlphaRender(RECT rc, D3DXVECTOR3 center, D3DXVECTOR3 location, int alpha)
{
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	{
		m_pSprite->Draw
		(
			texture,
			&rc,
			&center,
			&location,
			D3DCOLOR_ARGB(alpha, 255, 255, 255)
		);
	}
	m_pSprite->End();
}

void cSprite::AlphaRenderWinSize(D3DXVECTOR3 center, D3DXVECTOR3 location, int alpha)
{
	RECT rc;
	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);
	{

		SetRect(&rc, 0, 0,
			WINSIZEX, WINSIZEY); // 시작점과 너비
								 // 드로우
		m_pSprite->Draw
		(
			texture,
			&rc,
			&center,
			&location,
			D3DCOLOR_ARGB(alpha, 255, 255, 255)
		);
	}
	m_pSprite->End();
}

cTextureManager::cTextureManager()
{
}


cTextureManager::~cTextureManager()
{
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture(const char * szFullPath)
{
	if (m_mapTexture.find(szFullPath) == m_mapTexture.end())
	{
		wchar_t wtext[1024];
		size_t outSize;
		mbstowcs_s(&outSize, wtext, strlen(szFullPath) + 1, szFullPath, strlen(szFullPath));

		//D3DXCreateTextureFromFile(DEVICE, wtext, &m_mapTexture[szFullPath]);

		D3DXCreateTextureFromFileEx(
			DEVICE,
			wtext,
			D3DFMT_FROM_FILE,
			D3DFMT_FROM_FILE,
			1,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_DEFAULT,
			D3DX_FILTER_NONE,
			D3DX_FILTER_NONE,
			NULL,
			&m_mapImageInfo[szFullPath],
			0,
			&m_mapTexture[szFullPath]);
	}
	return m_mapTexture[szFullPath];
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture(const char * szFullPath, D3DXIMAGE_INFO * pImageInfo)
{
	if (m_mapTexture.find(szFullPath) == m_mapTexture.end() ||
		m_mapImageInfo.find(szFullPath) == m_mapImageInfo.end())
	{
		wchar_t wtext[1024];
		size_t outSize;
		mbstowcs_s(&outSize, wtext, strlen(szFullPath) + 1, szFullPath, strlen(szFullPath));

		D3DXCreateTextureFromFileEx(
			DEVICE,
			wtext,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_DEFAULT,
			NULL,
			&m_mapImageInfo[szFullPath],
			0,
			&m_mapTexture[szFullPath]);
	}

	*pImageInfo = m_mapImageInfo[szFullPath];

	return m_mapTexture[szFullPath];
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture(wstring szFullPath)
{
	string path;
	path.assign(szFullPath.begin(), szFullPath.end());

	if (m_mapTexture.find(path) == m_mapTexture.end())
	{
		D3DXCreateTextureFromFileEx(
			DEVICE,
			szFullPath.c_str(),
			D3DFMT_FROM_FILE,
			D3DFMT_FROM_FILE,
			1,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_DEFAULT,
			D3DX_FILTER_NONE,
			D3DX_FILTER_NONE,
			NULL,
			&m_mapImageInfo[path],
			0,
			&m_mapTexture[path]);
	}
	return m_mapTexture[path];
}

LPDIRECT3DTEXTURE9 cTextureManager::GetTexture(wstring szFullPath, D3DXIMAGE_INFO * pImageInfo)
{
	string path;
	path.assign(szFullPath.begin(), szFullPath.end());
	
	if (m_mapTexture.find(path) == m_mapTexture.end() ||
		m_mapImageInfo.find(path) == m_mapImageInfo.end())
	{
		D3DXCreateTextureFromFileEx(
			DEVICE,
			szFullPath.c_str(),
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_DEFAULT,
			NULL,
			&m_mapImageInfo[path],
			0,
			&m_mapTexture[path]);
	}

	*pImageInfo = m_mapImageInfo[path];

	return m_mapTexture[path];
}

cSprite * cTextureManager::GetSprite(string szFullPath)
{
	const char * path = szFullPath.c_str();
	cSprite * sprite = new cSprite;

	if (m_mapTexture.find(szFullPath) == m_mapTexture.end() ||
		m_mapImageInfo.find(szFullPath) == m_mapImageInfo.end())
	{
		wchar_t wtext[1024];
		size_t outSize;
		mbstowcs_s(&outSize, wtext, strlen(path) + 1, path, strlen(path));

		D3DXCreateTextureFromFileEx(
			DEVICE,
			wtext,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_DEFAULT,
			NULL,
			&m_mapImageInfo[szFullPath],
			0,
			&m_mapTexture[szFullPath]);
	}

	sprite->texture = m_mapTexture[szFullPath];
	sprite->textureInfo = m_mapImageInfo[szFullPath];

	return sprite;
}

cSprite * cTextureManager::GetSprite(wstring szFullPath)
{
	string  path;// = szFullPath.c_str();
	path.assign(szFullPath.begin(), szFullPath.end());

	cSprite * sprite = new cSprite;

	if (m_mapTexture.find(path) == m_mapTexture.end() ||
		m_mapImageInfo.find(path) == m_mapImageInfo.end())
	{
		D3DXCreateTextureFromFileEx(
			DEVICE,
			szFullPath.c_str(),
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT_NONPOW2,
			D3DX_DEFAULT,
			0,
			D3DFMT_UNKNOWN,
			D3DPOOL_MANAGED,
			D3DX_FILTER_NONE,
			D3DX_DEFAULT,
			NULL,
			&m_mapImageInfo[path],
			0,
			&m_mapTexture[path]);
	}

	sprite->texture = m_mapTexture[path];
	sprite->textureInfo = m_mapImageInfo[path];

	return sprite;
}



void cTextureManager::AddTexture(const char * szFullPath)
{
	wchar_t wtext[1024];
	size_t outSize;
	mbstowcs_s(&outSize, wtext, strlen(szFullPath) + 1, szFullPath, strlen(szFullPath));

	D3DXCreateTextureFromFileEx(
		DEVICE,
		wtext,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT_NONPOW2,
		D3DX_DEFAULT,
		0,
		D3DFMT_UNKNOWN,
		D3DPOOL_MANAGED,
		D3DX_FILTER_NONE,
		D3DX_DEFAULT,
		0,
		&m_mapImageInfo[szFullPath],
		0,
		&m_mapTexture[szFullPath]);
}

void cTextureManager::Destroy()
{
	for (auto p : m_mapTexture)
	{
		if (p.second)
			SAFE_RELEASE(p.second);
	}
	m_mapTexture.clear();
}
