#ifndef _coord_h
#define _coord_h 1

#define INITGUID
#import "msado15.dll" no_namespace rename ("EOF", "EndOfFile") rename ("EOS", "ADOEOS")

#ifndef _base_h
	#include "base.h"
#endif

class CCoord
{
private:
	C_STRING	coord_input;
	C_STRING	cat_coord_input;
	C_STRING	coord_output;
	C_STRING	cat_coord_output;

	C_STRING	coord_path;
	FILE	*coord_ini;

public:
	CCoord();
	~CCoord();

	FILE *openFile (C_STRING path);
	int closeFile ();
	int readFile ();
	int writeFile ();
	int writeFile (CCoord *coord);

	C_STRING  get_coord_input();
	C_STRING  get_cat_coord_input();
	C_STRING  get_coord_output();
	C_STRING  get_cat_coord_output();

	void set_coord_input(C_STRING s);
	void set_cat_coord_input(C_STRING s);
	void set_coord_output(C_STRING s);
	void set_cat_coord_output(C_STRING s);

	int set_write_coord_input(C_STRING s);
	int set_write_cat_coord_input(C_STRING s);
	int set_write_coord_output(C_STRING s);
	int set_write_cat_coord_output(C_STRING s);

	C_STRING get_coord_path();
	void null_coord_ini();
};

#endif