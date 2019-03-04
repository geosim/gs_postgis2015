/**********************************************************
Name: Gs_Postgis

Module description: File contenente il main di Gs_Postgis
            
Author: Maria Paola Deprati & Roberto Poltini

(c) Copyright 1995-2012 by IREN ACQUA GAS  S.p.A.

              
Modification history:
              
Notes and restrictions on use: 

**********************************************************/


//-----------------------------------------------------------------------------
//- GS_PostGIS.cpp : Initialization functions
//-----------------------------------------------------------------------------
#include "StdAfx.h"
#include "resource.h"
#include <afxdllx.h>

#include "GS_PostGIS.h"


C_CMDLIST_POSTGIS PostGISAppl::CMDLIST_POSTGIS;              
AcRx::AppMsgCode PostGISAppl::LastAppMsgCode = AcRx::kNullMsg;


//-----------------------------------------------------------------------------
//- Define the sole extension module object.
AC_IMPLEMENT_EXTENSION_MODULE(GS_PostGISDLL)
//- Now you can use the CAcModuleResourceOverride class in
//- your application to switch to the correct resource instance.
//- Please see the ObjectARX Documentation for more details

//-----------------------------------------------------------------------------
//- DLL Entry Point
extern "C"
BOOL WINAPI DllMain (HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved) {
	//- Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved) ;

	if ( dwReason == DLL_PROCESS_ATTACH ) {
        _hdllInstance =hInstance ;
		GS_PostGISDLL.AttachInstance (hInstance) ;
		InitAcUiDLL () ;
	} else if ( dwReason == DLL_PROCESS_DETACH ) {
		GS_PostGISDLL.DetachInstance () ;
	}
	return (TRUE) ;
}


//-----------------------------------------------------------------------------
// Classe C_CMDLIST_POSTGIS
//-----------------------------------------------------------------------------
C_CMDLIST_POSTGIS::C_CMDLIST_POSTGIS()
{
	int i=0, offset=0;

	typedef struct 
	{
		TCHAR *name;
		int   (*fptr)();
		int   flag;		// controllo 1° Bit operativita su spazio carta no-si
						// controllo 2° Bit comando disponibile con o senza login
	} ftblent;

	// Il seguente vettore contiene i nomi delle funzioni LISP
	ftblent VectorLispFunction[] =
	{
	//  Inglese                    Nome interno               Flag di funzionamento
	//                                                        1° Bit = operativita su spazio carta no-si
	//                                                        2° Bit = comando disponibile con o senza login
		{_T("map_list"),			map_list,			0},
		{_T("map_save_list"),		map_save_list,		0},
		{_T("map_read_info"),		map_read_info,		0},
		{_T("map_postgisSecExt"),	map_postgisSecExt,	0},
		{_T("map_postgis"),			map_postgis,		0}
	};

	LenExfunPostgis = ELEMENTS(VectorLispFunction);
   
	// alloco exfun
	if ((exfunPostgis = (FunTable *) calloc(LenExfunPostgis, sizeof(FunTable))) == NULL)
		return; 

//	// PAOLETTA
//	//// caricamento vettore comandi GEOsim proprietari
//	//for (i = 0; i < ELEMENTS(VectorGeosimCmds); i++)
//	//{
//	//	exfun[i].name = VectorGeosimCmds[i].MultiName[index];
//	//	exfun[i].fptr = VectorGeosimCmds[i].fptr;
//	//	exfun[i].flag = VectorGeosimCmds[i].flag;
//	//}
//	//offset = i;
//	// caricamento vettore funzioni GEOsim LISP
	for (i = 0; i < ELEMENTS(VectorLispFunction); i++)
	{
		exfunPostgis[i + offset].name = VectorLispFunction[i].name;
		exfunPostgis[i + offset].fptr = VectorLispFunction[i].fptr;
		exfunPostgis[i + offset].flag = VectorLispFunction[i].flag;
	} 
}

int C_CMDLIST_POSTGIS::funcLoad()
{
   int i;

   for (i = 0; i < LenExfunPostgis; i++) 
      if (!acedDefun(exfunPostgis[i].name.get_name(), i)) return RTERROR;
    
   return RTNORM;
}

int C_CMDLIST_POSTGIS::funcUnload()
{
   int i;

   ////// se c'era una sessione di GEOsim aperta la chiudo
   ////if (GS_CURRENT_AREA) gsc_ExitCurrSession(GS_GOOD);

   // Undefine each function we defined
   for (i = 0; i < LenExfunPostgis; i++) 
      acedUndef(exfunPostgis[i].name.get_name(), i);
    
   return RTNORM;
}

