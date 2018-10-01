#pragma once
struct ST_PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DCOLOR	c;

	enum {
		FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE
	};
};

struct ST_PT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR2 t;

	enum {
		FVF = D3DFVF_XYZ | D3DFVF_TEX1
	};
};

struct ST_PNT_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
	D3DXVECTOR2 t;

	enum {
		FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1
	};
};

struct ST_PCT_VERTEX
{
	D3DXVECTOR3 p;
	D3DCOLOR	c;
	D3DXVECTOR2 t;

	enum {
		FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1
	};
};

// 이해하면 주석 지워라

struct ST_BONE : public D3DXFRAME
{
	// 프레임 -> 자기행렬만 들고잇어서
	// 누적으로 계산된 행렬을 넣을 매트릭스
	D3DXMATRIX		CombinedTransformationMatrix;
};
// 기존 것을 상속 + @ ( 더 사용할 것들 정의 )

struct ST_BONE_MESH : public D3DXMESHCONTAINER
{
	// 원본 메시를 저장할 공간
	// 계산할 때마다 메시가 바뀔거라 그럼
	LPD3DXMESH		pOrigMesh;

	// 메시에 영향을 주는 모든 본들의 정보
	D3DXMATRIX**	ppBoneMatrixPtrs;

	// 원본 메시를 월드 메트릭스 말고 로컬 스페이스로
	// 보내는 메트릭스를 넣을 공간
	D3DXMATRIX*		pBoneOffsetMatrices;

	// 현재 본에 계산된 원본 매트릭스를 저장할
	// 포인터 ( 본인 로컬에 곱해줄 누적된 메트릭스?)
	D3DXMATRIX*		pCurrentBoneMatrices;

	vector<D3DMATERIAL9>		vecMtl;
	vector<LPDIRECT3DTEXTURE9>	vecTex;

	// 속성 개수
	//DWORD			NumAttributeGroups;
	//D3DXATTRIBUTERANGE*			attributeTable;
};

// UI 와 관련된 구조체

struct ST_UI_SIZE
{
	float fWidth;
	float fHeight;

	ST_UI_SIZE() : fWidth(0), fHeight(0) {}
	ST_UI_SIZE(int w, int h)
		: fWidth(w), fHeight(h) {}
};

struct ST_SPHERE
{

};

struct ST_OBB // OBB구조체
{
	D3DXVECTOR3 vCenterPos; // 상자 중앙의 좌표
	D3DXVECTOR3 vAxisDir[3]; //상자에 평행한 세 축의 단위벡터
	float  fAxisLen[3]; // 상자의 평행한 세 축의 길이 fAxisLen[n]은 vAxisDir[n]에 각각 대응한다.
};