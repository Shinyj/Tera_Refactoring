#pragma once

// 파티클 점 하나의 객체
class cParticle
{
private:

	//bool				m_isUse;		// 사용여부 
	SYNTHESIZE(bool, m_isUse, IsUse);
	ST_PC_VERTEX		m_stPaticle;	// 실제로 렌더될 버텍스

	D3DXMATRIX			m_matTS;

	float				m_fFirstSpeed;
	float				m_fTime;		// 파티클의 생존 시간
	LPDIRECT3DTEXTURE9	m_pTexture;		// 파티클의 텍스쳐

	D3DXVECTOR3			m_vPosition;	// 파티클의 위치
	D3DXVECTOR3			m_vDirecton;	// 파티클의 이동 방향
	float				m_fSpeed;		// 파티클의 속도
	float				m_fAcc;			// 파티클의 가속도
	float				m_fRdian;		// 파티클의 회전각
	D3DXCOLOR			m_stColor;		// 파티클의 색상

	float				scale;

	float				m_curTime;
public:
	cParticle();
	~cParticle();

	void Setup(float _fTime, D3DXVECTOR3 _vPos, D3DXVECTOR3 _vDir,
		float _speed, float _acc, float _radian, D3DXCOLOR _color,
		const char * szFile);

	void Setup();
	void Update(D3DXMATRIX world);
	void Render();
};

enum PARTICLE_TYPE
{
	PTC_TYPE_LOOP,
	PTC_TYPE_ONCE
};

// 파티클 집단
class cParticleSet
{
private:

	SYNTHESIZE(D3DXMATRIX, m_matWorld, World);

	bool					m_isStart;

	vector<cParticle*>		m_vecParticle;
	PARTICLE_TYPE			m_type;

	float					m_fTime;		// 파티클 수명
	float					m_fSpeed;

	float					m_fAcc;
	int						m_fAccMin;
	int						m_fAccMax;

	float					m_fCurTime;

	// 초기 위치들
	int						m_fPositionX;
	int						m_fRandPosXMin;
	int						m_fRandPosXMax;

	int						m_fPositionY;
	int						m_fRandPosYMin;
	int						m_fRandPosYMax;

	int						m_fPositionZ;
	int						m_fRandPosZMin;
	int						m_fRandPosZMax;

	// 방향 값 ( 렌덤부분은 -1.0f ~ 1.0f )
	int						m_fDirectionX;
	int						m_fRandDirXMin;
	int						m_fRandDirXMax;

	int						m_fDirectionY;
	int						m_fRandDirYMin;
	int						m_fRandDirYMax;

	int						m_fDirectionZ;
	int						m_fRandDirZMin;
	int						m_fRandDirZMax;

	char 					m_szFile[1024];
	D3DXCOLOR				m_stColor;
	LPDIRECT3DTEXTURE9		m_pTexture;

	bool					useIt = false;
public:
	cParticleSet();
	~cParticleSet();

	void Setup(PARTICLE_TYPE type, float time, float speed,
		int acc, int accMin, int accMax,
		int posX, int randPosXMin, int randPosXMax,
		int posY, int randPosYMin, int randPosYMax,
		int posZ, int randPosZMin, int randPosZMax,
		int dirX, int randDirXMin, int randDirXMax,
		int dirY, int randDirYMin, int randDirYMax,
		int dirZ, int randDirZMin, int randDirZMax,
		const char * szFile, D3DXCOLOR color);
	void Update();

	void Render();
	void RenderOnce();

	void Start();
	void End();
};


// 파티클 컨트롤러 ( 매니저 )

class cParticleManager
{
	SINGLETONE(cParticleManager);

private:

	map<string, cParticleSet*>	m_mapParticleSet;

	vector<cParticleSet*>		m_vecParticle;
public:

	void AddParticle(string key, const char * szFile);
	cParticleSet* GetParticle(string key);

	void AddChild(cParticleSet* child);

	void Destroy();

	void Update();
	void Render();

};

#define PARTICLE	cParticleManager::GetInstance()
