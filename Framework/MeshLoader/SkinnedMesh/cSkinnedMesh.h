#pragma once

class cSkinnedMesh
{
private:

	SYNTHESIZE_REF(LPD3DXFRAME, m_pRoot, Frame);

	LPD3DXANIMATIONCONTROLLER	m_pAnimController;
	D3DXMATRIX					m_matWorld;

	float m_fBlendTime;
	float m_fPassedBlendTime;
	bool m_bBlend;


public:
	cSkinnedMesh();
	cSkinnedMesh(const char* szFolder, const char* szFile);
	~cSkinnedMesh();

	void Setup(const char* szFolder, const char* szFile);
	void Setup(wstring szFullPath);

	void Update(D3DXMATRIX world);
	void Update();
	void Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent, D3DXMATRIX world);
	void Update(LPD3DXFRAME pFrame, LPD3DXFRAME pParent);

	void DontMoveUpdate(D3DXMATRIX world);

	void UpdateSkinnedMesh(LPD3DXFRAME pFrame);

	void Render(LPD3DXFRAME pFrame);
	void Render(LPD3DXFRAME pFrame, LPD3DXEFFECT shader);
	void Render(IN LPD3DXFRAME pFrame, IN LPD3DXEFFECT shader, IN int time, IN LPDIRECT3DTEXTURE9 texture,
		OUT bool &Modeonoff, OUT bool &IsGen, OUT float &TOD);

	void Render(LPD3DXFRAME pFrame, char * key, ST_BONE_MESH* equit);

	void RenderWhite(LPD3DXFRAME pFrame);

	void SetupBoneMatrixPtrs(LPD3DXFRAME pFrame);

	void SetWolrd(D3DXMATRIX world, LPD3DXFRAME pFrame);

	void SetAnimationIndex(int nIndex);
	void SetAnimationIndexBlend(int nIndex);

	bool IsFinishAnumOnce();

	float GetAnimPosition();
	void SetAnimPosition(float pos);

	void AnimAdvanceTime();
	void AnimAdvanceTimeDouble();
};

