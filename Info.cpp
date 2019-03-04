/*************************************************************************/
/* INCLUDES                                                              */
/*************************************************************************/
#include "stdafx.h"

#include "info.h"


CInfo::CInfo() 
{  
	db_prj = 0;
	prj_index = 0;
	// CLASS
	class_index = 0;
	sub_index = 0;
	class_type = CL_NONE;
	// SCRIPT
	script_number = 0;
	// DWG
	dwg_total = 0;
	dwg_name_list.RemoveAll();
	removed_dwg_name_list.RemoveAll();
	current_id1 = 0;
	current_id2 = 0;
	K = 0;

	// SQL
	sql = 0;
	// LAY
	layer_name_list.RemoveAll();

	map_ini = NULL;

	// COMPLEX
	complex_list.remove_all();
	complex = false;
}

CInfo::~CInfo() 
{  
}

FILE *CInfo::openFile(C_STRING path) 
{  
	map_postGis_path = path;
	map_postGis_path += POSTGIS;
	map_postGis_path += "\\mapPostgis.ini";

	map_ini = gsc_open_profile(map_postGis_path, READONLY);
	return map_ini;
}

int CInfo::closeFile() 
{ 
	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;

	null_mapsql_ini();
	return GS_GOOD;
}

int CInfo::readFile() 
{  
	if ((map_ini = gsc_open_profile(map_postGis_path, READONLY)) == NULL)
		return GS_BAD;

	TCHAR *result=NULL;

	TRY
	{
		if (get_int_value(map_ini, _T("PRJ"), _T("DB_PRJ"), &db_prj) == GS_BAD)
			AfxThrowUserException();
		if (get_int_value(map_ini, _T("PRJ"), _T("PRJ_INDEX"), &prj_index) == GS_BAD)
			AfxThrowUserException();
		if (gsc_get_profile(map_ini, _T("PRJ"), _T("PRJ_NAME"), &result, 0, 0) == GS_BAD)
			AfxThrowUserException();
		prj_name = result;
		free (result);
		result = NULL;
		if (gsc_get_profile(map_ini, _T("PRJ"), _T("PRJ_PATH"), &result, 0, 0) == GS_BAD)
			AfxThrowUserException();
		prj_path = result;
		free (result);
		result = NULL;

		if (get_int_value(map_ini, _T("CLASS"), _T("CLASS_INDEX"), &class_index) == GS_BAD)
			AfxThrowUserException();
		if (get_int_value(map_ini, _T("CLASS"), _T("SUB_INDEX"), &sub_index) == GS_BAD)
			AfxThrowUserException();
		if (get_int_value(map_ini, _T("CLASS"), _T("CLASS_TYPE"), &class_type) == GS_BAD)
			AfxThrowUserException();
		if (gsc_get_profile(map_ini, _T("CLASS"), _T("CLASS_TYPE_TEXT"), &result, 0, 0) == GS_BAD)
			AfxThrowUserException();
		class_type_text = result;
		free (result);
		result = NULL;
		if (gsc_get_profile(map_ini, _T("CLASS"), _T("CLASS_NAME"), &result, 0, 0) == GS_BAD)
			AfxThrowUserException();
		class_name = result;
		free (result);
		result = NULL;
		if (gsc_get_profile(map_ini, _T("CLASS"), _T("SUB_NAME"), &result, 0, 0) == GS_BAD)
			AfxThrowUserException();
		sub_name = result;
		free (result);
		result = NULL;
		if (gsc_get_profile(map_ini, _T("CLASS"), _T("CLASS_DWG_PATH"), &result, 0, 0) == GS_BAD)
			AfxThrowUserException();
		class_dwg_path = result;
		free (result);
		result = NULL;
		if (get_int_value(map_ini, _T("SCRIPT"), _T("SCRIPT_NUMBER"), &script_number) == GS_BAD)
			AfxThrowUserException();
		if (gsc_get_profile(map_ini, _T("INI"), _T("INI_PATH"), &result, 0, 0) == GS_BAD)
			AfxThrowUserException();
		ini_path = result;
		free (result);
		result = NULL;
		if (gsc_get_profile(map_ini, _T("INI"), _T("SERVER"), &result, 0, 0) == GS_BAD)
			AfxThrowUserException();
		server = result;
		free (result);
		result = NULL;
		if (gsc_get_profile(map_ini, _T("INI"), _T("USER"), &result, 0, 0) == GS_BAD)
			AfxThrowUserException();
		user = result;
		free (result);
		result = NULL;
		if (gsc_get_profile(map_ini, _T("INI"), _T("FONT"), &result, 0, 0) == GS_BAD)
			AfxThrowUserException();
		font = result;
		free (result);
		result = NULL;
		if (gsc_get_profile(map_ini, _T("INI"), _T("SCHEMA"), &result, 0, 0) == GS_BAD)
			AfxThrowUserException();
		schema = result;
		free (result);
		result = NULL;
		if (gsc_get_profile(map_ini, _T("INI"), _T("DATABASE"), &result, 0, 0) == GS_BAD)
			AfxThrowUserException();
		database = result;
		free (result);
		result = NULL;
		if (gsc_get_profile(map_ini, _T("INI"), _T("SRID"), &result, 0, 0) == GS_BAD)
			AfxThrowUserException();
		srid = result;
		free (result);
		result = NULL;
		if (gsc_get_profile(map_ini, _T("INI"), _T("INF"), &result, 0, 0) == GS_BAD)
			AfxThrowUserException();
		inf = result;
		free (result);
		result = NULL;
		if (gsc_get_profile(map_ini, _T("INI"), _T("ISORACLE"), &result, 0, 0) == GS_BAD)
			AfxThrowUserException();
		isOracle = result;
		free (result);
		result = NULL;
	
		if (readSql() == GS_BAD)
			AfxThrowUserException();	

		if (readLayer_name() == GS_BAD)
			AfxThrowUserException();	

		if (readDwg() == GS_BAD)
			AfxThrowUserException();	

		if (readRemovedDwg() == GS_BAD)
			AfxThrowUserException();	

		if (readComplex() == GS_BAD)
			AfxThrowUserException();	
	}
	CATCH( CUserException, e )
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}
	END_CATCH

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::writeFile() 
{
   C_STRING ValueStr;

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	TRY
	{
      ValueStr = db_prj;
		if (gsc_set_profile(map_ini, _T("PRJ"), _T("DB_PRJ"), ValueStr.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
      ValueStr = prj_index;
		if (gsc_set_profile(map_ini, _T("PRJ"), _T("PRJ_INDEX"), ValueStr.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
		if (gsc_set_profile(map_ini, _T("PRJ"), _T("PRJ_NAME"), prj_name.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
		if (gsc_set_profile(map_ini, _T("PRJ"), _T("PRJ_PATH"), prj_path.get_name(), 0) == GS_BAD)
			AfxThrowUserException();		
      ValueStr = class_index;
		if (gsc_set_profile(map_ini, _T("CLASS"), _T("CLASS_INDEX"), ValueStr.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
      ValueStr = sub_index;
		if (gsc_set_profile(map_ini, _T("CLASS"), _T("SUB_INDEX"), ValueStr.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
      ValueStr = class_type;
		if (gsc_set_profile(map_ini, _T("CLASS"), _T("CLASS_TYPE"), ValueStr.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
		if (gsc_set_profile(map_ini, _T("CLASS"), _T("CLASS_TYPE_TEXT"), class_type_text.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
		if (gsc_set_profile(map_ini, _T("CLASS"), _T("CLASS_NAME"), class_name.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
		if (gsc_set_profile(map_ini, _T("CLASS"), _T("SUB_NAME"), sub_name.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
		if (gsc_set_profile(map_ini, _T("CLASS"), _T("CLASS_DWG_PATH"), class_dwg_path.get_name(), 0) == GS_BAD)
			AfxThrowUserException();

      ValueStr = script_number;
		if (gsc_set_profile(map_ini, _T("SCRIPT"), _T("SCRIPT_NUMBER"), ValueStr.get_name(), 0) == GS_BAD)
			AfxThrowUserException();

		if (gsc_set_profile(map_ini, _T("INI"), _T("INI_PATH"), ini_path.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
		if (gsc_set_profile(map_ini, _T("INI"), _T("SERVER"), server.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
		if (gsc_set_profile(map_ini, _T("INI"), _T("USER"), user.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
		if (gsc_set_profile(map_ini, _T("INI"), _T("FONT"), font.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
		if (gsc_set_profile(map_ini, _T("INI"), _T("SCHEMA"), schema.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
		if (gsc_set_profile(map_ini, _T("INI"), _T("DATABASE"), database.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
		if (gsc_set_profile(map_ini, _T("INI"), _T("SRID"), srid.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
		if (gsc_set_profile(map_ini, _T("INI"), _T("INF"), inf.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
		if (gsc_set_profile(map_ini, _T("INI"), _T("ISORACLE"), isOracle.get_name(), 0) == GS_BAD)
			AfxThrowUserException();

      ValueStr = current_id1;
		if (gsc_set_profile(map_ini, _T("DWG"), _T("CURRENT_ID1"), ValueStr.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
      ValueStr = current_id2;
		if (gsc_set_profile(map_ini, _T("DWG"), _T("CURRENT_ID2"), ValueStr.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
      ValueStr = K;
		if (gsc_set_profile(map_ini, _T("DWG"), _T("K"), ValueStr.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
	}
	CATCH( CUserException, e )
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}
	END_CATCH

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::writeFile(CInfo *info) 
{  
	if (set_write_db_prj(info->get_db_prj()) == GS_BAD)
		return GS_BAD;

	if (set_write_prj_index(info->get_prj_index()) == GS_BAD)
		return GS_BAD;

	if (set_write_prj_name(info->get_prj_name()) == GS_BAD)
		return GS_BAD;

	if (set_write_prj_path(info->get_prj_path()) == GS_BAD)
		return GS_BAD;

	if (set_write_class_index(info->get_class_index()) == GS_BAD)
		return GS_BAD;

	if (set_write_sub_index(info->get_sub_index()) == GS_BAD)
		return GS_BAD;

	if (set_write_class_type(info->get_class_type()) == GS_BAD)
		return GS_BAD;

	if (set_write_class_type_text(info->get_class_type_text()) == GS_BAD)
		return GS_BAD;

	if (set_write_class_name(info->get_class_name()) == GS_BAD)
		return GS_BAD;
	
	if (set_write_sub_name(info->get_sub_name()) == GS_BAD)
		return GS_BAD;

	if (set_write_class_dwg_path(info->get_class_dwg_path()) == GS_BAD)
		return GS_BAD;

	if (set_write_script_number(info->get_script_number()) == GS_BAD)
		return GS_BAD;

	return GS_GOOD;
}


int CInfo::clear() 
{  
	CInfo temp;

	if (writeFile(&temp) == GS_BAD)
		return GS_BAD;

	return GS_GOOD;
}

int CInfo::get_db_prj() 
{  
	return db_prj;
}

int CInfo::get_prj_index() 
{  
	return prj_index;
}

C_STRING CInfo::get_prj_name() 
{  
	return prj_name;
}

C_STRING CInfo::get_prj_path() 
{  
	return prj_path;
}

int CInfo::get_class_index() 
{  
	return class_index;
}

int CInfo::get_sub_index() 
{  
	return sub_index;
}

int CInfo::get_class_type() 
{  
	return class_type;
}

C_STRING CInfo::get_class_type_text() 
{  
	return class_type_text;
}

C_STRING CInfo::get_class_name() 
{  
	return class_name;
}

C_STRING CInfo::get_sub_name() 
{  
	return sub_name;
}

C_STRING CInfo::get_class_dwg_path() 
{  
	return class_dwg_path;
}

int CInfo::get_sql() 
{  
	return sql;
}

int CInfo::get_current_id1()
{
	return current_id1;
}

int CInfo::get_current_id2()
{
	return current_id2;
}

int CInfo::get_K()
{
	return K;
}

int CInfo::get_script_number() 
{  
	return script_number;
}

C_STRING CInfo::get_ini_path() 
{  
	return ini_path;
}

C_STRING CInfo::get_server() 
{  
	return server;
}

C_STRING CInfo::get_user() 
{  
	return user;
}

C_STRING CInfo::get_font() 
{  
	return font;
}

C_STRING CInfo::get_schema() 
{  
	return schema;
}

C_STRING CInfo::get_database() 
{  
	return database;
}

long CInfo::get_srid() 
{  
	return srid.tol();
}

bool CInfo::get_inf() 
{  
	if (inf == "1")
		return true;
	else
		return false;
}

bool CInfo::get_isOracle() 
{  
	if (isOracle == "true")
		return true;
	else
		return false;
}

C_STRING CInfo::get_coord_input() 
{  
	return coord_input;
}

C_STRING CInfo::get_cat_coord_input() 
{  
	return cat_coord_input;
}

bool CInfo::get_complex() 
{  
	return complex;
}

void CInfo::set_db_prj(int i)
{
	db_prj = i;
}

void CInfo::set_prj_index(int i)
{
	prj_index = i;
}

void CInfo::set_prj_name(C_STRING s)
{
	prj_name = correctString1(s).tolower();
}

void CInfo::set_prj_path(C_STRING s)
{
	prj_path = s;
}

void CInfo::set_class_index(int i)
{
	class_index = i;
}

void CInfo::set_sub_index(int i)
{
	sub_index = i;
}

void CInfo::set_class_type(int i)
{
	class_type = i;
}

void CInfo::set_class_type_text(C_STRING s)
{
	class_type_text = s;
}

void CInfo::set_class_name(C_STRING s)
{
	class_name = s;
}

void CInfo::set_sub_name(C_STRING s)
{
	sub_name = s;
}

void CInfo::set_class_dwg_path(C_STRING s)
{
	class_dwg_path = s;
}

void CInfo::set_script_number(int i)
{
	script_number = i;
}

void CInfo::set_ini_path(C_STRING s)
{
	ini_path = s;
}

void CInfo::set_server(C_STRING s)
{
	server = s;
}


void CInfo::set_user(C_STRING s)
{
	user = s;
}

void CInfo::set_font(C_STRING s)
{
	font = s;
}

void CInfo::set_schema(C_STRING s)
{
	schema = s;
}

void CInfo::set_database(C_STRING s)
{
	database = s;
}

void CInfo::set_srid(long s)
{
	srid = s;
}

void CInfo::set_inf(C_STRING s)
{
	inf = s;
}

void CInfo::set_isOracle(C_STRING s)
{
	isOracle = s;
}

void CInfo::set_coord_input(C_STRING s)
{
	coord_input = s;
}

void CInfo::set_cat_coord_input(C_STRING s)
{
	cat_coord_input = s;
}

void CInfo::set_current_id1(int i)
{
	current_id1 = i;
}

void CInfo::set_current_id2(int i)
{
	current_id2 = i;
}

void CInfo::set_K(int i)
{
	K = i;
}

void CInfo::set_complex(bool b)
{
	complex = b;
}

int CInfo::read_fid_list(TB_Item_List *pList, C_STRING path)
{
	TB_Item	*tb_item;

	C_STRING fid_file;
	FILE*	fidFile;

	long	fid;	

	fid_file = path;
	fid_file += POSTGIS;
	fid_file += "\\fid_list.txt";
	if (gsc_path_exist(fid_file))
	{
		if ((fidFile = gsc_fopen(fid_file, _T("r"), ONETEST)) == NULL)
			AfxThrowUserException();				

		while (!feof(fidFile))
		{
			fwscanf_s(fidFile, _T("%d\n"), &fid);

			tb_item = new TB_Item(fid, true);
			pList->AddTail(tb_item);
		}
		
		if (gsc_fclose(fidFile) == GS_BAD)
			AfxThrowUserException();
	}

	return GS_GOOD;
}

int CInfo::set_write_db_prj(int i)
{
   C_STRING ValueStr;

   set_db_prj(i);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
   ValueStr = db_prj;
	if (gsc_set_profile(map_ini, _T("PRJ"), _T("DB_PRJ"), ValueStr.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_prj_index(int i)
{
   C_STRING ValueStr;

   set_prj_index(i);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
   ValueStr = prj_index;
	if (gsc_set_profile(map_ini, _T("PRJ"), _T("PRJ_INDEX"), ValueStr.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_prj_name(C_STRING s)
{
	set_prj_name(s);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	if (gsc_set_profile(map_ini, _T("PRJ"), _T("PRJ_NAME"), prj_name.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_prj_path(C_STRING s)
{
	set_prj_path(s);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	if (gsc_set_profile(map_ini, _T("PRJ"), _T("PRJ_PATH"), prj_path.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_class_index(int i)
{
   C_STRING ValueStr;

   set_class_index(i);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
   ValueStr = class_index;
	if (gsc_set_profile(map_ini, _T("CLASS"), _T("CLASS_INDEX"), ValueStr.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_sub_index(int i)
{
   C_STRING ValueStr;

   set_sub_index(i);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
   ValueStr = sub_index;
	if (gsc_set_profile(map_ini, _T("CLASS"), _T("SUB_INDEX"), ValueStr.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_class_type(int i)
{
   C_STRING ValueStr;

	set_class_type(i);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
   ValueStr = class_type;
	if (gsc_set_profile(map_ini, _T("CLASS"), _T("CLASS_TYPE"), ValueStr.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_class_type_text(C_STRING s)
{
	set_class_type_text(s);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	if (gsc_set_profile(map_ini, _T("CLASS"), _T("CLASS_TYPE_TEXT"), class_type_text.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_class_name(C_STRING s)
{
	set_class_name(s);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	if (gsc_set_profile(map_ini, _T("CLASS"), _T("CLASS_NAME"), class_name.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_sub_name(C_STRING s)
{
	set_sub_name(s);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	if (gsc_set_profile(map_ini, _T("CLASS"), _T("SUB_NAME"), sub_name.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_class_dwg_path(C_STRING s)
{
	set_class_dwg_path(s);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	if (gsc_set_profile(map_ini, _T("CLASS"), _T("CLASS_DWG_PATH"), class_dwg_path.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_script_number(int i)
{
   C_STRING ValueStr;

   set_script_number(i);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
   ValueStr = script_number;
	if (gsc_set_profile(map_ini, _T("SCRIPT"), _T("SCRIPT_NUMBER"), ValueStr.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

void CInfo::set_sql(int i)
{
	sql = i;
}

int CInfo::set_write_sql(int i)
{
   C_STRING ValueStr;

   set_sql(i);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
   ValueStr = sql;
	if (gsc_set_profile(map_ini, _T("SQL"), _T("SQL"), ValueStr.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_ini_path(C_STRING s)
{
	set_ini_path(s);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	if (gsc_set_profile(map_ini, _T("INI"), _T("INI_PATH"), ini_path.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_server(C_STRING s)
{
	set_server(s);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	if (gsc_set_profile(map_ini, _T("INI"), _T("SERVER"), server.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_user(C_STRING s)
{
	set_user(s);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	if (gsc_set_profile(map_ini, _T("INI"), _T("USER"), user.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_font(C_STRING s)
{
	set_font(s);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	if (gsc_set_profile(map_ini, _T("INI"), _T("FONT"), font.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_schema(C_STRING s)
{
	set_schema(s);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	if (gsc_set_profile(map_ini, _T("INI"), _T("SCHEMA"), schema.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_database(C_STRING s)
{
	set_database(s);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	if (gsc_set_profile(map_ini, _T("INI"), _T("DATABASE"), database.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_srid(long s)
{
	set_srid(s);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	if (gsc_set_profile(map_ini, _T("INI"), _T("SRID"), srid.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_inf(bool s)
{
	C_STRING temp;
	
	if (s == true)
		temp = "1";
	else
		temp = "0";

	set_inf(temp);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	if (gsc_set_profile(map_ini, _T("INI"), _T("INF"), temp.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_isOracle(bool s)
{
	C_STRING temp;
	
	if (s == true)
		temp = "true";
	else
		temp = "false";

	set_isOracle(temp);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	if (gsc_set_profile(map_ini, _T("INI"), _T("ISORACLE"), temp.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_current_id1(int i)
{
   C_STRING ValueStr;

   set_current_id1(i);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
   ValueStr = current_id1;
	if (gsc_set_profile(map_ini, _T("DWG"), _T("CURRENT_ID1"), ValueStr.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_current_id2(int i)
{
   C_STRING ValueStr;

   set_current_id2(i);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
   ValueStr = current_id2;
	if (gsc_set_profile(map_ini, _T("DWG"), _T("CURRENT_ID2"), ValueStr.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::set_write_K(int i)
{
   C_STRING ValueStr;

	set_K(i);

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
   ValueStr = K;
	if (gsc_set_profile(map_ini, _T("DWG"), _T("K"), ValueStr.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::write_fid_list(TB_Item_List *pList, C_STRING path)
{
	POSITION pos;
	TB_Item	*tb_item;

	C_STRING	str;

	C_STRING fid_file;
	FILE* fidFile;

	str = "";
	pos = pList->GetHeadPosition();
	while (pos)
	{
		tb_item = (TB_Item*) pList->GetNext(pos);		
		str += tb_item->get_fid();
		str += "\n";
	}

	fid_file = path;
	fid_file += POSTGIS;
	fid_file += "\\fid_list.txt";

	if ((fidFile = gsc_fopen(fid_file, _T("w"), ONETEST)) == NULL)
		AfxThrowUserException();				

	if (fwprintf(fidFile, _T("%s"), str) < 0)
	{
		if (gsc_fclose(fidFile) == GS_BAD)
			AfxThrowUserException();
		AfxThrowUserException();
	}

	if (gsc_fclose(fidFile) == GS_BAD)
		AfxThrowUserException();

	return GS_GOOD;
}

int CInfo::remove_fid_list(C_STRING path)
{
	C_STRING fid_file;

	fid_file = path;
	fid_file += POSTGIS;
	fid_file += "\\fid_list.txt";
	if (gsc_path_exist(fid_file))
		if (gsc_delfile(fid_file) == GS_BAD)
			AfxThrowUserException();

	return GS_GOOD;
}

int CInfo::readSql()
{
	if (gsc_find_sez(map_ini, _T("SQL"), 3, NULL) == GS_GOOD)
		get_int_value(map_ini, _T("SQL"), _T("SQL"), &sql);

	return GS_GOOD;
}

// DWG
int CInfo::add_dwg(C_STRING s)
{
	C_STRING temp, str, entry;
	temp = s;

	dwg_name_list.AddTail(temp.get_name());

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
   entry = count_dwg();
	if (gsc_set_profile(map_ini, _T("DWG"), _T("TOTAL"), entry.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}
	if (gsc_set_profile(map_ini, _T("DWG"), _T("COUNT"), entry.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}
	if (gsc_set_profile(map_ini, _T("DWG"), entry.get_name(), temp.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

   entry = current_id1;
	if (gsc_set_profile(map_ini, _T("DWG"), _T("CURRENT_ID1"), entry.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

   entry = current_id2;
	if (gsc_set_profile(map_ini, _T("DWG"), _T("CURRENT_ID2"), entry.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

   entry = K;
	if (gsc_set_profile(map_ini, _T("DWG"), _T("K"), entry.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::add_removed_dwg(C_STRING s)
{
	C_STRING temp, entry;
	temp = s;

	removed_dwg_name_list.AddTail(temp.get_name());

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
   entry = (int) removed_dwg_name_list.GetCount();
	if (gsc_set_profile(map_ini, _T("REMOVED_DWG"), _T("COUNT"), entry.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}
	if (gsc_set_profile(map_ini, _T("REMOVED_DWG"), entry.get_name(), temp.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::remove_last_dwg()
{
   C_STRING entry;

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;

   entry = count_dwg();
	if (gsc_delete_entry(map_ini, _T("DWG"), entry.get_name()) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	dwg_name_list.RemoveTail();

	if (gsc_set_profile(map_ini, _T("DWG"), _T("COUNT"), entry.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::remove_all_dwg()
{
	dwg_name_list.RemoveAll();

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	if (gsc_find_sez(map_ini, _T("DWG"), 3, NULL) == GS_GOOD)
		if (gsc_delete_sez(map_ini, _T("DWG")) == GS_BAD)
		{
			if (gsc_close_profile(map_ini) == GS_BAD)
				return GS_BAD;
			null_mapsql_ini();
			return GS_BAD;
		}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::count_dwg()
{
	return (int) dwg_name_list.GetCount();
}

int CInfo::total_dwg()
{
	return dwg_total;
}

void CInfo::get_last_dwg(C_STRING *name)
{
	if (count_dwg() > 0)
	{
		C_STRING temp = dwg_name_list.GetTail();
		*name = temp;
	}
	else
		*name = "";
}

int CInfo::search_dwg(C_STRING name)
{
	C_STRING temp;
	POSITION i;

	if (count_dwg() > 0)
	{
		i = dwg_name_list.GetHeadPosition() ;
		while (i) 
		{
			temp = dwg_name_list.GetNext(i);
			if (temp == name)
				return GS_GOOD;
		}
	}
	return GS_BAD;
}

int CInfo::search_removed_dwg(C_STRING name)
{
	C_STRING temp;
	POSITION i;

	if (removed_dwg_name_list.GetCount() > 0)
	{
		i = removed_dwg_name_list.GetHeadPosition() ;
		while (i) 
		{
			temp = removed_dwg_name_list.GetNext(i);
			if (temp == name)
				return GS_GOOD;
		}
	}
	return GS_BAD;
}

int CInfo::readDwg()
{
	int count=0;

	TCHAR *result=NULL;

	C_STRING	ValueStr;

	if (gsc_find_sez(map_ini, _T("DWG"), 3, NULL) == GS_GOOD)
	{
		if (get_int_value(map_ini, _T("DWG"), _T("TOTAL"), &dwg_total) == GS_BAD)
			return GS_BAD;	
		if (get_int_value(map_ini, _T("DWG"), _T("COUNT"), &count) == GS_BAD)
			return GS_BAD;	
		for (int i=1; i<=count; i++)			
		{
         ValueStr = i;
			if (gsc_get_profile(map_ini, _T("DWG"), ValueStr.get_name(), &result, 0, 0) == GS_BAD)
				return GS_BAD;
			if (result)
			{
				dwg_name_list.AddTail(result);
				free(result);
				result = NULL;
			}
		}

		if (get_int_value(map_ini, _T("DWG"), _T("CURRENT_ID1"),  &(int)current_id1) == GS_BAD)
			return GS_BAD;	
		if (get_int_value(map_ini, _T("DWG"), _T("CURRENT_ID2"),  &(int)current_id2) == GS_BAD)
			return GS_BAD;	
		if (get_int_value(map_ini, _T("DWG"), _T("K"),  &(int)K) == GS_BAD)
			return GS_BAD;	
	}

	return GS_GOOD;
}

int CInfo::readRemovedDwg()
{
   C_STRING ValueStr;
	int      count = 0;
	TCHAR *result=NULL;

	if (gsc_find_sez(map_ini, _T("REMOVED_DWG"), 11, NULL) == GS_GOOD)
	{
		if (get_int_value(map_ini, _T("REMOVED_DWG"), _T("COUNT"), &count) == GS_BAD)
			return GS_BAD;	

		for (int i=1; i<=count; i++)			
		{
         ValueStr = i;
			if (gsc_get_profile(map_ini, _T("REMOVED_DWG"), ValueStr.get_name(), &result, 0, 0) == GS_BAD)
				return GS_BAD;
			if (result)
			{
				removed_dwg_name_list.AddTail(result);
				free(result);
				result = NULL;
			}
		}	
	}

	return GS_GOOD;
}

// COMPLEX
int CInfo::count_complex()
{
	return complex_list.get_count();
}

int CInfo::add_complex(C_STRING path, C_INT* item)
{
   C_STRING ValueStr1, ValueStr2;
	int      count = 0;
	int      i = 0;

	complex_list.add_tail(item);

	if ((map_ini = gsc_open_profile(path, UPDATEABLE)) == NULL)
		return GS_BAD;

	count = complex_list.get_count();
   ValueStr1 = count;
	if (gsc_set_profile(map_ini, _T("COMPLEX_LIST"), _T("COUNT"), ValueStr1.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	i=0;
	item = (C_INT*) complex_list.get_head();
	while (item)
	{
		i++;
      ValueStr1 = i;
      ValueStr2 = item->get_key();
		if (gsc_set_profile(map_ini, _T("COMPLEX_LIST"), ValueStr1.get_name(), ValueStr2.get_name(), 0) == GS_BAD)
		{
			if (gsc_close_profile(map_ini) == GS_BAD)
				return GS_BAD;
			null_mapsql_ini();
			return GS_BAD;
		}
		item = (C_INT*) complex_list.get_next();
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::remove_all_complex()
{
	complex_list.remove_all();

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	if (gsc_find_sez(map_ini, _T("COMPLEX_LIST"), 12, NULL) == GS_GOOD)
		if (gsc_delete_sez(map_ini, _T("COMPLEX_LIST")) == GS_BAD)
		{
			if (gsc_close_profile(map_ini) == GS_BAD)
				return GS_BAD;
			null_mapsql_ini();
			return GS_BAD;
		}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::readComplex()
{
   C_STRING ValueStr;
	int      count = 0;
	int	   id;
	C_INT	   *item;

	if (gsc_find_sez(map_ini, _T("COMPLEX_LIST"), 12, NULL) == GS_GOOD)
	{
		if (get_int_value(map_ini, _T("COMPLEX_LIST"), _T("COUNT"), &count) == GS_BAD)
			return GS_BAD;	
		for (int i=1; i<=count; i++)			
		{
         ValueStr = i;
			if (get_int_value(map_ini, _T("COMPLEX_LIST"), ValueStr.get_name(), &id) == GS_BAD)
				return GS_BAD;
			if (id != 0)
			{
				item = new C_INT(id);
				complex_list.add_tail(item);
			}
		}
	}

	return GS_GOOD;
}

C_INT_LIST* CInfo::get_complex_list()
{
	return &complex_list;
}

// LAY
int CInfo::add_layer_name(C_STRING s)
{
   C_STRING entry;

	layer_name_list.AddTail(s.get_name());

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
   entry = count_layer_name();
	if (gsc_set_profile(map_ini, _T("LAY"), _T("COUNT"), entry.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_set_profile(map_ini, _T("LAY"), entry.get_name(), s.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::remove_last_layer_name()
{
   C_STRING entry;

	if ((map_ini = gsc_open_profile(map_postGis_path, UPDATEABLE)) == NULL)
		return GS_BAD;

   entry = count_layer_name();
	if (gsc_delete_entry(map_ini, _T("LAY"), entry.get_name()) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	layer_name_list.RemoveTail();

	if (gsc_set_profile(map_ini, _T("LAY"), _T("COUNT"), entry.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(map_ini) == GS_BAD)
			return GS_BAD;
		null_mapsql_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(map_ini) == GS_BAD)
		return GS_BAD;
	null_mapsql_ini();

	return GS_GOOD;
}

int CInfo::count_layer_name()
{
	return (int) layer_name_list.GetCount();
}

void CInfo::get_last_layer_name(C_STRING *name)
{
	if (count_layer_name() > 0)
		*name = layer_name_list.GetTail();
}

void CInfo::get_layer_name(C_STRING *name, int i)
{
	POSITION	pos;
	int			j;

	if (count_layer_name() > 0)
	{
		pos = layer_name_list.GetTailPosition();
		for (j=1; j<i;j++)
		{
			layer_name_list.GetPrev(pos);
		}

		*name = layer_name_list.GetAt(pos);
	}
}

int CInfo::readLayer_name()
{
   C_STRING ValueStr;
	int      count = 0;
	TCHAR    *result = NULL;

	if (gsc_find_sez(map_ini, _T("LAY"), 3, NULL) == GS_GOOD)
	{
		if (get_int_value(map_ini, _T("LAY"), _T("COUNT"), &count) == GS_BAD)
			return GS_BAD;	

		for (int i=1; i<=count; i++)			
		{
         ValueStr = i;
			if (gsc_get_profile(map_ini, _T("LAY"), ValueStr.get_name(), &result, 0, 0) == GS_BAD)
				return GS_BAD;
			if (result)
			{
				layer_name_list.AddTail(result);
				free(result);
				result = NULL;
			}
		}	
	}
	
	return GS_GOOD;
}

POSITION CInfo::find_layer_name(C_STRING s)
{
	return layer_name_list.Find(s.get_name());
}

C_STRING CInfo::get_map_postGis_path()
{
	return map_postGis_path;
}

void CInfo::null_mapsql_ini()
{
	map_ini = NULL;
}

C_STRING CInfo::get_path_with_ext(C_STRING ext)
{
	TCHAR *cod36prj=NULL, *cod36cls=NULL;
	C_STRING temp = "";

	if(gsc_long2base36(&cod36prj, get_prj_index(), 2) == GS_BAD)
		return temp;
	if(gsc_long2base36(&cod36cls, get_class_index(), 3) == GS_BAD)
	{ 
		if (cod36prj) 
			free(cod36prj); 
		return temp;
	}
	
	// in temp la path dei dwg della classe
	temp = get_class_dwg_path();
	temp += '\\';
	temp += cod36prj;
	temp += cod36cls;

	if (ext != "") 
	{	
		temp += "*.";
		temp += ext;
	}

	return temp;
}

bool CInfo::create_table()
{
	bool create;

	create = true;
	if (dwg_total - dwg_name_list.GetCount() > 0)
		create = false;

	return create;
}
