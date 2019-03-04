#ifndef _info_h
#define _info_h 1

#define INITGUID
#import "msado15.dll" no_namespace rename ("EOF", "EndOfFile") rename ("EOS", "ADOEOS")

#ifndef _gs_utily_h
	#include "gs_utily.h"
#endif

#ifndef _gs_plot_h
	#include "gs_plot.h"
#endif

#ifndef _base_h
	#include "base.h"
#endif

#ifndef _tb_item_h
	#include "TB_Item.h"
#endif

enum CLASS_TYPES {CL_NONE, CL_NODE, CL_TEXT, CL_POL, CL_SUP, CL_SPAG, CL_COMPL, CL_SIM};

class CInfo
{
private:
	// PRJ
	int		prj_index;
	C_STRING	prj_name;
	int		db_prj;
	C_STRING	prj_path;
	// CLASS
	int		class_index;
	int		sub_index;
	int		class_type;
	C_STRING	class_type_text;
	C_STRING	class_name;
	C_STRING	sub_name;
	C_STRING	class_dwg_path;
	// SCRIPT
	int		script_number;
	// SQL
	int		sql;
	// DWG
	int			dwg_total;
	CStringList	dwg_name_list;
	CStringList	removed_dwg_name_list;

	int	current_id1;
	int	current_id2;
	int	K;

	// LAY
	CStringList	layer_name_list;

	C_STRING	map_postGis_path;
	FILE	*map_ini;

	C_STRING	ini_path;
	C_STRING	server;
	C_STRING	user;
	C_STRING	font;
	C_STRING schema;
	C_STRING database;
	C_STRING srid;
	C_STRING inf;
	C_STRING	isOracle;

	C_STRING	coord_input;
	C_STRING cat_coord_input;

	C_INT_LIST	complex_list;
	bool		complex;

public:
	CInfo();
	~CInfo();

	FILE *openFile (C_STRING path);
	int closeFile ();
	int readFile ();
	int writeFile ();
	int writeFile (CInfo *info);
	int clear ();

	int get_db_prj();
	int get_prj_index();
	C_STRING get_prj_name();
	C_STRING get_prj_path();
	int get_class_index();
	int get_sub_index();
	int get_class_type();
	C_STRING get_class_type_text();
	C_STRING get_class_name();
	C_STRING get_sub_name();
	C_STRING get_class_dwg_path();
	int get_script_number();
	int get_sql();
	int get_current_id1();
	int get_current_id2();
	int get_K();
	C_INT_LIST* get_complex_list();
	bool get_complex();

	C_STRING get_ini_path();
	C_STRING	get_server();
	C_STRING	get_user();
	C_STRING	get_font();
	C_STRING	get_schema();
	C_STRING	get_database();
	long	get_srid();
	bool	get_inf();
	bool	get_isOracle();
	C_STRING get_coord_input();
	C_STRING get_cat_coord_input();

	void set_db_prj(int i);
	void set_prj_index(int i);
	void set_prj_name(C_STRING s);
	void set_prj_path(C_STRING s);
	void set_class_index(int i);
	void set_sub_index(int i);
	void set_class_type(int i);
	void set_class_type_text(C_STRING s);
	void set_class_name(C_STRING s);
	void set_sub_name(C_STRING s);
	void set_class_dwg_path(C_STRING s);
	void set_script_number(int i);
	void set_ini_path(C_STRING s);
	void set_coord_input(C_STRING s);
	void set_cat_coord_input(C_STRING s);
	void set_current_id1(int i);
	void set_current_id2(int i);
	void set_K(int i);
	int read_fid_list(TB_Item_List *pList, C_STRING path);
	void set_complex(bool b);
	void set_server(C_STRING s);
	void set_user(C_STRING s);
	void set_font(C_STRING s);
	void set_schema(C_STRING s);
	void set_database(C_STRING s);
	void set_srid(long s);
	void set_inf(C_STRING s);
	void set_isOracle(C_STRING s);

	int set_write_db_prj(int i);
	int set_write_prj_index(int i);
	int set_write_prj_name(C_STRING s);
	int set_write_prj_path(C_STRING s);
	int set_write_class_index(int i);
	int set_write_sub_index(int i);
	int set_write_class_type(int i);
	int set_write_class_type_text(C_STRING s);
	int set_write_class_name(C_STRING s);
	int set_write_sub_name(C_STRING s);
	int set_write_class_dwg_path(C_STRING s);
	int set_write_script_number(int i);
	void set_sql(int i);
	int set_write_sql(int i);
	int set_write_ini_path(C_STRING s);
	int set_write_server(C_STRING s);
	int set_write_user(C_STRING s);
	int set_write_font(C_STRING s);
	int set_write_schema(C_STRING s);
	int set_write_database(C_STRING s);
	int set_write_srid(long s);
	int set_write_inf(bool s);
	int set_write_isOracle(bool s);
	int set_write_current_id1(int i);
	int set_write_current_id2(int i);
	int set_write_K(int i);
	int write_fid_list(TB_Item_List *pList, C_STRING path);

	int add_dwg(C_STRING s);
	int add_removed_dwg(C_STRING s);
	int remove_last_dwg();
	int count_dwg();
	int total_dwg();
	void get_last_dwg(C_STRING *name);
	int search_dwg(C_STRING name);
	int search_removed_dwg(C_STRING name);
	int remove_all_dwg();
	int readDwg();
	int readSql();
	int readRemovedDwg();

	int add_complex(C_STRING path, C_INT* item);
	int count_complex();
	int remove_all_complex();
	int readComplex();

	int add_layer_name(C_STRING s);
	int remove_last_layer_name();
	int count_layer_name();
	void get_last_layer_name(C_STRING *name);
	void get_layer_name(C_STRING *name, int i);
	int readLayer_name();
	POSITION find_layer_name(C_STRING s);
	int remove_fid_list(C_STRING path);

	C_STRING get_map_postGis_path();
	void null_mapsql_ini();

	C_STRING get_path_with_ext(C_STRING ext);

	bool create_table();
};

#endif