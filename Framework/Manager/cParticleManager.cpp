#include "stdafx.h"
#include "cParticleManager.h"


#pragma region 파티클
cParticle::cParticle()
	: m_curTime(0.0f)
{
	m_isUse = true;
}


cParticle::~cParticle()
{
}

void cParticle::Setup(float _fTime, D3DXVECTOR3 _vPos, D3DXVECTOR3 _vDir,
	float _speed, float _acc, float _radian, D3DXCOLOR _color, const char * szFile)
{
	m_isUse = true;
	m_fTime = _fTime;
	m_stPaticle.p = _vPos;
	m_vPosition = _vPos;
	m_vDirecton = _vDir;
	m_fSpeed = _speed;
	m_fFirstSpeed = _speed;
	m_fAcc = _acc;
	m_fRdian = _radian;
	m_stPaticle.c = _color;

	m_pTexture = TEXTURE->GetTexture(szFile);

}

void cParticle::Setup()
{
	m_isUse = true;
	m_stPaticle.p = m_vPosition;
	m_curTime = 0.0f;
	m_fSpeed = m_fFirstSpeed;
}

void cParticle::Update(D3DXMATRIX world)
{

	if (m_isUse)
	{
		m_curTime += TIME->GetEllapsedTime();
		// 정해진 방향에 회전각을 입히고 // 일단 보류
		D3DXMATRIX matS, matT;
		//ST_PCT_VERTEX  vertex = m_stPaticle;
		// 시간에 따른 스케일 변경
		scale = m_curTime / m_fTime;

		if (scale > 1.0f)
		{
			m_isUse = false;
			return;
		}

		D3DXMatrixScaling(&matS, scale, scale, scale);

		// 이동 방향, 스피드가 적용된 벡터로 T 행렬 만듦
		D3DXVECTOR3 v = m_vDirecton * m_fSpeed;
		D3DXMatrixIdentity(&matT);
		D3DXMatrixTranslation(&matT, v.x, v.y, v.z);

		// 변환 행렬 생성
		m_matTS = matT;
		D3DXVec3TransformCoord(&m_stPaticle.p, &m_stPaticle.p, &m_matTS);

		// 스피드에 가속도를 적용
		m_fSpeed += m_fAcc;
	}
}

void cParticle::Render()
{
	if (m_isUse)
	{
		//DEVICE->SetTransform(D3DTS_WORLD, &m_matTS);
		DEVICE->SetRenderState(D3DRS_POINTSIZE, FtoDW((1.0f - scale) * 100));
		DEVICE->DrawPrimitiveUP(D3DPT_POINTLIST,
			1, &m_stPaticle, sizeof(ST_PC_VERTEX));
	}

}
#pragma endregion

#pragma region 파티클 셋 (집단)

cParticleSet::cParticleSet()
	: m_fCurTime(0.0f)
{
	D3DXMatrixIdentity(&m_matWorld);
	m_isStart = false;
	useIt = false;
}


cParticleSet::~cParticleSet()
{
	for (int i = 0; i < m_vecParticle.size(); i++)
		SAFE_DELETE(m_vecParticle[i]);
}

