/*************************************************************************
Name: map_postgis

Module description: File per le definizioni dei tipi
            
Author: Caterina Gaeta

**************************************************************************/

/*************************************************************************
      
/*************************************************************************/

/*  map_postgis.H                                                            */

/*************************************************************************/

#ifndef _map_postgis_h
#define _map_postgis_h 1

#define INITGUID
#import "msado15.dll" no_namespace rename ("EOF", "EndOfFile") rename ("EOS", "ADOEOS")
#include <adeads.h>

#ifndef _gs_prjct_h
	#include "gs_prjct.h"
#endif

#ifndef _gs_error_h
	#include "gs_error.h"
#endif

#ifndef _gs_init_h
	#include "gs_init.h"
#endif

#ifndef _info_h
   #include "info.h"
#endif

#ifndef _coord_h
   #include "coord.h"
#endif

#ifndef _setting_h
   #include "setting.h"
#endif

#ifndef _base_h
	#include "base.h"
#endif

#ifndef _tb_class_h
	#include "tb_class.h"
#endif

/*************************************************************************/
/*  LOCAL FUNCTIONS FORWARD DECLARATIONS                                 */
/*************************************************************************/
int map_list(void);       // funzione chiamata dal lisp
int map_save_list(void);  // funzione chiamata dal lisp
int map_read_info(void);  // funzione chiamata dal lisp


class CMapPostGIS
{
public:
	CMapPostGIS(C_STRING name, C_STRING _server, C_STRING _user, C_STRING _font, C_STRING _schema, C_STRING _database, C_STRING _srid, C_STRING _inf, C_STRING _isOracle);
   ~CMapPostGIS();

protected:
	short EchoOld, MenuOld, CmddiaOld, BlipmodeOld, HighlightOld;			
	
	C_STRING	log_name, err_name, html_path;
	
	C_STRING geowork_path;
	C_STRING geosim_path;
	C_STRING sql_path;		// path dei file sql finali
	C_STRING postGis_path;

	CInfo map_info;
	CCoord coord_info;

	CSetting setting_info;
	
	C_STRING	geoininame;
	C_STRING	server;
	C_STRING user;
	C_STRING font;
	C_STRING schema;
	C_STRING database;
	long srid;
	bool inf;
	bool isOracle;
	C_STRING scriptname;

	FILE *geoscript;

	C_INT_INT_STR_LIST classList;
	CStringList blockList;

	C_STRING dwg_name;

	int K;

	CStringList reserved_list;

public:
	int Exec();
	int ExecSecExt();

private:
	int FindGeowork();
	int FindGeosim();
	int FindPostGIS();
	int Settings();
	int Start();
	int CoordSettings();	
	int SettingIniControl();
	int MapPostGISIniControl();
	int ProjectControl();
	int ProjectCreate();
	int ProjectClassWrite();
	int DwgsControl();
	int ChangeScriptNumber();
	int DwgExist();
	int DwgNoExist();
	int SecExtExist();

	int IsCurrentOpenedDwg(ade_id *dwg_id = ADE_NULLID);
	int ExistGeowebObjects();

	int CreateAllSqls(FILE *file, ade_id dwg_id);
	int CreateAllSecExt(FILE *file);

	int FindSql(presbuf sqlnames, presbuf sqldates, C_STRING name, C_STRING *sql_date);
	int DwgListBuild(presbuf dwgnames, presbuf dwgdates, presbuf sqlnames, presbuf sqldates, presbuf dwknames);
	int DwgListBuildNoDate(presbuf dwgnames, presbuf dwgdates, presbuf sqlnames, presbuf sqldates, presbuf dwknames);
	int FindFileDate(C_STRING path, C_STRING name, C_STRING *date);

	int UpdateMapPostGISIni();
	
	int FindPrjList(presbuf *rb);
	int FindClassList(presbuf *rb, int prj);
	int FindSelectedClasses();
	int FindFirstPrjIndex(CInfo *info);
	int FindClassIndex(C_INT_INT_STR *pItem, CInfo *info);
	int IsValidClass(int cls, int sub, CInfo *info);
	C_STRING FindTopobaseType(int category, int type);
	int FindDwk(presbuf dwknames);

	int DisableUndo();
	int set_cmddia();
	int set_blipmode();
	int set_highlight();
	int DeleteMapPostGISIni();
	int DeleteSettingIni();
	int RegSystemVar();
	int ResetSystemVar();
	int reset_cmddia(short val);
	int reset_blipmode(short val);
	int reset_highlight(short val);
	int RegLogFile(C_STRING input);
	int RegErrFile(C_STRING input);

	int ClassCoord();
	int PathWithExt(C_STRING *path, C_STRING ext);
};

#endif
