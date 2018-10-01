#include "stdafx.h"
#include "ProgressBar.h"


ProgressBar::ProgressBar()
{
}


ProgressBar::~ProgressBar()
{
	SAFE_DELETE(_progressBarFront);
	SAFE_DELETE(_progressBarBack);
}

void ProgressBar::Setup(char * frontImage, char * backImage, float x, float y, int width, int height)
{
	// 체력 바 위치 초기화
	_x = x;
	_y = y;
	// 체력바 렉트 위치 및 크기 초기화
	_rcProgress = RectMake(x, y, width, height);

	// 체력바 이미지 초기화
	_progressBarFront = TEXTURE->GetSprite(frontImage);
	//_progressBarFront->Init(frontImage, x, y, width, height, true, RGB(255, 0, 255));

	_progressBarBack = TEXTURE->GetSprite(backImage);
	//_progressBarBack->Init(backImage, x, y, width, height, true, RGB(255, 0, 255));

	// 가로 길이 초기화
	_width = _progressBarFront->textureInfo.Width;
}

void ProgressBar::Update()
{
	_rcProgress = RectMakeCenter(
		_x + _progressBarBack->textureInfo.Width / 2,
		_y + _progressBarBack->textureInfo.Height / 2,
		_progressBarBack->textureInfo.Width,
		_progressBarBack->textureInfo.Height);
}

void ProgressBar::Render()
{
	RECT rc;
	SetRect(&rc, 0, 0, _progressBarBack->textureInfo.Width, _progressBarBack->textureInfo.Height);
	_progressBarBack->Render(rc, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(_x, _y, 0));

	SetRect(&rc, 0, 0, _width, _progressBarBack->textureInfo.Height);
	_progressBarFront->Render(rc, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(_x, _y, 0));
}

void ProgressBar::SetGauge(float currentGauge, float maxGauge)
{
	// 게이지 세팅은 숙제
	_width = (currentGauge / maxGauge) * _progressBarBack->textureInfo.Width;
}
