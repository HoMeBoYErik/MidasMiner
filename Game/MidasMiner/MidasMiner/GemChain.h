#ifndef __GemChain__
#define __GemChain__

#include <iostream>
#include <vector>
#include <memory>
#include "GameObject.h"
#include <memory>


enum GemChainType
{
	ChainTypeHorizontal = 1,
	ChainTypeVertical = 2
};

class GemChain
{
public:
	std::vector<std::shared_ptr<GameObject>> gems;
	GemChainType chainType;
	void addGem(std::shared_ptr<GameObject> gem);

	GemChain(){};

	~GemChain()
	{
		gems.clear();
	};
};


#endif