#include "stdafx.h"
#include "Beast.h"

Beast::Beast()
	: m_pMonster(NULL)
	, m_currState(MON_STATE_unarmedwait)
{
	m_state = MON_STATE_unarmedwait;

	//루프애니메이션이 아닐때, 동작이 끝난걸 알려주는 변수.
	m_bIsDone = true;

	m_fCurAnimTime = 0.0f;
	m_fTime = 0.0f;
	m_nTime = 0;
	float animTime[MON_STATE_COUNT] =
	{
		40,		60,		50,		23,		41,
		31,		71,		31,		141,	30,
		31,		22,		1,		131,	101,
		106,	51,		56
	};

	memcpy(m_fAnimTime, animTime, sizeof(animTime));

	for (int i = 0; i < MON_STATE_COUNT; i++)
		m_fAnimTime[i] -= 1.0f;
	for (int i = 0; i < MON_STATE_COUNT; i++)
		m_fAnimTime[i] /= 30.0f;

	// 블랜딩 처리 할거니
	//블렌드 처리하니깐 제식해버리더라구. 그래서 해제시켰다.
	m_bIsBlend = false;

	//Monster02은 이런 특성을 가지고 있다.
	m_fAreaRadius = 100.0f;
	m_fTracableArea = 1000.0f;
	m_fRunSpeed = 1.2f;
	m_fFightZone = 50.0f;
	m_fHpCur = m_fHpMax;

	MODE = IDLE;

	// 패턴의 가짓 수
	m_nNumofPattern = 2;

	m_pParticleBleeding = PARTICLE->GetParticle("Bleeding");
	PARTICLE->AddChild(m_pParticleBleeding);
}


Beast::~Beast()
{
	SAFE_DELETE(m_pMonster);
	SAFE_DELETE(m_pSphereR);
	SAFE_DELETE(m_pSphereL);

	//셰이더 관련
	SAFE_RELEASE(DeathShader);
	SAFE_RELEASE(SKIN);
}

void Beast::Setup(D3DXVECTOR3 v)
{
	MonsterBase::Setup();

	m_sName = "야생곰";

	//처음에 얘로 셋팅해놓는다.
	//처음 젠되는 위치 설정
	m_vBehaviorSpot = v;//D3DXVECTOR3(1247, 0, 3578);
	m_vPosition = v;

	m_fHpMax = 105.0f;
	m_fHpCur = 105.0f;
	m_fAttack = 10.0f;
	m_fDefense = 5.0f;

	m_pMonster = new cSkinnedMesh;
	m_pMonster->Setup("Resource/XFile/Monster", "Bear.X");
	m_pMonster->SetAnimationIndexBlend(m_currState);

	// 위치를 가진 루트 본
	m_pDummyRoot = (ST_BONE*)D3DXFrameFind(m_pMonster->GetFrame(),
		"Dummy_root");

	m_pBIP = (ST_BONE*)D3DXFrameFind(m_pMonster->GetFrame(),
		"Bip01");

	// 싸대기 때릴 양손 본
	m_pHandR = (ST_BONE*)D3DXFrameFind(m_pMonster->GetFrame(),
		"Bip01-R-Hand");

	m_pHandL = (ST_BONE*)D3DXFrameFind(m_pMonster->GetFrame(),
		"Bip01-L-Hand");

	////양발 판정 구체
	m_pSphereR = new cSpere;
	m_pSphereR->Setup(D3DXVECTOR3(0, 0, 0), 5);

	m_pSphereL = new cSpere;
	m_pSphereL->Setup(D3DXVECTOR3(0, 0, 0), 5);




	// 바운딩 박스 생성
	m_pBoundingBox = new cBoundingBox;
	m_pBoundingBox->Setup(D3DXVECTOR3(-15, 0, -15), D3DXVECTOR3(15, 20, 15));

	// 구 충돌 영역 생성(싸움존 거리)
	m_pSpere = new cSpere;
	m_pSpere->Setup(m_vPosition, m_fFightZone);


	//셰이더관련
	DeathShader = Shader::LoadShader("Resource/XFile/Monster", "DeathShader.fx");
	SKIN = TEXTURE->GetTexture("Resource/XFile/Monster/EnragedBear_diff.tga");

	m_pRimLight = Shader::LoadShader("Resource/Shader/Effect/", "RimFlash.fx");
	m_pRimLight->SetFloat("Offset", 0.2f);

	MODE = IDLE;
}

