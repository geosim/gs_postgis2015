#include "stdafx.h"

#include <dbapserv.h>     // per acdbHostApplicationServices()
#include "base.h"

const TCHAR * E_MONTH_NAMES[12]={_T("Jan"),_T("Feb"),_T("Mar"),_T("Apr"),_T("May"),_T("Jun"),
								 _T("Jul"),_T("Aug"),_T("Sep"),_T("Oct"),_T("Nov"),_T("Dec")};
const TCHAR * I_MONTH_NAMES[12]={_T("Gen"),_T("Feb"),_T("Mar"),_T("Apr"),_T("Mag"),_T("Giu"),
								 _T("Lul"),_T("Ago"),_T("Set"),_T("Ott"),_T("Nov"),_T("Dic")};


/********************************************************/
/*.doc month_conv										*/
/*+
	Questa funzione converte una stringa rappresentante 
	un mese nel corrispondente intero.

	Parametri:
	-input
		C_STRING	month		mese
-*/  
/********************************************************/
int month_conv(C_STRING month)
{
	int i=0;

	do
	{
		 if ((!month.comp(E_MONTH_NAMES[i], FALSE))	||
			  (!month.comp(I_MONTH_NAMES[i], FALSE)))
		 {
			 i++;
			 return i;
		 }
		 i++;
	}
	while (i<12);
	
	return 0;
}


/********************************************************/
/*.doc convert_to_CTime									*/
/*+
	Questa funzione converte una stringa in un oggetto 
	CTime.

	Parametri:
	-input
		C_STRING	date		data da trasformare

-*/  
/********************************************************/
CTime convert_to_CTime(C_STRING date)
{
	C_STRING copy = date;
	
	C_STRING name;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	int year;

	name = date.left(3);
	date = date.right(date.len() - 4);
	month = month_conv(date.left(3));
	date = date.right(date.len() - 4);
	day = date.left(2).toi();
	date = date.right(date.len() - 3);
	hour = date.left(2).toi();
	date = date.right(date.len() - 3);
	minute = date.left(2).toi();
	date = date.right(date.len() - 3);
	second = date.left(2).toi();
	date = date.right(date.len() - 3);
	year = date.left(4).toi();

	return CTime(year, month, day, hour, minute, second, -1);
}


/********************************************************/
/*.doc is_after											*/
/*+
	Questa funzione confronta le due date, restituisce 1
	se first_date è posteriore a second_date.

	Parametri:
	-input
		C_STRING	first_date		prima data
		C_STRING	first_date		seconda data
-*/  
/********************************************************/
int is_after(C_STRING first_date, C_STRING second_date)
{
	CTime first = convert_to_CTime(first_date);
	CTime second = convert_to_CTime(second_date);

	if (first >= second)
		return 1;
	else
		return 0;
}


/*********************************************************/
/*.doc set_demandLoad                           */
/*+
	Questa funzione setta il valore della variabile globale
	di AutoCAD "DEMANDLOAD" a 0
      Parametri:
	- input		
      
	Restituisce GS_BAD se non riesce a settare il valore
-*/  
/*********************************************************/
int set_demandLoad()
{
	presbuf new_val;
	if ((new_val = acutBuildList(RTSHORT, 0, 0)) == NULL)
		return GS_BAD;
	if (acedSetVar(_T("DEMANDLOAD"), new_val) != RTNORM)
		return GS_BAD;
	return GS_GOOD;
}

/*********************************************************/
/*.doc reset_demandLoad                           */
/*+
	Questa funzione setta il valore della variabile globale
    di AutoCAD "DEMANDLOAD" a 1
      Parametri:
	- input		
      
	Restituisce GS_BAD se non riesce a settare il valore
-*/  
/*********************************************************/
int reset_demandLoad()
{
	presbuf new_val;
	if ((new_val = acutBuildList(RTSHORT, 1, 0)) == NULL)
		return GS_BAD;
	if (acedSetVar(_T("DEMANDLOAD"), new_val) != RTNORM)
		return GS_BAD;
	return GS_GOOD;
}

