#include "stdafx.h"

HRESULT LoadItem::InitForTexture(const char * szFullPath)
{
	_kind = LOAD_KIND_TEXTURE;

	memset(&_textureResource, 0, sizeof(tagTextureResouce));
	_textureResource.szFullPath = szFullPath;
	return S_OK;
}

HRESULT LoadItem::InitForTexture(wstring szFullPath)
{
	string * path = new string;
	path->assign(szFullPath.begin(), szFullPath.end());

	_kind = LOAD_KIND_TEXTURE;

	memset(&_textureResource, 0, sizeof(tagTextureResouce));
	_textureResource.szFullPath = path->c_str();
	return S_OK;
}

HRESULT LoadItem::InitForSkinnedMesh(const char * szFolderName, const  char * szFileName)
{
	_kind = LOAD_KIND_SKINNEDMESH;

	memset(&_skinnedMeshResource, 0, sizeof(tagSkinnedMeshResouce));
	_skinnedMeshResource.szFolderName = szFolderName;
	_skinnedMeshResource.szFileName = szFileName;

	return S_OK;
}

HRESULT LoadItem::InitForSkinnedMesh(wstring szFullPath)
{
	string * path = new string;
	path->assign(szFullPath.begin(), szFullPath.end());

	_kind = LOAD_KIND_SKINNEDMESH;

	memset(&_skinnedMeshResource, 0, sizeof(tagSkinnedMeshResouce));
	_skinnedMeshResource.szFolderName = "";
	_skinnedMeshResource.szFileName = path->c_str();
	
	return S_OK;
}

HRESULT LoadItem::InitForStaticMesh(const char * szFileName)
{
	_kind = LOAD_KIND_STATICMESH;

	memset(&_staticMeshResource, 0, sizeof(tagStaticMeshResouce));
	_staticMeshResource.fileName = szFileName;

	return S_OK;
}

HRESULT LoadItem::InitForStaticMesh(wstring szFullPath)
{
	string * path = new string;
	path->assign(szFullPath.begin(), szFullPath.end());

	_kind = LOAD_KIND_STATICMESH;

	memset(&_staticMeshResource, 0, sizeof(tagStaticMeshResouce));
	_staticMeshResource.fileName = path->c_str();

	return S_OK;
}

HRESULT LoadItem::IniteForSound(string KeyName, const char* fileName, bool bgm, bool Loop)
{
	_kind = LOAD_KIND_SOUND;

	memset(&_soundResource, 0, sizeof(tagSoundResouce));
	_soundResource.keyName = (string)KeyName;
	_soundResource.fileName = fileName;
	_soundResource.bgm = bgm;
	_soundResource.loop = Loop;

	return S_OK;
}

HRESULT LoadItem::IniteForSound(string KeyName, wstring szFullPath, bool bgm, bool Loop)
{
	string * path = new string;
	path->assign(szFullPath.begin(), szFullPath.end());

	_kind = LOAD_KIND_SOUND;

	memset(&_soundResource, 0, sizeof(tagSoundResouce));
	_soundResource.keyName = (string)KeyName;
	_soundResource.fileName = path->c_str();
	_soundResource.bgm = bgm;
	_soundResource.loop = Loop;

	return S_OK;
}

HRESULT LoadItem::IniteForParticle(string key, const char * fileName)
{
	_kind = LOAD_KIND_PARTICLE;
	memset(&_particleResource, 0, sizeof(tagParticleResource));
	_particleResource.key = (string)key;
	_particleResource.fileName = fileName;
	return S_OK;
}

HRESULT LoadItem::IniteForParticle(string key, wstring szFullPath)
{
	string * path = new string;
	path->assign(szFullPath.begin(), szFullPath.end());
	
	_kind = LOAD_KIND_PARTICLE;
	memset(&_particleResource, 0, sizeof(tagParticleResource));
	_particleResource.key = (string)key;
	_particleResource.fileName = path->c_str();

	return S_OK;
}

Loading::Loading()
	: m_pBackGround(NULL)
	, _loadingBar(NULL)
{
}

Loading::~Loading()
{
	SAFE_DELETE(m_pBackGround);
	SAFE_DELETE(_loadingBar);
}

void  Loading::Setup()
{
	
}
void  Loading::Release()
{
	
}
void Loading::Setup(const char * backTexture)
{
	//로딩화면 백그라운드 초기화
	m_pBackGround = TEXTURE->GetSprite(backTexture);

	//_loading = IMAGEMANAGER->AddImage("endLoading", "images/intro/console_loading.bmp", WINSIZEX / 2 - 96, 500, 192, 46, true, RGB(255, 0, 255));
	//_endLoading = IMAGEMANAGER->AddImage("loading", "images/intro/continue.bmp",
	//	WINSIZEX / 2 - 220, 500, 440, 36, true, RGB(255, 0, 255));



	//로딩바 초기화
	_loadingBar = new ProgressBar;
	_loadingBar->Setup(
		"Resource/Texture/SceneTexture/LoadingBar.png",
		"Resource/Texture/SceneTexture/BGLoadingBar.png",
		WINSIZEX / 2 - WINSIZEX * 3.0f / 8.0f, WINSIZEY - 100, WINSIZEX * 3.0f / 4.0f, 28
	);
	//
	_loadingBar->SetGauge(0, 0);

	////로딩바 게이지 초기화
	_currentGauge = 0;

}

