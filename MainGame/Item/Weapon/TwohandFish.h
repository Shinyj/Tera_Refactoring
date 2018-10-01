#pragma once
#include "MainGame\Item\cItemBase.h"

class TwohandFish : public cItemBase
{
public:
	TwohandFish();
	virtual~TwohandFish();

	void Setup();
	void Update();
	void Rendeer();
};