void cParticleSet::Setup(PARTICLE_TYPE type, float time, float speed,
	int acc, int accMin, int accMax,
	int posX, int randPosXMin, int randPosXMax,
	int posY, int randPosYMin, int randPosYMax,
	int posZ, int randPosZMin, int randPosZMax,
	int dirX, int randDirXMin, int randDirXMax,
	int dirY, int randDirYMin, int randDirYMax,
	int dirZ, int randDirZMin, int randDirZMax,
	const char * szFile, D3DXCOLOR color)
{
	m_type = type;
	m_fTime = time;
	m_fSpeed = speed;
	m_fAcc = acc;
	m_fAccMin = accMin;
	m_fAccMax = accMax;
	m_fPositionX = posX;
	m_fRandPosXMin = randPosXMin;
	m_fRandPosXMax = randPosXMax;
	m_fPositionY = posY;
	m_fRandPosYMin = randPosYMin;
	m_fRandPosYMax = randPosYMax;
	m_fPositionZ = posZ;
	m_fRandPosZMin = randPosZMin;
	m_fRandPosZMax = randPosZMax;
	m_fDirectionX = dirX;
	m_fRandDirXMin = randDirXMin;
	m_fRandDirXMax = randDirXMax;
	m_fDirectionY = dirY;
	m_fRandDirYMin = randDirYMin;
	m_fRandDirYMax = randDirYMax;
	m_fDirectionZ = dirZ;
	m_fRandDirZMin = randDirZMin;
	m_fRandDirZMax = randDirZMax;

	//strcpy(m_szFile, szFile);
	m_stColor = color;

	m_pTexture = TEXTURE->GetTexture(szFile);

	m_vecParticle.resize(300);
	for (int i = 0; i < m_vecParticle.size(); i++)
	{
		// 위치 방향 렌덤하게, 속도 가속도 타임 전달받은데로
		// 퉥스쳐 경로 받은데로, 칼라 또한 입력받은 걸루
		D3DXVECTOR3		pos(0, 0, 0);
		D3DXVECTOR3		dir(0, 0, 0);

		if (m_fAccMax - m_fAccMin > 0)
			m_fAcc = rand() % ((m_fAccMax - m_fAccMin + 1) + m_fAccMin) * 0.001f;

		if (m_fRandPosXMax - m_fRandPosXMin > 0)
			pos.x = rand() % (m_fRandPosXMax - m_fRandPosXMin + 1) + m_fRandPosXMin;
		else
			pos.x = m_fPositionX;

		if (m_fRandPosYMax - m_fRandPosYMin > 0)
			pos.y = rand() % (m_fRandPosYMax - m_fRandPosYMin + 1) + m_fRandPosYMin;
		else
			pos.y = m_fPositionY;

		if (m_fRandPosZMax - m_fRandPosZMin > 0)
			pos.z = rand() % (m_fRandPosZMax - m_fRandPosZMin + 1) + m_fRandPosZMin;
		else
			pos.z = m_fPositionZ;

		if (m_fRandDirXMax - m_fRandDirXMin > 0)
			dir.x = (rand() % (m_fRandDirXMax - m_fRandDirXMin + 1) + m_fRandDirXMin) * 0.01f;
		else
			dir.x = m_fDirectionX * 0.01f;

		if (m_fRandDirYMax - m_fRandDirYMin > 0)
			dir.y = (rand() % (m_fRandDirYMax - m_fRandDirYMin + 1) + m_fRandDirYMin) * 0.01f;
		else
			dir.y = m_fDirectionY * 0.01f;

		if (m_fRandDirZMax - m_fRandDirZMin > 0)
			dir.z = (rand() % (m_fRandDirZMax - m_fRandDirZMin + 1) + m_fRandDirZMin) * 0.01f;
		else
			dir.z = m_fDirectionZ * 0.01f;

		D3DXVec3Normalize(&dir, &dir);

		cParticle * newParticle = new cParticle;
		newParticle->Setup(m_fTime, pos, dir, m_fSpeed, m_fAcc, 0.0f, m_stColor, m_szFile);
		if (m_type == PTC_TYPE_LOOP)
			newParticle->SetIsUse(false);

		m_vecParticle[i] = newParticle;
	}

	DEVICE->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	DEVICE->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	//DEVICE->SetRenderState(D3DRS_POINTSIZE, FtoDW(10.0f));
	DEVICE->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(0.0f));
	DEVICE->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(0.0f));
	DEVICE->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(1.0f));
	DEVICE->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDW(100.0f));
	DEVICE->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(0.0f));

	// 텍스처 알파 옵션 셋팅
	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// 알파블랜딩 방식 결정
	DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
}

void cParticleSet::Update()
{

	m_fCurTime += TIME->GetEllapsedTime();

	if (m_fCurTime >= m_fTime)
		m_isStart = false;

	if (useIt)
	{
		if (m_type == PTC_TYPE_LOOP)
		{
			for (int i = 0; i < m_vecParticle.size(); i++)
			{
				if (m_vecParticle[i]->GetIsUse() == false)
				{
					m_vecParticle[i]->Setup();
					break;
				}
			}
		}
	}



	for (int i = 0; i < m_vecParticle.size(); i++)
		m_vecParticle[i]->Update(m_matWorld);

}

