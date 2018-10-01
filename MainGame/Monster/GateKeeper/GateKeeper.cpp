#include "stdafx.h"
#include "GateKeeper.h"

GateKeeper::GateKeeper()
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
		97, 41, 49, 66, 35,
		35, 29, 61, 50, 31,
		50, 31, 31, 41, 41,
		30, 31, 40, 1, 131,
		76, 76, 31, 76, 76,
		46, 51
	};

	memcpy(m_fAnimTime, animTime, sizeof(animTime));

	for (int i = 0; i < MON_STATE_COUNT; i++)
		m_fAnimTime[i] -= 1.0f;
	for (int i = 0; i < MON_STATE_COUNT; i++)
		m_fAnimTime[i] /= 30.0f;

	// 블랜딩 처리 할거니
	//블렌드 처리하니깐 제식해버리더라구. 그래서 해제시켰다.
	m_bIsBlend = false;

	//Monster01은 이런 특성을 가지고 있다.
	m_fAreaRadius = 300.0f;
	m_fTracableArea = 1000.0f;
	m_fRunSpeed = 1.0f;
	m_fFightZone = 100.0f;
	m_fHpCur = 100.0f;


	// 패턴의 가짓 수
	m_nNumofPattern = 2;

	m_pParticleBleeding = PARTICLE->GetParticle("Bleeding");
	PARTICLE->AddChild(m_pParticleBleeding);
}


GateKeeper::~GateKeeper()
{
	SAFE_DELETE(m_pMonster);
	SAFE_DELETE(m_pSphereR);
	SAFE_DELETE(m_pSphereL);

	//셰이더 관련
	SAFE_RELEASE(DeathShader);
	SAFE_RELEASE(SKIN);
}

void GateKeeper::Setup(D3DXVECTOR3 v)
{
	MonsterBase::Setup();

	m_sName = "게이트키퍼";

	//처음에 얘로 셋팅해놓는다.
	m_vBehaviorSpot = v;//D3DXVECTOR3(1247, 0, 3578);
	m_vPosition = v;

	m_fHpMax = 500.0f;
	m_fHpCur = 500.0f;
	m_fAttack = 20.0f;
	m_fDefense = 10.0f;

	m_pMonster = new cSkinnedMesh;
	//m_pMonster->Setup("XFile/Monster", "Moster01.X");//Moster01.X
	m_pMonster = SKINNEDMESH->GetSkinnedMesh(MonsterXFilePath + L"Moster01.X");
	m_pMonster->SetAnimationIndexBlend(m_currState);

	// 위치를 가진 루트 본
	m_pDummyRoot = (ST_BONE*)D3DXFrameFind(m_pMonster->GetFrame(),
		"Dummy_root");

	// 싸대기 때릴 양손 본
	m_pHandR = (ST_BONE*)D3DXFrameFind(m_pMonster->GetFrame(),
		"Bip01-R-Hand");

	m_pHandL = (ST_BONE*)D3DXFrameFind(m_pMonster->GetFrame(),
		"Bip01-L-Hand");

	////싸대기 판정 구체
	m_pSphereR = new cSpere;
	m_pSphereR->Setup(D3DXVECTOR3(0, 0, 0), 20);

	m_pSphereL = new cSpere;
	m_pSphereL->Setup(D3DXVECTOR3(0, 0, 0), 20);



	//처음 젠되는 위치 설정

	//m_vPosition = m_vBehaviorSpot;

	// 바운딩 박스 생성
	m_pBoundingBox = new cBoundingBox;
	m_pBoundingBox->Setup(D3DXVECTOR3(-30, 0, -30), D3DXVECTOR3(30, 110, 30));

	// 구 충돌 영역 생성(싸움존 거리)
	m_pSpere = new cSpere;
	m_pSpere->Setup(m_vPosition, m_fFightZone);

	//셰이더관련
	DeathShader = Shader::LoadShader(MonsterXFilePath + L"DeathShader.fx");
	SKIN = TEXTURE->GetTexture(MonsterXFilePath + L"AbandonedAutomatedGuardian_diff.tga");

	m_pRimLight = Shader::LoadShader(ShaderPath + L"RimFlash.fx");
	m_pRimLight->SetFloat("Offset", 0.2);
}

