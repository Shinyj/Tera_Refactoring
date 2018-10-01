#include "stdafx.h"
#include "cStaticMeshManager.h"

#include "Framework\MeshLoader\StaticMesh\cStaticMesh.h"

cStaticMeshManager::cStaticMeshManager()
{
}


cStaticMeshManager::~cStaticMeshManager()
{
}

void cStaticMeshManager::AddStaticMesh(const char * szFullPath)
{
	wchar_t wtext[1024];
	size_t outSize;
	mbstowcs_s(&outSize, wtext, strlen(szFullPath) + 1, szFullPath, strlen(szFullPath));

	cStaticMesh loader;
	cXMesh* pStaticMesh;
	pStaticMesh = loader.xFimeLoad(szFullPath);

	m_mapStaticMesh[szFullPath] = pStaticMesh;
}

void cStaticMeshManager::AddStaticMesh(wstring szFullPath)
{
	string  path;
	path.assign(szFullPath.begin(), szFullPath.end());

	cStaticMesh loader;
	cXMesh* pStaticMesh;
	pStaticMesh = loader.xFimeLoad(path.c_str());

	m_mapStaticMesh[path] = pStaticMesh;
}

cXMesh * cStaticMeshManager::GetStaticMesh(const char * szFullPath)
{

	if (m_mapStaticMesh.find(szFullPath) == m_mapStaticMesh.end())
	{
		wchar_t wtext[1024];
		size_t outSize;
		mbstowcs_s(&outSize, wtext, strlen(szFullPath) + 1, szFullPath, strlen(szFullPath));

		cStaticMesh loader;
		cXMesh* pStaticMesh;
		pStaticMesh = loader.xFimeLoad(szFullPath);

		m_mapStaticMesh[szFullPath] = pStaticMesh;
	}

	return m_mapStaticMesh[szFullPath];
}

cXMesh * cStaticMeshManager::GetStaticMesh(wstring szFullPath)
{
	string  path;
	path.assign(szFullPath.begin(), szFullPath.end());

	if (m_mapStaticMesh.find(path) == m_mapStaticMesh.end())
	{
		cStaticMesh loader;
		cXMesh* pStaticMesh;
		pStaticMesh = loader.xFimeLoad(path.c_str());

		m_mapStaticMesh[path] = pStaticMesh;
	}
	if (m_mapStaticMesh[path]->GetNumMaterial() > 1000000000)
	{
		cStaticMesh loader;
		cXMesh* pStaticMesh;
		pStaticMesh = loader.xFimeLoad(path.c_str());

		m_mapStaticMesh[path] = pStaticMesh;
	}
	return m_mapStaticMesh[path];
}

void cStaticMeshManager::Destroy()
{
	for (auto p : m_mapStaticMesh)
	{
		//p.second->Destroy();
		SAFE_DELETE(p.second);
	}
}
