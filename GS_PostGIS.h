// Gs_PostGIS.h : file di intestazione principale per la DLL Gs_PostGIS

#define INITGUID
#import "msado15.dll" no_namespace rename ("EOF", "EndOfFile") rename ("EOS", "ADOEOS")

#include "resource.h"		// simboli principali

#ifndef _gs_list_h
	#include "gs_list.h"
#endif

#ifndef _gs_cmd_h
	#include "gs_cmd.h"
#endif

#ifndef _map_postgis_h
	#include "Map_PostGIS.h"
#endif

#ifndef _map_h
	#include "Map.h"
#endif
 
class C_CMDLIST_POSTGIS : public C_CMDLIST
{
private:
	FunTable    *exfunPostgis;
	int         LenExfunPostgis;

public:
	C_CMDLIST_POSTGIS();
	~C_CMDLIST_POSTGIS() {};

    int funcLoad();
    int funcUnload();
    FunTable *get_ptr_fun(int val);
	int doFunc();
};

class PostGISAppl
{
   public:

	PostGISAppl() {};
   ~PostGISAppl() {};

	static C_CMDLIST_POSTGIS CMDLIST_POSTGIS;       // Lista dei comandi di Gs_PostGIS
 
	static AcRx::AppMsgCode LastAppMsgCode;
};
