#ifndef TOOLS_H 
#define TOOLS_H 

#include "Headers.h" 

namespace Tools
{
	IDirect3DTexture9* RedTexture = NULL;
	IDirect3DTexture9* YellowTexture = NULL;
	IDirect3DTexture9* SkyBlueTexture = NULL;
	IDirect3DTexture9* GreenTexture = NULL;
//codigo da m$ (todo ele, por issu ta separado
	HRESULT GenerateTexture( IDirect3DDevice9* Device, IDirect3DTexture9** Texture, DWORD Color )
	{
		if( FAILED( Device->CreateTexture( 8, 8, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, Texture, NULL ) ) )
			return E_FAIL;
	
		WORD Color16 =	( ( WORD )( ( Color >> 28 ) & 0xF ) << 12 ) | ( WORD )( ( ( Color >> 20 ) & 0xF ) << 8 ) | ( WORD )( ( ( Color >> 12 ) & 0xF ) << 4 ) | ( WORD )( ( ( Color >> 4 ) & 0xF ) << 0 );

		D3DLOCKED_RECT d3dlr;    
		( *Texture )->LockRect( 0, &d3dlr, 0, 0 );

		WORD *Dst16 = ( WORD* )d3dlr.pBits;

		for( INT xy = 0; xy < 8 * 8; xy++ )
			*Dst16++ = Color16;

		( *Texture )->UnlockRect( 0 );
 
		return S_OK;
	}

	VOID WriteLog( CHAR* String, ... )
	{
		static CHAR Buffer[MAX_PATH];

		FILE* LogFile;

		va_list valist;
		va_start( valist, String );
		vsnprintf_s( Buffer, sizeof( Buffer ), sizeof( Buffer ), String, valist );
		va_end( valist );

		fopen_s( &LogFile, "LogFile.txt", "a+" );

		CHAR Timestruct[16] = "hh':'mm':'ss tt";                                  
		GetTimeFormat( NULL, NULL, NULL, NULL, Timestruct, 15 );

		fprintf( LogFile, "[%s] %s\n", Timestruct, Buffer );
		fclose( LogFile );

	}

	DWORD JMPHook( BYTE* Source, BYTE* Dest, INT Length )
	{
		DWORD Protect = 0x0;

		if( VirtualProtect( Source, Length, PAGE_EXECUTE_READWRITE, &Protect ) )
		{
			BYTE* Buffer = ( BYTE* )VirtualAlloc( NULL, Length + 5, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE );
			memcpy( Buffer, Source, Length );

			Buffer += Length;
			Buffer[0] = 0xE9;
			*( DWORD* )( Buffer + 1 ) = ( DWORD )( Source + Length - Buffer ) - 5;

			Source[0] = 0xE9;
			*( DWORD* )( Source + 1 ) = ( DWORD )( Dest - Source ) - 5;

			VirtualProtect( Source, Length, Protect, &Protect );

			return ( DWORD )( Buffer - Length );
		}

		return NULL;
	}
};

#endif