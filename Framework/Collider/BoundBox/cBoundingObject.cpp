#include "stdafx.h"
#include "cBoundingObject.h"


cBoundingObject::cBoundingObject()
	: m_pSpere(NULL)
	, m_pBoundingBox(NULL)
	, m_bSightBox(false)
	, m_bSightSpere(false)
{
}


cBoundingObject::~cBoundingObject()
{
	SAFE_DELETE(m_pSpere);
	SAFE_DELETE(m_pBoundingBox);

	for (auto b : m_vecBBoxGroup)
	{
		SAFE_DELETE(b);
	}
	delete this;
}

void cBoundingObject::Setup()
{
	
}

void cBoundingObject::Update()
{
	if (!m_vecBBoxGroup.empty()) // SightBox && m_vecBBoxGroup.size()
	{
		for (auto g : m_vecBBoxGroup)
		{
			g->Update();
		}
	}

	if (m_bSightSpere && m_pSpere)
		m_pSpere->Update();
}

void cBoundingObject::Render()
{
	if (*SCENE->GetCurrentSceneName() == "MapEdit") // loader로 불러울경우의 렌더 예외처리
	{
		m_bSightBox = true;
		m_bSightSpere = true;
	}

	// 바운딩 박스 렌더
	if (m_bSightBox && !m_vecBBoxGroup.empty())
	{
		for (auto g : m_vecBBoxGroup)
		{
			g->Render();
		}
	}

	if (m_bSightSpere && m_pSpere)
		m_pSpere->Render();
}

void cBoundingObject::Destroy_g()
{
	if (!m_vecBBoxGroup.empty())
	{
		for (auto g : m_vecBBoxGroup)
		{
			SAFE_DELETE(g);
		}
		delete this;
	}
}