void  Loading::Update()
{
	_loadingBar->Update();
}
void  Loading::Render()
{
	m_pBackGround->Render(D3DXVECTOR3(m_pBackGround->textureInfo.Width / 2, 0, 0),
		D3DXVECTOR3(WINSIZEX / 2, 0, 0));

	LoadingDone();
	_loadingBar->Render();
	// 벡그라운드 
}


HRESULT Loading::InitForTexture(const char * szFullPath)
{
	LoadItem * item = new LoadItem;
	item->InitForTexture(szFullPath);
	_vLoadItem.push_back(item);
	return S_OK;
}

HRESULT Loading::InitForTexture(wstring szFullPath)
{
	LoadItem * item = new LoadItem;
	item->InitForTexture(szFullPath);
	_vLoadItem.push_back(item);
	return S_OK;
}

HRESULT Loading::InitForSkinnedMesh(const char * szFolderName, const  char * szFileName)
{
	LoadItem * item = new LoadItem;
	item->InitForSkinnedMesh(szFolderName, szFileName);
	_vLoadItem.push_back(item);
	return S_OK;
}

HRESULT Loading::InitForSkinnedMesh(wstring szFullPath)
{
	LoadItem * item = new LoadItem;
	item->InitForSkinnedMesh(szFullPath);
	_vLoadItem.push_back(item);
	return S_OK;
}

HRESULT Loading::InitForStaticMesh(const char * szFileName)
{
	LoadItem * item = new LoadItem;
	item->InitForStaticMesh(szFileName);
	_vLoadItem.push_back(item);
	return S_OK;
}

HRESULT Loading::InitForStaticMesh(wstring szFullPath)
{
	LoadItem * item = new LoadItem;
	item->InitForStaticMesh(szFullPath);
	_vLoadItem.push_back(item);
	return S_OK;
}

HRESULT Loading::IniteForParticle(string key, const char * fileName)
{
	LoadItem * item = new LoadItem;
	item->IniteForParticle(key, fileName);
	_vLoadItem.push_back(item);

	return S_OK;
}

HRESULT Loading::IniteForParticle(string key, wstring szFullPath)
{
	LoadItem * item = new LoadItem;
	item->IniteForParticle(key, szFullPath);
	_vLoadItem.push_back(item);

	return S_OK;
}

HRESULT Loading::InitForSound(string keyName, const char * fileName, bool bgm, bool loop)
{
	LoadItem * item = new LoadItem;
	item->IniteForSound(keyName, fileName, bgm, loop);

	_vLoadItem.push_back(item);
	return S_OK;
}

HRESULT Loading::InitForSound(string keyName, wstring szFullPath, bool bgm, bool loop)
{
	LoadItem * item = new LoadItem;
	item->IniteForSound(keyName, szFullPath, bgm, loop);

	_vLoadItem.push_back(item);
	return S_OK;
}

// 키값으로 프레임 이미지 파일 초기화

//로딩이 완료 됬니???????(로딩 완료휴 씬 변경)
BOOL  Loading::LoadingDone()
{
	if (_currentGauge >= _vLoadItem.size())
	{
		return true;
	}

	LoadItem * item = _vLoadItem[_currentGauge];

	tagImageResource img;
	tagTextureResouce texture;
	tagStaticMeshResouce staticMesh;
	tagSkinnedMeshResouce skinnedMesh;
	tagSoundResouce sound;
	tagParticleResource particle;

	switch (item->GetLoadingKind())
	{
	case LOAD_KIND_TEXTURE:
		texture = item->GetTextrueResource();
		TEXTURE->AddTexture(texture.szFullPath);
		break;
	case LOAD_KIND_SOUND:
		sound = item->GetSoundResource();
		SOUND->AddSound(sound.keyName, sound.fileName, sound.bgm, sound.loop);
		break;
	case LOAD_KIND_SKINNEDMESH:
		skinnedMesh = item->GetSkinnedResource();
		SKINNEDMESH->AddSkinnedMesh(skinnedMesh.szFolderName, skinnedMesh.szFileName);
		break;
	case LOAD_KIND_STATICMESH:
		staticMesh = item->GetStaticMeshResource();
		STATICMESH->AddStaticMesh(staticMesh.fileName);
		break;
	case LOAD_KIND_PARTICLE:
		particle = item->GetParticleResource();
		PARTICLE->AddParticle(particle.key, particle.fileName);
	}

	//// 로딩바 이미지 변경
	_loadingBar->SetGauge(_currentGauge, _vLoadItem.size());
	////카운트
	_currentGauge++;

	return false;
}