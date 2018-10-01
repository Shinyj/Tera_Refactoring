#pragma once
#include "MainGame\Item\cItemBase.h"

class TwohandIce : public cItemBase
{
public:
	TwohandIce();
	virtual~TwohandIce();

	void Setup();
	void Update();
	void Rendeer();
};

