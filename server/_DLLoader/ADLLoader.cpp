#include "ADLLoader.h"

template <typename PLUGIN>
ADLLoader<PLUGIN>::ADLLoader()
	: fptr(0), singleton(0)
{
}

template <typename PLUGIN>
PLUGIN *ADLLoader<PLUGIN>::getInstance()
{
	if (!this->singleton)
		this->singleton = this->fptr();
	return this->singleton;
}