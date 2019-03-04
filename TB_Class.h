#ifndef _tb_class_h
#define _tb_class_h 1

#define INITGUID
#import "msado15.dll" no_namespace rename ("EOF", "EndOfFile") rename ("EOS", "ADOEOS")

//#import <mscorlib.dll>

#include <dbpl.h>
#include <dbhatch.h>
#include <dbspline.h>
#include <dbelipse.h>

#ifndef _gs_resbf_h
	#include "gs_resbf.h"
#endif

#ifndef _gs_utily_h
	#include "gs_utily.h"
#endif

#ifndef _gs_init_h
	#include "gs_init.h"
#endif

#ifndef _gs_class_h
	#include "gs_class.h"
#endif

#ifndef _gs_filtr_h
	#include "gs_filtr.h"
#endif

#ifndef _gs_query_h
	#include "gs_query.h"
#endif

#ifndef _tb_attribute_h
	#include "TB_Attribute.h"
#endif

#ifndef _tb_item_h
	#include "TB_Item.h"
#endif

//using namespace System;

class TB_Class : public CObject
{
	friend class TB_Class_List;

private:
	long		f_class_id;
	C_STRING		f_prj_name;
	C_STRING		f_class_name;
	C_STRING		caption;
	C_STRING		f_class_type;
	int			read_only;
	int			active;
	int			table_does_not_exist;
	long		topic_id;
	long		parent_f_class_id;
	C_STRING		spatial_relate;
	double		min_x;
	double		min_y;
	double		max_x;
	double		max_y;
	double		min_z;
	double		max_z;
	double		tolerance;
	int			dimension;
	int			user_defined_orientation;

	CInfo		*info;
	C_CLASS		*pCls;

	TB_Class_List	*ptb_class_list;
	TB_Class_List	*ptb_sec_list;

	int			num_vertex;		// utilizzato per rendere gli oggetti cerchi/ellissi attraverso una serie di punti

public:
	TB_Item_List	tb_item_list;

	CObList			tb_item_attribute_list;
	CObList			tb_attribute_list;

 	CStringList		allLayers;

public: 
	TB_Class(CInfo *_info, CStringList *reserved_list);
	~TB_Class();

	int	WriteSql(bool isOracle, FILE *file, bool create, C_STRING &prj_name, long srid, long *current_id1, long *current_id2, CStringList *reserved_list);
	int	WriteSecExt(bool isOracle, FILE *file, C_STRING &prj_name, CStringList *reserved_list);

private:
	void	set_f_class_id(long _l);
	void	set_f_prj_name(C_STRING &_s);
	void	set_f_prj_name(const TCHAR *_s);
	void	set_f_class_name(C_STRING &_s);
	void	set_f_class_name(const TCHAR *_s);
	void	set_caption(C_STRING &_s);
	void	set_caption(const TCHAR *_s);
	void	set_f_class_type(C_STRING &_s);
	void	set_f_class_type(const TCHAR *_s);
	void	set_attribute_list(bool isOracle, C_ATTRIB_LIST *pList, C_INFO *pInfo);

	C_STRING	get_f_prj_name();
	C_STRING	get_f_class_name();
	C_STRING	get_caption();
	C_STRING	get_f_class_type();
	CObList* get_attribute_list();

	int	Load(bool isOracle, C_STRING &prj_name, long *current_id1, long *current_id2, CStringList *reserved_list);
	int	LoadPoints(long *current_id1, long *current_id2, CStringList *reserved_list);
	int	LoadTexts(long *current_id1, CStringList *reserved_list);
	int	LoadSurfaces(bool isOracle, long *current_id1, CStringList *reserved_list);
	int	LoadPolylines(bool isOracle, long *current_id1, long *current_id2, CStringList *reserved_list);
	int	LoadSpaghetti(bool isOracle, long *current_id1);
	int	LoadBlocks(C_SELSET selSet, TB_Class* _tb_class, C_CLASS *pClsint, long *current_id1);
	int	LoadSecondaries(bool isOracle, CStringList *reserved_list);
	int	LoadSecondariesExt(bool isOracle, CStringList *reserved_list);

	int AllLayers();
	int Explode_LeaderDimension(C_STRING layer);
	int Explode_Insert(C_STRING layer);

	int CreateTableOracle(FILE *file, bool create1, bool create2, CStringList *reserved_list, C_DBCONNECTION *pConn);
	int CreateTablePostgres(FILE *file, bool create1, bool create2, long srid, CStringList *reserved_list, C_DBCONNECTION *pConn);
	int CreateTableSecExtOracle(FILE *file, C_DBCONNECTION *pConn);
	int CreateTableSecExtPostgres(FILE *file, C_DBCONNECTION *pConn);
	int CreateTableSecsExtOracle(FILE *file, C_STRING &prj_name);
	int CreateTableSecsExtPostgres(FILE *file, C_STRING &prj_name);
	//int AddTableSpa(FILE *file, C_STRING _prjname, C_STRING _name);

	int UpdateAttribs(TB_Item *tb_item);
	int UpdateAttribs(TB_Item *tb_item, C_SECONDARY *pSec);
	int UpdateAttribsSecExt(TB_Item *tb_item, C_RB_LIST &row);

	int GetPointsOfObj(AcDbObject *pObject, CList<AcGePoint3d,AcGePoint3d> &OutListPoint, double *length, double *area);

public:
	long	get_f_class_id();
};

class TB_Class_List : public CObList
{
public:
};

int get_row_values(C_SECONDARY *pSec, _RecordsetPtr &pRs);

#endif