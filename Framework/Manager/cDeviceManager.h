#pragma once

#define DEVICE cDeviceManager::GetInstance()->GetDevice()
#define DEVICEMANAGER cDeviceManager::GetInstance()

class cDeviceManager
{
	SINGLETONE(cDeviceManager);

	LPDIRECT3D9			m_pD3D;
	LPDIRECT3DDEVICE9	m_pD3DDevice;

public:
	LPDIRECT3DDEVICE9 GetDevice();
	void Destroy();
};

