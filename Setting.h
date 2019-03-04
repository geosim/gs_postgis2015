#ifndef _setting_h
#define _setting_h 1

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


class CSetting
{
private:
	C_STRING setting_path;
	FILE	*setting_ini;

	int first;
	int start;
	int finish;

public:
	CSetting();
	~CSetting();

	FILE *openFile (C_STRING path);
	int closeFile ();
	int readFile ();
	int writeFile ();

	int get_first();
	int get_start();
	int get_finish();

	void set_first(int f);
	void set_start(int f);
	void set_finish(int f);

	int set_write_first(int f);
	int set_write_start(int f);
	int set_write_finish(int f);

	C_STRING get_setting_path();
	void null_setting_ini();
	int clear();
};

#endif