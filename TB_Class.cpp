/*************************************************************************/
/* INCLUDES                                                              */
/*************************************************************************/

#include "stdafx.h"

#include "Info.h"
#include "TB_Class.h"

TB_Class::TB_Class(CInfo *_info, CStringList *reserved_list) 
{
	read_only = 0;
	active = 1;
	table_does_not_exist = 0;
	spatial_relate = "anyinteract";
	min_x = -10;
	min_y = -10;
	max_x = 1000000;
	max_y = 1000000;
	min_z = -10;
	max_z = 1000000;
	tolerance = 0.005;
	dimension = 2;
	user_defined_orientation = 0;

	info = _info;
	if (_info)
	{
		if ((pCls = gsc_find_class(info->get_prj_index(), info->get_class_index(), info->get_sub_index())) == NULL)
			AfxThrowUserException();	
	}
	else
		pCls = NULL;
	
	ptb_class_list = NULL;
	ptb_sec_list = NULL;

	num_vertex = 100;
}

TB_Class::~TB_Class()
{
	POSITION pos;

	TB_Class	*tb_class;
	TB_Item		*tb_item;
	CObject		*obj;

	if (ptb_class_list)
	{
		pos = ptb_class_list->GetHeadPosition();
		while (pos)
		{
			tb_class = (TB_Class*) ptb_class_list->GetNext(pos);
			delete tb_class;
		}
	}
	if (ptb_sec_list)
	{
		pos = ptb_sec_list->GetHeadPosition();
		while (pos)
		{
			tb_class = (TB_Class*) ptb_sec_list->GetNext(pos);
			delete tb_class;
		}
	}

	if (tb_item_list.GetCount() > 0)
	{
		pos = tb_item_list.GetHeadPosition();
		while (pos)
		{
			tb_item = (TB_Item*) tb_item_list.GetNext(pos);
			delete tb_item;
		}
	}
	if (tb_item_attribute_list.GetCount() > 0)
	{
		pos = tb_item_attribute_list.GetHeadPosition();
		while (pos)
		{
			obj = (CObject*) tb_item_attribute_list.GetNext(pos);
			delete obj;
		}
	}
	if (tb_attribute_list.GetCount() > 0)
	{
		pos = tb_attribute_list.GetHeadPosition();
		while (pos)
		{
			obj = (CObject*) tb_attribute_list.GetNext(pos);
			delete obj;
		}
	}
}

void TB_Class::set_f_class_id(long _l)
{  
	f_class_id = _l;
}

void TB_Class::set_f_prj_name(C_STRING &_s)
{  
	f_prj_name = _s;
   f_prj_name.tolower();
}
void TB_Class::set_f_prj_name(const TCHAR *_s)
{  
	f_prj_name = _s;
   f_prj_name.tolower();
}

void TB_Class::set_f_class_name(C_STRING &_s)
{  
	f_class_name = _s;
   f_class_name.tolower();
}
void TB_Class::set_f_class_name(const TCHAR *_s)
{  
	f_class_name = _s;
   f_class_name.tolower();
}

void TB_Class::set_caption(C_STRING &_s)
{  
	caption = _s;
}
void TB_Class::set_caption(const TCHAR *_s)
{  
	caption = _s;
}

void TB_Class::set_f_class_type(C_STRING &_s)
{  
	f_class_type = _s;
}
void TB_Class::set_f_class_type(const TCHAR *_s)
{  
	f_class_type = _s;
}

void TB_Class::set_attribute_list(bool isOracle, C_ATTRIB_LIST *pList, C_INFO *pInfo)
{  
	C_ATTRIB		*pAttrib;

	C_DBCONNECTION	*pDestConn;

	TB_Attribute	*tb_attribute;
	
	C_STRING	type, def;
	long	len;
	int		dec;

	C_2STR_LIST UDLProp;
	if (isOracle)
		pDestConn = get_pDBCONNECTION_LIST()->get_Connection(_T("Oracle"), &UDLProp);
	else
		pDestConn = get_pDBCONNECTION_LIST()->get_Connection(_T("PostgreSQL"), &UDLProp);

	if ((pList) && (pList->get_count() > 0))
	{  
		pAttrib = (C_ATTRIB *) pList->get_head();

		while (pAttrib)
		{
			type = pAttrib->type;
			len = pAttrib->len;
			dec = pAttrib->dec;
			pAttrib->Convert2ProviderType(pInfo->getDBConnection(OLD), pDestConn);
			// Tapullo, che permette di gestire il dimensionamento dei campi di tipo numerico.
			// In Geosim i valori di len e dec per un numero vengono utilizzati per la sola visualizzazione e
			// non per il dimensionamento del valore che è a virgola mobile. Dopo la conversione verso Oracle,
			// come deve essere, si ottiene il tipo FLOAT non dimensionato. Essendo per Topobase importante il
			// dimensionamento del tipo NUMBER vengono risettati i valori originali di len e dec.
			if (isOracle)
			{
				pAttrib->len = len;
				pAttrib->dec = dec;
			}	

			def = "";
			if ((pAttrib->def) && (pAttrib->def->restype != RTNIL))
			{
				if (pAttrib->def->restype == RTREAL)
					def = pAttrib->def->resval.rreal;
				else
				{
					if ((pAttrib->def->restype == RTLONG) || (pAttrib->def->restype == RTSHORT))
						def = pAttrib->def->resval.rint;
					else
						def = pAttrib->def->resval.rstring;
				}	
			}	
			tb_attribute = new TB_Attribute(pAttrib->get_name(), pAttrib->type, type, (int) pAttrib->len, 
											pAttrib->dec, (pAttrib->man == GS_GOOD) ? true : false , def);
			tb_attribute_list.AddTail(tb_attribute);

			pAttrib = (C_ATTRIB *) pList->get_next();
		}	
	}
}

long TB_Class::get_f_class_id()
{  
	return f_class_id;
}

C_STRING TB_Class::get_f_prj_name()
{  
	return correctString1(f_prj_name);
}

C_STRING TB_Class::get_f_class_name()
{  
	return correctString1(f_class_name);
}

C_STRING TB_Class::get_caption()
{  
	return caption;
}

C_STRING TB_Class::get_f_class_type()
{  
	return f_class_type;
}

CObList* TB_Class::get_attribute_list()
{  
	return &tb_attribute_list;
}