void GateKeeper::Update()
{
	temp = *g_vPlayerPos - m_vPosition;

	tt.x = m_vPosition.x - m_vBehaviorSpot.x;
	tt.y = m_vPosition.z - m_vBehaviorSpot.z;

	m_pDummyRoot->TransformationMatrix._42 = m_matWorld._42;

	if (DissapearingMode)
		m_nTime += 1;

	//몬스터 죽는거
	if (m_fHpCur <= 0 && !DissapearingMode)
		Die();

	//젠되어있을때 
	if (m_bIsGen && !DissapearingMode)
	{

		MonoBehavior();


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
			//m_fTime += TIMEMANAGER->GetEllapsedTime();
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



		//조작

		Move();

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


		MonsterBase::Update();



		m_matWorld = matR * matT;
		m_pBoundingBox->SetWorld(m_matWorld);
		m_pSpere->SetWorld(m_matWorld);
	}

	//죽고 몬스터가 완전히 사라졌을때
	else if (!m_bIsGen)
	{
		//다시 처음으로 설정
		m_nTime = 0;
		m_vPosition = m_vBehaviorSpot;
		m_state = MON_STATE_unarmedwait;
		//m_currState = MON_STATE_unarmedwait;
		m_fRotY = 0.0f;
		m_vDirection = D3DXVECTOR3(1, 0, 0);
		m_vCurAnimPos = D3DXVECTOR3(0, 0, 0);
		m_fHpCur = m_fHpMax;
		m_bAnimation = false;

		//5초뒤에 부활. -> 그냥 젠 안시킬꺼야. 다시 젠시킬려면 isGen을 true로 바꿔줘.
		if (GetTickCount() - m_fTimeofDeath >= 5000.0f)
		{
			m_bIsGen = false;
			m_bDeath = false;
			DissapearingMode = false;
			m_pMonster->AnimAdvanceTime();
		}
	}

	//구간애니메이션 처리부분.
	if (m_bAnimation)
	{
		//애니메이션을 처리해야 하므로 시간을 잴 변수가 필요하다.
		m_fTime += TIME->GetEllapsedTime();

		if (m_state == MON_STATE_Death &&
			m_fCurAnimTime - 0.05 <= m_fTime &&
			!DissapearingMode)
		{
			DissapearingMode = true;
			m_vCurAnimPos = D3DXVECTOR3(0, 0, 0);
			m_vBeforeAnimPos = D3DXVECTOR3(0, 0, 0);
			m_fTime = 0.0f;
			m_state == MON_STATE_deathwait;
			m_bAngleLock = false;
		}

		else if (m_fCurAnimTime - 0.05 <= m_fTime)
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
		Attack(m_fAttack * 3);
}

void GateKeeper::Render()
{//g_pD3DDevice->SetTransform(D3DTS_WORLD, &m_matWorld);
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

	cGameObject::Render();

	if (SightSpere && m_pSphereR)
		m_pSphereR->Render();
	if (SightSpere && m_pSphereL)
		m_pSphereL->Render();

	MonsterBase::Render();
}

bool GateKeeper::isUseLocalAnim()
{
	if (
		m_state == MON_STATE_atk02 ||
		m_state == MON_STATE_atk01
		)
		return true;

	//이게 진짜 중요!!!
	if (m_vCurAnimPos.x - m_vBeforeAnimPos.x > 10.0f)
	{
		m_vCurAnimPos = D3DXVECTOR3(0, 0, 0);
		m_vBeforeAnimPos = D3DXVECTOR3(0, 0, 0);
		return true;
	}

	return false;
}

void GateKeeper::SetAnimWorld()
{// 이동값이 있는 애니메이션의 스타트 월드 매트릭스를 세팅
	D3DXMATRIX matR, matT;
	D3DXMatrixRotationY(&matR, m_fRotY);
	D3DXVec3TransformNormal(&m_vDirection, &D3DXVECTOR3(1, 0, 0), &matR);
	D3DXMatrixTranslation(&matT, m_vPosition.x, m_vPosition.y, m_vPosition.z);
	m_matAnimWorld = matR * matT;
	m_matWorld = matR * matT;
}

void GateKeeper::ProcessDamaged()
{
}

void GateKeeper::ProcessBigDamaged()
{
}

void GateKeeper::ProcessDie()
{
}

void GateKeeper::Move()
{//캐릭터 인식상태일때
	if (m_bAwake)
	{
		//혹시모르니깐 어슬렁패턴에 쓰인 불변수는 모두 초기화 시켜주자.
		{
			m_bStart = false;
			m_bWalkOnOff = false;
		}

		//적이 캐릭터를 인식이되면 기본상태는 해제.
		m_bIdle = false;

		//공격모드
		if (m_bFight)
		{
			//앵글락 상태에서는 퍼포먼스도중에 회전하지 않는다.(때리면서 돌아가는거 방지)
			if (!m_bAngleLock)
			{

				D3DXVECTOR3 u = D3DXVECTOR3(1, 0, 0);
				D3DXVECTOR3 v;
				D3DXVec3Normalize(&v, &temp);

				m_fCosVal = D3DXVec3Dot(&v, &u);
				m_fCosVal = acosf(m_fCosVal);

				if (m_vPosition.z < g_vPlayerPos->z)
					m_fCosVal = D3DX_PI * 2 - m_fCosVal;
			}

			//공격을 시작할때(아직 애니메이션을 시작하지 않은 상태여야 한다.)
			if (!m_bAtkTerm && !m_bAnimation && D3DXVec3Length(&temp) < m_fFightZone)
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

					SOUND->Play("M1_MON_STATE_atk01");
				}
				break;
				case 1:
				{
					m_state = MON_STATE_atk02;
					m_fCurAnimTime = m_fAnimTime[MON_STATE_atk02];

					SOUND->Play("M1_MON_STATE_atk02");
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


		}
		else
		{
			/////////////예외처리. 바운더리에서는 더이상 쫓아오지않는다.///////////
			//(아직 바운더리 근처에서 전투할 경우에 몬스터가 넘어가 멈춰버리는 현상에 대해서는 처리 안함)
			if (m_bEscapeToggle)
			{
				m_state = MON_STATE_Wait;
				// u벡터 -> 기준벡터
				D3DXVECTOR3 u = D3DXVECTOR3(1, 0, 0);
				D3DXVECTOR3 v;
				D3DXVec3Normalize(&v, &temp);

				m_fCosVal = D3DXVec3Dot(&v, &u);
				m_fCosVal = acosf(m_fCosVal);

				if (m_vPosition.z < g_vPlayerPos->z)
					m_fCosVal = D3DX_PI * 2 - m_fCosVal;

				if (D3DXVec3Length(&temp) > m_fAreaRadius)
				{
					m_bEscapeToggle = false;
					m_bAwake = false;
				}

				//SOUNDMANAGER->Play("M1_MON_STATE_Wait");
			}
			///////////////////////////////////////////////////////////////////////
			else
			{
				m_state = MON_STATE_Walk;
				// u벡터 -> 기준벡터
				D3DXVECTOR3 u = D3DXVECTOR3(1, 0, 0);
				D3DXVECTOR3 v;
				D3DXVec3Normalize(&v, &temp);

				m_fCosVal = D3DXVec3Dot(&v, &u);
				m_fCosVal = acosf(m_fCosVal);

				if (m_vPosition.z < g_vPlayerPos->z)
					m_fCosVal = D3DX_PI * 2 - m_fCosVal;

				m_vPosition += (m_fRunSpeed * v);

				//SOUNDMANAGER->Play("M1_MON_STATE_Walk");
			}
		}
	}
	else
	{
		if (m_bIdle)
			Roaming();

		else
		{
			//혹시모르니깐 어슬렁패턴에 쓰인 불변수는 모두 초기화 시켜주자.
			{
				m_bStart = false;
				m_bWalkOnOff = false;
			}
			//왜그런지 모르겠는데 이 값이 작아질수록 Idle로 바뀌는 시간이 길어진다.
			if (D3DXVec2Length(&tt) < 50.0f)
				m_bIdle = true;
			else
			{
				m_state = MON_STATE_run;
				// u벡터 -> 기준벡터
				D3DXVECTOR3 u = D3DXVECTOR3(1, 0, 0);
				D3DXVECTOR3 v;
				D3DXVECTOR3 t;
				t = m_vBehaviorSpot - m_vPosition;
				D3DXVec3Normalize(&v, &t);

				m_fCosVal = D3DXVec3Dot(&v, &u);
				m_fCosVal = acosf(m_fCosVal);

				if (m_vPosition.z < m_vBehaviorSpot.z)
					m_fCosVal = D3DX_PI * 2 - m_fCosVal;

				m_vPosition += (1.5f * m_fRunSpeed * v);

				//SOUNDMANAGER->Play("M1_MON_STATE_run");
			}
		}
	}

	m_fRotY = m_fCosVal;
}

void GateKeeper::Die()
{
	if (m_state != MON_STATE_Death)
		ItemDrop("아이스소드");

	m_fHpCur = 0.0f;

	m_state = MON_STATE_Death;
	m_fCurAnimTime = m_fAnimTime[MON_STATE_Death];
	m_bIsBlend = false;
	m_bAnimation = true;

	SOUND->Play("M1_MON_STATE_Death");
}

void GateKeeper::MonoBehavior(void)
{
	if (D3DXVec3Length(&temp) <= m_fAreaRadius)
	{
		m_bAwake = true;

		if (D3DXVec3Length(&temp) < m_fFightZone)
		{
			m_bEscapeToggle2 = false;
			m_bFight = true;
		}
		else if (D3DXVec3Length(&temp) >= m_fFightZone && !m_bEscapeToggle2)
		{
			m_bEscapeToggle2 = true;
			m_fEscapeTime2 = (float)GetTickCount();
		}

		if (m_bEscapeToggle2)
		{
			if (GetTickCount() - m_fEscapeTime2 > 2000.0f && !m_bAnimation)
			{
				m_bFight = false;
				m_bAtkTerm = true;			//전투상태초기화.
				m_bEscapeToggle2 = false;
			}
		}
	}

	//갓 탈출했으면
	if (D3DXVec2Length(&tt) > m_fTracableArea && m_bAwake)
	{
		m_bEscapeToggle = true;
	}
}

void GateKeeper::Roaming(void)
{	//기다리는과정(첫세팅)
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

		//SOUNDMANAGER->Play("M1_MON_STATE_Wait");
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
		m_fCosVal = acosf(m_fCosVal);

		if (m_vPosition.z < NextSpot.z)
			m_fCosVal = D3DX_PI * 2 - m_fCosVal;

		m_vPosition += (0.3f*m_fRunSpeed * v);

		//이거 오차범위 넉넉하게 줘야 한다. 안그러면 정확하게 맞추려고 부르르르르 떤다. 
		if (D3DXVec3Length(&v2) < 1.0f)
		{
			m_bWalkOnOff = false;
			m_bStart = false;
		}

		//SOUNDMANAGER->Play("M1_MON_STATE_Walk");
	}

}

