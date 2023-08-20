#include "effectGainVolume.h"

extern "C"
{
	EffectGainVolume *allocator()
	{
		return new EffectGainVolume();
	}

	void deleter(EffectGainVolume *ptr)
	{
		delete ptr;
	}
}
