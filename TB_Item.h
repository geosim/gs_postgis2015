#ifndef _tb_item_h
#define _tb_item_h 1

#define INITGUID
#import "msado15.dll" no_namespace rename ("EOF", "EndOfFile") rename ("EOS", "ADOEOS")

#ifndef _gs_class_h
	#include "gs_class.h"
#endif

#ifndef _gs_sec_h
	#include "gs_sec.h"
#endif

#ifndef _tb_attribute_h
	#include "TB_Attribute.h"
#endif

class TB_Item : public CObject
{
	DECLARE_DYNAMIC(TB_Item)

private:
	long	fid;
	long	job_version;
	bool	old;

public:
	TB_Attrib_List	tb_attrib_list;

private:
	virtual void WriteSQLOracle(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list,
                               CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn) {};
	virtual void WriteSQLPostgres(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list,
                                 CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn, long srid) {};
	virtual void get_fid_parent(long *_fid_parent) {};

public:
	TB_Item();
	TB_Item(long _l, bool _b=false);
	~TB_Item();

	void set_fid(long _l);
	void set_job_version(long _l);

	long get_fid();
	long get_job_version();
	bool is_old();

	virtual int WriteSQLOracle(FILE *file, C_STRING &_prjname, C_STRING &_name);
	virtual int WriteSQLPostgres(FILE *file, C_STRING &_prjname, C_STRING &_name);
	virtual int WriteSQLOracle(FILE *file, C_STRING &_prjname, C_STRING &_name, CObList* tb_attribute_list,
                              CObList* tb_item_attribute_list, CStringList *reserved_list, 
                              C_DBCONNECTION *pConn=NULL);
	virtual int WriteSQLPostgres(FILE *file, C_STRING &_prjname, C_STRING &_name, CObList* tb_attribute_list,
                                CObList* tb_item_attribute_list, CStringList *reserved_list,
                                long srid, C_DBCONNECTION *pConn=NULL);

	void get_item_attribute_list(CObList* tb_item_attribute_list);
};

class TB_Item_List : public CObList
{
public:
	TB_Item* find(long _fid);
	TB_Item* find_fid_parent(long _fid_parent, AcGePoint3d _point3d);
};

class TB_Item_Label: public TB_Item
{
	DECLARE_DYNAMIC(TB_Item_Label)

private:
	C_STRING	geom;
	C_STRING	suf;
	double	orientation;
	int		origin;
	long	label_def_id;
	long	fid_parent;    
	C_STRING	label_text;
	C_STRING	field_name;
	C_STRING	pre;
	int		hidden;
	C_STRING	layer;
	C_COLOR  n_color;
	double	text_height;
	C_STRING	text_style;
	double	orientation_degree;
	double	x;
	double	y;
	long	id_label_block;
	double	orientation_label_block;
	double	x_label_block;
	double	y_label_block;

private:
	void WriteSQLOracle(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list, 
                       CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn);
	void WriteSQLPostgres(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list,
                         CObList* tb_item_attribute_list, CStringList *reserved_list,
                         C_DBCONNECTION *pConn, long srid);
	C_STRING WriteGeomOracle();
	C_STRING WriteGeomPostgres(long srid);

public:
	TB_Item_Label() {};
	TB_Item_Label(long _fid, double _orientation, long _fid_parent, C_STRING _label_text, C_STRING _field_name, 
				int _hidden, double _x, double _y, double _x_label_block, double _y_label_block);
	virtual ~TB_Item_Label() {};

	void get_item_attribute_list(CObList* tb_item_attribute_list);

	void set_text_height(double _d);
	void set_text_style(C_STRING _s);
	void set_layer(C_STRING _s);
	void set_color(C_COLOR &_l);
};

class TB_Item_Point: public TB_Item
{
	DECLARE_DYNAMIC(TB_Item_Point)

private:
	C_STRING geom;
	double  orientation;
	long    quality;
	double  z;
	C_STRING block_name;
	double  block_scale_factor;
	C_STRING layer;
	C_COLOR n_color;
	double  orientation_degree;
	double  x;
	double  y;
	long    fid_parent;

private:
	void WriteSQLOracle(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list,
                       CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn);
	void WriteSQLPostgres(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list,
                         CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn, long srid);
	C_STRING WriteGeomOracle();
	C_STRING WriteGeomPostgres(long srid);

public:
	TB_Item_Point() {};
	TB_Item_Point(long _fid, double _orientation, double _x, double _y, double _z, long _fid_parent=-1);
	virtual ~TB_Item_Point() {};