/*********************************************************/
/*.doc onlyLeft											 */
/*+
	Questa funzione ritorna la parte sinistra rispetto 
	al carattere ch
	Parametri:
	- input	
		C_STRING	input		stringa completa
		TCHAR	ch			carattere di separazione      
-*/  
/*********************************************************/
C_STRING onlyLeft(C_STRING input, TCHAR ch)
{								
	int p;
	C_STRING	temp;

	p=-1;
	temp = input;
	p = temp.reverseFind(ch);
	if (p != -1)
	{								
		if ((temp.right(p)).len() > 0)
			temp = temp.left(p);
	}
	return C_STRING(temp);
}

C_STRING correctString1(C_STRING &in)
{								
	C_STRING	out;

	out = in;
	geowebRightString(&out); //modificare 2009

	out.strtran(_T(" "), _T("_"));
	out.strtran(_T("-"), _T("_"));

	return out;
}

C_STRING correctString2(C_STRING &in)
{								
	C_STRING	out;

	TCHAR *p, *q, *r;

	out = in;
	geowebRightString(&out); //modificare 2009

   p = out.at(_T("'"));
   q = out.at(_T("\\"));
   r = out.at(_T(" "));
	if (p || q || r)
	{	
		out = _T("\"");
		out += in;
		out += _T("\"");
	}

	return out;
}

C_STRING correctString3(C_STRING &in)
{								
	C_STRING	out;

	out = in;
	geowebRightString(&out); //modificare 2009

	out.strtran(_T("'"), _T("''"));
	out.strtran(_T("\\"), _T("\\\\"));
	
	return out;
}

C_STRING correctString4(C_STRING &in)
{								
	C_STRING	out;
	C_STRING s;

	s = in.get_chr(0);
	if (s.isDigit())
	{								
		out = "_";
		out += in;
	}
	else
		out = in;

	return out;
}

void erase(C_STRING *name, char ch)
{
	name->remove(ch);
}

void eraseMinorAndMajor(C_STRING *name)
{
	erase(name, '<');
	erase(name, '>');
}

int replace(C_STRING *input, C_STRING str1, C_STRING str2)
{
	return input->strtran(str1.get_name(), str2.get_name());
}

/********************************************************/
/*.doc geowebRightString								*/
/*+
	Questa funzione corregge una stringa eliminando 
		"%%c" e sostituendoli con "Ø"   ALT+0216
		"%%d" e sostituendoli con "º"   ALT+0186
		"%%p" e sostituendoli con "±"   ALT+0177
-*/  
/********************************************************/
void geowebRightString(C_STRING *input)
{
	C_STRING temp;

	temp = *input;
	temp.strtran(_T("%%C"), _T("Ø"));
	temp.strtran(_T("%%D"), _T("º"));
	temp.strtran(_T("%%P"), _T("±"));
	temp.strtran(_T("%%c"), _T("Ø"));
	temp.strtran(_T("%%d"), _T("º"));
	temp.strtran(_T("%%p"), _T("±"));
	temp.strtran(_T("'"),   _T(" "));

	*input = temp;
}

// roby
int gsc_crea_layer_senza_scan(C_STRING layer)
{
	AcDbLayerTable *pTable;
	AcDbObjectId   layerId = AcDbObjectId::kNull;

	if (gsc_validlayer(layer.get_name()) == GS_BAD) return GS_BAD;
   
	if (acdbHostApplicationServices()->workingDatabase()->getLayerTable(pTable, AcDb::kForWrite) != Acad::eOk)
      return GS_BAD;
   if (!pTable->has(layer.get_name()))
   {
      // Create a new layer table record using the layer name passed in
	   AcDbLayerTableRecord *pLTRec = new AcDbLayerTableRecord;
	   pLTRec->setName(layer.get_name());
      // aggiungo nuovo layer
      if (pTable->add(pLTRec) != Acad::eOk)
         { delete pLTRec; pTable->close(); return GS_BAD; }
      pLTRec->close();
   }

   pTable->close();

   return GS_GOOD;
}