void Beast::Update()
{
	m_pDummyRoot->TransformationMatrix._42 = m_matWorld._42;

	switch (MODE)
	{
	case IDLE:
		Idle();
		break;
	case AWAKE:
		Awake();
		break;
	case CHASE:
		Chase();
		break;
	case BATTLE:
		Battle();
		break;
	case RETURN:
		Return();
		break;
	case DEATH:
		Death();
		break;
	case DISAPPEAR:
		Disappear();
		break;
	case REBIRTH:
		Rebirth();
		break;
	}

	// 애니메이션 진행
	if (m_currState != m_state)
	{
		if (m_bIsBlend)
		{
			m_pMonster->SetAnimationIndexBlend(m_state);

		}
		else
		{
			m_pMonster->SetAnimationIndex(m_state);

		}

		m_currState = m_state;

		m_pMonster->AnimAdvanceTime();
	}

	// 이동값이 있는 애니메이션의 업데이트면
	// 애니메이션 스타트 지점의 월드에서 부터 업데이트
	// 아니면 평소처럼 적용된 월드에서 업데이트
	if (isUseLocalAnim())
	{
		m_matAnimWorld._42 = m_matWorld._42;

		m_pMonster->Update(m_matAnimWorld);
	}
	else
	{
		m_pMonster->Update(m_matWorld);

	}
	m_vBeforeAnimPos = m_vCurAnimPos;
	m_vCurAnimPos = D3DXVECTOR3(m_pDummyRoot->TransformationMatrix._41, 0, 0);



	m_pSphereR->SetWorld(m_pHandR->CombinedTransformationMatrix);
	m_pSphereL->SetWorld(m_pHandL->CombinedTransformationMatrix);

	//이거 안해주면 몬스터 이상한데 보면서 간다~
	m_fRotY = m_fCosVal;

	// 이동값이 있는 애니메이션 적용 시
	// 애니메이션 로컬을 현재 포지션으로 적용시키는 증가량을 계산 
	//D3DXMATRIX matR, matT;

	if (m_vCurAnimPos.x - m_vBeforeAnimPos.x < 30.f)
		m_vPosition += (m_vDirection * (m_vCurAnimPos.x - m_vBeforeAnimPos.x));
	else
	{
		m_vCurAnimPos = D3DXVECTOR3(0, 0, 0);
		m_vBeforeAnimPos = D3DXVECTOR3(0, 0, 0);
	}


	D3DXMATRIX mat, matR, matT;
	D3DXMatrixRotationY(&matR, m_fRotY);
	D3DXVec3TransformNormal(&m_vDirection, &D3DXVECTOR3(1, 0, 0), &matR);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);


	m_matWorld = matR * matT;
	m_pBoundingBox->SetWorld(m_matWorld);
	m_pSpere->SetWorld(m_matWorld);

	MonsterBase::Update();
}

