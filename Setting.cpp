/*************************************************************************/
/* INCLUDES                                                              */
/*************************************************************************/
#include "stdafx.h"

#include "setting.h"


CSetting::CSetting() 
{  
	setting_ini = NULL;

	first = TRUE;
	start = TRUE;
	finish = FALSE;
}

CSetting::~CSetting() 
{  
}

FILE *CSetting::openFile(C_STRING path) 
{  
	setting_path = path;
	setting_path += POSTGIS;
	setting_path += "\\setting.ini";

	setting_ini = gsc_open_profile(setting_path, READONLY);
	return setting_ini;
}

int CSetting::closeFile() 
{ 
	if (gsc_close_profile(setting_ini) == GS_BAD)
		return GS_BAD;
	null_setting_ini();

	return GS_GOOD;
}

int CSetting::readFile() 
{  
	if ((setting_ini = gsc_open_profile(setting_path, READONLY)) == NULL)
		return GS_BAD;

	TRY
	{
		if (get_int_value(setting_ini, _T("SET"), _T("FIRST"), &first) == GS_BAD)
			AfxThrowUserException();	
		if (get_int_value(setting_ini, _T("SET"), _T("START"), &start) == GS_BAD)
			AfxThrowUserException();	
		if (get_int_value(setting_ini, _T("SET"), _T("FINISH"), &finish) == GS_BAD)
			AfxThrowUserException();	
	}
	CATCH( CUserException, e )
	{
		if (gsc_close_profile(setting_ini) == GS_BAD)
			return GS_BAD;
		null_setting_ini();
		return GS_BAD;
	}
	END_CATCH

	if (gsc_close_profile(setting_ini) == GS_BAD)
		return GS_BAD;
	null_setting_ini();

	return GS_GOOD;
}


int CSetting::writeFile() 
{  
   C_STRING ValueStr;

	if ((setting_ini = gsc_open_profile(setting_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
	TRY
	{
      ValueStr = first;
		if (gsc_set_profile(setting_ini, _T("SET"), _T("FIRST"), ValueStr.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
      ValueStr = start;
		if (gsc_set_profile(setting_ini, _T("SET"), _T("START"), ValueStr.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
      ValueStr = finish;
		if (gsc_set_profile(setting_ini, _T("SET"), _T("FINISH"), ValueStr.get_name(), 0) == GS_BAD)
			AfxThrowUserException();
	}
	CATCH( CUserException, e )
	{
		if (gsc_close_profile(setting_ini) == GS_BAD)
			return GS_BAD;
		null_setting_ini();
		return GS_BAD;
	}
	END_CATCH

	if (gsc_close_profile(setting_ini) == GS_BAD)
		return GS_BAD;
	null_setting_ini();

	return GS_GOOD;
}


int CSetting::get_first() 
{  
	return first;
}

int CSetting::get_start() 
{  
	return start;
}

int CSetting::get_finish() 
{  
	return finish;
}

void CSetting::set_first(int f)
{
	first = f;
}

void CSetting::set_start(int f)
{
	start = f;
}

void CSetting::set_finish(int f)
{
	finish = f;
}

int CSetting::set_write_first(int f)
{
   C_STRING ValueStr;

	set_first(f);

	if ((setting_ini = gsc_open_profile(setting_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
   ValueStr = first;
	if (gsc_set_profile(setting_ini, _T("SET"), _T("FIRST"), ValueStr.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(setting_ini) == GS_BAD)
			return GS_BAD;
		null_setting_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(setting_ini) == GS_BAD)
		return GS_BAD;
	null_setting_ini();

	return GS_GOOD;
}

int CSetting::set_write_start(int f)
{
   C_STRING ValueStr;

	set_start(f);

	if ((setting_ini = gsc_open_profile(setting_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
   ValueStr = start;
	if (gsc_set_profile(setting_ini, _T("SET"), _T("START"), ValueStr.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(setting_ini) == GS_BAD)
			return GS_BAD;
		null_setting_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(setting_ini) == GS_BAD)
		return GS_BAD;
	null_setting_ini();

	return GS_GOOD;
}

int CSetting::set_write_finish(int f)
{
   C_STRING ValueStr;

	set_finish(f);

	if ((setting_ini = gsc_open_profile(setting_path, UPDATEABLE)) == NULL)
		return GS_BAD;
	
   ValueStr = finish;
	if (gsc_set_profile(setting_ini, _T("SET"), _T("FINISH"), ValueStr.get_name(), 0) == GS_BAD)
	{
		if (gsc_close_profile(setting_ini) == GS_BAD)
			return GS_BAD;
		null_setting_ini();
		return GS_BAD;
	}

	if (gsc_close_profile(setting_ini) == GS_BAD)
		return GS_BAD;
	null_setting_ini();

	return GS_GOOD;
}

void CSetting::null_setting_ini()
{
	setting_ini = NULL;
}

C_STRING CSetting::get_setting_path()
{
	return setting_path;
}

int CSetting::clear() 
{  
	setting_ini = NULL;

	first = TRUE;
	start = TRUE;
	finish = FALSE;

	return GS_GOOD;
}