int TB_Class::Load(bool isOracle, C_STRING &prj_name, long *current_id1, long *current_id2, CStringList *reserved_list)
{                                               
	int ret = GS_GOOD;
	
	C_RB_LIST	ColValues;

	C_STRING		class_name;

	long	_current_id;

	TB_Attribute	*tb_attribute;

	TRY
	{
		_current_id = *current_id1;

		set_f_class_id(pCls->get_key());
		//set_f_prj_name(pCls->ptr_id()->pPrj->get_name());
		set_f_prj_name(prj_name);
		if (info->get_sub_index() == 0)
		{
			set_f_class_name(pCls->get_name());
			set_caption(pCls->get_name());
		}
		else
		{
			class_name = "s";
			class_name += pCls->ptr_id()->code;
			class_name += "_";
			class_name += pCls->get_name();
			set_f_class_name(class_name);
			set_caption(class_name);
		}
		if (pCls->ptr_attrib_list())
			set_attribute_list(isOracle, pCls->ptr_attrib_list(), pCls->ptr_info());

		switch (info->get_class_type())
		{
			case CL_NODE:
				set_f_class_type(_T("P"));
				LoadPoints(current_id1, current_id2, reserved_list);
				break;
			case CL_TEXT:
				set_f_class_type(_T("T"));
				LoadTexts(current_id1, reserved_list);
				break;
			case CL_POL:
				set_f_class_type(_T("T"));
				LoadPolylines(isOracle, current_id1, current_id2, reserved_list);
				break;
			case CL_SUP:
				set_f_class_type(_T("O"));
				LoadSurfaces(isOracle, current_id1, reserved_list);
				break;
			case CL_SPAG:
				set_f_class_type(_T("T"));
				LoadSpaghetti(isOracle, current_id1);
				break;
		}

		if (info->count_complex())
		//if (info->get_complex())
		{
			tb_attribute = new TB_Attribute("complex_fid_parent", "int4", "Long", 10, 0, true, "");
			if (tb_attribute_list.Find(tb_attribute) == false)
				tb_attribute_list.AddTail(tb_attribute);
		}

		if (_current_id == 0)
			LoadSecondaries(isOracle, reserved_list);
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

int TB_Class::CreateTablePostgres(FILE *file, bool create1, bool create2, long srid, CStringList *reserved_list,  C_DBCONNECTION *pConn = NULL)
{                                               
	int ret = GS_GOOD;

	C_STRING str, str1, str2, tb_class_name, attr;

	POSITION	pos, pos1, pos2;

	TB_Attribute *tb_attribute, *tb_attribute1;

	bool		found;

	TB_Item		*tb_item;

	TB_Class	*tb_class=NULL;

	C_DBCONNECTION *pTempConn;

	TRY
	{
		pTempConn = pConn;
		if ((!pConn) && pCls)
		{
			if (pCls->get_category() != CAT_SPAGHETTI)
				pTempConn = pCls->ptr_info()->getDBConnection(OLD);
		}

		if (create1)
		{
			if ((ptb_class_list) && (ptb_class_list->GetCount() == 1) && ((get_f_class_type() == "O") || (get_f_class_type() == "P") || (get_f_class_type() == "T")))
			{
				str = "SELECT gw_dropview('";
				str += get_f_prj_name();
				//str += "@SCHEMA@";
				str += "', '";
				str += correctString4(get_f_class_name());
				
				if ((get_f_class_type() == "O") || (get_f_class_type() == "P"))
					str += "_tbl_v');";
				if (get_f_class_type() == "T")
					str += "_v');";

				if (fwprintf(file, _T("%s\n"), str) < 0)
				{
					if (gsc_fclose(file) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				if (fwprintf(file, _T("%s\n"), _T("COMMIT;")) < 0)
				{
					if (gsc_fclose(file) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
				if (fwprintf(file, _T("%s\n"), _T("BEGIN;")) < 0)
				{
					if (gsc_fclose(file) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				if ((get_f_class_type() == "T") && (pCls) && (pCls->get_category() != CAT_SPAGHETTI) && (pCls->ptr_id()->type == TYPE_POLYLINE))
				{
					str = "SELECT gw_dropview('";
					str += get_f_prj_name();
					//str += "@SCHEMA@";
					str += "', '";
					str += correctString4(get_f_class_name());
					str += "_l_tbl_v');";				

					if (fwprintf(file, _T("%s\n"), str) < 0)
					{
						if (gsc_fclose(file) == GS_BAD)
							AfxThrowUserException();
						AfxThrowUserException();
					}

					if (fwprintf(file, _T("%s\n"), _T("COMMIT;")) < 0)
					{
						if (gsc_fclose(file) == GS_BAD)
							AfxThrowUserException();
						AfxThrowUserException();
					}
					if (fwprintf(file, _T("%s\n"), _T("BEGIN;")) < 0)
					{
						if (gsc_fclose(file) == GS_BAD)
							AfxThrowUserException();
						AfxThrowUserException();
					}
				}
			}

			str = "SELECT gw_droptable('";
			str += get_f_prj_name();
			//str += "@SCHEMA@";
			str += "', '";
			str += correctString4(get_f_class_name());
			str += "');";
			if (fwprintf(file, _T("%s\n"), str) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}

			str = "CREATE TABLE ";
			str += get_f_prj_name();
			//str += "@SCHEMA@";
			str += ".";
			str += correctString4(get_f_class_name());
			str += " (";

			if (tb_item_attribute_list.GetCount() > 0)
			{
				pos = tb_item_attribute_list.GetHeadPosition();
				while (pos)
				{
					tb_attribute = (TB_Attribute*) tb_item_attribute_list.GetNext(pos);			
					
					if (str1.len() > 0)
						str1 += ", ";
					str1 += tb_attribute->get_name();
					str1 += " ";
					str1 += tb_attribute->get_type();
				}
			}
			if (tb_attribute_list.GetCount() > 0)
			{
				pos = tb_attribute_list.GetHeadPosition();
				while (pos)
				{
					tb_attribute = (TB_Attribute*) tb_attribute_list.GetNext(pos);			
					
					if (str1.len() > 0)
						str1 += ", \"";
					
					str2 = tb_attribute->get_name();
					str2.tolower();
					pos2 = reserved_list->Find(str2.get_name());
					if (pos2)
						str2 += "_";
					// le " " vengono introdotte a prescindere dalla presenza 
					// di caratteri speciali nella stringa
					str2 = correctString1(str2);
					str2.tolower();

					str1 += str2;
					str1 += "\" ";
					str1 += tb_attribute->get_type();
				}
			}	
			str += str1;
			str += ", CONSTRAINT pkey_";
			str += get_f_class_name();
			str += " PRIMARY KEY (fid)";
			str += ");";
			if (fwprintf(file, _T("%s\n"), str) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}

			if (create2)
			{
				//-- Index
				str = "CREATE INDEX idx_";
				str += get_f_class_name();
				str += "_fid_parent ON ";
                str += get_f_prj_name();
				//str += "@SCHEMA@";
				str += ".";
				str += correctString4(get_f_class_name());
				str += " USING btree(fid_parent);";

				if (fwprintf(file, _T("%s\n"), str) < 0)
				{
					if (gsc_fclose(file) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
			}

			// solo se TB_Item_Label, TB_Item_Point, TB_Item_Surface, TB_Item_Polyline_L
			if ((get_f_class_type() == "A") || (get_f_class_type() == "P") || (get_f_class_type() == "O") || (get_f_class_type() == "L"))
			{
				if (fwprintf(file, _T("%s\n"), _T("COMMIT;")) < 0)
				{
					if (gsc_fclose(file) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
				if (fwprintf(file, _T("%s\n"), _T("BEGIN;")) < 0)
				{
					if (gsc_fclose(file) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				str = "SELECT AddGeometryColumn('";
				str += get_f_prj_name();
				//str += "@SCHEMA@";
				str += "', '";
				str += correctString4(get_f_class_name());
				str += "', 'wkb_geometry', ";
				str += srid;
				str += ", '";
				if ((get_f_class_type() == "P") || (get_f_class_type() == "A"))
					str += "POINT";
				else
				{
					if (get_f_class_type() == "O")
						str += "POLYGON";
					else
						str += "LINESTRING";
				}
				str += "', 2);";

				if (fwprintf(file, _T("%s\n"), str) < 0)
				{
					if (gsc_fclose(file) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				if (fwprintf(file, _T("%s\n"), _T("COMMIT;")) < 0)
				{
					if (gsc_fclose(file) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
				if (fwprintf(file, _T("%s\n"), _T("BEGIN;")) < 0)
				{
					if (gsc_fclose(file) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
			
				//-- Index
				str = "CREATE INDEX idx_";
				str += get_f_class_name();
				str += "_geom ON ";
                str += get_f_prj_name();
				//str += "@SCHEMA@";
				str += ".";
				str += correctString4(get_f_class_name());
				str += " USING gist(wkb_geometry);";
				if (fwprintf(file, _T("%s\n"), str) < 0)
				{
					if (gsc_fclose(file) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
			}
		}

		// solo se TB_Item_Label, TB_Item_Point, TB_Item_Surface, TB_Item_Polyline_L
		if (get_f_class_type() == "A" || get_f_class_type() == "P" || get_f_class_type() == "O" || get_f_class_type() == "L")
		{
			pos = tb_item_attribute_list.GetHeadPosition();
			tb_attribute = new TB_Attribute("wkb_geometry", "geometry", "", 0, 0, true, "");
			tb_item_attribute_list.InsertAfter(pos, tb_attribute);
		}

		// COMMENTATO IN QUANTO NON SI RITIENE NECESSARIA LA TABELLA MADRE DEGLI SPAGHETTI
		// INOLTRE NELLA LOGICA DELLA FUNZIONE AddTableSpa VI E' UN PROBLEMA LEGATO AL FATTO
		// CHE VIENE PRESO IN CONSIDERAZIONE IL fid_parent CHE IN ALCUNI PARTICOLARI CASI 
		// (ES. MTEXT) PUO' ESSERE DOPPIO E QUINDI PROVOCA NON UNICITA'
		//if ((pCls) && (pCls->get_category() == CAT_SPAGHETTI))
		//{
		//	if ((ptb_class_list) && (ptb_class_list->GetCount() > 0))
		//	{
		//		pos = ptb_class_list->GetHeadPosition();
		//		while (pos)
		//		{
		//			tb_class = (TB_Class*) ptb_class_list->GetNext(pos);
		//			tb_class->AddTableSpa(file, get_f_prj_name(), get_f_class_name());
		//		}
		//	}
		//}
		
		if (tb_item_list.GetCount() > 0)
		{
			if (fwprintf(file, _T("%s\n"), _T("COMMIT;")) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}
			if (fwprintf(file, _T("%s\n"), _T("BEGIN;")) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}

			pos = tb_item_list.GetHeadPosition();
			while (pos)
			{
				tb_item = (TB_Item*) tb_item_list.GetNext(pos);
				tb_item->WriteSQLPostgres(file, get_f_prj_name(), get_f_class_name(), &tb_attribute_list, &tb_item_attribute_list, reserved_list, srid, pTempConn);
			}
		
			if (fwprintf(file, _T("%s\n"), _T("COMMIT;")) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}
			if (fwprintf(file, _T("%s\n"), _T("BEGIN;")) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}
		}

		if ((ptb_class_list) && (ptb_class_list->GetCount() > 0))
		{
			pos = ptb_class_list->GetHeadPosition();
			while (pos)
			{
				tb_class = (TB_Class*) ptb_class_list->GetNext(pos);
				tb_class->CreateTablePostgres(file, create1, true, srid, reserved_list, pTempConn);
			}
		}

		if (create1)
		{
			if ((ptb_class_list) && (ptb_class_list->GetCount() == 1) && ((get_f_class_type() == "O") || (get_f_class_type() == "P") || (get_f_class_type() == "T")))
			{
				if (fwprintf(file, _T("%s\n"), _T("COMMIT;")) < 0)
				{
					if (gsc_fclose(file) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
				if (fwprintf(file, _T("%s\n"), _T("BEGIN;")) < 0)
				{
					if (gsc_fclose(file) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				pos = ptb_class_list->GetHeadPosition();
				tb_class = (TB_Class*) ptb_class_list->GetAt(pos);
				tb_class_name = tb_class->get_f_class_name(); 
				
				//-- View
				str = "CREATE OR REPLACE VIEW ";
				str += get_f_prj_name(); 
				//str += "@SCHEMA@";
				str += ".";
				str += correctString4(get_f_class_name());

				if ((get_f_class_type() == "O") || (get_f_class_type() == "P"))
					str += "_tbl_v AS SELECT ";
				if (get_f_class_type() == "T")
					str += "_v AS SELECT ";

				str += correctString4(tb_class_name);
				str += ".*";

				str1 = "";
				if (tb_item_attribute_list.GetCount() > 0)
				{
					pos = tb_item_attribute_list.GetHeadPosition();
					while (pos)
					{
						tb_attribute = (TB_Attribute*) tb_item_attribute_list.GetNext(pos);
						
						found = false;	
						pos1 = tb_class->tb_item_attribute_list.GetHeadPosition();
						while (pos1)
						{
							tb_attribute1 = (TB_Attribute*) tb_class->tb_item_attribute_list.GetNext(pos1);
							if (tb_attribute->get_name() == tb_attribute1->get_name())
							{
								found = true;
								break;
							}
						}
						if (!found)
						{
							pos1 = tb_class->tb_attribute_list.GetHeadPosition();
							while (pos1)
							{
								tb_attribute1 = (TB_Attribute*) tb_class->tb_attribute_list.GetNext(pos1);
								if (tb_attribute->get_name() == tb_attribute1->get_name())
								{
									found = true;
									break;
								}
							}
						}

						if (!found)
						{
							if (str1.len() > 0)
								str1 += ", ";
							str1 += correctString4(get_f_class_name());
							str1 += ".";
							attr = correctString2(correctString1(tb_attribute->get_name()));
							str1 += attr.tolower();
						}
					}
				}
				if (tb_attribute_list.GetCount() > 0)
				{
					pos = tb_attribute_list.GetHeadPosition();
					while (pos)
					{
						tb_attribute = (TB_Attribute*) tb_attribute_list.GetNext(pos);
						
						found = false;	
						pos1 = tb_class->tb_item_attribute_list.GetHeadPosition();
						while (pos1)
						{
							tb_attribute1 = (TB_Attribute*) tb_class->tb_item_attribute_list.GetNext(pos1);
							if (tb_attribute->get_name() == tb_attribute1->get_name())
							{
								found = true;
								break;
							}
						}
						if (!found)
						{
							pos1 = tb_class->tb_attribute_list.GetHeadPosition();
							while (pos1)
							{
								tb_attribute1 = (TB_Attribute*) tb_class->tb_attribute_list.GetNext(pos1);
								if (tb_attribute->get_name() == tb_attribute1->get_name())
								{
									found = true;
									break;
								}
							}
						}

						if (!found)
						{
							if (str1.len() > 0)
								str1 += ", ";
							str1 += correctString4(get_f_class_name());
							str1 += ".\"";
							// le " " vengono introdotte a prescindere dalla presenza 
							// di caratteri speciali nella stringa
							attr = correctString1(tb_attribute->get_name());
							attr.tolower();
							pos2 = reserved_list->Find(attr.get_name());
							if (pos2)
								attr += "_";
							str1 += attr.tolower();
							str1 += "\"";
						}
					}
				}
				if (str1.len() > 0)
				{
					str += ", ";
					str += str1;
				}
				str += " FROM ";
				str += get_f_prj_name(); 
				str += ".";
				str += correctString4(tb_class_name);
				str += " JOIN ";
				str += get_f_prj_name(); 
				str += ".";
				str += correctString4(get_f_class_name());
				str += " ON ";
				str += correctString4(tb_class_name);
				str += ".fid_parent = ";
				str += correctString4(get_f_class_name());
				str += ".fid;";

				if (fwprintf(file, _T("%s\n"), str) < 0)
				{
					if (gsc_fclose(file) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				if (fwprintf(file, _T("%s\n"), _T("COMMIT;")) < 0)
				{
					if (gsc_fclose(file) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
				if (fwprintf(file, _T("%s\n"), _T("BEGIN;")) < 0)
				{
					if (gsc_fclose(file) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				str = "INSERT INTO public.geometry_columns";
				str += " (f_table_catalog, f_table_schema, f_table_name, f_geometry_column, coord_dimension, srid, type) VALUES ('', '";
				str += get_f_prj_name();
				str += "', '";
				str += correctString4(get_f_class_name());

				if ((get_f_class_type() == "O") || (get_f_class_type() == "P"))
				{
					str += "_tbl_v', 'wkb_geometry', 2, ";
					str += srid;
					str += ", '";
				}
				if (get_f_class_type() == "T")
				{
					str += "_v', 'wkb_geometry', 2, ";
					str += srid;
					str += ", '";
				}
				
				if (tb_class->get_f_class_type() == "A")
					str += "POINT";
				else
					str += "LINESTRING";
				str += "');";

				if (fwprintf(file, _T("%s\n"), str) < 0)
				{
					if (gsc_fclose(file) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}

				if (fwprintf(file, _T("%s\n"), _T("COMMIT;")) < 0)
				{
					if (gsc_fclose(file) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
				if (fwprintf(file, _T("%s\n"), _T("BEGIN;")) < 0)
				{
					if (gsc_fclose(file) == GS_BAD)
						AfxThrowUserException();
					AfxThrowUserException();
				}
			
				if ((get_f_class_type() == "T") && (pCls) && (pCls->get_category() != CAT_SPAGHETTI) && (pCls->ptr_id()->type == TYPE_POLYLINE))
				{
					pos = ptb_class_list->GetHeadPosition();
					tb_class = (TB_Class*) ptb_class_list->GetAt(pos);
					pos = tb_class->ptb_class_list->GetHeadPosition();
					tb_class = (TB_Class*) tb_class->ptb_class_list->GetAt(pos);
					tb_class_name = tb_class->get_f_class_name(); 
					
					//-- View
					str = "CREATE OR REPLACE VIEW ";
					str += get_f_prj_name(); 
					//str += "@SCHEMA@";
					str += ".";
					str += correctString4(get_f_class_name());
					str += "_l_tbl_v AS SELECT ";

					str += correctString4(tb_class_name);
					str += ".*";

					str1 = "";
					if (tb_item_attribute_list.GetCount() > 0)
					{
						pos = tb_item_attribute_list.GetHeadPosition();
						while (pos)
						{
							tb_attribute = (TB_Attribute*) tb_item_attribute_list.GetNext(pos);
							
							found = false;	
							pos1 = tb_class->tb_item_attribute_list.GetHeadPosition();
							while (pos1)
							{
								tb_attribute1 = (TB_Attribute*) tb_class->tb_item_attribute_list.GetNext(pos1);
								if (tb_attribute->get_name() == tb_attribute1->get_name())
								{
									found = true;
									break;
								}
							}
							if (!found)
							{
								pos1 = tb_class->tb_attribute_list.GetHeadPosition();
								while (pos1)
								{
									tb_attribute1 = (TB_Attribute*) tb_class->tb_attribute_list.GetNext(pos1);
									if (tb_attribute->get_name() == tb_attribute1->get_name())
									{
										found = true;
										break;
									}
								}
							}

							if (!found)
							{
								if (str1.len() > 0)
									str1 += ", ";
								str1 += correctString4(get_f_class_name());
								str1 += ".";
								attr = correctString2(correctString1(tb_attribute->get_name()));
								str1 += attr.tolower();
							}
						}
					}
					if (tb_attribute_list.GetCount() > 0)
					{
						pos = tb_attribute_list.GetHeadPosition();
						while (pos)
						{
							tb_attribute = (TB_Attribute*) tb_attribute_list.GetNext(pos);
							
							found = false;	
							pos1 = tb_class->tb_item_attribute_list.GetHeadPosition();
							while (pos1)
							{
								tb_attribute1 = (TB_Attribute*) tb_class->tb_item_attribute_list.GetNext(pos1);
								if (tb_attribute->get_name() == tb_attribute1->get_name())
								{
									found = true;
									break;
								}
							}
							if (!found)
							{
								pos1 = tb_class->tb_attribute_list.GetHeadPosition();
								while (pos1)
								{
									tb_attribute1 = (TB_Attribute*) tb_class->tb_attribute_list.GetNext(pos1);
									if (tb_attribute->get_name() == tb_attribute1->get_name())
									{
										found = true;
										break;
									}
								}
							}

							if (!found)
							{
								if (str1.len() > 0)
									str1 += ", ";
								str1 += correctString4(get_f_class_name());
								str1 += ".\"";
								// le " " vengono introdotte a prescindere dalla presenza 
								// di caratteri speciali nella stringa
								attr = correctString1(tb_attribute->get_name());
								attr.tolower();
								pos2 = reserved_list->Find(attr.get_name());
								if (pos2)
									attr += "_";
								str1 += attr.tolower();
								str1 += "\"";
							}
						}
					}
					if (str1.len() > 0)
					{
						str += ", ";
						str += str1;
					}
					str += " FROM ";
					str += get_f_prj_name(); 
					str += ".";
					str += correctString4(tb_class_name);
					str += " JOIN ";
					str += get_f_prj_name(); 
					str += ".";
					str += correctString4(get_f_class_name());
					str += " ON ";
					str += correctString4(tb_class_name);
					str += ".fid_parent = ";
					str += correctString4(get_f_class_name());
					str += ".fid;";

					if (fwprintf(file, _T("%s\n"), str) < 0)
					{
						if (gsc_fclose(file) == GS_BAD)
							AfxThrowUserException();
						AfxThrowUserException();
					}

					if (fwprintf(file, _T("%s\n"), _T("COMMIT;")) < 0)
					{
						if (gsc_fclose(file) == GS_BAD)
							AfxThrowUserException();
						AfxThrowUserException();
					}
					if (fwprintf(file, _T("%s\n"), _T("BEGIN;")) < 0)
					{
						if (gsc_fclose(file) == GS_BAD)
							AfxThrowUserException();
						AfxThrowUserException();
					}

					str = "INSERT INTO public.geometry_columns";
					str += " (f_table_catalog, f_table_schema, f_table_name, f_geometry_column, coord_dimension, srid, type) VALUES ('', '";
					str += get_f_prj_name();
					str += "', '";
					str += correctString4(get_f_class_name());
					str += "_l_tbl_v', 'wkb_geometry', 2, ";
					str += srid;
					str += ", '";
					
					if (tb_class->get_f_class_type() == "A")
						str += _T("POINT");
					else
						str += _T("LINESTRING");
					str += _T("');");

					if (fwprintf(file, _T("%s\n"), str) < 0)
					{
						if (gsc_fclose(file) == GS_BAD)
							AfxThrowUserException();
						AfxThrowUserException();
					}

					if (fwprintf(file, _T("%s\n"), _T("COMMIT;")) < 0)
					{
						if (gsc_fclose(file) == GS_BAD)
							AfxThrowUserException();
						AfxThrowUserException();
					}
					if (fwprintf(file, _T("%s\n"), _T("BEGIN;")) < 0)
					{
						if (gsc_fclose(file) == GS_BAD)
							AfxThrowUserException();
						AfxThrowUserException();
					}
				}
			}
		}

		if ((ptb_sec_list) && (ptb_sec_list->GetCount() > 0))
		{
			pos = ptb_sec_list->GetHeadPosition();
			while (pos)
			{
				tb_class = (TB_Class*) ptb_sec_list->GetNext(pos);
				tb_class->CreateTablePostgres(file, create1, false, srid, reserved_list, pTempConn);
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

int TB_Class::CreateTableOracle(FILE *file, bool create1, bool create2, CStringList *reserved_list,  C_DBCONNECTION *pConn = NULL)
{                                               
	int ret = GS_GOOD;
	C_STRING str, str1, str2, tb_class_name, attr;
	POSITION	pos, pos2;
	TB_Attribute *tb_attribute;
	TB_Item		*tb_item;
	TB_Class	*tb_class=NULL;
	C_DBCONNECTION *pTempConn;

	TRY
	{
		pTempConn = pConn;
		if ((!pConn) && pCls)
		{
			if (pCls->get_category() != CAT_SPAGHETTI)
				pTempConn = pCls->ptr_info()->getDBConnection(OLD);
		}

		if (create1)
		{
			//str = "SELECT gw_droptable('";
			//str += get_f_prj_name();
			//str += "', '";
			//str += correctString4(get_f_class_name());
			//str += "');";
			//if (fwprintf(file, _T("%s\n"), str) < 0)
			//{
			//	if (gsc_fclose(file) == GS_BAD)
			//		AfxThrowUserException();
			//	AfxThrowUserException();
			//}

			str = "CREATE TABLE \"";
			str2 = get_f_prj_name();
			str2.toupper();
			str += str2;
			str += "\".\"";
			str2 = correctString4(get_f_class_name());
			str2.toupper();
			str += str2;
			str += "\" (\"";

			if (tb_item_attribute_list.GetCount() > 0)
			{
				pos = tb_item_attribute_list.GetHeadPosition();
				while (pos)
				{
					tb_attribute = (TB_Attribute*) tb_item_attribute_list.GetNext(pos);			
					
					if (str1.len() > 0)
						str1 += ", \"";
					str2 = tb_attribute->get_name();
					str2.toupper();
					str1 += str2;
					str1 += "\" ";
					str1 += tb_attribute->get_type(true);
				}
			}
			if (tb_attribute_list.GetCount() > 0)
			{
				pos = tb_attribute_list.GetHeadPosition();
				while (pos)
				{
					tb_attribute = (TB_Attribute*) tb_attribute_list.GetNext(pos);			
					
					if (str1.len() > 0)
						str1 += ", \"";
					
					str2 = tb_attribute->get_name();
					str2.tolower();
					pos2 = reserved_list->Find(str2.get_name());
					if (pos2)
						str2 += "_";
					// le " " vengono introdotte a prescindere dalla presenza 
					// di caratteri speciali nella stringa
					str2 = correctString1(str2);
					str2.toupper();

					str1 += str2;
					str1 += "\" ";
					str1 += tb_attribute->get_type(true);
				}
			}	
			str += str1;

			// solo se TB_Item_Label, TB_Item_Point, TB_Item_Surface, TB_Item_Polyline_L
			if ((get_f_class_type() == "A") || (get_f_class_type() == "P") || (get_f_class_type() == "O") || (get_f_class_type() == "L"))
			{
				str += ", \"GEOM\" SDO_GEOMETRY";
			}

			str += ", CONSTRAINT PKEY_";
			str2 = get_f_class_name();
			str2.toupper();
			str += str2;
			str += " PRIMARY KEY (\"FID\")";
			str += ");";
			if (fwprintf(file, _T("%s\n"), str) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}
		}

		// solo se TB_Item_Label, TB_Item_Point, TB_Item_Surface, TB_Item_Polyline_L
		if (get_f_class_type() == "A" || get_f_class_type() == "P" || get_f_class_type() == "O" || get_f_class_type() == "L")
		{
			pos = tb_item_attribute_list.GetHeadPosition();
			tb_attribute = new TB_Attribute("geom", "sdo_geometry", "", 0, 0, true, "");
			tb_item_attribute_list.InsertAfter(pos, tb_attribute);
		}

		if (tb_item_list.GetCount() > 0)
		{
			if (fwprintf(file, _T("%s\n"), _T("COMMIT;")) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}
			if (fwprintf(file, _T("%s\n"), _T("BEGIN;")) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}

			pos = tb_item_list.GetHeadPosition();
			while (pos)
			{
				tb_item = (TB_Item*) tb_item_list.GetNext(pos);
				tb_item->WriteSQLOracle(file, get_f_prj_name(), get_f_class_name(), &tb_attribute_list, &tb_item_attribute_list, reserved_list, pTempConn);
			}
		
			if (fwprintf(file, _T("%s\n"), _T("COMMIT;")) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}
			if (fwprintf(file, _T("%s\n"), _T("BEGIN;")) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}
		}

		if ((ptb_class_list) && (ptb_class_list->GetCount() > 0))
		{
			pos = ptb_class_list->GetHeadPosition();
			while (pos)
			{
				tb_class = (TB_Class*) ptb_class_list->GetNext(pos);
				tb_class->CreateTableOracle(file, create1, true, reserved_list, pTempConn);
			}
		}

		if ((ptb_sec_list) && (ptb_sec_list->GetCount() > 0))
		{
			pos = ptb_sec_list->GetHeadPosition();
			while (pos)
			{
				tb_class = (TB_Class*) ptb_sec_list->GetNext(pos);
				tb_class->CreateTableOracle(file, create1, false, reserved_list, pTempConn);
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

int TB_Class::CreateTableSecsExtPostgres(FILE *file, C_STRING &prj_name)
{                                               
	int ret = GS_GOOD;

	POSITION pos;

	TB_Class	*tb_class=NULL;

	C_SECONDARY	*pSec;
	
	C_DBCONNECTION *pConn;

	C_STRING 		table;

	TRY
	{
		if ((ptb_sec_list) && (ptb_sec_list->GetCount() > 0))
		{
			pos = ptb_sec_list->GetHeadPosition();
			while (pos)
			{
				tb_class = (TB_Class*) ptb_sec_list->GetNext(pos);
				tb_class->set_f_prj_name(prj_name);
	
				pSec = (C_SECONDARY*) pCls->find_sec(tb_class->get_f_class_id());
				pConn = pSec->ptr_info()->getDBConnection(OLD);
				
				table = pSec->ptr_info()->OldTableRef.get_name();
				table.strtran(_T("["), _T(""));
				table.strtran(_T("]"), _T(""));
				tb_class->set_f_class_name(table);
				tb_class->set_caption(table);
				tb_class->CreateTableSecExtPostgres(file, pConn);
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

int TB_Class::CreateTableSecsExtOracle(FILE *file, C_STRING &prj_name)
{                                               
	int ret = GS_GOOD;

	POSITION pos;

	TB_Class	*tb_class=NULL;

	C_SECONDARY	*pSec;
	
	C_DBCONNECTION *pConn;

	C_STRING		table;

	TRY
	{
		if ((ptb_sec_list) && (ptb_sec_list->GetCount() > 0))
		{
			pos = ptb_sec_list->GetHeadPosition();
			while (pos)
			{
				tb_class = (TB_Class*) ptb_sec_list->GetNext(pos);
				tb_class->set_f_prj_name(prj_name);

				pSec = (C_SECONDARY*) pCls->find_sec(tb_class->get_f_class_id());
				pConn = pSec->ptr_info()->getDBConnection(OLD);
				
				table = pSec->ptr_info()->OldTableRef.get_name();
				table.strtran(_T("["), _T(""));
				table.strtran(_T("]"), _T(""));
				tb_class->set_f_class_name(table);
				tb_class->set_caption(table);
				tb_class->CreateTableSecExtOracle(file, pConn);
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

int TB_Class::CreateTableSecExtPostgres(FILE *file, C_DBCONNECTION *pConn)
{                                               
	int ret = GS_GOOD;

	C_STRING str, str1, str2;

	POSITION pos;

	TB_Attribute *tb_attribute;

	TB_Item_Sec  *tb_item_sec;

	TRY
	{
		str = "SELECT gw_droptable('";
		str += get_f_prj_name();
		//str += "@SCHEMA@";
		str += "', '";
		str += correctString4(get_f_class_name());
		str += "');";
		if (fwprintf(file, _T("%s\n"), str) < 0)
		{
			if (gsc_fclose(file) == GS_BAD)
				AfxThrowUserException();
			AfxThrowUserException();
		}

		str = "CREATE TABLE ";
		str += get_f_prj_name();
		//str += "@SCHEMA@";
		str += ".";
		str += correctString4(get_f_class_name());
		str += " (\"";
		if (tb_attribute_list.GetCount() > 0)
		{
			pos = tb_attribute_list.GetHeadPosition();
			while (pos)
			{
				tb_attribute = (TB_Attribute*) tb_attribute_list.GetNext(pos);			
				
				if (str1.len() > 0)
					str1 += ", \"";

				str2 = tb_attribute->get_name();
				// le " " vengono introdotte a prescindere dalla presenza 
				// di caratteri speciali nella stringa
				str2 = correctString1(str2);
				str2.tolower();

				str1 += str2;
				str1 += "\" ";
				str1 += tb_attribute->get_type();
			}
		}	
		str += str1;
		//str += ", CONSTRAINT pkey_";
		//str += get_f_class_name();
		//str += " PRIMARY KEY (fid)";
		str += ");";
		if (fwprintf(file, _T("%s\n"), str) < 0)
		{
			if (gsc_fclose(file) == GS_BAD)
				AfxThrowUserException();
			AfxThrowUserException();
		}

		if (tb_item_list.GetCount() > 0)
		{
			pos = tb_item_list.GetHeadPosition();
			while (pos)
			{
				tb_item_sec = (TB_Item_Sec*) tb_item_list.GetNext(pos);
				tb_item_sec->WriteSQLPostgres(file, get_f_prj_name(), get_f_class_name(), &tb_attribute_list, pConn);
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

int TB_Class::CreateTableSecExtOracle(FILE *file, C_DBCONNECTION *pConn)
{                                               
	int ret = GS_GOOD;

	C_STRING str, str1, str2;

	POSITION pos;

	TB_Attribute *tb_attribute;

	TB_Item_Sec  *tb_item_sec;

	TRY
	{
		str = "SELECT gw_droptable('";
		str += get_f_prj_name();
		//str += "@SCHEMA@";
		str += "', '";
		str += correctString4(get_f_class_name());
		str += "');";
		if (fwprintf(file, _T("%s\n"), str) < 0)
		{
			if (gsc_fclose(file) == GS_BAD)
				AfxThrowUserException();
			AfxThrowUserException();
		}

		str = "CREATE TABLE ";
		str += get_f_prj_name();
		//str += "@SCHEMA@";
		str += ".";
		str += correctString4(get_f_class_name());
		str += " (\"";
		if (tb_attribute_list.GetCount() > 0)
		{
			pos = tb_attribute_list.GetHeadPosition();
			while (pos)
			{
				tb_attribute = (TB_Attribute*) tb_attribute_list.GetNext(pos);			
				
				if (str1.len() > 0)
					str1 += ", \"";

				str2 = tb_attribute->get_name();
				// le " " vengono introdotte a prescindere dalla presenza 
				// di caratteri speciali nella stringa
				str2 = correctString1(str2);
				str2.tolower();

				str1 += str2;
				str1 += "\" ";
				str1 += tb_attribute->get_type(true);
			}
		}	
		str += str1;
		//str += ", CONSTRAINT pkey_";
		//str += get_f_class_name();
		//str += " PRIMARY KEY (fid)";
		str += ");";
		if (fwprintf(file, _T("%s\n"), str) < 0)
		{
			if (gsc_fclose(file) == GS_BAD)
				AfxThrowUserException();
			AfxThrowUserException();
		}

		if (tb_item_list.GetCount() > 0)
		{
			pos = tb_item_list.GetHeadPosition();
			while (pos)
			{
				tb_item_sec = (TB_Item_Sec*) tb_item_list.GetNext(pos);
				tb_item_sec->WriteSQLOracle(file, get_f_prj_name(), get_f_class_name(), &tb_attribute_list, pConn);
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

//int TB_Class::AddTableSpa(FILE *file, C_STRING _prjname, C_STRING _name)
//{                                               
//	int ret = GS_GOOD;
//
//	POSITION pos;
//
//	TB_Item		*tb_item;
//
//	TRY
//	{
//		if (tb_item_list.GetCount() > 0)
//		{
//			pos = tb_item_list.GetHeadPosition();
//			while (pos)
//			{
//				tb_item = (TB_Item*) tb_item_list.GetNext(pos);
//				tb_item->WriteSQL(file, _prjname, _name);
//			}
//		}
//	}
//	CATCH( CUserException, e )
//	{
//		ret=GS_BAD;
//	}
//	END_CATCH
//
//	return ret;
//}

int TB_Class::LoadPoints(long *current_id1, long *current_id2, CStringList *reserved_list)
{                                               
	int ret = GS_GOOD;

	C_RB_LIST	entMask;
	C_SELSET	selSet;

	long i;
	
	ads_name	ent;
	C_EED		eed;
	
	AcDbObjectId			objectId, objectId1;
	AcDbEntity				*pEntity;
	AcDbBlockReference		*pBlockReference;	
	AcDbBlockTableRecord	*pBlockTableRecord;
	AcGeScale3d				scale3d;
	AcGePoint3d				point3d;

	TCHAR		*str;

	long		_fid, _tfid;

	C_STRING block_name, layer;
	C_STRING block_name_, layer_;
	C_COLOR n_color;
	double  orientation;

	TB_Item_Point	*tb_item_point = NULL;
	TB_Item_Label	*tb_item_label = NULL;

	TB_Class	*tb_class = NULL;

	POSITION	pos;

	TRY
	{	
		tb_item_point = new TB_Item_Point();
		tb_item_point->get_item_attribute_list(&tb_item_attribute_list);

		ptb_class_list = new TB_Class_List();

		tb_class = new TB_Class(NULL, reserved_list);
		tb_class->set_f_class_id(get_f_class_id());
		tb_class->set_f_prj_name(get_f_prj_name());
		tb_class->set_f_class_name(get_f_class_name() + "_tbl");
		tb_class->set_caption(get_f_class_name() + "_tbl");
		tb_item_label = new TB_Item_Label();
		tb_item_label->get_item_attribute_list(&tb_class->tb_item_attribute_list);
		tb_class->set_f_class_type(_T("A"));

		ptb_class_list->AddTail(tb_class);

		// maschera per filtrare le entità esclusi i "$T", "GEOTHM", "USRTHM"
		if ((entMask << acutBuildList(-4, _T("<and"),
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
			AfxThrowUserException();
   
		_tfid = 0;
		//	ottiene la lista di tutte le entità esclusi i $T
		if (gsc_ssget(_T("_X"), NULL, NULL, entMask.get_head(), selSet) == RTNORM) 
		{
			_tfid = *current_id2;

			i=0;		
			// per ogni elemento
			while (selSet.entname(i++, ent) == GS_GOOD)
			{
				if (eed.load(ent) == GS_GOOD)
				{
					if ((eed.cls == pCls->ptr_id()->code) && (eed.sub == pCls->ptr_id()->sub_code))
					{
						_fid = eed.gs_id;

						tb_item_point = NULL;
						tb_item_point = (TB_Item_Point*) tb_item_list.find(_fid);

						// Ricavo le informazioni dell'entità
						if (acdbGetObjectId(objectId, ent) == Acad::eOk)
						{
							if (acdbOpenObject(pEntity, objectId, AcDb::kForRead) == Acad::eOk)
							{
								if (pEntity->isKindOf(AcDbBlockReference::desc()))
								{
									pBlockReference = AcDbBlockReference::cast(pEntity);

									objectId1 = pBlockReference->blockTableRecord();
									if (acdbOpenObject((AcDbObject *&)pBlockTableRecord, objectId1, AcDb::kForRead) != Acad::eOk) 
										break;
									pBlockTableRecord->getName(str);
                           C_STRING dummy(str);
									block_name = correctString1(dummy);
									acdbFree(str);
									pBlockTableRecord->close();

									layer = pBlockReference->layer();
									n_color.setAutoCADColorIndex(pBlockReference->colorIndex());
									orientation = pBlockReference->rotation();
									orientation = gsc_rad2grd(orientation);
									point3d = pBlockReference->position();
									scale3d = pBlockReference->scaleFactors();
		
									pBlockReference->close();

									_tfid++;
									tb_item_point = NULL;
									tb_item_point = new TB_Item_Point(_fid, orientation, point3d.x, point3d.y, point3d.z, _tfid);

									block_name_ = block_name.toupper();
									tb_item_point->set_block_name(block_name_);

									layer_ = layer.toupper();
									tb_item_point->set_layer(layer_);

									tb_item_point->set_color(n_color);

									tb_item_point->set_block_scale_factor(scale3d.sx);
									
									UpdateAttribs(tb_item_point);
									// PAOLETTA
									// per gestire gli oggetti puntuali aggregati
									tb_item_point->set_fid(_tfid);
									tb_item_point->set_fid_parent(_fid);
									tb_item_list.AddTail(tb_item_point);
								}

								pEntity->close();
							}
						}			
					}
				}
			}
		
			*current_id2 = _tfid;
		}

   		//	maschera per filtrare i blocchi $T
		if ((entMask << acutBuildList(RTDXF0, _T("INSERT"), 2, _T("$T"), 0)) == NULL)
			AfxThrowUserException();
		if (gsc_ssget(_T("_X"), NULL, NULL, entMask.get_head(), selSet) == RTNORM) 
			LoadBlocks(selSet, tb_class, pCls, current_id1);

		// Per gestire la particolarità di blocchi attributi spenti,
		// ovvero per un oggetto puntuale con blocco attributi spento e che quindi 
		// non viene trattato nella funzione LoadBlocks, rimane assegnato il valore 
		// fid_parent, che per gli oggetti puntuali dovrebbe essere settato a -1
		// altrimenti errori nella stringa SQL
		tb_item_point = NULL;
		if (tb_item_list.GetCount() > 0)
		{
			pos = tb_item_list.GetHeadPosition();
			while (pos)
			{
				tb_item_point = (TB_Item_Point*) tb_item_list.GetNext(pos);
				if (tb_item_point->IsKindOf(RUNTIME_CLASS(TB_Item_Point)))
					tb_item_point->set_fid_parent(-1);
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

int TB_Class::LoadTexts(long *current_id1, CStringList *reserved_list)
{                                               
	int ret = GS_GOOD;

	C_RB_LIST	entMask;
	C_SELSET	selSet;

	long i;
	
	ads_name	ent;
	C_EED		eed;
	
	AcDbObjectId	objectId;
	AcDbEntity		*pEntity;
	AcDbText		*pText;
	AcDbMText		*pMText;

	long	_fid;
	C_STRING	text;

	TB_Item_Text	*tb_item_text = NULL;
	TB_Item_Label	*tb_item_label = NULL;

	TB_Class	*tb_class = NULL;

	TRY
	{
		tb_item_text = new TB_Item_Text();
		tb_item_text->get_item_attribute_list(&tb_item_attribute_list);

		ptb_class_list = new TB_Class_List();

		tb_class = new TB_Class(NULL, reserved_list);
		tb_class->set_f_class_id(get_f_class_id());
		tb_class->set_f_prj_name(get_f_prj_name());
		tb_class->set_f_class_name(get_f_class_name() + "_tbl");
		tb_class->set_caption(get_f_class_name() + "_tbl");
		tb_item_label = new TB_Item_Label();
		tb_item_label->get_item_attribute_list(&tb_class->tb_item_attribute_list);
		tb_class->set_f_class_type(_T("A"));

		ptb_class_list->AddTail(tb_class);

		if ((entMask << acutBuildList(-4, _T("<or"),
										RTDXF0, _T("TEXT"),
										-4, _T("<or"),
										RTDXF0, _T("MTEXT"),
										-4, _T("or>"),
										-4, _T("or>"), 0)) == NULL)
			AfxThrowUserException();

		//	ottiene la lista di tutte le entità esclusi i $T
		if (gsc_ssget(_T("_X"), NULL, NULL, entMask.get_head(), selSet) == RTNORM) 
		{
			i=0;		
			// per ogni elemento
			while (selSet.entname(i++, ent) == GS_GOOD)
			{
				if (eed.load(ent) == GS_GOOD)
				{
					if ((eed.cls == pCls->ptr_id()->code) && (eed.sub == pCls->ptr_id()->sub_code))
					{
						_fid = eed.gs_id;

						tb_item_text = NULL;
						tb_item_text = (TB_Item_Text*) tb_item_list.find(_fid);
						if (!tb_item_text)
						{
							// Ricavo le informazioni dell'entità
							if (acdbGetObjectId(objectId, ent) == Acad::eOk)
							{
								if (acdbOpenObject(pEntity, objectId, AcDb::kForRead) == Acad::eOk)
								{
									if (pEntity->isKindOf(AcDbText::desc())) 
									{
										pText = AcDbText::cast(pEntity);

										text = pText->textString();

										tb_item_text = new TB_Item_Text(_fid, text);
										UpdateAttribs(tb_item_text);
										tb_item_list.AddTail(tb_item_text);
									}
									else if (pEntity->isKindOf(AcDbMText::desc()))
									{
										pMText = AcDbMText::cast(pEntity);

										text = pMText->contents();
										// PAOLETTA

										tb_item_text = new TB_Item_Text(_fid, text);
										UpdateAttribs(tb_item_text);
										tb_item_list.AddTail(tb_item_text);
									}

									pEntity->close();
								}
							}
						}
					}
				}
			}
		}

		//	ottiene la lista di tutti i testi
		if (gsc_ssget(_T("_X"), NULL, NULL, entMask.get_head(), selSet) == RTNORM) 
			LoadBlocks(selSet, tb_class, pCls, current_id1);
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

int TB_Class::LoadSurfaces(bool isOracle, long *current_id1, CStringList *reserved_list)
{                                               
	int ret = GS_GOOD;

	C_RB_LIST	entMask;
	C_SELSET	selSet;

	long i;
	
	ads_name	ent;
	C_EED		eed;
	
	AcDbObjectId	objectId;
	AcDbEntity		*pEntity;
	AcDbPolyline	*pPolyline;
	AcDb3dPolyline	*p3dPolyline;
	AcDb2dPolyline	*p2dPolyline;

	AcDbHatch		*pHatch;
	AcGePoint3d		point3d, closePoint3d, startPoint3d, endPoint3d;
	AcGeCircArc2d	circArc2d;

	long		_fid;

	C_STRING l_type, layer, fl_type;
	C_STRING l_type_, layer_;
	C_COLOR n_color;
	double  thickness, l_width;

	C_STRING hatch_name, hatch_l_type;
	C_STRING hatch_name_;
	C_COLOR hatch_color;
	double  hatch_space;

	TB_Item_Surface	*tb_item_surface = NULL;
	TB_Item_Label	*tb_item_label = NULL;

	TB_Class	*tb_class = NULL;

	C_FAS		*pFas = NULL;

	CList<AcGePoint3d,AcGePoint3d>	point_list;
	double		length;
	double		area;

	TRY
	{
		pFas = pCls->ptr_fas();

		tb_item_surface = new TB_Item_Surface();
		tb_item_surface->get_item_attribute_list(&tb_item_attribute_list);

		ptb_class_list = new TB_Class_List();

		tb_class = new TB_Class(NULL, reserved_list);
		tb_class->set_f_class_id(get_f_class_id());
		tb_class->set_f_prj_name(get_f_prj_name());
		tb_class->set_f_class_name(get_f_class_name() + "_tbl");
		tb_class->set_caption(get_f_class_name() + "_tbl");
		tb_item_label = new TB_Item_Label();
		tb_item_label->get_item_attribute_list(&tb_class->tb_item_attribute_list);
		tb_class->set_f_class_type(_T("A"));

		ptb_class_list->AddTail(tb_class);

		// maschera per filtrare le entità esclusi i "$T", "GEOTHM", "USRTHM"
		if ((entMask << acutBuildList(-4, _T("<or"),
										RTDXF0, _T("POLYLINE"),
										-4, _T("<or"),
										RTDXF0, _T("LWPOLYLINE"),
										-4, _T("<or"),
										RTDXF0, _T("LINE"),
										-4, _T("<or"),
										RTDXF0, _T("HATCH"),
										-4, _T("or>"),
										-4, _T("or>"),
										-4, _T("or>"),
										-4, _T("or>"), 0)) == NULL)
			AfxThrowUserException();

		//	ottiene la lista di tutte le entità esclusi i $T
		if (gsc_ssget(_T("_X"), NULL, NULL, entMask.get_head(), selSet) == RTNORM) 
		{
			i=0;		
			// per ogni elemento
			while (selSet.entname(i++, ent) == GS_GOOD)
			{
				if (eed.load(ent) == GS_GOOD)
				{
					if ((eed.cls == pCls->ptr_id()->code) && (eed.sub == pCls->ptr_id()->sub_code))
					{
						_fid = eed.gs_id;
						tb_item_surface = NULL;
						tb_item_surface = (TB_Item_Surface*) tb_item_list.find(_fid);

						// Ricavo le informazioni dell'entità
						if (acdbGetObjectId(objectId, ent) == Acad::eOk)
						{
							if (acdbOpenObject(pEntity, objectId, AcDb::kForRead) == Acad::eOk)
							{
								if ((pEntity->isKindOf(AcDbPolyline::desc())) || (pEntity->isKindOf(AcDb3dPolyline::desc())) || (pEntity->isKindOf(AcDb2dPolyline::desc())))
								{
									if (pEntity->isKindOf(AcDbPolyline::desc()))
									{
										pPolyline = AcDbPolyline::cast(pEntity);

										n_color.setAutoCADColorIndex(pPolyline->colorIndex());
										l_type = pPolyline->linetype();
										layer = pPolyline->layer();
										thickness = pPolyline->thickness();
										l_width = 0;
										if (pPolyline->hasWidth())
											 pPolyline->getConstantWidth(l_width);

										pPolyline->close();
									} 
									else if (pEntity->isKindOf(AcDb3dPolyline::desc()))
									{
										p3dPolyline = AcDb3dPolyline::cast(pEntity);
									
										n_color.setAutoCADColorIndex(p3dPolyline->colorIndex());
										l_type = p3dPolyline->linetype();
										layer = p3dPolyline->layer();
										thickness = 0;
										l_width = 0;

										p3dPolyline->close();
									} 
									else if (pEntity->isKindOf(AcDb2dPolyline::desc()))
									{
										p2dPolyline = AcDb2dPolyline::cast(pEntity);
									
										n_color.setAutoCADColorIndex(p2dPolyline->colorIndex());
										l_type = p2dPolyline->linetype();
										layer = p2dPolyline->layer();
										thickness = p2dPolyline->thickness();
										l_width = 0;

										p2dPolyline->close();
									} 

									// Ricavo i vertici per TB eventualmente corretti
									point_list.RemoveAll();
									if ((GetPointsOfObj(pEntity, point_list, &length, &area)) && (point_list.GetCount() > 1))
									{
										startPoint3d = point_list.GetHead();
										endPoint3d = point_list.GetTail();
										if ((startPoint3d.x != endPoint3d.x) || (startPoint3d.y != endPoint3d.y))
											point_list.AddTail(startPoint3d);

										if (!tb_item_surface)
										{
											tb_item_surface = new TB_Item_Surface(isOracle, _fid, area, length, &point_list);

											layer_ = layer.toupper();	
											tb_item_surface->set_layer(layer_);
										
											fl_type = pCls->ptr_fas()->line;
											if (fl_type.toupper() == _T("DALAYER"))
												l_type = _T("BYLAYER");
											l_type_ = l_type.toupper();
											tb_item_surface->set_l_type(l_type);

											tb_item_surface->set_color(n_color);

											tb_item_surface->set_thickness(thickness);

											tb_item_surface->set_l_width(l_width);

											UpdateAttribs(tb_item_surface);
											tb_item_list.AddTail(tb_item_surface);
										}
										else if (tb_item_surface->is_old() == false)
										{
											layer_ = layer.toupper();
											tb_item_surface->set_layer(layer_);
										
											fl_type = pCls->ptr_fas()->line;
											if (fl_type.toupper() == "DALAYER")
												l_type = "BYLAYER";
											l_type_ = l_type.toupper();
											tb_item_surface->set_l_type(l_type_);

											tb_item_surface->set_color(n_color);

											tb_item_surface->set_thickness(thickness);

											tb_item_surface->set_l_width(l_width);

											tb_item_surface->update_area(area);
											tb_item_surface->update_length(length);
											tb_item_surface->Update(isOracle, &point_list);
										}
									}
								}
								else
								{
									if (pEntity->isKindOf(AcDbHatch::desc()))
									{
										pHatch = AcDbHatch::cast(pEntity);
										
										//pHatch->getArea(area);
										area = 0; // 26/11/07 Paola

										hatch_name = pHatch->patternName();
										hatch_space = pHatch->patternSpace();
										hatch_l_type = pHatch->linetype();
										hatch_color.setAutoCADColorIndex(pHatch->colorIndex());

										if (!tb_item_surface)
										{
											tb_item_surface = new TB_Item_Surface(_fid, area);

											hatch_name_ = hatch_name.toupper();
											tb_item_surface->set_hatch(hatch_name_);

											//hatch_l_width
											//hatch_l_type

											tb_item_surface->set_hatch_color(hatch_color);

											tb_item_surface->set_hatch_space(hatch_space);
											
											UpdateAttribs(tb_item_surface);
											tb_item_list.AddTail(tb_item_surface);
										} 
										else if (tb_item_surface->is_old() == false)										
										{
											hatch_name_ = hatch_name.toupper();
											tb_item_surface->set_hatch(hatch_name_);

											//hatch_l_width
											//hatch_l_type;

											tb_item_surface->set_hatch_color(hatch_color);

											tb_item_surface->set_hatch_space(hatch_space);
										}
									}
								}
							
								pEntity->close();
							}
						}
					}
				}
			}
		}

   		//	maschera per filtrare i blocchi $T
		if ((entMask << acutBuildList(RTDXF0, _T("INSERT"), 2, _T("$T"), 0)) == NULL)
			AfxThrowUserException();
		if (gsc_ssget(_T("_X"), NULL, NULL, entMask.get_head(), selSet) == RTNORM) 
			LoadBlocks(selSet, tb_class, pCls, current_id1);
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

int TB_Class::LoadPolylines(bool isOracle, long *current_id1, long *current_id2, CStringList *reserved_list)
{                                               
	int ret = GS_GOOD;

	C_RB_LIST	entMask;
	C_SELSET	selSet;

	long i;
	
	ads_name	ent;
	C_EED		eed;

	long		_fid, _tfid;

	AcDbObjectId	objectId;
	AcDbEntity		*pEntity;
    AcDbLine		*pLine;
	AcDbPolyline	*pPolyline;
	AcDb2dPolyline	*p2dPolyline;
	AcDb3dPolyline	*p3dPolyline;

	C_STRING layer, l_type, fl_type;
	C_STRING layer_, l_type_;
	C_COLOR n_color;
	double  thickness, l_width;

	CList<AcGePoint3d,AcGePoint3d>	point_list;
	double		length;
	double		area;

	TB_Item_Polyline	*tb_item_polyline = NULL;
	TB_Item_Polyline_L	*tb_item_polyline_l = NULL;
	TB_Item_Label		*tb_item_label = NULL;

	TB_Class	*tb_class = NULL, *tb_class1 = NULL;

	TRY
	{
		tb_item_polyline = new TB_Item_Polyline();
		tb_item_polyline->get_item_attribute_list(&tb_item_attribute_list);

		ptb_class_list = new TB_Class_List();
		tb_class = new TB_Class(NULL, reserved_list);
		tb_class->set_f_class_id(get_f_class_id());
		tb_class->set_f_prj_name(get_f_prj_name());
		tb_class->set_f_class_name(get_f_class_name() + "_l");
		tb_class->set_caption(get_f_class_name() + "_l");
		tb_item_polyline_l = new TB_Item_Polyline_L();
		tb_item_polyline_l->get_item_attribute_list(&tb_class->tb_item_attribute_list);
		tb_class->set_f_class_type(_T("L"));
		ptb_class_list->AddTail(tb_class);

		tb_class->ptb_class_list = new TB_Class_List();
		tb_class1 = new TB_Class(NULL, reserved_list);
		tb_class1->set_f_class_id(get_f_class_id());
		tb_class1->set_f_prj_name(get_f_prj_name());
		tb_class1->set_f_class_name(get_f_class_name() + "_l_tbl");
		tb_class1->set_caption(get_f_class_name() + "_l_tbl");
		tb_item_label = new TB_Item_Label();
		tb_item_label->get_item_attribute_list(&tb_class1->tb_item_attribute_list);
		tb_class1->set_f_class_type(_T("A"));
		tb_class->ptb_class_list->AddTail(tb_class1);

		// maschera per filtrare le entità esclusi i "$T", "GEOTHM", "USRTHM"
		if ((entMask << acutBuildList(-4, _T("<or"),
										RTDXF0, _T("POLYLINE"),
										-4, _T("<or"),
										RTDXF0, _T("LWPOLYLINE"),
										-4, _T("<or"),
										RTDXF0, _T("LINE"),
										-4, _T("or>"),
										-4, _T("or>"),
										-4, _T("or>"), 0)) == NULL)
			AfxThrowUserException();

		_tfid = 0;
		//	ottiene la lista di tutte le entità esclusi i $T
		if (gsc_ssget(_T("_X"), NULL, NULL, entMask.get_head(), selSet) == RTNORM) 
		{
			_tfid = *current_id2;

			i=0;
			// per ogni elemento
			while (selSet.entname(i++, ent) == GS_GOOD)
			{
				if (eed.load(ent) == GS_GOOD)
				{
					if ((eed.cls == pCls->ptr_id()->code) && (eed.sub == pCls->ptr_id()->sub_code))
					{
						_fid = eed.gs_id;

						tb_item_polyline = NULL;
						tb_item_polyline = (TB_Item_Polyline*) tb_item_list.find(_fid);
						
						// Ricavo le informazioni dell'entità
						if (acdbGetObjectId(objectId, ent) == Acad::eOk)
						{
							if (acdbOpenObject(pEntity, objectId, AcDb::kForRead) == Acad::eOk)
							{
								if (pEntity->isKindOf(AcDbLine::desc()))
								{
									pLine = AcDbLine::cast(pEntity);

									n_color.setAutoCADColorIndex(pLine->colorIndex());
									l_type = pLine->linetype();
									layer = pLine->layer();
									thickness = pLine->thickness();
									l_width = 0;

									pLine->close();
								}
								else if (pEntity->isKindOf(AcDbPolyline::desc()))
								{
									pPolyline = AcDbPolyline::cast(pEntity);

									n_color.setAutoCADColorIndex(pPolyline->colorIndex());
									l_type = pPolyline->linetype();
									layer = pPolyline->layer();
									thickness = pPolyline->thickness();
									l_width = 0;
									if (pPolyline->hasWidth())
										pPolyline->getConstantWidth(l_width);

									pPolyline->close();
								}
								else if (pEntity->isKindOf(AcDb2dPolyline::desc()))
								{
									p2dPolyline = AcDb2dPolyline::cast(pEntity);

									n_color.setAutoCADColorIndex(p2dPolyline->colorIndex());
									l_type = p2dPolyline->linetype();
									layer = p2dPolyline->layer();
									thickness = p2dPolyline->thickness();
									l_width = 0;

									p2dPolyline->close();
								}
								else if (pEntity->isKindOf(AcDb3dPolyline::desc()))
								{
									p3dPolyline = AcDb3dPolyline::cast(pEntity);

									n_color.setAutoCADColorIndex(p3dPolyline->colorIndex());
									l_type = p3dPolyline->linetype();
									layer = p3dPolyline->layer();
									thickness = 0;
									l_width = 0;

									p3dPolyline->close();
								}

								// Ricavo i vertici per TB eventualmente corretti
								point_list.RemoveAll();
								if ((GetPointsOfObj(pEntity, point_list, &length, &area)) && (point_list.GetCount() > 1))
								{
									if (!tb_item_polyline)
									{
										tb_item_polyline = new TB_Item_Polyline(_fid, length);
										UpdateAttribs(tb_item_polyline);
										tb_item_list.AddTail(tb_item_polyline);			
									}
									else
										tb_item_polyline->update_length(length);
									
									_tfid++;
									tb_item_polyline_l = NULL;
									tb_item_polyline_l = new TB_Item_Polyline_L(isOracle, _tfid, length, &point_list, _fid);
									
									layer_ = layer.toupper();
									tb_item_polyline_l->set_layer(layer_);
									
									fl_type = pCls->ptr_fas()->line;
									if (fl_type.toupper() == "DALAYER")
										l_type = "BYLAYER";
									l_type_ = l_type.toupper();
									tb_item_polyline_l->set_l_type(l_type_);

									tb_item_polyline_l->set_color(n_color);

									tb_item_polyline_l->set_thickness(thickness);

									tb_item_polyline_l->set_l_width(l_width);

									UpdateAttribs(tb_item_polyline_l);
									tb_class->tb_item_list.AddTail(tb_item_polyline_l);
								}

								pEntity->close();
							}
						}
					}
				}
			}
	
			*current_id2 = _tfid;
		}
  		
		//	maschera per filtrare i blocchi $T
		if ((entMask << acutBuildList(RTDXF0, _T("INSERT"), 2, _T("$T"), 0)) == NULL)
			AfxThrowUserException();
		if (gsc_ssget(_T("_X"), NULL, NULL, entMask.get_head(), selSet) == RTNORM) 
			LoadBlocks(selSet, tb_class1, pCls, current_id1);
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

int TB_Class::AllLayers()
{
	int ret = GS_GOOD;

	C_RB_LIST   DescrLayer;
	presbuf     p = NULL;

	C_STRING		layer;

	TRY
	{
		// Prendo il primo layer
		DescrLayer << ads_tblnext(_T("LAYER"), 1); 
		while (DescrLayer.get_head() != NULL)
		{
			// Ricavo il nome del layer
			if ((p = DescrLayer.SearchType(2)) == NULL)
				AfxThrowUserException();
      
			layer = p->resval.rstring;
			allLayers.AddHead(layer.get_name());
      
			// Prendo il successivo LAYER
			DescrLayer << ads_tblnext(_T("LAYER"), 0); 
		}
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

//AcDb3DPolyline
//AcDbLWPolyline
//AcDbPolyline
//AcDbLine		
//AcDbCircle
//AcDbArc			
//AcDbSolid
//AcDbEllipse
//AcDbPoint
//AcDbBlockReference
//AcDbText
//AcDbMText
int TB_Class::LoadSpaghetti(bool isOracle, long *current_id1)
{                                               
	int ret = GS_GOOD;

	TB_Item				*tb_item;
	TB_Item_Polyline_L	*tb_item_polyline_l;
	TB_Item_Surface		*tb_item_surface;
	TB_Item_Point		*tb_item_point;
	TB_Item_Text		*tb_item_text;
	TB_Item_Label		*tb_item_label;

	TB_Class	*tb_class_l;
	TB_Class	*tb_class_plg;
	TB_Class	*tb_class_p;
	TB_Class	*tb_class_t;
	TB_Class	*tb_class1;

	C_SELSET	selSet;

	ads_name	ent;

	int		i, x;
	long	_fid, _fid_parent, _tfid_parent;
	POSITION j;

	AcDbObjectId	objectId, objectId1;
	AcDbEntity		*pEntity;
 
	AcDbLine		*pLine;
	AcDbPolyline	*pPolyline;
	AcDb2dPolyline	*p2dPolyline;
	AcDb3dPolyline	*p3dPolyline;
	AcDbBlockReference		*pBlockReference;	
	AcDbBlockTableRecord	*pBlockTableRecord;

	AcDbPoint		*pPoint;
	AcDbText		*pText;
	AcDbCircle		*pCircle;
	AcDbArc			*pArc;
	AcDbMText		*pMText;
	AcDbSolid		*pSolid;
	AcDbEllipse		*pEllipse;
	AcDbFace		*pFace;
	AcDbPolyFaceMesh	*pPolyFaceMesh;

	AcGePoint3d		point3d, tpoint3d, point3d1, point3d2, point3d3, startpoint3d, endpoint3d;
	AcGeScale3d		scale3d;

	C_COLOR n_color;
	C_STRING layer, l_type, block_name, text_style, fl_type, text, _layer, _temp_layer;
	C_STRING layer_, l_type_, block_name_, text_style_;
	double  thickness, l_width, orientation, height, spacing;
	TCHAR	  *str;
	double  endParam, offset, dist;

	C_STRING		line;
	CStringList lineList;

	int		pos;
	long	cont;

	bool	closed;

	CList<AcGePoint3d,AcGePoint3d>	point_list;
	double		length;
	double		area;

	C_RB_LIST	entmask;

	AcDbAttribute *pAttribute = NULL;

	AcDbObjectIterator		*pObjectIterator;

	C_STRING		style, attributeName, attributeLayer, style_;
	AcGePoint3d	attributePoint3d;

	int     hidden;
	C_COLOR attributeColor;

	TRY
	{
		tb_item = new TB_Item();
		tb_item->get_item_attribute_list(&tb_item_attribute_list);

		ptb_class_list = new TB_Class_List();
		
		tb_class_l = new TB_Class(NULL, NULL);
		tb_class_l->set_f_class_id(get_f_class_id());
		tb_class_l->set_f_prj_name(get_f_prj_name());
		tb_class_l->set_f_class_name(get_f_class_name() + "_l");
		tb_class_l->set_caption(get_f_class_name() + "_l");
		tb_item_polyline_l = new TB_Item_Polyline_L();
		tb_item_polyline_l->get_item_attribute_list(&tb_class_l->tb_item_attribute_list);
		tb_class_l->set_f_class_type(_T("L"));
		ptb_class_list->AddTail(tb_class_l);

		tb_class_plg = new TB_Class(NULL, NULL);
		tb_class_plg->set_f_class_id(get_f_class_id());
		tb_class_plg->set_f_prj_name(get_f_prj_name());
		tb_class_plg->set_f_class_name(get_f_class_name() + "_plg");
		tb_class_plg->set_caption(get_f_class_name() + "_plg");
		tb_item_surface = new TB_Item_Surface();
		tb_item_surface->get_item_attribute_list(&tb_class_plg->tb_item_attribute_list, true);
		tb_class_plg->set_f_class_type(_T("O"));
		ptb_class_list->AddTail(tb_class_plg);

		tb_class_p = new TB_Class(NULL, NULL);
		tb_class_p->set_f_class_id(get_f_class_id());
		tb_class_p->set_f_prj_name(get_f_prj_name());
		tb_class_p->set_f_class_name(get_f_class_name() + "_p");
		tb_class_p->set_caption(get_f_class_name() + "_p");
		tb_item_point = new TB_Item_Point();
		tb_item_point->get_item_attribute_list(&tb_class_p->tb_item_attribute_list, true);
		tb_class_p->set_f_class_type(_T("P"));
		ptb_class_list->AddTail(tb_class_p);

		tb_class_t = new TB_Class(NULL, NULL);
		tb_class_t->set_f_class_id(get_f_class_id());
		tb_class_t->set_f_prj_name(get_f_prj_name());
		tb_class_t->set_f_class_name(get_f_class_name() + "_t");
		tb_class_t->set_caption(get_f_class_name() + "_t");
		tb_item_text = new TB_Item_Text();
		tb_item_text->get_item_attribute_list(&tb_class_t->tb_item_attribute_list, true);
		tb_class_t->set_f_class_type(_T("T"));
		ptb_class_list->AddTail(tb_class_t);

		tb_class_t->ptb_class_list = new TB_Class_List();
		tb_class1 = new TB_Class(NULL, NULL);
		tb_class1->set_f_class_id(get_f_class_id());
		tb_class1->set_f_prj_name(get_f_prj_name());
		tb_class1->set_f_class_name(get_f_class_name() + "_t_tbl");
		tb_class1->set_caption(get_f_class_name() + "_t_tbl");
		tb_item_label = new TB_Item_Label();
		tb_item_label->get_item_attribute_list(&tb_class1->tb_item_attribute_list);
		tb_class1->set_f_class_type(_T("A"));
		tb_class_t->ptb_class_list->AddTail(tb_class1);

		AllLayers();
		// determino il nome del layer temporaneo verificando che tale layer non esista già
		x = 1;
		while (x)
		{
			_temp_layer = _T("temp");
			_temp_layer += x;

			j = 0;
			j = allLayers.Find(_temp_layer.get_name());
			if (j)
				x++;
			else
				x=0;
		}
		// CHANGE DA LAYER 0 A LAYER TEMPx
		gsc_crea_layer_senza_scan(_temp_layer);
		// filtro su tutti gli oggetti presenti sul piano 0
		if ((entmask << acutBuildList(8, _T("0"), 0)) == NULL)
			AfxThrowUserException();
		if (gsc_ssget(_T("_X"), NULL, NULL, entmask.get_head(), selSet) == RTNORM) 
		{
			i = 0;
			while (selSet.entname(i++, ent) == GS_GOOD)
			{
				if (gsc_setLayer(ent, _temp_layer.get_name()) != GS_GOOD)
					AfxThrowUserException();
			}
		}

		_fid=*current_id1;
		_fid_parent = -1;




		allLayers.RemoveAll();
		AllLayers();
		j = allLayers.GetHeadPosition();
		while (j) 
		{
			_layer = allLayers.GetAt(j);

			if ((entmask << acutBuildList(8, _layer, 0)) == NULL)
				AfxThrowUserException();

			// filtro su tutti gli oggetti presenti sui piani
			if (gsc_ssget(_T("_X"), NULL, NULL, entmask.get_head(), selSet) == RTNORM) 
			{
				i = 0;
				while (selSet.entname(i++, ent) == GS_GOOD)
				{
					if (acdbGetObjectId(objectId, ent) == Acad::eOk)
					{
						if (acdbOpenObject(pEntity, objectId, AcDb::kForRead) == Acad::eOk)
						{
							if (pEntity->isKindOf(AcDbBlockReference::desc()))
							{
								pBlockReference = AcDbBlockReference::cast(pEntity);

								pObjectIterator = pBlockReference->attributeIterator();
								// Inizio il ciclo sugli attributi
								for (pObjectIterator->start(); Adesk::kTrue != pObjectIterator->done(); pObjectIterator->step())
								{
									_fid++;			

									// Ricavo l' attributo
     								if (pBlockReference->openAttribute(pAttribute, pObjectIterator->objectId(), AcDb::kForRead,	true) == Acad::eOk)
									{
										attributeName = pAttribute->tag();
										attributePoint3d = pAttribute->position();
										hidden = (pAttribute->isInvisible() == Adesk::kTrue) ? 1 : 0;

										attributeColor.setAutoCADColorIndex(pAttribute->colorIndex());
										attributeLayer = pAttribute->layer();  
										height = pAttribute->height();
										text = pAttribute->textString();

										// Stile di testo dell' attributo
										objectId1 = pAttribute->textStyle();
										if (acdbOpenObject((AcDbObject *&)pBlockTableRecord, objectId1, AcDb::kForRead) != Acad::eOk) break;
											pBlockTableRecord->getName(str);
										style = str;
										acdbFree(str);
										pBlockTableRecord->close();

										// Leggo la rotazione in radianti, passo ai gradi 
										orientation = pAttribute->rotation();
										orientation = gsc_rad2grd(orientation);

										tb_item_label = new TB_Item_Label(_fid, orientation, _fid_parent, text, attributeName, hidden, 
																			attributePoint3d.x, attributePoint3d.y, point3d.x, point3d.y);

										layer_ = _layer.toupper();
										tb_item_label->set_layer(layer_);

										tb_item_label->set_color(attributeColor);

										tb_item_label->set_text_height(height);

										style_ = style.toupper();
										tb_item_label->set_text_style(style_);

										tb_class1->tb_item_list.AddTail(tb_item_label);
									}
								}

								delete (pObjectIterator);

								pBlockReference->close();
							}
							
							pEntity->close();
						}
					}
				}
			}


			if (Explode_LeaderDimension(_layer) == GS_BAD)
				AfxThrowUserException();
			if (Explode_Insert(_layer) == GS_BAD)
				AfxThrowUserException();

			if (gsc_ssget(_T("_X"), NULL, NULL, entmask.get_head(), selSet) == RTNORM) 
			{
				if (_layer == _temp_layer)
					_layer = "0";

				i=0;
				// per ogni elemento
				while (selSet.entname(i++, ent) == GS_GOOD)
				{
					// Ricavo le informazioni dell'entità
					if (acdbGetObjectId(objectId, ent) == Acad::eOk)
					{
						_fid++;
						_fid_parent = _fid;
						_fid++;

						if (acdbOpenObject(pEntity, objectId, AcDb::kForRead) == Acad::eOk)
						{
							if ((pEntity->isKindOf(AcDbLine::desc())) || 
								(pEntity->isKindOf(AcDbPolyline::desc())) || 
								(pEntity->isKindOf(AcDb2dPolyline::desc())) || 
								(pEntity->isKindOf(AcDb3dPolyline::desc())))
							{
								if (pEntity->isKindOf(AcDbLine::desc()))
								{
									pLine = AcDbLine::cast(pEntity);

									n_color.setAutoCADColorIndex(pLine->colorIndex());
									l_type = pLine->linetype();
									//layer = pLine->layer();
									layer = _layer;
									thickness = pLine->thickness();
									l_width = 0;

									closed = (pLine->isClosed()) ? true : false;

									pLine->close();
								}
								else if (pEntity->isKindOf(AcDbPolyline::desc()))
								{
									pPolyline = AcDbPolyline::cast(pEntity);

									n_color.setAutoCADColorIndex(pPolyline->colorIndex());
									l_type = pPolyline->linetype();
									//layer = pPolyline->layer();
									layer = _layer;
									thickness = pPolyline->thickness();
									l_width = 0;
									if (pPolyline->hasWidth())
										pPolyline->getConstantWidth(l_width);

									closed = (pPolyline->isClosed()) ? true : false;

									pPolyline->close();
								}
								else if (pEntity->isKindOf(AcDb2dPolyline::desc()))
								{
									p2dPolyline = AcDb2dPolyline::cast(pEntity);

									n_color.setAutoCADColorIndex(p2dPolyline->colorIndex());
									l_type = p2dPolyline->linetype();
									//layer = p2dPolyline->layer();
									layer = _layer;
									thickness = p2dPolyline->thickness();
									l_width = 0;

									closed = (p2dPolyline->isClosed()) ? true : false;

									p2dPolyline->close();
								}
								else if (pEntity->isKindOf(AcDb3dPolyline::desc()))
								{
									p3dPolyline = AcDb3dPolyline::cast(pEntity);

									n_color.setAutoCADColorIndex(p3dPolyline->colorIndex());
									l_type = p3dPolyline->linetype();
									//layer = p3dPolyline->layer();
									layer = _layer;
									thickness = 0;
									l_width = 0;

									closed = (p3dPolyline->isClosed()) ? true : false;

									p3dPolyline->close();
								}

								// Ricavo i vertici per TB eventualmente corretti
								point_list.RemoveAll();
								if ((GetPointsOfObj(pEntity, point_list, &length, &area)) && (point_list.GetCount() > 1))
								{
									if ((closed) && (area >0))
									{
										startpoint3d = point_list.GetHead();
										endpoint3d = point_list.GetTail();
										if ((startpoint3d.x != endpoint3d.x) || (startpoint3d.y != endpoint3d.y) || (startpoint3d.z != endpoint3d.z))
										{
											endpoint3d = startpoint3d;										
											point_list.AddTail(endpoint3d);
										}

										tb_item_surface = NULL;
										tb_item_surface = new TB_Item_Surface(isOracle, _fid, area, length, &point_list, _fid_parent);

										layer_ = layer.toupper();	
										tb_item_surface->set_layer(layer_);
										
										fl_type = pCls->ptr_fas()->line;
										if (fl_type.toupper() == "DALAYER")
											l_type = "BYLAYER";
										l_type_ = l_type.toupper();
										tb_item_surface->set_l_type(l_type_);

										tb_item_surface->set_color(n_color);

										tb_item_surface->set_thickness(thickness);

										tb_item_surface->set_l_width(l_width);
										
										tb_class_plg->tb_item_list.AddTail(tb_item_surface);
									}
									else
									{
										tb_item_polyline_l = NULL;
										tb_item_polyline_l = new TB_Item_Polyline_L(isOracle, _fid, length, &point_list, _fid_parent);
										
										layer_ = layer.toupper();
										tb_item_polyline_l->set_layer(layer_);
										
										fl_type = pCls->ptr_fas()->line;
										if (fl_type.toupper() == "DALAYER")
											l_type = "BYLAYER";
										l_type_ = l_type.toupper();
										tb_item_polyline_l->set_l_type(l_type_);

										tb_item_polyline_l->set_color(n_color);

										tb_item_polyline_l->set_thickness(thickness);

										tb_item_polyline_l->set_l_width(l_width);
										
										tb_class_l->tb_item_list.AddTail(tb_item_polyline_l);
									}
								}
							}
							else if (pEntity->isKindOf(AcDbBlockReference::desc()))
							{
								pBlockReference = AcDbBlockReference::cast(pEntity);

								objectId = pBlockReference->blockTableRecord();
								if (acdbOpenObject((AcDbObject *&)pBlockTableRecord, objectId, AcDb::kForRead) != Acad::eOk) 
									break;
								pBlockTableRecord->getName(str);
                        C_STRING dummy(str);
								block_name = correctString1(dummy);
								acdbFree(str);
								pBlockTableRecord->close();				
			
								if ((block_name.toupper() != "GEOTHM") && (block_name.toupper() != "USRTHM"))
								{
									// Memorizzo le caratteristiche
									point3d = pBlockReference->position();
									//layer = pBlockReference->layer();
									layer = _layer;
									n_color.setAutoCADColorIndex(pBlockReference->colorIndex());
									scale3d = pBlockReference->scaleFactors();
									orientation = pBlockReference->rotation();
									orientation = gsc_rad2grd(orientation);

									tb_item_point = new TB_Item_Point(_fid, orientation, point3d.x, point3d.y, point3d.z, _fid_parent);

									block_name_ = block_name.toupper();
									tb_item_point->set_block_name(block_name);

									layer_ = layer.toupper();
									tb_item_point->set_layer(layer_);

									tb_item_point->set_color(n_color);

									tb_item_point->set_block_scale_factor(scale3d.sx);

									tb_class_p->tb_item_list.AddTail(tb_item_point);
								} 

								pBlockReference->close();
							} 
							else if (pEntity->isKindOf(AcDbPoint::desc()))
							{
								pPoint = AcDbPoint::cast(pEntity);

								// Memorizzo le caratteristiche
								n_color.setAutoCADColorIndex(pPoint->colorIndex());
								//layer = pPoint->layer();
								layer = _layer;
								point3d = pPoint->position();

								tb_item_point = new TB_Item_Point(_fid, 0, point3d.x, point3d.y, point3d.z, _fid_parent);
								
								layer_ = layer.toupper();
								tb_item_point->set_layer(layer_);

								tb_item_point->set_color(n_color);
								
								tb_class_p->tb_item_list.AddTail(tb_item_point);

								pPoint->close();
							} 
							else if (pEntity->isKindOf(AcDbText::desc()))
							{
								pText = AcDbText::cast(pEntity);

								// Memorizzo le caratteristiche
								n_color.setAutoCADColorIndex(pText->colorIndex());
								//layer = pText->layer();
								layer = _layer;
								height = pText->height();
								objectId = pText->textStyle();
								if (acdbOpenObject((AcDbObject *&)pBlockTableRecord, objectId, AcDb::kForRead) != Acad::eOk) 
									break;
								pBlockTableRecord->getName(str);
								text_style = str;
								acdbFree(str);
								pBlockTableRecord->close();

								orientation = pText->rotation();
								orientation = gsc_rad2grd(orientation);
								text = pText->textString();
								point3d = pText->position();

								if (text.len() > 0)
								{
									tb_item_text = new TB_Item_Text(_fid, text, _fid_parent);
									tb_class_t->tb_item_list.AddTail(tb_item_text);
								} 
							
								_fid_parent = _fid;
								_fid++;
								tb_item_label = new TB_Item_Label(_fid, orientation, _fid_parent, text, "", 0, point3d.x, point3d.y, point3d.x, point3d.y);

								layer_ = layer.toupper();
								tb_item_label->set_layer(layer_);

								tb_item_label->set_color(n_color);

								tb_item_label->set_text_height(height);

								text_style_ = text_style.toupper();
								tb_item_label->set_text_style(text_style_);

								tb_class1->tb_item_list.AddTail(tb_item_label);

								pText->close();
							} 
							else if (pEntity->isKindOf(AcDbMText::desc()))
							{
								pMText = AcDbMText::cast(pEntity);

								// Memorizzo le caratteristiche
								n_color.setAutoCADColorIndex(pMText->colorIndex());
								layer = pMText->layer();
								//height = pMText->height(); // 26/11/07 Paola
								height = pMText->textHeight();
								objectId = pMText->textStyle();
								if (acdbOpenObject((AcDbObject *&)pBlockTableRecord, objectId, AcDb::kForRead) != Acad::eOk) 
									break;
								pBlockTableRecord->getName(str);
								text_style = str;
								acdbFree(str);
								pBlockTableRecord->close();

								orientation = pMText->rotation();
								orientation = gsc_rad2grd(orientation);
								line = pMText->contents();
								spacing = pMText->actualHeight();
								point3d = pMText->location();

								lineList.RemoveAll();
								if ((pos = line.find(_T("\\A1"))) != -1)
								{
									line = line.right(line.len() - pos - 3);
									if ((pos = line.find(_T(";"))) != -1)
									{
										line = line.right(line.len() - pos - 1);
									}		
								}		
								if ((pos = line.find(_T("\\A"))) != -1)
								{
									line = line.right(line.len() - pos - 2);
									if ((pos = line.find(_T(";"))) != -1)
									{
										line = line.right(line.len() - pos - 1);
									}		
								}		
								if ((pos = line.find(_T("\\H"))) != -1)
								{
									line = line.right(line.len() - pos - 2);
									if ((pos = line.find(_T(";"))) != -1)
									{
										line = line.right(line.len() - pos - 1);
									}	
									line.remove(_T('"'));
								}
								if ((pos = line.find(_T("\\F"))) != -1)
								{
									line = line.right(line.len() - pos - 2);
									if ((pos = line.find(_T("shx;"))) != -1)
									{
										line = line.right(line.len() - pos - 4);
									}		
								}		
								while (line.len() > 0)
								{
									if ((pos = line.find(_T("\\P"))) != -1)
									{
										lineList.AddTail(line.left(pos).get_name());
										line = line.right(line.len() - pos - 2);
									}
									else
									{
										lineList.AddTail(line.get_name());
										line = "";
									}
								}

								cont = (long) lineList.GetCount();
								if (cont > 1)
									spacing = (spacing - height) / (cont-1);
								else
								{
									spacing = 0;
									height = 0;	
								}

								POSITION i = lineList.GetHeadPosition();
								cont = 0;

								_tfid_parent = _fid_parent;
								while (i) 
								{
									line = lineList.GetAt(i);

									if (line.len() > 0)
									{
										tb_item_text = new TB_Item_Text(_fid, line, _tfid_parent);
										tb_class_t->tb_item_list.AddTail(tb_item_text);
									} 

									_fid_parent = _fid;
									_fid++;
									tpoint3d.x = point3d.x + ((cont * spacing) * sin(pMText->rotation()));
									tpoint3d.y = point3d.y - ((cont * spacing) * cos(pMText->rotation()));
									tb_item_label = new TB_Item_Label(_fid, orientation, _fid_parent, line, "", 0, tpoint3d.x, tpoint3d.y, tpoint3d.x, tpoint3d.y);
									_fid++;

									layer_ = layer.toupper();
									tb_item_label->set_layer(layer_);

									tb_item_label->set_color(n_color);

									tb_item_label->set_text_height(height);

									text_style_ = text_style.toupper();
									tb_item_label->set_text_style(text_style_);

									tb_class1->tb_item_list.AddTail(tb_item_label);

									lineList.GetNext(i);
									cont ++;				
								}

								pMText->close();
							} 
							else if (pEntity->isKindOf(AcDbCircle::desc()))
							{
								pCircle = AcDbCircle::cast(pEntity);

								// Memorizzo le caratteristiche
								n_color.setAutoCADColorIndex(pCircle->colorIndex());
								l_type = pCircle->linetype();
								//layer = pCircle->layer();
								layer = _layer;
								thickness = pCircle->thickness();

								// Ricavo i vertici per TB eventualmente corretti
								point_list.RemoveAll();
								if ((GetPointsOfObj(pEntity, point_list, &length, &area)) && (point_list.GetCount() > 1))
								{
									tb_item_polyline_l = NULL;
									tb_item_polyline_l = new TB_Item_Polyline_L(isOracle, _fid, length, &point_list, _fid_parent);
									
									layer_ = layer.toupper();	
									tb_item_polyline_l->set_layer(layer_);
									
									fl_type = pCls->ptr_fas()->line;
									if (fl_type.toupper() == "DALAYER")
										l_type = "BYLAYER";
									l_type_ = l_type.toupper();
									tb_item_polyline_l->set_l_type(l_type_);

									tb_item_polyline_l->set_color(n_color);

									tb_item_polyline_l->set_thickness(thickness);
							
									tb_class_l->tb_item_list.AddTail(tb_item_polyline_l);
								}

								pCircle->close();
							} 
							else if (pEntity->isKindOf(AcDbArc::desc()))
							{
								pArc = AcDbArc::cast(pEntity);

								// Memorizzo le caratteristiche
								n_color.setAutoCADColorIndex(pArc->colorIndex());
								l_type = pArc->linetype();
								//layer = pArc->layer();
								layer = _layer;
								thickness = pArc->thickness();
							
								// Ricavo i vertici per TB eventualmente corretti
								point_list.RemoveAll();
								if ((GetPointsOfObj(pEntity, point_list, &length, &area)) && (point_list.GetCount() > 1))
								{
									tb_item_polyline_l = NULL;
									tb_item_polyline_l = new TB_Item_Polyline_L(isOracle, _fid, length, &point_list, _fid_parent);
									
									layer_ = layer.toupper();	
									tb_item_polyline_l->set_layer(layer_);
									
									fl_type = pCls->ptr_fas()->line;
									if (fl_type.toupper() == "DALAYER")
										l_type = "BYLAYER";
									l_type_ = l_type.toupper();
									tb_item_polyline_l->set_l_type(l_type_);

									tb_item_polyline_l->set_color(n_color);

									tb_item_polyline_l->set_thickness(thickness);

									tb_class_l->tb_item_list.AddTail(tb_item_polyline_l);
								}

								pArc->close();						
							} 
							else if (pEntity->isKindOf(AcDbPolyFaceMesh::desc())) //modificare 2009
							{
								pPolyFaceMesh = AcDbPolyFaceMesh::cast(pEntity);

								// Memorizzo le caratteristiche
								n_color.setAutoCADColorIndex(pPolyFaceMesh->colorIndex());
								l_type = pPolyFaceMesh->linetype();
								//layer = pPolyFaceMesh->layer();
								layer = _layer;

								// Ricavo i vertici per TB eventualmente corretti
								point_list.RemoveAll();
								if ((GetPointsOfObj(pEntity, point_list, &length, &area)) && (point_list.GetCount() > 1))
								{
									startpoint3d = point_list.GetHead();
									endpoint3d = point_list.GetTail();
									if ((startpoint3d.x != endpoint3d.x) || (startpoint3d.y != endpoint3d.y) || (startpoint3d.z != endpoint3d.z))
									{
										endpoint3d = startpoint3d;										
										point_list.AddTail(endpoint3d);
									}

									tb_item_surface = NULL;
									tb_item_surface = new TB_Item_Surface(isOracle, _fid, area, length, &point_list, _fid_parent);

									layer_ = layer.toupper();	
									tb_item_surface->set_layer(layer_);
									
									fl_type = pCls->ptr_fas()->line;
									if (fl_type.toupper() == "DALAYER")
										l_type = "BYLAYER";
									l_type_ = l_type.toupper();
									tb_item_surface->set_l_type(l_type_);

									tb_item_surface->set_color(n_color);

									tb_item_surface->set_thickness(thickness);

									tb_item_surface->set_l_width(l_width);
									
									tb_class_plg->tb_item_list.AddTail(tb_item_surface);
								}

								pPolyFaceMesh->close();						
							}
							else if (pEntity->isKindOf(AcDbSolid::desc()))
							{
								pSolid = AcDbSolid::cast(pEntity);

								// Memorizzo le caratteristiche
								n_color.setAutoCADColorIndex(pSolid->colorIndex());
								l_type = pSolid->linetype();
								//layer = pSolid->layer();
								layer = _layer;
								thickness = pSolid->thickness();

								// Ricavo i vertici per TB eventualmente corretti
								pSolid->getPointAt(0, point3d);
								pSolid->getPointAt(1, point3d1);
								pSolid->getPointAt(2, point3d2);
						
								point_list.RemoveAll();							
								point_list.AddTail(point3d);
								point_list.AddTail(point3d1);
								point_list.AddTail(point3d2);
								point_list.AddTail(point3d);
								length = 0;

								tb_item_polyline_l = NULL;
								tb_item_polyline_l = new TB_Item_Polyline_L(isOracle, _fid, length, &point_list, _fid_parent);
								
								layer_ = layer.toupper();
								tb_item_polyline_l->set_layer(layer_);
								
								fl_type = pCls->ptr_fas()->line;
								if (fl_type.toupper() == "DALAYER")
									l_type = "BYLAYER";
								l_type_ = l_type.toupper();
								tb_item_polyline_l->set_l_type(l_type_);

								tb_item_polyline_l->set_color(n_color);

								tb_item_polyline_l->set_thickness(thickness);
							
								tb_class_l->tb_item_list.AddTail(tb_item_polyline_l);
						
								pSolid->close();						
							} 
							else if (pEntity->isKindOf(AcDbEllipse::desc()))
							{
								pEllipse = AcDbEllipse::cast(pEntity);
								
								// Memorizzo le caratteristiche
								n_color.setAutoCADColorIndex(pEllipse->colorIndex());
								l_type = pEllipse->linetype();
								//layer = pEllipse->layer();
								layer = _layer;
								thickness = 0;

								pEllipse->getEndParam(endParam);
								pEllipse->getDistAtParam(endParam, length);

								// primo punto
								pEllipse->getPointAtDist(0, point3d1);

								point_list.RemoveAll();							
								point_list.AddTail(point3d1);
								// punti successivi fino al penultimo compreso
								offset = length / num_vertex;
								dist = offset;
								while (dist < length)
								{
									pEllipse->getPointAtDist(dist, point3d);
									point_list.AddTail(point3d);
									dist += offset;
								}
								// ultimo punto = al primo punto
								point_list.AddTail(point3d1);

								tb_item_polyline_l = NULL;
								tb_item_polyline_l = new TB_Item_Polyline_L(isOracle, _fid, length, &point_list, _fid_parent);
								
								layer_ = layer.toupper();
								tb_item_polyline_l->set_layer(layer_);
								
								fl_type = pCls->ptr_fas()->line;
								if (fl_type.toupper() == "DALAYER")
									l_type = "BYLAYER";
								l_type_ = l_type.toupper();
								tb_item_polyline_l->set_l_type(l_type_);

								tb_item_polyline_l->set_color(n_color);

								tb_item_polyline_l->set_thickness(thickness);
							
								tb_class_l->tb_item_list.AddTail(tb_item_polyline_l);			
								
								pEllipse->close();						
							} 
							else if (pEntity->isKindOf(AcDbFace::desc()))
							{
								pFace = AcDbFace::cast(pEntity);

								// Memorizzo le caratteristiche
								n_color.setAutoCADColorIndex(pFace->colorIndex());
								l_type = pFace->linetype();
								//layer = pFace->layer();
								layer = _layer;
								thickness = 0;

								// Ricavo i vertici per TB eventualmente corretti
								pFace->getVertexAt(0, point3d);
								pFace->getVertexAt(1, point3d1);
								pFace->getVertexAt(2, point3d2);
								pFace->getVertexAt(3, point3d3);
			
								point_list.RemoveAll();							
								point_list.AddTail(point3d);
								point_list.AddTail(point3d1);
								point_list.AddTail(point3d2);
								point_list.AddTail(point3d3);
								point_list.AddTail(point3d);
								length = 0;

								tb_item_polyline_l = NULL;
								tb_item_polyline_l = new TB_Item_Polyline_L(isOracle, _fid, length, &point_list, _fid_parent);
								
								layer_ = layer.toupper();
								tb_item_polyline_l->set_layer(layer_);
								
								fl_type = pCls->ptr_fas()->line;
								if (fl_type.toupper() == "DALAYER")
									l_type = "BYLAYER";
								l_type_ = l_type.toupper();
								tb_item_polyline_l->set_l_type(l_type_);

								tb_item_polyline_l->set_color(n_color);

								tb_item_polyline_l->set_thickness(thickness);
							
								tb_class_l->tb_item_list.AddTail(tb_item_polyline_l);

								pFace->close();
							} 
							
							pEntity->close();
						}
					}
				}
			}
			*current_id1=_fid;

			allLayers.GetNext(j);
		}

		// CHANGE DA LAYER TEMPx A LAYER 0
		// filtro su tutti gli oggetti presenti sul piano _temp_layer
		if ((entmask << acutBuildList(8, _temp_layer, 0)) == NULL)
			AfxThrowUserException();
		if (gsc_ssget(_T("_X"), NULL, NULL, entmask.get_head(), selSet) == RTNORM) 
		{
			i = 0;
			while (selSet.entname(i++, ent) == GS_GOOD)
			{
				if (gsc_setLayer(ent, _T("0")) != GS_GOOD)
					AfxThrowUserException();
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

int TB_Class::Explode_Insert(C_STRING layer)
{
	int ret = GS_GOOD;

	long i=0;
	ads_name	ent;
	
	C_RB_LIST	entmask;
	C_SELSET	SelSet;

 	C_RB_LIST	Attrib;		// ottiene la lista di tutti gli oggetti grafici 

	presbuf		pRB;

	C_STRING		str;

	int			j;			// variabile utilizzata per gestire i casi in cui essendo presenti oggetti "errati"
							// nel disegno il comando gsc_ssget entra in un loop. La variabile permette di uscire 
							// forzatamente dall'while

	TRY
	{
		//if ((entmask <<	acutBuildList(RTDXF0, _T("INSERT"),	0)) == NULL)
		//	AfxThrowUserException();
		if ((entmask << acutBuildList(-4, _T("<and"),
										RTDXF0, _T("INSERT"),
										-4, _T("<and"),
										8, layer,
										-4, _T("and>"),
										-4, _T("and>"), 0)) == NULL)
			AfxThrowUserException();

		j = 0;
		// ottiene la lista di tutti i blocchi
		while ((gsc_ssget(_T("_X"), NULL, NULL, entmask.get_head(), SelSet) == RTNORM) && SelSet.length()>0 && (j<50))	
		{
			acutPrintf(_T("\n"));
       
			i=0;
			j++;
			while (SelSet.entname(i++, ent) == GS_GOOD)			// per ogni oggetto 		
			{
				if ((Attrib << ads_entget(ent)) == NULL) 
					AfxThrowUserException();

				if ((pRB = Attrib.SearchType(2)) != NULL)		// name
					str = pRB->resval.rstring;

				//if (str == "_CLOSEDBLANK")
				//{
					if (gsc_explode(ent, true) == GS_BAD)
						AfxThrowUserException();
					//if (acedCommand(RTSTR, _T("_.EXPLODE"), RTENAME, ent, 0) != RTNORM)		// esplosione blocco
					//	AfxThrowUserException();
				//}
			}
		}

		// filtro su tutti gli oggetti presenti sul piano 0
		if ((entmask << acutBuildList(8, _T("0"), 0)) == NULL)
			AfxThrowUserException();
	
		if (gsc_ssget(_T("_X"), NULL, NULL, entmask.get_head(), SelSet) == RTNORM) 
		{
			j = 0;
			while (SelSet.entname(j++, ent) == GS_GOOD)	//	per ogni oggetto calcola la corrispondente polilinea
			{
				if (gsc_setLayer(ent, layer.get_name()) != GS_GOOD)
					AfxThrowUserException();
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

int TB_Class::Explode_LeaderDimension(C_STRING layer)
{
	int ret = GS_GOOD;

	long i=0;
	ads_name	ent;
	
	C_RB_LIST	entmask;
	C_SELSET	SelSet;

	int			j;			// variabile utilizzata per gestire i casi in cui essendo presenti oggetti "errati"
							// nel disegno il comando gsc_ssget entra in un loop. La variabile permette di uscire 
							// forzatamente dall'while

	TRY
	{
		if ((entmask << acutBuildList(-4, _T("<and"),
											-4, _T("<or"),
											RTDXF0, _T("DIMENSION"),
											-4, _T("<or"),
											RTDXF0, _T("LEADER"),
											-4, _T("or>"),
											-4, _T("or>"),
										-4, _T("<and"),
										8, layer,
										-4, _T("and>"),
										-4, _T("and>"), 0)) == NULL)
			AfxThrowUserException();

		j = 0;
		// ottiene la lista di tutti i blocchi
		while ((gsc_ssget(_T("_X"), NULL, NULL, entmask.get_head(), SelSet) == RTNORM) && SelSet.length()>0 && (j<50))	
		{
			acutPrintf(_T("\n"));
       
			i=0;
			j++;
			while (SelSet.entname(i++, ent) == GS_GOOD)			// per ogni oggetto 		
			{
				if (gsc_explode(ent, true) == GS_BAD)
					AfxThrowUserException();
				//if (acedCommand(RTSTR, _T("_.EXPLODE"), RTENAME, ent, 0) != RTNORM)		// esplosione blocco
				//	AfxThrowUserException();
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

int TB_Class::LoadBlocks(C_SELSET selSet, TB_Class* _tb_class, C_CLASS *pCls, long *current_id1)
{                                               
	int ret = GS_GOOD;

	long i, j;
	
	ads_name	ent;
	C_EED		eed;

	AcDbObjectId			objectId, objectId1;
	AcDbEntity				*pEntity;
	AcDbBlockReference		*pBlockReference;	
	AcDbBlockTableRecord	*pBlockTableRecord;
	AcGePoint3d				point3d, tpoint3d;
	AcDbObjectIterator		*pObjectIterator;
	AcDbAttribute			*pAttribute = NULL;
	AcDbText				*pText = NULL;
	AcDbMText				*pMText = NULL;

	TCHAR		*str;

	long		_fid, _fid_parent;
	
	C_STRING	layer, style, attributeName, attributeLayer, text;
	C_STRING	layer_, style_;
	int		hidden;
	C_COLOR  n_color, attributeColor;
	double	height, orientation, spacing;

	TB_Item_Label	*tb_item_label = NULL;
	TB_Item_Point	*tb_item_point = NULL;

	AcGePoint3d	attributePoint3d;

	C_STRING		line;
	CStringList	lineList;

	long		pos, cont;

	TRY
	{
		i=0;		
		_fid=*current_id1;
		// per ogni elemento
		while (selSet.entname(i++, ent) == GS_GOOD)
		{
			if (eed.load(ent) == GS_GOOD)
			{
				if ((eed.cls == pCls->ptr_id()->code) && (eed.sub == pCls->ptr_id()->sub_code))
				{
					_fid_parent = eed.gs_id;
					// AUGUGLIARO
					j = 0;

					// Ricavo le informazioni dell' entità
					if (acdbGetObjectId(objectId, ent) == Acad::eOk)
					{
						if (acdbOpenObject(pEntity, objectId, AcDb::kForRead) == Acad::eOk)
						{
							// Se non è in grafica salto l' entità
							if (pEntity->isErased()) 
								continue;

							if (pEntity->isKindOf(AcDbBlockReference::desc()))
							{
								pBlockReference = AcDbBlockReference::cast(pEntity);

								// Memorizzo il punto di inserimento
								point3d = pBlockReference->position();
								layer = pBlockReference->layer();
								n_color.setAutoCADColorIndex(pBlockReference->colorIndex());

								pObjectIterator = pBlockReference->attributeIterator();
								// Inizio il ciclo sugli attributi
								for (pObjectIterator->start(); Adesk::kTrue != pObjectIterator->done(); pObjectIterator->step())
								{
									_fid++;			
									// Ricavo l' attributo
         							if (pBlockReference->openAttribute(pAttribute, pObjectIterator->objectId(), AcDb::kForRead,	true) == Acad::eOk)
									{
										attributeName = pAttribute->tag();
										attributePoint3d = pAttribute->position();
										hidden = (pAttribute->isInvisible() == Adesk::kTrue) ? 1 : 0;

										attributeColor.setAutoCADColorIndex(pAttribute->colorIndex());
										attributeLayer = pAttribute->layer();  
										height = pAttribute->height();
										text = pAttribute->textString();
										// Stile di testo dell' attributo
										objectId1 = pAttribute->textStyle();
										if (acdbOpenObject((AcDbObject *&)pBlockTableRecord, objectId1, AcDb::kForRead) != Acad::eOk) break;
											pBlockTableRecord->getName(str);
										style = str;
										acdbFree(str);
										pBlockTableRecord->close();

										// Leggo la rotazione in radianti, passo ai gradi 
										orientation = pAttribute->rotation();
										orientation = gsc_rad2grd(orientation);

										// AUGUGLIARO
										if (j == 0)
										{
											j = 1;
											tb_item_point = NULL;
											tb_item_point = (TB_Item_Point*) this->tb_item_list.find_fid_parent(_fid_parent, point3d);
											if (tb_item_point)
											{
												_fid_parent = tb_item_point->get_fid();
												tb_item_point->set_fid_parent(-1);
											}
										}
										tb_item_label = new TB_Item_Label(_fid, orientation, _fid_parent, text, attributeName, hidden, 
																			attributePoint3d.x, attributePoint3d.y, point3d.x, point3d.y);

										layer_ = layer.toupper();
										tb_item_label->set_layer(layer_);

										tb_item_label->set_color(attributeColor);

										tb_item_label->set_text_height(height);

										style_ = style.toupper();
										tb_item_label->set_text_style(style_);

										_tb_class->tb_item_list.AddTail(tb_item_label);
            						}
								}

								delete (pObjectIterator);
								pBlockReference->close();
							} 
							else if (pEntity->isKindOf(AcDbText::desc()))
							{
								_fid++;			

								pText = AcDbText::cast(pEntity);

								n_color.setAutoCADColorIndex(pText->colorIndex());
								layer = pText->layer();
								orientation = pText->rotation();
								orientation = gsc_rad2grd(orientation);
								point3d = pText->position();
								text = pText->textString();
								height = pText->height();
								hidden = false;
								
								// Stile di testo dell' attributo
								objectId1 = pText->textStyle();
								if (acdbOpenObject((AcDbObject *&)pBlockTableRecord, objectId1, AcDb::kForRead) != Acad::eOk) break;
									pBlockTableRecord->getName(str);
								style = str;
								acdbFree(str);
								pBlockTableRecord->close();

								tb_item_label = new TB_Item_Label(_fid, orientation, _fid_parent, text, "", hidden, point3d.x, point3d.y, point3d.x, point3d.y);

								layer_ = layer.toupper();
								tb_item_label->set_layer(layer_);

								tb_item_label->set_color(n_color);

								tb_item_label->set_text_height(height);

								style_ = style.toupper();
								tb_item_label->set_text_style(style_);

								_tb_class->tb_item_list.AddTail(tb_item_label);
							
								pText->close();
							} 
							else if (pEntity->isKindOf(AcDbMText::desc()))
							{
								_fid++;			

								pMText = AcDbMText::cast(pEntity);

								// Memorizzo le caratteristiche
								n_color.setAutoCADColorIndex(pMText->colorIndex());
								layer = pMText->layer();
								//height = pMText->height(); // 26/11/07 Paola
								height = pMText->textHeight();
								objectId = pMText->textStyle();
								if (acdbOpenObject((AcDbObject *&)pBlockTableRecord, objectId, AcDb::kForRead) != Acad::eOk) 
									break;
								pBlockTableRecord->getName(str);
								style = str;
								acdbFree(str);
								pBlockTableRecord->close();

								orientation = pMText->rotation();
								orientation = gsc_rad2grd(orientation);
								line = pMText->contents();
								spacing = pMText->actualHeight();
								point3d = pMText->location();

								lineList.RemoveAll();
								if ((pos = line.find(_T("\\A1"))) != -1)
								{
									line = line.right(line.len() - pos - 3);
									if ((pos = line.find(_T(";"))) != -1)
									{
										line = line.right(line.len() - pos - 1);
									}		
								}		
								if ((pos = line.find(_T("\\A"))) != -1)
								{
									line = line.right(line.len() - pos - 2);
									if ((pos = line.find(_T(";"))) != -1)
									{
										line = line.right(line.len() - pos - 1);
									}		
								}		
								if ((pos = line.find(_T("\\F"))) != -1)
								{
									line = line.right(line.len() - pos - 2);
									if ((pos = line.find(_T("shx;"))) != -1)
									{
										line = line.right(line.len() - pos - 4);
									}		
								}		
								while (line.len() > 0)
								{
									if ((pos = line.find(_T("\\P"))) != -1)
									{
										lineList.AddTail(line.left(pos).get_name());
										line = line.right(line.len() - pos - 2);
									}
									else
									{
										lineList.AddTail(line.get_name());
										line = "";
									}
								}

								cont = (long) lineList.GetCount();
								if (cont > 1)
									spacing = (spacing - height) / (cont-1);
								else
								{
									spacing = 0;
									height = 0;	
								}

								POSITION i = lineList.GetHeadPosition() ;
								cont = 0;

								while (i) 
								{
									line = lineList.GetAt(i);

									tpoint3d.x = point3d.x + ((cont * spacing) * sin(pMText->rotation()));
									tpoint3d.y = point3d.y - ((cont * spacing) * cos(pMText->rotation()));
									tb_item_label = new TB_Item_Label(_fid, orientation, _fid_parent, line, "", 0, tpoint3d.x, tpoint3d.y, tpoint3d.x, tpoint3d.y);
									_fid++;

									layer_ = layer.toupper();
									tb_item_label->set_layer(layer_);

									tb_item_label->set_color(n_color);

									tb_item_label->set_text_height(height);

									style_ = style.toupper();
									tb_item_label->set_text_style(style_);

									_tb_class->tb_item_list.AddTail(tb_item_label);

									lineList.GetNext(i);
									cont ++;				
								}

								pMText->close();
							}

							pEntity->close();
						}
					}
				}
			}
		}
		*current_id1=_fid;
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

int TB_Class::LoadSecondaries(bool isOracle, CStringList *reserved_list)
{                                               
	int ret = GS_GOOD;

	C_SECONDARY *pSec;

	C_INT_INT_STR_LIST	sec_list;
	C_INT_INT_STR		*sec;
	
	TB_Class	*tb_sec;
	TB_Item_Sec	*tb_item_sec = NULL;

	C_STRING 		name;

	_RecordsetPtr  pRs;

	C_RB_LIST	Row;
	presbuf		p;
	long		key_attrib, key_pri;

	TRY
	{
		ptb_sec_list = new TB_Class_List();

		// elenco secondarie
		sec_list.remove_all();
		pCls->rtab_sec(sec_list);
		if (sec_list.get_count() > 0)
		{
			sec = (C_INT_INT_STR *) sec_list.get_head();
			while (sec)
			{
				if (sec->get_type() == GSInternalSecondaryTable)
				{
					if ((pSec = (C_SECONDARY*) pCls->find_sec(sec->get_key())) == NULL)
					{
						sec = (C_INT_INT_STR *) sec_list.get_next();
						continue;
					}

					name = pCls->get_name();
					if (pCls->ptr_id()->sub_code != 0)
					{
						name += "_";
						name += get_f_class_id();
					}
					name += "_";
					name += pSec->get_name();

					tb_sec = new TB_Class(NULL, reserved_list);
					tb_sec->set_f_class_id(get_f_class_id());
					tb_sec->set_f_prj_name(get_f_prj_name());
					tb_sec->set_f_class_name(name);
					tb_sec->set_caption(pSec->get_name());
					tb_item_sec = new TB_Item_Sec();
					tb_item_sec->get_item_attribute_list(&tb_sec->tb_item_attribute_list);
					tb_sec->set_f_class_type(_T("T"));
					tb_sec->set_attribute_list(isOracle, pSec->ptr_attrib_list(), pSec->ptr_info());

   					// dati secondarie
					if (get_row_values(pSec, pRs) == GS_GOOD)
					{  
						// lettura record
						while (gsc_isEOF(pRs) == GS_BAD)
						{  // lettura record
							if (gsc_DBReadRow(pRs, Row) == GS_BAD)
							{ 
								gsc_DBCloseRs(pRs); 
								return NULL; 
							}

							if ((p = Row.CdrAssoc("KEY_ATTRIB", FALSE)) == GS_BAD) 
							{ 
								gsc_DBCloseRs(pRs); 
								return NULL; 
							}
							gsc_rb2Lng(p, &key_attrib);
							if ((p = Row.CdrAssoc("KEY_PRI", FALSE)) == GS_BAD) 
							{ 
								gsc_DBCloseRs(pRs); 
								return NULL; 
							}
							gsc_rb2Lng(p, &key_pri);

							tb_item_sec = new TB_Item_Sec(key_attrib, key_pri);
							tb_sec->UpdateAttribs(tb_item_sec, pSec);
							tb_sec->tb_item_list.AddTail(tb_item_sec);

							gsc_Skip(pRs);
						}
						gsc_DBCloseRs(pRs);
					}

					ptb_sec_list->AddTail(tb_sec);
				}
				
				sec = (C_INT_INT_STR *) sec_list.get_next();
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

int TB_Class::WriteSql(bool isOracle, FILE *file, bool create, C_STRING &prj_name, 
                       long srid, long *current_id1, long *current_id2, CStringList *reserved_list)
{      
	int ret = GS_GOOD;

	C_STRING	str;

	TRY
	{
		Load(isOracle, prj_name, current_id1, current_id2, reserved_list);
		
		if (fwprintf(file, _T("%s\n"), _T("BEGIN;")) < 0)
		{
			if (gsc_fclose(file) == GS_BAD)
				AfxThrowUserException();
			AfxThrowUserException();
		}

		if (isOracle)
			CreateTableOracle(file, create, false, reserved_list);
		else
			CreateTablePostgres(file, create, false, srid, reserved_list);

		if (fwprintf(file, _T("%s\n"), _T("COMMIT;")) < 0)
		{
			if (gsc_fclose(file) == GS_BAD)
				AfxThrowUserException();
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

int TB_Class::UpdateAttribs(TB_Item *tb_item) 
{
	int ret = GS_GOOD;

	C_PREPARED_CMD	pCmd;

	C_RB_LIST		ColValues;
	C_STRING		_time;

	presbuf			p = NULL;
	C_STRING			str;
	int				i;
	long			lng;
	double			dbl;

	TB_Attrib		*tb_attrib;
	TB_Attribute	*tb_attribute;

	POSITION	pos;

	C_DBCONNECTION   *pConn;

	C_INT		*item;
	C_STRING	TableRef, statement1, statement2, LnkTableRef;
	_RecordsetPtr  pRs1, pRs2;
	long		qty1, qty2;
	C_RB_LIST	ColValues1, ColValues2;
	presbuf		pka, pei, pci;

	int			key_attrib, ent_id, complex_id;

	int			pos1;
	
	C_CLASS		*pComplex;
	C_INT_LIST	*pList;

	TRY
	{
		// Leggo valori DB
		// Preparo i comandi di lettura dei dati della classe dal temp/old
		if (pCls->prepare_data(pCmd, OLD) == GS_BAD)
			AfxThrowUserException( );
		if (gsc_get_data(pCmd, tb_item->get_fid(), ColValues) == GS_BAD)
			AfxThrowUserException( );

		if (tb_attribute_list.GetCount() > 0)
		{
			pos = tb_attribute_list.GetHeadPosition();
			while (pos)
			{
				tb_attribute = (TB_Attribute*) tb_attribute_list.GetNext(pos);		

				if ((p = ColValues.CdrAssoc(tb_attribute->get_name().get_name())) == NULL) // strano ma vero!
					AfxThrowUserException();
				if (p->restype != RTNIL)
				{
					switch (p->restype)
					{
						case RTSTR:
								// Ricavo la connessione OLE-DB alla tabella OLD
							if ((pConn = pCls->ptr_info()->getDBConnection(OLD)) == NULL) 
								AfxThrowUserException();

							if (pConn->IsDate(tb_attribute->get_srcType().get_name()) == GS_GOOD)
							{
								_time.clear();
								gsc_getSQLDateTime(p->resval.rstring, _time);
								str = _time.get_name();						
								tb_attrib = new TB_Attrib(tb_attribute->get_name(), &str);
							}
							else
							{
								str = p;
								geowebRightString(&str);
								if ((str.find(_T("<")) > -1) && (str.find(_T(">"))>-1))
								{
									pos1 = str.find(_T(">"));
									str = str.right(str.len() - (pos1+1));
									str.strtran(_T("\\"), _T("/"));
								}
								tb_attrib = new TB_Attrib(tb_attribute->get_name(), &str);
							}
							break;

						case RTREAL:
                  {
                     C_STRING dummy;

							gsc_rb2Dbl(p, &dbl);
                     dummy = dbl;
							tb_attrib = new TB_Attrib(tb_attribute->get_name(), &dummy);
   							break;
                  }
						case RTSHORT:
                  {
                     C_STRING dummy;

							gsc_rb2Int(p, &i);
                     dummy = i;
							tb_attrib = new TB_Attrib(tb_attribute->get_name(), &dummy);
							break;
                  }
						case RTLONG:
                  {
                     C_STRING dummy;

                     gsc_rb2Lng(p, &lng);
                     dummy = lng;
							tb_attrib = new TB_Attrib(tb_attribute->get_name(), &dummy);
							break;
                  }
						default:
							tb_attrib = new TB_Attrib(tb_attribute->get_name());
							break;
					}
				}
				else
					tb_attrib = new TB_Attrib(tb_attribute->get_name());

				tb_item->tb_attrib_list.AddTail(tb_attrib);
			}
		}

		if (info->count_complex() > 0)
		{
			pList = info->get_complex_list();
			item = (C_INT*) pList->get_head();
			while (item)
			{
				if (((C_PROJECT*) pCls->ptr_id()->pPrj)->getGrpsTabInfo(&pConn, &TableRef) == GS_BAD)
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
				if (qty1 == 1)
				{
					if (gsc_InitDBReadRow(pRs1, ColValues1) == GS_BAD)
					{ 
						gsc_DBCloseRs(pRs1);
						AfxThrowUserException();
					}
					pci = ColValues1.CdrAssoc("CLASS_ID");

					if (gsc_DBReadRow(pRs1, ColValues1) == GS_BAD) 
					{ 
						gsc_DBCloseRs(pRs1); 
						AfxThrowUserException();
					}
					// codice della classe collegata 
					if (gsc_rb2Int(pci, &complex_id) == GS_BAD) 
					{ 
						gsc_DBCloseRs(pRs1); 
						AfxThrowUserException();
					}
				}
				gsc_DBCloseRs(pRs1);


				pComplex = gsc_find_class(((C_PROJECT*) pCls->ptr_id()->pPrj)->get_key(), complex_id, 0);
				pComplex->getOldLnkTableRef(LnkTableRef);
				TableRef = LnkTableRef;

				statement1 = "SELECT KEY_ATTRIB FROM ";
				statement1 += TableRef;
				statement1 += " WHERE CLASS_ID=";
				statement1 += info->get_class_index();
				statement1 += " AND ENT_ID=";
				statement1 += tb_item->get_fid();

				// leggo le righe della tabella senza bloccarla
				if (pComplex->ptr_info()->getDBConnection(OLD)->ExeCmd(statement1, pRs1) == GS_BAD)
					AfxThrowUserException();
				if (gsc_RowsQty(pRs1, &qty1) == GS_BAD)
					AfxThrowUserException();
				if (qty1 == 1)
				{
					if (gsc_InitDBReadRow(pRs1, ColValues1) == GS_BAD)
					{ 
						gsc_DBCloseRs(pRs1);
						AfxThrowUserException();
					}
					pka = ColValues1.CdrAssoc("KEY_ATTRIB");

					if (gsc_DBReadRow(pRs1, ColValues1) == GS_BAD) 
					{ 
						gsc_DBCloseRs(pRs1); 
						AfxThrowUserException();
					}
					// codice della classe collegata 
					if (gsc_rb2Int(pka, &key_attrib) == GS_BAD) 
					{ 
						gsc_DBCloseRs(pRs1); 
						AfxThrowUserException();
					}

					statement2 = "SELECT ENT_ID FROM ";
					statement2 += TableRef;
					statement2 += " WHERE CLASS_ID=";
					statement2 += item->get_key();
					statement2 += " AND KEY_ATTRIB=";
					statement2 += key_attrib;

					// leggo le righe della tabella senza bloccarla
					if (pComplex->ptr_info()->getDBConnection(OLD)->ExeCmd(statement2, pRs2) == GS_BAD)
						AfxThrowUserException();
					if (gsc_RowsQty(pRs2, &qty2) == GS_BAD)
						AfxThrowUserException();
					if (qty2 == 1)
					{
						if (gsc_InitDBReadRow(pRs2, ColValues2) == GS_BAD)
						{ 
							gsc_DBCloseRs(pRs2);
							AfxThrowUserException();
						}
						pei = ColValues2.CdrAssoc("ENT_ID");

						if (gsc_DBReadRow(pRs2, ColValues2) == GS_BAD) 
						{ 
							gsc_DBCloseRs(pRs2); 
							AfxThrowUserException();
						}
						// codice della classe collegata 
						if (gsc_rb2Int(pei, &ent_id) == GS_BAD) 
						{ 
							gsc_DBCloseRs(pRs2); 
							AfxThrowUserException();
						}

                  C_STRING dummy1(_T("complex_fid_parent")), dummy2;

                  dummy2 = ent_id;
						tb_attrib = new TB_Attrib(dummy1, &dummy2);
						tb_item->tb_attrib_list.AddTail(tb_attrib);

						info->set_complex(true);
					}
					gsc_DBCloseRs(pRs2);
				}
				gsc_DBCloseRs(pRs1);

				item = (C_INT*) pList->get_next();
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

int TB_Class::UpdateAttribs(TB_Item *tb_item, C_SECONDARY *pSec) 
{
	int ret = GS_GOOD;

	C_PREPARED_CMD	pCmd;

	C_RB_LIST		ColValues;
	
	presbuf			p = NULL;
	C_STRING			str;
	double			dbl;

	TB_Attrib		*tb_attrib;
	TB_Attribute	*tb_attribute;

	POSITION	pos;

	TRY
	{
		// Leggo valori DB
		// Preparo i comandi di lettura dei dati della classe dal temp/old
		if (pSec->prepare_data(pCmd, OLD) == GS_BAD)
			AfxThrowUserException( );
		if (gsc_get_data(pCmd, tb_item->get_fid(), ColValues) == GS_BAD)
			AfxThrowUserException( );

		if (tb_attribute_list.GetCount() > 0)
		{
			pos = tb_attribute_list.GetHeadPosition();
			while (pos)
			{
				tb_attribute = (TB_Attribute*) tb_attribute_list.GetNext(pos);			

				if ((p = ColValues.CdrAssoc(tb_attribute->get_name().get_name())) == NULL) // strano ma vero !
					AfxThrowUserException();
				if (p->restype != RTNIL)
				{
					if ((tb_attribute->get_type().find(_T("int")) > -1) || (tb_attribute->get_type().find(_T("float")) > -1))
					{
                  C_STRING dummy;

						gsc_rb2Dbl(p, &dbl);
                  dummy = dbl;
						tb_attrib = new TB_Attrib(tb_attribute->get_name(), &dummy);
					}
					else
					{
						if ((tb_attribute->get_type().find(_T("char")) > -1) || (tb_attribute->get_type().find(_T("time")) > -1))
						{
							str = p;
							geowebRightString(&str);
							tb_attrib = new TB_Attrib(tb_attribute->get_name(), &str);
						}
						else
							tb_attrib = new TB_Attrib(tb_attribute->get_name());
					}
				}
				else
					tb_attrib = new TB_Attrib(tb_attribute->get_name());

				tb_item->tb_attrib_list.AddTail(tb_attrib);
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

int TB_Class::UpdateAttribsSecExt(TB_Item *tb_item, C_RB_LIST &row) 
{
	int ret = GS_GOOD;

	presbuf			p = NULL;
	C_STRING			str;
	double			dbl;

	TB_Attrib		*tb_attrib;
	TB_Attribute	*tb_attribute;

	POSITION	pos;

	TRY
	{					
		if (tb_attribute_list.GetCount() > 0)
		{
			pos = tb_attribute_list.GetHeadPosition();
			while (pos)
			{
				tb_attribute = (TB_Attribute*) tb_attribute_list.GetNext(pos);			

				if ((p = row.CdrAssoc(tb_attribute->get_name().get_name())) == NULL) // strano ma vero !
					AfxThrowUserException();
				if (p->restype != RTNIL)
				{
					if ((tb_attribute->get_type().find(_T("int")) > -1) || (tb_attribute->get_type().find(_T("float")) > -1))
					{
                  C_STRING dummy;

						gsc_rb2Dbl(p, &dbl);
                  dummy = dbl;
						tb_attrib = new TB_Attrib(tb_attribute->get_name(), &dummy);
					}
					else
					{
						if ((tb_attribute->get_type().find(_T("char")) > -1) || (tb_attribute->get_type().find(_T("time")) > -1))
						{
							str = p;
							geowebRightString(&str);
							tb_attrib = new TB_Attrib(tb_attribute->get_name(), &str);
						}
						else
							tb_attrib = new TB_Attrib(tb_attribute->get_name());
					}
				}
				else
					tb_attrib = new TB_Attrib(tb_attribute->get_name());

				tb_item->tb_attrib_list.AddTail(tb_attrib);
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

//AcDb3DPolyline
//AcDbLWPolyline
//AcDbPolyline
//AcDbLine
//AcDbCircle
//AcDbArc			
//AcDbSolid
//AcDbEllipse
// Funzione di memorizzazione dei punti e del fattore di bulge di una polyline
int TB_Class::GetPointsOfObj(AcDbObject *pObject, CList<AcGePoint3d,AcGePoint3d> &point_list, double *length, double *area)
{
	int ret = GS_GOOD;

	AcDbPolyline	*pPolyline;
	AcDb3dPolyline	*p3dPolyline;
	AcDb2dPolyline	*p2dPolyline;
	AcDbLine		*pLine;
	AcDbCircle		*pCircle;
	AcDbArc			*pArc;
	AcDbEllipse		*pEllipse;
	AcDbFace		*pFace;
	AcDbPolyFaceMesh	*pPolyFaceMesh;	//modificare 2009
	AcDbFaceRecord	*pFaceRecord;

	AcDbObjectIterator	*pObjectIterator;
	AcDb3dPolylineVertex	*p3dPolylineVertex;
	AcDbVertex				*pVertex;
   AcDb2dVertex         *p2dPolylineVertex; // roby

	AcDbObjectId	objectId;
	AcGePoint3d		point3d, tpoint3d, startPoint3d, endPoint3d;
	AcGePoint2d		point2d;
	AcGeCircArc2d	*pArc2d, arc;
	AcGePoint2dArray array;

	int	num, i, numj, j;
	POSITION pos, pos1;
	double bulge, radius;
    ads_point center;
    presbuf	prb, temp;
	bool isArc;
	double	endParam, offset, dist, len;

	CList<AcGePoint3d,AcGePoint3d>	point_list_temp;
	CList<double,double>			bulge_list_temp;

	TRY
	{
		*length = 0;
		*area = 0;

		// Tipo POLYLINE - POLYLINELW
		if (pObject->isKindOf(AcDbPolyline::desc()))
		{
			pPolyline = AcDbPolyline::cast(pObject);

			// numero di vertici della POLYLINE
			num = pPolyline->numVerts();
			if (num == 2)
			{
				pPolyline->getPointAt(0, point3d);
				pPolyline->getPointAt(1, tpoint3d);
				if ((point3d.x == tpoint3d.x) && (point3d.y == tpoint3d.y))
					return GS_GOOD;
			}

			pPolyline->getEndParam(endParam);
			pPolyline->getDistAtParam(endParam, *length);
			pPolyline->getArea(*area);

			// ciclo sui vertici
			for (i = 0; i < num; i++)
			{
				// coordinate del vertice corrente
				pPolyline->getPointAt(i, point3d);
				point_list_temp.AddTail(point3d);
				
				// bulge del vertice corrente
				bulge = 0;
				pPolyline->getBulgeAt(i, bulge);
				if (bulge != 0)
				{
					if (pPolyline->getArcSegAt(i, arc) != Acad::eOk) 
						bulge = 0;
					else
					{
						radius = arc.radius();
						if (radius == 0)
							bulge = 0;
					}
				}
				bulge_list_temp.AddTail(bulge);
			}

			// Verifica se è un polyline chiusa
			if ((pPolyline->isClosed() == Adesk::kTrue) && (num > 2)) 
			{
				// Verifica i punti per la eventuale chiusura della Polilinea
				startPoint3d = point_list_temp.GetHead();
				endPoint3d = point_list_temp.GetTail();
		
				// Verifica l'uguaglianza dei punti
				if (!(startPoint3d == endPoint3d))
				{
					point_list_temp.AddTail(startPoint3d);
					bulge_list_temp.AddTail(bulge);
				}
			}
			
			pPolyline->close();  
		}
		else if (pObject->isKindOf(AcDb3dPolyline::desc())) 		// Tipo 3DPOLYLINE
		{
			// 3dPolyline non ha bulge
			p3dPolyline = AcDb3dPolyline::cast(pObject);
		
			i = 0;
			pObjectIterator = p3dPolyline->vertexIterator();
			for (; !pObjectIterator->done();  pObjectIterator->step())
				i++;

			p3dPolyline->getEndParam(endParam);
			p3dPolyline->getDistAtParam(endParam, *length);
			p3dPolyline->getArea(*area);

			pObjectIterator->start();
			if (i == 2)
			{
				objectId = pObjectIterator->objectId();
				if (((AcDb3dPolyline*) pObject)->openVertex(p3dPolylineVertex, objectId, AcDb::kForRead) == Acad::eOk)
				{
					// Memorizzo il vertice
					point3d = ((AcDb3dPolylineVertex*) p3dPolylineVertex)->position();
					p3dPolylineVertex->close();
				}
				pObjectIterator->step();
				objectId = pObjectIterator->objectId();
				if (((AcDb3dPolyline*) pObject)->openVertex(p3dPolylineVertex, objectId, AcDb::kForRead) == Acad::eOk)
				{
					// Memorizzo il vertice
					tpoint3d = ((AcDb3dPolylineVertex*) p3dPolylineVertex)->position();
					p3dPolylineVertex->close();
				}

				if ((point3d.x == tpoint3d.x) && (point3d.y == tpoint3d.y) && (point3d.z == tpoint3d.z))
				{
					delete pObjectIterator;
					return GS_GOOD;
				}
			}
		
			pObjectIterator->start();
			for (; !pObjectIterator->done();  pObjectIterator->step())
			{
				objectId = pObjectIterator->objectId();
				if (((AcDb3dPolyline*) pObject)->openVertex(p3dPolylineVertex, objectId, AcDb::kForRead) == Acad::eOk)
				{
					// Memorizzo il vertice
					point3d = ((AcDb3dPolylineVertex*) p3dPolylineVertex)->position();
					// Aggiunta dei valori letti alle liste
					point_list.AddTail(point3d);

					p3dPolylineVertex->close();
				}
			}
			
			delete pObjectIterator;

			// Verifica se è un polyline chiusa
			if ((p3dPolyline->isClosed() == Adesk::kTrue) && (i > 2))
			{
				// Verifica i punti per la eventuale chiusura della Polilinea
				startPoint3d = point_list.GetHead();
				endPoint3d = point_list.GetTail();

				if (!(startPoint3d == endPoint3d))
					point_list.AddTail(startPoint3d);
			}
			
			p3dPolyline->close();		
		}
		else if (pObject->isKindOf(AcDb2dPolyline::desc()))     
		{
			p2dPolyline = AcDb2dPolyline::cast(pObject);

			i = 0;
			pObjectIterator = p2dPolyline->vertexIterator();
			for (; !pObjectIterator->done();  pObjectIterator->step())
				i++;

			p2dPolyline->getEndParam(endParam);
			p2dPolyline->getDistAtParam(endParam, *length);
			p2dPolyline->getArea(*area);

			pObjectIterator->start();
			if (i == 2)
			{
				objectId = pObjectIterator->objectId();
				if (((AcDb2dPolyline*) pObject)->openVertex(p2dPolylineVertex, objectId, AcDb::kForRead) == Acad::eOk)
				{
					// Memorizzo il vertice
					point3d = p2dPolylineVertex->position();
					p2dPolylineVertex->close();
				}
				
				pObjectIterator->step();
				objectId = pObjectIterator->objectId();
				if (((AcDb2dPolyline*) pObject)->openVertex(p2dPolylineVertex, objectId, AcDb::kForRead) == Acad::eOk)
				{
					// Memorizzo il vertice
					tpoint3d = p2dPolylineVertex->position();
					p2dPolylineVertex->close();
				}

				if ((point3d.x == tpoint3d.x) && (point3d.y == tpoint3d.y))
				{
					delete pObjectIterator;
					return GS_GOOD;
				}
			}

			pObjectIterator->start();
			for (; !pObjectIterator->done();  pObjectIterator->step())
			{
				objectId = pObjectIterator->objectId();
				if (((AcDb2dPolyline*) pObject)->openVertex(p2dPolylineVertex, objectId, AcDb::kForRead) == Acad::eOk)
				{
					// Memorizzo il vertice
					point3d = p2dPolylineVertex->position();

					// Aggiunta dei valori letti alle liste
					point_list.AddTail(point3d);
					p2dPolylineVertex->close();
				}
			}
			delete pObjectIterator;

			// Verifica se è un polyline chiusa
			if ((p2dPolyline->isClosed() == Adesk::kTrue) && (i > 2))
			{
				// Verifica i punti per la eventuale chiusura della Polilinea
				startPoint3d = point_list.GetHead();
				endPoint3d = point_list.GetTail();

				if (startPoint3d.x && endPoint3d.x)
				{
					if (!(startPoint3d == endPoint3d))
						point_list.AddTail(startPoint3d);
				}
			}	
					
			p2dPolyline->close();
		}
		else if (pObject->isKindOf(AcDbLine::desc()))
		{
			pLine = AcDbLine::cast(pObject);

			startPoint3d = pLine->startPoint();
			endPoint3d = pLine->endPoint();

			pLine->getEndParam(endParam);
			pLine->getDistAtParam(endParam, *length);
			pLine->getArea(*area);

			if ((startPoint3d.x == endPoint3d.x) && (startPoint3d.y == endPoint3d.y))
				return GS_GOOD;

			if (!(startPoint3d == endPoint3d))
			{
				point_list.AddTail(startPoint3d);
				point_list.AddTail(endPoint3d);
			}
			
			pLine->close();
		}
		else if (pObject->isKindOf(AcDbCircle::desc()))
		{
			pCircle = AcDbCircle::cast(pObject);

			pCircle->getEndParam(endParam);
			pCircle->getDistAtParam(endParam, *length);
			pCircle->getArea(*area);

			point3d = pCircle->center();
			radius = pCircle->radius();
			center[X] = point3d.x;
			center[Y] = point3d.y;
			center[Z] = point3d.z;

			if (gsc_circle_point(center, radius, num_vertex, &prb) == GS_GOOD)
			{
				point_list.RemoveAll();
				
				i = 1;
				temp = prb;
				while (temp != NULL)
				{
					if (temp->restype == RTPOINT)
					{
						point3d.x = temp->resval.rpoint[0];
						point3d.y = temp->resval.rpoint[1];
						point3d.z = temp->resval.rpoint[2];

						if (i == 1)
							startPoint3d = point3d;

						// Aggiunta dei valori letti alle liste
						point_list.AddTail(point3d);
					}
					temp = temp->rbnext;
					i++;
				}
				acutRelRb(prb);

				// Per chiudere il cerchio
				point_list.AddTail(startPoint3d);
			}
		
			pCircle->close();
		}
		else if (pObject->isKindOf(AcDbArc::desc()))
		{
			pArc = AcDbArc::cast(pObject);
			
			pArc->getEndParam(endParam);
			pArc->getDistAtParam(endParam, *length);
			pArc->getArea(*area);

			point3d = pArc->center();
			point2d.x = point3d.x;
			point2d.y = point3d.y;
			pArc2d = new AcGeCircArc2d(point2d, pArc->radius(), pArc->startAngle(), pArc->endAngle());

			pArc2d->getSamplePoints(100, array);
			num = array.length();
			for (i=0; i<num; i++)
			{
				point2d = array.at(i);

				tpoint3d.x = point2d.x;
				tpoint3d.y = point2d.y;
				tpoint3d.z = 0;
				point_list.AddTail(tpoint3d);
			}

			free(pArc2d);
			pArc->close();
		}
		else if (pObject->isKindOf(AcDbEllipse::desc()))
		{
			pEllipse = AcDbEllipse::cast(pObject);
			
			pEllipse->getEndParam(endParam);
			pEllipse->getDistAtParam(endParam, len);
			pEllipse->getEndParam(endParam);
			pEllipse->getDistAtParam(endParam, *length);
			pEllipse->getArea(*area);

			// primo punto
			pEllipse->getPointAtDist(0, tpoint3d);

			point_list.RemoveAll();							
			point_list.AddTail(tpoint3d);
			// punti successivi fino al penultimo compreso
			offset = len / num_vertex;
			dist = offset;
			while (dist < len)
			{
				pEllipse->getPointAtDist(dist, point3d);
				point_list.AddTail(point3d);
				dist += offset;
			}
			// ultimo punto = al primo punto
			point_list.AddTail(tpoint3d);

			pEllipse->close();
		}
		else if (pObject->isKindOf(AcDbFace::desc()))
		{
			pFace = AcDbFace::cast(pObject);

			// Ricavo i vertici per TB eventualmente corretti
			pFace->getVertexAt(0, point3d);
			pFace->getVertexAt(1, tpoint3d);
			pFace->getVertexAt(2, startPoint3d);
			pFace->getVertexAt(3, endPoint3d);

			point_list.RemoveAll();							
			point_list.AddTail(point3d);
			point_list.AddTail(tpoint3d);
			point_list.AddTail(startPoint3d);
			point_list.AddTail(endPoint3d);
			point_list.AddTail(point3d);

			pFace->close();
		} 
		else if (pObject->isKindOf(AcDbPolyFaceMesh::desc())) 		// Tipo MESH	//modificare 2009
		{
			pPolyFaceMesh = AcDbPolyFaceMesh::cast(pObject);
		
			i = pPolyFaceMesh->numVertices();
			i = 0;
			pObjectIterator = pPolyFaceMesh->vertexIterator();
			for (; !pObjectIterator->done();  pObjectIterator->step())
				i++;

			//////pPolyFaceMesh->getEndParam(endParam);
			//////pPolyFaceMesh->getDistAtParam(endParam, *length);
			//////pPolyFaceMesh->getArea(*area);

			pObjectIterator->start();
			if (i == 2)
			{
				objectId = pObjectIterator->objectId();
				if (((AcDbPolyFaceMesh*) pObject)->openVertex(pVertex, objectId, AcDb::kForRead) == Acad::eOk)
				{
					// Memorizzo il vertice
					point3d = ((AcDbPolyFaceMeshVertex*) pVertex)->position();
					pVertex->close();
				}
				pObjectIterator->step();
				objectId = pObjectIterator->objectId();
				if (((AcDbPolyFaceMesh*) pObject)->openVertex(pVertex, objectId, AcDb::kForRead) == Acad::eOk)
				{
					// Memorizzo il vertice
					tpoint3d = ((AcDbPolyFaceMeshVertex*) pVertex)->position();
					pVertex->close();
				}

				if ((point3d.x == tpoint3d.x) && (point3d.y == tpoint3d.y) && (point3d.z == tpoint3d.z))
				{
					delete pObjectIterator;
					return GS_GOOD;
				}
			}
		
			pObjectIterator->start();
			for (; !pObjectIterator->done();  pObjectIterator->step())
			{
				objectId = pObjectIterator->objectId();
				if (((AcDbPolyFaceMesh*) pObject)->openVertex(pVertex, objectId, AcDb::kForRead) == Acad::eOk)
				{
					if ((pVertex->isKindOf(AcDbPolyFaceMeshVertex::desc())) ||
						(pVertex->isKindOf(AcDb2dVertex::desc())) ||
						(pVertex->isKindOf(AcDb3dPolylineVertex::desc())) ||
						(pVertex->isKindOf(AcDbPolygonMeshVertex::desc())))
					{
						if (pVertex->isKindOf(AcDbPolyFaceMeshVertex::desc()))
							point3d = ((AcDbPolyFaceMeshVertex*) pVertex)->position();
						else if(pVertex->isKindOf(AcDb2dVertex::desc()))
							point3d = ((AcDb2dVertex*) pVertex)->position();
						else if(pVertex->isKindOf(AcDb3dPolylineVertex::desc()))
							point3d = ((AcDb3dPolylineVertex*) pVertex)->position();
						else if(pVertex->isKindOf(AcDbPolygonMeshVertex::desc()))
							point3d = ((AcDbPolygonMeshVertex*) pVertex)->position();
						point_list.AddTail(point3d);	// Aggiunta dei valori letti alle liste
					}
					else if(pVertex->isKindOf(AcDbFaceRecord::desc()))
					{
						pFaceRecord = AcDbFaceRecord::cast(pVertex);

						Adesk::Int16 a, b, c, d;
						Acad::ErrorStatus err;

						// Ricavo i vertici per TB eventualmente corretti
						err=pFaceRecord->getVertexAt(0, a);
						err=pFaceRecord->getVertexAt(1, b);
						err=pFaceRecord->getVertexAt(2, c);
						err=pFaceRecord->getVertexAt(3, d);

						/*point_list.RemoveAll();*/							
						point_list.AddTail(point3d);
						point_list.AddTail(tpoint3d);
						point_list.AddTail(startPoint3d);
						point_list.AddTail(endPoint3d);
						/*point_list.AddTail(point3d);*/
					}
 
					pVertex->close();
				}
			}
			
			delete pObjectIterator;

			// Verifica se è un polyline chiusa
			//if ((pPolyFaceMesh->isClosed() == Adesk::kTrue) && (i > 2))
			//{
			//	// Verifica i punti per la eventuale chiusura della Polilinea
			//	startPoint3d = point_list.GetHead();
			//	endPoint3d = point_list.GetTail();

			//	if (!(startPoint3d == endPoint3d))
			//		point_list.AddTail(startPoint3d);
			//}
			
			pPolyFaceMesh->close();		
		}


		// Solo nel caso di AcDbPolyline possibile esistenza di archi, 
		// pertanto la lista dei punti deve essere reinterpretata
		if (pObject->isKindOf(AcDbPolyline::desc()))     
		{
			pPolyline = AcDbPolyline::cast(pObject);
   
			// Ciclo sulle liste dei punti e dei bulge per calcolare i punti degli archi
			i = 1;
			num = (long) point_list_temp.GetCount();

			isArc = false;
			pos = point_list_temp.GetHeadPosition();
			while (pos)
			{
				point3d = point_list_temp.GetNext(pos);

				// verifico il bulge
				pos1 = bulge_list_temp.FindIndex(i-1);
				if (pos1)
				{
					bulge = bulge_list_temp.GetAt(pos1);
					if (bulge != 0)
					{
						isArc = true;

						if (pPolyline->getArcSegAt(i - 1, arc) != Acad::eOk) 
							continue;

						arc.getSamplePoints(10, array);
						numj = array.length();
						for (j=0; j<numj; j++)
						{
							point2d = array.at(j);

							tpoint3d.x = point2d.x;
							tpoint3d.y = point2d.y;
							tpoint3d.z = 0;
							point_list.AddTail(tpoint3d);
						}
					}
					else
					{
						if (!isArc)
							point_list.AddTail(point3d);
						isArc = false;
					}
				}
				
				i++;
			}

			pPolyline->close();
			      
			// Svuoto le due liste temporanee per il controllo dei vertici
			point_list_temp.RemoveAll();
			bulge_list_temp.RemoveAll();
		}

		// A questo punto se la lista dei punti contiene solo due vertici
		// vado a verificare l'uguaglianza di questi, e se sono coincidenti svuoto
		// tutto e restituisco GS_GOOD
		if (point_list.GetCount() == 2)
		{
			startPoint3d = point_list.GetHead();
			endPoint3d = point_list.GetTail();

			if (startPoint3d == endPoint3d) 
				point_list.RemoveAll();
		}
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

int get_row_values(C_SECONDARY *pSec, _RecordsetPtr &pRs)
{                                               
	C_STRING          tablename, statement, LnkTableRef;   
	C_DBCONNECTION   *pConn;

	if (pSec->ptr_info() == NULL) 
		return GS_BAD;

	// Ricavo la connessione OLE-DB alla tabella OLD
	if ((pConn = pSec->ptr_info()->getDBConnection(OLD)) == NULL) 
		return GS_BAD;

	if (pSec->get_type() == GSInternalSecondaryTable)
	{                                               
		// ricavo riferimento a tabella OLD dei link
		if (pSec->getOldLnkTableRef(LnkTableRef) == GS_BAD) 
			return GS_BAD;

		// Verifico l' esistenza della tabella
		if (pConn->ExistTable(pSec->ptr_info()->OldLnkTableRef.get_name()) != GS_GOOD)
			return GS_BAD;

		// Costruisco l' istruzione Sql
		statement = "SELECT * FROM ";
		statement += pSec->ptr_info()->OldLnkTableRef; 

		if (pConn->OpenRecSet(statement, pRs, adOpenKeyset) == GS_BAD) 
			return GS_BAD;
	}
	else
	{                                               
		// Verifico l' esistenza della tabella
		if (pConn->ExistTable(pSec->ptr_info()->OldTableRef.get_name()) != GS_GOOD)
			return GS_BAD;

		// Costruisco l' istruzione Sql
		statement = "SELECT * FROM ";
		statement += pSec->ptr_info()->OldTableRef; 

		if (pConn->OpenRecSet(statement, pRs) == GS_BAD) 
			return GS_BAD;
	}

	return GS_GOOD;
}

int TB_Class::WriteSecExt(bool isOracle, FILE *file, C_STRING &prj_name, CStringList *reserved_list)
{      
	int ret = GS_GOOD;

	C_STRING	str;

	TRY
	{
		LoadSecondariesExt(isOracle, reserved_list);

		if (fwprintf(file, _T("%s\n"), _T("BEGIN;")) < 0)
		{
			if (gsc_fclose(file) == GS_BAD)
				AfxThrowUserException();
			AfxThrowUserException();
		}

		if (isOracle)
			CreateTableSecsExtOracle(file, prj_name);
		else
			CreateTableSecsExtPostgres(file, prj_name);

		if (fwprintf(file, _T("%s\n"), _T("COMMIT;")) < 0)
		{
			if (gsc_fclose(file) == GS_BAD)
				AfxThrowUserException();
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

int TB_Class::LoadSecondariesExt(bool isOracle, CStringList *reserved_list)
{                                               
	int ret = GS_GOOD;

	C_SECONDARY *pSec;

	C_INT_INT_STR_LIST	sec_list;
	C_INT_INT_STR		*sec;
	
	TB_Class	*tb_sec;
	TB_Item_Sec	*tb_item_sec = NULL;

	C_STRING		name;

	_RecordsetPtr  pRs;

	C_RB_LIST	Row;

	TRY
	{
		ptb_sec_list = new TB_Class_List();

		// elenco secondarie
		sec_list.remove_all();
		pCls->rtab_sec(sec_list);
		if (sec_list.get_count() > 0)
		{
			sec = (C_INT_INT_STR *) sec_list.get_head();
			while (sec)
			{
				if (sec->get_type() != GSInternalSecondaryTable)
				{
					if ((pSec = (C_SECONDARY*) pCls->find_sec(sec->get_key())) == NULL)
					{
						sec = (C_INT_INT_STR *) sec_list.get_next();
						continue;
					}

					name = pCls->get_name();
					if (pCls->ptr_id()->sub_code != 0)
					{
						name += "_";
						name += get_f_class_id();
					}
					name += "_";
					name = pSec->get_name();

					tb_sec = new TB_Class(NULL, reserved_list);
					tb_sec->set_f_class_id(pSec->code);
					tb_sec->set_f_prj_name(pCls->ptr_id()->pPrj->get_name());
					tb_sec->set_f_class_name(name);
					tb_sec->set_caption(pSec->get_name());
					tb_item_sec = new TB_Item_Sec();
					tb_sec->set_f_class_type(_T("T"));
					tb_sec->set_attribute_list(isOracle, pSec->ptr_attrib_list(), pSec->ptr_info());

   					// dati secondarie
					if (get_row_values(pSec, pRs) == GS_GOOD)
					{  
						long qty;
						gsc_RowsQty(pRs, &qty);

						// lettura record
						while (gsc_isEOF(pRs) == GS_BAD)
						{  
							Row.remove_all();
							// lettura record
							if (gsc_DBReadRow(pRs, Row) == GS_BAD)
							{ 
								gsc_DBCloseRs(pRs); 
								return NULL; 
							}

							tb_item_sec = new TB_Item_Sec();
							tb_sec->UpdateAttribsSecExt(tb_item_sec, Row);
							tb_sec->tb_item_list.AddTail(tb_item_sec);

							gsc_Skip(pRs);
						}
						gsc_DBCloseRs(pRs);
					}

					ptb_sec_list->AddTail(tb_sec);
				}

				sec = (C_INT_INT_STR *) sec_list.get_next();
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