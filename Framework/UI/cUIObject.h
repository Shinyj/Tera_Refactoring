#pragma once

#define INDEXNOTSET -1

enum tagClicked
{
	NON,
	CLICKING,
	CLICK,
	UNMOVABLE,
	NEVERCLICKED


};

class cUIObject
{
protected :

	SYNTHESIZE(int, m_nAlpha, Alpha);
	SYNTHESIZE(D3DXMATRIX, m_matWorld, MatWorld);
	SYNTHESIZE(D3DXVECTOR3, m_vPosition, Position);		// 위치
	SYNTHESIZE(cUIObject*, m_pParent, Parent);	// 부모 정보
	SYNTHESIZE(ST_UI_SIZE, m_stSize, StSize);

	SYNTHESIZE(bool, m_isItemInfoWnd, IsItemInfoWnd);

	SYNTHESIZE(bool, m_isCollisionPT, IsCollisionPT);
	SYNTHESIZE(bool, m_isColDragRcAndPT, IsColDragRcAndPT);

	SYNTHESIZE(POINT, m_move, Move);
	SYNTHESIZE(tagClicked, m_enClickState, ClickState);
	SYNTHESIZE(bool, m_isMove, IsMove);
	SYNTHESIZE(RECT, m_CollisionRect, CollisionRect);
	SYNTHESIZE(RECT, m_DragRect, DragRect);
	SYNTHESIZE(RECT, m_resizeRect, ResizeRect);
	SYNTHESIZE(D3DXVECTOR2, m_nReduceDragRange, ReduceDragRange);

	POINT m_beforeMousePT = { 0,0 };

	SYNTHESIZE(D3DXMATRIX, m_matWorld2, MatWorld2);

	SYNTHESIZE(vector<cUIObject*>, m_vecChild, VecChild);			// 자식 정보
					
public:
	cUIObject();
	virtual~cUIObject();

	

	virtual void AddChild(cUIObject * pChild);
	virtual void DelChild(int index);
	virtual void Destroy();
	virtual void Update(ST_UI_SIZE dragSize = { 1,1 });
	virtual void Render(LPD3DXSPRITE pSprite);
	virtual void ImageDrag(D3DXVECTOR2 vec2ReduceDragRange = { 1,1 });


	//
	inline void SetMatWorldChangedPosXY(D3DXVECTOR3 vec3ChangedPos) { m_matWorld._41 = vec3ChangedPos.x, m_matWorld._42 = vec3ChangedPos.y; }
};

