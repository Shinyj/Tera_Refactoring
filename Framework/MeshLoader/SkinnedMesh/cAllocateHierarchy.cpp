#include "stdafx.h"
#include "cAllocateHierarchy.h"


cAllocateHierarchy::cAllocateHierarchy()
{
}


cAllocateHierarchy::~cAllocateHierarchy()
{
}

// 자체적으로 알아서 씀. 우리가 호출하는게 아니다

STDMETHODIMP cAllocateHierarchy::CreateFrame(LPCSTR Name, LPD3DXFRAME * ppNewFrame)
{

	HRESULT hr = S_OK;

	ST_BONE* pBone = new ST_BONE;
	ZeroMemory(pBone, sizeof(ST_BONE));

	// 본 이름 초기화
	if (Name)
	{
		UINT length = strlen(Name) + 1;
		pBone->Name = new char[length * sizeof(TCHAR)];
		strcpy_s(pBone->Name, length * sizeof(TCHAR), Name);
	}

	// 추가한 매트릭스 정보 초기화
	D3DXMatrixIdentity(&pBone->CombinedTransformationMatrix);
	D3DXMatrixIdentity(&pBone->TransformationMatrix);

	// 새로만든 정보(구조채)를 아웃풋으로 전달
	*ppNewFrame = pBone;


	return hr;
}

STDMETHODIMP cAllocateHierarchy::CreateMeshContainer(
	LPCSTR						Name,					//
	CONST D3DXMESHDATA *		pMeshData,				//
	CONST D3DXMATERIAL *		pMaterials,				//
	CONST D3DXEFFECTINSTANCE *	pEffectInstances,		//
	DWORD						NumMaterials,			//
	CONST DWORD *				pAdjacency,				//
	LPD3DXSKININFO				pSkinInfo,				// 본 정보를 세팅을 하는 구조 
	LPD3DXMESHCONTAINER *		ppNewMeshContainer)		//
{
	LPDIRECT3DDEVICE9 device;
	pMeshData->pMesh->GetDevice(&device);

	ST_BONE_MESH* pBoneMesh = new ST_BONE_MESH;

	ZeroMemory(pBoneMesh, sizeof(ST_BONE_MESH));

	for (DWORD i = 0; i < NumMaterials; i++)
	{
		pBoneMesh->vecMtl.push_back(pMaterials[i].MatD3D);
		std::string sFullPath = m_sFolder;
		sFullPath = sFullPath + std::string("/") + std::string(pMaterials[i].pTextureFilename);
		pBoneMesh->vecTex.push_back(TEXTURE->GetTexture(sFullPath.c_str()));
	}

	if (pSkinInfo)
	{
		pSkinInfo->AddRef();
		pBoneMesh->pSkinInfo = pSkinInfo;

		pMeshData->pMesh->AddRef();
		pBoneMesh->MeshData.pMesh = pMeshData->pMesh;
		pMeshData->pMesh->CloneMeshFVF(
			pMeshData->pMesh->GetOptions(),
			pMeshData->pMesh->GetFVF(),
			device,
			&pBoneMesh->pOrigMesh);

		DWORD dwNumBones = pSkinInfo->GetNumBones();
		pBoneMesh->ppBoneMatrixPtrs = new D3DXMATRIX*[dwNumBones];
		pBoneMesh->pBoneOffsetMatrices = new D3DXMATRIX[dwNumBones];
		pBoneMesh->pCurrentBoneMatrices = new D3DXMATRIX[dwNumBones];

		for (DWORD i = 0; i < dwNumBones; i++)
		{
			pBoneMesh->pBoneOffsetMatrices[i] =
				*(pSkinInfo->GetBoneOffsetMatrix(i));
		}
	}

	*ppNewMeshContainer = pBoneMesh;

	device->Release();

	return S_OK;
}

STDMETHODIMP cAllocateHierarchy::DestroyFrame(LPD3DXFRAME pFrameToFree)
{
	ST_BONE* pBone = (ST_BONE*)pFrameToFree;

	SAFE_DELETE_ARRAY(pBone->Name);
	SAFE_DELETE(pBone);

	return S_OK;
}

STDMETHODIMP cAllocateHierarchy::DestroyMeshContainer(LPD3DXMESHCONTAINER pMeshContainerToFree)
{
	ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pMeshContainerToFree;

	SAFE_RELEASE(pBoneMesh->pSkinInfo);
	SAFE_RELEASE(pBoneMesh->MeshData.pMesh);
	SAFE_RELEASE(pBoneMesh->pOrigMesh);

	SAFE_DELETE_ARRAY(pBoneMesh->ppBoneMatrixPtrs);
	SAFE_DELETE_ARRAY(pBoneMesh->pBoneOffsetMatrices);
	SAFE_DELETE_ARRAY(pBoneMesh->pCurrentBoneMatrices);

	SAFE_DELETE_ARRAY(pBoneMesh->Name);
	SAFE_DELETE_ARRAY(pBoneMesh->pAdjacency);
	SAFE_DELETE_ARRAY(pBoneMesh->pMaterials);


	UINT iMaterial;

	// release all the allocated textures

	for (iMaterial = 0; iMaterial < pBoneMesh->NumMaterials; iMaterial++)
	{
		SAFE_RELEASE(pBoneMesh->vecTex[iMaterial]);
	}

	SAFE_DELETE_ARRAY(pBoneMesh->ppBoneMatrixPtrs);

	SAFE_DELETE(pBoneMesh);

	return S_OK;
}

HRESULT cAllocateHierarchy::AllocateName(LPCSTR Name, LPSTR * pNewName)
{
	UINT cbLength;
	if (Name != NULL)
	{
		cbLength = strlen(Name) + 1;
		*pNewName = new CHAR[cbLength];

		if (*pNewName == NULL)
			return E_OUTOFMEMORY;

		memcpy(*pNewName, Name, cbLength * sizeof(TCHAR));
	}
	else
		*pNewName = NULL;

	return S_OK;
}