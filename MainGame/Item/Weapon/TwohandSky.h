#pragma once
#include "MainGame\Item\cItemBase.h"

class TwohandSky : public cItemBase
{
public:
	TwohandSky();
	virtual~TwohandSky();

	void Setup();
	void Update();
	void Rendeer();
};

