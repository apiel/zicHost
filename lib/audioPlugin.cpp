#ifndef PLUGIN_NAME
#define PLUGIN_NAME EffectGainVolume
#define PLUGIN_INC "EffectGainVolume.h"
#endif

#include PLUGIN_INC

extern "C"
{
	PLUGIN_NAME *allocator()
	{
		return new PLUGIN_NAME();
	}

	void deleter(PLUGIN_NAME *ptr)
	{
		delete ptr;
	}
}
