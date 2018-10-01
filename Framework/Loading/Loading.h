#pragma once

enum LOAD_KIND
{
	LOAD_KIND_TEXTURE,
	LOAD_KIND_SOUND,
	LOAD_KIND_SKINNEDMESH,
	LOAD_KIND_STATICMESH,
	LOAD_KIND_PARTICLE,
	LOAD_KIND_END
};
//�̹��� ���ҽ� ����ü
struct tagImageResource
{
	const char* fileName;	//���� �̸�
};
// ���� ���ҽ� ����ü
struct tagSoundResouce
{
	string keyName;
	const char* fileName;
	bool bgm;
	bool loop;
};

struct tagTextureResouce
{
	const char *  szFullPath;
};
struct tagSkinnedMeshResouce
{
	const char *  szFolderName;
	const char *  szFileName;
};

struct tagStaticMeshResouce
{
	const char * fileName;
};

struct tagParticleResource
{
	const char * fileName;
	string		 key;
};
class LoadItem
{
private:
	LOAD_KIND _kind;
	tagImageResource		_imageResource;
	tagTextureResouce		_textureResource;
	tagSoundResouce			_soundResource;
	tagStaticMeshResouce	_staticMeshResource;
	tagSkinnedMeshResouce	_skinnedMeshResource;
	tagParticleResource		_particleResource;

public:

	// �ؽ���
	HRESULT InitForTexture(const char* szFullPath);
	HRESULT InitForTexture(wstring szFullPath);
	// ��Ų�� �޽�
	HRESULT InitForSkinnedMesh(const char* szFolderName, const  char* szFileName);
	HRESULT InitForSkinnedMesh(wstring szFullPath);
	// ����ƽ �޽�
	HRESULT InitForStaticMesh(const char * szFileName);
	HRESULT InitForStaticMesh(wstring szFullPath);

	// Ű ������ ���� ���� �ʱ�ȭ
	HRESULT IniteForSound(string KeyName, const char* fileName, bool bgm = FALSE, bool Loop = false);
	HRESULT IniteForSound(string KeyName, wstring szFullPath, bool bgm = FALSE, bool Loop = false);

	// ��ƼŬ
	HRESULT IniteForParticle(string key, const char * fileName);
	HRESULT IniteForParticle(string key, wstring szFullPath);

	//�ε� ������ ���� ��������
	LOAD_KIND GetLoadingKind() { return _kind; }
	//�̹��� ���ҽ� ��������
	tagImageResource GetImageResource() { return _imageResource; }
	tagSoundResouce GetSoundResource() { return _soundResource; }

	tagTextureResouce GetTextrueResource() { return _textureResource; }
	tagStaticMeshResouce GetStaticMeshResource() { return _staticMeshResource; }
	tagSkinnedMeshResouce GetSkinnedResource() { return _skinnedMeshResource; }
	tagParticleResource GetParticleResource() { return _particleResource; }
};


class Loading : public cScene
{
private:
	typedef vector<LoadItem*> arrLoadItem;
private:
	arrLoadItem _vLoadItem;

	//�ε�ȭ�鿡�� ����� �̹��� �� �ε���
	cSprite*					m_pBackGround;
	//cSprite*					_loading;
	//cSprite*					_endLoading;
	ProgressBar*				_loadingBar;
	int							_currentGauge;

public:
	Loading();
	~Loading();

	void Setup(const char * backTexture);
	void Setup();
	void Release();
	void Update();
	void Render();

	// �ؽ���
	HRESULT InitForTexture(const char* szFullPath);
	HRESULT InitForTexture(wstring szFullPath);
	// ��Ų�� �޽�
	HRESULT InitForSkinnedMesh(const char* szFolderName, const char* szFileName);
	HRESULT InitForSkinnedMesh(wstring szFullPath);

	// ����ƽ �޽�
	HRESULT InitForStaticMesh(const char * szFileName);
	HRESULT InitForStaticMesh(wstring szFullPath);

	// ��ƼŬ
	HRESULT IniteForParticle(string key, const char * fileName);
	HRESULT IniteForParticle(string key, wstring szFullPath);
	
	// ���� ����
	HRESULT InitForSound(string keyName, const char* fileName, bool bgm = false, bool loop = false);
	HRESULT InitForSound(string keyName, wstring szFullPath, bool bgm = false, bool loop = false);

	//�ε��� �Ϸ� ���???????(�ε� �Ϸ��� �� ����)
	BOOL LoadingDone();
	//�ε� ������ ���� ��������
	vector<LoadItem*> GetLoadItem() { return _vLoadItem; }
};

