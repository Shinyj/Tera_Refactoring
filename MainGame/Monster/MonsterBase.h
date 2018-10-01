#pragma once

class iMap;

class MonsterBase : public cGameObject
{
public:
	string			m_sName;
	float			m_fSpeed;

	//iMap*			m_pMap;
	SYNTHESIZE(iMap*, m_pMap, Map);


	// 기본 적으로 몬스터가 갖는 속성


	float			m_fAttack;
	float			m_fDefense;


	float			m_fCosVal; // 카메라 기준 각도

	D3DXMATRIX		m_matAnimWorld;
	D3DXVECTOR3		m_vCurAnimPos;
	D3DXVECTOR3		m_vBeforeAnimPos;


	float			m_fHpMax;
	float			m_fHpCur;
	float			m_fHpUI;

	float			m_fMpMax;
	float			m_fMpCur;
	float			m_fMpUI;

	//구간 애니메이션이 진행중이냐
	bool			m_bAnimation;
	bool			m_bAngleLock;

	//몬스터 네비게이션 관련

	bool			m_bIsGen;									//몬스터 젠여부. 죽어 사라지면 false.
	bool			m_bAwake;									//캐릭터를 인지했을경우 True, 다시 멀어지면 false
	bool			m_bFight;									//특정거리로 좁혀지면 전투시작.
	SYNTHESIZE_REF(D3DXVECTOR3, m_vBehaviorSpot, Spot);			//몬스터 활동 지점
	float			m_fAreaRadius;								//중점을 기준으로 한 몬스터 활동반경.
	float			m_fTracableArea;							//추적영역
	float			m_fRunSpeed;								//몬스터 이동속도, Awake된 경우에는 별도로 +처리.
	float			m_fTimeofDeath;								//몬스터죽은 시점을 기록. 몬스터가 죽은후 일정시간뒤에 다시 해당위치에 부활.
	float			m_fEscapeTime;								//인식범위에서 탈출한 시점을 기록
	bool			m_bEscapeToggle;							//토글변수를 이용하여 시점 기록
	float			m_fEscapeTime2;
	bool			m_bEscapeToggle2;							//토글변수를 이용하여 시점 기록(전투인 경우)
	bool			m_bIdle;									//아무인식도 없는 상태. 
	bool			m_bAtkTerm;									//공격사이사이의 쉬는거

	bool			m_bDeath;									//죽음을 알리는 불변수
	bool			m_bFlinch;									//피격을 알리는 불변수


																//어슬렁거리는 모션 관련.
	D3DXVECTOR3		NextSpot;									//다음지점
	float			m_fStopTime;								//멈춘시점.
	bool			m_bStart;								//멈춘시점기록을 위한 불변수.
	bool			m_bWalkOnOff;								//걷느냐 마느냐. 걷는시점에선 걷는 모션
																//아니면 멈추는 모션.

																// 패턴 가짓 수
	int				m_nNumofPattern;

	cParticleSet*		m_pParticleBleeding;					//파티클 효과 처리.

																//DeathShader
	bool				DissapearingMode = false;				//몬스터 죽을때 데스쉐이딩 적용할때 쓰는 변수들.

	LPD3DXEFFECT		DeathShader = NULL;
	LPDIRECT3DTEXTURE9	SKIN = NULL;

	// 체력바
	ProgressBar*		m_pHpBar;
	ProgressBar*		m_pMpBar;
	cSprite*			m_BackBar;

public:


public:
	MonsterBase();
	~MonsterBase();

	void Setup();
	void Update();
	void Render();

	//몬스터 높이맵 적용
	void PlusMapHeight();

	//몬스터 어슬렁거리는것 구현
	virtual void MonoBehavior(void);

	// 공격
	virtual bool Attack(float damage);

	//몬스터 서식지 적용.
	virtual void SetupBehaviorSpot(D3DXVECTOR3	v) { m_vBehaviorSpot = v; }

	virtual void ItemDrop(const char * itemName);

	void SetUpStateBar();
	void UpdateUpStateBar();
	void RenderUpStateBar(string name);

};

