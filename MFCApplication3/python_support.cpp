#include "stdafx.h"
#include "python_support.h"
PyObject  *pRet = nullptr;
PyObject  *pModule = nullptr;
#define  TMP_NAME "__c2p2c__"
void InitPython()
{
	Py_SetProgramName(_T("ÈÈ¼üµ÷ÓÃ"));  /* optional but recommended */
	Py_Initialize();
	pModule = PyImport_ImportModule("__main__");
}
void PySetStr(wchar_t *arg)//assign arg to TMP_NAME in python.
{
	PyObject *p = PyUnicode_FromUnicode(arg, wcslen(arg));
	PyObject_SetAttrString(pModule, TMP_NAME, p);
	Py_DECREF(p);
}
wchar_t *PyGetStr()//get value of TMP_NAME in python.
{
	if (pRet)Py_DECREF(pRet);
	pRet = PyObject_GetAttrString(pModule, TMP_NAME);
	return PyUnicode_AsUnicode(pRet);
}
wchar_t *PyExecW(wchar_t *arg)
{
	PySetStr(arg);
	PyRun_SimpleString("try :\n exec("TMP_NAME")\nexcept Exception as exp:\n "TMP_NAME"=str(exp)");
	return PyGetStr();
}
wchar_t *PyEvalW(wchar_t *arg)
{
	PySetStr(arg);
	PyRun_SimpleString("try :\n "TMP_NAME"=str(eval("TMP_NAME"))\nexcept Exception as exp : \n "TMP_NAME" = str(exp)");
	return PyGetStr();
}
wchar_t *PyRunFile(wchar_t *fn)
{
	PySetStr(fn);
	PyRun_SimpleString("try: exec(open("TMP_NAME").read())\nexcept Exception as ex:\n "TMP_NAME"=str(ex)");
	return PyGetStr();
}