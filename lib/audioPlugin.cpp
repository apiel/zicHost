#ifndef PLUGIN_NAME
#define PLUGIN_NAME EffectGainVolume
#endif

#ifndef PLUGIN_INCLUDE
#define PLUGIN_INCLUDE "EffectGainVolume.h" 
#endif

#include PLUGIN_INCLUDE

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
