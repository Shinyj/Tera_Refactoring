#include "stdafx.h"
#include "cSkinnedMeshManager.h"

#include "Framework\MeshLoader\SkinnedMesh\cSkinnedMesh.h"

cSkinnedMeshManager::cSkinnedMeshManager()
{
}


cSkinnedMeshManager::~cSkinnedMeshManager()
{
}

cSkinnedMesh * cSkinnedMeshManager::GetSkinnedMesh(const char * szFolder, const char * szFileName)
{
	std::string sFullPath(szFolder);
	sFullPath = sFullPath + "/" + string(szFileName);
	cSkinnedMesh* pSkinnedMesh;

	if (m_mapSkinnedMesh.find(sFullPath) == m_mapSkinnedMesh.end())
	{
		pSkinnedMesh = new cSkinnedMesh;
		pSkinnedMesh->Setup(szFolder, szFileName);
		m_mapSkinnedMesh[sFullPath] = pSkinnedMesh;
	}

	return m_mapSkinnedMesh[sFullPath];
}

cSkinnedMesh * cSkinnedMeshManager::GetSkinnedMesh(const char * szFullPath)
{
	return nullptr;
}

cSkinnedMesh * cSkinnedMeshManager::GetSkinnedMesh(wstring szFullPath)
{
	string path;
	path.assign(szFullPath.begin(), szFullPath.end());

	cSkinnedMesh* pSkinnedMesh;

	if (m_mapSkinnedMesh.find(path) == m_mapSkinnedMesh.end())
	{
		pSkinnedMesh = new cSkinnedMesh;
		pSkinnedMesh->Setup(szFullPath);
		m_mapSkinnedMesh[path] = pSkinnedMesh;
	}

	return m_mapSkinnedMesh[path];
}

void cSkinnedMeshManager::AddSkinnedMesh(const char * szFolder, const char * szFileName)
{
	std::string sFullPath(szFolder);

	if (szFolder != "")
		sFullPath = sFullPath + "/" + string(szFileName);
	else
		sFullPath = szFileName;

	cSkinnedMesh* pSkinnedMesh = new cSkinnedMesh;
	pSkinnedMesh->Setup(szFolder, szFileName);
	m_mapSkinnedMesh[sFullPath] = pSkinnedMesh;
}

void cSkinnedMeshManager::Destroy()
{
	for (auto p : m_mapSkinnedMesh)
	{
		//p.second->Destroy();
		SAFE_DELETE(p.second);
	}
}