void Beast::Idle()
{
	//기다리는과정(첫세팅)
	if (!m_bWalkOnOff && !m_bStart)
	{
		m_bStart = true;
		m_state = MON_STATE_unarmedwait;
		m_fStopTime = (float)GetTickCount();
	}
	//기다리는 과정
	else if (!m_bWalkOnOff)
	{
		if (GetTickCount() - m_fStopTime > 4000.0f)
		{
			m_bWalkOnOff = true;
			m_bStart = false;
		}
	}
	//걷는과정.처음셋팅할때
	else if (m_bWalkOnOff && !m_bStart)
	{
		m_bStart = true;
		//시간없어서 패트롤 영역을 사각형으로 처리했어요 미안해 ㅠㅠ
		int plusminus = rand() % 2;
		int plusminus2 = rand() % 2;

		if (plusminus)
			plusminus = 1;
		else
			plusminus = -1;

		if (plusminus2)
			plusminus2 = 1;
		else
			plusminus2 = -1;

		int AreaX = rand() % (int)(m_fAreaRadius / 5.0f);
		int AreaZ = rand() % (int)(m_fAreaRadius / 5.0f);

		NextSpot = m_vBehaviorSpot + D3DXVECTOR3(plusminus*AreaX, 0, plusminus2*AreaZ);
	}
	//임의의 지점까지 걷는 모션.
	else if (m_bWalkOnOff)
	{
		m_state = MON_STATE_Walk;
		// u벡터 -> 기준벡터
		D3DXVECTOR3 u = D3DXVECTOR3(1, 0, 0);
		D3DXVECTOR3 v, v2;
		v2 = NextSpot - m_vPosition;
		//이거안해주면 높이맵인식되서 문워크한다.
		v2.y = 0;
		D3DXVec3Normalize(&v, &v2);

		m_fCosVal = D3DXVec3Dot(&v, &u);
		if (m_fCosVal < -1.0f)
			m_fCosVal = -0.99f;
		else if (m_fCosVal > 1.0f)
			m_fCosVal = 0.99;
		m_fCosVal = acosf(m_fCosVal);

		if (m_vPosition.z < NextSpot.z)
			m_fCosVal = D3DX_PI * 2 - m_fCosVal;

		m_vPosition += (0.3f*m_fRunSpeed * v);

		//이거 오차범위 넉넉하게 줘야 한다. 안그러면 정확하게 맞추려고 부르르르르 떤다. 
		if (DistanceXZ(NextSpot, m_vPosition) < 1.0f)
		{
			m_bWalkOnOff = false;
			m_bStart = false;
		}
	}

	float Distance_Player_Monster = DistanceXZ(*g_vPlayerPos, m_vPosition);
	if (Distance_Player_Monster < m_fAreaRadius)
	{
		MODE = AWAKE;
		m_bWalkOnOff = false;
		m_bStart = false;
		return;
	}
}

void Beast::Awake()
{
	m_state = MON_STATE_Wait;
	// u벡터 -> 기준벡터
	D3DXVECTOR3 u = D3DXVECTOR3(1, 0, 0);
	D3DXVECTOR3 v;
	D3DXVec3Normalize(&v, &(*g_vPlayerPos - m_vPosition));

	m_fCosVal = D3DXVec3Dot(&v, &u);
	if (m_fCosVal < -1.0f)
		m_fCosVal = -0.99f;
	else if (m_fCosVal > 1.0f)
		m_fCosVal = 0.99;
	m_fCosVal = acosf(m_fCosVal);

	if (m_vPosition.z < g_vPlayerPos->z)
		m_fCosVal = D3DX_PI * 2 - m_fCosVal;

	//경계모드에서 경계거리에서 벗어나면 다시 Idle로 진입
	if (DistanceXZ(*g_vPlayerPos, m_vPosition) > m_fAreaRadius)
	{
		MODE = IDLE;
		m_fTime = 0.0f;
		return;
	}

	//경계모드에서 싸움존으로 거리가 좁혀지면 교전시작.
	if (DistanceXZ(*g_vPlayerPos, m_vPosition) < m_fFightZone)
	{
		MODE = BATTLE;
		m_fTime = 0.0f;
		return;
	}

	//SOUNDMANAGER->Play("M2_MON_STATE_Idle");
}

