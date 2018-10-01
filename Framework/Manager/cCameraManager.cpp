#include "stdafx.h"
#include "cCameraManager.h"

cCamera::cCamera()
	: m_vEye(-100, 50, 0)
	, m_vLookAt(300, 0, 0)
	, m_vUp(0, 1, 0)
	, beforePt{ 0,0 }
	, currentPt{ 0,0 }
	, isDrag(false)
	, m_fWorldX(0.0f)
	, m_fWorldY(0.0f)
	, m_fWorldZ(0.0f)
{
	D3DXMatrixIdentity(&m_matRotateX);
	D3DXMatrixIdentity(&m_matRotateY);
	D3DXMatrixIdentity(&m_matRotateZ);
	D3DXMatrixIdentity(&m_matTraslation);
	D3DXMatrixIdentity(&m_matWorld);
}


cCamera::~cCamera()
{
}

void cCamera::Setup()
{
	m_position = D3DXVECTOR3(0, 0, 0);
	RECT rc;
	GetClientRect(hWnd, &rc);

	D3DXMATRIX matProj;
	D3DXMatrixPerspectiveFovLH // 왼손 좌표계의 프로젝션 매트릭스를 만드는 함수
	(
		&matProj,
		D3DX_PI / 4.0f,
		rc.right / (float)rc.bottom,
		1.0f,
		50000.0f
	);

	DEVICE->SetTransform(D3DTS_PROJECTION, &matProj); // 세팅

}

void cCamera::Update(D3DXVECTOR3 target)
{
	m_fWorldY += (currentPt.x - beforePt.x);

	beforePt = currentPt;
	currentPt = ptMouse;

	D3DXMatrixRotationY(&m_matRotateY, m_fWorldY * D3DX_PI / 180.0f);
	D3DXMatrixRotationZ(&m_matRotateX, m_fWorldX * D3DX_PI / 180.0f);

	D3DXMatrixTranslation(&m_matTraslation, (float)Wheel, 0, 0);

	D3DXVECTOR3 vEye;
	float x = m_vEye.x;

	D3DXVec3TransformCoord(&vEye, &m_vEye, &m_matRotateX);
	D3DXVec3TransformCoord(&vEye, &vEye, &m_matTraslation);
	D3DXVec3TransformCoord(&vEye, &vEye, &m_matRotateY);
	//D3DXVec3TransformCoord(&vEye, &vEye, &m_matRotateZ);
	D3DXVec3TransformCoord(&vEye, &vEye, &m_matWorld);
	x = vEye.x - x;

	m_vLocal = vEye;


	if (target)
		vEye += target;

	viewPos = vEye;
	//viewPos.y = 0;
	//D3DXVec3Normalize(&viewPos, &viewPos);

	D3DXVECTOR3 vLookAt = m_vLookAt;
	D3DXVec3TransformCoord(&vLookAt, &vLookAt, &m_matRotateY);
	D3DXVec3TransformCoord(&vLookAt, &vLookAt, &m_matWorld);

	if (target)
		vLookAt += target;

	g_vCamera = vLookAt - vEye;
	g_vCamera.y = 0;
	D3DXVec3Normalize(&g_vCamera, &g_vCamera);

	D3DXVECTOR3 vUp = m_vUp;

	//D3DXVec3TransformCoord(&vUp, &vUp, &m_matRotateZ);

	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView, &vEye, &vLookAt, &vUp);

	DEVICE->SetTransform(D3DTS_VIEW, &matView);
}

