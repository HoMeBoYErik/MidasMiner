#ifndef __GemChain__
#define __GemChain__

#include <iostream>
#include <vector>
#include "GameObject.h"


enum GemChainType
{
	ChainTypeHorizontal = 1,
	ChainTypeVertical = 2
};

class GemChain
{
public:
	std::vector<GameObject*> gems;
	GemChainType chainType;
	void addGem(GameObject* gem);

	GemChain(){};

	~GemChain(){};
};


#endif