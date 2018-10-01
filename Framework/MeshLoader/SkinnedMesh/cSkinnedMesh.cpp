#include "stdafx.h"
#include "cSkinnedMesh.h"

#include "Framework\MeshLoader\SkinnedMesh\cAllocateHierarchy.h"
#include "Framework\MeshLoader\StaticMesh\cStaticMesh.h"


cSkinnedMesh::cSkinnedMesh()
	: m_pRoot(NULL)
	, m_pAnimController(NULL)
	, m_fBlendTime(0.15f)
	, m_fPassedBlendTime(0.0f)
	, m_bBlend(false)
{
	D3DXMatrixIdentity(&m_matWorld);
}

cSkinnedMesh::cSkinnedMesh(const char * szFolder, const char * szFile)
{
	cSkinnedMesh* pSkinnedMesh = SKINNEDMESH->GetSkinnedMesh(szFolder, szFile);

	m_pRoot = pSkinnedMesh->m_pRoot;
	pSkinnedMesh->m_pAnimController->CloneAnimationController(
		pSkinnedMesh->m_pAnimController->GetMaxNumAnimationOutputs(),
		pSkinnedMesh->m_pAnimController->GetMaxNumAnimationSets(),
		pSkinnedMesh->m_pAnimController->GetMaxNumTracks(),
		pSkinnedMesh->m_pAnimController->GetMaxNumEvents(),
		&m_pAnimController);
}


cSkinnedMesh::~cSkinnedMesh()
{
	//SAFE_DELETE(m_pRoot);
	cAllocateHierarchy ah;
	D3DXFrameDestroy(m_pRoot, &ah);
	SAFE_RELEASE(m_pAnimController);
}

void cSkinnedMesh::Setup(const char* szFolder, const char* szFile)
{

	string sFullPath(szFolder);
	sFullPath = sFullPath + string("/") + string(szFile);

	std::wstring stemp = std::wstring(sFullPath.begin(), sFullPath.end());
	LPCWSTR sw = stemp.c_str();

	cAllocateHierarchy ah;

	ah.SetFolder(szFolder);

	D3DXLoadMeshHierarchyFromX(sw,
		D3DXMESH_MANAGED,
		DEVICE,
		&ah,
		NULL,
		&m_pRoot,
		&m_pAnimController
	);

	if (m_pRoot)
		SetupBoneMatrixPtrs(m_pRoot);
}

void cSkinnedMesh::Setup(wstring szFullPath)
{

	string path;
	path.assign(szFullPath.begin(), szFullPath.end());

	cAllocateHierarchy ah;

	ah.SetFolder(path);

	D3DXLoadMeshHierarchyFromX(szFullPath.c_str(),
		D3DXMESH_MANAGED,
		DEVICE,
		&ah,
		NULL,
		&m_pRoot,
		&m_pAnimController
	);

	if (m_pRoot)
		SetupBoneMatrixPtrs(m_pRoot);
}

void cSkinnedMesh::Update(D3DXMATRIX world)
{
	if (m_bBlend)
	{
		m_fPassedBlendTime += TIME->GetEllapsedTime();
		if (m_fPassedBlendTime > m_fBlendTime)
		{
			m_pAnimController->SetTrackWeight(0, 1.0f);
			m_pAnimController->SetTrackEnable(1, false);
			m_bBlend = false;
		}
		else
		{
			float fWeight = m_fPassedBlendTime / m_fBlendTime;
			m_pAnimController->SetTrackWeight(0, fWeight);
			m_pAnimController->SetTrackWeight(1, 1.0f - fWeight);
		}
	}

	if (m_pAnimController)
		m_pAnimController->AdvanceTime(TIME->GetEllapsedTime(), NULL);

	Update(m_pRoot, NULL, world);
	UpdateSkinnedMesh(m_pRoot);
}

void cSkinnedMesh::Update()
{
	//int n = m_pAnimController->GetTime();
	//m_pAnimController->GetTrackAnimationSet();

	if (m_bBlend)
	{
		m_fPassedBlendTime += TIME->GetEllapsedTime();
		if (m_fPassedBlendTime > m_fBlendTime)
		{
			m_pAnimController->SetTrackWeight(0, 1.0f);
			m_pAnimController->SetTrackEnable(1, false);
			m_bBlend = false;
		}
		else
		{
			float fWeight = m_fPassedBlendTime / m_fBlendTime;
			m_pAnimController->SetTrackWeight(0, fWeight);
			m_pAnimController->SetTrackWeight(1, 1.0f - fWeight);
		}
	}


	m_pAnimController->AdvanceTime(TIME->GetEllapsedTime(), NULL);
	//m_pAnimController->AdvanceTime(27, NULL);


	Update(m_pRoot, NULL);
	UpdateSkinnedMesh(m_pRoot);
}

