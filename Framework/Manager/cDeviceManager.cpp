#include "stdafx.h"
#include "cDeviceManager.h"


cDeviceManager::cDeviceManager()
{
	m_pD3D = NULL;
	m_pD3DDevice = NULL;

	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION); // sdk 버전을 인자로 받음  <- D3D 객체 생성
											   // 다이렉트 인터페이스를 생성하는 함수와 인자로 sdk의 버전을 넘겨준다. ( 통상 이런식으로 생성 )


											   // 하드웨어가 지원하는지 확인하는 작업을 해줘야함
	D3DCAPS9  stCaps;// 하드웨어의 정보를 담을 수 있는 구조체
	m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &stCaps); // stCaps 에 하드 정보가 들어옴

	int nVertaxProcessing;
	if (stCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		// 디바이스 캡스 값이 D3DDEVCAPS_HWTRANSFORMANDLIGHT 와 비트연산이 되면 ( 트랜스폼, 라이트 계산이 가능한 애면
		nVertaxProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	}
	else
	{
		nVertaxProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING | D3DCREATE_MULTITHREADED;
	}

	// 우리가 만들고자 하는 디바이스의 속성을 결정하는데 사용된다.
	D3DPRESENT_PARAMETERS stD3DPP;
	ZeroMemory(&stD3DPP, sizeof(D3DPRESENT_PARAMETERS));
	// 스왑 이펙트의 가장 효율적인 세팅
	stD3DPP.SwapEffect = D3DSWAPEFFECT_DISCARD;				// 플리핑 체인이 교환되는 방식을 지정
	stD3DPP.Windowed = true;								// 창으로 띄울래 전체화면으로 띄울래
	stD3DPP.BackBufferFormat = D3DFMT_UNKNOWN;				// 벡버퍼의 크기 지정
	stD3DPP.EnableAutoDepthStencil = true;					// 스탠실? 버퍼 사용 여부
	stD3DPP.AutoDepthStencilFormat = D3DFMT_D24S8;			// 포멧 ( 그 크기를 지정)

	m_pD3D->CreateDevice
	(
		D3DADAPTER_DEFAULT,		// 어뎁터
		D3DDEVTYPE_HAL,			// 디바이스 타입
		hWnd,						// 윈도 핸들
		nVertaxProcessing,		// 플래그
		&stD3DPP,				// 디바이스의 속성
		&m_pD3DDevice			// 생성할 디바이스 오브젝트의 포인터가 리턴돼서 여기에 들어감
	);
}


cDeviceManager::~cDeviceManager()
{

}

LPDIRECT3DDEVICE9 cDeviceManager::GetDevice()
{
	return m_pD3DDevice;
}

void cDeviceManager::Destroy()
{
	ULONG ul = m_pD3DDevice->Release(); // 디바이스의 릴리즈는 레퍼런스 카운트를 반환한다.
	//assert(ul == 0 && "디바이스를 이용해 생성한 객체 중에 해제되지 않은 객체가 있습니다.");

	m_pD3D->Release();
}