#pragma once
#include "python.h"

void InitPython();//initiate the python environment.
wchar_t *PyEval(wchar_t *arg);//do the same as eval(),return the result or exception info when sth. wrong.
wchar_t *PyExec(wchar_t *arg);//do the same as exec(),return arg when ok or exception info when sth. wrong.
wchar_t *PyRunFile(wchar_t *fn);//run the whole file specified by file name "fn",return file content when ok or exception info when wrong.



