/************************************************************/
/*Name: map.cpp
                                   
	Module description: File funzioni principali.
         
	Author: Maria Paola Deprati
                                      
	(c) Copyright 2000 by AMGA S.P.A

/************************************************************/

#include "stdafx.h"

#include "map.h"

int map_postgis(void)
{
	C_STRING path, server, user, font, schema, database, srid, inf, isoracle;

	isoracle = "false";
	inf = "1";

	presbuf arg = ads_getargs();
	if (!arg || arg->restype != RTSTR)
	{
		set_GS_ERR_COD(eGSInvalidArg); 
		return GS_BAD;
	}
	path = arg->resval.rstring;

	arg = arg->rbnext;
	if (arg)
	{
		if (arg->restype != RTSTR)
		{
			set_GS_ERR_COD(eGSInvalidArg); 
			return GS_BAD;
		}
		server = arg->resval.rstring;

		arg = arg->rbnext;
		if (arg)
		{
			if (arg->restype != RTSTR)
			{
				set_GS_ERR_COD(eGSInvalidArg); 
				return GS_BAD;
			}
			user = arg->resval.rstring;

			arg = arg->rbnext;
			if (arg)
			{
				if (arg->restype != RTSTR)
				{
					set_GS_ERR_COD(eGSInvalidArg); 
					return GS_BAD;
				}
				font = arg->resval.rstring;

				arg = arg->rbnext;
				if (arg)
				{
					if (arg->restype != RTSTR)
					{
						set_GS_ERR_COD(eGSInvalidArg); 
						return GS_BAD;
					}
					schema = arg->resval.rstring;

					arg = arg->rbnext;
					if (arg)
					{
						if (arg->restype != RTSTR)
						{
							set_GS_ERR_COD(eGSInvalidArg); 
							return GS_BAD;
						}
						database = arg->resval.rstring;

						arg = arg->rbnext;
						if (arg)
						{
							if (arg->restype != RTSTR)
							{
								set_GS_ERR_COD(eGSInvalidArg); 
								return GS_BAD;
							}
							srid = arg->resval.rstring;

							arg = arg->rbnext;
							if (arg)
							{
								if (arg->restype != RTSTR)
								{
									set_GS_ERR_COD(eGSInvalidArg); 
									return GS_BAD;
								}
								inf = arg->resval.rstring;

								arg = arg->rbnext;
								if (arg)
								{
									if (arg->restype != RTSTR)
									{
										set_GS_ERR_COD(eGSInvalidArg); 
										return GS_BAD;
									}
									isoracle = arg->resval.rstring;
								}
							}
						}
					}
				}
			}
		}
	}

	ads_retvoid();
	set_GS_ERR_COD(eGSNoError);

	CMapPostGIS mapPostgis(path.get_name(), server.get_name(), user.get_name(), font.get_name(), schema.get_name(), database.get_name(), srid.get_name(), inf.get_name(), isoracle.get_name());
		
//		gsc_disable_reactors();

	mapPostgis.Exec();

//		gsc_enable_reactors();
	return GS_GOOD;
}

int map_postgisSecExt(void)
{
	C_STRING path, server, user, schema, database, srid, inf, isoracle;

	isoracle = "false";
	inf = "1";

	presbuf arg = ads_getargs();
	if (!arg || arg->restype != RTSTR)
	{
		set_GS_ERR_COD(eGSInvalidArg); 
		return GS_BAD;
	}

	path = arg->resval.rstring;

	arg = arg->rbnext;
	if (arg)
	{
		if (arg->restype != RTSTR)
		{
			set_GS_ERR_COD(eGSInvalidArg); 
			return GS_BAD;
		}
		server = arg->resval.rstring;
	
		arg = arg->rbnext;
		if (arg)
		{
			if (arg->restype != RTSTR)
			{
				set_GS_ERR_COD(eGSInvalidArg); 
				return GS_BAD;
			}
			user = arg->resval.rstring;

			arg = arg->rbnext;	// salto Font
			arg = arg->rbnext;
			if (arg)
			{
				if (arg->restype != RTSTR)
				{
					set_GS_ERR_COD(eGSInvalidArg); 
					return GS_BAD;
				}
				schema = arg->resval.rstring;

				arg = arg->rbnext;
				if (arg)
				{
					if (arg->restype != RTSTR)
					{
						set_GS_ERR_COD(eGSInvalidArg); 
						return GS_BAD;
					}
					database = arg->resval.rstring;

					arg = arg->rbnext;
					if (arg)
					{
						if (arg->restype != RTSTR)
						{
							set_GS_ERR_COD(eGSInvalidArg); 
							return GS_BAD;
						}
						srid = arg->resval.rstring;

						arg = arg->rbnext;
						if (arg)
						{
							if (arg->restype != RTSTR)
							{
								set_GS_ERR_COD(eGSInvalidArg); 
								return GS_BAD;
							}
							inf = arg->resval.rstring;

							arg = arg->rbnext;
							if (arg)
							{
								if (arg->restype != RTSTR)
								{
									set_GS_ERR_COD(eGSInvalidArg); 
									return GS_BAD;
								}
								isoracle = arg->resval.rstring;
							}
						}
					}
				}
			}
		}	
	}

	ads_retvoid();
	set_GS_ERR_COD(eGSNoError);

	CMapPostGIS mapPostgis(path.get_name(), server.get_name(), user.get_name(), "", schema.get_name(), database.get_name(), "", "", isoracle.get_name());
		
//		gsc_disable_reactors();

	mapPostgis.ExecSecExt();

//		gsc_enable_reactors();
	return GS_GOOD;
}