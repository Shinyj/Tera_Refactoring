#pragma once

class cXMesh
{
private:
	SYNTHESIZE_REF(LPD3DXMESH, g_pMesh, Mesh);
	SYNTHESIZE_REF(D3DMATERIAL9*, g_pMeshMaterials, Materials);
	SYNTHESIZE_REF(LPDIRECT3DTEXTURE9*, g_pMeshTextures, Texture);
	SYNTHESIZE_REF(DWORD, g_dwNumMaterials, NumMaterial);

public:
	cXMesh();
	~cXMesh();

	void Render();
};

class cStaticMesh
{
private:
	LPD3DXMESH				g_pMesh; // 메시 객체
	D3DMATERIAL9*			g_pMeshMaterials; // 메시의 재질
	LPDIRECT3DTEXTURE9*		g_pMeshTextures;// 메시의 텍스쳐
	DWORD					g_dwNumMaterials; // 재질의 수

public:
	cStaticMesh();
	virtual~cStaticMesh();

	cXMesh* xFimeLoad(const char* xFileName);
};

