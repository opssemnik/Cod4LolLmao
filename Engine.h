#ifndef ENGINE_H 
#define ENGINE_H 

#include "Headers.h" 

namespace Engine
{
	DWORD DrawXModelSkinnedCachedOffset = 0x646870;
	IDirect3DDevice9* D3DDevice = *( IDirect3DDevice9** )0xCC9A408;

	typedef VOID( *tDrawXModelSkinnedCached )( INT a1, INT a2, INT a3 ); 
	tDrawXModelSkinnedCached DrawXModelSkinnedCached = NULL;
};

#endif