FunTable* C_CMDLIST_POSTGIS::get_ptr_fun(int val)
{
   int    rc = GS_BAD;
   resbuf rb;

   acedRetNil();

   if (val >= LenExfunPostgis) return NULL;

   // se ci si trova nello spazio carta
   if (ads_getvar(_T("CVPORT"), &rb) != RTNORM || rb.restype != RTSHORT)
      { set_GS_ERR_COD(eGSVarNotDef); return NULL; }
   if (rb.resval.rint == 1)
      if (!(exfunPostgis[val].flag & 1))         // comando non abilitato allo spazio carta.
	  { set_GS_ERR_COD(eGSPaperSpaceFound); return NULL; }
                         
   //if (gsc_whoami(NULL) == GS_BAD)
   //   if (!(exfun[val].flag & 2))         // comando non abilitato senza login.
   //      return NULL;
	
   return &(exfunPostgis[val]);
}

int C_CMDLIST_POSTGIS::doFunc()
{
   int      val, rc = GS_BAD, old_reactor_abilit;
   FunTable *pfun;

   acedRetNil();

   if ((val = acedGetFunCode()) < 0) return RTERROR;

   do
   {
      if ((pfun = get_ptr_fun(val)) == NULL) break;

      // disattivo il controllo sul reattore
      old_reactor_abilit = gsc_disable_reactors();

      set_GS_ERR_COD(eGSNoError);

      rc = (*(pfun->fptr))();          // richiamo la funzione
   }
   while (0);

   if (rc == RTERROR || rc == GS_BAD)
      gsc_print_error();
   else
      if (rc == RTCAN || rc == GS_CAN)
         acutPrintf(gsc_msg(220)); // "\n*Annulla*\n"

   return ((rc == RTNORM) ? RSRSLT:RSERR);
}

//-----------------------------------------------------------------------------
#define szRDS _RXST("")

//-----------------------------------------------------------------------------
//----- ObjectARX EntryPoint
class CGS_PostGISApp : public AcRxArxApp {

public:
	CGS_PostGISApp () : AcRxArxApp () {}

	virtual AcRx::AppRetCode On_kInitAppMsg (void *pkt) {
		PostGISAppl::LastAppMsgCode = AcRx::kUnloadDwgMsg;

		// You *must* call On_kInitAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kInitAppMsg (pkt) ;
			
		C_STRING	newTitle;

		acutPrintf(_T("\nCaricamento GS_PostGIS.\n2012 IREN ACQUA GAS S.p.A."));
		newTitle = _T("Versione 2012.0.a");
		acutPrintf(_T("\n%s\n"), newTitle);

		return (retCode) ;
	}

	virtual AcRx::AppRetCode On_kUnloadAppMsg (void *pkt) {
		PostGISAppl::LastAppMsgCode = AcRx::kUnloadDwgMsg;

		// You *must* call On_kUnloadAppMsg here
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadAppMsg (pkt) ;

		// TODO: Unload dependencies here

		return (retCode) ;
	}

	virtual void RegisterServerComponents () {
	}

	virtual AcRx::AppRetCode On_kLoadDwgMsg (void *pkt)
	{
		PostGISAppl::LastAppMsgCode = AcRx::kUnloadDwgMsg;
		AcRx::AppRetCode retCode =AcRxArxApp::On_kLoadDwgMsg (pkt) ;

		////// se c'era una sessione di GEOsim aperta la chiudo
		////if (GS_CURRENT_AREA) gsc_ExitCurrSession(GS_GOOD);
		PostGISAppl::CMDLIST_POSTGIS.funcLoad();
		////// New AcDbDatabase, so if reactor exist attach it
		////if (GSDBReactor != NULL) DbReactorOn();

		return retCode;
	}

	virtual AcRx::AppRetCode On_kUnloadDwgMsg (void *pkt)
	{
		PostGISAppl::LastAppMsgCode = AcRx::kUnloadDwgMsg;
		AcRx::AppRetCode retCode =AcRxArxApp::On_kUnloadDwgMsg (pkt) ;
		PostGISAppl::CMDLIST_POSTGIS.funcUnload();
      
		return retCode;
	}

