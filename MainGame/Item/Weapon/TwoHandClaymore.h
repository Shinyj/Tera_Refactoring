#pragma once
#include "MainGame\Item\cItemBase.h"

class TwoHandClaymore : public cItemBase
{
public:
	TwoHandClaymore();
	virtual~TwoHandClaymore();
	
	void Setup();
	void Update();
	void Rendeer();
};