void cCamera::UpdateFix(D3DXVECTOR3 target)
{
	if (KEY->IsStayKeyDown(VK_CONTROL))
	{
		if (KEY->IsStayKeyDown(VK_RBUTTON))
		{
			if (isDrag == false)
			{
				isDrag = true;
				beforePt = ptMouse;
			}
			else
			{
				m_fWorldY += (ptMouse.x - beforePt.x);
				D3DXMatrixRotationY(&m_matRotateY, m_fWorldY * D3DX_PI / 180.0f);
				beforePt = ptMouse;
			}
		}
	}

	if (KEY->IsOnceKeyUp(VK_RBUTTON))
	{
		isDrag = false;
		beforePt = { 0,0 };
	}

	if (KEY->IsStayKeyDown(VK_NUMPAD8))
		m_fWorldX++;
	else if (KEY->IsStayKeyDown(VK_NUMPAD2))
		m_fWorldX--;

	if (KEY->IsStayKeyDown(VK_NUMPAD1))
		m_fWorldY++;
	else if (KEY->IsStayKeyDown(VK_NUMPAD3))
		m_fWorldY--;

	if (KEY->IsStayKeyDown(VK_NUMPAD7))
		Wheel += 10;
	else if (KEY->IsStayKeyDown(VK_NUMPAD9))
		Wheel -= 10;

	if (KEY->IsStayKeyDown(VK_NUMPAD4))
		m_fWorldZ += 10;
	else if (KEY->IsStayKeyDown(VK_NUMPAD6))
		m_fWorldZ -= 10;

	D3DXMatrixRotationX(&m_matRotateX, m_fWorldX * D3DX_PI / 180.0f);

	// 여기 주석 풀어야함
	D3DXMatrixRotationY(&m_matRotateY, m_fWorldY * D3DX_PI / 180.0f);

	//D3DXMatrixRotationZ(&m_matRotateZ, m_fWorldZ * D3DX_PI / 180.0f);

	D3DXMatrixTranslation(&m_matTraslation, (float)Wheel, 0, 0);

	D3DXVECTOR3 vEye;
	float x = m_vEye.x;

	D3DXVec3TransformCoord(&vEye, &m_vEye, &m_matTraslation);
	//D3DXVec3TransformCoord(&vEye, &vEye, &m_matRotateX);
	D3DXVec3TransformCoord(&vEye, &vEye, &m_matRotateY);
	//D3DXVec3TransformCoord(&vEye, &vEye, &m_matRotateZ);
	D3DXVec3TransformCoord(&vEye, &vEye, &m_matWorld);
	x = vEye.x - x;

	m_vLocal = vEye;

	if (target)
		vEye += target;

	viewPos = vEye;

	D3DXVECTOR3 vLookAt = m_vLookAt;
	//vLookAt.x += x;

	//D3DXVec3TransformCoord(&vLookAt, &m_vLookAt, &m_matTraslation);
	//D3DXVec3TransformCoord(&vLookAt, &vLookAt, &m_matRotateX);
	D3DXVec3TransformCoord(&vLookAt, &vLookAt, &m_matRotateY);
	D3DXVec3TransformCoord(&vLookAt, &vLookAt, &m_matWorld);
	if (target)
		vLookAt += target;


	g_vCamera = vLookAt - vEye;
	g_vCamera.y = 0;
	D3DXVec3Normalize(&g_vCamera, &g_vCamera);

	D3DXVECTOR3 vUp = m_vUp;

	//D3DXVec3TransformCoord(&vUp, &vUp, &m_matRotateZ);

	D3DXMATRIX matView;
	D3DXMatrixLookAtLH(&matView, &vEye, &vLookAt, &vUp);


	DEVICE->SetTransform(D3DTS_VIEW, &matView);
}

D3DXVECTOR3 cCamera::GetCameraWorld()
{
	return viewPos;
	//return m_vEye;
}

D3DXVECTOR3 cCamera::GetCameraLocal()
{
	return m_vLocal;
}
cCameraManager::cCameraManager()
	: m_pCamera(NULL)
	, m_fShakingTime(0.0f)
{
}


cCameraManager::~cCameraManager()
{
}

void cCameraManager::Create()
{
	m_pCamera = new cCamera;
	m_pCamera->Setup();
	m_type = CAMERA_FREE;
}

void cCameraManager::Update()
{
	D3DXMATRIX	mat;
	D3DXMatrixIdentity(&mat);
	if (m_fShakingTime > 0.0f)
	{
		m_fShakingTime -= TIME->GetEllapsedTime();

		mat._41 = rand() % 10 - 5;
		mat._42 = rand() % 10 - 5;
		mat._43 = rand() % 10 - 5;
	}
	m_pCamera->SetWorld(mat);

	switch (m_type)
	{
	case CAMERA_FREE:
		if (g_vPlayerPos)
			m_pCamera->Update(*g_vPlayerPos);
		else
			m_pCamera->Update(D3DXVECTOR3(0, 0, 0));
		break;
	case CAMERA_FIX:
		if (g_vPlayerPos)
			m_pCamera->UpdateFix(*g_vPlayerPos);
		else
			m_pCamera->UpdateFix(D3DXVECTOR3(0, 0, 0));
		break;
	}
}

void cCameraManager::Shaking(float time)
{
	m_fShakingTime = time;
}

void cCameraManager::Destroy()
{
	SAFE_DELETE(m_pCamera);
}

void cCameraManager::SetType(CAMERA_TYPE type)
{
	m_type = type;
	if (m_type == CAMERA_FREE)
	{
		m_pCamera->SetBeforePT(POINT{ 0,0 });
		m_pCamera->SetCurrentPT(POINT{ 0,0 });
	}
}

D3DXVECTOR3 cCameraManager::GetCameraWorld()
{
	return m_pCamera->GetCameraWorld();
}

D3DXVECTOR3 cCameraManager::GetCameraLocal()
{
	return m_pCamera->GetCameraLocal();
}