	void get_item_attribute_list(CObList* tb_item_attribute_list, bool parent=false);
	void get_fid_parent(long *_fid_parent);

	double get_x();
	double get_y();

	void set_block_name(C_STRING &_s);
	void set_block_scale_factor(double _d);
	void set_layer(C_STRING &_s);
	void set_color(C_COLOR &_l);
	void set_fid_parent(long _l);
};

class TB_Item_Surface: public TB_Item
{
	DECLARE_DYNAMIC(TB_Item_Surface)

private:
	C_STRING geom;
	double  area;
	C_STRING hatch_name;
	double  hatch_l_width;
	C_STRING hatch_l_type;
	C_COLOR hatch_color;
	double  hatch_space;
	C_STRING layer;
	C_STRING l_type;
	double  l_width;
	C_COLOR n_color;
	double  thickness;
	double  length;
	long    fid_parent;

private:
	void WriteSQLOracle(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list,
                       CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn);
	void WriteSQLPostgres(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list,
                         CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn, long srid);
	C_STRING WriteGeomOracle();
	C_STRING WriteGeomPostgres(long srid);

public:
	TB_Item_Surface() {};
	TB_Item_Surface(bool isOracle, long _fid, double _area, double _length, CList<AcGePoint3d,AcGePoint3d> *point_list, long _fid_parent=-1);
	TB_Item_Surface(long _fid, double _area);
	virtual ~TB_Item_Surface() {};

	void Update(bool isOracle, CList<AcGePoint3d,AcGePoint3d> *point_list);

	void get_item_attribute_list(CObList* tb_item_attribute_list, bool parent=false);
	void get_fid_parent(long *_fid_parent);

	void set_area(double _d);
	void update_area(double _d);
	void set_length(double _d);
	void update_length(double _d);

	void set_layer(C_STRING _s);
	void set_l_type(C_STRING _s);
	void set_color(C_COLOR &_l);
	void set_thickness(double _d);
	void set_l_width(double _d);
	void set_hatch(C_STRING _h);
	void set_hatch_color(C_COLOR &_l);
	void set_hatch_space(double _d);
};

class TB_Item_Text: public TB_Item
{
	DECLARE_DYNAMIC(TB_Item_Text)

private:
	C_STRING	text;
	long		fid_parent;

private:
	void WriteSQLOracle(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list,
                       CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn);
	void WriteSQLPostgres(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list,
                         CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn, long srid);

public:
	TB_Item_Text() {};
	TB_Item_Text(long _fid, C_STRING _text, long _fid_parent=-1);
	virtual ~TB_Item_Text() {};

	void get_item_attribute_list(CObList* tb_item_attribute_list, bool parent=false);
	void get_fid_parent(long *_fid_parent);
};

class TB_Item_Polyline_L: public TB_Item
{
	DECLARE_DYNAMIC(TB_Item_Polyline_L)

private:
	C_STRING geom;
	double  length;
	C_STRING layer;
	C_STRING l_type;
	double  l_width;
	C_COLOR n_color;
	double  thickness;
	long    fid_parent;

private:
	void WriteSQLOracle(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list, CObList* tb_item_attribute_list, CStringList* reserved_list, C_DBCONNECTION *pConn);
	void WriteSQLPostgres(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list, CObList* tb_item_attribute_list, CStringList* reserved_list, C_DBCONNECTION *pConn, long srid);
	C_STRING WriteGeomOracle();
	C_STRING WriteGeomPostgres(long srid);

public:
	TB_Item_Polyline_L() {};
	TB_Item_Polyline_L(bool isOracle, long _fid, double _length, CList<AcGePoint3d,AcGePoint3d> *point_list, long _fid_parent=-1);
	virtual ~TB_Item_Polyline_L() {};

	void get_item_attribute_list(CObList* tb_item_attribute_list);
	void get_fid_parent(long *_fid_parent);

	void set_layer(C_STRING _s);
	void set_l_type(C_STRING _s);
	void set_color(C_COLOR &_l);
	void set_thickness(double _d);
	void set_l_width(double _d);
};

class TB_Item_Polyline: public TB_Item
{
	DECLARE_DYNAMIC(TB_Item_Polyline)

private:
	double		length;

private:
	void WriteSQLOracle(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list, CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn);
	void WriteSQLPostgres(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list, CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn, long srid);

public:
	TB_Item_Polyline() {};
	TB_Item_Polyline(long _fid, double _length);
	virtual ~TB_Item_Polyline() {};

