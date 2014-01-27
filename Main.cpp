#include "Headers.h" 
LONG WINAPI ExceptionHandler( EXCEPTION_POINTERS* ExceptionInfo ); 
BYTE OriginalBytes[5];

VOID SetBreakpoints()
{
	AddVectoredExceptionHandler( rand() % 0xFFFFFF, ExceptionHandler ); 
	CONTEXT Context = { CONTEXT_DEBUG_REGISTERS };

	Context.Dr0 = Engine::DrawXModelSkinnedCachedOffset;
	Context.Dr7 = 0x1;

	SetThreadContext( GetCurrentThread(), &Context );
}

VOID MyDrawXModelSkinnedCached( INT a1, INT a2, INT a3 )
{
	CHAR* ModelName;
	_asm{
		PUSHAD;
		MOV EBX, [EDI + 0xB8];
		MOV EAX, [EBX];
		MOV ModelName, EAX;
	}
	static BOOL bRemove = FALSE;

	if( !bRemove )
	{
		DWORD Protect = 0x0;
		VirtualProtect( ( VOID* )Engine::DrawXModelSkinnedCachedOffset, 5, PAGE_EXECUTE_READWRITE, &Protect );		
		for( INT i = 0; i < 5; i++ )
			*( BYTE* )( Engine::DrawXModelSkinnedCachedOffset + i ) = OriginalBytes[i];
		VirtualProtect( ( VOID* )Engine::DrawXModelSkinnedCachedOffset, 5, Protect, &Protect );
		SetBreakpoints();
		bRemove = TRUE;
	}

	if( Engine::D3DDevice != NULL)
	{
		if(Tools::Textura == NULL)
		{
			Tools::CreateTexturaDoCairo(Engine::D3DDevice, Tools::Textura);
		}

		if( !strstr( ModelName, "mi24p" ) && !strstr( ModelName, "weapon" ) && !strstr( ModelName, "cobra" ) )
		{
			if( strstr( ModelName, "sas" ) || strstr( ModelName, "usmc" ) )
			{
				Engine::D3DDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
				Engine::D3DDevice->SetTexture( 0, Tools::Textura );
				Engine::DrawXModelSkinnedCached( a1, a2, a3 );
				Engine::D3DDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
				Engine::D3DDevice->SetTexture(0, Tools::Textura);
			}
			else if( strstr( ModelName, "opforce" ) || strstr( ModelName, "arab" ) || strstr( ModelName, "head_suren" ) )
			{
				Engine::D3DDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
				Engine::D3DDevice->SetTexture(0, Tools::Textura);
				Engine::DrawXModelSkinnedCached( a1, a2, a3 );
				Engine::D3DDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
				Engine::D3DDevice->SetTexture(0, Tools::Textura);
			}
		}
	}

	_asm POPAD;

	Engine::DrawXModelSkinnedCached( a1, a2, a3 );
}

LONG WINAPI ExceptionHandler( EXCEPTION_POINTERS* ExceptionInfo )
{
	if( ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP && ( DWORD )ExceptionInfo->ExceptionRecord->ExceptionAddress == Engine::DrawXModelSkinnedCachedOffset )
	{
		ExceptionInfo->ContextRecord->Eip = ( DWORD )MyDrawXModelSkinnedCached;
		return EXCEPTION_CONTINUE_EXECUTION;
	} 

	return EXCEPTION_CONTINUE_SEARCH;
}

DWORD WINAPI HookThread( VOID* Arguments )
{
	for( INT i = 0; i < 5; i++ )
		OriginalBytes[i] = *( BYTE* )( Engine::DrawXModelSkinnedCachedOffset + i );

	Engine::DrawXModelSkinnedCached = ( Engine::tDrawXModelSkinnedCached )Tools::JMPHook( ( BYTE* )Engine::DrawXModelSkinnedCachedOffset, ( BYTE* )MyDrawXModelSkinnedCached, 5 );

	return 0; 
}

BOOL WINAPI DllMain( HMODULE hModule, DWORD Reason, VOID* Reserved )
{	
	if( Reason == DLL_PROCESS_ATTACH )
		CreateThread( NULL, 0, ( LPTHREAD_START_ROUTINE )HookThread, NULL, 0x0, NULL );

	return TRUE;
} 