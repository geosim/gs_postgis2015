/*************************************************************************
Name: map_postgis.cpp

Module description: File funzioni di base per la creazione di files sql
            
Author: Caterina Gaeta & Maria Paola Deprati

**************************************************************************/
      
/*************************************************************************/
/*  map_postgis.CPP                                                          */
/*************************************************************************/

/*************************************************************************/
/* INCLUDES                                                              */
/*************************************************************************/
#include "stdafx.h"

#include "acedCmdNF.h" // per acedCommandS e acedCmdS
#include "map_postgis.h"

#include "gs_ase.h"
#include "gs_ade.h"
#include "gs_graph.h"   // per gsc_setLayer
#include "gs_thm.h"     // per gsc_explode

/*********************************************************/
/*.doc int  map_list <external> */
/*+
      Questa funzione lisp restituire al lisp la 
      lista dei progetti e delle classi 
      per il passaggio dati di PostGIS
      

      Restituisce RTNORM in caso di successo altrimenti restituisce RTERROR. 
-*/  
/*********************************************************/
extern int map_list(void)
{
	C_RB_LIST	result;			//	lista costruita leggendo in <giorno>.ini
	int			res = GS_GOOD;     
	int			int_val;

	TCHAR		val[10]; 

	FILE *geoini;
	C_STRING geoininame;
   
	acedRetNil();
   
	do
	{
		presbuf arg = ads_getargs();
		if (!arg || arg->restype != RTSTR)
		{ 
			set_GS_ERR_COD(eGSInvalidArg); 
			return GS_BAD;
		}
		geoininame = arg->resval.rstring;
		geoininame += ".ini";

		if (gsc_path_exist(geoininame))
		{
			if ((geoini = gsc_fopen(geoininame, _T("r"), ONETEST)) == NULL)
			{
				set_GS_ERR_COD(eGSOpenFile);
				res = GS_BAD;
				break;
			}
	
			//	legge la lista e la scrive in result
			while (!feof(geoini))
			{
				fwscanf_s(geoini, _T("%s\n"), &val);
				if (!gsc_strcmp(val, _T("(")))
					result += acutBuildList(RTLB, 0); 
				else if (!gsc_strcmp(val, _T(")")))
					result += acutBuildList(RTLE, 0); 
				else if (!gsc_strcmp(val, _T("nil")))
					result += acutBuildList(RTNIL, 0); 
				else
				{
         			swscanf_s(val, _T("%d"), &int_val);//conversione da stringa a int      
					result += acutBuildList(RTSHORT, int_val,0); 
				}
			}

			//	chiude <giorno>.ini
			if (gsc_fclose(geoini) == GS_BAD)
				return GS_BAD;
		}
		else
			break;
	}
	while (0);

	if (res == GS_BAD)
	{
		acedRetNil();
		return RTERROR;
	}
	else
	{
		result.LspRetList();

		//int i=1;
		//ads_retint(i);

		return RTNORM;
	}
}

/***************************************************************************/
/*.doc int  map_read_info <external> */
/*+
      Restituisce RTNORM in caso di successo altrimenti restituisce RTERROR. 
-*/  
/***************************************************************************/
extern int map_read_info(void)
{
	int		res = GS_GOOD;     

	FILE	*geoscr;
	C_STRING geoscrname;

	C_STRING	mask;
	TCHAR		str[255];
	C_STRING		_dir, _server, _user, _font, _schema, _database, _srid, _inf, _temp;

	bool	found;
	int		pos;

	acedRetNil();
   
	do
	{
		presbuf arg = ads_getargs();
		if (!arg || arg->restype != RTSTR)
		{ 
			set_GS_ERR_COD(eGSInvalidArg); 
			res = GS_BAD;
			break;
		}

		geoscrname = arg->resval.rstring;
		geoscrname += _T(".scr");

		if (gsc_path_exist(geoscrname))
		{
			if ((geoscr = gsc_fopen(geoscrname, _T("r"), ONETEST)) == NULL)
			{
				set_GS_ERR_COD(eGSOpenFile);
				res = GS_BAD;
				break;
			}

			if (fseek(geoscr, 0, SEEK_SET)!=0)
			{
				res = GS_BAD;
				break;
			}

			while (!feof(geoscr))
			{
				mask = _T("%s");
				do
				{
					fwscanf(geoscr, mask.get_name(), &str);
				}
				while (gsc_strcmp(str, _T("(map_postgis")) != 0);

				found = false;
				do
				{
					mask = _T("%s");
					fwscanf(geoscr, mask.get_name(), &str);
					
					_temp = str;
					pos = _temp.reverseFind(_T('"'));
					if ((pos != -1) && (pos != 0))
						found = true;

					if (_dir.len() > 0)
						_dir += " ";
					_dir += str;
				}
				while (!found);

				found = false;
				do
				{
					mask = _T("%s");
					fwscanf(geoscr, mask.get_name(), &str);
					
					_temp = str;
					pos = _temp.reverseFind(_T('"'));
					if ((pos != -1) && (pos != 0))
						found = true;

					if (_server.len() > 0)
						_server += " ";
					_server += str;
				}
				while (!found);

				found = false;
				do
				{
					mask = _T("%s");
					fwscanf(geoscr, mask.get_name(), &str);
					
					_temp = str;
					pos = _temp.reverseFind(_T('"'));
					if ((pos != -1) && (pos != 0))
						found = true;

					if (_user.len() > 0)
						_user += " ";
					_user += str;
				}
				while (!found);

				found = false;
				do
				{
					mask = _T("%s");
					fwscanf(geoscr, mask.get_name(), &str);
					
					_temp = str;
					pos = _temp.reverseFind(_T('"'));
					if ((pos != -1) && (pos != 0))
						found = true;

					if (_font.len() > 0)
						_font += " ";
					_font += str;
				}
				while (!found);

				found = false;
				do
				{
					mask = _T("%s");
					fwscanf(geoscr, mask.get_name(), &str);
					
					_temp = str;
					pos = _temp.reverseFind(_T('"'));
					if ((pos != -1) && (pos != 0))
						found = true;
					if (_schema.len() > 0)
						_schema += " ";
					_schema += str;
				}
				while (!found);

				found = false;
				do
				{
					mask = _T("%s");
					fwscanf(geoscr, mask.get_name(), &str);
					
					_temp = str;
					pos = _temp.reverseFind(_T('"'));
					if ((pos != -1) && (pos != 0))
						found = true;
					if (_database.len() > 0)
						_database += " ";
					_database += str;
				}
				while (!found);

				found = false;
				do
				{
					mask = _T("%s");
					fwscanf(geoscr, mask.get_name(), &str);
					
					_temp = str;
					pos = _temp.reverseFind(_T('"'));
					if ((pos != -1) && (pos != 0))
						found = true;
					if (_srid.len() > 0)
						_srid += " ";
					_srid += str;
				}
				while (!found);

				found = false;
				do
				{
					mask = _T("%s");
					fwscanf(geoscr, mask.get_name(), &str);
					
					_temp = str;
					pos = _temp.reverseFind(_T('"'));
					if ((pos != -1) && (pos != 0))
						found = true;
					if (_inf.len() > 0)
						_inf += " ";
					_inf += str;
				}
				while (!found);

				_dir.remove(_T(')'));
				_dir.remove(_T('"'));
				_server.remove(_T(')'));
				_server.remove(_T('"'));
				_user.remove(_T(')'));
				_user.remove(_T('"'));
				_font.remove(_T(')'));
				_font.remove(_T('"'));
				_schema.remove(_T(')'));
				_schema.remove(_T('"'));
				_database.remove(_T(')'));
				_database.remove(_T('"'));
				_srid.remove(_T(')'));
				_srid.remove(_T('"'));
				_inf.remove(_T(')'));
				_inf.remove(_T('"'));

				C_RB_LIST ret(acutBuildList(RTSTR, _server, RTSTR, _user, RTSTR, _font, RTSTR, _schema, RTSTR, _database, RTSTR, _srid, RTSTR, _inf, 0));
			   ret.LspRetList();
				break;
			}

			if (gsc_fclose(geoscr) == GS_BAD)
			{
				res = GS_BAD;
				break;
			}
		}
		else
			break;
	}
	while (0);

	return RTNORM;
}

/*********************************************************/
/*.doc int  map_save_list <external> */
/*+
      Questa funzione lisp prende come input la 
      lista dei progetti e delle classi 
      per il passaggio dati di PostGIS
      e la salva in <giorno>.ini
      

      Restituisce RTNORM in caso di successo altrimenti restituisce RTERROR. 
-*/  
/*********************************************************/
/*   ESEMPIO DI LISTA_FINALE (le parentesi iniziale e finale non vanno salvate)
(
 (1 (
      (28 nil)       (23 nil)       (70 nil)         nil
    )
 ) 
 
 (2 (
      (1 nil)         (2 nil)       (13 nil)         nil
    )
 ) 
 
 (6 (
      (6 nil)         (4 nil)         nil
    )
 ) 
 
 nil
)
*/
extern int map_save_list(void)
{
	C_STRING	path, server, user, font, schema, database, srid ,inf;
	C_STRING	name;
	C_STRING	ini;
	C_STRING	scr;

	FILE	*f;

	CInfo map_info;
   
	int	res = GS_GOOD;       
	int	val;

	C_STRING	str;
	
	acedRetNil();
   
	presbuf arg = ads_getargs();
	if (!arg || arg->restype != RTSTR)
	{ 
		set_GS_ERR_COD(eGSInvalidArg); 
		return GS_BAD;
	}
	
	path = arg->resval.rstring;
	path = gsc_strtran(path.get_name(), _T("\\"), _T("/"));
	if (gsc_drive2nethost(path) == GS_BAD) 
		return GS_BAD;
	name = path.get_name();

	ini = name;
	ini += ".ini";

	arg = arg->rbnext;
	server = arg->resval.rstring;

	arg = arg->rbnext;
	user = arg->resval.rstring;

	arg = arg->rbnext;
	font = arg->resval.rstring;

	arg = arg->rbnext;
	schema = arg->resval.rstring;

	arg = arg->rbnext;
	database = arg->resval.rstring;

	arg = arg->rbnext;
	srid = arg->resval.rstring;

	arg = arg->rbnext;
	inf = arg->resval.rstring;

	arg = arg->rbnext;

	do
	{
		//	apre il file <giorno>.ini in lettura
// PAOLA
//		if (find_geowork()!=GS_GOOD)//trova gs_path
//		{
//			res = GS_BAD;	
//			break;
//		}
      
		if ((f = gsc_fopen(ini, _T("w"), ONETEST)) == NULL)
		{
			set_GS_ERR_COD(eGSOpenFile);
			res = GS_BAD;
			break;
		}
      
		if (arg == NULL)		//	la lista può essere vuota
		{
			res = GS_BAD; 
			break; 
		}  
		else	//	lista da salvare in <giorno>.ini
		{
			//	salta la prima parentesi
			if (arg->restype != RTLB)                              
			{
				res = GS_BAD; 
				break;
			}
			
			do	//	per ogni progetto
			{
				if (res == GS_BAD)
					break;

				arg=arg->rbnext;  

				if (arg == NULL)	//	errori nella lista
				{
					res=GS_BAD; 
					break;
				}            
            
				if ((arg->restype == RTNIL)&&(arg->rbnext->restype == RTLE)) //fine lista (salta l'ultima parentesi)
				{
					fwprintf(f, _T("nil\n"));   
					break;
				}

				if (arg->restype != RTLB)                              
				{
					res = GS_BAD; 
					break;
				}
				fwprintf(f, _T("(\n"));			//	inizio progetto

				arg=arg->rbnext;  

				if (arg->restype != RTSHORT)	//	codice progetto                           
				{
					res = GS_BAD; 
					break;
				}
				val=arg->resval.rint;
				fwprintf(f, _T("%d\n"), val);

				arg=arg->rbnext;  

				if (arg->restype != RTLB)                              
				{
					res = GS_BAD; 
					break;
				}
				fwprintf(f, _T("(\n"));
            
				arg=arg->rbnext;  
				do		//	lista classi
				{
					//	fine lista classi
					if ((arg->restype == RTNIL)&&(arg->rbnext->restype == RTLE))  
					{
						arg=arg->rbnext->rbnext;  
						fwprintf(f, _T("nil\n)\n"));
						break; 
					}

					if (arg->restype != RTLB)                              
					{
						res = GS_BAD; 
						break;
					}
					fwprintf(f, _T("(\n"));

					arg=arg->rbnext;  
					if (arg->restype != RTSHORT)                              
					{	
						res = GS_BAD; 
						break;
					}
					val=arg->resval.rint;
					fwprintf(f, _T("%d\n"), val);

					arg=arg->rbnext;  
					if ((arg->restype != RTNIL)&&(arg->restype != RTSHORT))
					{	
						res = GS_BAD; 
						break;
					}
					if (arg->restype == RTNIL)
					{	
						fwprintf(f, _T("nil\n"));
					}
					if (arg->restype == RTSHORT)
					{	
						val=arg->resval.rint;
						fwprintf(f, _T("%d\n"), val);

						arg=arg->rbnext;  
						if ((arg->restype != RTNIL)&&(arg->restype != RTSHORT))
						{	
							res = GS_BAD; 
							break;
						}
						if (arg->restype == RTNIL)
						{	
							fwprintf(f, _T("nil\n"));
						}
					}

					arg=arg->rbnext;  
					if (arg->restype != RTLE)                              
					{
						res = GS_BAD; 
						break;
					}
					fwprintf(f, _T(")\n"));

					arg=arg->rbnext;  

				} while (1);

				if (arg->restype != RTLE)                              
				{
					res = GS_BAD; 
					break;
				}
				
				fwprintf(f, _T(")\n"));		//	fine progetto
			
			} while (1);
		}
	} while (0);

	if (res == GS_GOOD)
	{
		//	cancella map_postgis.ini
// PAOLA
//		DeleteMapPostGISIni();
		//	chiude <giorno>.ini
		if (gsc_fclose(f) == GS_BAD)
			return GS_BAD;
		acutPrintf(_T("\nLista delle classi selezionate per il passaggio dati PostGIS salvata nel file %s"), ini);
	}
	else
	{
		if (gsc_fclose(f) == GS_BAD)
			return GS_BAD;
		gsc_delfile(ini, MORETESTS);
		acutPrintf(_T("\nLista non valida o lista vuota"));
		
		C_STRING msg;
		msg = "\nIl file ";
		msg += ini;
		msg += " è stato cancellato";
		acutPrintf(msg.get_name());
	
		return GS_BAD;
	}

	do
	{
		scr = name;
		scr += ".scr";

		if ((f = gsc_fopen(scr, _T("w"), ONETEST)) == NULL)
		{
			set_GS_ERR_COD(eGSOpenFile);
			res = GS_BAD;
			break;
		}

		str =	"(setq geosim_workdir (getenv \"GEOWORK\"))";
		str += "\n(load (strcat geosim_workdir \"/gs\"))";
		str += "\n(gs_login \"";
		str += get_CURRENT_USER()->log; // login utente corrente
		str += "\" \"";
		str += get_CURRENT_USER()->pwd; // password utente corrente
		str += "\" nil)";
		str += "\n(load (strcat geosim_workdir \"/postgis/postgis\"))";
		str += "\n(map_postgis \"";
		str += ini;
		str += "\" \"";
		str += server.get_name();
		str += "\" \"";
		str += user.get_name();
		str += "\" \"";
		str += font.get_name();
		str += "\" \"";
		str += schema.get_name();
		str += "\" \"";
		str += database.get_name();
		str += "\" \"";
		str += srid.get_name();
		str += "\" \"";
		str += inf.get_name();
		str += "\")\n";

		fwprintf(f, str.get_name());
	} while (0);

	if (res == GS_GOOD)
	{
		if (gsc_fclose(f) == GS_BAD)
			return GS_BAD;
	}
	else
	{
		C_STRING msg;
		msg = "\nIl file ";
		msg += scr;
		msg += " è stato cancellato";
		acutPrintf(msg.get_name());
	
		return GS_BAD;
	}

	do
	{
		scr = name;
		scr += "SecExt.scr";

		if ((f = gsc_fopen(scr, _T("w"), ONETEST)) == NULL)
		{
			set_GS_ERR_COD(eGSOpenFile);
			res = GS_BAD;
			break;
		}

		str =	"(setq geosim_workdir (getenv \"GEOWORK\"))";
		str += "\n(load (strcat geosim_workdir \"/gs\"))";
		str += "\n(gs_login \"";
		str += get_CURRENT_USER()->log; // login utente corrente
		str += "\" \"";
		str += get_CURRENT_USER()->pwd; // password utente corrente
		str += "\" nil)";
		str += "\n(load (strcat geosim_workdir \"/postgis/postgis\"))";
		str += "\n(map_postgisSecExt \"";
		str += ini;
		str += "\" \"";
		str += server.get_name();
		str += "\" \"";
		str += user.get_name();
		str += "\" nil \"";
		str += schema.get_name();
		str += "\" \"";
		str += database.get_name();
		str += "\" \"";
		str += srid.get_name();
		str += "\" \"";
		str += inf.get_name();
		str += "\")\n";

		fwprintf(f, str.get_name());	

	} while (0);

	if (res == GS_GOOD)
	{
		if (gsc_fclose(f) == GS_BAD)
			return GS_BAD;
	}
	else
	{
		C_STRING msg;
		msg = "\nIl file ";
		msg += scr;
		msg += " è stato cancellato";
		acutPrintf(msg.get_name());
	
		return GS_BAD;
	}
		
	return RTNORM;
}

CMapPostGIS::CMapPostGIS(C_STRING name, C_STRING _server, C_STRING _user, C_STRING _font, C_STRING _schema, C_STRING _database, C_STRING _srid, C_STRING _inf, C_STRING _isOracle)
{  
	geoininame = name;
	server = _server;
	if (_user.len()>0)
		user = _user;
	else
		user = _T("postgres");
	font = _font;
	schema = _schema;
	database = _database;
	if (_srid.len()>0)
		srid = _srid.tol();
	else
		srid = -1;
	if (_inf == "1")
		inf = true;
	else
		inf = false;
	geoscript = NULL;

	reserved_list.AddTail("area");
	reserved_list.AddTail("block_name");
	reserved_list.AddTail("block_scale_factor");
	reserved_list.AddTail("fid");
	reserved_list.AddTail("fid_parent");
	reserved_list.AddTail("hatch_color");
	reserved_list.AddTail("hatch_name");
	reserved_list.AddTail("hatch_l_width");
	reserved_list.AddTail("hatch_l_type");
	reserved_list.AddTail("hatch_space");
	reserved_list.AddTail("hidden");
	reserved_list.AddTail("label_text");
	reserved_list.AddTail("field_name");
	reserved_list.AddTail("layer");
	reserved_list.AddTail("length");
	reserved_list.AddTail("l_type");
	reserved_list.AddTail("l_width");
	reserved_list.AddTail("n_color");
	reserved_list.AddTail("orientation_degree");
	reserved_list.AddTail("text_");
	reserved_list.AddTail("text_height");
	reserved_list.AddTail("text_style");
	reserved_list.AddTail("thickness");
	reserved_list.AddTail("geom");
	reserved_list.AddTail("wkb_geometry");
	reserved_list.AddTail("data");

	if (_isOracle == "true")
		isOracle = true;
	else
		isOracle = false;
}

CMapPostGIS::~CMapPostGIS() 
{
}