	void get_item_attribute_list(CObList* tb_item_attribute_list);

	void update_length(double _d);
};

class TB_Item_Sec: public TB_Item
{
	DECLARE_DYNAMIC(TB_Item_Sec)

private:
	long	fid_parent;

private:
	void WriteSQLOracle(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list, CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn);
	void WriteSQLPostgres(C_STRING &str1, C_STRING &str2, CObList* tb_attribute_list, CObList* tb_item_attribute_list, CStringList *reserved_list, C_DBCONNECTION *pConn, long srid);

public:
	TB_Item_Sec() {};
	TB_Item_Sec(long _fid, long _fid_parent);
	virtual ~TB_Item_Sec() {};

	int WriteSQLOracle(FILE *file, C_STRING &_prjname, C_STRING &_name, 
                      CObList* tb_item_attribute_list, C_DBCONNECTION *pConn);
	int WriteSQLPostgres(FILE *file, C_STRING &_prjname, C_STRING &_name,
                        CObList* tb_item_attribute_list, C_DBCONNECTION *pConn);

	void get_item_attribute_list(CObList* tb_item_attribute_list);
};

class Geosim_Layer_List : public CObList
{
};

class Geoweb_Catalog : public CObject
{
public:
	C_STRING	project_name;
	C_STRING	name;
	C_STRING path;

public:
	Geoweb_Catalog(C_STRING _project_name, C_STRING _name, C_STRING _database);
	~Geoweb_Catalog() {};
};

class Geoweb_Style : public CObject
{
public:
	BYTE    colorR;
	BYTE    colorG;
	BYTE    colorB;
	BYTE    outlinecolorR;
	BYTE    outlinecolorG;
	BYTE    outlinecolorB;
	C_STRING style_name;

public:
	Geoweb_Style(C_STRING _name, C_CLASS *pCls, bool load);
	~Geoweb_Style() {};
};

class Geoweb_Class : public CObject
{
public:
	C_STRING theme_name;
	C_STRING layergroup_name;
	C_STRING layer_name;
	C_STRING class_name;
	C_STRING original_class_name;
	
	C_STRING expression;
	bool	  legendtype_id;
	C_STRING label_font;
	C_STRING label_block;
	C_STRING label_angle;
	BYTE	  label_colorR;
	BYTE	  label_colorG;
	BYTE	  label_colorB;
	BYTE	  label_outlinecolorR;
	BYTE	  label_outlinecolorG;
	BYTE	  label_outlinecolorB;
	C_STRING label_position;

	Geoweb_Style	*geoweb_style;

	Geosim_Layer_List	*geosim_layer_list;

public:
	Geoweb_Class(C_STRING _name, C_STRING _ext, bool _legendtype_id, C_STRING _label_font, C_STRING _label_angle, bool is_tbl, C_CLASS *pCls, Geosim_Layer_List *_geosim_layer_list, CStringList *_geoweb_symbol_ttf_list, bool style, bool load=false);
	~Geoweb_Class() {};
};

class Geoweb_Class_List : public CObList
{
};

class Geoweb_Layer : public CObject
{
public:
	C_STRING	layergroup_name;
	C_STRING	layer_name;
	int		layertype_id;
	C_STRING	project_name;
	C_STRING	catalog_name;
	C_STRING	data;
	C_STRING	data_geom;
	C_STRING	data_unique;
	long	data_srid;
	C_STRING	data_filter;
	C_STRING	labelitem;
	long	symbolscale;
	long	layer_order;
	C_STRING	layer_def;

public:
	Geoweb_Layer(C_STRING _layergroup_name, C_STRING _layer_name, int _layertype_id, C_STRING _project_name, C_STRING _catalog_name, C_STRING _data, long _srid, C_STRING _ext= "", bool _view = false);
	~Geoweb_Layer() {};

	void Update(C_STRING _data_filter, C_STRING _labelitem);
	void Update(C_STRING _layer_def);
};

class Geoweb_Layer_List : public CObList
{
};

class Geoweb_Layergroup : public CObject
{
public:
	C_STRING	theme_name;
	C_STRING	project_name;
	C_STRING	layergroup_name;
	C_STRING	layergroup_title;

public:
	Geoweb_Layergroup(C_STRING _theme_name, C_STRING _project_name, C_STRING _layergroup_name, C_STRING _layergroup_title);
	~Geoweb_Layergroup() {};
};