void cParticleSet::Render()
{

	DEVICE->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	DEVICE->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	//DEVICE->SetRenderState(D3DRS_POINTSIZE, FtoDW(10.0f));
	DEVICE->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(0.0f));
	DEVICE->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(0.0f));
	DEVICE->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(1.0f));
	DEVICE->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDW(100.0f));
	DEVICE->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(0.0f));

	// 텍스처 알파 옵션 셋팅
	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// 알파블랜딩 방식 결정
	DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//


	DEVICE->SetTransform(D3DTS_WORLD, &m_matWorld);

	//
	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//
	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	//DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, false);
	DEVICE->SetTexture(0, m_pTexture);
	DEVICE->SetFVF(ST_PC_VERTEX::FVF);

	for (int i = 0; i < m_vecParticle.size(); i++)
		m_vecParticle[i]->Render();

	DEVICE->SetRenderState(D3DRS_LIGHTING, true);
	//DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);

	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);


	//
}

void cParticleSet::RenderOnce()
{
	if (useIt)
	{
		DEVICE->SetTransform(D3DTS_WORLD, &m_matWorld);
		DEVICE->SetTexture(0, m_pTexture);

		for (int i = 0; i < m_vecParticle.size(); i++)
			m_vecParticle[i]->Render();
	}

}

void cParticleSet::Start()
{
	if (m_isStart)
		return;

	useIt = true;

	for (int i = 0; i < m_vecParticle.size(); i++)
	{
		if (m_vecParticle[i]->GetIsUse() == false)
		{
			m_vecParticle[i]->Setup();
		}
	}

}

void cParticleSet::End()
{
	useIt = false;

	for (int i = 0; i < m_vecParticle.size(); i++)
	{
		if (m_vecParticle[i]->GetIsUse() == true)
		{
			m_vecParticle[i]->GetIsUse() == false;
		}
	}
}

#pragma endregion

#pragma region 파티클 매니져

cParticleManager::cParticleManager()
{
}


cParticleManager::~cParticleManager()
{
}

void cParticleManager::AddParticle(string key, const char * szFile)
{

	PARTICLE_TYPE _type; float _fTime;
	float _fSpeed;

	float _fAcc;		int	_nAccMin;		int	_nAccMax;

	int	_nPositionX;	int	_nRandPosXMin;	int	_nRandPosXMax;
	int	_nPositionY;	int	_nRandPosYMin;	int	_nRandPosYMax;
	int	_nPositionZ;	int	_nRandPosZMin;	int	_nRandPosZMax;
	int	_nDirectionX;	int	_nRandDirXMin;	int	_nRandDirXMax;

	int	_nDirectionY;	int	_nRandDirYMin;	int	_nRandDirYMax;

	int	_nDirectionZ;	int	_nRandDirZMin;	int	_nRandDirZMax;

	char _szFile[1024];

	int	_nAlpha;	int	_nColorR;	int	_nColorG;	int	_nColorB;


	cParticleSet * newParticleSet = new cParticleSet;

	FILE* fp;
	fopen_s(&fp, szFile, "r");

	char s[1024];

	fgets(s, 1024, fp);		_fTime = atof(s);
	fgets(s, 1024, fp);		_type = (PARTICLE_TYPE)atoi(s);
	fgets(s, 1024, fp);		_fSpeed = atof(s);

	fgets(s, 1024, fp);		_fAcc = atof(s);
	fgets(s, 1024, fp);		_nAccMin = atoi(s);
	fgets(s, 1024, fp);		_nAccMax = atoi(s);

	fgets(s, 1024, fp);		_nPositionX = atoi(s);
	fgets(s, 1024, fp);		_nRandPosXMin = atoi(s);
	fgets(s, 1024, fp);		_nRandPosXMax = atoi(s);

	fgets(s, 1024, fp);		_nPositionY = atoi(s);
	fgets(s, 1024, fp);		_nRandPosYMin = atoi(s);
	fgets(s, 1024, fp);		_nRandPosYMax = atoi(s);

	fgets(s, 1024, fp);		_nPositionZ = atoi(s);
	fgets(s, 1024, fp);		_nRandPosZMin = atoi(s);
	fgets(s, 1024, fp);		_nRandPosZMax = atoi(s);

	fgets(s, 1024, fp);		_nDirectionX = atoi(s);
	fgets(s, 1024, fp);		_nRandDirXMin = atoi(s);
	fgets(s, 1024, fp);		_nRandDirXMax = atoi(s);

	fgets(s, 1024, fp);		_nDirectionY = atoi(s);
	fgets(s, 1024, fp);		_nRandDirYMin = atoi(s);
	fgets(s, 1024, fp);		_nRandDirYMax = atoi(s);

	fgets(s, 1024, fp);		_nDirectionZ = atoi(s);
	fgets(s, 1024, fp);		_nRandDirZMin = atoi(s);
	fgets(s, 1024, fp);		_nRandDirZMax = atoi(s);

	fgets(s, 1024, fp);		_nAlpha = atoi(s);
	fgets(s, 1024, fp);		_nColorR = atoi(s);
	fgets(s, 1024, fp);		_nColorG = atoi(s);
	fgets(s, 1024, fp);		_nColorB = atoi(s);

	fgets(s, 1024, fp);		strcpy(_szFile, s);

	fclose(fp);

	newParticleSet->Setup(
		_type,
		_fTime, _fSpeed,
		_fAcc, _nAccMin, _nAccMax,
		_nPositionX, _nRandPosXMin, _nRandPosXMax,
		_nPositionY, _nRandPosYMin, _nRandPosYMax,
		_nPositionZ, _nRandPosZMin, _nRandPosZMax,
		_nDirectionX, _nRandDirXMin, _nRandDirXMax,
		_nDirectionY, _nRandDirYMin, _nRandDirYMax,
		_nDirectionZ, _nRandDirZMin, _nRandDirZMax,
		_szFile, D3DCOLOR_ARGB(_nAlpha, _nColorR, _nColorG, _nColorB));

	m_mapParticleSet[key] = newParticleSet;
}

