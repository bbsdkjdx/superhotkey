#include "stdafx.h"
#include "python_support.h"

PyObject  *pFunc = nullptr;
PyObject  *pTuple = nullptr;
PyObject  *pRet = nullptr;
wchar_t *PyEval(wchar_t *arg)
{
	if (pRet)Py_DECREF(pRet);
	PyTuple_SetItem(pTuple, 0, PyUnicode_FromUnicode(arg, wcslen(arg)));
	pRet = PyObject_CallObject(pFunc, pTuple);
	return PyUnicode_AsUnicode(pRet);
}

wchar_t *PyExec(wchar_t *arg)
{
	//convert unicode to ansi,then call PyRun_SimpleString.
	#define  TMP_NAME "__c2p2c__"
	int ansiLen = ::WideCharToMultiByte(CP_ACP, NULL, arg, wcslen(arg), NULL, 0, NULL, NULL);
	char* szAnsi = new char[ansiLen + strlen(TMP_NAME) + 1 + 8];//8 means "=r''''''"
	strcpy_s(szAnsi, strlen(TMP_NAME) + 1, TMP_NAME);//copy tmp_name.
	strcpy_s(szAnsi + strlen(TMP_NAME), 6, "=r'''");//copy "=r'''".
	::WideCharToMultiByte(CP_ACP, NULL, arg, wcslen(arg), szAnsi + strlen(TMP_NAME) + 5, ansiLen, NULL, NULL);
	strcpy_s(szAnsi + strlen(TMP_NAME) + 5 + ansiLen, 4, "'''");
	PyRun_SimpleString(szAnsi);//assign arg to tmp_name.
	PyRun_SimpleString("try :\n exec("TMP_NAME")\nexcept Exception as exp:\n "TMP_NAME"=str(exp)");
	delete szAnsi;
	return PyEval(_T(TMP_NAME));// ret;
}

void InitPython()
{
	Py_SetProgramName(_T("ÈÈ¼üµ÷ÓÃ"));  /* optional but recommended */
	Py_Initialize();
	PyRun_SimpleString("def __pyfun(s):\n try:\n  return str(eval(s))\n except Exception as exp:\n  return str(exp)");
	PyObject  *pModule = PyImport_Import(PyUnicode_FromString("__main__"));
	pFunc = PyObject_GetAttrString(pModule, "__pyfun");
	pTuple = PyTuple_New(1);
}

wchar_t *PyRunFile(wchar_t *fn)
{
	CString ret, pycode;
	CStdioFile csf;
	if (csf.Open(fn, CStdioFile::modeRead))
	{
		CString line;
		while (csf.ReadString(line))
		{
			pycode += line + _T("\n");
		}
		csf.Close();
	}
	return PyExec(pycode.GetBuffer());
}

