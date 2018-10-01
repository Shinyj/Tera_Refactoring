#include "stdafx.h"
#include "cStaticMesh.h"


cXMesh::cXMesh()
{
}


cXMesh::~cXMesh()
{
	//SAFE_RELEASE(g_pMesh);
	//SAFE_DELETE(g_pMeshMaterials);
	//SAFE_DELETE(g_pMeshTextures);
}

void cXMesh::Render()
{
	for (size_t i = 0; i < g_dwNumMaterials; i++)
	{
		DEVICE->SetMaterial(&g_pMeshMaterials[i]);
		// 현재 sub Set에 대한 재질 설정
		DEVICE->SetTexture(0, g_pMeshTextures[i]);
		// 그리자
		g_pMesh->DrawSubset(i);
	}
	DEVICE->SetTexture(0, NULL);
}


cStaticMesh::cStaticMesh()
{
}


cStaticMesh::~cStaticMesh()
{
}

cXMesh * cStaticMesh::xFimeLoad(const char * xFileName)
{
	wchar_t wtext[1024];
	size_t outSize;
	mbstowcs_s(&outSize, wtext, strlen(xFileName) + 1, xFileName, strlen(xFileName));

	LPD3DXBUFFER pD3DXMtrBuffer;

	// x 파일을 불러오는 부분
	D3DXLoadMeshFromX(wtext, D3DXMESH_SYSTEMMEM,
		DEVICE, NULL, &pD3DXMtrBuffer, NULL, &g_dwNumMaterials, &g_pMesh);

	// 텍스쳐 파일이 다른 폴더에 있을 경우 텍스쳐 패스 위치 닫기?
	char texturePath[256];
	// 현재 폴더라면

	char temp[256];
	char* pChar;
	strcpy_s(temp, 256, xFileName);
	_strrev(temp);
	pChar = strchr(temp, '/');
	strcpy_s(texturePath, 256, pChar);
	_strrev(texturePath);

	D3DXMATERIAL* d3dxMaterials = (D3DXMATERIAL*)pD3DXMtrBuffer->GetBufferPointer();
	g_pMeshMaterials = new D3DMATERIAL9[g_dwNumMaterials];
	g_pMeshTextures = new LPDIRECT3DTEXTURE9[g_dwNumMaterials];
	for (size_t i = 0; i < g_dwNumMaterials; i++)
	{
		// 재질 복사
		g_pMeshMaterials[i] = d3dxMaterials[i].MatD3D;
		// 재질에 대한 엠비언트 색상 설정 (d3dx가 하동으로 안해준다고 한다)
		g_pMeshMaterials[i].Ambient = g_pMeshMaterials[i].Diffuse;
		g_pMeshTextures[i] = NULL;

		// 여기서 텍스쳐 파일이 존재한다면!!
		if (d3dxMaterials[i].pTextureFilename > 0)
		{
			// 경로, 텍스쳐 이름을 만들어서 텍스쳐를 넣어줘야함
			char tempFile[256];
			sprintf_s(tempFile, "%s%s", texturePath, d3dxMaterials[i].pTextureFilename, 256);
			g_pMeshTextures[i] = TEXTURE->GetTexture(tempFile);
		}
	}
	// 재질 버퍼 사용 끝, 해제
	pD3DXMtrBuffer->Release();

	cXMesh* newGroup = new cXMesh;
	newGroup->SetMesh(g_pMesh);
	newGroup->SetMaterials(g_pMeshMaterials);
	newGroup->SetTexture(g_pMeshTextures);
	newGroup->SetNumMaterial(g_dwNumMaterials);

	return newGroup;
}