class Geoweb_Layergroup_List : public CObList
{
};

class Geoweb_Project : public CObject
{
public:
	C_STRING	project_name;
	C_STRING	base_path;
	long	project_srid;

public:
	Geoweb_Project(C_STRING _project_name, C_STRING _base_path, long _srid);
	~Geoweb_Project() {};
};

class Geoweb_Theme : public CObject
{
public:
	C_STRING	project_name;
	C_STRING	theme_name;
	C_STRING	theme_title;

public:
	Geoweb_Theme(C_STRING _project_name, C_STRING _theme_name, C_STRING _theme_title);
	~Geoweb_Theme() {};
};

class Geoweb_Theme_List : public CObList
{
};

class Geoweb_Qtfield : public CObject
{
public:
	C_STRING qtfield_name;
  
public:
	Geoweb_Qtfield(C_STRING _qtfield_name);
	~Geoweb_Qtfield() {};
};


class Geoweb_Qtfield_List : public CObList
{
};

class Geoweb_Qtrelation : public CObject
{
public:
	C_STRING data_field;
	C_STRING catalog_name;
	C_STRING table_name;
	C_STRING table_field;
	int		qtrelationtype_id;

public:
	Geoweb_Qtrelation(C_STRING _data_field, C_STRING _catalog_name, C_STRING _table_name, C_STRING _table_field, int _qtrelationtype_id);
	~Geoweb_Qtrelation() {};
};

class Geoweb_Qt : public CObject
{
public:
	C_STRING theme_name;
	C_STRING layergroup_name;
	C_STRING	layer_name;
	C_STRING	qt_name;

	Geoweb_Qtrelation	*geoweb_qtrelation;
	Geoweb_Qtfield_List	geoweb_qtfield_list;

public:
	Geoweb_Qt(C_STRING _name, C_STRING _ext, C_STRING _qt_name, C_CLASS *pCls);
	Geoweb_Qt(C_STRING _name1, C_STRING _name2, C_STRING _ext, C_STRING _qt_name, C_CLASS *pCls);
	~Geoweb_Qt() {};
};

class Geoweb_Qt_List : public CObList
{
};

class Geosim_Layer : public CObject
{
public:
	C_STRING		layer_name;
	BYTE			colorR;
	BYTE			colorG;
	BYTE			colorB;

public:
	Geosim_Layer(C_STRING _layer_name, BYTE _R, BYTE _G, BYTE _B);
	~Geosim_Layer() {};
};

class Geosim_Class : public CObject
{
public:
	long		f_class_id;
	C_STRING	f_class_name;
	C_STRING	caption;
	C_STRING	f_class_type;
	long		last;
	long		topic_id;
	long		parent_f_class_id;
	Geosim_Layer_List	geosim_layer_list;

public:
	Geosim_Class(long _id, C_STRING _name, C_STRING _caption, C_STRING _type, long _last, long _tid, long _pid);
	~Geosim_Class() {};
};

class Geosim_Class_List : public CObList
{
};


class Geoweb_Structure : public CObject
{
public:
	Geosim_Class_List		geosim_class_list;

	Geoweb_Project			*geoweb_project;
	Geoweb_Catalog			*geoweb_catalog;
    Geoweb_Theme_List		geoweb_theme_list;
    Geoweb_Layergroup_List	geoweb_layergroup_list;
	Geoweb_Layer_List		geoweb_layer_list;
	Geoweb_Class_List		geoweb_class_list;
    Geoweb_Qt_List			geoweb_qt_list;

	CStringList				*geoweb_symbol_ttf_list;

public:
	Geoweb_Structure();
	~Geoweb_Structure() {};

	int Update(int prj, C_STRING prj_name, C_STRING prj_path, C_STRING database, C_STRING font, C_INT_INT_STR_LIST &classList, long srid);
	int UpdateTheme(C_STRING prj_name, C_INT_INT_STR_LIST &classList);
	int UpdateLayer(int prj, C_STRING prj_name, C_STRING catalog_name, C_INT_INT_STR_LIST &classList, long srid);
	int UpdateClass(int prj, C_STRING prj_name, C_STRING font, C_INT_INT_STR_LIST &classList);
	int UpdateQt(int prj, C_STRING prj_name, C_INT_INT_STR_LIST &classList);
};

#endif