void cSkinnedMesh::Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent, D3DXMATRIX world)
{
	ST_BONE* pBone = (ST_BONE*)pFrame;
	// 여기서는 행렬을 누적시킴
	pBone->CombinedTransformationMatrix = pBone->TransformationMatrix;
	if (pBone == m_pRoot && pParent == NULL)
	{
		pBone->CombinedTransformationMatrix *= world;
	}
	else if (pParent)
	{
		pBone->CombinedTransformationMatrix *=
			((ST_BONE*)pParent)->CombinedTransformationMatrix;
	}

	if (pFrame->pFrameFirstChild)
		Update(pFrame->pFrameFirstChild, pFrame, world);

	if (pFrame->pFrameSibling)
		Update(pFrame->pFrameSibling, pParent, world);
}

void cSkinnedMesh::Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent)
{
	ST_BONE* pBone = (ST_BONE*)pFrame;
	// 여기서는 행렬을 누적시킴
	pBone->CombinedTransformationMatrix = pBone->TransformationMatrix;
	if (pBone == m_pRoot && pParent == NULL)
	{
		pBone->CombinedTransformationMatrix;
	}
	else if (pParent)
	{
		pBone->CombinedTransformationMatrix *=
			((ST_BONE*)pParent)->CombinedTransformationMatrix;
	}

	if (pFrame->pFrameFirstChild)
		Update(pFrame->pFrameFirstChild, pFrame);

	if (pFrame->pFrameSibling)
		Update(pFrame->pFrameSibling, pParent);
}

void cSkinnedMesh::DontMoveUpdate(D3DXMATRIX world)
{
	Update(m_pRoot, NULL, world);
	UpdateSkinnedMesh(m_pRoot);
}

void cSkinnedMesh::UpdateSkinnedMesh(LPD3DXFRAME pFrame)
{
	// TODO : 커헌트본매트릭스가 최종적으로 사용되는 매트릭스
	// pCurrentBoneMatrices를 계산
	// how pCurrentBoneMatrices = pBoneOffsetMatices *  ppBonematixPtrs
	// 매시를 커런트 본 매트릭스로 계산

	if (pFrame && pFrame->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;

		if (pBoneMesh->pSkinInfo)
		{
			DWORD dwNumBones = pBoneMesh->pSkinInfo->GetNumBones();

			for (DWORD i = 0; i < dwNumBones; i++)
			{
				pBoneMesh->pCurrentBoneMatrices[i] =
					pBoneMesh->pBoneOffsetMatrices[i] * *(pBoneMesh->ppBoneMatrixPtrs[i]);
			}

			// TODO	2 : Mesh 정보를 업데이트 ( 함수 이름에 업데이트가 들어간다 하신다

			BYTE* src = NULL;
			BYTE* dest = NULL;
			pBoneMesh->pOrigMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&src);
			{
				pBoneMesh->MeshData.pMesh->LockVertexBuffer(0, (void**)&dest);
				{
					pBoneMesh->pSkinInfo->UpdateSkinnedMesh(
						pBoneMesh->pCurrentBoneMatrices,
						NULL, src, dest);
				}
				pBoneMesh->MeshData.pMesh->UnlockVertexBuffer();
			}
			pBoneMesh->pOrigMesh->UnlockVertexBuffer();
		}
	}


	// TODO 3 : 재귀함수 호출
	if (pFrame->pFrameFirstChild)
		UpdateSkinnedMesh(pFrame->pFrameFirstChild);

	if (pFrame->pFrameSibling)
		UpdateSkinnedMesh(pFrame->pFrameSibling);

}

