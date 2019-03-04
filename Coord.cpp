/*************************************************************************/
/* INCLUDES                                                              */
/*************************************************************************/
#include "stdafx.h"

#include "coord.h"

CCoord::CCoord() 
{  
	coord_ini = NULL;
}

CCoord::~CCoord() 
{  
}

FILE *CCoord::openFile(C_STRING path) 
{  
	coord_path = path;
	coord_path += POSTGIS;
	coord_path += "\\coord_sql.ini";

	coord_ini = gsc_open_profile(coord_path, READONLY);
	return coord_ini;
}

int CCoord::closeFile() 
{ 
	if (gsc_close_profile(coord_ini) == GS_BAD)
		return GS_BAD;

	null_coord_ini();
	return GS_GOOD;
}

int CCoord::readFile() 
{
   bool isUnicode;

	if ((coord_ini = gsc_open_profile(coord_path, READONLY, ONETEST, &isUnicode)) == NULL)
		return GS_BAD;

	TCHAR *result=NULL;

	TRY
	{
		if (gsc_get_profile(coord_ini, _T("COORD"), _T("CAT_COORD_INPUT"), &result, 0, 0, isUnicode) == GS_BAD)
			AfxThrowUserException();
		cat_coord_input = result;
		free (result);
		result = NULL;
		if (gsc_get_profile(coord_ini, _T("COORD"), _T("COORD_INPUT"), &result, 0, 0, isUnicode) == GS_BAD)
			AfxThrowUserException();
		coord_input = result;
		free (result);
		result = NULL;
		if (gsc_get_profile(coord_ini, _T("COORD"), _T("CAT_COORD_OUTPUT"), &result, 0, 0, isUnicode) == GS_BAD)
			AfxThrowUserException();
		cat_coord_output = result;
		free (result);
		result = NULL;
		if (gsc_get_profile(coord_ini, _T("COORD"), _T("COORD_OUTPUT"), &result, 0, 0, isUnicode) == GS_BAD)
			AfxThrowUserException();
		coord_output = result;
		free (result);
		result = NULL;

	}
	CATCH( CUserException, e )
	{
		if (gsc_close_profile(coord_ini) == GS_BAD)
			return GS_BAD;
		null_coord_ini();
		return GS_BAD;
	}
	END_CATCH

	if (gsc_close_profile(coord_ini) == GS_BAD)
		return GS_BAD;
	null_coord_ini();

	return GS_GOOD;
}

int CCoord::writeFile() 
{  
	if ((coord_ini = gsc_open_profile(coord_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	TRY
	{
		if (gsc_set_profile(coord_ini, _T("COORD"), _T("CAT_COORD_INPUT"), cat_coord_input.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
		if (gsc_set_profile(coord_ini, _T("COORD"), _T("COORD_INPUT"), coord_input.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
		if (gsc_set_profile(coord_ini, _T("COORD"), _T("CAT_COORD_OUTPUT"), cat_coord_output.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
		if (gsc_set_profile(coord_ini, _T("COORD"), _T("COORD_OUTPUT"), coord_output.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
	}
	CATCH( CUserException, e )
	{
		if (gsc_close_profile(coord_ini) == GS_BAD)
			return GS_BAD;
		null_coord_ini();
		return GS_BAD;
	}
	END_CATCH

	if (gsc_close_profile(coord_ini) == GS_BAD)
		return GS_BAD;
	null_coord_ini();

	return GS_GOOD;
}

int CCoord::writeFile(CCoord *coord) 
{  
	if (set_write_cat_coord_input(coord->get_cat_coord_input()) == GS_BAD)
		return GS_BAD;
	if (set_write_coord_input(coord->get_coord_input()) == GS_BAD)
		return GS_BAD;
	if (set_write_cat_coord_output(coord->get_cat_coord_output()) == GS_BAD)
		return GS_BAD;
	if (set_write_coord_output(coord->get_coord_output()) == GS_BAD)
		return GS_BAD;

	return GS_GOOD;
}

C_STRING CCoord::get_coord_input()
{
	return coord_input;
}

C_STRING CCoord::get_cat_coord_input()
{
	return cat_coord_input;   
}

C_STRING  CCoord::get_coord_output()
{
	return coord_output;
}

C_STRING  CCoord::get_cat_coord_output()
{
	return cat_coord_output;
}

void CCoord::set_coord_input(C_STRING s)
{
	s.rtrim();
	coord_input = s;
}

void CCoord::set_cat_coord_input(C_STRING s)
{
	s.rtrim();
	cat_coord_input = s;
}

void CCoord::set_coord_output(C_STRING s)
{
	s.rtrim();
	coord_output = s;
}

void CCoord::set_cat_coord_output(C_STRING s)
{
	s.rtrim();
	cat_coord_output = s;
}

int CCoord::set_write_coord_input(C_STRING s)
{
	set_coord_input(s);

	if ((coord_ini = gsc_open_profile(coord_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	if (gsc_set_profile(coord_ini, _T("COORD"), _T("COORD_INPUT"), coord_input.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(coord_ini) == GS_BAD)
			return GS_BAD;
		null_coord_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(coord_ini) == GS_BAD)
		return GS_BAD;
	null_coord_ini();

	return GS_GOOD;
}

int CCoord::set_write_cat_coord_input(C_STRING s)
{
	set_cat_coord_input(s);

	if ((coord_ini = gsc_open_profile(coord_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	if (gsc_set_profile(coord_ini, _T("COORD"), _T("CAT_COORD_INPUT"), cat_coord_input.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(coord_ini) == GS_BAD)
			return GS_BAD;
		null_coord_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(coord_ini) == GS_BAD)
		return GS_BAD;
	null_coord_ini();

	return GS_GOOD;
}

int CCoord::set_write_coord_output(C_STRING s)
{
	set_coord_output(s);

	if ((coord_ini = gsc_open_profile(coord_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	if (gsc_set_profile(coord_ini, _T("COORD"), _T("COORD_OUTPUT"), coord_output.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(coord_ini) == GS_BAD)
			return GS_BAD;
		null_coord_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(coord_ini) == GS_BAD)
		return GS_BAD;
	null_coord_ini();

	return GS_GOOD;
}

int CCoord::set_write_cat_coord_output(C_STRING s)
{
	set_cat_coord_output(s);

	if ((coord_ini = gsc_open_profile(coord_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	if (gsc_set_profile(coord_ini, _T("COORD"), _T("CAT_COORD_OUTPUT"), cat_coord_output.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(coord_ini) == GS_BAD)
			return GS_BAD;
		null_coord_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(coord_ini) == GS_BAD)
		return GS_BAD;
	null_coord_ini();

	return GS_GOOD;
}

C_STRING CCoord::get_coord_path()
{
	return coord_path;
}

void CCoord::null_coord_ini()
{
	coord_ini = NULL;
}
