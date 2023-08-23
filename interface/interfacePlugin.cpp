#ifndef PLUGIN_NAME
#define PLUGIN_NAME OscInterface
#endif

#ifndef PLUGIN_INCLUDE
#define PLUGIN_INCLUDE "OscInterface.h"
#endif

#include PLUGIN_INCLUDE

extern "C"
{
	PLUGIN_NAME *allocator(InterfacePluginProps& props)
	{
		return new PLUGIN_NAME(props);
	}

	void deleter(PLUGIN_NAME *ptr)
	{
		delete ptr;
	}
}
