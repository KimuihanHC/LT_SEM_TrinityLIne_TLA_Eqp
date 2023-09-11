#ifndef Def_Chroma_h__
#define Def_Chroma_h__

#include <afxwin.h>

enum enCL200AErr
{
	Cl200A_Err_OK = 0,
	Cl200A_Err_FuncHold,
	Cl200A_Err_FuncExt,
	Cl200A_Err_FuncMeasure,
};

static LPCTSTR g_szCl200A[] = 
{
	_T("[CL200A] Not Error"),
	_T("[CL200A] Error Func Hold"),
	_T("[CL200A] Error Func Ext"),
	_T("[CL200A] Error Func Measure"),
};
#endif // Def_Chroma_h__