cParticleSet * cParticleManager::GetParticle(string key)
{
	if (m_mapParticleSet.find(key) == m_mapParticleSet.end())
		return NULL;

	return m_mapParticleSet[key];
}

void cParticleManager::AddChild(cParticleSet * child)
{
	m_vecParticle.push_back(child);
}

void cParticleManager::Destroy()
{
	for (auto p : m_mapParticleSet)
	{
		if (p.second)
			SAFE_DELETE(p.second);
	}
	m_mapParticleSet.clear();

	//for (auto p : m_vecParticle)
	//{
	//	SAFE_DELETE(p);
	//}
}

void cParticleManager::Update()
{
	for (int i = 0; i < m_vecParticle.size(); i++)
		m_vecParticle[i]->Update();
}

void cParticleManager::Render()
{
	DEVICE->SetRenderState(D3DRS_POINTSPRITEENABLE, true);
	DEVICE->SetRenderState(D3DRS_POINTSCALEENABLE, true);
	//DEVICE->SetRenderState(D3DRS_POINTSIZE, FtoDW(10.0f));
	DEVICE->SetRenderState(D3DRS_POINTSCALE_A, FtoDW(0.0f));
	DEVICE->SetRenderState(D3DRS_POINTSCALE_B, FtoDW(0.0f));
	DEVICE->SetRenderState(D3DRS_POINTSCALE_C, FtoDW(1.0f));
	DEVICE->SetRenderState(D3DRS_POINTSIZE_MAX, FtoDW(100.0f));
	DEVICE->SetRenderState(D3DRS_POINTSIZE_MIN, FtoDW(0.0f));

	// 텍스처 알파 옵션 셋팅
	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	DEVICE->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	// 알파블랜딩 방식 결정
	DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	//


	//
	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//
	DEVICE->SetRenderState(D3DRS_LIGHTING, false);
	//DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, false);
	DEVICE->SetFVF(ST_PC_VERTEX::FVF);

	for (int i = 0; i < m_vecParticle.size(); i++)
		m_vecParticle[i]->RenderOnce();

	DEVICE->SetRenderState(D3DRS_LIGHTING, true);
	//DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, false);
	DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, true);

	DEVICE->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

#pragma endregion