void Beast::Chase()
{//배틀모드->추격모드일 경우 피격모드에서 쓰였던 bool변수와 float변수를 초기화해준다.(중복방지)
	if (GetDamaged)
	{
		GetDamaged = false;
		DamageTerm = 0.0f;
	}


	//이 함수에서는 항상 몬스터와 플레이어간 거리가 중요하므로 항상 제일 먼저 갱신해준다.
	float Distance_Player_Monster = DistanceXZ(*g_vPlayerPos, m_vPosition);

	//싸움존 내부로 거리가 좁혀지면 다시 배틀모드. 그리고
	//만일 경계선근처에서 싸우게 되면 배틀존 내부에서 있게되어 싸우는게 우선이다.
	if (Distance_Player_Monster < m_fFightZone)
	{
		MODE = BATTLE;
		return;
	}
	else if (DistanceXZ(m_vPosition, m_vBehaviorSpot) > m_fTracableArea)
	{
		MODE = RETURN;
		return;
	}

	m_state = MON_STATE_run;
	// u벡터 -> 기준벡터
	D3DXVECTOR3 u = D3DXVECTOR3(1, 0, 0);
	D3DXVECTOR3 v, v2;
	v2 = *g_vPlayerPos - m_vPosition;
	v2.y = 0;
	D3DXVec3Normalize(&v, &v2);

	m_fCosVal = D3DXVec3Dot(&v, &u);
	if (m_fCosVal < -1.0f)
		m_fCosVal = -0.99f;
	else if (m_fCosVal > 1.0f)
		m_fCosVal = 0.99;
	m_fCosVal = acosf(m_fCosVal);

	if (m_vPosition.z < g_vPlayerPos->z)
		m_fCosVal = D3DX_PI * 2 - m_fCosVal;

	m_vPosition += (m_fRunSpeed * v);
}

void Beast::Battle()
{//최상위조건. 몬스터가 죽으면 나머지 다 꽝이야.
	if (m_fHpCur <= 0)
	{
		SOUND->Play("M2_MON_STATE_Death");
		MODE = DEATH;
		m_bAtkTerm = true;
		m_bAnimation = false;
		m_bAngleLock = false;
		return;
	}

	//싸움을 시작하기전에 항상 조건을 먼저 확인한다. 싸움존에서 벗어나면 추격을 시작.
	if (!m_bAnimation && DistanceXZ(*g_vPlayerPos, m_vPosition) > m_fFightZone)
	{
		MODE = CHASE;
		m_bAtkTerm = true;
		m_bAnimation = false;
		m_bAngleLock = false;
		return;
	}

	//만약 피격당했다면 일정시간동안 무적이어야 한다. GetDamaged를 On시켜서 시간을 잰후에 일정시간이 되면 다시 OFF시킨다.
	if (GetDamaged)
	{
		DamageTerm += TIME->GetEllapsedTime();
		if (DamageTerm > 0.5f)
		{
			GetDamaged = false;
			DamageTerm = 0.0f;
		}

	}

	//앵글락 상태에서는 퍼포먼스도중에 회전하지 않는다.(때리면서 돌아가는거 방지)
	if (!m_bAngleLock)
	{

		D3DXVECTOR3 u = D3DXVECTOR3(1, 0, 0);
		D3DXVECTOR3 v;
		D3DXVec3Normalize(&v, &(*g_vPlayerPos - m_vPosition));

		m_fCosVal = D3DXVec3Dot(&v, &u);
		if (m_fCosVal < -1.0f)
			m_fCosVal = -0.99f;
		else if (m_fCosVal > 1.0f)
			m_fCosVal = 0.99;
		m_fCosVal = acosf(m_fCosVal);

		if (m_vPosition.z < g_vPlayerPos->z)
			m_fCosVal = D3DX_PI * 2 - m_fCosVal;
	}

	//공격을 시작할때(아직 애니메이션을 시작하지 않은 상태여야 한다.)
	if (!m_bAtkTerm && !m_bAnimation && DistanceXZ(*g_vPlayerPos, m_vPosition) < m_fFightZone)
	{
		SetAnimWorld();
		//구간애니메이션이므로 이를 온 시켜준다.
		m_bAnimation = true;
		//공격중에는 방향을 바꾸지 않으므로 앵글락을 온시켜줍니다.
		m_bAngleLock = true;


		int patternNum = rand() % m_nNumofPattern;

		switch (patternNum)
		{
		case 0:
		{
			//전투모션은 atk01타입이다.
			m_state = MON_STATE_atk01;
			//현재 애니메이션 구간길이를 입력해줍니다.
			m_fCurAnimTime = m_fAnimTime[MON_STATE_atk01];

			SOUND->Play("M2_MON_STATE_atk01");
		}
		break;
		case 1:
		{
			m_state = MON_STATE_atk02;
			m_fCurAnimTime = m_fAnimTime[MON_STATE_atk02];

			SOUND->Play("M2_MON_STATE_atk01");
		}
		break;
		}
	}
	else if (m_bAtkTerm && !m_bAnimation)
	{
		//전투는 구간애니메이션 합으로 이루어져있으므로 이렇게 설정함.
		m_bAnimation = true;
		m_state = MON_STATE_Wait;
		m_fCurAnimTime = m_fAnimTime[MON_STATE_Wait];
	}

	//구간애니메이션 처리부분.
	if (m_bAnimation)
	{
		//애니메이션을 처리해야 하므로 시간을 잴 변수가 필요하다.
		m_fTime += TIME->GetEllapsedTime();

		if (m_fCurAnimTime - 0.05 <= m_fTime)
		{
			m_vCurAnimPos = D3DXVECTOR3(0, 0, 0);
			m_vBeforeAnimPos = D3DXVECTOR3(0, 0, 0);
			m_fTime = 0.0f;
			m_bAnimation = false;
			m_bAngleLock = false;
			m_bAtkTerm = !m_bAtkTerm;
		}
	}

	// 공격
	if (m_state == MON_STATE_atk01)
		Attack(m_fAttack);
	if (m_state == MON_STATE_atk02)
		Attack(m_fAttack);
}