bool GateKeeper::Attack(float damage)
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

void GateKeeper::Damaged(float damage, D3DXVECTOR3 pos)
{
	if (m_state == MON_STATE_flinch ||
		m_state == MON_STATE_deathwait ||
		m_state == MON_STATE_Death)	 return;
	else if (m_state != MON_STATE_flinch &&
		m_state != MON_STATE_deathwait &&
		m_state != MON_STATE_Death)
		SOUND->Play("WPN_Sword_Attack");


	m_fHpCur -= damage;

	if (damage)
	{
		m_bAnimation = true;
		m_state = MON_STATE_flinch;
		m_fCurAnimTime = m_fAnimTime[MON_STATE_flinch];
		m_fTime = 0.0f;
		//m_bIsBlend = true;

		// 블리딩 터트릴 좌표를 만들어서 세팅하고 시작
		D3DXMATRIX matTS, matR, matT;
		D3DXMatrixRotationY(&matR, m_fRotY);
		float x = 30 + m_vPosition.x;
		float y = rand() % 80 + 10 + m_vPosition.y;
		float z = rand() % 60 - 30 + m_vPosition.z;
		D3DXMatrixTranslation(&matT, x, y, z);
		m_pParticleBleeding->SetWorld(matR * matT);
		m_pParticleBleeding->Start();

		SOUND->Play("M1_MON_STATE_Damage");

	}
}
