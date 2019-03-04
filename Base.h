#ifndef _base_h
#define _base_h 1

#define INITGUID
#import "msado15.dll" no_namespace rename ("EOF", "EndOfFile") rename ("EOS", "ADOEOS")

#ifndef _gs_utily_h
	#include "gs_utily.h"
#endif

#ifndef _gs_thm_h
	#include "gs_thm.h"
#endif

#define   POSTGIS	"POSTGIS"

int month_conv(C_STRING month);
CTime convert_to_CTime(C_STRING date);
int is_after(C_STRING first_date, C_STRING second_date);

int set_demandLoad();
int reset_demandLoad();

C_STRING onlyLeft(C_STRING input, TCHAR ch);

C_STRING correctString1(C_STRING &in);
C_STRING correctString2(C_STRING &in);
C_STRING correctString3(C_STRING &in);
C_STRING correctString4(C_STRING &in);

void eraseMinorAndMajor(C_STRING *name);

int replace(C_STRING *input, C_STRING str1, C_STRING str2);

void geowebRightString(C_STRING *input);

int gsc_crea_layer_senza_scan(C_STRING layer);

#endif
