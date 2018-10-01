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
	LPD3DXMESH				g_pMesh; // �޽� ��ü
	D3DMATERIAL9*			g_pMeshMaterials; // �޽��� ����
	LPDIRECT3DTEXTURE9*		g_pMeshTextures;// �޽��� �ؽ���
	DWORD					g_dwNumMaterials; // ������ ��

public:
	cStaticMesh();
	virtual~cStaticMesh();

	cXMesh* xFimeLoad(const char* xFileName);
};

