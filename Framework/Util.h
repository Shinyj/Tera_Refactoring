#pragma once

//====================================================================
//	(필요한 부분은 직접 만들어서 추가 할 것!!) 
//====================================================================

//POINT
inline POINT PointMake(int x, int y)
{
	POINT pt = { x,y };
	return pt;
}

//선그리기
inline void LineMake(HDC hdc, int x1, int y1, int x2, int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
}

//RECT 만들기
inline RECT RectMake(int x, int y, int width, int height)
{
	RECT rc = { x ,y, x + width, y + height };
	return rc;
}

//Rect 만들기(센터부터)
inline RECT RectMakeCenter(int x, int y, int width, int height)
{
	RECT rc = { x - width / 2, y - height / 2, x + width / 2, y + height / 2 };
	return rc;
}

//사각형 그리기
inline void RectangleMake(HDC hdc, int x, int y, int width, int height)
{
	Rectangle(hdc, x, y, x + width, y + height);
}
//원그리기

inline void EllipseMake(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x, y, x + width, y + height);
}

//원그리기 (센터로 부터)
inline void EllipseMakeCenter(HDC hdc, int x, int y, int width, int height)
{
	Ellipse(hdc, x - width / 2, y - height / 2, x + width / 2, y + height / 2);
}

inline DWORD FtoDW(float f)
{
	return *((DWORD*)(&f));
}

inline bool IsEqualString(const char * c1, const char * c2)
{
	return strcmp(c1, c2) == 0;
}

//vector3의 y(높이)성분을 제외한 거리를 구할때 쓰세요.
inline float DistanceXZ(D3DXVECTOR3 a, D3DXVECTOR3 b)
{
	D3DXVECTOR2 temp;
	temp.x = a.x - b.x;
	temp.y = a.z - b.z;

	return D3DXVec2Length(&temp);
}

//
// 원 그리기 (3d)
// 인자로 센터 좌쵸랑 월 반지름을 받는다.
inline void DrawPickCircle(D3DXVECTOR3 PickPos, float fCircleR)
{

	const int size = 36;
	const float angle = D3DX_PI *2.0f / (size - 1);
	ST_PC_VERTEX Line[size];
	ST_PC_VERTEX LineEx[2];
	D3DXVECTOR3 iniPos = D3DXVECTOR3(0.0f, 0.0f, -1.0f);
	D3DXVECTOR3 rotatedPos;

	D3DXMATRIX mat;
	D3DXMatrixRotationY(&mat, angle * 0);
	D3DXVec3TransformCoord(&rotatedPos, &iniPos, &mat);
	D3DXVec3Normalize(&rotatedPos, &rotatedPos);
	Line[0].p = fCircleR * rotatedPos + PickPos;
	Line[0].p.y += 0.01f;
	Line[0].c = 0xff00ff00; // Green  

	for (int i = 1; i<size; ++i)
	{
		D3DXMatrixRotationY(&mat, angle*i);
		D3DXVec3TransformCoord(&rotatedPos, &iniPos, &mat);
		D3DXVec3Normalize(&rotatedPos, &rotatedPos);
		Line[i].p = fCircleR * rotatedPos + PickPos;
		Line[i].p.y += 0.01f;
		Line[i].c = 0xff00ff00;
	}

	DEVICE->SetTexture(0, NULL);
	for (int i = 1; i<size; ++i)
	{
		LineEx[0] = Line[i - 1];
		LineEx[1] = Line[i];

		DEVICE->SetFVF(ST_PC_VERTEX::FVF);
		DEVICE->DrawPrimitiveUP(D3DPT_LINELIST, 1, LineEx, sizeof(ST_PC_VERTEX));
	}

	return;
}