void cSkinnedMesh::Render(LPD3DXFRAME pFrame)
{
	if (pFrame == NULL)
		pFrame = m_pRoot;

	ST_BONE* pBone = (ST_BONE*)pFrame;

	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;

		// 스킨 인포가 존재하는 메시
		if (pBoneMesh->pSkinInfo != NULL)
		{
			if (pBoneMesh->MeshData.pMesh)
			{
				//DEVICE->SetTransform(D3DTS_WORLD, &(pBone->CombinedTransformationMatrix * m_matWorld));

				for (DWORD i = 0; i < pBoneMesh->vecMtl.size(); i++)
				{
					DEVICE->SetTexture(0, pBoneMesh->vecTex[i]);
					DEVICE->SetMaterial(&pBoneMesh->vecMtl[i]);
					pBoneMesh->MeshData.pMesh->DrawSubset(i);
				}
			}
		}
		else
		{
			for (DWORD i = 0; i < pBoneMesh->vecMtl.size(); i++)
			{
				DEVICE->SetTexture(0, pBoneMesh->vecTex[i]);
				DEVICE->SetMaterial(&pBoneMesh->vecMtl[i]);
				pBoneMesh->pOrigMesh->DrawSubset(i);
			}
		}
	}

	if (pFrame->pFrameFirstChild)
		Render(pFrame->pFrameFirstChild);

	if (pFrame->pFrameSibling)
		Render(pFrame->pFrameSibling);
}

void cSkinnedMesh::Render(LPD3DXFRAME pFrame, LPD3DXEFFECT shader)
{
	UINT numPasses = 0;
	shader->Begin(&numPasses, NULL);

	if (pFrame == NULL)
		pFrame = m_pRoot;

	ST_BONE* pBone = (ST_BONE*)pFrame;

	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;

		// 스킨 인포가 존재하는 메시
		if (pBoneMesh->pSkinInfo != NULL)
		{
			if (pBoneMesh->MeshData.pMesh)
			{
				for (UINT i = 0; i < numPasses; i++)
				{
					shader->SetTexture("DiffuseMap_Tex", pBoneMesh->vecTex[i]);
					//shader->SetTexture("Texture0", pBoneMesh->vecTex[i]);
					shader->BeginPass(i);
					pBoneMesh->MeshData.pMesh->DrawSubset(i);
					shader->EndPass();
				}
			}
		}
		else
		{
			for (UINT i = 0; i < numPasses; i++)
			{
				shader->SetTexture("DiffuseMap_Tex", pBoneMesh->vecTex[i]);
				//shader->SetTexture("Texture0", pBoneMesh->vecTex[i]);
				shader->BeginPass(i);
				pBoneMesh->pOrigMesh->DrawSubset(i);
				shader->EndPass();
			}
		}
	}

	shader->End();

	if (pFrame->pFrameFirstChild)
		Render(pFrame->pFrameFirstChild, shader);

	if (pFrame->pFrameSibling)
		Render(pFrame->pFrameSibling, shader);
}

//DeathShader전용 함수.
void cSkinnedMesh::Render(IN LPD3DXFRAME pFrame, IN LPD3DXEFFECT shader, IN int time, IN LPDIRECT3DTEXTURE9 texture,
	OUT bool &Modeonoff, OUT bool &IsGen, OUT float &TOD)
{
	UINT numPasses = 0;
	shader->Begin(&numPasses, NULL);

	D3DXMATRIX   matView, matWorld, matProj, matViewProj;
	DEVICE->GetTransform(D3DTS_VIEW, &matView);
	DEVICE->GetTransform(D3DTS_PROJECTION, &matProj);
	matViewProj = matView * matProj;
	D3DXMatrixIdentity(&matWorld);
	shader->SetMatrix("matViewProjection", &matViewProj);
	shader->SetMatrix("matWorld", &matWorld);
	shader->SetTexture("Earth_Tex", texture);
	shader->SetFloat("Offset", 0.0045f * time);

	if (pFrame == NULL)
		pFrame = m_pRoot;

	ST_BONE* pBone = (ST_BONE*)pFrame;

	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;

		// 스킨 인포가 존재하는 메시
		if (pBoneMesh->pSkinInfo != NULL)
		{
			if (pBoneMesh->MeshData.pMesh)
			{
				for (UINT i = 0; i < numPasses; i++)
				{
					shader->BeginPass(i);
					pBoneMesh->MeshData.pMesh->DrawSubset(i);
					shader->EndPass();
				}

			}
		}
		else
		{
			for (UINT i = 0; i < numPasses; i++)
			{
				shader->BeginPass(i);
				pBoneMesh->pOrigMesh->DrawSubset(i);
				shader->EndPass();
			}
		}
	}

	shader->End();

	/*char numString[100];
	_itoa(time, numString, 10);
	OutputDebugStringA(numString);*/

	if (pFrame->pFrameFirstChild)
		Render(pFrame->pFrameFirstChild, shader, time, texture, Modeonoff, IsGen, TOD);

	if (pFrame->pFrameSibling)
		Render(pFrame->pFrameSibling, shader, time, texture, Modeonoff, IsGen, TOD);

	if (0.0045f * time > 1.0f)
	{
		Modeonoff = false;
		IsGen = false;
		//완전히 사라진 시점 기록
		TOD = (float)GetTickCount();
	}
}

