#pragma once
#include "Framework\UI\cUIObject.h"



class cUIImageView : public cUIObject
{
private:


	SYNTHESIZE(const char*, m_szName, Name);





protected:
	LPDIRECT3DTEXTURE9		m_pTexture;

public:
	cUIImageView();
	virtual~cUIImageView();

	
	virtual void SetTexture(const char * szFullPath);
	virtual void Update();
	virtual void Render(LPD3DXSPRITE pPprite);


};

