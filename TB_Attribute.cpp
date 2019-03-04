/*************************************************************************/
/* INCLUDES                                                              */
/*************************************************************************/

#include "stdafx.h"

#include "TB_Attribute.h"

TB_Attribute::TB_Attribute(C_STRING _name, C_STRING _type, C_STRING _srcType, int _len, int _dec, bool _nullValue, C_STRING _defaultValue) 
{
	name = _name;
	len = _len;
	dec = _dec;
	nullValue = _nullValue;
	defaultValue = _defaultValue;
	set_type(_type);
	set_srcType(_srcType);
}

void TB_Attribute::set_name(C_STRING _s) 
{
	name = _s;
}

void TB_Attribute::set_type(C_STRING _s)
{
	type = _s;

	if ((type.find(_T("char")) > -1) && (len > 0))
	{
		type += "(";
		type += len;
		type += ")";
	}
}

void TB_Attribute::set_srcType(C_STRING _s)
{
	srcType = _s;

	if ((srcType.find(_T("char")) > -1) && (len > 0))
	{
		srcType += "(";
		srcType += len;
		srcType += ")";
	}
}

C_STRING TB_Attribute::get_name() 
{
	return name;
}

C_STRING TB_Attribute::get_type(bool isOracle) 
{
	C_STRING temp;
	temp = type;

	if (isOracle)
	{
		if ((type.find(_T("int")) > -1) || (type.find(_T("float")) > -1) ||
          (type.find(_T("NUMBER")) > -1) || (type.find(_T("FLOAT")) > -1))
		{
			temp = "NUMBER(";
			temp += len;
			temp += ",";
			temp += dec;
			temp += ")";
		}
		else
		{
			if ((type.find(_T("char")) > -1) || (type.find(_T("NVARCHAR2")) > -1))
			{
				temp = "VARCHAR2(";
				temp += len;
				temp += ")";			
			}
		}
	}
	else
	{
		// PAOLETTA TAPULLO per risolvere il problema di GEOsim8 che in alcuni casi
		// invece che il tipo varchar(n) (come dovrebbe essere) restituisce char(1).
		// Problema che dovrebbe essere risolgo in GEOsim2007
		if (temp == "char(1)")
			temp = "varchar";
	}

	return temp;
}

C_STRING TB_Attribute::get_srcType() 
{
	return srcType;
}

C_STRING TB_Attribute::get_defaultValue() 
{
	return defaultValue;
}

TB_Attrib::TB_Attrib(C_STRING &_name, C_STRING *_value) 
{
	name = _name;
   if (_value)	value = *_value;
   else value = _T("NULL");
}

void TB_Attrib::set_name(C_STRING _s) 
{
	name = _s;
}

void TB_Attrib::set_value(C_STRING _s) 
{
	value = _s;
}

C_STRING TB_Attrib::get_name(bool correct) 
{
	C_STRING s;

	s = name;
	if (correct)
		s = correctString2(s);

	return s;
}

C_STRING TB_Attrib::get_value() 
{
	C_STRING s;

	s = value;
	s = correctString3(s);

	return s;
}

TB_Attrib* TB_Attrib_List::find(C_STRING &_name)
{
	TB_Attrib	*tb_attrib = NULL;

	POSITION	pos;

	if (GetCount() > 0)
	{
		pos = GetHeadPosition();
        while (pos)
		{
			tb_attrib = (TB_Attrib*) GetNext(pos);
			if (tb_attrib->get_name() == _name)
				return tb_attrib;
		}
	}

	return NULL;
}

C_STRING TB_Attrib_List::writeSQL(bool isOracle, C_STRING &s, CStringList *reserved_list)
{
	C_STRING str, str1, str2;

	POSITION	pos, pos2;

	TB_Attrib	*tb_attrib;

	if (GetCount() > 0)
	{  
		pos = GetHeadPosition();	
		while (pos)
		{	
			tb_attrib = (TB_Attrib*) GetNext(pos);
			
			if (str2.len() > 0)
				str2 += ", ";

			//str1 = correctString1(tb_attrib->get_name(true));
			str1 = correctString1(tb_attrib->get_name());
			str1.tolower();
			pos2 = reserved_list->Find(str1.get_name());
			if (pos2)
				str1 += "_";
			if (isOracle)
			{	
				str1.toupper();
				str2 += "\"";
				str2 += str1;
				str2 += "\"";
			}
			else
			{	
				str2 += "\"";
				str2 += str1;
				str2 += "\"";
			}
		}

		if (s.len() > 0)
			str = ", ";
		str += str2;
	}

	return str;
}

C_STRING TB_Attrib_List::writeSQL(C_STRING &s, CObList* tb_attribute_list, C_DBCONNECTION *pConn)
{  
	C_STRING	str, str1;
	C_STRING	_time;

	POSITION	pos;

	TB_Attrib	*tb_attrib;
	TB_Attribute	*tb_attribute;

	if (tb_attribute_list->GetCount() > 0)
	{  
		pos = tb_attribute_list->GetHeadPosition();
		while (pos)
		{
			tb_attribute = (TB_Attribute*) tb_attribute_list->GetNext(pos);
				
			tb_attrib = NULL;
			tb_attrib = find(tb_attribute->get_name());
				
			if (tb_attrib)
			{	
				if (str1.len() > 0)
					str1 += ", ";

				if ((pConn->IsNumeric(tb_attribute->get_srcType().get_name()) == GS_GOOD) ||
                (pConn->IsNumericWithDecimals(tb_attribute->get_srcType().get_name()) == GS_GOOD) ||
                (pConn->IsBoolean(tb_attribute->get_srcType().get_name()) == GS_GOOD))
				{
					str1 += tb_attrib->get_value();
				}
				else if (pConn->IsChar(tb_attribute->get_srcType().get_name()) == GS_GOOD)
				{
					if (tb_attrib->get_value() == "NULL")
						str1 += "NULL";
					else
					{	
						str1 += "'";
						str1 += tb_attrib->get_value();
						str1 += "'";
					}
				}
				else if (pConn->IsDate(tb_attribute->get_srcType().get_name()) == GS_GOOD)
				{
					if (tb_attrib->get_value() == "NULL")
						str1 += "NULL";
					else
					{	
						_time.clear();
						gsc_getSQLDateTime(tb_attrib->get_value().get_name(), _time);
						str1 += "'";
						str1 += _time.get_name();
						str1 += "'";
					}
				}
				else // PAOLETTA - ULTIMA MODIFICA PER RISOLVERE IL CASO varBinary 
					 // DEL CAMPO numero DELLA CLASSE Fabbricati
				{
					str1 += "NULL";
				}
			}
		}
	
		if ((s.len() > 0) && (str1.len() > 0))
			str = ", ";
		str += str1;
	}

	return str;
}