void cSkinnedMesh::Render(LPD3DXFRAME pFrame, char * key, ST_BONE_MESH * equit)
{
	if (pFrame == NULL)
		pFrame = m_pRoot;

	ST_BONE* pBone = (ST_BONE*)pFrame;

	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;

		// 스킨 인포가 존재하는 메시
		if (pBoneMesh->MeshData.pMesh)
		{
			//DEVICE->SetTransform(D3DTS_WORLD, &(pBone->CombinedTransformationMatrix * m_matWorld));

			if (strcmp(pBone->Name, key))
			{
				for (DWORD i = 0; i < pBoneMesh->vecMtl.size(); i++)
				{
					DEVICE->SetTexture(0, pBoneMesh->vecTex[i]);
					DEVICE->SetMaterial(&pBoneMesh->vecMtl[i]);
					pBoneMesh->MeshData.pMesh->DrawSubset(i);
				}
			}
			else
			{
				for (DWORD i = 0; i < equit->vecMtl.size(); i++)
				{
					DEVICE->SetTexture(0, equit->vecTex[i]);
					DEVICE->SetMaterial(&equit->vecMtl[i]);
					pBoneMesh->MeshData.pMesh->DrawSubset(i);
				}
			}
		}
	}

	if (pFrame->pFrameFirstChild)
		Render(pFrame->pFrameFirstChild, key, equit);

	if (pFrame->pFrameSibling)
		Render(pFrame->pFrameSibling, key, equit);
}

void cSkinnedMesh::RenderWhite(LPD3DXFRAME pFrame)
{
	if (pFrame == NULL)
		pFrame = m_pRoot;

	ST_BONE* pBone = (ST_BONE*)pFrame;

	if (pBone->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pBone->pMeshContainer;

		// 스킨 인포가 존재하는 메시
		if (pBoneMesh->pSkinInfo != NULL)
		{
			if (pBoneMesh->MeshData.pMesh)
			{

				for (DWORD i = 0; i < pBoneMesh->vecMtl.size(); i++)
				{
					DEVICE->SetTexture(0, pBoneMesh->vecTex[i]);
					pBoneMesh->vecMtl[i].Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
					pBoneMesh->vecMtl[i].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pBoneMesh->vecMtl[i].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					DEVICE->SetMaterial(&pBoneMesh->vecMtl[i]);
					pBoneMesh->MeshData.pMesh->DrawSubset(i);

				}
			}
		}
		else
		{
			for (DWORD i = 0; i < pBoneMesh->vecMtl.size(); i++)
			{
				DEVICE->SetTexture(0, pBoneMesh->vecTex[i]);
				pBoneMesh->vecMtl[i].Ambient = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
				pBoneMesh->vecMtl[i].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pBoneMesh->vecMtl[i].Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				DEVICE->SetMaterial(&pBoneMesh->vecMtl[i]);
				pBoneMesh->pOrigMesh->DrawSubset(i);

			}
		}
	}

	if (pFrame->pFrameFirstChild)
		RenderWhite(pFrame->pFrameFirstChild);

	if (pFrame->pFrameSibling)
		RenderWhite(pFrame->pFrameSibling);
}