/************************************************************
/*.doc	int CMapPostGIS::Exec()	 							*/
/*+
	Questa funzione esegue l'operazione di trasformazione.
   
  	Parametri:             
    
	GS_GOOD in caso di successo altrimenti GS_BAD.
-*/
/************************************************************/
int CMapPostGIS::Exec()
{
	int		ret=GS_GOOD; 

	TRY
	{
		if (FindGeowork() == GS_BAD)	// setta gs_path alla directory di GEOWORK
			AfxThrowUserException();

		if (FindGeosim() == GS_BAD)		// setta gs_path alla directory di GEOSIM
			AfxThrowUserException();

		if (FindPostGIS() == GS_BAD)	// setta postGis_path alla directory di POSTGIS
			AfxThrowUserException();

		if (Settings() == GS_BAD)
			AfxThrowUserException();

		if (Start() == GS_BAD)
			AfxThrowUserException();
		
		if (CoordSettings() == GS_BAD)	// setta parametri per trasformazioni coordinate
			AfxThrowUserException();	// e percorso per file sql finali

		if (SettingIniControl() == GS_BAD)
			AfxThrowUserException();

		if (MapPostGISIniControl() == GS_BAD)
			AfxThrowUserException();

		if (ProjectControl() == GS_BAD)
			AfxThrowUserException();

		if (ProjectCreate() == GS_BAD)
			AfxThrowUserException();

		if (ProjectClassWrite() == GS_BAD)
			AfxThrowUserException();

		if (DwgsControl() == GS_BAD)
			AfxThrowUserException();

		if (ChangeScriptNumber() == GS_BAD)
			AfxThrowUserException();

		map_info.get_last_dwg(&dwg_name);
		if (dwg_name != "")
			DwgExist();
		else
			DwgNoExist();
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

int CMapPostGIS::Settings()
{
	int		ret=GS_GOOD; 

	TRY
	{
		if (DisableUndo() == GS_BAD)			// UNDO disabilitato
			AfxThrowUserException();
		
		if (set_echo(&EchoOld) != GS_GOOD)		//	registra valori preesistenti
			AfxThrowUserException();
		
		if (set_menu(&MenuOld) != GS_GOOD)
			AfxThrowUserException();
		
		if (set_cmddia() != GS_GOOD)
			AfxThrowUserException();
		
		if (set_blipmode() != GS_GOOD)
			AfxThrowUserException();
		
		if (set_highlight() != GS_GOOD)
			AfxThrowUserException();

		if (RegSystemVar() == GS_BAD)
		{
			ResetSystemVar();		
			AfxThrowUserException();			//	registra i valori di CMDECHO, MENUECHO e CMDDIA
		}
	
		scriptname = geowork_path;
		scriptname += POSTGIS;
		scriptname += "\\GEOSCRIP";
		scriptname += map_info.get_script_number();
		scriptname += ".SCR";
	}
	CATCH( CUserException, e )
	{
		set_GS_ERR_COD(eGSInitPar);
		DeleteMapPostGISIni();
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

/****************************************************************************/
/*.doc CMapPostGIS::DisableUndo           										*/
/*+
	Questa funzione disabilita l'<undo> di AutoCAD

	Restituisce GS_GOOD in caso di successo altrimenti restituisce GS_BAD.
-*/  
/****************************************************************************/
int CMapPostGIS::DisableUndo()
{
	int ret = GS_GOOD;

	struct resbuf rb;
	int UndoState = 0;

	TRY
	{
		if (ads_getvar(_T("UNDOCTL"), &rb) != RTNORM)
			AfxThrowUserException();

		if (rb.restype == RTSHORT && (UndoState = rb.resval.rint) > 0)	// undo abilitato
			gsc_disableUndoRecording(true);	// undo disabilito 
				AfxThrowUserException();
		acutPrintf(gsc_msg(597)); // "\nLe operazioni effettuate fino ad ora non saranno più annullabili."
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

/************************************************************/
/*.doc CMapPostGIS::set_cmddia									*/
/*+
	Questa funzione legge il valore della variabile globale
	di AutoCAD "CMDDIA", lo memorizza nell'intero CmddiaOld 
	e setta il nuovo valore di "CMDDIA" a 0

	Restituisce GS_BAD se non riesce a settare il nuovo 
	valore o ad ottenere il vecchio
-*/  
/************************************************************/
int CMapPostGIS::set_cmddia()
{
	int ret = GS_GOOD;

	struct resbuf ValOld;

	TRY
	{
		if (ads_getvar(_T("CMDDIA"), &ValOld) != RTNORM)
			AfxThrowUserException();
		CmddiaOld=ValOld.resval.rint;

		if (reset_cmddia(0) == GS_BAD)
			AfxThrowUserException();
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

/************************************************************/
/*.doc CMapPostGIS::set_blipmode								*/
/*+
	Questa funzione legge il valore della variabile globale
	di AutoCAD "BLIPMODE", lo memorizza nell'intero 
	BlipmodeOld e setta il nuovo valore di "BLIPMODE" a 0

	Restituisce GS_BAD se non riesce a settare il nuovo 
	valore o ad ottenere il vecchio
-*/  
/************************************************************/
int CMapPostGIS::set_blipmode()
{
	int ret = GS_GOOD;

	struct resbuf ValOld;

	TRY
	{
		if (ads_getvar(_T("BLIPMODE"), &ValOld) != RTNORM)
			AfxThrowUserException();
		BlipmodeOld=ValOld.resval.rint;

		if (reset_blipmode(0) == GS_BAD)
			AfxThrowUserException();
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

/************************************************************/
/*.doc CMapPostGIS::set_highlight								*/
/*+
	Questa funzione legge il valore della variabile globale
	di AutoCAD "HIGHLIGHT", lo memorizza nell' intero 
	HighlightOld e setta il nuovo valore di "HIGHLIGHT" a 0

	Restituisce GS_BAD se non riesce a settare il nuovo 
	valore o ad ottenere il vecchio
-*/  
/************************************************************/
int CMapPostGIS::set_highlight()
{
	int ret = GS_GOOD;

	struct resbuf ValOld;

	TRY
	{
		if (ads_getvar(_T("HIGHLIGHT"), &ValOld) != RTNORM)
			AfxThrowUserException();
		HighlightOld=ValOld.resval.rint;

		if (reset_highlight(0) == GS_BAD)
			AfxThrowUserException();
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}


/************************************************************/
/*.doc CMapPostGIS::DeleteMapPostGISIni								*/
/*+
	Questa funzione elimina il file mapPostgis.ini.

	Parametri:

	Restituisce GS_BAD se non riesce a eliminarlo.
-*/  
/************************************************************/
int CMapPostGIS::DeleteMapPostGISIni()
{
	int ret = GS_GOOD;

	TRY
	{
		if (map_info.remove_fid_list(geowork_path) == GS_BAD)
				AfxThrowUserException();

		if (gsc_path_exist(map_info.get_map_postGis_path()) == GS_GOOD)
		{
			map_info.clear();
			if (gsc_delfile(map_info.get_map_postGis_path(), ONETEST) == GS_BAD)
				AfxThrowUserException();
		}
	}
	CATCH( CUserException, e )
	{
      C_STRING Msg;

      Msg = _T("Il file <");
      Msg +=  map_info.get_map_postGis_path();
      Msg += _T("> potrebbe non essere stato cancellato.<BR>");
		RegLogFile(Msg.get_name());
		acutPrintf(Msg.get_name());
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}


int CMapPostGIS::DeleteSettingIni()
{
	int ret = GS_GOOD;

	TRY
	{
		if (gsc_path_exist(setting_info.get_setting_path()) == GS_GOOD)
		{
			setting_info.clear();
			if (gsc_delfile(setting_info.get_setting_path(), ONETEST) == GS_BAD)
				AfxThrowUserException();
		}
	}
	CATCH( CUserException, e )
	{
      C_STRING Msg;

      Msg = _T("Il file <");
      Msg += setting_info.get_setting_path();
      Msg += _T("> potrebbe non essere stato cancellato.");
		RegLogFile(Msg.get_name());
		acutPrintf(Msg.get_name());
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}


/************************************************************/
/*.doc CMapPostGIS::FindGeowork									*/
/*+
	Questa funzione trova il path di GEOWORK e lo memorizza 
	in gs_path

	Restituisce GS_BAD se non ottiene il valore
-*/  
/************************************************************/
int CMapPostGIS::FindGeowork() 
{
	int ret = GS_GOOD;

	presbuf result;

	TRY
	{
		if ((result = gsc_GetVar(_T("GS_WORKDIR"))) == NULL) 
			AfxThrowUserException();
		else
			if (result->restype != RTSTR)
			{
				ads_relrb(result);
				result=NULL;
				AfxThrowUserException();
			}

		geowork_path = result->resval.rstring;
		geowork_path+="/";	
			
		ads_relrb(result); 
		result=NULL;
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

/************************************************************/
/*.doc CMapPostGIS::FindGeosim								*/
/*+
	Questa funzione trova il path di GEOSIM e lo memorizza 
	in gs_path

	Restituisce GS_BAD se non ottiene il valore
-*/  
/************************************************************/
int CMapPostGIS::FindGeosim() 
{
	int ret = GS_GOOD;

	presbuf result;

	TRY
	{
		if ((result = gsc_GetVar(_T("GS_GEODIR"))) == NULL) 
			AfxThrowUserException();
		else
			if (result->restype != RTSTR)
			{
				ads_relrb(result);
				result=NULL;
				AfxThrowUserException();
			}

		geosim_path = result->resval.rstring;
		geosim_path+="/";	
			
		ads_relrb(result); 
		result=NULL;
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

/************************************************************/
/*.doc CMapPostGIS::FindPostGIS									*/
/*+
	Questa funzione trova il path di POSTGIS e lo memorizza 
	in postGis_path

	Restituisce GS_BAD se non ottiene il valore
-*/  
/************************************************************/
int CMapPostGIS::FindPostGIS() 
{
	int ret = GS_GOOD;

	TRY
	{
		postGis_path = _wgetenv(_T("POSTGIS"));
		if (postGis_path == "")
		{
			acutPrintf(_T("\nVariabile di sistema POSTGIS non settata.\n"));
			AfxThrowUserException();
		}
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

/************************************************************/
/*.doc CMapPostGIS::RegSystemVar								*/
/*+
	Questa funzione memorizza i valori delle variabili 
	globali di AutoCAD "CMDECHO", "MENUECHO" e "CMDDIA" 
	nel file di inizializzazione "geows_postgis.ini" 

      
	Restituisce GS_BAD se non riesce a memorizzare i valori 
-*/  
/************************************************************/
int CMapPostGIS::RegSystemVar()
{
	int ret = GS_GOOD;

	FILE *file;
	C_STRING path;

	TCHAR val[50];
	TCHAR *result=NULL;

	TRY
	{
		path = geowork_path;
		path += POSTGIS;
		path += "\\geows_postgis.ini";								

		if ((file = gsc_open_profile(path, READONLY)) == NULL)			// file non esistente
		{
			if ((file = gsc_open_profile(path, UPDATEABLE)) == NULL)
				AfxThrowUserException();

			gsc_itoa(EchoOld,val,10);					//	conversione da int a stringa - base 10
			if (gsc_set_profile(file, _T("SYSTEM"), _T("ECHO"), val, 0) == GS_BAD)
				AfxThrowUserException();
									
			gsc_itoa(MenuOld,val,10);
			if (gsc_set_profile(file, _T("SYSTEM"), _T("MENU"), val, 0) == GS_BAD)
				AfxThrowUserException();

			gsc_itoa(CmddiaOld,val,10);
			if (gsc_set_profile(file, _T("SYSTEM"), _T("CMD"), val, 0) == GS_BAD)
				AfxThrowUserException();
									
			gsc_itoa(BlipmodeOld,val,10);
			if (gsc_set_profile(file, _T("SYSTEM"), _T("BLIP"), val, 0) == GS_BAD)
				AfxThrowUserException();

			gsc_itoa(HighlightOld,val,10);
			if (gsc_set_profile(file, _T("SYSTEM"), _T("HIGH"), val, 0) == GS_BAD)
				AfxThrowUserException();
      
			if (gsc_close_profile(file) == GS_BAD)
				AfxThrowUserException();
		}
		else		// file esistente
		{
			if (gsc_find_sez(file, _T("SYSTEM"), 6, NULL) == GS_BAD)		// sezione non	esistente
			{	
				if (gsc_close_profile(file) == GS_BAD)
					AfxThrowUserException();
								
				if ((file = gsc_open_profile(path, UPDATEABLE)) == NULL)
					AfxThrowUserException();
									
				gsc_itoa(EchoOld,val,10);					//	conversione da int a stringa - base 10
				if (gsc_set_profile(file, _T("SYSTEM"), _T("ECHO"), val, 0) == GS_BAD)
					AfxThrowUserException();
										
				gsc_itoa(MenuOld,val,10);
				if (gsc_set_profile(file, _T("SYSTEM"), _T("MENU"), val, 0) == GS_BAD)
					AfxThrowUserException();

				gsc_itoa(CmddiaOld,val,10);
				if (gsc_set_profile(file, _T("SYSTEM"), _T("CMD"), val, 0) == GS_BAD)
					AfxThrowUserException();
										
				gsc_itoa(BlipmodeOld,val,10);
				if (gsc_set_profile(file, _T("SYSTEM"), _T("BLIP"), val, 0) == GS_BAD)
					AfxThrowUserException();

				gsc_itoa(HighlightOld,val,10);
				if (gsc_set_profile(file, _T("SYSTEM"), _T("HIGH"), val, 0) == GS_BAD)
					AfxThrowUserException();
      
				if (gsc_close_profile(file) == GS_BAD)
					AfxThrowUserException();
			}
			else		// sezione esistente
			{
				if (gsc_get_profile(file, _T("SYSTEM"), _T("ECHO"),	&result, 0, 0) == GS_BAD)
				{	
					if (gsc_close_profile(file) == GS_BAD)
						AfxThrowUserException();

					if ((file = gsc_open_profile(path, UPDATEABLE)) == NULL)
						AfxThrowUserException();
						
					gsc_itoa(EchoOld,val,10);					//	conversione da int a stringa - base 10
					if (gsc_set_profile(file, _T("SYSTEM"), _T("ECHO"), val, 0) == GS_BAD)
						AfxThrowUserException();

					if (gsc_close_profile(file) == GS_BAD)
						AfxThrowUserException();

					if ((file = gsc_open_profile(path, UPDATEABLE)) == NULL)
						AfxThrowUserException();
				}
					
				if (result)
					{free (result); result=NULL;}

				if (gsc_get_profile(file, _T("SYSTEM"), _T("MENU"),	&result, 0, 0) == GS_BAD)
				{	
					if (gsc_close_profile(file) == GS_BAD)
						AfxThrowUserException();

					if ((file = gsc_open_profile(path, UPDATEABLE)) == NULL)
						AfxThrowUserException();
										
					gsc_itoa(MenuOld,val,10);
					if (gsc_set_profile(file, _T("SYSTEM"), _T("MENU"), val, 0) == GS_BAD)
						AfxThrowUserException();

					if (gsc_close_profile(file) == GS_BAD)
						AfxThrowUserException();

					if ((file = gsc_open_profile(path, UPDATEABLE)) == NULL)
						AfxThrowUserException();
				}
									
				if (result)
					{free (result); result=NULL;}

				if (gsc_get_profile(file, _T("SYSTEM"), _T("CMD"),	&result, 0, 0) == GS_BAD)
				{	
					if (gsc_close_profile(file) == GS_BAD)
						AfxThrowUserException();

					if ((file = gsc_open_profile(path, UPDATEABLE)) == NULL)
						AfxThrowUserException();
										
					gsc_itoa(CmddiaOld,val,10);
					if (gsc_set_profile(file, _T("SYSTEM"), _T("CMD"), val, 0) == GS_BAD)
						AfxThrowUserException();

					if (gsc_close_profile(file) == GS_BAD)
						AfxThrowUserException();

					if ((file = gsc_open_profile(path, UPDATEABLE)) == NULL)
						AfxThrowUserException();
				}

				if (result)
					{free (result); result=NULL;}

				if (gsc_get_profile(file, _T("SYSTEM"), _T("BLIP"),	&result, 0, 0) == GS_BAD)
				{	
					if (gsc_close_profile(file) == GS_BAD)
						AfxThrowUserException();

					if ((file = gsc_open_profile(path, UPDATEABLE)) == NULL)
						AfxThrowUserException();
										
					gsc_itoa(BlipmodeOld,val,10);
					if (gsc_set_profile(file, _T("SYSTEM"), _T("BLIP"), val, 0) == GS_BAD)
						AfxThrowUserException();

					if (gsc_close_profile(file) == GS_BAD)
						AfxThrowUserException();

					if ((file = gsc_open_profile(path, UPDATEABLE)) == NULL)
						AfxThrowUserException();
				}
				
				if (result)
					{free (result); result=NULL;}

				if (gsc_get_profile(file, _T("SYSTEM"), _T("HIGH"),	&result, 0, 0) == GS_BAD)
				{	
					if (gsc_close_profile(file) == GS_BAD)
						AfxThrowUserException();

					if ((file = gsc_open_profile(path, UPDATEABLE)) == NULL)
						AfxThrowUserException();
										
					gsc_itoa(HighlightOld,val,10);
					if (gsc_set_profile(file, _T("SYSTEM"), _T("HIGH"), val, 0) == GS_BAD)
						AfxThrowUserException();

					if (gsc_close_profile(file) == GS_BAD)
						AfxThrowUserException();

					if ((file = gsc_open_profile(path, UPDATEABLE)) == NULL)
						AfxThrowUserException();
				}
									
				if (result) 
					{free (result); result=NULL;}
								
				if (gsc_close_profile(file) == GS_BAD)
					AfxThrowUserException();
			}
		}
	}
	CATCH( CUserException, e )
	{
		if (file)
			gsc_close_profile(file);
		
		if (result) 
			{ free (result); result=NULL;}
		
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

/************************************************************/
/*.doc CMapPostGIS::ResetSystemVar								*/
/*+
	Questa funzione legge i valori memorizzati precedentemente
	delle variabili globali di AutoCAD "CMDECHO", "MENUECHO" e
	"CMDDIA" nel file di inizializzazione "geows_postgis.ini" e 
	richiama le funzioni per settare questi valori.


	Restituisce GS_BAD se non riesce a leggere i valori o a 
	settarli
-*/  
/************************************************************/
int CMapPostGIS::ResetSystemVar()
{
	int ret = GS_GOOD;

	FILE *file;
	C_STRING path;

	short echo_val, menu_val, cmd_val, blip_val, high_val;
	TCHAR *result=NULL;

	TRY
	{
		path = geowork_path;
		path += POSTGIS;
		path += "\\geows_postgis.ini";

		if ((file = gsc_open_profile(path, READONLY)) == NULL)
			AfxThrowUserException();
							
		if (gsc_get_profile(file, _T("SYSTEM"), _T("ECHO"),	&result, 0, 0) == GS_BAD)
			AfxThrowUserException();
		echo_val = _wtoi(result);

		if (result)
			{free (result); result=NULL;}
		if (gsc_get_profile(file, _T("SYSTEM"), _T("MENU"), &result, 0, 0) == GS_BAD)
			AfxThrowUserException();
		menu_val = _wtoi(result);

		if (result)
			{free (result); result=NULL;}
		if (gsc_get_profile(file, _T("SYSTEM"), _T("CMD"), &result, 0, 0) == GS_BAD)
			AfxThrowUserException();
		cmd_val = _wtoi(result);

		if (result)
			{free (result); result=NULL;}
		if (gsc_get_profile(file, _T("SYSTEM"), _T("BLIP"),	&result, 0, 0) == GS_BAD)
			AfxThrowUserException();
		blip_val = _wtoi(result);

		if (result)
			{free (result); result=NULL;}
		if (gsc_get_profile(file, _T("SYSTEM"), _T("HIGH"),	&result, 0, 0) == GS_BAD)
			AfxThrowUserException();
		high_val = _wtoi(result);

		if (result)
			{free (result); result=NULL;}
		if (gsc_close_profile(file) == GS_BAD)
			AfxThrowUserException();
							
		if (reset_echo(echo_val) == GS_BAD)
			AfxThrowUserException();
		if (reset_menu(menu_val) == GS_BAD)
			AfxThrowUserException();
		if (reset_cmddia(cmd_val) == GS_BAD)
			AfxThrowUserException();
		if (reset_blipmode(blip_val) == GS_BAD)
			AfxThrowUserException();
		if (reset_highlight(high_val) == GS_BAD)
			AfxThrowUserException();
		//if (reset_demandLoad() == GS_BAD)
			//	return GS_BAD;
	}
	CATCH( CUserException, e )
	{
		if (result)
			{free (result); result=NULL;}
		set_GS_ERR_COD(eGSInitPar); 
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}


/************************************************************/
/*.doc CMapPostGIS::reset_cmddia								*/
/*+
	Questa funzione setta il valore della variabile globale
	di AutoCAD "CMDDIA" con "val"
	Parametri:
	- input		
      short    val;    Valore per "CMDDIA"

	Restituisce GS_BAD se non riesce a settare il valore
-*/  
/************************************************************/
int CMapPostGIS::reset_cmddia(short val)
{
	presbuf new_val;
	if ((new_val = acutBuildList(RTSHORT, val, 0)) == NULL)
		return GS_BAD;
	if (acedSetVar(_T("CMDDIA"), new_val) != RTNORM)
		return GS_BAD;
	return GS_GOOD;
}

/************************************************************/
/*.doc CMapPostGIS::reset_blipmode								*/
/*+
	Questa funzione setta il valore della variabile globale
	di AutoCAD "BLIPMODE" con "val"
	Parametri:
	- input		
	  short    val;    Valore per "BLIPMODE"

	Restituisce GS_BAD se non riesce a settare il valore
-*/  
/************************************************************/
int CMapPostGIS::reset_blipmode(short val)
{
	presbuf new_val;
	if ((new_val = acutBuildList(RTSHORT, val, 0)) == NULL)
		return GS_BAD;
	if (acedSetVar(_T("BLIPMODE"), new_val) != RTNORM)
		return GS_BAD;
	return GS_GOOD;
}

/************************************************************/
/*.doc CMapPostGIS::reset_highlight								*/
/*+
	Questa funzione setta il valore della variabile globale
	di AutoCAD "HIGHLIGHT" con "val"
	Parametri:
	- input		
      short    val;    Valore per "HIGHLIGHT"

	Restituisce GS_BAD se non riesce a settare il valore
-*/  
/************************************************************/
int CMapPostGIS::reset_highlight(short val)
{
	presbuf new_val;
	if ((new_val = acutBuildList(RTSHORT, val, 0)) == NULL)
		return GS_BAD;
	if (acedSetVar(_T("HIGHLIGHT"), new_val) != RTNORM)
		return GS_BAD;
	return GS_GOOD;
}

/************************************************************/
/*.doc CMapPostGIS::RegLogFile									*/
/*+
	Questa funzione gestisce il file di log GEOinit.log, 
	scrivendo la stringa in input preceduta dall'ora e 
	dalla data.
	
	Parametri:
	- input		
	  C_STRING *input		stringa da scrivere nel file
      
	Restituisce GS_BAD se non riesce a creare il file o a 
	scrivere
-*/  
/************************************************************/
int CMapPostGIS::RegLogFile(C_STRING input)
{
	int ret = GS_GOOD;

	FILE *f = NULL;
	
	C_STRING Date, Time;
	C_STRING	log_path;

	TRY
	{
		gsc_current_Date(Date);
		gsc_current_Time(Time);

		log_path = geowork_path;
		log_path += POSTGIS;
		log_path += "/";
		log_path += log_name;

		if ((f = gsc_fopen(log_path, _T("a"), ONETEST)) == NULL)
			AfxThrowUserException();

		if (fwprintf(f, _T("%s - %s - %s"), Date.get_name(), Time.get_name(), input) < 0)
		{
			if (gsc_fclose(f) == GS_BAD)
				AfxThrowUserException();
			set_GS_ERR_COD(eGSWriteFile); 
			AfxThrowUserException();
		}
		
		if (gsc_fclose(f) == GS_BAD)
			AfxThrowUserException();
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

int CMapPostGIS::RegErrFile(C_STRING input)
{
	int ret = GS_GOOD;

	FILE *f = NULL;
	
	C_STRING Date, Time;

	C_STRING	err_path;
	int		ch;
	fpos_t	pos;
	TCHAR	word[8];
	int		err = FALSE;

	TRY
	{
		err_path = geowork_path;
		err_path += POSTGIS;
		err_path += "/";
		err_path += err_name;

		gsc_current_Date(Date);
		gsc_current_Time(Time);
						
		if ((f = gsc_fopen(err_path, _T("a"), ONETEST)) == NULL)
			AfxThrowUserException();

		if (fwprintf(f, _T("%s - %s - %s<BR>"), Date.get_name(), Time.get_name(), input) < 0)
		{
			if (gsc_fclose(f) == GS_BAD)
				AfxThrowUserException();
			set_GS_ERR_COD(eGSWriteFile); 
			AfxThrowUserException();
		}
						
		if (gsc_fclose(f) == GS_BAD)
			AfxThrowUserException();
						
		if ((f = gsc_fopen(html_path, _T("r"), ONETEST)) == NULL)
			AfxThrowUserException();

		if (fseek(f, 0, SEEK_SET)!=0)
			AfxThrowUserException();

		while (!feof(f))
		{
			ch = fgetc(f);
			if (ch == ']')
				fgetpos(f, &pos);
		}

		fsetpos(f, &pos);

		while (!feof(f))
		{
			ch = fgetc(f);
			if (ch == '>')
			{
				fread(word, sizeof(TCHAR), 7, f);
				word[7] = '\0';
				if (gsc_strcmp(word, _T("errfile")) == 0) 
				{
					err = TRUE;
					break;
				}
			}
		}

		if (gsc_fclose(f) == GS_BAD)
			AfxThrowUserException();

		if (err == FALSE)
		{
			if ((f = gsc_fopen(html_path, _T("a"), ONETEST)) == NULL)
				AfxThrowUserException();
			
			// file PostGIS.html
			if (fwprintf(f, _T("; <A HREF=\"%s\">errfile</A>"), err_name) < 0)
			{
				if (gsc_fclose(f) == GS_BAD)
					AfxThrowUserException();
				set_GS_ERR_COD(eGSWriteFile); 
					AfxThrowUserException();
			}

			if (gsc_fclose(f) == GS_BAD)
				AfxThrowUserException();
		}
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}


int CMapPostGIS::Start()
{
	int ret = GS_GOOD;

	FILE *html = NULL;

	C_STRING Date, Time;

	C_STRING	msg;

	C_INT_INT_STR	*pItem;
	int c;

	long		prj;
	C_CLASS		*pClass;

	TRY
	{
		gsc_current_Date(Date);

		Date.get_name()[2] = '_';
		Date.get_name()[5] = '_';
			
		log_name = "GEOinit";
		log_name += Date.get_name();
		log_name += ".html";

		err_name = "GEOerr";
		err_name += Date.get_name();
		err_name += ".html";

		html_path = geowork_path;
		html_path += POSTGIS;
		html_path += "/PostGIS.html";

		if (setting_info.get_start() && (geoininame != ""))
		{
			if (gsc_path_exist(html_path) == GS_BAD)
			{
				if ((html = gsc_fopen(html_path, _T("a"), ONETEST)) == NULL)
					AfxThrowUserException();
				
				if (fwprintf(html, _T("<HTML>\n<HEAD>\n<TITLE>PostGIS log file</TITLE>\n</HEAD>\n<BODY>\n")) < 0)
				{
					set_GS_ERR_COD(eGSWriteFile); 
					AfxThrowUserException();
				}

				if (gsc_fclose(html) == GS_BAD)
					AfxThrowUserException();
			}		
					
			if ((html = gsc_fopen(html_path, _T("a"), ONETEST)) == NULL)
				AfxThrowUserException();

			gsc_current_Date(Date);
			gsc_current_Time(Time);
					
			if (fwprintf(html, _T("<BR>[%s] [%s]&nbsp; <A HREF=\"%s\">logfile</A>&nbsp"), Date.get_name(), Time.get_name(), log_name) < 0)
			{
				set_GS_ERR_COD(eGSWriteFile); 
				AfxThrowUserException();
			}

			if (gsc_fclose(html) == GS_BAD)
				AfxThrowUserException();

			if (RegLogFile(_T("<FONT COLOR=\"red\"> Inizio passaggio dati. </FONT><BR>")) != GS_GOOD)			// scrive nel file di log
				AfxThrowUserException();

			if ((FindSelectedClasses() == GS_BAD) && 
				(gsc_fopen(geoininame, _T("r"), ONETEST)))
			{
				RegErrFile(_T("Errore nel file contenente le classi selezionate (es. progetto non più esistente)."));					// scrive nel file di err

				if ((geoscript = gsc_fopen(scriptname.get_name(), _T("w"), ONETEST)) == NULL)
					AfxThrowUserException();
						
				if (fwprintf(geoscript, _T("_.QUIT\n_Y\n")) < 0)
				{
					set_GS_ERR_COD(eGSWriteFile);
					AfxThrowUserException();
				}

				if (gsc_fclose(geoscript) == GS_BAD)
					AfxThrowUserException();

				if (DeleteSettingIni() == GS_BAD)
					AfxThrowUserException();

				if (DeleteMapPostGISIni() == GS_BAD)
					AfxThrowUserException();

				if (ResetSystemVar() == GS_BAD)
					AfxThrowUserException();

				if (acedCommandS(RTSTR, _T("_.SCRIPT"), RTSTR, scriptname.get_name(), 0) != RTNORM)	//	script per l'apertura di un DWG
					AfxThrowUserException();	// codice errore
				
				AfxThrowUserException();
			}

			msg = "Classi selezionate per il passaggio dati PostGIS:<BR>";
			pItem = (C_INT_INT_STR*) classList.get_head();

			while (pItem) 
			{
				c=0;

				if (pItem->get_key() == 0)
				{
					prj = pItem->get_type();
					msg += "<TABLE>";
					msg += "<TR><TD WIDTH=\"170\"></TD><TD>progetto:</TD><TD>";
					msg += pItem->get_name();
					msg += "</TD><TD></TD></TR><TR><TD></TD><TD></TD><TD>classi:</TD>";
				}
		
				pItem = (C_INT_INT_STR*) classList.get_next();
				while ((pItem) && (pItem->get_key() != 0))
				{	
					if (c == 0)
					{
						if (pItem->get_type() == 0)
							msg += "<TD>";
						else
						{
							if ((pClass = gsc_find_class(prj, pItem->get_key(), 0)) == NULL)
								AfxThrowUserException();
							msg += "<TD>";
							msg += pClass->get_name();
							msg += "-";			
						}			
						msg += pItem->get_name();
						msg += "</TD></TR>";

						c++;
					}
					else
					{
						if (pItem->get_type() == 0)
							msg += "<TR><TD></TD><TD></TD><TD></TD><TD>";
						else
						{
							if ((pClass = gsc_find_class(prj, pItem->get_key(), 0)) == NULL)
								AfxThrowUserException();
							//msg += "<TR><TD></TD><TD></TD><TD></TD><TD></TD><TD>";
							msg += "<TR><TD></TD><TD></TD><TD></TD><TD>";
							msg += pClass->get_name();
							msg += "-";			
						}			
						msg += pItem->get_name();
						msg += "</TD></TR>";
					}
					pItem = (C_INT_INT_STR*) classList.get_next();
				}

				msg += "</TABLE>";
			}

			if (RegLogFile(msg) != GS_GOOD)			// scrive nel file di log
				AfxThrowUserException();

			setting_info.set_write_start(FALSE);
		}
	}
	CATCH( CUserException, e )
	{
		DeleteMapPostGISIni();
		ResetSystemVar();
		if (html)
			gsc_fclose(html);
		if (geoscript)
			gsc_fclose(geoscript);
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

/************************************************************/
/*.doc CMapPostGIS::CoordSettings								*/
/*+
	Questa funzione ricava i valori delle variabili della 
	classe coord, ovvero
	  coord_input		sistema coordinate del DWG
	  cat_coord_input	categoria sistema coordinate del DWG
	  coord_output		sistema coordinate per l'sql
	  cat_coord_output	categoria sistema coordinate per l'sql
	dal file coord_sql.ini, se esiste, altrimenti setta dei 
	valori di default,
	
	ed il valore della variabile
	  sql_path		path per i files sql
	dalla variabile di sistema POSTGIS
      
	Restituisce GS_BAD se non riesce a ottenere i valori 
-*/  
/************************************************************/
int CMapPostGIS::CoordSettings()
{
	int ret = GS_GOOD;

	CCoord temp;

	TRY
	{
		if (coord_info.openFile(geowork_path) == NULL)
		{
	//		temp.set_cat_coord_input(_T("UTM-ITALY");
	//		temp.set_coord_input(_T("UTM-INTL");
	//		temp.set_cat_coord_output(_T("Lat Longs");
	//		temp.set_coord_output(_T("LL-ERP50");    
			temp.set_cat_coord_input(_T(""));
			temp.set_coord_input(_T(""));
			temp.set_cat_coord_output(_T(""));
			temp.set_coord_output(_T(""));

			if (coord_info.writeFile(&temp) == GS_BAD)	// crea, setta a valori default e chiude il file map_postgis
				AfxThrowUserException();
		}
		else
		{
			if (coord_info.closeFile() == GS_BAD)	// chiude il file coord_sql
				AfxThrowUserException();

			if (coord_info.readFile() == GS_BAD)	// legge il file coord_sql
				AfxThrowUserException();
		}
						
		sql_path = postGis_path;
		sql_path += "/sql";

		// eventuale creazione
		if (gsc_mkdir(sql_path) == GS_BAD)
			AfxThrowUserException();
	}
	CATCH( CUserException, e )
	{
		ResetSystemVar();		
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

int CMapPostGIS::SettingIniControl()
{
	int ret = GS_GOOD;

	TRY
	{
		if (setting_info.openFile(geowork_path) == NULL)	//	il file setting esiste
		{
			if (geoininame == "")
				AfxThrowUserException();

			if (setting_info.writeFile() == GS_BAD)	// crea, setta a valori default e chiude il file setting
				AfxThrowUserException();
		}
		else
		{
			if (setting_info.closeFile() == GS_BAD)	// chiude il file setting
				AfxThrowUserException();

			if (setting_info.readFile() == GS_BAD)	// legge il file setting
				AfxThrowUserException();
		}
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

int CMapPostGIS::MapPostGISIniControl()
{
	int ret = GS_GOOD;

	TRY
	{
		if (map_info.openFile(geowork_path) == NULL)	//	il file map_postgis non esiste ancora, inizio passaggio dati
		{
			if ((geoininame == "") && setting_info.get_start())
				AfxThrowUserException();

			if (map_info.set_write_ini_path(geoininame) == GS_BAD)
				AfxThrowUserException();
			if (map_info.set_write_server(server) == GS_BAD)
				AfxThrowUserException();
			if (map_info.set_write_user(user) == GS_BAD)
				AfxThrowUserException();
			if (map_info.set_write_font(font) == GS_BAD)
				AfxThrowUserException();
			if (map_info.set_write_schema(schema) == GS_BAD)
				AfxThrowUserException();
			if (map_info.set_write_database(database) == GS_BAD)
				AfxThrowUserException();
			if (map_info.set_write_srid(srid) == GS_BAD)
				AfxThrowUserException();
			if (map_info.set_write_inf(inf) == GS_BAD)
				AfxThrowUserException();
			if (map_info.set_write_isOracle(isOracle) == GS_BAD)
				AfxThrowUserException();

			if (map_info.writeFile() == GS_BAD)	// crea, setta a valori default e chiude il file map_postgis
				AfxThrowUserException();
		}
		else
		{
			if (map_info.closeFile() == GS_BAD)	// chiude il file map_postgis
				AfxThrowUserException();

			if (map_info.readFile() == GS_BAD)	// legge il file map_postgis
				AfxThrowUserException();

			geoininame = map_info.get_ini_path();
			server = map_info.get_server();
			user = map_info.get_user();
			font = map_info.get_font();
			schema = map_info.get_schema();
			database = map_info.get_database();
			srid = map_info.get_srid();
			inf = map_info.get_inf();
			isOracle = map_info.get_isOracle();
		}
	}
	CATCH( CUserException, e )
	{
		if (geoininame != "")
			ResetSystemVar();
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

int CMapPostGIS::ProjectControl()
{
	int ret = GS_GOOD;

	CInfo map_temp;

	FILE *html = NULL;

	C_STRING Date, Time;

	TRY
	{
		if (map_info.get_prj_index() == 0)	// lettura indice progetto
		{	      
			if (FindFirstPrjIndex(&map_temp) == GS_BAD)
			{
				if (setting_info.get_finish() != TRUE) 
				{
					if ((html = gsc_fopen(html_path, _T("a"), ONETEST)) == NULL)
						AfxThrowUserException();

					gsc_current_Date(Date);
					gsc_current_Time(Time);
					
					if (fwprintf(html, _T("<BR>[%s] [%s]&nbsp; <A HREF=\"%s\">logfile</A>&nbsp"), Date.get_name(), Time.get_name(), log_name) < 0)
					{
						set_GS_ERR_COD(eGSWriteFile); 
						AfxThrowUserException();
					}

					if (gsc_fclose(html) == GS_BAD)
						AfxThrowUserException();
					
					if (RegLogFile("<FONT COLOR=\"blue\">Nessuna classe selezionata per il passaggio dati. Fine passaggio dati.</FONT><BR>") != GS_GOOD)
					{
						set_GS_ERR_COD(eGSWriteFile); 
						AfxThrowUserException();
					}
				}

				if ((geoscript = gsc_fopen(scriptname.get_name(), _T("w"), ONETEST)) == NULL)
					AfxThrowUserException();
								
				if (fwprintf(geoscript, _T("_.QUIT\n_Y\n")) < 0)
				{
					set_GS_ERR_COD(eGSWriteFile);
					AfxThrowUserException();	//	ESCE
				}

				if (gsc_fclose(geoscript) == GS_BAD)
					AfxThrowUserException();
								
				if (DeleteSettingIni() == GS_BAD)
					AfxThrowUserException();

				if (DeleteMapPostGISIni() == GS_BAD)
					AfxThrowUserException();

				if (ResetSystemVar() == GS_BAD)
					AfxThrowUserException();
								
				if (acedCommandS(RTSTR, _T("_.SCRIPT"), RTSTR, scriptname.get_name(), 0) != RTNORM)	//	script per l'apertura di un DWG
					AfxThrowUserException();

				return GS_BAD;	//	TERMINA se i progetti e le classi per il passaggio sono finiti 
			}
			else
			{
				if (map_info.writeFile(&map_temp) == GS_BAD)
					AfxThrowUserException();
			}
		}
	}
	CATCH( CUserException, e )
	{
		ResetSystemVar();
		if (html)
			gsc_fclose(html);
		if (geoscript)
			gsc_fclose(geoscript);
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

int CMapPostGIS::ProjectCreate()
{
	int ret = GS_GOOD;

	resbuf	*rb, *temp;	// lista delle classi

	int		cls, sub, level, category, type;
	C_STRING	name, tb_type, str1, str2, str3;

	C_STRING	alias;
	C_STRING		sql_file, alias_path, dest_file, sql_dest_file, total_file, log_file, ini, sqlfont_file;
	FILE		*sqlFile, *destFile, *totalFile, *sqlFontFile;

	POSITION	pos, pos1, pos2, pos7, pos8;
	int			pos3, pos4, pos5, pos6;

	bool			found;
	
	Geosim_Class	*geosim_class;
	
	Geoweb_Structure	geoweb_structure;
	Geoweb_Layer		*geoweb_layer = NULL;
	Geoweb_Layergroup	*geoweb_layergroup = NULL;
	Geoweb_Theme		*geoweb_theme = NULL;
	Geoweb_Qt			*geoweb_qt = NULL;
	Geoweb_Qtfield		*geoweb_qtfield = NULL;
	Geoweb_Class		*geoweb_class = NULL;

	C_DWG_LIST	dwg_list;
	C_DWG		*pDwg;
	double		xMin, yMin, xMax, yMax;
	double		_xMin=0, _yMin=0, _xMax=0, _yMax=0;
	long		j, x;

    AcDbSymbolTable 		*pSymbolTable   = NULL; 
	AcDbSymbolTableIterator	*pSymbolTableIt = NULL;
	AcDbObjectId    objectId; 
	AcDbLayerTableRecord	*pLTRec;
	AcDbBlockTableRecord    *pBTRec;
	const ACHAR *pName;
	C_STRING		layer, layer1, field1, original_class_name;
	AcCmColor	color;
	Adesk::UInt16 color1;
	unsigned long RGB;
	int	red;
	int	green;
	int	blue;

	Geosim_Layer	*geosim_layer;

	C_CLASS		*pClass;
	long		last;

	C_STRING		block, block1;
	int			cont;

	C_STRING		path;
	C_STRING		db;

	int			orderValue;

	C_INT_INT_STR	*pItem;

	TRY
	{
		if (map_info.get_db_prj() == 0)			//	controllo se la lista è già stata creata
		{
			if (FindClassList(&rb, map_info.get_prj_index()) == GS_BAD)		// non esistono progetti
			{	
				set_GS_ERR_COD(eGSListError); 
				AfxThrowUserException();
			}

			// (<code> <class_name> <level> <category> <type> <sub_list>)
			temp = rb;
			while (temp != NULL)
			{
				sub = 0;
				
				temp = temp->rbnext;
				if (temp->rbnext == NULL)		// fine lista o lista "vuota"
					AfxThrowUserException();

				if (temp->restype == RTSHORT)
				{
					cls = temp->resval.rint;
					temp = temp->rbnext;	
					if (temp->restype == RTSTR)
					{
						name = temp->resval.rstring;				
						temp = temp->rbnext;
						if (temp->restype == RTSHORT)
						{
							level = temp->resval.rint;
							temp = temp->rbnext;
							if (temp->restype == RTSHORT)
							{
								category = temp->resval.rint;

								temp = temp->rbnext;
								if (temp->restype == RTSHORT)
								{
									type = temp->resval.rint;
									tb_type = FindTopobaseType(category, type);

									last = 0;								
									if (category != CAT_SPAGHETTI)
									{
										pClass = gsc_find_class(map_info.get_prj_index(), cls, 0);
										last = pClass->ptr_info()->OldLastId;
									}
									geosim_class = new Geosim_Class(cls, name, name, tb_type, last, 0, 0);
									geoweb_structure.geosim_class_list.AddTail(geosim_class);

									temp = temp->rbnext;
									// (<sub_code> <sub_class_name> <level> <category> <type>)
									if (temp->restype == RTLB)
									{
										temp = temp->rbnext;
										while (temp->restype == RTLB)
										{
											temp = temp->rbnext;
											sub = temp->resval.rint;
											temp = temp->rbnext;	
											if (temp->restype == RTSTR)
											{
												name = temp->resval.rstring;				
												temp = temp->rbnext;
												if (temp->restype == RTSHORT)
												{
													level = temp->resval.rint;
													temp = temp->rbnext;
													if (temp->restype == RTSHORT)
													{
														category = temp->resval.rint;
														temp = temp->rbnext;
														if (temp->restype == RTSHORT)
														{
															type = temp->resval.rint;
															tb_type = FindTopobaseType(category, type);
														
															pClass = gsc_find_class(map_info.get_prj_index(), cls, sub);
															last = pClass->ptr_info()->OldLastId;

															geosim_class = new Geosim_Class(-1, name, name, tb_type, last, 0, cls);
															geoweb_structure.geosim_class_list.AddTail(geosim_class);

															temp = temp->rbnext;
														}
													}
												}
											}			

											temp = temp->rbnext;
										}
										temp = temp->rbnext->rbnext;
									}
									else
									{
										temp = temp->rbnext->rbnext;

										if (temp != NULL)
											if (temp->rbnext == NULL)
												temp = NULL;
									}
								}
							}
						}
					}
				}
			}

			// rielabora l'elenco delle classi
			geoweb_structure.Update(map_info.get_prj_index(), map_info.get_prj_name().tolower(), map_info.get_prj_path(), map_info.get_database(), font, classList, map_info.get_srid());

			if (inf)
			{
				pos = geoweb_structure.geosim_class_list.GetHeadPosition();
				while (pos)
				{
					geosim_class = (Geosim_Class*) geoweb_structure.geosim_class_list.GetNext(pos);
					
					if ((geosim_class->f_class_type != "X") && (geosim_class->last > 0))
					{
						if (dwg_list.load(map_info.get_prj_index(), geosim_class->f_class_id) == GS_GOOD)
						{
							pDwg = (C_DWG*) dwg_list.get_head();

							if (pDwg)
							{
								if (pDwg->regen_extents_from_dwg())
								{
									pDwg->get_extents(&xMin, &yMin, &xMax, &yMax);
									_xMin = xMin;
									_yMin = yMin;
									_xMax = xMax;
									_yMax = yMax;
								}
							}
							break;
						}
					}
					else
					{
						_xMin = 0;
						_yMin = 0;
						_xMax = 0;
						_yMax = 0;
					}
				}
			}
			
			pos = geoweb_structure.geosim_class_list.GetHeadPosition();
			while (pos)
			{
				geosim_class = (Geosim_Class*) geoweb_structure.geosim_class_list.GetNext(pos);
				if (geosim_class->last > 0)
				{
					if (dwg_list.load(map_info.get_prj_index(), geosim_class->f_class_id) == GS_GOOD)
					{
						pDwg = (C_DWG*) dwg_list.get_head();
						while (pDwg)
						{
							if (inf)
							{
								if (pDwg->regen_extents_from_dwg())
								{
									pDwg->get_extents(&xMin, &yMin, &xMax, &yMax);				
									if (xMin < _xMin)
										_xMin = xMin;
									if (yMin < _yMin)
										_yMin = yMin;
									if (xMax > _xMax)
										_xMax = xMax;
									if (yMax > _yMax)
										_yMax = yMax;
								}
							}

							found = false;
							pItem = (C_INT_INT_STR*) classList.get_head();
							while (pItem) 
							{
								if (geosim_class->f_class_name == pItem->get_name()) 
								{
									found = true;
									break;
								}
								pItem = (C_INT_INT_STR*) classList.get_next();
							}

							if (found)
							{
								// Lettura del dwg
								AcDbDatabase	extDatabase(Adesk::kFalse);
								if (Acad::eOk == extDatabase.readDwgFile(pDwg->get_name(), _SH_DENYNO))
								{
									// Lettura dei layers nel dwg
									if (Acad::eOk == extDatabase.getLayerTable(pSymbolTable, AcDb::kForRead))
									{
										if (pSymbolTable->newIterator(pSymbolTableIt, Adesk::kTrue, Adesk::kTrue) != Acad::eOk)
										{ 
											pSymbolTable->close();
											set_GS_ERR_COD(eGSWriteFile); 
											AfxThrowUserException();
										}
										while (!pSymbolTableIt->done())
										{
											if (pSymbolTableIt->getRecordId(objectId) != Acad::eOk)
											{ 
												delete pSymbolTableIt; 
												pSymbolTable->close(); 
												set_GS_ERR_COD(eGSInvGraphObjct); 
												AfxThrowUserException();
											}
											if (acdbOpenAcDbObject((AcDbObject *&) pLTRec, objectId, AcDb::kForRead) != Acad::eOk)
											{ 
												delete pSymbolTableIt; 
												pSymbolTable->close(); 
												set_GS_ERR_COD(eGSInvGraphObjct); 
												AfxThrowUserException();
											}

											if (pLTRec->getName(pName) != Acad::eOk) 
											{ 
												delete pSymbolTableIt; 
												pSymbolTable->close(); 
												pLTRec->close();
												AfxThrowUserException();
											}
											layer = pName;
											color = pLTRec->color();
											if (color.isByColor())
											{
												red = color.red();
												green = color.green();
												blue = color.blue();				
											}
											else
											{
												color1 = color.colorIndex();
												RGB = acedGetRGB(color1);
												red = GetRValue(RGB);
												green = GetGValue(RGB);
												blue = GetBValue(RGB);
											}
											
											found = false;
											pos8 = geosim_class->geosim_layer_list.GetHeadPosition();
											while (pos8)
											{
												geosim_layer = (Geosim_Layer*) geosim_class->geosim_layer_list.GetNext(pos8);		
												if (geosim_layer->layer_name == layer)
													found = true;
											}
											if (!found)
											{
												geosim_layer = new Geosim_Layer(layer, red, green, blue);
												geosim_class->geosim_layer_list.AddTail(geosim_layer);
											}

											pLTRec->close();
											pSymbolTableIt->step();
										}
										delete pSymbolTableIt;
										pSymbolTable->close();
									}
								}
							}

							pDwg = (C_DWG*) dwg_list.get_next();
						}
					}
				}
			}

         // files utente opzionali (tutti i files DWG che iniziano per "USRTHM")
         C_STR_LIST FileNameList;

			AcDbDatabase	extDatabase(Adesk::kFalse);
			path = geosim_path;
			path += '\\';
			path += GEOTHMDIR;
			path += '\\';
         path += USRTHMS; 
         if (gsc_adir(path.get_name(), &FileNameList, NULL, NULL, NULL, true) > 0)
         {
            C_STR *pFileName = (C_STR *) FileNameList.get_head();

            while (pFileName)
            {
               path = geosim_path;
               path += _T('\\');
               path += GEOTHMDIR;
               path += _T('\\');
               path += pFileName->get_name();

               if (Acad::eOk == extDatabase.readDwgFile(path.get_name(), _SH_DENYNO))
               {
                  // Lettura dei blocchi nel dwg
                  if (Acad::eOk == extDatabase.getBlockTable(pSymbolTable, AcDb::kForRead))
                  {
                     if (pSymbolTable->newIterator(pSymbolTableIt, Adesk::kTrue, Adesk::kTrue) != Acad::eOk)
                     { 
	                     pSymbolTable->close();
	                     set_GS_ERR_COD(eGSWriteFile); 
	                     AfxThrowUserException();
                     }
                     while (!pSymbolTableIt->done())
                     {
	                     if (pSymbolTableIt->getRecordId(objectId) != Acad::eOk)
	                     { 
		                     delete pSymbolTableIt; 
		                     pSymbolTable->close(); 
		                     set_GS_ERR_COD(eGSInvGraphObjct); 
		                     AfxThrowUserException();
	                     }

	                     if (acdbOpenAcDbObject((AcDbObject *&) pBTRec, objectId, AcDb::kForRead) != Acad::eOk)
	                     { 
		                     delete pSymbolTableIt; 
		                     pSymbolTable->close(); 
		                     set_GS_ERR_COD(eGSInvGraphObjct); 
		                     AfxThrowUserException();
	                     }

	                     if (pBTRec->getName(pName) != Acad::eOk) 
	                     { 
		                     delete pSymbolTableIt; 
		                     pSymbolTable->close(); 
		                     pLTRec->close();
		                     AfxThrowUserException();
	                     }

	                     pos3 = pos4 = -1;
	                     block = pName;
	                     block1 = block.toupper();
	                     pos3 = block1.find(_T("_SPACE"));
	                     pos4 = block1.find(_T("$T"));
	                     if ((pos3 == -1) && (pos4 == -1))
		                     blockList.AddTail(block.get_name());

	                     pBTRec->close();
	                     pSymbolTableIt->step();
                     }
                     delete pSymbolTableIt;
                     pSymbolTable->close();
                  }
               }

               pFileName = (C_STR *) FileNameList.get_next();
            }
         }
			pos6 = geoininame.reverseFind(_T('/'));
			ini = geoininame.right(geoininame.len() - pos6 -1);
			ini.strtran(_T(".ini"), _T(""));

			sql_file = map_info.get_prj_path();
			sql_file += "/geoweb_structure_";
			sql_file += ini;
			sql_file += ".sql";

			if ((sqlFile = gsc_fopen(sql_file, _T("w"), ONETEST)) == NULL)
				AfxThrowUserException();				

			sqlfont_file = map_info.get_prj_path();
			sqlfont_file += "/geoweb_font.sql";

			if ((sqlFontFile = gsc_fopen(sqlfont_file, _T("w"), ONETEST)) == NULL)
				AfxThrowUserException();

			if (geoweb_structure.geoweb_project)
			{
				if (fwprintf(sqlFile, _T("%s\n"), _T("BEGIN;")) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				str1 = "INSERT INTO gisclient_21.project";
				str1 += " (project_name,project_title,project_extent,project_srid) VALUES (";
				str1 += "'";
				str1 += geoweb_structure.geoweb_project->project_name;
				str1 += "', '";
				str1 += geoweb_structure.geoweb_project->project_name;
				str1 += "', '";
				j = (long) (_xMin - (_xMax - _xMin)/100);
				str1 += j;
				str1 += " ";
				j = (long) (_yMin - (_yMax - _yMin)/100);
				str1 += j;
				str1 += " ";
				j = (long) (_xMax + (_xMax - _xMin)/100);
				str1 += j;
				str1 += " ";
				j = (long) (_yMax + (_yMax - _yMin)/100);
				str1 += j;
				str1 += "', ";
				str1 += geoweb_structure.geoweb_project->project_srid;
				str1 += ");";

				if (fwprintf(sqlFile, _T("%s\n"), str1) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				if (fwprintf(sqlFile, _T("%s\n"), _T("COMMIT;")) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				if (fwprintf(sqlFontFile, _T("%s\n"), _T("BEGIN;")) < 0)
				{
					if (gsc_fclose(sqlFontFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				str1 = "INSERT INTO gisclient_21.font";
				str1 += " (font_name,file_name) VALUES ('";
				if (font == "")
					str1 += geoweb_structure.geoweb_project->project_name;
				else
					str1 += font;
				str1 += "', '";
				if (font == "")
					str1 += geoweb_structure.geoweb_project->project_name;
				else
					str1 += font;
				str1 += ".ttf');";

				if (fwprintf(sqlFontFile, _T("%s\n"), str1) < 0)
				{
					if (gsc_fclose(sqlFontFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				if (fwprintf(sqlFontFile, _T("%s\n"), _T("COMMIT;")) < 0)
				{
					if (gsc_fclose(sqlFontFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				if (blockList.GetCount() > 0)
				{
					if (fwprintf(sqlFontFile, _T("%s\n"), _T("BEGIN;")) < 0)
					{
						if (gsc_fclose(sqlFontFile) == GS_BAD)
							AfxThrowUserException();
						AfxThrowUserException();
					}

					cont = 33;
					POSITION y = blockList.GetHeadPosition();
					while (y) 
					{
						if (cont == 127)
							cont = 160;
						if (cont == 256)
							cont = 288;

						block = blockList.GetAt(y);

						str1 = "INSERT INTO gisclient_21.symbol_ttf (symbol_ttf_name,ascii_code,font_name,position,symbolcategory_id) VALUES ('";
						str1 += block;
						str1 += "', ";
						str1 += cont;
						str1 += ", '";
						if (font == "")
							str1 += geoweb_structure.geoweb_project->project_name;
						else
							str1 += font;
						str1 += "', 'CC', 1);";

						if (fwprintf(sqlFontFile, _T("%s\n"), str1) < 0)
						{
							if (gsc_fclose(sqlFontFile) == GS_BAD)
								AfxThrowUserException();
							AfxThrowUserException();
						}

						cont++;
						blockList.GetNext(y);
					}

					if (fwprintf(sqlFontFile, _T("%s\n"), _T("COMMIT;")) < 0)
					{
						if (gsc_fclose(sqlFontFile) == GS_BAD)
							AfxThrowUserException();
						AfxThrowUserException();
					}
				}
			}

			if (gsc_fclose(sqlFontFile) == GS_BAD)
				AfxThrowUserException();

			if (geoweb_structure.geoweb_catalog)
			{
				if (fwprintf(sqlFile, _T("%s\n"), _T("BEGIN;")) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				str1 = "INSERT INTO gisclient_21.catalog";
				str1 += " (catalog_id,project_name,catalog_name,connection_type,catalog_path) VALUES (";
				str1 += "gisclient_21.new_pkey('catalog', 'catalog_id')";
				str1 += ", '";
				str1 += geoweb_structure.geoweb_catalog->project_name;
				str1 += "', '";
				str1 += geoweb_structure.geoweb_catalog->name;
				str1 += "', (SELECT conntype_id FROM gisclient_21.e_conntype WHERE conntype_name = 'Postgis'), '";
				str1 += geoweb_structure.geoweb_catalog->path;
				str1 += "');";

				if (fwprintf(sqlFile, _T("%s\n"), str1) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				if (fwprintf(sqlFile, _T("%s\n"), _T("COMMIT;")) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
			}

			// Theme
			if (geoweb_structure.geoweb_theme_list.GetCount() > 0)
			{
				if (fwprintf(sqlFile, _T("%s\n"), _T("BEGIN;")) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				pos = geoweb_structure.geoweb_theme_list.GetHeadPosition();
				while (pos)
				{
					geoweb_theme = (Geoweb_Theme*) geoweb_structure.geoweb_theme_list.GetNext(pos);			

					str1 = "INSERT INTO gisclient_21.theme";
					str1 += " (theme_id,project_name,theme_name,theme_title) VALUES (";
					str1 += "gisclient_21.new_pkey('theme', 'theme_id')";
					str1 += ", '";
					str1 += geoweb_theme->project_name;
					str1 += "', '";
					str3 = geoweb_theme->theme_name;
					str3 = str3.toupper();
					str1 += str3;
					str1 += "', '";
					str1 += geoweb_theme->theme_title;
					str1 += "');";

					if (fwprintf(sqlFile, _T("%s\n"), str1) < 0)
					{
						if (gsc_fclose(sqlFile) == GS_BAD)
							AfxThrowUserException();
						AfxThrowUserException();
					}
				}

				if (fwprintf(sqlFile, _T("%s\n"), _T("COMMIT;")) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
			}

			// Layergroup
			if (geoweb_structure.geoweb_layergroup_list.GetCount() > 0)
			{
				if (fwprintf(sqlFile, _T("%s\n"), _T("BEGIN;")) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				pos = geoweb_structure.geoweb_layergroup_list.GetHeadPosition();
				while (pos)
				{
					geoweb_layergroup = (Geoweb_Layergroup*) geoweb_structure.geoweb_layergroup_list.GetNext(pos);			

					str1 = "INSERT INTO gisclient_21.layergroup";
					str1 += " (layergroup_id,theme_id,layergroup_name,layergroup_title) VALUES (";
					str1 += "gisclient_21.new_pkey('layergroup', 'layergroup_id')"; 
					str1 += ",(SELECT theme_id FROM gisclient_21.theme WHERE theme_name = '";
					str3 = geoweb_layergroup->theme_name;
					str3 = str3.toupper();
					str1 += str3;
					str1 += "' AND project_name = '";
					str1 += geoweb_layergroup->project_name;												
					str1 += "'),'";
					str1 += geoweb_layergroup->layergroup_name;
					str1 += "', ";
					if (geoweb_layergroup->layergroup_title != "")
					{  
						str1 += "'";
						str1 += geoweb_layergroup->layergroup_title;
						str1 += "'";
					}
					else
						str1 += "NULL";
					str1 += ");";

					if (fwprintf(sqlFile, _T("%s\n"), str1) < 0)
					{
						if (gsc_fclose(sqlFile) == GS_BAD)
							AfxThrowUserException();
						AfxThrowUserException();
					}
				}

				if (fwprintf(sqlFile, _T("%s\n"), _T("COMMIT;")) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
			}

			// Layer
			if (geoweb_structure.geoweb_layer_list.GetCount() > 0)
			{
				if (fwprintf(sqlFile, _T("%s\n"), _T("BEGIN;")) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				pos = geoweb_structure.geoweb_layer_list.GetHeadPosition();
				while (pos)
				{
					geoweb_layer = (Geoweb_Layer*) geoweb_structure.geoweb_layer_list.GetNext(pos);			

					str1 = "INSERT INTO gisclient_21.layer";
					str1 += " (layer_id,layergroup_id,layer_name,layertype_id,catalog_id,data,data_geom,";
					//str1 += "data_unique,data_srid,data_filter,labelitem,symbolscale,layer_order,layer_def) VALUES (";
					str1 += "data_unique,data_srid,data_filter,labelitem,layer_order,layer_def) VALUES (";
					str1 += "gisclient_21.new_pkey('layer', 'layer_id')";
					str1 += ",(SELECT layergroup_id FROM gisclient_21.layergroup WHERE layergroup_name = '";
					str1 += geoweb_layer->layergroup_name;
					str1 += "' AND theme_id IN (SELECT theme_id FROM gisclient_21.theme WHERE project_name = '";
					str1 += geoweb_layer->project_name;	
					str1 +=	"')),'";
					str1 += correctString3(geoweb_layer->layer_name);
					str1 += "', ";
					str1 += geoweb_layer->layertype_id;
					str1 += ",(SELECT catalog_id FROM gisclient_21.catalog WHERE project_name = '";
					str1 += geoweb_layer->project_name;
					str1 += "' AND catalog_name = '";
					str1 += geoweb_layer->catalog_name;
					str1 += "'), '";
					str1 += correctString3(correctString4(geoweb_layer->data));
					str1 += "', '";
					str1 += geoweb_layer->data_geom;
					str1 += "', '";
					str1 += geoweb_layer->data_unique;
                    str1 += "', ";
					str1 += geoweb_layer->data_srid;
                    str1 += ", ";
					if (geoweb_layer->data_filter != "")
					{  
						str1 += "'";
						str1 += geoweb_layer->data_filter;
						str1 += "'";
					}
					else
						str1 += "NULL";
					str1 += ", ";
					if (geoweb_layer->labelitem != "")
					{  
						str1 += "'";
						str1 += geoweb_layer->labelitem;
						str1 += "'";
					}
					else
						str1 += "NULL";
					str1 += ", ";
					//str1 += geoweb_layer->symbolscale;
					//str1 += ", ";
					str1 += geoweb_layer->layer_order;
					str1 += ", ";
					if (geoweb_layer->layer_def != "")
					{  
						str1 += "'";
						str1 += geoweb_layer->layer_def;
						str1 += "'";
					}
					else
						str1 += "NULL";
					str1 += ");";

					if (fwprintf(sqlFile, _T("%s\n"), str1) < 0)
					{
						if (gsc_fclose(sqlFile) == GS_BAD)
							AfxThrowUserException();
						AfxThrowUserException();
					}
				}

				if (fwprintf(sqlFile, _T("%s\n"), _T("COMMIT;")) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
			}

			// Qt
			if (geoweb_structure.geoweb_qt_list.GetCount() > 0)
			{
				if (fwprintf(sqlFile, _T("%s\n"), _T("BEGIN;")) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				pos = geoweb_structure.geoweb_qt_list.GetHeadPosition();
				while (pos)
				{
					geoweb_qt = (Geoweb_Qt*) geoweb_structure.geoweb_qt_list.GetNext(pos);			

					str1 = "INSERT INTO gisclient_21.qt";
					str1 += " (qt_id,theme_id,layer_id,qt_name) VALUES (";
					str1 += "gisclient_21.new_pkey('qt', 'qt_id')";
					str1 += ", ";
					str1 += "(SELECT theme_id FROM gisclient_21.theme WHERE theme_name = '";
					str3 = geoweb_qt->theme_name;
					str3 = str3.toupper();
					str1 += str3;
					str1 += "' AND project_name = '";
					str1 += geoweb_structure.geoweb_catalog->project_name;
					str1 += "'),";
					str1 += "(SELECT layer_id FROM gisclient_21.layer WHERE layer_name = '";
					str1 += geoweb_qt->layer_name;
					str1 += "' AND layergroup_id = (SELECT layergroup_id FROM gisclient_21.layergroup WHERE layergroup_name = '";
					str1 += geoweb_qt->layergroup_name;
					str1 += "' AND theme_id = (SELECT theme_id FROM gisclient_21.theme WHERE theme_name = '";
					str3 = geoweb_qt->theme_name;
					str3 = str3.toupper();
					str1 += str3;
					str1 += "' AND project_name = '";
					str1 += geoweb_structure.geoweb_catalog->project_name;
					str1 += "'))),'";
					str1 += geoweb_qt->qt_name;
					str1 += "');";

					if (fwprintf(sqlFile, _T("%s\n"), str1) < 0)
					{
						if (gsc_fclose(sqlFile) == GS_BAD)
							AfxThrowUserException();
						AfxThrowUserException();
					}
				}

				if (fwprintf(sqlFile, _T("%s\n"), _T("COMMIT;")) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				//if (fwprintf(sqlFile, _T("%s\n"), _T("BEGIN;")) < 0)
				//{
				//	if (gsc_fclose(sqlFile) == GS_BAD)
				//		AfxThrowUserException();
				//	AfxThrowUserException();
				//}

				//pos = geoweb_structure.geoweb_qt_list.GetHeadPosition();
				//while (pos)
				//{
				//	geoweb_qt = (Geoweb_Qt*) geoweb_structure.geoweb_qt_list.GetNext(pos);			

				//	if (geoweb_qt->geoweb_qtrelation != NULL)
				//	{
				//		str1 = "INSERT INTO gisclient_21.qtrelation";
				//		str1 += " (qtrelation_id,qtrelation_name,qt_id,data_field_1,catalog_id,table_name,table_field_1,qtrelationtype_id) VALUES (";
				//		str1 += "gisclient_21.new_pkey('qtrelation', 'qtrelation_id')";
				//		str1 += ", '";
				//		str1 += correctString4(geoweb_qt->geoweb_qtrelation->table_name);
				//		str1 += "',(SELECT qt_id FROM gisclient_21.qt WHERE qt_name = '";
				//		str1 += geoweb_qt->geoweb_qtrelation->table_name;
				//		str1 += "' AND layer_id = ";
				//		str1 += "(SELECT layer_id FROM gisclient_21.layer WHERE layer_name = '";
				//		str1 += geoweb_qt->layer_name;
				//		str1 += "' AND layergroup_id = (SELECT layergroup_id FROM gisclient_21.layergroup WHERE layergroup_name = '";
				//		str1 += geoweb_qt->layergroup_name;
				//		str1 += "' AND theme_id = (SELECT theme_id FROM gisclient_21.theme WHERE theme_name = '";
				//		str3 = geoweb_qt->theme_name;
				//		str3 = str3.toupper();
				//		str1 += str3;
				//		str1 += "' AND project_name = '";
				//		str1 += geoweb_structure.geoweb_catalog->project_name;
				//		str1 += "')))),'";
				//		str1 += geoweb_qt->geoweb_qtrelation->data_field;
				//		str1 += "',(SELECT catalog_id FROM gisclient_21.catalog WHERE catalog_name = '";
				//		str1 += geoweb_qt->geoweb_qtrelation->catalog_name;
				//		str1 += "'), '";
				//		str1 += correctString4(geoweb_qt->geoweb_qtrelation->table_name);
				//		str1 += "', '";
				//		str1 += geoweb_qt->geoweb_qtrelation->table_field;
				//		str1 += "', '";
				//		str1 += geoweb_qt->geoweb_qtrelation->qtrelationtype_id;
				//		str1 += "');";

				//		if (fwprintf(sqlFile, _T("%s\n"), str1) < 0)
				//		{
				//			if (gsc_fclose(sqlFile) == GS_BAD)
				//				AfxThrowUserException();
				//			AfxThrowUserException();
				//		}
				//	}
				//}

				//if (fwprintf(sqlFile, _T("%s\n"), _T("COMMIT;")) < 0)
				//{
				//	if (gsc_fclose(sqlFile) == GS_BAD)
				//		AfxThrowUserException();
				//	AfxThrowUserException();
				//}

				if (fwprintf(sqlFile, _T("%s\n"), _T("BEGIN;")) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				pos = geoweb_structure.geoweb_qt_list.GetHeadPosition();
				while (pos)
				{
					geoweb_qt = (Geoweb_Qt*) geoweb_structure.geoweb_qt_list.GetNext(pos);			

					if (geoweb_qt->geoweb_qtfield_list.GetCount() > 0)
					{
						pos1 = geoweb_qt->geoweb_qtfield_list.GetHeadPosition();
						orderValue = 0;
						while (pos1)
						{
							geoweb_qtfield = (Geoweb_Qtfield*) geoweb_qt->geoweb_qtfield_list.GetNext(pos1);			

							str1 = "INSERT INTO gisclient_21.qtfield";
							str1 += " (qtfield_id,qt_id,qtrelation_id,qtfield_name,field_header,qtfield_order,searchtype_id,resultype_id) VALUES (";
							str1 += "gisclient_21.new_pkey('qtfield', 'qtfield_id')";
							str1 += ",(SELECT qt_id FROM gisclient_21.qt WHERE qt_name = '";
							str1 += geoweb_qt->qt_name;
							str1 += "' AND layer_id = ";
							str1 += "(SELECT layer_id FROM gisclient_21.layer WHERE layer_name = '";
							str1 += geoweb_qt->layer_name;
							str1 += "' AND layergroup_id = (SELECT layergroup_id FROM gisclient_21.layergroup WHERE layergroup_name = '";
							str1 += geoweb_qt->layergroup_name;
							str1 += "' AND theme_id = (SELECT theme_id FROM gisclient_21.theme WHERE theme_name = '";
							str3 = geoweb_qt->theme_name;
							str3 = str3.toupper();
							str1 += str3;
							str1 += "' AND project_name = '";
							str1 += geoweb_structure.geoweb_catalog->project_name;
							str1 += "')))),";
							//if (geoweb_qt->geoweb_qtrelation)
							//{
							//	//str1 += "(SELECT qtrelation_id FROM gisclient_21.qtrelation WHERE table_alias = '";
							//	//str1 += correctString4(geoweb_qt->geoweb_qtrelation->table_name);
							//	//str1 += "_1";
							//	//str1 += "' AND qt_id = (SELECT qt_id FROM gisclient_21.qt WHERE qt_name = '";
							//	str1 += "(SELECT qtrelation_id FROM gisclient_21.qtrelation WHERE qt_id = (SELECT qt_id FROM gisclient_21.qt WHERE qt_name = '";
							//	str1 += geoweb_qt->qt_name;
							//	str1 += "' AND layer_id = ";
							//	str1 += "(SELECT layer_id FROM gisclient_21.layer WHERE layer_name = '";
							//	str1 += geoweb_qt->layer_name;
							//	str1 += "' AND layergroup_id = (SELECT layergroup_id FROM gisclient_21.layergroup WHERE layergroup_name = '";
							//	str1 += geoweb_qt->layergroup_name;
							//	str1 += "' AND theme_id = (SELECT theme_id FROM gisclient_21.theme WHERE theme_name = '";
							//	str3 = geoweb_qt->theme_name;
							//	str3 = str3.toupper();
							//	str1 += str3;
							//	str1 += "' AND project_name = '";
							//	str1 += geoweb_structure.geoweb_catalog->project_name;
							//	str1 += "'))))),'";
							//}
							//else
							str1 += "0,'";
							field1 = correctString1(correctString3(geoweb_qtfield->qtfield_name));
							field1.tolower();
							pos7 = reserved_list.Find(field1.get_name());
							if (pos7)
								field1 += "_";
							str1 += field1;
							str1 += "', '";
							str1 += correctString3(geoweb_qtfield->qtfield_name);
							str1 += "', ";
							orderValue = orderValue + 5;
							str1 += orderValue;
							str1 += ", 0, 3";
							str1 += ");";

							if (fwprintf(sqlFile, _T("%s\n"), str1) < 0)
							{
								if (gsc_fclose(sqlFile) == GS_BAD)
									AfxThrowUserException();
								AfxThrowUserException();
							}
						}
					}
				}

				if (fwprintf(sqlFile, _T("%s\n"), _T("COMMIT;")) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
			}

			// Class
			if (geoweb_structure.geoweb_class_list.GetCount() > 0)
			{
				if (fwprintf(sqlFile, _T("%s\n"), _T("BEGIN;")) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				pos = geoweb_structure.geoweb_class_list.GetHeadPosition();
				while (pos)
				{
					geoweb_class = (Geoweb_Class*) geoweb_structure.geoweb_class_list.GetNext(pos);		

  //label_wrap character(1),
  //label_def text,
					if (geoweb_class->geosim_layer_list->GetCount() == 1)
					{
						str1 = "INSERT INTO gisclient_21.class";
						str1 += " (class_id,layer_id,class_name,class_title,legendtype_id,label_font,label_angle,label_color,label_position,symbol_ttf_name";
						str1 += ") VALUES (";
						str1 += "gisclient_21.new_pkey('class', 'class_id')";
						str1 += ",(SELECT layer_id FROM gisclient_21.layer WHERE layer_name = '";
						str1 += correctString3(geoweb_class->layer_name);
						str1 += "' AND layergroup_id = (SELECT layergroup_id FROM gisclient_21.layergroup WHERE layergroup_name = '";
						str1 += geoweb_class->layergroup_name;
						str1 += "' AND theme_id = (SELECT theme_id FROM gisclient_21.theme WHERE theme_name = '";
						str3 = geoweb_class->theme_name;
						str3 = str3.toupper();
						str1 += str3;
						str1 += "' AND project_name = '";
						str1 += geoweb_structure.geoweb_catalog->project_name;
						str1 += "'))),'";
						str1 += geoweb_class->class_name;
						str1 += "', '";
						str3 = geoweb_class->class_name;
						str3 = str3.tolower();
						str1 += str3;
						str1 += "', ";
						str1 += geoweb_class->legendtype_id;
						if (geoweb_class->geoweb_style)
							str1 += ", NULL, NULL, NULL, NULL, NULL);";
						else
						{
							str1 += ", '";
							str1 += geoweb_class->label_font;
							str1 += "', ";
							if (geoweb_class->label_angle == "")
								str1 += "NULL";
							else
							{
								str1 += "'";
								str1 += geoweb_class->label_angle;
								str1 += "'";
							}	
							str1 += ", '";
							str1 += (int) geoweb_class->label_colorR;
							str1 += " ";
							str1 += (int) geoweb_class->label_colorG;
							str1 += " ";
							str1 += (int) geoweb_class->label_colorB;
							str1 += "', '";
							str1 += geoweb_class->label_position;
							str1 += "', ";
							if ((geoweb_class->label_font != map_info.get_font()) && (geoweb_class->label_font != "geoweb"))
								str1 += "NULL";
							else
							{
								str1 += "'";
								str1 += geoweb_class->label_block.toupper();
								str1 += "'";
							}	
							str1 += ");";
						}				

						if (fwprintf(sqlFile, _T("%s\n"), str1) < 0)
						{
							if (gsc_fclose(sqlFile) == GS_BAD)
								AfxThrowUserException();
							AfxThrowUserException();
						}
					}
					else
					{
						str1 = "INSERT INTO gisclient_21.class";
						str1 += " (class_id,layer_id,class_name,class_title,legendtype_id,label_font,label_angle,label_color,label_position,symbol_ttf_name";
						str1 += ") VALUES (";
						str1 += "gisclient_21.new_pkey('class', 'class_id')";
						str1 += ",(SELECT layer_id FROM gisclient_21.layer WHERE layer_name = '";
						str1 += correctString3(geoweb_class->layer_name);
						str1 += "' AND layergroup_id = (SELECT layergroup_id FROM gisclient_21.layergroup WHERE layergroup_name = '";
						str1 += geoweb_class->layergroup_name;
						str1 += "' AND theme_id = (SELECT theme_id FROM gisclient_21.theme WHERE theme_name = '";
						str3 = geoweb_class->theme_name;
						str3 = str3.toupper();
						str1 += str3;
						str1 += "' AND project_name = '";
						str1 += geoweb_structure.geoweb_catalog->project_name;
						str1 += "'))), '";
						str1 += geoweb_class->class_name;
						str1 += "', '";
						str1 += geoweb_class->class_name;					
						str1 += "', ";
						str1 += geoweb_class->legendtype_id;
						if (geoweb_class->geoweb_style)
							str1 += ", NULL, NULL, NULL, NULL, NULL);";
						else
						{
							str1 += ", '";
							str1 += geoweb_class->label_font;
							str1 += "', ";
							if (geoweb_class->label_angle == "")
								str1 += "NULL";
							else
							{
								str1 += "'";
								str1 += geoweb_class->label_angle;
								str1 += "'";
							}				
							str1 += ", '";
							str1 += (int) geoweb_class->label_colorR;
							str1 += " ";
							str1 += (int) geoweb_class->label_colorG;
							str1 += " ";
							str1 += (int) geoweb_class->label_colorB;
							str1 += "', '";
							str1 += geoweb_class->label_position;
							str1 += "', ";
							if ((geoweb_class->label_font != map_info.get_font()) && (geoweb_class->label_font != "geoweb"))
								str1 += "NULL";
							else
							{
								str1 += "'";
								str1 += geoweb_class->label_block.toupper();
								str1 += "'";
							}	
							str1 += ");";
						}				
					
						if (fwprintf(sqlFile, _T("%s\n"), str1) < 0)
						{
							if (gsc_fclose(sqlFile) == GS_BAD)
								AfxThrowUserException();
							AfxThrowUserException();
						}
					
						x = 0;
						pos2 = geoweb_class->geosim_layer_list->GetHeadPosition();
						while (pos2)
						{
							geosim_layer = (Geosim_Layer*) geoweb_class->geosim_layer_list->GetNext(pos2);

							layer1 = geosim_layer->layer_name;
							layer1.toupper();
							original_class_name = correctString3(geoweb_class->original_class_name);
							original_class_name.toupper();
							pos5 = -1;
							pos5 = layer1.find(original_class_name);
							// solo i layer aventi parte iniziale corrispondente al nome della classe
							if (pos5 == 0)
							{
								str1 = "INSERT INTO gisclient_21.class";
								str1 += " (class_id,layer_id,class_name,class_title,expression,legendtype_id,label_font,label_angle,label_color,label_position,symbol_ttf_name";
								str1 += ") VALUES (";
								str1 += "gisclient_21.new_pkey('class', 'class_id')";
								str1 += ",(SELECT layer_id FROM gisclient_21.layer WHERE layer_name = '";
								str1 += correctString3(geoweb_class->layer_name);
								str1 += "' AND layergroup_id = (SELECT layergroup_id FROM gisclient_21.layergroup WHERE layergroup_name = '";
								str1 += geoweb_class->layergroup_name;
								str1 += "' AND theme_id = (SELECT theme_id FROM gisclient_21.theme WHERE theme_name = '";
								str3 = geoweb_class->theme_name;
								str3 = str3.toupper();
								str1 += str3;
								str1 += "' AND project_name = '";
								str1 += geoweb_structure.geoweb_catalog->project_name;
								str1 += "'))), '";
								str1 += geoweb_class->class_name;
								str1 += x;
								str1 += "', '";
								str1 += geoweb_class->class_name;
								str1 += x;
								str1 += "', '\"[layer]\"=\"";
								str1 += layer1;
								str1 += "\"', ";
								str1 += geoweb_class->legendtype_id;
								if (geoweb_class->geoweb_style)
									str1 += ", NULL, NULL, NULL, NULL, NULL);";
								else
								{
									str1 += ", '";
									str1 += geoweb_class->label_font;
									str1 += "', ";
									if (geoweb_class->label_angle == "")
										str1 += "NULL";
									else
									{
										str1 += "'";
										str1 += geoweb_class->label_angle;
										str1 += "'";
									}				
									str1 += ", '";
									str1 += (int) geosim_layer->colorR;
									str1 += " ";
									str1 += (int) geosim_layer->colorG;
									str1 += " ";
									str1 += (int) geosim_layer->colorB;
									str1 += "', '";
									str1 += geoweb_class->label_position;
									str1 += "', ";
									if ((geoweb_class->label_font != map_info.get_font()) && (geoweb_class->label_font != "geoweb"))
										str1 += "NULL";
									else
									{
										str1 += "'";
										str1 += geoweb_class->label_block.toupper();
										str1 += "'";
									}	
									str1 += ");";
								}				
							
								if (fwprintf(sqlFile, _T("%s\n"), str1) < 0)
								{
									if (gsc_fclose(sqlFile) == GS_BAD)
										AfxThrowUserException();
									AfxThrowUserException();
								}
						
								x++;
							}
						}
					}
				}

				if (fwprintf(sqlFile, _T("%s\n"), _T("COMMIT;")) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
			}
			
			// Style
			if (geoweb_structure.geoweb_class_list.GetCount() > 0)
			{
				if (fwprintf(sqlFile, _T("%s\n"), _T("BEGIN;")) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				pos = geoweb_structure.geoweb_class_list.GetHeadPosition();
				while (pos)
				{
					geoweb_class = (Geoweb_Class*) geoweb_structure.geoweb_class_list.GetNext(pos);

					if (geoweb_class->geoweb_style)
					{
						str1 = "INSERT INTO gisclient_21.style";
						str1 += " (style_id,class_id,color,outlinecolor,style_name";
						str1 += ") VALUES (";
						str1 += "gisclient_21.new_pkey('style', 'style_id')";
						str1 += ",(SELECT class_id FROM gisclient_21.class WHERE class_name = '";
						str1 += geoweb_class->class_name;
						str1 += "' AND layer_id = (SELECT layer_id FROM gisclient_21.layer WHERE layer_name = '";
						str1 += correctString3(geoweb_class->layer_name);
						str1 += "' AND layergroup_id = (SELECT layergroup_id FROM gisclient_21.layergroup WHERE layergroup_name = '";
						str1 += geoweb_class->layergroup_name;
						str1 += "' AND theme_id = (SELECT theme_id FROM gisclient_21.theme WHERE theme_name = '";
						str3 = geoweb_class->theme_name;
						str3 = str3.toupper();
						str1 += str3;
						str1 += "' AND project_name = '";
						str1 += geoweb_structure.geoweb_catalog->project_name;
						str1 += "')))),'";
						str1 += (int) geoweb_class->geoweb_style->colorR;
						str1 += " ";	
						str1 += (int) geoweb_class->geoweb_style->colorG;
						str1 += " ";	
						str1 += (int) geoweb_class->geoweb_style->colorB;
						str1 += "', '";
						str1 += (int) geoweb_class->geoweb_style->outlinecolorR;
						str1 += " ";	
						str1 += (int) geoweb_class->geoweb_style->outlinecolorG;
						str1 += " ";	
						str1 += (int) geoweb_class->geoweb_style->outlinecolorB;
						str1 +=	"', '";
						str1 += geoweb_class->geoweb_style->style_name;
						str1 += "');";

						if (fwprintf(sqlFile, _T("%s\n"), str1) < 0)
						{
							if (gsc_fclose(sqlFile) == GS_BAD)
								AfxThrowUserException();
							AfxThrowUserException();
						}

						x = 0;
						pos2 = geoweb_class->geosim_layer_list->GetHeadPosition();
						while (pos2)
						{
							geosim_layer = (Geosim_Layer*) geoweb_class->geosim_layer_list->GetNext(pos2);

							layer1 = geosim_layer->layer_name;
							layer1.toupper();
							original_class_name = correctString3(geoweb_class->original_class_name);
							original_class_name.toupper();
							pos5 = -1;
							pos5 = layer1.find(original_class_name);
							// solo i layer aventi parte iniziale corrispondente al nome della classe
							if (pos5 == 0)
							{
								str1 = "INSERT INTO gisclient_21.style";
								str1 += " (style_id,class_id,color,outlinecolor,style_name";
								str1 += ") VALUES (";
								str1 += "gisclient_21.new_pkey('style', 'style_id')";
								str1 += ",(SELECT class_id FROM gisclient_21.class WHERE class_name = '";
								str1 += geoweb_class->class_name;
								str1 += x;
								str1 += "' AND layer_id = (SELECT layer_id FROM gisclient_21.layer WHERE layer_name = '";
								str1 += correctString3(geoweb_class->layer_name);
								str1 += "' AND layergroup_id = (SELECT layergroup_id FROM gisclient_21.layergroup WHERE layergroup_name = '";
								str1 += geoweb_class->layergroup_name;
								str1 += "' AND theme_id = (SELECT theme_id FROM gisclient_21.theme WHERE theme_name = '";
								str3 = geoweb_class->theme_name;
								str3 = str3.toupper();
								str1 += str3;
								str1 += "' AND project_name = '";
								str1 += geoweb_structure.geoweb_catalog->project_name;
								str1 += "')))),'";
								str1 += (int) geosim_layer->colorR;
								str1 += " ";	
								str1 += (int) geosim_layer->colorG;
								str1 += " ";	
								str1 += (int) geosim_layer->colorB;
								str1 += "', '";
								str1 += (int) geosim_layer->colorR;
								str1 += " ";	
								str1 += (int) geosim_layer->colorG;
								str1 += " ";	
								str1 += (int) geosim_layer->colorB;
								str1 +=	"', '";
								str1 += geoweb_class->class_name;
								str1 += x;
								str1 += "_S0');";

								if (fwprintf(sqlFile, _T("%s\n"), str1) < 0)
								{
									if (gsc_fclose(sqlFile) == GS_BAD)
										AfxThrowUserException();
									AfxThrowUserException();
								}
						
								x++;
							}
						}
					}
				}

				if (fwprintf(sqlFile, _T("%s\n"), _T("COMMIT;")) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
			}

			if (gsc_fclose(sqlFile) == GS_BAD)
				AfxThrowUserException();

			// converte il direttorio con l'alias
			alias = map_info.get_prj_path();
			if (gsc_drive2nethost(alias) == GS_BAD) 
				AfxThrowUserException();
			alias_path = alias.get_name();
			eraseMinorAndMajor(&alias_path);

			dest_file = postGis_path;
			dest_file += "/sql/";
			dest_file += alias_path;
			//	eventuale creazione direttorio
			if (gsc_mkdir(dest_file) == GS_BAD)
				AfxThrowUserException();

			sql_dest_file = dest_file;
			sql_dest_file += "/geoweb_font.sql";
			
			// copio il file
			if (gsc_copyfile(sqlfont_file, sql_dest_file) == GS_BAD)
				AfxThrowUserException();

			sql_dest_file = dest_file;
			sql_dest_file += "/geoweb_structure_";
			sql_dest_file += ini;
			sql_dest_file += ".sql";
			
			// copio il file
			if (gsc_copyfile(sql_file, sql_dest_file) == GS_BAD)
				AfxThrowUserException();

			sql_dest_file.strtran(_T("\\"), _T("/"));
			log_file = dest_file;
			//log_file += "/geoweb_structure.txt";
			log_file += "/geoweb_structure_";
			log_file += ini;
			log_file += ".txt";
			log_file.strtran(_T("\\"), _T("/"));

			sql_file = dest_file;
			//sql_file += "/geoweb_structure.bat";
			sql_file += "/geoweb_structure_";
			sql_file += ini;
			sql_file += ".bat";

			if ((sqlFile = gsc_fopen(sql_file, _T("w"), ONETEST)) == NULL)
				AfxThrowUserException();

			if (map_info.get_database() == "")
			{
				db = _T("geoweb_");
				db += map_info.get_prj_name();
			}
			else
				db = map_info.get_database();

			if (server == "")
			{
				//if (fwprintf(sqlFile, _T("%s %s %s %s \"%s\" %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-o"), log_file, _T("-f"), sql_dest_file, _T("geoweb")) < 0)
				if (fwprintf(sqlFile, _T("%s %s %s %s \"%s\" %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-o"), log_file, _T("-f"), sql_dest_file, db) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
			}
			else
			{
				//if (fwprintf(sqlFile, _T("%s %s %s %s %s %s \"%s\" %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-h"), server, _T("-o"), log_file, _T("-f"), sql_dest_file, _T("geoweb")) < 0)
				if (fwprintf(sqlFile, _T("%s %s %s %s %s %s \"%s\" %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-h"), server, _T("-o"), log_file, _T("-f"), sql_dest_file, db) < 0)
				{
					if (gsc_fclose(sqlFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
			}

			if (gsc_fclose(sqlFile) == GS_BAD)
				AfxThrowUserException();

			total_file = dest_file;
			total_file += "/";
			//total_file += "totale.bat";
			total_file += "totale_";
			total_file += ini;
			total_file += ".bat";
			if ((totalFile = gsc_fopen(total_file, _T("w"), ONETEST)) == NULL)
				AfxThrowUserException();

			sql_dest_file = dest_file;			
			//sql_dest_file += "/createschema.sql";
			sql_dest_file += "/createschema_";
			sql_dest_file += ini;
			sql_dest_file += ".sql";
			sql_dest_file.strtran(_T("\\"), _T("/"));
			if ((destFile = gsc_fopen(sql_dest_file, _T("w"), ONETEST)) == NULL)
				AfxThrowUserException();

			str2 = "BEGIN;\n";
			str2 += "CREATE SCHEMA ";
			str2 += map_info.get_prj_name();
			//str2 += " AUTHORIZATION postgres;\n";
			str2 += " AUTHORIZATION ";
			str2 += user;
			str2 += ";\n";
			str2 += "GRANT ALL ON SCHEMA ";
			str2 += map_info.get_prj_name();
			//str2 += " TO postgres;\n";
			str2 += " TO ";
			str2 += user;
			str2 += ";\n";
			str2 += "GRANT ALL ON SCHEMA ";
			str2 += map_info.get_prj_name();
			str2 += " TO \"Admin\";\n";
			str2 += "GRANT USAGE ON SCHEMA ";
			str2 += map_info.get_prj_name();
			str2 += " TO mapserver;\n";
			str2 += "COMMIT;\n";

			if (fwprintf(destFile, _T("%s"), str2) < 0)
			{
				if (gsc_fclose(destFile) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}

			if (gsc_fclose(destFile) == GS_BAD)
				AfxThrowUserException();

			if (map_info.get_database() == "")
			{
				db = _T("geoweb_");
				db += map_info.get_prj_name();
			}
			else
				db = map_info.get_database();

			if (server == "")
			{
				//if (fwprintf(totalFile, _T("%s %s %s %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-f"), sql_dest_file, _T("geoweb_dati")) < 0)
				if (fwprintf(totalFile, _T("%s %s %s %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-f"), sql_dest_file, db) < 0)
				{
					if (gsc_fclose(totalFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
			}
			else
			{
				//if (fwprintf(totalFile, _T("%s %s %s %s %s %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-h"), server, _T("-f"), sql_dest_file, _T("geoweb_dati")) < 0)
				if (fwprintf(totalFile, _T("%s %s %s %s %s %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-h"), server, _T("-f"), sql_dest_file, db) < 0)
				{
					if (gsc_fclose(totalFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
			}

			sql_dest_file = dest_file;			
			sql_dest_file += "/createfunctions.sql";
			sql_dest_file.strtran(_T("\\"), _T("/"));
			if ((destFile = gsc_fopen(sql_dest_file, _T("w"), ONETEST)) == NULL)
				AfxThrowUserException();

			str2 = "BEGIN;\n";
			str2 += "CREATE OR REPLACE FUNCTION gw_droptable(sk character varying, tb character varying)\n";
			str2 += "  RETURNS text AS\n";
			str2 += "$BODY$\n";
			str2 += "DECLARE\n";
			str2 += "schema_name varchar;\n";
			str2 += "BEGIN\n";
			str2 += "IF coalesce(tb,'')='' THEN\n";
			str2 += "return 'TABELLA NON DEFINITA';\n";
			str2 += "END IF;\n";
			str2 += "IF coalesce(sk,'')='' THEN\n";
			str2 += "schema_name:='public';\n";
			str2 += "ELSE\n";
			str2 += "schema_name:=sk;\n";
			str2 += "END IF;\n";
			str2 += "BEGIN\n";
			str2 += "EXECUTE 'DROP TABLE ' || quote_ident(schema_name) || '.' || quote_ident(tb) || ' CASCADE';\n";
			str2 += "EXCEPTION\n";
			str2 += "WHEN undefined_table THEN\n";
			str2 += "return 'LA TABELLA '||schema_name||'.'||tb||' NON ESISTE';\n";
			str2 += "WHEN dependent_objects_still_exist THEN\n";
			str2 += "return 'ALTRI OGGETTI DIPENDONO DALLA TABELLA '||schema_name||'.'||tb;\n";
			str2 += "END;\n";
			str2 += "return 'TABELLA '||schema_name||'.'||tb||' CANCELLATA';\n";
			str2 += "END;\n";
			str2 += "$BODY$\n";
			str2 += "  LANGUAGE 'plpgsql' VOLATILE;\n";
			//str2 += "ALTER FUNCTION gw_droptable(sk character varying, tb character varying) OWNER TO postgres;\n";
			//str2 += "GRANT EXECUTE ON FUNCTION gw_droptable(sk character varying, tb character varying) TO postgres;\n";
			str2 += "ALTER FUNCTION gw_droptable(sk character varying, tb character varying) OWNER TO ";
			str2 += user;
			str2 += ";\n";
			str2 += "GRANT EXECUTE ON FUNCTION gw_droptable(sk character varying, tb character varying) TO ";
			str2 += user;
			str2 += ";\n";
			str2 += "GRANT EXECUTE ON FUNCTION gw_droptable(sk character varying, tb character varying) TO public;\n";
			str2 += "GRANT EXECUTE ON FUNCTION gw_droptable(sk character varying, tb character varying) TO mapserver;\n";
			str2 += "COMMIT;\n";

			str2 += "BEGIN;\n";
			str2 += "CREATE OR REPLACE FUNCTION gw_dropview(sk character varying, vw character varying)\n";
			str2 += "  RETURNS text AS\n";
			str2 += "$BODY$\n";
			str2 += "DECLARE\n";
			str2 += "schema_name varchar;\n";
			str2 += "BEGIN\n";
			str2 += "IF coalesce(vw,'')='' THEN\n";
			str2 += "return 'VISTA NON DEFINITA';\n";
			str2 += "END IF;\n";
			str2 += "IF coalesce(sk,'')='' THEN\n";
			str2 += "schema_name:='public';\n";
			str2 += "ELSE\n";
			str2 += "schema_name:=sk;\n";
			str2 += "END IF;\n";
			str2 += "BEGIN\n";
			str2 += "EXECUTE 'DROP VIEW ' || quote_ident(schema_name) || '.' || quote_ident(vw);\n";
			str2 += "EXCEPTION\n";
			str2 += "WHEN undefined_table THEN\n";
			str2 += "return 'LA VISTA '||schema_name||'.'||vw||' NON ESISTE';\n";
			str2 += "WHEN dependent_objects_still_exist THEN\n";
			str2 += "return 'ALTRI OGGETTI DIPENDONO DALLA VISTA '||schema_name||'.'||vw;\n";
			str2 += "END;\n";
			str2 += "return 'VISTA '||schema_name||'.'||vw||' CANCELLATA';\n";
			str2 += "END;\n";
			str2 += "$BODY$\n";
			str2 += "  LANGUAGE 'plpgsql' VOLATILE;\n";
			//str2 += "ALTER FUNCTION gw_dropview(sk character varying, vw character varying) OWNER TO postgres;\n";
			//str2 += "GRANT EXECUTE ON FUNCTION gw_dropview(sk character varying, vw character varying) TO postgres;\n";
			str2 += "ALTER FUNCTION gw_dropview(sk character varying, vw character varying) OWNER TO ";
			str2 += user;
			str2 += ";\n";
			str2 += "GRANT EXECUTE ON FUNCTION gw_dropview(sk character varying, vw character varying) TO ";
			str2 += user;
			str2 += ";\n";
			str2 += "GRANT EXECUTE ON FUNCTION gw_dropview(sk character varying, vw character varying) TO public;\n";
			str2 += "GRANT EXECUTE ON FUNCTION gw_dropview(sk character varying, vw character varying) TO mapserver;\n";
			str2 += "COMMIT;\n";

			if (fwprintf(destFile, _T("%s"), str2) < 0)
			{
				if (gsc_fclose(destFile) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}

			if (gsc_fclose(destFile) == GS_BAD)
				AfxThrowUserException();

			if (server == "")
			{
				//if (fwprintf(totalFile, _T("%s %s %s %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-f"), sql_dest_file, _T("geoweb_dati")) < 0)
				if (fwprintf(totalFile, _T("%s %s %s %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-f"), sql_dest_file, db) < 0)
				{
					if (gsc_fclose(totalFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
			}
			else
			{
				//if (fwprintf(totalFile, _T("%s %s %s %s %s %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-h"), server, _T("-f"), sql_dest_file, _T("geoweb_dati")) < 0)
				if (fwprintf(totalFile, _T("%s %s %s %s %s %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-h"), server, _T("-f"), sql_dest_file, db) < 0)
				{
					if (gsc_fclose(totalFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
			}

			if (gsc_fclose(totalFile) == GS_BAD)
				AfxThrowUserException();

			pos6 = geoininame.reverseFind(_T('/'));
			ini = geoininame.right(geoininame.len() - pos6 - 1);
			ini.strtran(_T(".ini"), _T(""));

			sql_dest_file = dest_file;			
			sql_dest_file += "/grant.sql";
			sql_dest_file.strtran(_T("\\"), _T("/"));
			if ((destFile = gsc_fopen(sql_dest_file, _T("w"), ONETEST)) == NULL)
				AfxThrowUserException();

			str2 = "BEGIN;\n";
			str2 += "SELECT gw_grantselect('mapserver', '";
			str2 += map_info.get_prj_name();
			str2 += "', 1);\n";
			str2 += "COMMIT;\n";

			if (fwprintf(destFile, _T("%s"), str2) < 0)
			{
				if (gsc_fclose(destFile) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}

			if (gsc_fclose(destFile) == GS_BAD)
				AfxThrowUserException();

			if (map_info.set_write_db_prj(1) == GS_BAD)
			{
				set_GS_ERR_COD(eGSWriteFile); 
				AfxThrowUserException();
			}
		}
	}
	CATCH( CUserException, e )
	{
		ResetSystemVar();
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

int CMapPostGIS::ProjectClassWrite()
{
	int ret = GS_GOOD;

	C_STRING input;

	TRY
	{
		//	da "<ALIAS>\dir progetto\sottodir classe\nome tabella classe
		//	VA COPIATA IN "<POSTGIS>\[nome alias]\dir progetto\sottodir classe\nome tabella classe"
		//	Questo controllo andrebbe fatto una volta sola per ogni CLASSE
		if (setting_info.get_first() && (geoininame != "")) 
		{
			if (map_info.get_sub_index())
			{
				input = "progetto: ";
				input += map_info.get_prj_name();
				input += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;classe: ";
				input += map_info.get_class_name();
				input += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;sottoclasse: ";
				input += map_info.get_sub_name();
			}
			else
			{
				input = "progetto: ";
				input += map_info.get_prj_name();
				input += "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;classe: ";
				input += map_info.get_class_name();
			}
			input += "<BR>";	

			if (RegLogFile(input) != GS_GOOD)
				AfxThrowUserException();

			setting_info.set_write_first(FALSE);
		}
	}
	CATCH( CUserException, e )
	{
		ResetSystemVar();
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

int CMapPostGIS::DwgsControl()
{
	int ret = GS_GOOD;

	C_STRING	path_cls;
	C_STRING path;

	long totale, dwg;

	presbuf dwgnames, dwgdates;
	presbuf sqlnames, sqldates;
	presbuf dwknames;

	TRY
	{
		//	verifica se ci sono dwg memorizzati, se la lista dei DWG è già stata costruita 
		//	allora è stata sicuramente copiata anche la tabella della classe
		if (map_info.count_dwg() == 0) 
		{	 
			if ((path_cls = map_info.get_path_with_ext(_T(""))) == "") 
				AfxThrowUserException();

			path = path_cls;
			path += "*.dwg";
			totale = gsc_adir(path.get_name(), &dwgnames, NULL, &dwgdates, NULL);	//	costruisce la lista dei files sql		
			dwg = totale;

			path = path_cls;
			path += "*.sql";
			totale = gsc_adir(path.get_name(), &sqlnames, NULL, &sqldates, NULL);	//	costruisce la lista dei files sql
			
			path = path_cls;
			path += "*.dwk";
			totale = gsc_adir(path.get_name(), &dwknames, NULL, NULL, NULL);		//	costuisce la lista dei dwk

			if (DwgListBuild(dwgnames, dwgdates, sqlnames, sqldates, dwknames) == GS_BAD)
			{
				ads_relrb(dwgnames);
				dwgnames=NULL;
				ads_relrb(dwgdates);
				dwgdates=NULL;
				ads_relrb(dwknames);
				dwknames=NULL;
				ads_relrb(sqlnames);
				sqlnames=NULL;
				ads_relrb(sqldates);
				sqldates=NULL;
				AfxThrowUserException();
			}		

			// PAOLETTA 
			// considerato che nei file .sql è presente il campo fid (univoco)
			// per garantire tale univocità una classe composta da più disegni deve essere ripassata 
			// nella sua totalità, pertanto vengono ripresi tutti i disegni
			if ((dwg > 1) && (map_info.count_dwg() > 0))
			{
				map_info.remove_all_dwg();
				if (DwgListBuildNoDate(dwgnames, dwgdates, sqlnames, sqldates, dwknames) == GS_BAD)
				{
					ads_relrb(dwgnames);
					dwgnames=NULL;
					ads_relrb(dwgdates);
					dwgdates=NULL;
					ads_relrb(dwknames);
					dwknames=NULL;
					ads_relrb(sqlnames);
					sqlnames=NULL;
					ads_relrb(sqldates);
					sqldates=NULL;
					AfxThrowUserException();
				}		
			}

			ads_relrb(dwgnames);
			dwgnames=NULL;
			ads_relrb(dwgdates);
			dwgdates=NULL;
			ads_relrb(sqlnames);
			sqlnames=NULL;
			ads_relrb(sqldates);
			sqldates=NULL;
			ads_relrb(dwknames);
			dwknames=NULL;
		}
	}
	CATCH( CUserException, e )
	{
		ResetSystemVar();
		if (geoscript);	
			gsc_fclose(geoscript);
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

int CMapPostGIS::ChangeScriptNumber()
{
	int ret = GS_GOOD;

	TRY
	{
		//	la sezione dei dwg esiste se sono stati trovati dei DWG
		//	al passaggio precedente o nel file mapPostgis.ini
		//	cerca il primo DWG da processare (valore 0)
		//	deve assegnare il sistema di coordinate
		//	inizio costruzione di GEOSCRIPT.SCR       -- GEOSCRIPT.SCR aperto in scrittura
		if (map_info.get_class_index() == 0)
			if (map_info.set_write_script_number(0) == GS_BAD)
			{
				set_GS_ERR_COD(eGSWriteFile); 
				AfxThrowUserException();
			}

		scriptname = geowork_path;
		scriptname += POSTGIS;
		scriptname += "\\GEOSCRIP";
		scriptname += map_info.get_script_number();
		scriptname += ".SCR";
		
		if (map_info.get_script_number())
		{
			if (map_info.set_write_script_number(0) == GS_BAD)
				AfxThrowUserException();
		}
		else
			if (map_info.set_write_script_number(1) == GS_BAD)
				AfxThrowUserException();
	}
	CATCH( CUserException, e )
	{
		ResetSystemVar();
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

int CMapPostGIS::DwgExist()
{
	int ret = GS_GOOD;

	int valid_dwg;

	C_STRING input, str;

	C_STRING		dwg_file, dwk_file, sql_file, dest_file, sql_dest_file, alias_path, total_file, sql_db, log_file;
	C_STRING	alias;

	int is_deleted;
	
	C_STRING sample, ini;

	FILE	*sqlFile, *totalFile;

	int		pos6;

	ade_id dwg_id = ADE_NULLID;

	C_STRING	db;

	TRY
	{
		//	 controllare che il file aperto sia quello corrispondente a DWG_NAME
		//	 altrimenti questa funzione va dopo l'apertura del DWG
		valid_dwg=0;   	
		if (IsCurrentOpenedDwg(&dwg_id) == GS_GOOD)	// DWG aperto
		{
			input = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;DWG processato: ";
			input += dwg_name;
			input += "<BR>";

			if (RegLogFile(input) != GS_GOOD)
				AfxThrowUserException();

			//	Controllo se esistono oggetti della classe nel DWG
			// PAOLA
			//if (ExistGeowebObjects() == GS_BAD)
			//{
			//	if (map_info.remove_last_dwg() == GS_BAD)	// rimuove il dwg nella lista
			//		AfxThrowUserException();

			//	input = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Nel DWG <";
			//	input += dwg_name;
			//	input += "> non vi sono oggetti per il passaggio dati.<BR>";
			//	if (RegLogFile(input) != GS_GOOD)
			//		AfxThrowUserException();

			//	map_info.add_removed_dwg(dwg_name);
			//	dwg_name = "";
			//	map_info.get_last_dwg(&dwg_name);

			//	valid_dwg=0;
			//}
			//else
				valid_dwg=1;
		}

		if (valid_dwg)			// DWG aperto e contenente dati GEOsim
		{	
			if (map_info.get_prj_index() && map_info.get_class_index())
			{
				if (ClassCoord() == GS_BAD)
					AfxThrowUserException();
			}
			else 
				map_info.set_coord_input(coord_info.get_coord_input());

//			if ((map_info.get_coord_input() != "") && (map_info.get_coord_input() != "NONE"))
//				if (fwprintf(geoscript, _T("_.ADESETCRDSYS\n_S\n%s\n_X\n"), map_info.get_coord_input()) <0 )
//					AfxThrowUserException();

			dwg_file = map_info.get_class_dwg_path();
			dwg_file += "\\";
			dwg_file += dwg_name;
			sql_file = onlyLeft(dwg_file, '.');
			if (map_info.get_sub_index() != 0)
			{
				sql_file += "_";
				sql_file += map_info.get_sub_name();
			}
			sql_file += ".sql";

			if ((sqlFile = gsc_fopen(sql_file, _T("w"), ONETEST)) == NULL)
				AfxThrowUserException();				

			if (CreateAllSqls(sqlFile, dwg_id) == GS_BAD)
				AfxThrowUserException();

			if (gsc_fclose(sqlFile) == GS_BAD)
				AfxThrowUserException();

			// converte il direttorio con l'alias
			alias = map_info.get_prj_path();
			if (gsc_drive2nethost(alias) == GS_BAD) 
				AfxThrowUserException();
			alias_path = alias.get_name();
			eraseMinorAndMajor(&alias_path);

			dest_file = postGis_path;
			dest_file += "/sql/";
			dest_file += alias_path;

			pos6 = geoininame.reverseFind(_T('/'));
			ini = geoininame.right(geoininame.len() - pos6 -1);
			ini.strtran(_T(".ini"), _T(""));

			total_file = dest_file;
			total_file += "/";
			//total_file += "totale.bat";
			total_file += "totale_";
			total_file += ini;
			total_file += ".bat";

			sql_dest_file = dest_file;			
			sql_dest_file += "/";
			sql_dest_file += onlyLeft(dwg_name, '.');
			if (map_info.get_sub_index() != 0)
			{
				sql_dest_file += "_";
				sql_dest_file += map_info.get_sub_name();
			}
			sql_dest_file += ".sql";
			// copio il file
			if (gsc_copyfile(sql_file, sql_dest_file) == GS_BAD)
				AfxThrowUserException();

			if ((totalFile = gsc_fopen(total_file, _T("a"), ONETEST)) == NULL)
				AfxThrowUserException();
			sql_dest_file.strtran(_T("\\"), _T("/"));
			sql_db = map_info.get_prj_name();

			log_file = dest_file;			
			log_file += "/log_";
			log_file += onlyLeft(dwg_name, '.');
			log_file += ".txt";
			log_file.strtran(_T("\\"), _T("/"));

			if (map_info.get_database() == "")
			{
				db = _T("geoweb_");
				db += map_info.get_prj_name();
			}
			else
				db = map_info.get_database();

			if (server == "")
			{
				//if (fwprintf(totalFile, _T("%s %s %s %s \"%s\" %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-o"), log_file, _T("-f"), sql_dest_file, _T("geoweb_dati")) < 0)
				if (fwprintf(totalFile, _T("%s %s %s %s \"%s\" %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-o"), log_file, _T("-f"), sql_dest_file, db) < 0)
				{
					if (gsc_fclose(totalFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
			}
			else
			{
				//if (fwprintf(totalFile, _T("%s %s %s %s %s %s \"%s\" %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-h"), server, _T("-o"), log_file, _T("-f"), sql_dest_file, _T("geoweb_dati")) < 0)
				if (fwprintf(totalFile, _T("%s %s %s %s %s %s \"%s\" %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-h"), server, _T("-o"), log_file, _T("-f"), sql_dest_file, db) < 0)
				{
					if (gsc_fclose(totalFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
			}

			if (gsc_fclose(totalFile) == GS_BAD)
				AfxThrowUserException();
		}				
		else     // DWG aperto, non contenente dati validi
		{
			if (dwg_name.len() != 0) 	// DWG da aprire
			{
				if ((geoscript = gsc_fopen(scriptname.get_name(), _T("w"), ONETEST)) == NULL)
					AfxThrowUserException();				

				dwg_file = map_info.get_class_dwg_path();
				dwg_file += "\\";
				dwg_file += dwg_name;
				dwk_file = onlyLeft(dwg_file, '.');
				dwk_file += ".dwk";

				is_deleted = 0;

				if (map_info.search_dwg(dwg_name) == GS_BAD)
					is_deleted=1;
				
				//	se il nome del dwg è stato cancellato da mapPostgis.ini
				//	oppure se esiste il dwk, allora deve saltare il DWG
				if ((is_deleted) || (gsc_path_exist(dwk_file)))
				{
					if (!is_deleted)
					{
						if (gsc_dsdetach(dwg_id) == GS_BAD)
							AfxThrowUserException();
						if (map_info.remove_last_dwg() == GS_BAD)
							AfxThrowUserException();
					}

					if (gsc_isCurrentDWGModified())
						str = _T("_.NEW\n_Y\n.\n"); // Il comando chiede se si vuole abbandonare le modifiche
					else
						str = _T("_.NEW\n.\n");

					//	scrivo nel file script
					if (fwprintf(geoscript, str.get_name()) < 0)
					{
						set_GS_ERR_COD(eGSWriteFile); 
						AfxThrowUserException();
					}
				}
				else
				{
					// copio il disegno prototipo di GEOsim (GEOPROTDWG = sample.dwg) nel direttorio
					// della classe
					sample = geowork_path + POSTGIS + _T('\\') + GEOSAMPLEDIR + _T('\\') + GEOPROTDWG;

					if (gsc_isCurrentDWGModified())
						str = _T("_.OPEN\n_Y\n%s\n"); // Il comando chiede se si vuole abbandonare le modifiche
					else
						str = _T("_.OPEN\n%s\n");

					//	scrivo nel file script
					if (fwprintf(geoscript, str.get_name(), sample) < 0)
					{
						set_GS_ERR_COD(eGSWriteFile); 
						AfxThrowUserException();
					}
				}
				
      			if (gsc_fclose(geoscript) == GS_BAD)
					AfxThrowUserException();

				//	lancio script
				if (acedCommandS(RTSTR, _T("_.SCRIPT"), RTSTR, scriptname.get_name(), 0) != RTNORM)	//	script per l'apertura di un DWG
					AfxThrowUserException();	// codice errore
			}
			else
			{
				//	aggiorna il file MAPSDF.INI
				if (UpdateMapPostGISIni() == GS_BAD)	
				{
					if ((geoscript = gsc_fopen(scriptname.get_name(), _T("w"), ONETEST)) == NULL)
						AfxThrowUserException();				

					if (fwprintf(geoscript, _T("_.QUIT\n_Y\n")) < 0)
					{
						set_GS_ERR_COD(eGSWriteFile);
						AfxThrowUserException();
					}

					if (gsc_fclose(geoscript) == GS_BAD)
						AfxThrowUserException();

					if (DeleteSettingIni() == GS_BAD)
						AfxThrowUserException();

					if (DeleteMapPostGISIni() == GS_BAD)
						AfxThrowUserException();

					if (ResetSystemVar() == GS_BAD)
						AfxThrowUserException();
					
					if (acedCommandS(RTSTR, _T("_.SCRIPT"), RTSTR, scriptname.get_name(), 0) != RTNORM)	//	script per l'apertura di un DWG
						AfxThrowUserException();	// codice errore
				}
				else
				{
					if ((geoscript = gsc_fopen(scriptname.get_name(), _T("w"), ONETEST)) == NULL)
						AfxThrowUserException();	

					if (gsc_isCurrentDWGModified())
						str = _T("_.NEW\n_Y\n.\n"); // Il comando chiede se si vuole abbandonare le modifiche
					else
						str = _T("_.NEW\n.\n");

					if (fwprintf(geoscript, str.get_name()) < 0)
					{
						set_GS_ERR_COD(eGSWriteFile);
						AfxThrowUserException();
					}
					
					if (gsc_fclose(geoscript) == GS_BAD)
						AfxThrowUserException();

					if (acedCommandS(RTSTR, _T("_.SCRIPT"), RTSTR, scriptname.get_name(), 0) != RTNORM)	//	script per l'apertura di un DWG
						AfxThrowUserException();	// codice errore
				}   
			}
		}
	}
	CATCH( CUserException, e )
	{
		if (geoscript)
			gsc_fclose(geoscript);
		if (sqlFile)
			gsc_fclose(sqlFile);
		ResetSystemVar();
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

int CMapPostGIS::DwgNoExist()
{
	int ret = GS_GOOD;

	C_STRING path_cls, file, temp;
	C_STRING path;

	presbuf sqlnames=NULL;
	C_RB_LIST	temp1, temp2;

	C_STRING layer;

	C_STRING input, str;

	TRY
	{
		// non esistono DWG
		// se non lo trova allora la lista dei DWG della classe
		// è esaurita o non è mai esistita
		// va costruito l'sdf generale con l'append (se append_sdf=1)
		// occorre passare alla classe successiva
		// cercare se esiste la sezione SDF
		// se la sezione esiste allora va aggiornato l'sdf generale
		// lo riapre in scrittura
		if (map_info.remove_all_dwg() == GS_BAD)
			AfxThrowUserException();

		if (map_info.get_sql() != 0)	//	controllare il valore
		{
			if (map_info.set_write_sql(0) == GS_BAD)
				AfxThrowUserException();
			
			if ((geoscript = gsc_fopen(scriptname.get_name(), _T("w"), ONETEST)) == NULL)
				AfxThrowUserException();

			//	scrivere nello script il comando map_sdf per ripartire
			//	if (fwprintf(geoscript, _T("_.new\n_Y\n.\nmap_sdf\n")) < 0)
			if (gsc_isCurrentDWGModified())
				str = _T("_.NEW\n_Y\n.\n"); // Il comando chiede se si vuole abbandonare le modifiche
			else
				str = _T("_.NEW\n.\n");
			if (fwprintf(geoscript, str.get_name()) < 0)
			{
				set_GS_ERR_COD(eGSWriteFile);
				AfxThrowUserException();
			}
			
			if (gsc_fclose(geoscript) == GS_BAD)
				AfxThrowUserException();

			map_info.get_last_dwg(&dwg_name);
			if (dwg_name == "")
 			{
				//	aggiorna il file MAPSDF.INI
				if (UpdateMapPostGISIni() == GS_BAD)	
				{										
					if (DeleteMapPostGISIni() == GS_BAD)
						AfxThrowUserException();														
					
					if (ResetSystemVar() == GS_BAD)
						AfxThrowUserException();
					//	lancio script        
					if (acedCommandS(RTSTR, _T("_.SCRIPT"), RTSTR, scriptname.get_name(), 0) != RTNORM)
						AfxThrowUserException();
					
					return GS_GOOD;		//	TERMINA se i progetti e le classi per il passaggio sono finiti
				}
			}

			//	lancio script
			if (acedCommandS(RTSTR, _T("_.SCRIPT"), RTSTR,	scriptname.get_name(), 0) != RTNORM)
				AfxThrowUserException();

            return GS_GOOD;		//	LA FUNZIONE RIPARTE
		}
		//	se non trova alcun sql allora non c'è niente da aggiornare
		//	e la funzione deve ripartire se ci sono ancora classi da considerare
		else
		{
			if (PathWithExt(&path_cls, "dwk") == GS_BAD)
				AfxThrowUserException();
			
			map_info.get_last_dwg(&dwg_name);
			if (dwg_name == "")
			{
				if (map_info.get_class_type() != CL_COMPL)
					input = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Nessuna modifica alla classe rispetto al passaggio dati precedente.<BR>";
				else
				{
					input = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;La classe (di tipo ";
					input += map_info.get_class_type_text();
					input += ") e\' collegata a un database.<BR>";
				}

				if (RegLogFile(input) != GS_GOOD)
					AfxThrowUserException();
				
				if (UpdateMapPostGISIni() == GS_BAD)	
				{
					if ((geoscript = gsc_fopen(scriptname.get_name(), _T("w"), ONETEST)) == NULL)
						AfxThrowUserException();				

					if (fwprintf(geoscript, _T("_.QUIT\n_Y\n")) < 0)
					{
						set_GS_ERR_COD(eGSWriteFile);
						AfxThrowUserException();
					}

					if (gsc_fclose(geoscript) == GS_BAD)
						AfxThrowUserException();
					
					if (acedCommandS(RTSTR, _T("_.SCRIPT"), RTSTR, scriptname.get_name(), 0) != RTNORM)	//	script per l'apertura di un DWG
						AfxThrowUserException();	// codice errore
								
					if (DeleteSettingIni() == GS_BAD)
						AfxThrowUserException();

					if (DeleteMapPostGISIni() == GS_BAD)
						AfxThrowUserException();

					if (ResetSystemVar() == GS_BAD)
						AfxThrowUserException();

					return GS_BAD;	//	TERMINA se i progetti e le classi per il passaggio sono finiti 
				}
			}
			
			//	scorre la lista e accoda gli sdf a quello creato per primo
			//	per queste operazioni si utilizza sdfld32i.exe da 
			//	copiare nella dir di installazione di GEOsim
			//	scrivere nello script il comando map_postgis per ripartire
			if ((geoscript = gsc_fopen(scriptname.get_name(), _T("w"), ONETEST)) == NULL)
				AfxThrowUserException();				
			
			if (gsc_isCurrentDWGModified())
				str = _T("_.NEW\n_Y\n.\n"); // Il comando chiede se si vuole abbandonare le modifiche
			else
				str = _T("_.NEW\n.\n");
			if (fwprintf(geoscript, str.get_name()) < 0)
			{
				set_GS_ERR_COD(eGSWriteFile); 
				AfxThrowUserException();
			}

			if (gsc_fclose(geoscript) == GS_BAD)
				AfxThrowUserException();
			
			// lancio script
			if (acedCommandS(RTSTR, _T("_.SCRIPT"), RTSTR, scriptname.get_name(), 0) != RTNORM)
				AfxThrowUserException();

			return GS_GOOD;		//	LA FUNZIONE RIPARTE
		}
	}
	CATCH( CUserException, e )
	{
		if (geoscript)
			gsc_fclose(geoscript);
		ResetSystemVar();
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

/********************************************************/
/*.doc CMapPostGIS::FindFileDate							*/
/*+
	Questa funzione ricerca un file di nome db_name 
	nel direttorio indicato da path, in caso lo trovi
	ne ottiene la data

	Parametri:
	- input
		C_STRING	path		path del direttorio
		C_STRING	name		nome file da ricercare
	- output
		C_STRING	*date		data file trovato

	Restituisce GS_BAD se non riesce a trovarlo.
-*/  
/*******************************************************/
int CMapPostGIS::FindFileDate(C_STRING path, C_STRING name, C_STRING *date)
{
	int ret = GS_BAD;

	C_STRING db_path;

	presbuf dbnames, dbdates;

	long totale;

	TRY
	{
		db_path = path;
		db_path += "\\";
		db_path += name;

		// ottengo la data del file
		totale = gsc_adir(db_path.get_name(), &dbnames, NULL, &dbdates, NULL);
 
		while ((dbnames != NULL) && (dbdates != NULL))
		{
			if ((dbnames->restype == RTSTR) && 
				(!gsc_strcmp(dbnames->resval.rstring, name.get_name(), FALSE)))
			{
				(*date) = dbdates->resval.rstring;
				ret=GS_GOOD;
				break;
			}

			if (dbnames != NULL)
				dbnames=dbnames->rbnext;
			if (dbdates != NULL)
				dbdates=dbdates->rbnext;
		}

		if (dbnames) ads_relrb(dbnames);
		if (dbdates) ads_relrb(dbdates);
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

/********************************************************/
/*.doc CMapPostGIS::FindPrjList								*/
/*+
	Questa funzione trova la lista dei progetti di GEOsim
	
	Parametri:
	- output
		presbuf     *rb   resbuf per i progetti

	Restituisce GS_BAD se fallisce il comando con 
	ads_invoke.
-*/  
/********************************************************/
int CMapPostGIS::FindPrjList(presbuf *rb)
{
	C_RB_LIST command;

	command << acutBuildList(RTSTR, _T("gs_rprj"), 0);
	if ((ads_invoke(command.get_head(), rb) == RTERROR) || (rb == NULL))
	{
		return GS_BAD;
	}			
   	return GS_GOOD;
}

/********************************************************/
/*.doc CMapPostGIS::FindFirstPrjIndex(...)					*/
/*+
	Questa funzione ricerca il primo indice valido nella
	lista dei progetti da processare.
	Memorizza il valore nella classe info.
      
	Parametri:
	- input
	- output
		CInfo	*info	informazioni sui progetti e le 
						classi

	Restituisce GS_BAD se non riesce a leggere nel file 
-*/  
/********************************************************/
int CMapPostGIS::FindFirstPrjIndex(CInfo *info)
{
	int ret = GS_GOOD;

	C_INT_INT_STR	*pItem;

	C_CLASS		*pClass;
	C_PROJECT	*pPrj = NULL;

	TRY
	{
		pItem = (C_INT_INT_STR*) classList.get_head();
		if ((pItem) && (pItem->get_key() == 0))
		{
			info->set_prj_index(pItem->get_type());
			if (schema != "")
				info->set_prj_name(schema);
			else
				info->set_prj_name(pItem->get_name());

			pItem = (C_INT_INT_STR*) classList.get_next();			
			if (pItem)
			{
				if ((FindClassIndex(pItem, info)) == GS_BAD)	// ricerca il primo indice della classe,
					AfxThrowUserException();

				if ((pClass = gsc_find_class(info->get_prj_index(), info->get_class_index(), 0)) == NULL)
					AfxThrowUserException();
				
				if (info->get_prj_path() == "")
				{
					if ((pPrj = (C_PROJECT*) pClass->ptr_id()->pPrj) == NULL)
						AfxThrowUserException();
					info->set_prj_path(pPrj->get_dir());
				}

				if (pClass->get_category() == CAT_EXTERN)
				{
					if ((FindClassIndex(pItem, info)) == GS_BAD)	// ricerca il primo indice della classe,
						AfxThrowUserException();
					info->set_sub_name(info->get_class_name());
						
					if ((pClass = gsc_find_class(info->get_prj_index(), info->get_class_index(), 0)) == NULL)
						AfxThrowUserException();
					info->set_class_name(pClass->get_name());
				}
			}
			else
				AfxThrowUserException();
		}
		else
			AfxThrowUserException();
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

/********************************************************/
/*.doc CMapPostGIS::FindClassList						*/
/*+
	Questa funzione trova la lista dei progetti di GEOsim
	
	Parametri:
	- output
		presbuf     *rb   resbuf per i progetti

	Restituisce GS_BAD se fallisce il comando con 
	ads_invoke.
-*/  
/********************************************************/
int CMapPostGIS::FindClassList(presbuf *rb, int prj)
{
	C_RB_LIST command;

	command << acutBuildList(RTSTR, _T("gs_rclass"), RTSHORT, prj, 0);
	if ((ads_invoke(command.get_head(), rb) == RTERROR) || (rb == NULL))
	{
		return GS_BAD;
	}			
   	return GS_GOOD;
}

/********************************************************/
/*.doc CMapPostGIS::FindClassIndex(...)						*/
/*+
	Questa funzione ricerca il primo indice valido nella
	lista delle classi del progetto da processare.
	Memorizza il valore nella classe info.

	Parametri:
	- input
		C_INT_INT_STR	*pItem
	- output
		CInfo	*info	informazioni sui progetti e le classi

	Restituisce GS_BAD se fallisce il command con 
	ads_invoke oppure se non trova un nuovo indice valido
-*/  
/********************************************************/
int CMapPostGIS::FindClassIndex(C_INT_INT_STR *pItem, CInfo *info)
{
	int ret = GS_GOOD;

	C_CLASS	*pClass;

	C_DBCONNECTION	*pConn;

	presbuf		pClId, pLClId;
	
	_RecordsetPtr  pRs1, pRs2;

	int			class_id, lnk_cl_id;
	C_STRING	TableRef, statement1, statement2;

	long		qty1, qty2;

	C_RB_LIST	ColValues1, ColValues2;

	C_INT		*item;

	TRY
	{
		if (geoininame == "")
			AfxThrowUserException();

		if (pItem)
		{
			if (IsValidClass(pItem->get_key(), pItem->get_type(), info))	// controllo validità classe
			{											
				info->set_class_index(pItem->get_key());
				info->set_sub_index(pItem->get_type());
				info->set_class_name(pItem->get_name());		// inserimento nome della classe
			
				if ((pClass = gsc_find_class(info->get_prj_index(), info->get_class_index(), 0)) == NULL)
					AfxThrowUserException();

				if (pClass->get_category() != CAT_SPAGHETTI)
				{											
					info->remove_all_complex();

					pConn = pClass->ptr_info()->getDBConnection(OLD);
					if (((C_PROJECT*) pClass->ptr_id()->pPrj)->getGrpsTabInfo(&pConn, &TableRef) == GS_BAD)
						AfxThrowUserException();
					
					statement1 = "SELECT CLASS_ID FROM ";
					statement1 += TableRef;
					statement1 += " WHERE LNK_CL_ID=";
					statement1 += info->get_class_index();

					// leggo le righe della tabella senza bloccarla
					if (pConn->ExeCmd(statement1, pRs1) == GS_BAD)
						AfxThrowUserException();
					if (gsc_RowsQty(pRs1, &qty1) == GS_BAD)
						AfxThrowUserException();
					if (qty1 > 0)
					{
						if (gsc_InitDBReadRow(pRs1, ColValues1) == GS_BAD)
						{ 
							gsc_DBCloseRs(pRs1);
							AfxThrowUserException();
						}
						pClId = ColValues1.CdrAssoc(_T("CLASS_ID"));

						// scorro l'elenco dei collegamenti
						while (gsc_isEOF(pRs1) == GS_BAD)
						{
							if (gsc_DBReadRow(pRs1, ColValues1) == GS_BAD) 
							{ 
								gsc_DBCloseRs(pRs1); 
								AfxThrowUserException();
							}
							// codice della classe collegata 
							if (gsc_rb2Int(pClId, &class_id) == GS_BAD) 
							{ 
								gsc_DBCloseRs(pRs1); 
								AfxThrowUserException();
							}

							statement2 = "SELECT LNK_CL_ID FROM ";
							statement2 += TableRef;
							statement2 += " WHERE CLASS_ID=";
							statement2 += class_id;
							statement2 += " AND LNK_CL_ID<>";
							statement2 += info->get_class_index();

							// leggo le righe della tabella senza bloccarla
							if (pConn->ExeCmd(statement2, pRs2) == GS_BAD)
								AfxThrowUserException();
							if (gsc_RowsQty(pRs2, &qty2) == GS_BAD)
								AfxThrowUserException();

							if (qty2 > 0)
							{
								if (gsc_InitDBReadRow(pRs2, ColValues2) == GS_BAD)
								{ 
									gsc_DBCloseRs(pRs2);
									AfxThrowUserException();
								}
								pLClId = ColValues2.CdrAssoc(_T("LNK_CL_ID"));

								// scorro l'elenco dei collegamenti
								while (gsc_isEOF(pRs2) == GS_BAD)
								{
									if (gsc_DBReadRow(pRs2, ColValues2) == GS_BAD) 
									{ 
										gsc_DBCloseRs(pRs2); 
										AfxThrowUserException();
									}
									// codice della classe collegata 
									if (gsc_rb2Int(pLClId, &lnk_cl_id) == GS_BAD) 
									{ 
										gsc_DBCloseRs(pRs2); 
										AfxThrowUserException();
									}

									item = new C_INT(lnk_cl_id);
									info->add_complex(map_info.get_map_postGis_path(), item);

									gsc_Skip(pRs2);
								}
							}

							gsc_Skip(pRs1);
						}
					}
					gsc_DBCloseRs(pRs1);
				}
			}
		}
		else
			AfxThrowUserException();
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

/******************************************************/
/*.doc CMapPostGIS::IsValidClass(...)		   		  */
/*+
	Questa funzione controlla il tipo della classe 
	ovvero:
		- NODO 
		- POLILINEA
		- TESTO
		- SPAGHETTI
		- SUPERFICIE
      
	e se è inclusa nella lista, contenuta nel file 
	<giorno>.ini, delle classi selezionate per il 
	passaggio dati.

	Parametri:
	- input
		int		cls		classe
		int		sub		sottoclasse
		C_STRING	ini		file
	- output
		CInfo		*info	informazioni sui progetti 
							e le classi

	Restituisce GS_BAD se non riesce a leggere nel file 
-*/  
/******************************************************/
int CMapPostGIS::IsValidClass(int cls, int sub, CInfo *info)
{
	int ret = GS_GOOD;

	C_CLASS *pClass;

	TRY
	{
		if ((pClass = gsc_find_class(info->get_prj_index(), cls, sub)) != NULL)
		{
			if (pClass->ptr_id()->category == CAT_SIMPLEX)
			{
				switch (pClass->ptr_id()->type)
				{
					case TYPE_NODE:
						info->set_class_type(CL_NODE);
						info->set_class_type_text("Nodo");
						break;
					case TYPE_TEXT:
						info->set_class_type(CL_TEXT);
						info->set_class_type_text("Testo");
						break;
					case TYPE_POLYLINE:
						info->set_class_type(CL_POL);
						info->set_class_type_text("Polilinea");
						break;
					case TYPE_SURFACE:
						info->set_class_type(CL_SUP);
						info->set_class_type_text("Superficie");
						break;
				}
			}
			else
			{
				if (pClass->get_category() == CAT_SPAGHETTI)
				{
					info->set_class_type(CL_SPAG);
					info->set_class_type_text("Spaghetti");
				}
				if (pClass->get_category() == CAT_GROUP)
				{
					info->set_class_type(CL_COMPL);
					info->set_class_type_text("Complessa");
				}
				if (pClass->get_category() == CAT_EXTERN)
				{
					info->set_class_type(CL_SIM);
					info->set_class_type_text("Simulazione");
				}
			}
		
   		info->set_class_dwg_path("");
         if ((pClass->get_category() != CAT_GROUP) && (pClass->get_category() != CAT_EXTERN))
            if (pClass->ptr_GphInfo())
               // Se la grafica era in formato DWG
               if (pClass->ptr_GphInfo()->getDataSourceType() == GSDwgGphDataSource)
      				info->set_class_dwg_path(((C_DWG_INFO *) pClass->ptr_GphInfo())->dir_dwg.get_name());
		}
		else
			AfxThrowUserException();
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

/******************************************************/
/*.doc CMapPostGIS::TopobaseType(...)		   		  */
/*+
	Questa funzione controlla il tipo della classe 

	Parametri:
	- input
		int		category	category
		int		type		type

	Restituisce GS_BAD se non riesce a leggere nel file 
-*/  
/******************************************************/
C_STRING CMapPostGIS::FindTopobaseType(int category, int type)
{
	C_STRING	topobase_type;

	topobase_type = "";

	switch (category)
	{
		case CAT_SIMPLEX:
			switch (type)
			{
				case TYPE_NODE:
					topobase_type = "P";
					break;
				case TYPE_TEXT:
					topobase_type = "T";
					break;
				case TYPE_POLYLINE:
					topobase_type = "L";
					break;
				case TYPE_SURFACE:
					topobase_type = "O";
					break;
			}
			break;
		case CAT_SPAGHETTI:
			topobase_type = "S";
			break;
		case CAT_GROUP:
		case CAT_EXTERN:
			topobase_type = "X";
			break;
	}

	return topobase_type;
}

int CMapPostGIS::FindSelectedClasses()
{
	int ret = GS_GOOD;

	FILE	*geoini;
	TCHAR	val[10]; 
	int		int_val;	    
	C_RB_LIST	result;	//	lista costruita leggendo in <giorno>.ini

	resbuf	*rb, *temp;		//	lista dei progetti
	presbuf	p;

	int prj, cls, sub=0;
	
	C_CLASS		*pClass;

	C_INT_INT_STR *pItem;

	TRY
	{
		if ((geoini = gsc_fopen(geoininame, _T("r"), ONETEST)) == NULL)
		{
			set_GS_ERR_COD(eGSOpenFile);
			AfxThrowUserException();
		}

		while (!feof(geoini))
		{
			fwscanf_s(geoini, _T("%s\n"), &val);
			if (!gsc_strcmp(val, _T("(")))
				result += acutBuildList(RTLB, 0); 
			else if (!gsc_strcmp(val, _T(")")))
				result += acutBuildList(RTLE, 0); 
			else if (!gsc_strcmp(val, _T("nil")))
				result += acutBuildList(RTNIL, 0); 
			else
			{
				swscanf_s(val, _T("%d"), &int_val);
				result += acutBuildList(RTSHORT, int_val,0); 
			}
		}

		if (gsc_fclose(geoini) == GS_BAD)
		{
			set_GS_ERR_COD(eGSCloseFile);
			AfxThrowUserException();
		}
				
		if (FindPrjList(&rb) == GS_BAD)		// non esistono progetti
		{	
			set_GS_ERR_COD(eGSListError); 
			AfxThrowUserException();
		}

		if ((p = result.get_head())->restype != RTLB)
			AfxThrowUserException();
		while (p->restype != RTNIL)
		{
			p = p->rbnext;
			if (p->restype != RTSHORT)
				continue;
			else
			{
				prj = (int) p->resval.rint;

				temp = rb;
				do
				{
					if ((temp == NULL) || (temp->rbnext == NULL))
						AfxThrowUserException();
					while ((temp->restype != RTLB) && (temp->rbnext != NULL) &&
							 (temp->rbnext->restype != RTSHORT))
					{
						temp = temp->rbnext;
					}
					
					if (temp->rbnext == NULL)		// fine lista o lista "vuota"
						AfxThrowUserException();
					
					if ((temp->rbnext->restype == RTSHORT) && 
						 (temp->rbnext->rbnext->restype == RTSTR))
					{
						if (temp->rbnext->resval.rint == prj)
						{
							pItem = new C_INT_INT_STR();
							pItem->set_key(0);
							pItem->set_type(prj);
							pItem->set_name(temp->rbnext->rbnext->resval.rstring);
							classList.add_tail(pItem);
							break;
						}
						else
						{
							temp = temp->rbnext;
							continue;					
						}
					}			
					else
					{
						temp = temp->rbnext;
						continue;					
					}
				}
				while(1);

				p = p->rbnext;
				
				while ((p->restype != RTLE) || (p->rbnext->restype != RTLE))
				{
					p = p->rbnext;
					if (p->restype != RTLB)
						continue;
					else
					{
						p = p->rbnext;
						if (p->restype != RTSHORT)
							continue;
						else
						{
							cls = p->resval.rint;
							sub = 0;
							p = p->rbnext;
							if (p->restype == RTSHORT)
								sub = p->resval.rint;

							if ((pClass = gsc_find_class(prj, cls, sub)) != NULL)
							{
								pItem = new C_INT_INT_STR();
								pItem->set_key(cls);
								pItem->set_type(sub);
								pItem->set_name(pClass->get_name());
								classList.add_tail(pItem);
							}
						}
					}
				}
				p = p->rbnext;
			}
		}
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}




/********************************************************/
/*.doc CMapPostGIS::FindSql								*/
/*+
	Questa funzione ricerca nella lista sqlnames il file
	sql corrispondente al DWG considerato. Se trova il 
	file sql ne ricerca la data nella lista sqldates.

	Parametri:
	- input
		presbuf	sqlnames	lista degli sql contenuti nel dir del DWG
		presbuf	sqldates	lista date degli sql
	- output
		C_STRING	*sql_date	data dell'sql

	Restituisce GS_BAD se non trova il file sql.
-*/  
/********************************************************/
int CMapPostGIS::FindSql(presbuf sqlnames, presbuf sqldates, C_STRING name, C_STRING *sql_date)
{
	C_STRING sql_filename, dwg_filename, filename;

	dwg_filename = onlyLeft(name, '.');

	while ((sqlnames!=NULL) && (sqldates!=NULL))
	{
		if (sqlnames->restype == RTSTR)
		{
			sql_filename = sqlnames->resval.rstring;
			sql_filename = onlyLeft(sql_filename, '.');

			filename = dwg_filename;
			if (map_info.get_sub_index() != 0)
			{
				filename += "_";
				filename += map_info.get_sub_name();			
			}				

			if (sql_filename.comp(filename, FALSE) == 0)	//	ha trovato il file sdf
			{
				(*sql_date) = sqldates->resval.rstring;
				return GS_GOOD;
			}

			sql_filename = onlyLeft(sql_filename, 'T');
			if (sql_filename.comp(dwg_filename, FALSE) == 0)	//	ha trovato il file sdf
			{
				(*sql_date) = sqldates->resval.rstring;
				return GS_GOOD;
			}
		}
		sqlnames = sqlnames->rbnext;
		sqldates = sqldates->rbnext;
	}
	
	return GS_BAD;
}


/**********************************************************/
/*.doc CMapPostGIS::FindDwk									  */
/*+
	Questa funzione ricerca nella lista dwknames il file 
	DWK corrispondente al DWG considerato.
	Parametri:
	- input
		presbuf	dwknames	lista dei DWK contenuti 
							nel dir del DWG

	Restituisce GS_BAD se non trova il file SDF.
-*/  
/**********************************************************/
int CMapPostGIS::FindDwk(presbuf dwknames)
{
	C_STRING dwk_filename, dwg_filename;
	
	dwg_filename = onlyLeft(dwg_name, '.');
	
	while (dwknames != NULL)
	{
		if (dwknames->restype == RTSTR)
		{
			dwk_filename = dwknames->resval.rstring;
			dwk_filename = onlyLeft(dwk_filename, '.');
			
         if (dwk_filename.comp(dwg_filename, FALSE) == 0) // trovato il file dwk
				return GS_GOOD;
		}
		dwknames = dwknames->rbnext;
	}
	return GS_BAD;
}

/********************************************************/
/*.doc CMapPostGIS::DwgListBuild(...)						*/
/*+
	per ogni dwg nella lista                    dwgnames 
	considera la data nella lista               dwgdates
	cerca il corrispondente sdf nella lista     sqlnames 
	e la corrispondente data nella lista        sqldates
	se la data del sql è anteriore a quella del dwg
	il dwg va inserito nella lista da processare
	questa lista sarà memorizzata nel file ini
-*/  
/********************************************************/
int CMapPostGIS::DwgListBuild(presbuf dwgnames, presbuf dwgdates, presbuf sqlnames, presbuf sqldates, presbuf dwknames)
{
	int ret = GS_GOOD;

	C_STRING dwg_date, sql_date;
	
	C_STRING input;

	TRY
	{
		while ((dwgnames != NULL) && (dwgdates != NULL))
		{
			if (dwgnames->restype == RTSTR)
			{
				dwg_name = dwgnames->resval.rstring;
				dwg_date = dwgdates->resval.rstring;
				
				if (FindDwk(dwknames) == GS_BAD)	// se trova il dwk corrispondente,
				{															
					// il dwg non può essere aperto
					if (FindSql(sqlnames, sqldates, dwg_name, &sql_date) == GS_BAD)	//trova il file corrispondente sql e la data
					{
						if (map_info.search_removed_dwg(dwg_name) == NULL)
							if (map_info.add_dwg(dwg_name) == GS_BAD)	// inserisce il dwg nella lista
								AfxThrowUserException();
					}
					else
					{					
						//////if (is_after(dwg_date, sdf_date) ||  
						//////	!AllSdfFiles(sqlnames, sqldates, dwg_date))
						//////	if (map_info.add_dwg(dwg_name) == GS_BAD)	// inserisce il dwg nella lista
						//////		AfxThrowUserException();
						if (is_after(dwg_date, sql_date))
							if (map_info.add_dwg(dwg_name) == GS_BAD)	// inserisce il dwg nella lista
								AfxThrowUserException();
					}
				}
				else
				{
					input = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;FILE ";
					input += dwg_name;
					input += " NON PROCESSATO per esistenza relativi file .dwk - .dwl" ;
					input += "<BR>";

					if (RegLogFile(input) != GS_GOOD)
						AfxThrowUserException();
				}
			}
			if (dwgnames != NULL)
				dwgnames = dwgnames->rbnext;
			if (dwgdates != NULL)
				dwgdates = dwgdates->rbnext;
		}
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

int CMapPostGIS::DwgListBuildNoDate(presbuf dwgnames, presbuf dwgdates, presbuf sqlnames, presbuf sqldates, presbuf dwknames)
{
	int ret = GS_GOOD;

	C_STRING dwg_date, sql_date;
	
	C_STRING input;

	TRY
	{
		while ((dwgnames != NULL) && (dwgdates != NULL))
		{
			if (dwgnames->restype == RTSTR)
			{
				dwg_name = dwgnames->resval.rstring;
				dwg_date = dwgdates->resval.rstring;
				
				if (FindDwk(dwknames) == GS_BAD)	// se trova il dwk corrispondente,
				{															
					// il dwg non può essere aperto
					if (FindSql(sqlnames, sqldates, dwg_name, &sql_date) == GS_BAD)	//trova il file corrispondente sql e la data
					{
						if (map_info.search_removed_dwg(dwg_name) == NULL)
							if (map_info.add_dwg(dwg_name) == GS_BAD)	// inserisce il dwg nella lista
								AfxThrowUserException();
					}
					else
					{					
						if (map_info.add_dwg(dwg_name) == GS_BAD)	// inserisce il dwg nella lista
							AfxThrowUserException();
					}
				}
				else
				{
					input = "&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;FILE ";
					input += dwg_name;
					input += " NON PROCESSATO per esistenza relativi file .dwk - .dwl" ;
					input += "<BR>";

					if (RegLogFile(input) != GS_GOOD)
						AfxThrowUserException();
				}
			}
			if (dwgnames != NULL)
				dwgnames = dwgnames->rbnext;
			if (dwgdates != NULL)
				dwgdates = dwgdates->rbnext;
		}
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

/********************************************************/
/*.doc CMapPostGIS::IsCurrentOpenedDwg						*/
/*+
	Verifica se il DWG aperto corrisponde al DWG della 
	classe da processare
      
	Restituisce GS_BAD se non riesce a creare questa copia
-*/  
/********************************************************/
int CMapPostGIS::IsCurrentOpenedDwg(ade_id *dwg_id)
{
	int ret = GS_GOOD;

	struct resbuf res;

	C_STRING		path;
	C_STRING	path_new;

	C_RB_LIST	p_estract_cond;

	TRY
	{
		if (ads_getvar(_T("DWGNAME"), &res) != RTNORM)
			AfxThrowUserException();

		if (gsc_strcmp(GEOPROTDWG, res.resval.rstring, FALSE) == 0) //	i nomi sono diversi
		{
			Sleep(2000);

			// effettuo l'attach
			path = map_info.get_class_dwg_path();
			path += "\\";
			path += dwg_name;
			if (gsc_ADEdrv2nethost(path.get_name(), path_new) == GS_BAD) 
				AfxThrowUserException();
			if ((*dwg_id = gsc_dsattach(path_new.get_name())) != ADE_NULLID)
				if (gsc_dwgactivate(*dwg_id) != GS_GOOD)
				{ 
					set_GS_ERR_COD(eGSDwgCannotActivate); 
					AfxThrowUserException();
				}

			// imposto condizione spaziale con ALL    // settare errore
			if ((p_estract_cond << acutBuildList(RTLB, RTSTR, _T("ALL"), RTLE, 0)) == NULL) 
				AfxThrowUserException();
			if (ade_qrydefine(_T(""), _T(""), _T(""), _T("Location"), p_estract_cond.get_head(), _T("")) == ADE_NULLID)
			{ 
				set_GS_ERR_COD(eGSQryCondNotDef); 
				AfxThrowUserException();
			}

			// setto la modalità di estrazione
			if (ade_qrysettype(_T("draw"), FALSE, _T(""), _T("")) != RTNORM) 
				AfxThrowUserException();

			// effettuo l'estrazione del disegno (sinottico)
			ade_qryexecute();

			if (acedCommandS(RTSTR, _T("_.VIEWRES"), RTSTR, _T("_Y"), RTSHORT, 20000, 0) != RTNORM)		// modifica risoluzione video
				AfxThrowUserException();
		}
		else
			AfxThrowUserException();
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

/*********************************************************/
/*.doc CMapPostGIS::ExistGeowebObjects()                     */
/*+
	Questa funzione controlla se esistono elementi di
	GEOsim nel DWG corrente
      
	Parametri: 

	Restituisce GS_BAD se non trova oggetti
-*/  
/*********************************************************/
int CMapPostGIS::ExistGeowebObjects()
{
	int ret = GS_GOOD;

	long	Key=0;
	int	class_type = map_info.get_class_type();

	C_CLASS	*pClass;
		
	ads_name	ent;
	C_SELSET	SelSet;

	long	i =0;
	int valid_ent=1;

	C_RB_LIST entmask;	// contiene la lista dei tipi di oggetto della classe
 	C_RB_LIST Attrib;	// ottiene la lista di tutti gli oggetti grafici 

	C_EED		peed;     

	TRY
	{
		if (map_info.get_sub_index())
		{
			if ((pClass = gsc_find_class(map_info.get_prj_index(), map_info.get_class_index(), map_info.get_sub_index())) == NULL)
				AfxThrowUserException();
		}
		else
		{
			if ((pClass = gsc_find_class(map_info.get_prj_index(), map_info.get_class_index(), 0)) == NULL)
				AfxThrowUserException();
		}
  
		if ((class_type == CL_NODE) || (class_type == CL_POL) || 
			(class_type == CL_SUP) || (class_type == CL_TEXT))			// classe con db
		{
			//	OCCORRE DISTINGUERE I TRE CASI (TRE MASCHERE DIVERSE)
			switch (class_type)
			{
				case CL_NODE:
					// maschera per filtrare testi e blocchi esclusi i "$T", "GEOTHM", "USRTHM"
					if ((entmask << acutBuildList(-4, _T("<and"),
													RTDXF0, _T("INSERT"),
													-4, _T("<not"),
													-4, _T("<or"),
													2, _T("$T"),
													-4, _T("<or"),
													2, _T("GEOTHM"),
													-4, _T("<or"),
													2, _T("USRTHM"),
													-4, _T("or>"),
													-4, _T("or>"),
													-4, _T("or>"),
													-4, _T("not>"),
													-4, _T("and>"), 0)) == NULL)
					{
						set_GS_ERR_COD(eGSOutOfMem);
						AfxThrowUserException();
					}
/*					
					//	maschera di selezione dei blocchi con nome 
					if ((entmask << acutBuildList(RTDXF0, _T("INSERT") ,0)) == NULL)
					{
						set_GS_ERR_COD(eGSOutOfMem);
						AfxThrowUserException();
					}
*/
					break;
				case CL_POL:
				case CL_SUP:
					if ((entmask << acutBuildList(-4, _T("<or"),
															RTDXF0, _T("POLYLINE"),
															-4, _T("<or"),
															RTDXF0, _T("LWPOLYLINE"),
															-4, _T("<or"),
															RTDXF0, _T("LINE"),
															-4, _T("or>"),
															-4, _T("or>"),
															-4, _T("or>"), 0)) == NULL)
					{ 
						set_GS_ERR_COD(eGSOutOfMem);
						AfxThrowUserException();
					}
					break;    
				case CL_TEXT:
					if ((entmask << acutBuildList(-4, _T("<or"),
															RTDXF0, _T("TEXT"),
															-4, _T("<or"),
															RTDXF0, _T("MTEXT"),
															-4, _T("or>"),
															-4, _T("or>"), 0)) == NULL)
					{ 
						set_GS_ERR_COD(eGSOutOfMem);
						AfxThrowUserException();
					}
					break;
				default:
					break;
			}
  
			if (entmask.get_head() != NULL)
			{
				//	ottiene la lista di tutti le entità nodo (nome blocco) oppure polilinea oppure superficie
				if (gsc_ssget(_T("_X"), NULL, NULL, entmask.get_head(), SelSet) != RTNORM)
					AfxThrowUserException();
			  
				valid_ent=1;

				while (SelSet.entname(i++, ent) == GS_GOOD)
				{
					//	ricavo il gs_id e lo memorizzo in key
					presbuf pRBName;

					valid_ent=1;
					//	verifica che non si tratti di un blocco "$T" nel caso classe NODO
					if (class_type == CL_NODE)
					{
						if ((Attrib << ads_entget(ent)) == NULL) 
							AfxThrowUserException();
            
						if (((pRBName = Attrib.SearchType(2)) != NULL))
						{
							//	verifica che il nome è diverso da "$T"
							if (gsc_strcmp(pRBName->resval.rstring, _T("$T")) == 0)
								valid_ent=0;
							else
								valid_ent=1;
						}
						else
							valid_ent=0;
					}

					if (valid_ent)
					{
						if (peed.load(ent) == GS_GOOD && (Key = peed.gs_id) != 0)
							break;
						else
							continue;
					}
				}
			}
		}
		else		//	se la classe è di tipo spaghetti occorre selezionare tutto 
		{
			if (gsc_ssget(_T("_X"), NULL, NULL, NULL, SelSet) != RTNORM) 
				AfxThrowUserException();
      
			while (SelSet.entname(i++, ent) == GS_GOOD)
			{
				if ((Attrib << ads_entget(ent)) != NULL)
				{
					if (peed.load(ent) == GS_GOOD)
					//if ((gs_code = Attrib.SearchType(-3)) != NULL)
   					//	controlla l'esistenza di GS
					{
						Key=1;
						break;
					}
					else
						continue;
				}
			}
		}

		if (Key == 0)
			AfxThrowUserException();
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

/********************************************************/
/*.doc CMapPostGIS::CreateAllSqls(...)						*/
/*+
	Questa funzione prepara il DWG alla creazione di 
	tutti gli SDF collegati. Scrive i comandi finali 
	in uno script
      
	Parametri:
	- input
		FILE	*script

	Restituisce GS_BAD in caso di fallimento
-*/  
/*******************************************************/
int CMapPostGIS::CreateAllSqls(FILE	*file, ade_id dwg_id)
{                                               
	int ret = GS_GOOD;

	C_STRING		oLayer;

	C_STRING input;
	
	C_STRING cat_coord_input;

	TB_Class	*tb_class;

	C_STRING		sql, str;

	long		current_id1;
	long		current_id2;

	TRY
	{
		if (ClassCoord() == GS_BAD)
			AfxThrowUserException();
		
		if (gsc_get_category_coord(map_info.get_coord_input().get_name(), cat_coord_input) == GS_BAD)
		{
			if ((map_info.get_coord_input() != "") && !(gsc_strcmp(cat_coord_input.get_name(), _T(""))))
			{			
				input = "<come digitato>";
				if (map_info.get_sub_index())
				{
					input += "progetto: ";
					input += map_info.get_prj_name();
					input += "    classe: ";
					input += map_info.get_class_name();
					input += "    sottoclasse: ";
					input += map_info.get_sub_name();
				}
				else
				{
					input = "progetto: ";
					input += map_info.get_prj_name();
					input += "    classe: ";
					input += map_info.get_class_name();
				}
				input += "\n</come digitato>";
				
				input += "Impossibile determinare la categoria delle coordinate ";
				input += map_info.get_coord_input();
				input += ", classe non passata. ";

				if (RegErrFile(input) != GS_GOOD)			
					AfxThrowUserException();

				//	aggiorna il file mapPostgis.ini
				if (UpdateMapPostGISIni() == GS_BAD)	
					AfxThrowUserException();
				
				if ((geoscript = gsc_fopen(scriptname.get_name(), _T("w"), ONETEST)) == NULL)
					AfxThrowUserException();
				
				if (fwprintf(geoscript, _T("_.QUIT\n_Y\n.\n")) < 0)
				{
					if (gsc_fclose(geoscript) == GS_BAD)
						AfxThrowUserException();
					set_GS_ERR_COD(eGSWriteFile); 
					AfxThrowUserException();
				}
  
				if (gsc_fclose(geoscript) == GS_BAD)
					AfxThrowUserException();
				
				// lancio script
				if (acedCommandS(RTSTR, _T("_.SCRIPT"), RTSTR, scriptname.get_name(), 0) != RTNORM)		// apertura DWG
					AfxThrowUserException();
			}
		}
		else
			map_info.set_cat_coord_input(cat_coord_input.get_name());

		//CL_NODE, CL_TEXT, CL_POL, CL_SUP, CL_SPAG, CL_COMPL
		tb_class = new TB_Class(&map_info, &(this->reserved_list));

		current_id1 = map_info.get_current_id1();
		current_id2 = map_info.get_current_id2();
		K = map_info.get_K();
		if (map_info.get_class_type() != CL_SPAG)
			map_info.read_fid_list(&tb_class->tb_item_list, geowork_path);

		if (tb_class->WriteSql(isOracle, file, map_info.create_table(), map_info.get_prj_name(), map_info.get_srid(), &current_id1, &current_id2, &(this->reserved_list)) == GS_BAD)
			AfxThrowUserException();

		K++;
		if (map_info.set_write_current_id1(current_id1) == GS_BAD)
			AfxThrowUserException();
		if (map_info.set_write_current_id2(current_id2) == GS_BAD)
			AfxThrowUserException();
		if (map_info.set_write_K(K) == GS_BAD)
			AfxThrowUserException();

		if (gsc_dsdetach(dwg_id) == GS_BAD)
			AfxThrowUserException();

		if (map_info.remove_last_dwg() == GS_BAD)
			AfxThrowUserException();

		if (map_info.set_write_sql(1) == GS_BAD)
			AfxThrowUserException();

		if ((map_info.total_dwg() > 1) && (map_info.get_class_type() != CL_SPAG))
			if (map_info.write_fid_list(&tb_class->tb_item_list, geowork_path) == GS_BAD)
				AfxThrowUserException();

		if (map_info.count_dwg() == 0)
			map_info.remove_fid_list(geowork_path);

		if ((geoscript = gsc_fopen(scriptname.get_name(), _T("w"), ONETEST)) == NULL)
			AfxThrowUserException();

		if (gsc_isCurrentDWGModified())
			str = _T("_.NEW\n_Y\n.\n"); // Il comando chiede se si vuole abbandonare le modifiche
		else
			str = _T("_.NEW\n.\n");
		if (fwprintf(geoscript, str.get_name())<0)
		{
			if (gsc_fclose(geoscript) == GS_BAD)
				AfxThrowUserException();
			set_GS_ERR_COD(eGSWriteFile); 
			AfxThrowUserException();
		}

		//	chiude il file dello script
		if (gsc_fclose(geoscript) == GS_BAD)
			AfxThrowUserException();

		if (tb_class)
			delete tb_class;

//		Da utilizzare per bloccare il processo
//		ricercare la lettera count (maiuscolo)
		if (K < 30)
		{
			//	lancio script per la creazione degli SDF e la funzione riparte
			if (acedCommandS(RTSTR, _T("_.SCRIPT"), RTSTR, scriptname.get_name(), 0) != RTNORM)
				AfxThrowUserException();
		}
		else
		{
			K = 0;
			if (map_info.set_write_K(K) == GS_BAD)
				AfxThrowUserException();

			if ((geoscript = gsc_fopen(scriptname.get_name(), _T("w"), ONETEST)) == NULL)
				AfxThrowUserException();
					
			if (fwprintf(geoscript, _T("_.QUIT\n_Y\n")) < 0)
			{
				set_GS_ERR_COD(eGSWriteFile);
				AfxThrowUserException();
			}

			if (gsc_fclose(geoscript) == GS_BAD)
				AfxThrowUserException();

			//	lancio script per la chiusura di AutoCAD
			if (acedCommandS(RTSTR, _T("_.SCRIPT"), RTSTR, scriptname.get_name(), 0) != RTNORM)
				AfxThrowUserException();
		}
	}
	CATCH( CUserException, e )
	{
		ResetSystemVar();
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}   

/********************************************************/
/*.doc int CMapPostGIS::UpdateMapPostGISIni(...)				*/
/*+
	L'aggiornamento del file mapPostgis.ini va fatto solo 
	se è esaurita la lista dei DWG da processare

	Restituisce GS_BAD in caso di fallimento
-*/  
/*******************************************************/
int CMapPostGIS::UpdateMapPostGISIni()
{
	int ret = GS_GOOD;

	int old_prj_index = map_info.get_prj_index();
	int old_class_index = map_info.get_class_index();
	int old_sub_index = map_info.get_sub_index();

	C_INT_INT_STR	*pItem;

	C_CLASS		*pClass;

	CInfo map_temp;

	C_STRING	alias;
	C_STRING		input, alias_path, dest_file, total_file, sql_dest_file, ini;
	int			pos6;

	FILE		*totalFile;

	C_STRING		db;

	TRY
	{
		if (FindSelectedClasses() == GS_BAD)
			AfxThrowUserException();

		pItem = (C_INT_INT_STR*) classList.search(0, old_prj_index);
		if (pItem)
		{
			pItem = (C_INT_INT_STR*) classList.search(old_class_index, old_sub_index);
			pItem = (C_INT_INT_STR*) classList.get_next();
			if (pItem)
			{
				if (pItem->get_key() == 0)
				{
   					map_info.set_prj_index(pItem->get_type());
					if (schema != "")
	   					map_info.set_prj_name(schema);
					else
	   					map_info.set_prj_name(pItem->get_name());
					pItem = (C_INT_INT_STR*) classList.get_next();
					if (pItem)
						if ((FindClassIndex(pItem, &map_info)) == GS_BAD)	// ricerca il primo indice della classe,
							AfxThrowUserException();
				}
				else
				{
					if ((FindClassIndex(pItem, &map_info)) == GS_BAD)	// ricerca il primo indice della classe
						AfxThrowUserException();
					
					if (map_info.get_sub_index() != 0)
					{
						map_info.set_sub_name(map_info.get_class_name());

						if ((pClass = gsc_find_class(map_info.get_prj_index(), map_info.get_class_index(), 0)) == NULL)
							AfxThrowUserException();
						map_info.set_class_name(pClass->get_name());
					}
					else
					{
						if (map_info.get_class_type() == CL_SIM)
						{
							pItem = (C_INT_INT_STR*) classList.get_next();			
							if (pItem)
							{
								if ((FindClassIndex(pItem, &map_info)) == GS_BAD)	// ricerca il primo indice della classe,
									AfxThrowUserException();
								map_info.set_sub_name(map_info.get_class_name());

								if ((pClass = gsc_find_class(map_info.get_prj_index(), map_info.get_class_index(), 0)) == NULL)
									AfxThrowUserException();
								map_info.set_class_name(pClass->get_name());
							}
						}
					}
				}
			}
			else
			{
				// converte il direttorio con l'alias
				alias = map_info.get_prj_path();
				if (gsc_drive2nethost(alias) == GS_BAD) 
					AfxThrowUserException();
				alias_path = alias.get_name();
				eraseMinorAndMajor(&alias_path);

				dest_file = postGis_path;
				dest_file += "/sql/";
				dest_file += alias_path;

				pos6 = geoininame.reverseFind(_T('/'));
				ini = geoininame.right(geoininame.len() - pos6 - 1);
				ini.strtran(_T(".ini"), _T(""));

				sql_dest_file = dest_file;			
				sql_dest_file += "/grant.sql";

				sql_dest_file.strtran(_T("\\"), _T("/"));

				total_file = dest_file;
				total_file += "/";
				//total_file += "totale.bat";
				total_file += "totale_";
				total_file += ini;
				total_file += ".bat";

				if ((totalFile = gsc_fopen(total_file, _T("a"), ONETEST)) == NULL)
					AfxThrowUserException();

				if (map_info.get_database() == "")
				{
					db = _T("geoweb_");
					db += map_info.get_prj_name();
				}
				else
					db = map_info.get_database();

				if (server == "")
				{
					//if (fwprintf(totalFile, _T("%s %s %s %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-f"), sql_dest_file, _T("geoweb_dati")) < 0)
					if (fwprintf(totalFile, _T("%s %s %s %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-f"), sql_dest_file, db) < 0)
					{
						if (gsc_fclose(totalFile) == GS_BAD)
							AfxThrowUserException();
						AfxThrowUserException();
					}
				}
				else
				{
					//if (fwprintf(totalFile, _T("%s %s %s %s %s %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-h"), server, _T("-f"), sql_dest_file, _T("geoweb_dati")) < 0)
					if (fwprintf(totalFile, _T("%s %s %s %s %s %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-h"), server, _T("-f"), sql_dest_file, db) < 0)
					{
						if (gsc_fclose(totalFile) == GS_BAD)
							AfxThrowUserException();
						AfxThrowUserException();
					}
				}

				if (gsc_fclose(totalFile) == GS_BAD)
					AfxThrowUserException();

	
				input = "<FONT COLOR=\"green\"> Fine passaggio dati. </FONT><BR>";
				if (RegLogFile(input) != GS_GOOD)
					AfxThrowUserException();				
				
				setting_info.set_write_finish(TRUE);
				
				acutPrintf(_T("\nFine passaggio dati PostGIS.\n"));
				AfxThrowUserException();		// TERMINA (non è errore)			
			}
		}

		map_temp.set_db_prj(map_info.get_db_prj());
		map_temp.set_prj_index(map_info.get_prj_index());
		map_temp.set_prj_name(map_info.get_prj_name());
		map_temp.set_prj_path(map_info.get_prj_path());

		map_temp.set_class_index(map_info.get_class_index());
		map_temp.set_sub_index(map_info.get_sub_index());
		map_temp.set_class_type(map_info.get_class_type());
		map_temp.set_class_type_text(map_info.get_class_type_text());
		map_temp.set_class_name(map_info.get_class_name());
		map_temp.set_sub_name(map_info.get_sub_name());
		
		map_temp.set_class_dwg_path(map_info.get_class_dwg_path());

		map_temp.set_sql(0);

		//	gli indici vengono salvati nel file
		if (map_info.writeFile(&map_temp) == GS_BAD)
			AfxThrowUserException();		

		if (map_info.remove_all_dwg() == GS_BAD)
			AfxThrowUserException();		

		setting_info.set_write_first(TRUE);
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

/********************************************************/
/*.doc int CMapPostGIS::ClassCoord()						*/
/*+
	Questa funzione trova, se esiste, il sistema di 
	coordinate assegnato alla classe
      
	Parametri:

	Restituisce GS_BAD se fallisce il command con 
	ads_invoke
-*/  
/********************************************************/
int CMapPostGIS::ClassCoord()
{
	int ret = GS_GOOD;

	C_RB_LIST command;
	presbuf rb;

	TRY
	{
  		if (map_info.get_sub_index())
		{
			command << acutBuildList(RTSTR, _T("gs_get_fas"), 
									 RTSHORT, map_info.get_prj_index(), 
									 RTSHORT, map_info.get_class_index(), 
									 RTSHORT, map_info.get_sub_index(), 0);
		}
		else
		{
			command << acutBuildList(RTSTR, _T("gs_get_fas"),
									 RTSHORT, map_info.get_prj_index(),
									 RTSHORT, map_info.get_class_index(), 0);
		}

		if ((ads_invoke(command.get_head(), &rb) == RTERROR) || (rb == NULL))
		{
			ads_relrb(rb);
			rb = NULL;
			AfxThrowUserException();
		}
		
		for (int i=0; i<14; i++)
		{
			rb=rb->rbnext;
		}
		
		map_info.set_coord_input(rb->resval.rstring);
		ads_relrb(rb);
		rb = NULL;
		
		if ((map_info.get_coord_input()).len() == 0)  
			map_info.set_coord_input(coord_info.get_coord_input());		// sistema di coordinate di default
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

int CMapPostGIS::PathWithExt(C_STRING *path, C_STRING ext)
{
	int ret = GS_GOOD;

	TCHAR *cod36prj=NULL, *cod36cls=NULL;
	C_STRING temp;

	TRY
	{
		if(gsc_long2base36(&cod36prj, map_info.get_prj_index(), 2) == GS_BAD)
			AfxThrowUserException();
		if(gsc_long2base36(&cod36cls, map_info.get_class_index(), 3) == GS_BAD)
		{ 
			if (cod36prj) 
				free(cod36prj); 
			AfxThrowUserException();
		}
		
		// in temp la path dei dwg della classe
		temp = map_info.get_class_dwg_path();
		temp += '\\';
		temp += cod36prj;
		temp += cod36cls;

		*path = temp;	
		if (ext != "") 
		{	
			*path += "*.";
			*path += ext;
		}
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

/************************************************************
/*.doc	int CMapPostGIS::ExecSecExt()						*/
/*+
	Questa funzione esegue l'operazione di trasformazione
	delle tabelle secondarie esterne.
   
  	Parametri:             
    
	GS_GOOD in caso di successo altrimenti GS_BAD.
-*/
/************************************************************/
int CMapPostGIS::ExecSecExt()
{
	int		ret=GS_GOOD; 

	TRY
	{
		if (FindGeowork() == GS_BAD)	// setta gs_path alla directory di GEOWORK
			AfxThrowUserException();

		if (FindGeosim() == GS_BAD)		// setta gs_path alla directory di GEOSIM
			AfxThrowUserException();

		if (FindPostGIS() == GS_BAD)	// setta postGis_path alla directory di POSTGIS
			AfxThrowUserException();

		if (SecExtExist() == GS_BAD)
			AfxThrowUserException();
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

int CMapPostGIS::SecExtExist()
{
	int ret = GS_GOOD;

	C_STRING	alias;
	C_STRING		alias_path, dest_file, sql_db, sql_dest_file, sql_file, total_file, ini, str2;
	
	FILE	*sqlFile, *destFile, *totalFile;

	int old_prj_index = 0;
	int old_class_index = 0;
	int old_sub_index = 0;

	C_CLASS			*pClass;
	C_INT_INT_STR	*pItem;

	int		pos6;
	int		i;

	C_STRING	db;

	TRY
	{
		if (FindSelectedClasses() == GS_BAD)
			AfxThrowUserException();

		pItem = (C_INT_INT_STR*) classList.get_head();
		if ((pItem) && (pItem->get_key() == 0))
		{
			map_info.set_prj_index(pItem->get_type());
			if (schema != "")
				map_info.set_prj_name(schema);
			else
				map_info.set_prj_name(pItem->get_name());
			old_prj_index = pItem->get_type();

			i = 0;
			pItem = (C_INT_INT_STR*) classList.get_next();
			while (pItem)
			{
				if ((FindClassIndex(pItem, &map_info)) == GS_BAD)	// ricerca il primo indice della classe,
					AfxThrowUserException();
				old_class_index = map_info.get_class_index();
				if ((pClass = gsc_find_class(map_info.get_prj_index(), map_info.get_class_index(), 0)) == NULL)
					AfxThrowUserException();
				map_info.set_prj_path(((C_PROJECT*) pClass->ptr_id()->pPrj)->get_dir());

            // Imposto la cartella del progetto che viene variata in cartella dei disegni della classe
            // se la classe ha sorgente grafica in formato DWG
            sql_file = ((C_PROJECT*) pClass->ptr_id()->pPrj)->get_dir();
            if (pClass->ptr_GphInfo())
               // Se la grafica era in formato DWG
               if (pClass->ptr_GphInfo()->getDataSourceType() == GSDwgGphDataSource)
      				sql_file = ((C_DWG_INFO *) pClass->ptr_GphInfo())->dir_dwg.get_name();

				sql_file += "\\";
				sql_file += pClass->get_name();
				sql_file += "_SecsExt.sql";

				if ((sqlFile = gsc_fopen(sql_file, _T("w"), ONETEST)) == NULL)
					AfxThrowUserException();				
			
				if (CreateAllSecExt(sqlFile) == GS_BAD)
					AfxThrowUserException();

				if (gsc_fclose(sqlFile) == GS_BAD)
					AfxThrowUserException();


				// converte il direttorio con l'alias
				alias = map_info.get_prj_path();
				if (gsc_drive2nethost(alias) == GS_BAD) 
					AfxThrowUserException();
				alias_path = alias.get_name();
				eraseMinorAndMajor(&alias_path);

				dest_file = postGis_path;
				dest_file += "/sql/";
				dest_file += alias_path;

				//	eventuale creazione direttorio
				if (gsc_mkdir(dest_file) == GS_BAD)
					AfxThrowUserException();

				pos6 = geoininame.reverseFind(_T('/'));
				ini = geoininame.right(geoininame.len() - pos6 -1);
				ini.strtran(_T(".ini"), _T(""));

				total_file = dest_file;
				total_file += "/";
				total_file += "totaleSecsExt_";
				total_file += ini;
				total_file += ".bat";

				sql_dest_file = dest_file;			
				sql_dest_file += "/";
				sql_dest_file += pClass->get_name();
				sql_dest_file += "_SecsExt.sql";

				if (map_info.get_sub_index() != 0)
				{
					sql_dest_file += "_";
					sql_dest_file += map_info.get_sub_name();
				}
				// copio il file
				if (gsc_copyfile(sql_file, sql_dest_file) == GS_BAD)
					AfxThrowUserException();

				if (i == 0) 
				{
					if ((totalFile = gsc_fopen(total_file, _T("w"), ONETEST)) == NULL)
						AfxThrowUserException();
				}
				else
				{
					if ((totalFile = gsc_fopen(total_file, _T("a"), ONETEST)) == NULL)
						AfxThrowUserException();
				}

				sql_dest_file.strtran(_T("\\"), _T("/"));
				sql_db = map_info.get_prj_name();

				if (database == "")
				{
					db = _T("geoweb_");
					db += map_info.get_prj_name();
				}
				else
					db = database;

				if (server == "")
				{
					//if (fwprintf(totalFile, _T("%s %s %s %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-f"), sql_dest_file, _T("geoweb_dati")) < 0)
					if (fwprintf(totalFile, _T("%s %s %s %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-f"), sql_dest_file, db) < 0)
					{
						if (gsc_fclose(totalFile) == GS_BAD)
							AfxThrowUserException();
						AfxThrowUserException();
					}
				}
				else
				{
					//if (fwprintf(totalFile, _T("%s %s %s %s %s %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-h"), server, _T("-f"), sql_dest_file, _T("geoweb_dati")) < 0)
					if (fwprintf(totalFile, _T("%s %s %s %s %s %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-h"), server, _T("-f"), sql_dest_file, db) < 0)
					{
						if (gsc_fclose(totalFile) == GS_BAD)
							AfxThrowUserException();
						AfxThrowUserException();
					}
				}

				if (gsc_fclose(totalFile) == GS_BAD)
					AfxThrowUserException();

				pItem = (C_INT_INT_STR*) classList.get_next();
				i++;
			}
		
			pos6 = geoininame.reverseFind(_T('/'));
			ini = geoininame.right(geoininame.len() - pos6 -1);
			ini.strtran(_T(".ini"), _T(""));

			sql_dest_file = dest_file;
			sql_dest_file += "/grant.sql";
			sql_dest_file.strtran(_T("\\"), _T("/"));

			if ((destFile = gsc_fopen(sql_dest_file, _T("w"), ONETEST)) == NULL)
				AfxThrowUserException();

			str2 = "BEGIN;\n";
			str2 += "SELECT gw_grantselect('mapserver', '";
			str2 += map_info.get_prj_name();
			str2 += "', 1);\n";
			str2 += "COMMIT;\n";
 
			if (fwprintf(destFile, _T("%s"), str2) < 0)
			{
				if (gsc_fclose(destFile) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}

			if (gsc_fclose(destFile) == GS_BAD)
				AfxThrowUserException();

			if ((totalFile = gsc_fopen(total_file, _T("a"), ONETEST)) == NULL)
				AfxThrowUserException();

			if (database == "")
			{
				db = _T("geoweb_");
				db += map_info.get_prj_name();
			}
			else
				db = database;

			if (server == "")
			{
				//if (fwprintf(totalFile, _T("%s %s %s %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-f"), sql_dest_file, _T("geoweb_dati")) < 0)
				if (fwprintf(totalFile, _T("%s %s %s %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-f"), sql_dest_file, db) < 0)
				{
					if (gsc_fclose(totalFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
			}
			else
			{
				//if (fwprintf(totalFile, _T("%s %s %s %s %s %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-h"), server, _T("-f"), sql_dest_file, _T("geoweb_dati")) < 0)
				if (fwprintf(totalFile, _T("%s %s %s %s %s %s \"%s\" %s\n"), _T("psql"), _T("-U"), user, _T("-h"), server, _T("-f"), sql_dest_file, db) < 0)
				{
					if (gsc_fclose(totalFile) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
			}

			if (gsc_fclose(totalFile) == GS_BAD)
				AfxThrowUserException();
		}

		scriptname = geowork_path;
		scriptname += POSTGIS;
		scriptname += "\\GEOSCRIP0";
		scriptname += ".SCR";

		if ((geoscript = gsc_fopen(scriptname.get_name(), _T("w"), ONETEST)) == NULL)
			AfxThrowUserException();				

		if (fwprintf(geoscript, _T("_.QUIT\n_Y\n")) < 0)
		{
			set_GS_ERR_COD(eGSWriteFile);
			AfxThrowUserException();
		}

		if (gsc_fclose(geoscript) == GS_BAD)
			AfxThrowUserException();
					
		if (acedCommandS(RTSTR, _T("_.SCRIPT"), RTSTR, scriptname.get_name(), 0) != RTNORM)	//	script per l'apertura di un DWG
			AfxThrowUserException();	// codice errore
	}
	CATCH( CUserException, e )
	{
		if (geoscript)
			gsc_fclose(geoscript);
		if (sqlFile)
			gsc_fclose(sqlFile);
		ResetSystemVar();
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

/********************************************************/
/*.doc CMapPostGIS::CreateAllSecExt(...)				*/
/*+
	Parametri:
	- input
		FILE	*script

	Restituisce GS_BAD in caso di fallimento
-*/  
/*******************************************************/
int CMapPostGIS::CreateAllSecExt(FILE *file)
{                                               
	int ret = GS_GOOD;

	TB_Class	*tb_class;

	TRY
	{
		//CL_NODE, CL_TEXT, CL_POL, CL_SUP, CL_SPAG, CL_COMPL
		tb_class = new TB_Class(&map_info, &(this->reserved_list));

		if (tb_class->WriteSecExt(isOracle, file, map_info.get_prj_name(), &(this->reserved_list)) == GS_BAD)
			AfxThrowUserException();

		if (tb_class)
			delete tb_class;
	}
	CATCH( CUserException, e )
	{
		ResetSystemVar();
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}   