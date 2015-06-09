#include "GemChain.h"


void GemChain::addGem(std::shared_ptr<GameObject> gem)
{
	gems.push_back(gem);
}