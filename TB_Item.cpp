/*************************************************************************/
/* INCLUDES                                                              */
/*************************************************************************/
#include "stdafx.h"

#include "TB_Item.h"

IMPLEMENT_DYNAMIC(TB_Item, CObject)
IMPLEMENT_DYNAMIC(TB_Item_Label, CObject)
IMPLEMENT_DYNAMIC(TB_Item_Point, CObject)
IMPLEMENT_DYNAMIC(TB_Item_Text, CObject)
IMPLEMENT_DYNAMIC(TB_Item_Surface, CObject)
IMPLEMENT_DYNAMIC(TB_Item_Polyline_L, CObject)
IMPLEMENT_DYNAMIC(TB_Item_Polyline, CObject)
IMPLEMENT_DYNAMIC(TB_Item_Sec, CObject)

TB_Item::TB_Item() 
{
	fid = 0;
	job_version = 0;
	old = false;
}

TB_Item::TB_Item(long _l, bool _b)
{
	fid = _l;
	job_version = 0;
	old = _b;
}

TB_Item::~TB_Item() 
{
	POSITION pos;

	TB_Attrib		*tb_attrib;

	if (tb_attrib_list.GetCount() > 0)
	{
		pos = tb_attrib_list.GetHeadPosition();
		if (pos)
		{	
			tb_attrib = (TB_Attrib*) tb_attrib_list.GetNext(pos);
			delete tb_attrib;
		}
	}
}

void TB_Item::set_fid(long _l)
{  
	fid = _l;
}

void TB_Item::set_job_version(long _l)
{  
	job_version = _l;
}

long TB_Item::get_fid()
{  
	return fid;
}

long TB_Item::get_job_version()
{  
	return job_version;
}

bool TB_Item::is_old()
{  
	return old;
}