	virtual AcRx::AppRetCode On_kInvkSubrMsg (void *pkt)
	{
		PostGISAppl::LastAppMsgCode = AcRx::kInvkSubrMsg;
		PostGISAppl::CMDLIST_POSTGIS.doFunc();
    
		return AcRx::kRetOK;
	}

	// The ACED_ARXCOMMAND_ENTRY_AUTO macro can be applied to any static member 
	// function of the CGS_PostGISApp class.
	// The function should take no arguments and return nothing.
	//
	// NOTE: ACED_ARXCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid and
	// have arguments to define context and command mechanism.
	
	// ACED_ARXCOMMAND_ENTRY_AUTO(classname, group, globCmd, locCmd, cmdFlags, UIContext)
	// ACED_ARXCOMMAND_ENTRYBYID_AUTO(classname, group, globCmd, locCmdId, cmdFlags, UIContext)
	// only differs that it creates a localized name using a string in the resource file
	//   locCmdId - resource ID for localized command

	// Modal Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CGS_PostGISApp, MyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL)
	static void MyGroupMyCommand () {
		// Put your command code here

	}

	// Modal Command with pickfirst selection
	// ACED_ARXCOMMAND_ENTRY_AUTO(CGS_PostGISApp, MyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET)
	static void MyGroupMyPickFirst () {
		ads_name result ;
		int iRet =acedSSGet (ACRX_T("_I"), NULL, NULL, NULL, result) ;
		if ( iRet == RTNORM )
		{
			// There are selected entities
			// Put your command using pickfirst set code here
		}
		else
		{
			// There are no selected entities
			// Put your command code here
		}
	}

	// Application Session Command with localized name
	// ACED_ARXCOMMAND_ENTRY_AUTO(CGS_PostGISApp, MyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION)
	static void MyGroupMySessionCmd () {
		// Put your command code here
	}

	// The ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO macros can be applied to any static member 
	// function of the CGS_PostGISApp class.
	// The function may or may not take arguments and have to return RTNORM, RTERROR, RTCAN, RTFAIL, RTREJ to AutoCAD, but use
	// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal to return
	// a value to the Lisp interpreter.
	//
	// NOTE: ACED_ADSFUNCTION_ENTRY_AUTO / ACED_ADSCOMMAND_ENTRY_AUTO has overloads where you can provide resourceid.
	
	//- ACED_ADSFUNCTION_ENTRY_AUTO(classname, name, regFunc) - this example
	//- ACED_ADSSYMBOL_ENTRYBYID_AUTO(classname, name, nameId, regFunc) - only differs that it creates a localized name using a string in the resource file
	//- ACED_ADSCOMMAND_ENTRY_AUTO(classname, name, regFunc) - a Lisp command (prefix C:)
	//- ACED_ADSCOMMAND_ENTRYBYID_AUTO(classname, name, nameId, regFunc) - only differs that it creates a localized name using a string in the resource file

	// Lisp Function is similar to ARX Command but it creates a lisp 
	// callable function. Many return types are supported not just string
	// or integer.
	// ACED_ADSFUNCTION_ENTRY_AUTO(CGS_PostGISApp, MyLispFunction, false)
	static int ads_MyLispFunction () {
		//struct resbuf *args =acedGetArgs () ;
		
		// Put your command code here

		//acutRelRb (args) ;
		
		// Return a value to the AutoCAD Lisp Interpreter
		// acedRetNil, acedRetT, acedRetVoid, acedRetInt, acedRetReal, acedRetStr, acedRetPoint, acedRetName, acedRetList, acedRetVal

		return (RTNORM) ;
	}
	
} ;

//-----------------------------------------------------------------------------
IMPLEMENT_ARX_ENTRYPOINT(CGS_PostGISApp)

ACED_ARXCOMMAND_ENTRY_AUTO(CGS_PostGISApp, MyGroup, MyCommand, MyCommandLocal, ACRX_CMD_MODAL, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CGS_PostGISApp, MyGroup, MyPickFirst, MyPickFirstLocal, ACRX_CMD_MODAL | ACRX_CMD_USEPICKSET, NULL)
ACED_ARXCOMMAND_ENTRY_AUTO(CGS_PostGISApp, MyGroup, MySessionCmd, MySessionCmdLocal, ACRX_CMD_MODAL | ACRX_CMD_SESSION, NULL)
ACED_ADSSYMBOL_ENTRY_AUTO(CGS_PostGISApp, MyLispFunction, false)

