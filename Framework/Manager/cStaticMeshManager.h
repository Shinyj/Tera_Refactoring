#pragma once

#define STATICMESH cStaticMeshManager::GetInstance()

class cXMesh;

class cStaticMeshManager
{
private:
	std::map<std::string, cXMesh*>	m_mapStaticMesh;

private:
	SINGLETONE(cStaticMeshManager);


public:
	void AddStaticMesh(const char * szFullPath);
	void AddStaticMesh(wstring szFullPath);
	cXMesh * GetStaticMesh(const char * szFullPath);
	cXMesh * GetStaticMesh(wstring szFullPath);

	void Destroy();
};

