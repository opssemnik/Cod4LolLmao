/*the fun starts kkk,qnd tiver pronto vai ser lecau*/

#include "Headers.h" 

LONG WINAPI ExceptionHandler( EXCEPTION_POINTERS* ExceptionInfo ); /*log usando winapi.h*/
BYTE OriginalBytes[5];

VOID SetBreakpoints()
{
	AddVectoredExceptionHandler( rand() % 0xFFFFFF, ExceptionHandler ); // n fiz issu, peguei cod da m$ pra add crash msm em programa ja compilado
	CONTEXT Context = { CONTEXT_DEBUG_REGISTERS };//checando os inject manero
	Context.Dr0 = Engine::DrawXModelSkinnedCachedOffset; //tamo de boa, vamo se mete nos metodo q da
	Context.Dr7 = 0x1; // fix: pareçe q o valor volta pra nulo qnd injetado
	SetThreadContext( GetCurrentThread(), &Context ); //check da injeçao
}

VOID MyDrawXModelSkinnedCached( INT a1, INT a2, INT a3 )
{
	CHAR* ModelName;
	//cheat engine start:axei os cod com cheat engine
	_asm PUSHAD;
	_asm MOV EBX, [EDI + 0xB8];
	_asm MOV EAX, [EBX];
	_asm MOV ModelName, EAX;
//ce ends herekkk
	static BOOL bRemove = FALSE;
//vamos criar nos buffer de render
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
//vc generated method
	if( Engine::D3DDevice != NULL && Engine::D3DDevice != NULL )
	{
		if( Tools::RedTexture == NULL || Tools::YellowTexture == NULL || Tools::SkyBlueTexture == NULL || Tools::GreenTexture == NULL )
		{
			Tools::GenerateTexture( Engine::D3DDevice, &Tools::RedTexture, D3DCOLOR_RGBA( 255, 0, 0, 255 ) );
			Tools::GenerateTexture( Engine::D3DDevice, &Tools::YellowTexture, D3DCOLOR_RGBA( 255, 255, 0, 255 ) );
			Tools::GenerateTexture( Engine::D3DDevice, &Tools::SkyBlueTexture, D3DCOLOR_RGBA( 0, 170, 255, 255 ) );
			Tools::GenerateTexture( Engine::D3DDevice, &Tools::GreenTexture, D3DCOLOR_RGBA( 0, 255, 0, 255 ) );
		}

		if( !strstr( ModelName, "mi24p" ) && !strstr( ModelName, "weapon" ) && !strstr( ModelName, "cobra" ) )
		{
			if( strstr( ModelName, "sas" ) || strstr( ModelName, "usmc" ) )
			{
				Engine::D3DDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
				Engine::D3DDevice->SetTexture( 0, Tools::YellowTexture );
				Engine::DrawXModelSkinnedCached( a1, a2, a3 );
				Engine::D3DDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
				Engine::D3DDevice->SetTexture( 0, Tools::RedTexture );
			}
			else if( strstr( ModelName, "opforce" ) || strstr( ModelName, "arab" ) || strstr( ModelName, "head_suren" ) )
			{
				Engine::D3DDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
				Engine::D3DDevice->SetTexture( 0, Tools::GreenTexture );
				Engine::DrawXModelSkinnedCached( a1, a2, a3 );
				Engine::D3DDevice->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
				Engine::D3DDevice->SetTexture( 0, Tools::SkyBlueTexture );
			}
		}
	}
//vc ends
	_asm POPAD;
//e vamos executar td
	Engine::DrawXModelSkinnedCached( a1, a2, a3 );
}
//debug
LONG WINAPI ExceptionHandler( EXCEPTION_POINTERS* ExceptionInfo )
{
	if( ExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_SINGLE_STEP && ( DWORD )ExceptionInfo->ExceptionRecord->ExceptionAddress == Engine::DrawXModelSkinnedCachedOffset )
	{
		ExceptionInfo->ContextRecord->Eip = ( DWORD )MyDrawXModelSkinnedCached;
		return EXCEPTION_CONTINUE_EXECUTION;
	} 

	return EXCEPTION_CONTINUE_SEARCH;
}
//inject sem tomar detect
DWORD WINAPI HookThread( VOID* Arguments )
{
	for( INT i = 0; i < 5; i++ )
		OriginalBytes[i] = *( BYTE* )( Engine::DrawXModelSkinnedCachedOffset + i );

	Engine::DrawXModelSkinnedCached = ( Engine::tDrawXModelSkinnedCached )Tools::JMPHook( ( BYTE* )Engine::DrawXModelSkinnedCachedOffset, ( BYTE* )MyDrawXModelSkinnedCached, 5 );

	return 0; 
}
//esse void é chamado qnd algo injeta esta .dll
BOOL WINAPI DllMain( HMODULE hModule, DWORD Reason, VOID* Reserved )
{	
	if( Reason == DLL_PROCESS_ATTACH )
		CreateThread( NULL, 0, ( LPTHREAD_START_ROUTINE )HookThread, NULL, 0x0, NULL );

	return TRUE;
} 