void Beast::Return()
{
	if (DistanceXZ(m_vPosition, m_vBehaviorSpot) < 1.0f)
		MODE = IDLE;
	else
	{
		m_state = MON_STATE_run;
		// u벡터 -> 기준벡터
		D3DXVECTOR3 u = D3DXVECTOR3(1, 0, 0);
		D3DXVECTOR3 v;
		D3DXVECTOR3 t;
		t = m_vBehaviorSpot - m_vPosition;
		t.y = 0;
		D3DXVec3Normalize(&v, &t);

		m_fCosVal = D3DXVec3Dot(&v, &u);
		if (m_fCosVal < -1.0f)
			m_fCosVal = -0.99f;
		else if (m_fCosVal > 1.0f)
			m_fCosVal = 0.99;
		m_fCosVal = acosf(m_fCosVal);

		if (m_vPosition.z < m_vBehaviorSpot.z)
			m_fCosVal = D3DX_PI * 2 - m_fCosVal;

		m_vPosition += (1.5f * m_fRunSpeed * v);
	}
}

void Beast::Death()
{
	if (m_state != MON_STATE_Death)
	{
		int n = rand() % 3;
		switch (n)
		{
		case 0:
			ItemDrop("검은마력의신발");
			break;
		case 1:
			ItemDrop("검은마력의장갑");
			break;
		case 2:
			ItemDrop("검은마력의옷");
			break;
		}
	}

	m_fHpCur = 0.0f;

	m_state = MON_STATE_Death;
	m_fCurAnimTime = m_fAnimTime[MON_STATE_Death];
	m_bIsBlend = false;
	//애니메이션을 처리해야 하므로 시간을 잴 변수가 필요하다.
	m_fTime += TIME->GetEllapsedTime();

	if (m_fCurAnimTime <= m_fTime)
	{
		m_vCurAnimPos = D3DXVECTOR3(0, 0, 0);
		m_vBeforeAnimPos = D3DXVECTOR3(0, 0, 0);
		m_fTime = 0.0f;
		MODE = DISAPPEAR;
	}
}

void Beast::Disappear()
{//렌더 과정을 거쳐서 완전히 사라지면 다시 태어나는 모드로.
	if (!m_bIsGen)
	{
		MODE = REBIRTH;
		m_nTime = 0;
		return;
	}
	m_state = MON_STATE_deathwait;
	DissapearingMode = true;
	m_nTime += 1;
}