int TB_Item::WriteSQLPostgres(FILE *file, C_STRING &_prjname, C_STRING &_name,
                              CObList* tb_attribute_list, CObList* tb_item_attribute_list, CStringList *reserved_list, long srid, C_DBCONNECTION *pConn) 
{
	int ret = GS_GOOD;

	C_STRING str1, str2, str3;

	TRY
	{
		str1 = "";
		str2 = "";
		WriteSQLPostgres(str1, str2, tb_attribute_list, tb_item_attribute_list, reserved_list, pConn, srid);

		if (str1 != _T("") || str2 != _T(""))
		{
			str3 = "INSERT INTO ";
			str3 += _prjname;
			//str3 += "@SCHEMA@";
			str3 += ".";
			str3 += correctString4(_name);
			str3 += " (";
			if (fwprintf(file, _T("%s"), str3) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}

			if (fwprintf(file, _T("%s"), str1) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}
			if (fwprintf(file, _T("%s"), _T(") VALUES (")) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}
			if (fwprintf(file, _T("%s"), str2) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}
			if (fwprintf(file, _T("%s\n"), _T(");")) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
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

int TB_Item::WriteSQLOracle(FILE *file, C_STRING &_prjname, C_STRING &_name, CObList* tb_attribute_list,
                            CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn) 
{
	int      ret = GS_GOOD;
	C_STRING str1, str2, str3, str4;

	TRY
	{
		str1 = "";
		str2 = "";
		WriteSQLOracle(str1, str2, tb_attribute_list, tb_item_attribute_list, reserved_list, pConn);

		if ((str1 != "") || (str2 != ""))
		{
			str3 = "INSERT INTO \"";
			str4 = _prjname;
			str4.toupper();
			str3 += str4;
			str3 += "\".\"";
			str4 = correctString4(_name);
			str4.toupper();
			str3 += str4;
			str3 += "\" (";
			if (fwprintf(file, _T("%s"), str3) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}

			if (fwprintf(file, _T("%s"), str1) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}
			if (fwprintf(file, _T("%s"), _T(") VALUES (")) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}
			if (fwprintf(file, _T("%s"), str2) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}
			if (fwprintf(file, _T("%s\n"), _T(");")) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
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

int TB_Item::WriteSQLPostgres(FILE *file, C_STRING &_prjname, C_STRING &_name) 
{
	int ret = GS_GOOD;
	C_STRING str1;
	long	_fid_parent;

	TRY
	{
		str1 = "INSERT INTO ";
		str1 += _prjname;
		//str1 += "@SCHEMA@";
		str1 += ".";
		str1 += correctString4(_name);
		str1 += " (fid";
		if (fwprintf(file, _T("%s"), str1) < 0)
		{
			if (gsc_fclose(file) == GS_BAD)
				AfxThrowUserException();
			AfxThrowUserException();
		}
		if (fwprintf(file, _T("%s"), _T(") VALUES (")) < 0)
		{
			if (gsc_fclose(file) == GS_BAD)
				AfxThrowUserException();
			AfxThrowUserException();
		}
		get_fid_parent(&_fid_parent);
		if (_fid_parent > 0)
			if (fwprintf(file, _T("%ld"), _fid_parent) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}		
		if (fwprintf(file, _T("%s\n"), _T(");")) < 0)
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

int TB_Item::WriteSQLOracle(FILE *file, C_STRING &_prjname, C_STRING &_name) 
{
	int ret = GS_GOOD;

	C_STRING str1, str2;

	long	_fid_parent;

	TRY
	{
		str1 = "INSERT INTO \"";
		str2 = _prjname;
		str2.toupper();
		str1 += str2;
		str1 += "\".\"";
		str2 = correctString4(_name);
		str2.toupper();
		str1 += str2;
		str1 += "\" (\"FID\"";
		if (fwprintf(file, _T("%s"), str1) < 0)
		{
			if (gsc_fclose(file) == GS_BAD)
				AfxThrowUserException();
			AfxThrowUserException();
		}
		if (fwprintf(file, _T("%s"), _T(") VALUES (")) < 0)
		{
			if (gsc_fclose(file) == GS_BAD)
				AfxThrowUserException();
			AfxThrowUserException();
		}
		get_fid_parent(&_fid_parent);
		if (_fid_parent > 0)
			if (fwprintf(file, _T("%ld"), _fid_parent) < 0)
			{
				if (gsc_fclose(file) == GS_BAD)
					AfxThrowUserException();
				AfxThrowUserException();
			}		
		if (fwprintf(file, _T("%s\n"), _T(");")) < 0)
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

TB_Item* TB_Item_List::find(long _fid)
{
	TB_Item	*tb_item = NULL;

	POSITION	pos;

	if (GetCount() > 0)
	{
		pos = GetHeadPosition();
		while (pos)
		{
			tb_item = (TB_Item*) GetNext(pos);
			if (tb_item->get_fid() == _fid)
				return tb_item;
		}
	}

	return NULL;
}

TB_Item* TB_Item_List::find_fid_parent(long _fid_parent, AcGePoint3d _point3d)
{
	TB_Item_Point	*tb_item_point = NULL;

	C_LONG_LIST		long_list;
	C_POINT_LIST	point_list;

	POSITION	pos;

	C_LONG	*_long;
	C_POINT *punt, *nearp = NULL;
	double  dist, min = -1;

	ads_point dummy;

	long	fid_parent, _fid, _id;

	int		i, k;

	if (GetCount() > 0)
	{
		pos = GetHeadPosition();
		while (pos)
		{
			tb_item_point = (TB_Item_Point*) GetNext(pos);
			tb_item_point->get_fid_parent(&fid_parent);
			if (fid_parent == _fid_parent)
			{
				long_list.add_tail_long(tb_item_point->get_fid());
				point_list.add_point(tb_item_point->get_x(), tb_item_point->get_y(), 0);
			}
		}
	}

	if (point_list.get_count() > 0)
	{
		dummy[X] = _point3d.x;
		dummy[Y] = _point3d.y;
		dummy[Z] = 0;

		i=0;
		punt = (C_POINT *) point_list.get_head();
		while (punt)
		{
			i++;
			if ((dist = ads_distance(dummy, punt->point)) < min || min == -1)
			{
				min = dist;
				k = i;
				nearp = punt;
			}
			punt = (C_POINT *) punt->get_next();
		}
	}

	if (nearp)
	{
		_long = (C_LONG *) long_list.getptr_at(k);
		if (_long)
			_id = _long->get_id();
		else
			_id = -1;

		pos = GetHeadPosition();
		while (pos)
		{
			tb_item_point = (TB_Item_Point*) GetNext(pos);
			_fid = tb_item_point->get_fid();
			if (_fid == _id)
				return tb_item_point;
		}
	}

	return NULL;
}

void TB_Item::get_item_attribute_list(CObList* tb_item_attribute_list)
{  
	TB_Attribute	*tb_attribute;

	tb_attribute = new TB_Attribute("fid", "int4", "", 10, 0, false, "");
	tb_item_attribute_list->AddTail(tb_attribute);
}



TB_Item_Label::TB_Item_Label(long _fid, double _orientation, long _fid_parent, 
                             C_STRING _label_text, C_STRING _field_name, 
							        int _hidden, double _x, double _y, double _x_label_block,
                             double _y_label_block) : TB_Item(_fid)
{
	//suf;
	orientation = _orientation;
	//label_def_id;
	fid_parent = _fid_parent;    
	label_text = _label_text;
	field_name = _field_name;
	//pre;
	hidden = _hidden;
	//orientation_degree;
	x = _x;
	y = _y;
	//id_label_block;
	//orientation_label_block;
	x_label_block = _x_label_block;
	y_label_block = _y_label_block;

	text_height = 1;
}

void TB_Item_Label::set_text_height(double _d)
{
	text_height = _d;
}

void TB_Item_Label::set_text_style(C_STRING _s)
{
	text_style = _s;
}

void TB_Item_Label::set_layer(C_STRING _s)
{
	layer = _s;
}

void TB_Item_Label::set_color(C_COLOR &_l)
{
	n_color = _l;
}

C_STRING TB_Item_Label::WriteGeomPostgres(long srid)
{  
	C_STRING str, StrNum;

	str = "GeometryFromText('POINT(";
   StrNum.paste(gsc_tostring(x, 12, 3)); // gsc_tostring alloca memoria
	str += StrNum;
	str += " ";
   StrNum.paste(gsc_tostring(y, 12, 3)); // gsc_tostring alloca memoria
	str += StrNum;
	str += ")', ";
	str += srid;
	str += ")";

	return str;
}

C_STRING TB_Item_Label::WriteGeomOracle()
{  
	C_STRING str, StrNum;

	str = "SDO_GEOMETRY(2001, NULL, SDO_POINT_TYPE(";
   StrNum.paste(gsc_tostring(x, 12, 3)); // gsc_tostring alloca memoria
	str += StrNum;
	str += ", ";
   StrNum.paste(gsc_tostring(y, 12, 3)); // gsc_tostring alloca memoria
	str += StrNum;
	str += ", NULL), NULL, NULL)";
	
	return str;
}
void TB_Item_Label::WriteSQLPostgres(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list,
                                     CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn, long srid) 
{
	POSITION	    pos;
	TB_Attribute *tb_attribute;
	C_STRING	    layer1, StrNum;
	
	if ((label_text != "") && (hidden != 1))
	{
		if (tb_item_attribute_list->GetCount() > 0)
		{  
			pos = tb_item_attribute_list->GetHeadPosition();
			while (pos)
			{  
				tb_attribute = (TB_Attribute*) tb_item_attribute_list->GetNext(pos);
				if (str1.len() > 0)
					str1 += ", ";
				str1 += tb_attribute->get_name();
			}
		}

		geom = WriteGeomPostgres(srid);

		str2 = get_fid();
		str2 += ", ";
   		str2 += geom;
		str2 += ", ";
      StrNum.paste(gsc_tostring(orientation, 7, 3)); // gsc_tostring alloca memoria
		str2 += StrNum;
		str2 += ", ";
		str2 += fid_parent;
		str2 += ", '";
		str2 += correctString3(label_text);
		str2 += "', ";
		if (field_name != "")
		{  			
			str2 +=	"'";
			str2 += correctString3(field_name);
			str2 += "'";
		}
		else
			str2 += "NULL";
		str2 += ", ";
		str2 += hidden;
		str2 += ", ";
		if (layer != "")
		{  
			layer1 = correctString3(layer);
			layer1.toupper();
			str2 += "'";
			str2 += layer1;
			str2 += "'";
		}
		else
			str2 += "NULL";
		str2 += ", ";
		if (n_color.getColorMethod() != C_COLOR::None)
      {
         int AutoCADColorIndex;
         n_color.getAutoCADColorIndex(&AutoCADColorIndex);
			str2 += AutoCADColorIndex;
      }
		else
			str2 += "NULL";
		str2 += ", ";
		if (text_height != 1)
			str2 += text_height;
		else
			//str2 += "NULL";
			str2 += "1";
		str2 += ", ";
		if (text_style != "")
		{  
			str2 += "'";
			str2 += text_style;
			str2 += "'";
		}
		else
			str2 += "NULL";		
	}
}

void TB_Item_Label::WriteSQLOracle(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list,
                                   CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn) 
{
	POSITION		pos;
	TB_Attribute	*tb_attribute;
	C_STRING			layer1;
	C_STRING			str3, StrNum;
	
	if ((label_text != "") && (hidden != 1))
	{
		if (tb_item_attribute_list->GetCount() > 0)
		{  
			pos = tb_item_attribute_list->GetHeadPosition();
			while (pos)
			{  
				tb_attribute = (TB_Attribute*) tb_item_attribute_list->GetNext(pos);
				if (str1.len() > 0)
					str1 += ", ";
				str3 = tb_attribute->get_name();
				str3.toupper();
				str1 += "\"";
				str1 += str3;
				str1 += "\"";
			}
		}

		geom = WriteGeomOracle();

		str2 = get_fid();
		str2 += ", ";
		str2 += geom;
		str2 += ", ";
      StrNum.paste(gsc_tostring(orientation, 7, 3)); // gsc_tostring alloca memoria
		str2 += StrNum;
		str2 += ", ";
		str2 += fid_parent;
		str2 += ", '";
		str2 += correctString3(label_text);
		str2 += "', '";
		str2 += field_name;
		str2 += "', ";
		str2 += hidden;
		str2 += ", ";
		if (layer != "")
		{  
			layer1 = correctString3(layer);
			layer1.toupper();
			str2 += "'";
			str2 += layer1;
			str2 += "'";
		}
		else
			str2 += "NULL";
		str2 += ", ";
		if (n_color.getColorMethod() != C_COLOR::None)
      {
         int AutoCADColorIndex;
         n_color.getAutoCADColorIndex(&AutoCADColorIndex);
			str2 += AutoCADColorIndex;
      }
		else
			str2 += "NULL";
		str2 += ", ";
		if (text_height != 1)
			str2 += text_height;
		else
			//str2 += "NULL";
			str2 += "1";
		str2 += ", ";
		if (text_style != "")
		{  
			str2 += "'";
			str2 += text_style;
			str2 += "'";
		}
		else
			str2 += "NULL";		
	}
}

void TB_Item_Label::get_item_attribute_list(CObList* tb_item_attribute_list)
{  
	TB_Attribute	*tb_attribute;

	tb_attribute = new TB_Attribute("fid", "int4", "", 10, 0, false, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("orientation_degree", "float4", "", 6, 3, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("fid_parent", "int4", "", 10, 0, false, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("label_text", "varchar", "", 255, 0, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("field_name", "varchar", "", 255, 0, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("hidden", "int4", "", 10, 0, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("layer", "varchar", "", 255, 0, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("n_color", "int4", "", 10, 0, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("text_height", "float8", "", 20, 8, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("text_style", "varchar", "", 255, 0, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
}



TB_Item_Point::TB_Item_Point(long _fid, double _orientation, double _x, double _y, double _z, long _fid_parent) : TB_Item(_fid)
{
	orientation = _orientation;
	quality = -1;
	orientation_degree = -1;
	x = _x;
	y = _y;
    z = _z;
	fid_parent = _fid_parent;
	
	block_scale_factor = 0;
}

void TB_Item_Point::set_block_name(C_STRING &_s)
{  
	block_name = correctString1(_s);
}

void TB_Item_Point::set_block_scale_factor(double _d)
{  
	block_scale_factor = _d;
}

void TB_Item_Point::set_layer(C_STRING &_s)
{  
	layer = _s;
}

void TB_Item_Point::set_color(C_COLOR &_l)
{  
	n_color = _l;
}

void TB_Item_Point::set_fid_parent(long _l)
{  
	fid_parent = _l;
}

void TB_Item_Point::WriteSQLPostgres(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list,
                                     CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn, long srid)
{  
	POSITION	    pos;
	TB_Attribute *tb_attribute;
	long	       _fid_parent;
	C_STRING	     layer1, StrNum;

	if (tb_item_attribute_list->GetCount() > 0)
	{  
		pos = tb_item_attribute_list->GetHeadPosition();
		while (pos)
		{  
			tb_attribute = (TB_Attribute*) tb_item_attribute_list->GetNext(pos);
			if (str1.len() > 0)
				str1 += ", ";
				str1 += tb_attribute->get_name();
		}
	}

	str1 += tb_attrib_list.writeSQL(false, str1, reserved_list);

	geom = WriteGeomPostgres(srid);

	str2 = get_fid();
	str2 += ", ";
	str2 += geom;
	str2 += ", ";
   StrNum.paste(gsc_tostring(orientation, 7, 3)); // gsc_tostring alloca memoria
	str2 += StrNum;
	str2 += ", ";
	if (block_name != "")
	{  
		str2 += "'";
		str2 += correctString3(block_name);
		str2 += "'";
	}
	else
		str2 += "NULL";
	str2 += ", ";
	if (block_scale_factor != 0)
		str2 += block_scale_factor;
	else
		str2 += "NULL";
	str2 += ", ";
	if (layer != "")
	{  
		layer1 = correctString3(layer); 
		layer1.toupper();
		str2 += "'";
		str2 += layer1; 
		str2 += "'";
	}
	else
		str2 += "NULL";
	str2 += ", ";
	if (n_color.getColorMethod() != C_COLOR::None)
   {
      int AutoCADColorIndex;
      n_color.getAutoCADColorIndex(&AutoCADColorIndex);
		str2 += AutoCADColorIndex;
   }
	else
		str2 += "NULL";
	get_fid_parent(&_fid_parent);
	if (_fid_parent > 0)
	{  
		str2 += ", ";
		str2 += _fid_parent;
	}
	str2 += tb_attrib_list.writeSQL(str2, tb_attribute_list, pConn);
}

void TB_Item_Point::WriteSQLOracle(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list,
                                   CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn)
{  
	POSITION	    pos;
	TB_Attribute *tb_attribute;
	long	       _fid_parent;
	C_STRING	    layer1;
   C_STRING	    str3, StrNum;

	if (tb_item_attribute_list->GetCount() > 0)
	{  
		pos = tb_item_attribute_list->GetHeadPosition();
		while (pos)
		{  
			tb_attribute = (TB_Attribute*) tb_item_attribute_list->GetNext(pos);
			if (str1.len() > 0)
				str1 += ", ";
			str3 = tb_attribute->get_name();
			str3.toupper();
			str1 += "\"";
			str1 += str3;
			str1 += "\"";
		}
	}

	str1 += tb_attrib_list.writeSQL(true, str1, reserved_list);

	geom = WriteGeomOracle();

	str2 = get_fid();
	str2 += ", ";
	str2 += geom;
	str2 += ", ";
   StrNum.paste(gsc_tostring(orientation, 7, 3)); // gsc_tostring alloca memoria
	str2 += StrNum;
	str2 += ", ";
	if (block_name != "")
	{  
		str2 += "'";
		str2 += correctString3(block_name);
		str2 += "'";
	}
	else
		str2 += "NULL";
	str2 += ", ";
	if (block_scale_factor != 0)
		str2 += block_scale_factor;
	else
		str2 += "NULL";
	str2 += ", ";
	if (layer != "")
	{  
		layer1 = correctString3(layer); 
		layer1.toupper();
		str2 += "'";
		str2 += layer1; 
		str2 += "'";
	}
	else
		str2 += "NULL";
	str2 += ", ";
	if (n_color.getColorMethod() != C_COLOR::None)
   {
      int AutoCADColorIndex;
      n_color.getAutoCADColorIndex(&AutoCADColorIndex);
		str2 += AutoCADColorIndex;
   }
	else
		str2 += "NULL";
	get_fid_parent(&_fid_parent);
	if (_fid_parent > 0)
	{  
		str2 += ", ";
		str2 += _fid_parent;
	}
	str2 += tb_attrib_list.writeSQL(str2, tb_attribute_list, pConn);
}

C_STRING TB_Item_Point::WriteGeomPostgres(long srid)
{  
	C_STRING str, StrNum;

	str = "GeometryFromText('POINT(";
   StrNum.paste(gsc_tostring(x, 12, 3)); // gsc_tostring alloca memoria
	str += StrNum;
	str += " ";
   StrNum.paste(gsc_tostring(y, 12, 3)); // gsc_tostring alloca memoria
	str += StrNum;
	str += ")', ";
	str += srid;
	str += ")";
	
	return str;
}

C_STRING TB_Item_Point::WriteGeomOracle()
{  
	C_STRING str, StrNum;

	str = "SDO_GEOMETRY(2001, NULL, SDO_POINT_TYPE(";
   StrNum.paste(gsc_tostring(x, 12, 3)); // gsc_tostring alloca memoria
	str += StrNum;
	str += ", ";
   StrNum.paste(gsc_tostring(y, 12, 3)); // gsc_tostring alloca memoria
	str += StrNum;
	str += ", NULL), NULL, NULL)";
	
	return str;
}

void TB_Item_Point::get_item_attribute_list(CObList* tb_item_attribute_list, bool parent)
{  
	TB_Attribute	*tb_attribute;

	tb_attribute = new TB_Attribute("fid", "int4", "", 10, 0, false, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("orientation_degree", "float4", "", 6, 3, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("block_name", "varchar", "", 255, 0, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("block_scale_factor", "float8", "", 20, 8, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("layer", "varchar", "", 255, 0, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("n_color", "int4", "", 10, 0, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	if (parent)
	{  
		tb_attribute = new TB_Attribute("fid_parent", "int4", "", 10, 0, false, "");
		tb_item_attribute_list->AddTail(tb_attribute);
	}
}

void TB_Item_Point::get_fid_parent(long *_fid_parent)
{  
	*_fid_parent = fid_parent;
}

double TB_Item_Point::get_x()
{  
	return x;
}

double TB_Item_Point::get_y()
{  
	return y;
}

TB_Item_Surface::TB_Item_Surface(bool isOracle, long _fid, double _area, double _length, CList<AcGePoint3d,AcGePoint3d> *point_list, long _fid_parent) : TB_Item(_fid)
{
   C_STRING StrNum;
	geom = "";
	area = _area;
	length = _length;
	fid_parent = _fid_parent;

	layer = "";
	hatch_l_width = -1;
	hatch_space = -1;
	l_width = -1;
	thickness = -1;

	AcGePoint3d	point3d, tpoint3d;

	int			i;
	POSITION	pos;

	// Se esistono almeno tre punti validi comincio la scrittura
	if (point_list->GetCount() > 2)
	{
		geom = "";
		for (i = 1; i <= point_list->GetCount(); i++)
		{
			pos = point_list->FindIndex(i-1);
			point3d = point_list->GetAt(pos);

			if (geom.len() > 0)
				geom += ", ";
			else 
				geom = "(";
			if (isOracle)
			{
            StrNum.paste(gsc_tostring(point3d.x, 12, 3)); // gsc_tostring alloca memoria
	         geom += StrNum;
				geom += ",";
            StrNum.paste(gsc_tostring(point3d.y, 12, 3)); // gsc_tostring alloca memoria
	         geom += StrNum;
			}	
			else
			{
            StrNum.paste(gsc_tostring(point3d.x, 12, 3)); // gsc_tostring alloca memoria
	         geom += StrNum;
				geom += " ";
            StrNum.paste(gsc_tostring(point3d.y, 12, 3)); // gsc_tostring alloca memoria
	         geom += StrNum;
			}
		}
	
		if (geom.len() > 0)
			geom += ")";
	}
	else
	{
		point3d = point_list->GetHead();
		tpoint3d = point_list->GetTail();

		if (geom.len() > 0)
			geom += ", ";
		else 
			geom = "(";
		if (isOracle)
		{
         StrNum.paste(gsc_tostring(point3d.x, 12, 3)); // gsc_tostring alloca memoria
	      geom += StrNum;
			geom += ",";
         StrNum.paste(gsc_tostring(point3d.y, 12, 3)); // gsc_tostring alloca memoria
	      geom += StrNum;
			geom += ", ";
         StrNum.paste(gsc_tostring(tpoint3d.x, 12, 3)); // gsc_tostring alloca memoria
	      geom += StrNum;
			geom += ",";
         StrNum.paste(gsc_tostring(tpoint3d.y, 12, 3)); // gsc_tostring alloca memoria
	      geom += StrNum;
			geom += ", ";		
         StrNum.paste(gsc_tostring(point3d.x, 12, 3)); // gsc_tostring alloca memoria
	      geom += StrNum;
			geom += ",";
         StrNum.paste(gsc_tostring(point3d.y, 12, 3)); // gsc_tostring alloca memoria
	      geom += StrNum;
		}
		else
		{
         StrNum.paste(gsc_tostring(point3d.x, 12, 3)); // gsc_tostring alloca memoria
	      geom += StrNum;
			geom += " ";
         StrNum.paste(gsc_tostring(point3d.y, 12, 3)); // gsc_tostring alloca memoria
	      geom += StrNum;
			geom += ", ";
         StrNum.paste(gsc_tostring(tpoint3d.x, 12, 3)); // gsc_tostring alloca memoria
	      geom += StrNum;
			geom += " ";
         StrNum.paste(gsc_tostring(tpoint3d.y, 12, 3)); // gsc_tostring alloca memoria
	      geom += StrNum;
			geom += ", ";		
         StrNum.paste(gsc_tostring(point3d.x, 12, 3)); // gsc_tostring alloca memoria
	      geom += StrNum;
			geom += " ";
         StrNum.paste(gsc_tostring(point3d.y, 12, 3)); // gsc_tostring alloca memoria
	      geom += StrNum;
		}

		if (geom.len() > 0)
			geom += ")";
	}
}

TB_Item_Surface::TB_Item_Surface(long _fid, double _area) : TB_Item(_fid)
{
	geom = "";
	area = _area;
	hatch_l_width = -1;
	hatch_space = -1;
	l_width = -1;
	thickness = -1;
	length = -1;
	fid_parent = -1;
}

void TB_Item_Surface::get_fid_parent(long *_fid_parent)
{  
	*_fid_parent = fid_parent;
}

void TB_Item_Surface::set_area(double _d)
{  
	area = _d;
}

void TB_Item_Surface::update_area(double _d)
{  
	if (area > _d)
        area = area - _d;
	else
		area = area + _d;
}

void TB_Item_Surface::set_length(double _d)
{  
	length = _d;
}

void TB_Item_Surface::update_length(double _d)
{  
	if (length > _d)
		length = length - _d;
	else
		length = length + _d;
}

void TB_Item_Surface::set_layer(C_STRING _s)
{  
	layer = _s;
}

void TB_Item_Surface::set_l_type(C_STRING _s)
{  
	l_type = _s;
}

void TB_Item_Surface::set_color(C_COLOR &_l)
{  
	n_color = _l;
}

void TB_Item_Surface::set_thickness(double _d)
{  
	thickness = _d;
}

void TB_Item_Surface::set_l_width(double _d)
{  
	l_width = _d;
}

void TB_Item_Surface::set_hatch(C_STRING _h)
{  
	hatch_name = _h;
}

void TB_Item_Surface::set_hatch_color(C_COLOR &_l)
{  
	hatch_color = _l;
}

void TB_Item_Surface::set_hatch_space(double _d)
{  
	hatch_space = _d;
}

void TB_Item_Surface::Update(bool isOracle, CList<AcGePoint3d,AcGePoint3d> *point_list)
{
	AcGePoint3d	point3d, tpoint3d;
	int			i;
	POSITION	   pos;
   C_STRING    StrNum;

	// Se esistono almeno tre punti validi comincio la scrittura
	if (point_list->GetCount() > 2)
	{
		if (geom.len() > 0)
			geom += ", ";

		for (i = 1; i <= point_list->GetCount(); i++)
		{
			pos = point_list->FindIndex(i-1);
			point3d = point_list->GetAt(pos);

			if (i == 1)
				geom += "(";
			else 
				geom += ", ";
			if (isOracle)
			{
            StrNum.paste(gsc_tostring(point3d.x, 12, 3)); // gsc_tostring alloca memoria
	         geom += StrNum;
				geom += ",";
            StrNum.paste(gsc_tostring(point3d.y, 12, 3)); // gsc_tostring alloca memoria
	         geom += StrNum;
			}
			else
			{
            StrNum.paste(gsc_tostring(point3d.x, 12, 3)); // gsc_tostring alloca memoria
	         geom += StrNum;
				geom += " ";
            StrNum.paste(gsc_tostring(point3d.y, 12, 3)); // gsc_tostring alloca memoria
	         geom += StrNum;
			}
		}
	
		if (geom.len() > 0)
			geom += ")";
	}
}

void TB_Item_Surface::WriteSQLPostgres(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list,
                                       CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn, long srid)
{
	POSITION	pos;
	TB_Attribute	*tb_attribute;
	long	_fid_parent;
	C_STRING		layer1;

	if (tb_item_attribute_list->GetCount() > 0)
	{  
		pos = tb_item_attribute_list->GetHeadPosition();
		while (pos)
		{  
			tb_attribute = (TB_Attribute*) tb_item_attribute_list->GetNext(pos);
			if (str1.len() > 0)
				str1 += ", ";
			str1 += tb_attribute->get_name();
		}
	}

	str1 += tb_attrib_list.writeSQL(false, str1, reserved_list);

	geom = WriteGeomPostgres(srid);

	str2 = get_fid();
	str2 += ", ";
	str2 += geom;
	str2 += ", ";
	str2 += area;
	str2 += ", ";
	str2 += length;
	str2 += ", ";
	if (hatch_name != "")
	{  
		str2 += "'";
		str2 += hatch_name;
		str2 += "'";
	}
	else
		str2 += "NULL";
	str2 += ", ";
	if (hatch_l_width != -1)
		str2 += hatch_l_width;
	else
		str2 += "NULL";	
	str2 += ", ";
	if (hatch_l_type != "")
	{  
		str2 += "'";
		str2 += hatch_l_type;
		str2 += "'";
	}
	else
		str2 += "NULL";
	str2 += ", ";
	if (hatch_color.getColorMethod() != C_COLOR::None)
   {
      int AutoCADColorIndex;
      hatch_color.getAutoCADColorIndex(&AutoCADColorIndex);
		str2 += AutoCADColorIndex;
   }
	else
		str2 += "NULL";
	str2 += ", ";
	if (hatch_space != -1)
		str2 += hatch_space;
	else
		str2 += "NULL";
	str2 += ", ";
	if (layer != "")
	{  
		layer1 = correctString3(layer); 
		layer1.toupper();
		str2 += "'";
		str2 += layer1;
		str2 += "'";
	}
	else
		str2 += "NULL";
	str2 += ", ";
	if (l_type != "")
	{  
		str2 += "'";
		str2 += correctString3(l_type);
		str2 += "'";
	}
	else
		str2 += "NULL";
	str2 += ", ";
	if (l_width != -1)
		str2 += l_width;
	else
		str2 += "NULL";
	str2 += ", ";
	if (n_color.getColorMethod() != C_COLOR::None)
   {
      int AutoCADColorIndex;
      n_color.getAutoCADColorIndex(&AutoCADColorIndex);
		str2 += AutoCADColorIndex;
   }
	else
		str2 += "NULL";
	str2 += ", ";
	if (thickness != -1)
		str2 += thickness;
	else
		str2 += "NULL";
	get_fid_parent(&_fid_parent);
	if (_fid_parent > 0)
	{  
		str2 += ", ";
		str2 += _fid_parent;
	}
	str2 += tb_attrib_list.writeSQL(str2, tb_attribute_list, pConn);
}

void TB_Item_Surface::WriteSQLOracle(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list,
                                     CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn)
{
	POSITION	pos;
	TB_Attribute	*tb_attribute;	
	long	_fid_parent;
	C_STRING		layer1;
	C_STRING		str3;

	if (tb_item_attribute_list->GetCount() > 0)
	{  
		pos = tb_item_attribute_list->GetHeadPosition();
		while (pos)
		{  
			tb_attribute = (TB_Attribute*) tb_item_attribute_list->GetNext(pos);
			if (str1.len() > 0)
				str1 += ", ";
			str3 = tb_attribute->get_name();
			str3.toupper();
			str1 += "\"";
			str1 += str3;
			str1 += "\"";
		}
	}

	str1 += tb_attrib_list.writeSQL(true, str1, reserved_list);

	geom = WriteGeomOracle();

	str2 = get_fid();
	str2 += ", ";
	str2 += geom;
	str2 += ", ";
	str2 += area;
	str2 += ", ";
	str2 += length;
	str2 += ", ";
	if (hatch_name != "")
	{  
		str2 += "'";
		str2 += hatch_name;
		str2 += "'";
	}
	else
		str2 += "NULL";
	str2 += ", ";
	if (hatch_l_width != -1)
		str2 += hatch_l_width;
	else
		str2 += "NULL";	
	str2 += ", ";
	if (hatch_l_type != "")
	{  
		str2 += "'";
		str2 += hatch_l_type;
		str2 += "'";
	}
	else
		str2 += "NULL";
	str2 += ", ";
	if (hatch_color.getColorMethod() != C_COLOR::None)
   {
      int AutoCADColorIndex;
      hatch_color.getAutoCADColorIndex(&AutoCADColorIndex);
		str2 += AutoCADColorIndex;
   }
	else
		str2 += "NULL";
	str2 += ", ";
	if (hatch_space != -1)
		str2 += hatch_space;
	else
		str2 += "NULL";
	str2 += ", ";
	if (layer != "")
	{  
		layer1 = correctString3(layer); 
		layer1.toupper();
		str2 += "'";
		str2 += layer1;
		str2 += "'";
	}
	else
		str2 += "NULL";
	str2 += ", ";
	if (l_type != "")
	{  
		str2 += "'";
		str2 += correctString3(l_type);
		str2 += "'";
	}
	else
		str2 += "NULL";
	str2 += ", ";
	if (l_width != -1)
		str2 += l_width;
	else
		str2 += "NULL";
	str2 += ", ";
	if (n_color.getColorMethod() != C_COLOR::None)
   {
      int AutoCADColorIndex;
      n_color.getAutoCADColorIndex(&AutoCADColorIndex);
		str2 += AutoCADColorIndex;
   }
	else
		str2 += "NULL";
	str2 += ", ";
	if (thickness != -1)
		str2 += thickness;
	else
		str2 += "NULL";
	get_fid_parent(&_fid_parent);
	if (_fid_parent > 0)
	{  
		str2 += ", ";
		str2 += _fid_parent;
	}
	str2 += tb_attrib_list.writeSQL(str2, tb_attribute_list, pConn);
}

C_STRING TB_Item_Surface::WriteGeomPostgres(long srid)
{  
	C_STRING str;

	str = "GeometryFromText('POLYGON(";
	str += geom;
	str += ")', ";
	str += srid;
	str += ")";

	return str;
}

C_STRING TB_Item_Surface::WriteGeomOracle()
{
	C_STRING str, str1, left, right, info;
	int   	p, q, i, j, x;

	p = geom.find(_T("), ("));

	str = "SDO_GEOMETRY(2003, NULL, NULL, ";
	if (p > -1)
	{  
		i = 0;
		left = geom.left(p);
		right = geom.right(geom.len() - p - 4);
		str1 = "";

		info = "(1, 1003, 1";

		// rielaborazione della stringa per Oracle
		while (left.len() > 0)
		{  
			j = 0;
			q = left.find(_T(","));
			while (q > 1)
			{
				i++;
				j++;
				if (str1.len() > 0)
					str1 += ",";
				if ((j == 1) && (i > 1))
					str1 += " ";
				str1 += left.left(q);
				left = left.right(left.len() - q - 1);
				q = left.find(_T(","));	
			}
			str1 += ",";
			str1 += left;
			i++;
			x = i;
			x++;

			if (right.len() > 0)
			{
				if (info.len() > 0)
					info += ", ";
				info += x;
				info += ", 2003, 1";
			}

			p = right.find(_T("), ("));
			if (p > -1)
			{  
				left = right.left(p);
				right = right.right(right.len() - p - 4);
			}
			else
			{  
				left = right;
				right = "";
			}
		}
		info += "), ";

		str += "SDO_ELEM_INFO_ARRAY";
		str += info;
		str += "SDO_ORDINATE_ARRAY";
		str += str1;
	}
	else
	{  
		str += "SDO_ELEM_INFO_ARRAY(1, 1003, 1), ";
		str += "SDO_ORDINATE_ARRAY";
		str += geom;
	}
	str += ")";

	return str;
}
//{  
//	C_STRING str, str1, left, right, info;
//
//	int	p, q, i, j, x;
//
//	p = geom.Find(_T("), ("));
//
//	str = "SDO_GEOMETRY(2003, NULL, NULL, ";
//	if (p > -1)
//	{  
//		i = 0;
//		left = geom.Left(p);
//		right = geom.Right(geom.len() - p - 4);
//		str1 = "";
//
//		info = "(1, 1003, 1";
//
//		// rielaborazione della stringa per Oracle
//		while (left.len() > 0)
//		{  
//			j = 0;
//			q = left.Find(_T(","));
//			while (q > 1)
//			{
//				i++;
//				j++;
//				if (str1.len() > 0)
//					str1 += ",";
//				if ((j == 1) && (i > 1))
//					str1 += " ";
//				str1 += left.Left(q);
//				left = left.Right(left.len() - q - 1);
//				q = left.Find(_T(","));	
//			}
//			str1 += ",";
//			str1 += left;
//			i++;
//			x = i;
//			x++;
//
//			if (right.len() > 0)
//			{
//				if (info.len() > 0)
//					info += ", ";
//				info += x;
//				info += ", 2003, 1";
//			}
//
//			p = right.Find(_T("), ("));
//			if (p > -1)
//			{  
//				left = right.Left(p);
//				right = right.Right(right.len() - p - 4);
//			}
//			else
//			{  
//				left = right;
//				right = "";
//			}
//		}
//		info += "), ";
//
//		str += "SDO_ELEM_INFO_ARRAY";
//		str += info;
//		str += "SDO_ORDINATE_ARRAY";
//		str += str1;
//	}
//	else
//	{  
//		str += "SDO_ELEM_INFO_ARRAY(1, 1003, 1), ";
//		str += "SDO_ORDINATE_ARRAY";
//		str += geom;
//	}
//	str += ")";
//
//	return str;
//}

void TB_Item_Surface::get_item_attribute_list(CObList* tb_item_attribute_list, bool parent)
{  
	TB_Attribute	*tb_attribute;

	tb_attribute = new TB_Attribute("fid", "int4", "", 10, 0, false, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("area", "float8", "", 20, 8, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("length", "float8", "", 20, 8, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("hatch_name", "varchar", "", 255, 0, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("hatch_l_width", "float8", "", 20, 8, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("hatch_l_type", "varchar", "", 255, 0, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("hatch_color", "int4", "", 10, 0, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("hatch_space", "float8", "", 20, 8, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("layer", "varchar", "", 255, 0, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("l_type", "varchar", "", 255, 0, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("l_width", "float8", "", 20, 8, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("n_color", "int4", "", 10, 0, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("thickness", "float8", "", 20, 8, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	if (parent)
	{  
		tb_attribute = new TB_Attribute("fid_parent", "int4", "", 10, 0, false, "");
		tb_item_attribute_list->AddTail(tb_attribute);	
	}
}


TB_Item_Text::TB_Item_Text(long _fid, C_STRING _text, long _fid_parent) : TB_Item(_fid)
{
	text = _text;
	fid_parent = _fid_parent;
}

void TB_Item_Text::get_fid_parent(long *_fid_parent)
{  
	*_fid_parent = fid_parent;
}

void TB_Item_Text::WriteSQLPostgres(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list,
                                    CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn, long srid)
{  
	POSITION	pos;
	TB_Attribute	*tb_attribute;
	long		_fid_parent;

	if (tb_item_attribute_list->GetCount() > 0)
	{  
		pos = tb_item_attribute_list->GetHeadPosition();
		while (pos)
		{  
			tb_attribute = (TB_Attribute*) tb_item_attribute_list->GetNext(pos);
			if (str1.len() > 0)
				str1 += ", ";
			str1 += tb_attribute->get_name();
		}
	}

	str1 += tb_attrib_list.writeSQL(false, str1, reserved_list);

	str2 = get_fid();
	str2 += ", '";
	str2 += correctString3(text);
	str2 += "'";

	get_fid_parent(&_fid_parent);
	if (_fid_parent > 0)
	{  
		str2 += ", ";
		str2 += _fid_parent;
	}
	str2 += tb_attrib_list.writeSQL(str2, tb_attribute_list, pConn);
}

void TB_Item_Text::WriteSQLOracle(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list,
                                  CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn)
{  
	POSITION	pos;
	TB_Attribute	*tb_attribute;
	long		_fid_parent;
	C_STRING		str3;

	if (tb_item_attribute_list->GetCount() > 0)
	{  
		pos = tb_item_attribute_list->GetHeadPosition();
		while (pos)
		{  
			tb_attribute = (TB_Attribute*) tb_item_attribute_list->GetNext(pos);
			if (str1.len() > 0)
				str1 += ", ";
			str3 = tb_attribute->get_name();
			str3.toupper();
			str1 += "\"";
			str1 += str3;
			str1 += "\"";
		}
	}

	str1 += tb_attrib_list.writeSQL(true, str1, reserved_list);

	str2 = get_fid();
	str2 += ", '";
	str2 += correctString3(text);
	str2 += "'";

	get_fid_parent(&_fid_parent);
	if (_fid_parent > 0)
	{  
		str2 += ", ";
		str2 += _fid_parent;
	}
	str2 += tb_attrib_list.writeSQL(str2, tb_attribute_list, pConn);
}

void TB_Item_Text::get_item_attribute_list(CObList* tb_item_attribute_list, bool parent)
{  
	TB_Attribute	*tb_attribute;

	tb_attribute = new TB_Attribute("fid", "int4", "", 10, 0, false, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("text_", "varchar", "", 255, 0, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	if (parent)
	{  
		tb_attribute = new TB_Attribute("fid_parent", "int4", "", 10, 0, false, "");
		tb_item_attribute_list->AddTail(tb_attribute);
	}
}



TB_Item_Polyline::TB_Item_Polyline(long _fid, double _length) : TB_Item(_fid)
{
	length = _length;
}

void TB_Item_Polyline::WriteSQLPostgres(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list,
                                        CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn, long srid)
{  
	POSITION	pos;
	TB_Attribute	*tb_attribute;

	if (tb_item_attribute_list->GetCount() > 0)
	{  
		pos = tb_item_attribute_list->GetHeadPosition();
		while (pos)
		{  
			tb_attribute = (TB_Attribute*) tb_item_attribute_list->GetNext(pos);
			if (str1.len() > 0)
				str1 += ", ";
			str1 += tb_attribute->get_name();
		}
	}

	str1 += tb_attrib_list.writeSQL(false, str1, reserved_list);

	str2 = get_fid();
	str2 += ", ";
	str2 += length;
	str2 += tb_attrib_list.writeSQL(str2, tb_attribute_list, pConn);
}

void TB_Item_Polyline::WriteSQLOracle(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list,
                                      CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn)
{  
	POSITION	    pos;
	TB_Attribute *tb_attribute;
	C_STRING		 str3;

	if (tb_item_attribute_list->GetCount() > 0)
	{  
		pos = tb_item_attribute_list->GetHeadPosition();
		while (pos)
		{  
			tb_attribute = (TB_Attribute*) tb_item_attribute_list->GetNext(pos);
			if (str1.len() > 0)
				str1 += ", ";
			str3 = tb_attribute->get_name();
			str3.toupper();
			str1 += "\"";
			str1 += str3;
			str1 += "\"";
		}
	}

	str1 += tb_attrib_list.writeSQL(true, str1, reserved_list);

	str2 = get_fid();
	str2 += ", ";
	str2 += length;
	str2 += tb_attrib_list.writeSQL(str2, tb_attribute_list, pConn);
}

void TB_Item_Polyline::get_item_attribute_list(CObList* tb_item_attribute_list)
{  
	TB_Attribute	*tb_attribute;

	tb_attribute = new TB_Attribute("fid", "int4", "", 10, 0, false, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("length", "float8", "", 20, 8, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
}

void TB_Item_Polyline::update_length(double _d)
{  
	length = length + _d;
}



TB_Item_Polyline_L::TB_Item_Polyline_L(bool isOracle, long _fid, double _length, CList<AcGePoint3d,AcGePoint3d> *point_list, long _fid_parent) : TB_Item(_fid)
{
	geom = "";
	length = _length;
	fid_parent = _fid_parent;

	l_width = -1;
	thickness = -1;

	AcGePoint3d	point3d, tpoint3d;
	POSITION	   pos, pos1;
	int			i;
   C_STRING    StrNum;

	// Se esistono almeno due punti validi comincio la scrittura
	if (point_list->GetCount() > 1)
	{
		if (point_list->GetCount() == 2)
		{
			pos = point_list->FindIndex(0);
			point3d = point_list->GetAt(pos);
			pos1 = point_list->FindIndex(1);
			tpoint3d = point_list->GetAt(pos1);

			if ((point3d.x == tpoint3d.x) && (point3d.y == tpoint3d.y) && (point3d.z == tpoint3d.z))
				geom = "";
			else
			{
				if (isOracle)
				{
					geom = "(";
               StrNum.paste(gsc_tostring(point3d.x, 12, 3)); // gsc_tostring alloca memoria
	            geom += StrNum;
					geom += ",";
               StrNum.paste(gsc_tostring(point3d.y, 12, 3)); // gsc_tostring alloca memoria
	            geom += StrNum;
					geom += ",";
               StrNum.paste(gsc_tostring(tpoint3d.x, 12, 3)); // gsc_tostring alloca memoria
	            geom += StrNum;
					geom += ",";
               StrNum.paste(gsc_tostring(tpoint3d.y, 12, 3)); // gsc_tostring alloca memoria
	            geom += StrNum;
				}				
				else
				{
					geom = "(";
               StrNum.paste(gsc_tostring(point3d.x, 12, 3)); // gsc_tostring alloca memoria
	            geom += StrNum;
					geom += " ";
               StrNum.paste(gsc_tostring(point3d.y, 12, 3)); // gsc_tostring alloca memoria
	            geom += StrNum;
					geom += ", ";
               StrNum.paste(gsc_tostring(tpoint3d.x, 12, 3)); // gsc_tostring alloca memoria
	            geom += StrNum;
					geom += " ";
               StrNum.paste(gsc_tostring(tpoint3d.y, 12, 3)); // gsc_tostring alloca memoria
	            geom += StrNum;
				}
			}
		}
		else
		{
			geom = "";
			for (i = 1; i <= point_list->GetCount(); i++)
			{
				pos = point_list->FindIndex(i-1);
				point3d = point_list->GetAt(pos);

				if (geom.len() > 0)
					geom += ", ";
				else 
					geom = "(";

				if (isOracle)
				{
               StrNum.paste(gsc_tostring(point3d.x, 12, 3)); // gsc_tostring alloca memoria
	            geom += StrNum;
					geom += ",";
               StrNum.paste(gsc_tostring(point3d.y, 12, 3)); // gsc_tostring alloca memoria
	            geom += StrNum;
				}
				else
				{
               StrNum.paste(gsc_tostring(point3d.x, 12, 3)); // gsc_tostring alloca memoria
	            geom += StrNum;
					geom += " ";
               StrNum.paste(gsc_tostring(point3d.y, 12, 3)); // gsc_tostring alloca memoria
	            geom += StrNum;
				}
			}
		}
	
		if (geom.len() > 0)
			geom += ")";
	}
}

C_STRING TB_Item_Polyline_L::WriteGeomPostgres(long srid)
{  
	C_STRING str;

	str = "GeometryFromText('LINESTRING";
	str += geom;
	str += "', ";
	str += srid;
	str += ")";

	return str;
}

C_STRING TB_Item_Polyline_L::WriteGeomOracle()
{  
	C_STRING str;

	str = "SDO_GEOMETRY(2002, NULL, NULL, ";
	str += "SDO_ELEM_INFO_ARRAY(1, 2, 1), ";
	str += "SDO_ORDINATE_ARRAY";
	str += geom;
	str += ")";

	return str;
}

void TB_Item_Polyline_L::set_layer(C_STRING _s)
{  
	layer = _s;
}

void TB_Item_Polyline_L::set_l_type(C_STRING _s)
{  
	l_type = _s;
}

void TB_Item_Polyline_L::set_color(C_COLOR &_l)
{  
	n_color = _l;
}

void TB_Item_Polyline_L::set_thickness(double _d)
{  
	thickness = _d;
}

void TB_Item_Polyline_L::set_l_width(double _d)
{  
	l_width = _d;
}

void TB_Item_Polyline_L::WriteSQLPostgres(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list,
                                          CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn, long srid)
{  
	POSITION	pos;
	TB_Attribute	*tb_attribute;
	long	_fid_parent;
	C_STRING		layer1;

	if (tb_item_attribute_list->GetCount() > 0)
	{  
		pos = tb_item_attribute_list->GetHeadPosition();
		while (pos)
		{  
			tb_attribute = (TB_Attribute*) tb_item_attribute_list->GetNext(pos);
			if (str1.len() > 0)
				str1 += ", ";
			str1 += tb_attribute->get_name();
		}
	}

	geom = WriteGeomPostgres(srid);

	str2 = get_fid();
	str2 += ", ";
	str2 += geom;
	str2 += ", ";
	str2 += length;
	str2 += ", ";
	if (layer != "")
	{  
		layer1 = correctString3(layer);
		layer1.toupper();
		str2 += "'";
		str2 += layer1;
		str2 += "'";
	}
	else
		str2 += "NULL";
	str2 += ", ";
	if (l_type != "")
	{  
		str2 += "'";
		str2 += correctString3(l_type);
		str2 += "'";
	}
	else
		str2 += "NULL";
	str2 += ", ";
	if (l_width != -1)
		str2 += l_width;
	else
		str2 += "NULL";
	str2 += ", ";
	if (n_color.getColorMethod() != C_COLOR::None)
   {
      int AutoCADColorIndex;
      n_color.getAutoCADColorIndex(&AutoCADColorIndex);
		str2 += AutoCADColorIndex;
   }
	else
		str2 += "NULL";
	str2 += ", ";
	if (thickness != -1)
		str2 += thickness;
	else
		str2 += "NULL";
	get_fid_parent(&_fid_parent);
	str2 += ", ";
	if (_fid_parent > 0)
		str2 += _fid_parent;
	else
		str2 += "NULL";
}

void TB_Item_Polyline_L::WriteSQLOracle(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list, CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn)
{  
	POSITION	pos;
	TB_Attribute	*tb_attribute;	
	long	_fid_parent;
	C_STRING		layer1;
	C_STRING		str3;

	if (tb_item_attribute_list->GetCount() > 0)
	{  
		pos = tb_item_attribute_list->GetHeadPosition();
		while (pos)
		{  
			tb_attribute = (TB_Attribute*) tb_item_attribute_list->GetNext(pos);
			if (str1.len() > 0)
				str1 += ", ";
			str3 = tb_attribute->get_name();
			str3.toupper();
			str1 += "\"";
			str1 += str3;
			str1 += "\"";
		}
	}

	geom = WriteGeomOracle();

	str2 = get_fid();
	str2 += ", ";
	str2 += geom;
	str2 += ", ";
	str2 += length;
	str2 += ", ";
	if (layer != "")
	{  
		layer1 = correctString3(layer);
		layer1.toupper();
		str2 += "'";
		str2 += layer1;
		str2 += "'";
	}
	else
		str2 += "NULL";
	str2 += ", ";
	if (l_type != "")
	{  
		str2 += "'";
		str2 += correctString3(l_type);
		str2 += "'";
	}
	else
		str2 += "NULL";
	str2 += ", ";
	if (l_width != -1)
		str2 += l_width;
	else
		str2 += "NULL";
	str2 += ", ";
	if (n_color.getColorMethod() != C_COLOR::None)
   {
      int AutoCADColorIndex;
      n_color.getAutoCADColorIndex(&AutoCADColorIndex);
		str2 += AutoCADColorIndex;
   }
	else
		str2 += "NULL";
	str2 += ", ";
	if (thickness != -1)
		str2 += thickness;
	else
		str2 += "NULL";
	get_fid_parent(&_fid_parent);
	str2 += ", ";
	if (_fid_parent > 0)
		str2 += _fid_parent;
	else
		str2 += "NULL";
}

void TB_Item_Polyline_L::get_item_attribute_list(CObList* tb_item_attribute_list)
{  
	TB_Attribute	*tb_attribute;

	tb_attribute = new TB_Attribute("fid", "int4", "", 10, 0, false, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("length", "float8", "", 20, 8, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("layer", "varchar", "", 255, 0, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("l_type", "varchar", "", 255, 0, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("l_width", "float8", "", 20, 8, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("n_color", "int4", "", 10, 0, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("thickness", "float8", "", 20, 8, true, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("fid_parent", "int4", "", 10, 0, false, "");
	tb_item_attribute_list->AddTail(tb_attribute);
}

void TB_Item_Polyline_L::get_fid_parent(long *_fid_parent)
{  
	*_fid_parent = fid_parent;
}

TB_Item_Sec::TB_Item_Sec(long _fid, long _fid_parent) : TB_Item(_fid)
{
	fid_parent = _fid_parent;
}

int TB_Item_Sec::WriteSQLPostgres(FILE *file, C_STRING &_prjname, C_STRING &_name, 
                                  CObList* tb_item_attribute_list, C_DBCONNECTION *pConn) 
{
	int ret = GS_GOOD;
	C_STRING		str, str1, str2;
	POSITION	pos;
	TB_Attribute	*tb_attribute;

	TRY
	{
		str = "INSERT INTO ";
		str += _prjname;
		//str += "@SCHEMA@";
		str += ".";
		str += correctString4(_name);
		str += "(\"";
		if (tb_item_attribute_list->GetCount() > 0)
		{  
			pos = tb_item_attribute_list->GetHeadPosition();
			while (pos)
			{  
				tb_attribute = (TB_Attribute*) tb_item_attribute_list->GetNext(pos);
				if (str1.len() > 0)
					str1 += ", \"";
				
				str2 = tb_attribute->get_name();
				// le " " vengono introdotte a prescindere dalla presenza 
				// di caratteri speciali nella stringa
				str2 = correctString1(str2);
				str2.tolower();

				str1 += str2;
				str1 += "\"";
			}
		}
		str += str1;
		str += ") VALUES (";
		if (fwprintf(file, _T("%s"), str) < 0)
		{
			if (gsc_fclose(file) == GS_BAD)
				AfxThrowUserException();
			AfxThrowUserException();
		}

      C_STRING dummy(_T(""));
		str = tb_attrib_list.writeSQL(dummy, tb_item_attribute_list, pConn);
		str += ");";
		if (fwprintf(file, _T("%s\n"), str) < 0)
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

int TB_Item_Sec::WriteSQLOracle(FILE *file, C_STRING &_prjname, C_STRING &_name, 
                                CObList* tb_item_attribute_list, C_DBCONNECTION *pConn) 
{
	int ret = GS_GOOD;
	C_STRING		str, str1, str2, str3;
	POSITION	pos;
	TB_Attribute	*tb_attribute;

   TRY
	{
		str = "INSERT INTO \"";
		str3 = _prjname;
		str3.toupper();
		str += str3;
		str += "\".\"";
		str3 = correctString4(_name);
		str3.toupper();
		str += str3;
		str += "\" (";
		if (tb_item_attribute_list->GetCount() > 0)
		{  
			pos = tb_item_attribute_list->GetHeadPosition();
			while (pos)
			{  
				tb_attribute = (TB_Attribute*) tb_item_attribute_list->GetNext(pos);
				if (str1.len() > 0)
					str1 += ", ";
				
				str2 = tb_attribute->get_name();
				// le " " vengono introdotte a prescindere dalla presenza 
				// di caratteri speciali nella stringa
				str2 = correctString1(str2);
				str2.toupper();
				str1 += "\"";
				str1 += str2;
				str1 += "\"";
			}
		}
		str += str1;
		str += ") VALUES (";
		if (fwprintf(file, _T("%s"), str) < 0)
		{
			if (gsc_fclose(file) == GS_BAD)
				AfxThrowUserException();
			AfxThrowUserException();
		}

      C_STRING dummy(_T(""));
		str = tb_attrib_list.writeSQL(dummy, tb_item_attribute_list, pConn);
		str += ");";
		if (fwprintf(file, _T("%s\n"), str) < 0)
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

void TB_Item_Sec::WriteSQLPostgres(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list, 
                                   CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn, long srid)
{  
	POSITION	pos;
	TB_Attribute	*tb_attribute;

	if (tb_item_attribute_list->GetCount() > 0)
	{  
		pos = tb_item_attribute_list->GetHeadPosition();
		while (pos)
		{  
			tb_attribute = (TB_Attribute*) tb_item_attribute_list->GetNext(pos);
			if (str1.len() > 0)
				str1 += ", ";
			str1 += tb_attribute->get_name();
		}
	}

	str1 += tb_attrib_list.writeSQL(false, str1, reserved_list);

	str2 = get_fid();
	str2 += ", ";
	str2 += fid_parent;
	str2 += tb_attrib_list.writeSQL(str2, tb_attribute_list, pConn);
}

void TB_Item_Sec::WriteSQLOracle(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list,
                                 CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn)
{  
	POSITION	pos;
	TB_Attribute	*tb_attribute;
	C_STRING	str3;

	if (tb_item_attribute_list->GetCount() > 0)
	{  
		pos = tb_item_attribute_list->GetHeadPosition();
		while (pos)
		{  
			tb_attribute = (TB_Attribute*) tb_item_attribute_list->GetNext(pos);
			if (str1.len() > 0)
				str1 += ", ";
			str3 = tb_attribute->get_name();
			str3.toupper();
			str1 += "\"";
			str1 += str3;
			str1 += "\"";
		}
	}

	str1 += tb_attrib_list.writeSQL(true, str1, reserved_list);

	str2 = get_fid();
	str2 += ", ";
	str2 += fid_parent;
	str2 += tb_attrib_list.writeSQL(str2, tb_attribute_list, pConn);
}

void TB_Item_Sec::get_item_attribute_list(CObList* tb_item_attribute_list)
{  
	TB_Attribute	*tb_attribute;

	tb_attribute = new TB_Attribute("fid", "int4", "", 10, 0, false, "");
	tb_item_attribute_list->AddTail(tb_attribute);
	tb_attribute = new TB_Attribute("fid_parent", "int4", "", 10, 0, false, "");
	tb_item_attribute_list->AddTail(tb_attribute);
}


Geoweb_Catalog::Geoweb_Catalog(C_STRING _project_name, C_STRING _name, C_STRING _database)
{
	project_name = _project_name;
	name = _name;
	//path = "/geoweb_dati/";
	//path = "/geoweb_";  // gisclient_21 non vuole lo slash iniziale
	if (_database == "")
	{
		path = "geoweb_";
		path += _project_name;
		path += "/";
		path += _name;
	}
	else
	{
		path = _database;
		path += "/";
		path += _name;
	}
}


Geoweb_Style::Geoweb_Style(C_STRING _name, C_CLASS *pCls, bool load)
{
	C_DWG_LIST	dwg_list;
	C_DWG		*pDwg;

	C_RB_LIST Descr;
	presbuf   p;

	Adesk::UInt16	cl;
	unsigned long	RGB;

	style_name = correctString1(_name);
	style_name += "_S0";

	if (pCls->ptr_fas())
	{
		if (pCls->ptr_fas()->color.getColorMethod() == C_COLOR::ByLayer)
		{
			colorR = 0;
			colorG = 0;
			colorB = 0;
			outlinecolorR = 0;
			outlinecolorG = 0;
			outlinecolorB = 0;

			if (dwg_list.load(pCls->ptr_id()->pPrj->get_key(), pCls->ptr_id()->code) == GS_GOOD)
			{
				pDwg = (C_DWG*) dwg_list.get_head();
				while (pDwg)
				{
					if (load)
						gsc_LoadThmFromDwg(pDwg->get_name());
								
					// leggo la sua descrizione
					Descr << acdbTblSearch(_T("LAYER"), pCls->ptr_fas()->layer, 0);
					p = Descr.SearchType(62);
					if (p)
					{
						cl = p->resval.rint;
						RGB = acedGetRGB(cl);
						colorR = GetRValue(RGB);
						colorG = GetGValue(RGB);
						colorB = GetBValue(RGB);
						outlinecolorR = GetRValue(RGB);
						outlinecolorG = GetGValue(RGB);
						outlinecolorB = GetBValue(RGB);
					}

					pDwg = (C_DWG*) dwg_list.get_next();
				}
			}
		}
		else
		{
			pCls->ptr_fas()->color.getRed(&colorR);
			pCls->ptr_fas()->color.getGreen(&colorG);
			pCls->ptr_fas()->color.getBlue(&colorB);
			outlinecolorR = colorR;
			outlinecolorG = colorG;
			outlinecolorB = colorB;
		}
	}
}


Geoweb_Class::Geoweb_Class(C_STRING _name, C_STRING _ext, bool _legendtype_id, C_STRING _label_font, C_STRING _label_angle, bool is_tbl, C_CLASS *pCls, Geosim_Layer_List *_geosim_layer_list, CStringList *_geoweb_symbol_ttf_list, bool style, bool load)
{
	C_DWG_LIST	dwg_list;
	C_DWG		*pDwg;

	Adesk::UInt16	cl;
	unsigned long RGB;

	C_RB_LIST Descr;
	presbuf   p;

	C_STRING temp, _name1;
	int	   pos1;

	C_CLASS *pClass;

	temp = pCls->get_name();
	if (pCls->ptr_id()->sub_code != 0)
	{
		pClass = gsc_find_class((pCls->ptr_id()->pPrj)->get_key(), pCls->ptr_id()->code, 0);
		temp = pClass->get_name();
	}
	pos1 = temp.find(_T("_"));
	if (pos1 > -1)
		theme_name = temp.left(pos1);
	else
		theme_name = "";
	
	_name1 = correctString1(_name);
	layergroup_name = _name1;
	temp = _name1;
	temp += _ext;
	layer_name = temp;
	class_name = temp;
	class_name += "_CLS";
	original_class_name = pCls->get_name();

	//expression = "";
	legendtype_id = _legendtype_id;

	geoweb_style = NULL;
	geosim_layer_list = _geosim_layer_list;

	if (pCls->ptr_fas())
	{
		if (!style)
		{
			if (pCls->ptr_fas()->color.getColorMethod() == C_COLOR::ByLayer)
			{
				label_colorR = 0;
				label_colorG = 0;
				label_colorB = 0;

				if (dwg_list.load(pCls->ptr_id()->pPrj->get_key(), pCls->ptr_id()->code) == GS_GOOD)
				{
					pDwg = (C_DWG*) dwg_list.get_head();
					while (pDwg)
					{
						if (load)
							gsc_LoadThmFromDwg(pDwg->get_name());
									
						// leggo la sua descrizione
						Descr << acdbTblSearch(_T("LAYER"), pCls->ptr_fas()->layer, 0);
						p = Descr.SearchType(62);
						if (p)
						{
							cl = p->resval.rint;
							RGB = acedGetRGB(cl);
							label_colorR = GetRValue(RGB);
							label_colorG = GetGValue(RGB);
							label_colorB = GetBValue(RGB);
							//label_outlinecolorR;
							//label_outlinecolorG;
							//label_outlinecolorB;
							//label_bgcolorR;
							//label_bgcolorG;
							//label_bgcolorB;
						}

						pDwg = (C_DWG*) dwg_list.get_next();
					}
				}
			}
			else
			{
			   pCls->ptr_fas()->color.getRed(&label_colorR);
			   pCls->ptr_fas()->color.getGreen(&label_colorG);
			   pCls->ptr_fas()->color.getBlue(&label_colorB);

				//label_outlinecolorR;
				//label_outlinecolorG;
				//label_outlinecolorB;
				//label_bgcolorR;
				//label_bgcolorG;
				//label_bgcolorB;
			}
		
			label_block = pCls->ptr_fas()->block;

			if (_geoweb_symbol_ttf_list)
			{
				if (_geoweb_symbol_ttf_list->Find(label_block.get_name()))
					label_font = "geoweb";
				else
					label_font = correctString1(_label_font);
			}
			else
				label_font = correctString1(_label_font);

			label_angle = _label_angle;
			
			if (is_tbl)
				label_position = "UR";
			else
				label_position = "CC";
		}
		else
			geoweb_style = new Geoweb_Style(class_name, pCls, load);
	}
	else
	{
		if (!style)
		{
			label_font = correctString1(_label_font);
			label_angle = _label_angle;
			label_position = "UR";
		}
	}
}


Geoweb_Layer::Geoweb_Layer(C_STRING _layergroup_name, C_STRING _layer_name, int _layertype_id, C_STRING _project_name, C_STRING _catalog_name, C_STRING _data, long _srid, C_STRING _ext, bool _view)
{
    layergroup_name = _layergroup_name;
	layer_name = correctString1(_layer_name);
	layertype_id = _layertype_id;
	project_name = _project_name;
	catalog_name = correctString1(_catalog_name);
	data = correctString1(_data);
	if (_view)
		data += "_v";
	else
		data += _ext;
	data_geom = "wkb_geometry";
	data_unique = "fid";
	data_srid = _srid;
	//symbolscale = 500;
	layer_order = 200;
}

void Geoweb_Layer::Update(C_STRING _data_filter, C_STRING _labelitem)
{
	data_filter = _data_filter;
	labelitem = _labelitem;
}

void Geoweb_Layer::Update(C_STRING _layer_def)
{
	layer_def = _layer_def;
}


Geoweb_Layergroup::Geoweb_Layergroup(C_STRING _theme_name, C_STRING _project_name, C_STRING _layergroup_name, C_STRING _layergroup_title)
{
	theme_name = _theme_name;
	project_name = _project_name;
	layergroup_name = correctString1(_layergroup_name);
	layergroup_title = _layergroup_title;
}


Geoweb_Project::Geoweb_Project(C_STRING _project_name, C_STRING _base_path, long _srid)
{
	project_name = _project_name;
	base_path = _base_path;
	project_srid = _srid;
}


Geoweb_Theme::Geoweb_Theme(C_STRING _project_name, C_STRING _theme_name, C_STRING _theme_title)
{
	project_name = _project_name;
	theme_name = _theme_name;
	theme_title = _theme_title;
}


Geoweb_Qtrelation::Geoweb_Qtrelation(C_STRING _data_field, C_STRING _catalog_name, C_STRING _table_name, C_STRING _table_field, int _qtrelationtype_id)
{
	data_field = _data_field;
	catalog_name = correctString1(_catalog_name);
	table_name = correctString1(_table_name);
	table_field = _table_field;
	qtrelationtype_id = _qtrelationtype_id;
}


Geoweb_Qtfield::Geoweb_Qtfield(C_STRING _qtfield_name)
{
	qtfield_name = _qtfield_name;
}


Geoweb_Qt::Geoweb_Qt(C_STRING _name, C_STRING _ext, C_STRING _qt_name, C_CLASS *pCls)
{
	C_ATTRIB_LIST	*pAttribList;
	C_ATTRIB		*pAttrib;

	Geoweb_Qtfield	*geoweb_qtfield;

	C_STRING	temp, temp1, _name1;
	int pos1;

	pos1 = _qt_name.find(_T("_"));
	if (pos1 > -1)
		theme_name = _qt_name.left(pos1);
	else
		theme_name = "";

	_name1 = correctString1(_name);
	layergroup_name = _name1;	
	temp = _name1;
	temp += _ext;
	layer_name = temp;
	qt_name = _qt_name;
	geoweb_qtrelation = NULL;

	if (pCls->ptr_attrib_list())
	{
		pAttribList	= pCls->ptr_attrib_list();

		pAttrib = (C_ATTRIB *) pAttribList->get_head();
		while (pAttrib)
		{
			geoweb_qtfield = new Geoweb_Qtfield(pAttrib->get_name());
			geoweb_qtfield_list.AddTail(geoweb_qtfield);
		
			pAttrib = (C_ATTRIB *) pAttrib->get_next();
		}
	}
}

Geoweb_Qt::Geoweb_Qt(C_STRING _name1, C_STRING _name2, C_STRING _ext, C_STRING _qt_name, C_CLASS *pCls)
{
	C_ATTRIB_LIST	*pAttribList;
	C_ATTRIB		*pAttrib;

	Geoweb_Qtfield	*geoweb_qtfield;

	C_STRING	temp, _name3;
	int	pos1;

	pos1 = _qt_name.find(_T("_"));
	if (pos1 > -1)
		theme_name = _qt_name.left(pos1);
	else
		theme_name = "";

	_name3 = correctString1(_name2);
	layergroup_name = _name3;
	temp = _name3;
	temp += _ext;
	layer_name = temp;
	qt_name = _qt_name;
	geoweb_qtrelation = new Geoweb_Qtrelation("fid_parent", _name1, _qt_name, "fid", 1);

	if (pCls->ptr_attrib_list())
	{
		pAttribList	= pCls->ptr_attrib_list();

		pAttrib = (C_ATTRIB *) pAttribList->get_head();
		while (pAttrib)
		{
			geoweb_qtfield = new Geoweb_Qtfield(pAttrib->get_name());
			geoweb_qtfield_list.AddTail(geoweb_qtfield);
		
			pAttrib = (C_ATTRIB *) pAttrib->get_next();
		}
	}
}

Geosim_Layer::Geosim_Layer(C_STRING _layer_name, BYTE _R, BYTE _G, BYTE _B)
{
	layer_name = correctString1(_layer_name);
	colorR = _R;
	colorG = _G;
	colorB = _B;
}

Geosim_Class::Geosim_Class(long _id, C_STRING _name, C_STRING _caption, C_STRING _type, long _last, long _tid, long _pid)
{
	f_class_id = _id;
	f_class_name = _name;
	caption = _caption;
	f_class_type = _type;
	last = _last;
	topic_id = _tid;
	parent_f_class_id = _pid;
}

Geoweb_Structure::Geoweb_Structure()
{
	geoweb_symbol_ttf_list = new CStringList();

	geoweb_symbol_ttf_list->AddTail("BT");
	geoweb_symbol_ttf_list->AddTail("CA");
	geoweb_symbol_ttf_list->AddTail("CM");
	geoweb_symbol_ttf_list->AddTail("CO");
	geoweb_symbol_ttf_list->AddTail("EC");
	geoweb_symbol_ttf_list->AddTail("FLANGIA");
	geoweb_symbol_ttf_list->AddTail("FR");
	geoweb_symbol_ttf_list->AddTail("GD");
	geoweb_symbol_ttf_list->AddTail("GEOSIM");
	geoweb_symbol_ttf_list->AddTail("GI");
	geoweb_symbol_ttf_list->AddTail("IDRANTE");
	geoweb_symbol_ttf_list->AddTail("MA");
	geoweb_symbol_ttf_list->AddTail("MO");
	geoweb_symbol_ttf_list->AddTail("NO");
	geoweb_symbol_ttf_list->AddTail("NODO");
	geoweb_symbol_ttf_list->AddTail("P1");
	geoweb_symbol_ttf_list->AddTail("P2");
	geoweb_symbol_ttf_list->AddTail("P3");
	geoweb_symbol_ttf_list->AddTail("P4");
	geoweb_symbol_ttf_list->AddTail("P5");
	geoweb_symbol_ttf_list->AddTail("PE");
	geoweb_symbol_ttf_list->AddTail("PF");
	geoweb_symbol_ttf_list->AddTail("AF");
	geoweb_symbol_ttf_list->AddTail("PM");
	geoweb_symbol_ttf_list->AddTail("PO");
	geoweb_symbol_ttf_list->AddTail("POMPA");
	geoweb_symbol_ttf_list->AddTail("PR");
	geoweb_symbol_ttf_list->AddTail("PT");
	geoweb_symbol_ttf_list->AddTail("PZ");
	geoweb_symbol_ttf_list->AddTail("RA");
	geoweb_symbol_ttf_list->AddTail("REGOLATORE");
	geoweb_symbol_ttf_list->AddTail("RG");
	geoweb_symbol_ttf_list->AddTail("RI");
	geoweb_symbol_ttf_list->AddTail("RT");
	geoweb_symbol_ttf_list->AddTail("RU");
	geoweb_symbol_ttf_list->AddTail("S2");
	geoweb_symbol_ttf_list->AddTail("S3");
	geoweb_symbol_ttf_list->AddTail("SE");
	geoweb_symbol_ttf_list->AddTail("SERBATOIO");
	geoweb_symbol_ttf_list->AddTail("SO");
	geoweb_symbol_ttf_list->AddTail("V1");
	geoweb_symbol_ttf_list->AddTail("V2");
	geoweb_symbol_ttf_list->AddTail("V3");
	geoweb_symbol_ttf_list->AddTail("V4");
	geoweb_symbol_ttf_list->AddTail("V5");
	geoweb_symbol_ttf_list->AddTail("VALVE");
}

int Geoweb_Structure::Update(int prj, C_STRING prj_name, C_STRING prj_path, C_STRING database, C_STRING font, C_INT_INT_STR_LIST &classList, long srid)
{
	int ret = GS_GOOD;

	TRY
	{
		prj_name = correctString1(prj_name);

		geoweb_project = new Geoweb_Project(prj_name, prj_path, srid);
		
		geoweb_catalog = new Geoweb_Catalog(prj_name, prj_name, database);

		UpdateTheme(prj_name, classList);
		
		UpdateLayer(prj, prj_name, prj_name, classList, srid);

		UpdateQt(prj, prj_name, classList);

		UpdateClass(prj, prj_name, font, classList);
        
		////////// elenco secondarie
		//max_id = 0;	

		//pos = geosim_class_list.GetHeadPosition();
		//while (pos)
		//{
		//	geosim_class = (Geosim_Class*) geosim_class_list.GetNext(pos);			
		//	id = geosim_class->f_class_id;
		//	if (id > max_id)
		//		max_id = id;
		//}

		////////sec_list.remove_all();
		////////pClass->rtab_sec(sec_list);
		////////if (sec_list.get_count() > 0)
		////////{
		////////	sec = (C_INT_INT_STR *) sec_list.get_head();
		////////	while (sec)
		////////	{
		////////		if (sec->get_type() == SEC_GEOSIM)
		////////		{
		////////			if ((pSec = (C_SECONDARY*) pClass->find_sec(sec->get_key())) == NULL)
		////////			{
		////////				sec = (C_INT_INT_STR *) sec_list.get_next();
		////////				continue;
		////////			}

		////////			max_id++;
		////////			name = "s";
		////////			if ((tb_dictionary->f_class_type == "P") && (tb_dictionary->f_class_id == -1))
		////////			{
		////////				name += pClass->ptr_id()->code;
		////////				name += "_";
		////////				name += pClass->get_key();
		////////			}
		////////			else
		////////			{
		////////				if ((tb_dictionary->f_class_type == "L") && (tb_dictionary->f_class_id == -1))
		////////				{
		////////					name += pClass->ptr_id()->code;
		////////					name += "_";
		////////					name += (pClass->get_key();
		////////				}
		////////				else
		////////					name += id;
		////////			}
		////////			name += "_";
		////////			name += pSec->get_name();
		////////			tb_dictionary1 = new Geoweb_Structure(max_id, name, pSec->get_name(), "T", 0, id);
		////////			tb_dictionary_list1.AddTail(tb_dictionary1);			
		////////		}
		////////		sec = (C_INT_INT_STR *) sec_list.get_next();
		////////	}
		////////}
	}
	CATCH( CUserException, e )
	{
		ret=GS_BAD;
	}
	END_CATCH

	return ret;
}

int Geoweb_Structure::UpdateLayer(int prj, C_STRING prj_name, C_STRING catalog_name, C_INT_INT_STR_LIST &classList, long srid)
{
	int ret = GS_GOOD;

	int		sub;
	long		id, parent_id;
	POSITION	pos;
	C_STRING	name, _name, str1, str2, theme_name;
	int		pos1;

	C_CLASS		*pClass, *pSub;

	Geosim_Class	*geosim_class;
	Geoweb_Layer	*geoweb_layer;
	Geoweb_Layergroup	*geoweb_layergroup;

	C_INT_INT_STR	*pItem;

	TRY
	{
		pos = geosim_class_list.GetHeadPosition();
		while (pos)
		{
			geosim_class = (Geosim_Class*) geosim_class_list.GetNext(pos);			
			id = geosim_class->f_class_id;

			pItem = (C_INT_INT_STR*) classList.get_head();
			if (pItem)
			{	
				pItem = (C_INT_INT_STR*) classList.get_next();
				while (pItem)
				{	
					if (pItem->get_key() == id)
					{
						name = geosim_class->f_class_name;
						pos1 = name.find(_T("_"));
						if (pos1 > -1)
							theme_name = name.left(pos1);
						else
							theme_name = "";
						_name = correctString3(name);
						parent_id = geosim_class->parent_f_class_id;

						pClass = gsc_find_class(prj, id, 0);

						if ((geosim_class->f_class_type == "P") && (id != -1))
						{
							str1 = _name;
							str1 = str1.toupper();
							str2 = _name;
							geoweb_layergroup = new Geoweb_Layergroup(theme_name, prj_name, str1, str2);
							geoweb_layergroup_list.AddTail(geoweb_layergroup);

							str1 = _name;
							str1 = str1.toupper();
							str2 = _name;
							str2 = str2.tolower();
							geoweb_layer = new Geoweb_Layer(geoweb_layergroup->layergroup_name, str1, 1, prj_name, catalog_name, str2, srid);
							//geoweb_layer->Update("POSTLABELCACHE TRUE");
							geoweb_layer_list.AddTail(geoweb_layer);

							str1 = _name;
							str1 += "_tbl";
							str1 = str1.toupper();
							str2 = _name;
							str2 = str2.tolower();
							geoweb_layer = new Geoweb_Layer(geoweb_layergroup->layergroup_name, str1, 5, prj_name, catalog_name, str2, srid, "_tbl_v");
							geoweb_layer->Update("hidden=0", "label_text");
							//geoweb_layer->Update("POSTLABELCACHE TRUE");
							geoweb_layer_list.AddTail(geoweb_layer);
						}
						else if (geosim_class->f_class_type == "T")
						{
							str1 = _name;
							str1 = str1.toupper();
							str2 = _name;
							geoweb_layergroup = new Geoweb_Layergroup(theme_name, prj_name, str1, str2);
							geoweb_layergroup_list.AddTail(geoweb_layergroup);

							str1 = _name;
							str1 += "_tbl";
							str1 = str1.toupper();
							str2 = _name;
							str2 = str2.tolower();
							geoweb_layer = new Geoweb_Layer(geoweb_layergroup->layergroup_name, str1, 5, prj_name, catalog_name, str2, srid, "_tbl", true);
							geoweb_layer->Update("hidden=0", "label_text");
							//geoweb_layer->Update("POSTLABELCACHE TRUE");
							geoweb_layer_list.AddTail(geoweb_layer);
						}
						else if (geosim_class->f_class_type == "O")
						{
							str1 = _name;
							str1 = str1.toupper();
							str2 = _name;
							geoweb_layergroup = new Geoweb_Layergroup(theme_name, prj_name, str1, str2);
							geoweb_layergroup_list.AddTail(geoweb_layergroup);

							str1 = _name;
							str1 = str1.toupper();
							str2 = _name;
							str2 = str2.tolower();
							geoweb_layer = new Geoweb_Layer(geoweb_layergroup->layergroup_name, str1, 3, prj_name, catalog_name, str2, srid);
							geoweb_layer_list.AddTail(geoweb_layer);

							str1 = _name;
							str1 += "_tbl";
							str1 = str1.toupper();
							str2 = _name;
							str2 = str2.tolower();
							geoweb_layer = new Geoweb_Layer(geoweb_layergroup->layergroup_name, str1, 5, prj_name, catalog_name, str2, srid, "_tbl_v");
							geoweb_layer->Update("hidden=0", "label_text");
							//geoweb_layer->Update("POSTLABELCACHE TRUE");
							geoweb_layer_list.AddTail(geoweb_layer);
						}
						else if ((geosim_class->f_class_type == "L") && (id != -1))
						{
							str1 = _name;
							str1 = str1.toupper();
							str2 = _name;
							geoweb_layergroup = new Geoweb_Layergroup(theme_name, prj_name, str1, str2);
							geoweb_layergroup_list.AddTail(geoweb_layergroup);

							str1 = _name;
							str1 += "_l";
							str1 = str1.toupper();
							str2 = _name;
							str2 = str2.tolower();
							geoweb_layer = new Geoweb_Layer(geoweb_layergroup->layergroup_name, str1, 2, prj_name, catalog_name, str2, srid, "_l", true);
							geoweb_layer_list.AddTail(geoweb_layer);

							str1 = _name;
							str1 += "_l_tbl";
							str1 = str1.toupper();
							str2 = _name;
							str2 = str2.tolower();
							geoweb_layer = new Geoweb_Layer(geoweb_layergroup->layergroup_name, str1, 5, prj_name, catalog_name, str2, srid, "_l_tbl_v");
							geoweb_layer->Update("hidden=0", "label_text");
							//geoweb_layer->Update("POSTLABELCACHE TRUE");
							geoweb_layer_list.AddTail(geoweb_layer);
						}
						else if (geosim_class->f_class_type == "S")
						{
							str1 = _name;
							str1 = str1.toupper();
							str2 = _name;
							geoweb_layergroup = new Geoweb_Layergroup(theme_name, prj_name, str1, str2);
							geoweb_layergroup_list.AddTail(geoweb_layergroup);

							str1 = _name;
							str1 += "_l";
							str1 = str1.toupper();
							str2 = _name;
							str2 = str2.tolower();
							geoweb_layer = new Geoweb_Layer(geoweb_layergroup->layergroup_name, str1, 2, prj_name, catalog_name, str2, srid, "_l");
							geoweb_layer_list.AddTail(geoweb_layer);

							str1 = _name;
							str1 += "_p";
							str1 = str1.toupper();
							str2 = _name;
							str2 = str2.tolower();
							geoweb_layer = new Geoweb_Layer(geoweb_layergroup->layergroup_name, str1, 1, prj_name, catalog_name, str2, srid, "_p"); 
							//geoweb_layer->Update("POSTLABELCACHE TRUE");
							geoweb_layer_list.AddTail(geoweb_layer);

							str1 = _name;
							str1 += "_plg";
							str1 = str1.toupper();
							str2 = _name;
							str2 = str2.tolower();
							geoweb_layer = new Geoweb_Layer(geoweb_layergroup->layergroup_name, str1, 3, prj_name, catalog_name, str2, srid, "_plg"); 
							geoweb_layer_list.AddTail(geoweb_layer);

							str1 = _name;
							str1 += "_t_tbl";
							str1 = str1.toupper();
							str2 = _name;
							str2 = str2.tolower();
							geoweb_layer = new Geoweb_Layer(geoweb_layergroup->layergroup_name, str1, 5, prj_name, catalog_name, str2, srid, "_t_tbl"); 
							geoweb_layer->Update("hidden=0", "label_text");
							//geoweb_layer->Update("POSTLABELCACHE TRUE");
							geoweb_layer_list.AddTail(geoweb_layer);														
						}
						else if ((geosim_class->f_class_type == "P") && (id == -1))
						{
							pClass = gsc_find_class(prj, parent_id, 0);
							name = pClass->get_name();
							pos1 = name.find(_T("_"));
							if (pos1 > -1)
								theme_name = name.left(pos1);
							else
								theme_name = "";
							sub = pClass->ptr_sub_list()->getpos_name(geosim_class->f_class_name.get_name());
							pSub = (C_SUB*) pClass->ptr_sub_list()->getptr_at(sub);

							str1 = "s";
							str1 += pSub->ptr_id()->code;
							str1 += "_";
							str1 += _name;
							str1.toupper();
							str2 = pClass->ptr_id()->name;
							str2 += "-";
							str2 += _name;
							geoweb_layergroup = new Geoweb_Layergroup(theme_name, prj_name, str1, str2);
							geoweb_layergroup_list.AddTail(geoweb_layergroup);

							str1 = "s";
							str1 += pSub->ptr_id()->code;
							str1 += "_";
							str1 += _name;
							str1.toupper();
							str2 = "s";
							str2 += pSub->ptr_id()->code;
							str2 += "_";
							str2 += _name;
							str2 = str2.tolower();
							geoweb_layer = new Geoweb_Layer(geoweb_layergroup->layergroup_name, str1, 1, prj_name, catalog_name, str2, srid);
							//geoweb_layer->Update("POSTLABELCACHE TRUE");
							geoweb_layer_list.AddTail(geoweb_layer);

							str1 = "s";
							str1 += pSub->ptr_id()->code;
							str1 += "_";
							str1 += _name;
							str1 += "_tbl";
							str1.toupper();
							str2 = "s";
							str2 += pSub->ptr_id()->code;
							str2 += "_";
							str2 += _name;
							str2 = str2.tolower();
							geoweb_layer = new Geoweb_Layer(geoweb_layergroup->layergroup_name, str1, 5, prj_name, catalog_name, str2, srid, "_tbl_v");
							geoweb_layer->Update("hidden=0", "label_text");
							//geoweb_layer->Update("POSTLABELCACHE TRUE");
							geoweb_layer_list.AddTail(geoweb_layer);
						}
						else if ((geosim_class->f_class_type == "L") && (id == -1))
						{
							pClass = gsc_find_class(prj, parent_id, 0);
							name = pClass->get_name();
							pos1 = name.find(_T("_"));
							if (pos1 > -1)
								theme_name = name.left(pos1);
							else
								theme_name = "";

							sub = pClass->ptr_sub_list()->getpos_name(geosim_class->f_class_name.get_name());
							pSub = (C_SUB*) pClass->ptr_sub_list()->getptr_at(sub);

							str1 = "s";
							str1 += pSub->ptr_id()->code;
							str1 += "_";
							str1 += _name;
							str1.toupper();
							str2 = pClass->ptr_id()->name;
							str2 += "-";
							str2 += _name;
							geoweb_layergroup = new Geoweb_Layergroup(theme_name, prj_name, str1, str2);
							geoweb_layergroup_list.AddTail(geoweb_layergroup);

							str1 = "s";
							str1 += pSub->ptr_id()->code;
							str1 += "_";
							str1 += _name;
							str1 += "_l";
							str1.toupper();
							str2 = "s";
							str2 += pSub->ptr_id()->code;
							str2 += "_";
							str2 += _name;
							str2 = str2.tolower();
							geoweb_layer = new Geoweb_Layer(geoweb_layergroup->layergroup_name, str1, 2, prj_name, catalog_name, str2, srid, "_l", true);
							geoweb_layer_list.AddTail(geoweb_layer);

							str1 = "s";
							str1 += pSub->ptr_id()->code;
							str1 += "_";
							str1 += _name;
							str1 += "_l_tbl";
							str1.toupper();
							str2 = "s";
							str2 += pSub->ptr_id()->code;
							str2 += "_";
							str2 += _name;
							str2 = str2.tolower();
							geoweb_layer = new Geoweb_Layer(geoweb_layergroup->layergroup_name, str1, 5, prj_name, catalog_name, str2, srid, "_l_tbl_v");
							geoweb_layer->Update("hidden=0", "label_text");
							//geoweb_layer->Update("POSTLABELCACHE TRUE");
							geoweb_layer_list.AddTail(geoweb_layer);
						}
						break;
					}
					pItem = (C_INT_INT_STR*) classList.get_next();
				}
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

int Geoweb_Structure::UpdateTheme(C_STRING _project_name, C_INT_INT_STR_LIST &classList)
{
	int ret = GS_GOOD;

	POSITION	pos, pos2;
	bool	found;
	
	long	id;
	C_STRING	name, name1, str1, str2;
	int		pos1;

	Geosim_Class	*geosim_class;
	Geoweb_Theme	*geoweb_theme;

	C_INT_INT_STR	*pItem;

	TRY
	{
		pos = geosim_class_list.GetHeadPosition();
		while (pos)
		{
			geosim_class = (Geosim_Class*) geosim_class_list.GetNext(pos);			
			id = geosim_class->f_class_id;

			pItem = (C_INT_INT_STR*) classList.get_head();
			if (pItem)
			{	
				pItem = (C_INT_INT_STR*) classList.get_next();
				while (pItem)
				{	
					if (pItem->get_key() == id)
					{	
						name = geosim_class->f_class_name;
						pos1 = name.find(_T("_"));

						if (pos1 > -1)
							name1 = name.left(pos1);
						else
							name1 = "";

						found = false;
						if (geoweb_theme_list.GetCount() > 0)
						{
							pos2 = geoweb_theme_list.GetHeadPosition();
							while (pos2)
							{
								geoweb_theme = (Geoweb_Theme*) geoweb_theme_list.GetNext(pos2);

								str1 = geoweb_theme->theme_name;
								str1 = str1.toupper();
								str2 = name1;
								str2 = str2.toupper();
								if (str1 == str2)
									found = true;
							}
						}

						if (found == false)
						{
							geoweb_theme = new Geoweb_Theme(_project_name, name1, name1);
							geoweb_theme_list.AddTail(geoweb_theme);
						}
						break;
					}
					pItem = (C_INT_INT_STR*) classList.get_next();
				}
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

int Geoweb_Structure::UpdateClass(int prj, C_STRING prj_name, C_STRING font, C_INT_INT_STR_LIST &classList)
{
	int ret = GS_GOOD;

	POSITION	pos;
	
	long		id, parent_id;
	int			sub;

	C_STRING		name, _name, str1, _font;

	Geosim_Class	*geosim_class;
	Geoweb_Class	*geoweb_class;

	C_CLASS			*pClass;
	C_SUB			*pSub;

	C_INT_INT_STR	*pItem;

	TRY
	{
		if (font == "")
			_font = prj_name;
		else
			_font = font;

		pos = geosim_class_list.GetHeadPosition();
		while (pos)
		{
			geosim_class = (Geosim_Class*) geosim_class_list.GetNext(pos);			

			id = geosim_class->f_class_id;
			pItem = (C_INT_INT_STR*) classList.get_head();
			if (pItem)
			{	
				pItem = (C_INT_INT_STR*) classList.get_next();
				while (pItem)
				{	
					if (pItem->get_key() == id)
					{	
						name = geosim_class->f_class_name;
						_name = correctString3(name);
						parent_id = geosim_class->parent_f_class_id;

						pClass = gsc_find_class(prj, id, 0);

						if ((geosim_class->f_class_type == "P") && (id != -1))
						{
							str1 = _name;
							str1 = str1.toupper();

							geoweb_class = new Geoweb_Class(str1, "", true, _font, "[orientation_degree]", false, pClass, &geosim_class->geosim_layer_list, geoweb_symbol_ttf_list, false, true);
							geoweb_class_list.AddTail(geoweb_class);

							geoweb_class = new Geoweb_Class(str1, "_TBL", false, "arial", "[orientation_degree]", true, pClass, &geosim_class->geosim_layer_list, NULL, false);
							geoweb_class_list.AddTail(geoweb_class);
						}
						else if (geosim_class->f_class_type == "T")
						{
							str1 = _name;
							str1 = str1.toupper();

							geoweb_class = new Geoweb_Class(str1, "_TBL", false, "arial", "[orientation_degree]", true, pClass, &geosim_class->geosim_layer_list, NULL, false, true);
							geoweb_class_list.AddTail(geoweb_class);
						}
						else if (geosim_class->f_class_type == "O")
						{
							str1 = _name;
							str1 = str1.toupper();

							geoweb_class = new Geoweb_Class(str1, "", true, _font, "", false, pClass, &geosim_class->geosim_layer_list, NULL, true, true);
							geoweb_class_list.AddTail(geoweb_class);

							geoweb_class = new Geoweb_Class(str1, "_TBL", false, "arial", "[orientation_degree]", true, pClass, &geosim_class->geosim_layer_list, NULL, false);
							geoweb_class_list.AddTail(geoweb_class);
						}
						else if ((geosim_class->f_class_type == "L") && (id != -1))
						{
							str1 = _name;
							str1 = str1.toupper();

							geoweb_class = new Geoweb_Class(str1, "_L", true, "", "", false, pClass, &geosim_class->geosim_layer_list, NULL, true, true);
							geoweb_class_list.AddTail(geoweb_class);

							geoweb_class = new Geoweb_Class(str1, "_L_TBL", false, "arial", "[orientation_degree]", true, pClass, &geosim_class->geosim_layer_list, NULL, false);
							geoweb_class_list.AddTail(geoweb_class);
						}
						else if (geosim_class->f_class_type == "S")
						{
							str1 = _name;
							str1 = str1.toupper();

							geoweb_class = new Geoweb_Class(str1, "_L", true, "", "", false, pClass, &geosim_class->geosim_layer_list, NULL, true, true);
							geoweb_class_list.AddTail(geoweb_class);	

							geoweb_class = new Geoweb_Class(str1, "_PLG", true, "", "", false, pClass, &geosim_class->geosim_layer_list, NULL, true);
							geoweb_class_list.AddTail(geoweb_class);

							geoweb_class = new Geoweb_Class(str1, "_P", true, _font, "[orientation_degree]", false, pClass, &geosim_class->geosim_layer_list, geoweb_symbol_ttf_list, false);
							geoweb_class_list.AddTail(geoweb_class);

							geoweb_class = new Geoweb_Class(str1, "_T_TBL", false, "arial", "[orientation_degree]", false, pClass, &geosim_class->geosim_layer_list, NULL, false);
							geoweb_class_list.AddTail(geoweb_class);
						}
						else if ((geosim_class->f_class_type == "P") && (id == -1))
						{
							pClass = gsc_find_class(prj, parent_id, 0);
							sub = pClass->ptr_sub_list()->getpos_name(geosim_class->f_class_name.get_name());
							pSub = (C_SUB*) pClass->ptr_sub_list()->getptr_at(sub);
							
							str1 = "s";
							str1 += pSub->ptr_id()->code;
							str1 += "_";
							str1 += _name;
							str1.toupper();

							geoweb_class = new Geoweb_Class(str1, "", true, _font, "[orientation_degree]", false, pSub, &geosim_class->geosim_layer_list, geoweb_symbol_ttf_list, false, true);
							geoweb_class_list.AddTail(geoweb_class);

							geoweb_class = new Geoweb_Class(str1, "_TBL", false, "arial", "[orientation_degree]", true, pSub, &geosim_class->geosim_layer_list, NULL, false);
							geoweb_class_list.AddTail(geoweb_class);
						}
						else if ((geosim_class->f_class_type == "L") && (id == -1))
						{
							pClass = gsc_find_class(prj, parent_id, 0);
							sub = pClass->ptr_sub_list()->getpos_name(geosim_class->f_class_name.get_name());
							pSub = (C_SUB*) pClass->ptr_sub_list()->getptr_at(sub);
							
							str1 = "s";
							str1 += pSub->ptr_id()->code;
							str1 += "_";
							str1 += _name;
							str1.toupper();

							geoweb_class = new Geoweb_Class(str1, "_L", true, "", "", false, pSub, &geosim_class->geosim_layer_list, NULL, true, true);
							geoweb_class_list.AddTail(geoweb_class);

							geoweb_class = new Geoweb_Class(str1, "_L_TBL", false, "arial", "[orientation_degree]", true, pSub, &geosim_class->geosim_layer_list, NULL, false);
							geoweb_class_list.AddTail(geoweb_class);
						}
						break;
					}
					pItem = (C_INT_INT_STR*) classList.get_next();
				}
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

int Geoweb_Structure::UpdateQt(int prj, C_STRING prj_name, C_INT_INT_STR_LIST &classList)
{
	int ret = GS_GOOD;
	long		id, parent_id;
	POSITION	pos;
	C_STRING	name, _name, str1, str2;
	int		sub;

	Geosim_Class	*geosim_class;

	Geoweb_Qt		*geoweb_qt;

	C_CLASS		*pClass;
	C_SUB		*pSub;

	C_INT_INT_STR	*pItem;
	TRY
	{
		pos = geosim_class_list.GetHeadPosition();
		while (pos)
		{
			geosim_class = (Geosim_Class*) geosim_class_list.GetNext(pos);			
			id = geosim_class->f_class_id;
			pItem = (C_INT_INT_STR*) classList.get_head();
			if (pItem)
			{	
				pItem = (C_INT_INT_STR*) classList.get_next();
				while (pItem)
				{	
					if (pItem->get_key() == id)
					{	
						name = geosim_class->f_class_name;
						_name = correctString3(name);
						parent_id = geosim_class->parent_f_class_id;

						if (((geosim_class->f_class_type == "P") && (id != -1)) || (geosim_class->f_class_type == "O"))
						{
 							pClass = gsc_find_class(prj, id, 0);

							str1 = _name;
							str1 = str1.toupper();
							str2 = _name;

							geoweb_qt = new Geoweb_Qt(str1, "", str2, pClass);
							geoweb_qt_list.AddTail(geoweb_qt);	
						}
						else if (geosim_class->f_class_type == "T")
						{
 							pClass = gsc_find_class(prj, id, 0);

							str1 = _name;
							str1 = str1.toupper();
							str2 = _name;

							geoweb_qt = new Geoweb_Qt(prj_name, str1, "_TBL", str2, pClass);
							geoweb_qt_list.AddTail(geoweb_qt);	
						}
						else if ((geosim_class->f_class_type == "L") && (id != -1))
						{
 							pClass = gsc_find_class(prj, id, 0);

							str1 = _name;
							str1 = str1.toupper();
							str2 = _name;
							
							geoweb_qt = new Geoweb_Qt(prj_name, str1, "_L", str2, pClass);
							geoweb_qt_list.AddTail(geoweb_qt);	
						}
						else if ((geosim_class->f_class_type == "P") && (id == -1))
						{
 							pClass = gsc_find_class(prj, parent_id, 0);
							sub = pClass->ptr_sub_list()->getpos_name(geosim_class->f_class_name.get_name());
							pSub = (C_SUB*) pClass->ptr_sub_list()->getptr_at(sub);

							str1 = "s";
							str1 += pSub->ptr_id()->code;
							str1 += "_";
							str1 += _name;
							str1.toupper();
							str2 = pClass->ptr_id()->name;
							str2 += "-";
							str2 += _name;
							
							geoweb_qt = new Geoweb_Qt(str1, "", str2, pSub);
							geoweb_qt_list.AddTail(geoweb_qt);	
						}
						else if ((geosim_class->f_class_type == "L") && (id == -1))
						{
 							pClass = gsc_find_class(prj, parent_id, 0);
							sub = pClass->ptr_sub_list()->getpos_name(geosim_class->f_class_name.get_name());
							pSub = (C_SUB*) pClass->ptr_sub_list()->getptr_at(sub);

							str1 = "s";
							str1 += pSub->ptr_id()->code;
							str1 += "_";
							str1 += _name;
							str1.toupper();
							str2 = pClass->ptr_id()->name;
							str2 += "-";
							str2 += _name;

							geoweb_qt = new Geoweb_Qt(prj_name, str1, "_L", str2, pSub);
							geoweb_qt_list.AddTail(geoweb_qt);	
						}
						break;
					}
					pItem = (C_INT_INT_STR*) classList.get_next();
				}
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