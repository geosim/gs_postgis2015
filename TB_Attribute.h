#ifndef _tb_attribute_h
#define _tb_attribute_h 1

#define INITGUID
#import "msado15.dll" no_namespace rename ("EOF", "EndOfFile") rename ("EOS", "ADOEOS")

#ifndef _gs_utily_h
	#include "gs_utily.h"
#endif

#ifndef _gs_class_h
	#include "gs_class.h"
#endif

#ifndef _base_h
	#include "base.h"
#endif

class TB_Attribute : public CObject
{
private:
	C_STRING		name;
	C_STRING		type;
	C_STRING		srcType;
	int			len;
	int			dec;
	bool		nullValue;
	C_STRING		defaultValue;

public:
	TB_Attribute(C_STRING _name, C_STRING _type, C_STRING _srcType, int _len, int _dec, bool _nullValue, C_STRING _defaultValue);
	~TB_Attribute() {};

	void	set_name(C_STRING _s);
	void	set_type(C_STRING _s);
	void	set_srcType(C_STRING _s);

	C_STRING	get_name();
	C_STRING	get_type(bool isOracle = false);
	C_STRING	get_srcType();
	C_STRING	get_defaultValue();
};

class TB_Attrib : public CObject
{
private:
	C_STRING		name;
	C_STRING		value;

public:
	TB_Attrib(C_STRING &_name, C_STRING *_value = NULL);
	~TB_Attrib() {};

	void	set_name(C_STRING _s);
	void	set_value(C_STRING _s);

	C_STRING	get_name(bool correct=false);
	C_STRING	get_value();

};

class TB_Attrib_List : public CObList
{
public:
	TB_Attrib* find(C_STRING &_name);

	C_STRING writeSQL(bool isOracle, C_STRING &s, CStringList *reserved_list);
	C_STRING writeSQL(C_STRING &s, CObList* tb_attribute_list, C_DBCONNECTION *pConn);
};

#endif