void Beast::Rebirth()
{
	m_vPosition = m_vBehaviorSpot;
	m_state = MON_STATE_unarmedwait;
	m_fRotY = 0.0f;
	m_vDirection = D3DXVECTOR3(1, 0, 0);
	m_vCurAnimPos = D3DXVECTOR3(0, 0, 0);
	m_fHpCur = m_fHpMax;

	//5초뒤에 부활.
	if (GetTickCount() - m_fTimeofDeath >= 5000.0f)
	{
		MODE = IDLE;
		m_pMonster->AnimAdvanceTime();
		m_bIsGen = true;
	}
}

void Beast::Render()
{
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	DEVICE->SetTransform(D3DTS_WORLD, &mat);

	RimLightSetup(0, 0, 0, 0, 0, 0, 0);

	if (m_bIsGen)
	{
		if (DissapearingMode)
			m_pMonster->Render(NULL, DeathShader, m_nTime, SKIN, DissapearingMode, m_bIsGen, m_fTimeofDeath);
		else
		{
			if (m_isPicked)
				m_pMonster->Render(NULL, m_pRimLight);
			else
				m_pMonster->Render(NULL);
		}
	}

	MonsterBase::Render();

	if (SightSpere && m_pSphereR)
		m_pSphereR->Render();
	if (SightSpere && m_pSphereL)
		m_pSphereL->Render();

	char szTemp[1024];
	sprintf_s(szTemp, 1024, "MODE : %d", MODE);
	RECT rc;
	SetRect(&rc, WINSIZEX - 200, 200, WINSIZEX, 300);

	LPD3DXFONT pFont = FONT->GetFont(cFontManager::FT_DEFAULT);
	pFont->DrawTextA(NULL,
		szTemp,
		strlen(szTemp),
		&rc,
		DT_LEFT | DT_VCENTER,
		D3DCOLOR_XRGB(255, 0, 0));
}

void Beast::SetAnimWorld()
{
	D3DXMATRIX matR, matT;
	D3DXMatrixRotationY(&matR, m_fRotY);
	D3DXVec3TransformNormal(&m_vDirection, &D3DXVECTOR3(1, 0, 0), &matR);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matAnimWorld = matR * matT;
	m_matWorld = matR * matT;
}

bool Beast::isUseLocalAnim()
{
	if (
		m_state == MON_STATE_atk02 ||
		m_state == MON_STATE_atk01
		)
		return true;

	if (m_vCurAnimPos.x - m_vBeforeAnimPos.x > 10.0f)
	{
		m_vCurAnimPos = D3DXVECTOR3(0, 0, 0);
		m_vBeforeAnimPos = D3DXVECTOR3(0, 0, 0);
		return true;
	}
	return false;
}

bool Beast::Attack(float damage)
{
	if (!OBJECTMANAGER->GiveDamagedChara(m_pSphereR, damage, m_vPosition))
	{
		if (OBJECTMANAGER->GiveDamagedChara(m_pSphereL, damage, m_vPosition))
			return true;
	}
	else
		return true;

	return false;
}

void Beast::Damaged(float damage, D3DXVECTOR3 pos)
{
	if (m_state == MON_STATE_deathwait ||
		m_state == MON_STATE_Death || GetDamaged)	 return;

	int Damage = damage - m_fDefense;
	if (Damage < 0) Damage = 0;
	m_fHpCur -= damage;

	if (damage)
	{
		//피경당했다면 피격당했다고 ON시켜준다.
		GetDamaged = true;
		// 블리딩 터트릴 좌표를 만들어서 세팅하고 시작
		D3DXMATRIX matTS, matR, matT;
		D3DXMatrixRotationY(&matR, m_fRotY);
		float x = 30 + m_vPosition.x;
		float y = rand() % 10 + 10 + m_vPosition.y;
		float z = rand() % 60 - 30 + m_vPosition.z;
		D3DXMatrixTranslation(&matT, x, y, z);
		m_pParticleBleeding->SetWorld(matR * matT);
		m_pParticleBleeding->Start();

		//SOUNDMANAGER->Play("M2_MON_STATE_Damage");

	}
}
