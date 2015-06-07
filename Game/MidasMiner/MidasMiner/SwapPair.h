#ifndef __SwapPair__
#define __SwapPair__

#include "GameObject.h"

class SwapPair
{
public:
	GameObject* a;
	GameObject* b;

	SwapPair(){};
	~SwapPair(){};

	// Override copy operator to copy user swap intent
	SwapPair& SwapPair::operator= (const SwapPair &cSource)
	{
		// do the copy
		a = cSource.a;
		b = cSource.b;

		// return the existing object
		return *this;
	}

	/*bool operator==(SwapPair* other)
	{
	if ((this->a->id == other->a->id && this->b->id == other->b->id) ||
	(this->b->id == other->a->id && this->a->id == other->b->id))
	{
	return true;
	}

	return false;
	}

	bool operator<(const SwapPair* other)
	{
	if (this == other)return false;
	return true;
	}*/

};

// Compare functor for SwapPair set
/*struct swap_compare
{

bool operator<(const SwapPair* other)const
{

}

bool operator()(const SwapPair* sp1, const SwapPair* sp2)const
{
return false;
}
};
typedef struct swap_compare SwapCompare;*/

#endif