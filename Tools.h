
#ifndef TOOLS_H 
#define TOOLS_H 
#include "Headers.h" 
namespace Tools
{
	IDirect3DTexture9* Textura = NULL;
	void CreateTexturaDoCairo(IDirect3DDevice9* Device, IDirect3DTexture9* text){
		HRESULT r = D3DXCreateTextureFromFile(Device, "cairo.jpg", &text);
		if (!r == D3D_OK){ //nao conseguimos criar textura ;//
			exit(r);
		}
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

		CHAR Timestruct[16] = "hh':'mm':'ss tt";    //format com date :D                              
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