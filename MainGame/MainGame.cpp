#include "stdafx.h"
#include "MainGame.h"

#include "Scene\Scenes.h"

MainGame::MainGame()
	: surfaceCursor(NULL)
	, m_pCursorArrow(NULL)
	, m_pCursorEmpty(NULL)
{
}


MainGame::~MainGame()
{
	//SAFE_DELETE(m_pCamera);
	//SAFE_DELETE(m_pGrid);
	//
	surfaceCursor->Release();
	m_pCursorArrow->Release();
	m_pCursorEmpty->Release();

	SCENE->Destroy();
	TEXTURE->Destroy();
	KEY->Delete();
	TIME->Delete();
	SOUND->Release();
	FONT->Destroy();
	SKINNEDMESH->Destroy();
	STATICMESH->Destroy();
	PARTICLE->Destroy();
	CAMERA->Destroy();
	ITEMMANAGER->Destroy();
	UI->Destroy();

	DEVICEMANAGER->Destroy();
}

void MainGame::Setup()
{
	DEVICE->SetRenderState(D3DRS_LIGHTING, true);
	DEVICE->SetRenderState(D3DRS_AMBIENT, 0x00202020);
	SetLight();

	CAMERA->Create();

	SCENE->AddScene("PaticleEdit", new ParticleEdit);

	SCENE->AddScene("Main", new scMain);
	SCENE->AddScene("LobbyLoading", new scFieldLoading);
	SCENE->AddScene("Test", new scField);
	SCENE->AddScene("BossLoading", new scBossRoomLoading);
	SCENE->AddScene("Boss", new scBossRoom);

	SCENE->ChangeScene("Main");

	SetCursor();
}

void MainGame::Update()
{
	CAMERA->Update();
	OBJECTMANAGER->PickObject();
	SCENE->Update();
}

void MainGame::Render()
{
	SCENE->Render();
	TIME->Render();
}

void MainGame::SetLight()
{
	D3DLIGHT9 stLight;
	ZeroMemory(&stLight, sizeof(D3DLIGHT9));

	stLight.Type = D3DLIGHT_DIRECTIONAL;

	stLight.Ambient = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f); // ȯ�汤
	stLight.Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	stLight.Specular = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//stLight.Range = 10000.0f;
	D3DXVECTOR3 vDir(0.0f, -1.0f, 0.0f);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;

	DEVICE->SetLight(100, &stLight);
	DEVICE->LightEnable(100, true);

	vDir = D3DXVECTOR3(-1.0f, 0.0f, 0.0f);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;

	DEVICE->SetLight(101, &stLight);
	DEVICE->LightEnable(101, true);

	vDir = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;

	DEVICE->SetLight(102, &stLight);
	DEVICE->LightEnable(102, true);

	vDir = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;

	DEVICE->SetLight(103, &stLight);
	DEVICE->LightEnable(103, true);

	vDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXVec3Normalize(&vDir, &vDir);
	stLight.Direction = vDir;

	DEVICE->SetLight(104, &stLight);
	DEVICE->LightEnable(104, true);
}

void MainGame::SetCursor()
{
	D3DXCreateTextureFromFile(DEVICE, L"Resource/Texture/Cursor/Arrow.png", &m_pCursorArrow);
	D3DXCreateTextureFromFile(DEVICE, L"Resource/Texture/Cursor/Empty.png", &m_pCursorEmpty);

	m_pCursorArrow->GetSurfaceLevel(0, &surfaceCursor);
	DEVICE->SetCursorProperties(0, 0, surfaceCursor);
}