void cSkinnedMesh::SetupBoneMatrixPtrs(LPD3DXFRAME pFrame)
{
	if (pFrame && pFrame->pMeshContainer)
	{
		ST_BONE_MESH* pBoneMesh = (ST_BONE_MESH*)pFrame->pMeshContainer;
		if (pBoneMesh->pSkinInfo)
		{
			DWORD dwNumBones = pBoneMesh->pSkinInfo->GetNumBones();
			for (DWORD i = 0; i < dwNumBones; i++)
			{
				ST_BONE* pBone = (ST_BONE*)D3DXFrameFind(m_pRoot,
					pBoneMesh->pSkinInfo->GetBoneName(i));

				// 본이 움직였을 때 최종적으로 갖게 되는 매트릭스 정보를 연결을 시켜야함
				pBoneMesh->ppBoneMatrixPtrs[i] =
					&(pBone->CombinedTransformationMatrix);
			}
		}
	}

	// 자식, 형제 노드들을 순회하면서 재귀적으로 셋팅
	if (pFrame->pFrameFirstChild)
		SetupBoneMatrixPtrs(pFrame->pFrameFirstChild);

	if (pFrame->pFrameSibling)
		SetupBoneMatrixPtrs(pFrame->pFrameSibling);
}



void cSkinnedMesh::SetWolrd(D3DXMATRIX world, LPD3DXFRAME pFrame)
{
	ST_BONE* pBone = (ST_BONE*)m_pRoot;

	pBone->CombinedTransformationMatrix = pBone->TransformationMatrix;

	if (pBone == m_pRoot)
	{
		pBone->CombinedTransformationMatrix *= world;
	}
}

void cSkinnedMesh::SetAnimationIndex(int nIndex)
{
	int n = m_pAnimController->GetNumAnimationSets();
	if (nIndex > n) nIndex = nIndex % n;

	LPD3DXANIMATIONSET	pAniSet = NULL;
	m_pAnimController->GetAnimationSet(nIndex, &pAniSet);
	m_pAnimController->SetTrackAnimationSet(0, pAniSet);

	m_pAnimController->SetTrackPosition(0, 0);

	SAFE_RELEASE(pAniSet);

}

void cSkinnedMesh::SetAnimationIndexBlend(int nIndex)
{
	m_bBlend = true;
	m_fPassedBlendTime = 0.0f;

	int n = m_pAnimController->GetNumAnimationSets();
	if (nIndex > n) nIndex = nIndex % n;

	LPD3DXANIMATIONSET	pPrevAniSet = NULL;
	LPD3DXANIMATIONSET	pNextAniSet = NULL;
	D3DXTRACK_DESC	stTrackDesc;

	m_pAnimController->GetTrackDesc(0, &stTrackDesc);
	m_pAnimController->GetTrackAnimationSet(0, &pPrevAniSet);

	m_pAnimController->SetTrackAnimationSet(1, pPrevAniSet);
	m_pAnimController->SetTrackDesc(1, &stTrackDesc);

	m_pAnimController->GetAnimationSet(nIndex, &pNextAniSet);
	m_pAnimController->SetTrackAnimationSet(0, pNextAniSet);

	m_pAnimController->SetTrackWeight(0, 0.0f);
	m_pAnimController->SetTrackWeight(1, 1.0f);

	// 애니메이션을 다시 0으로 돌리는 부분
	m_pAnimController->SetTrackPosition(0, 0);
	// 애니메이션을 다시 0으로 돌리는 부분
	m_pAnimController->SetTrackPosition(1, 1);

	SAFE_RELEASE(pPrevAniSet);
	SAFE_RELEASE(pNextAniSet);
	//SAFE_RELEASE(m_pAnimController);
}

bool cSkinnedMesh::IsFinishAnumOnce()
{
	LPD3DXANIMATIONSET animSet;
	D3DXTRACK_DESC track;

	m_pAnimController->GetTrackAnimationSet(0, &animSet);
	m_pAnimController->GetTrackDesc(0, &track);

	float period = animSet->GetPeriod();
	float current = fmod(track.Position, period);

	if (KEY->IsOnceKeyDown('Y'))
	{
		int a = period;
		int b = current;
	}
	if (current < period)
		int a = 10;

	SAFE_RELEASE(animSet);

	return current >= period - 0.1;
}

float cSkinnedMesh::GetAnimPosition()
{
	D3DXTRACK_DESC track;
	m_pAnimController->GetTrackDesc(0, &track);
	return track.Position;
}

void cSkinnedMesh::SetAnimPosition(float pos)
{
	m_pAnimController->SetTrackPosition(0, pos);
}

void cSkinnedMesh::AnimAdvanceTime()
{
	m_pAnimController->AdvanceTime(TIME->GetEllapsedTime(), NULL);
}

void cSkinnedMesh::AnimAdvanceTimeDouble()
{
	m_pAnimController->AdvanceTime(TIME->GetEllapsedTime() * 2, NULL);
}
