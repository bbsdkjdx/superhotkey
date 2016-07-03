#pragma once
#include "python.h"

void InitPython();//initiate the python environment.
wchar_t *PyExecW(wchar_t *arg);
wchar_t *PyEvalW(wchar_t *arg);
wchar_t *PyRunFile(wchar_t *fn);