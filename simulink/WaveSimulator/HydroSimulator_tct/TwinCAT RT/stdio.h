﻿// ****************** stdio.h *******************************
// Generated by TwinCAT Target for MATLAB/Simulink (TE1400)
// MATLAB R2019a (win64)
// TwinCAT 3.1.4022
// TwinCAT Target 1.2.1237
// Beckhoff Automation GmbH & Co. KG     (www.beckhoff.com)
// *************************************************************
#pragma once
#ifndef EOF
#define EOF -1
#endif

#include "RtlR0.h"

#ifdef  __cplusplus
extern "C" {
#endif
#ifndef _FILE_DEFINED
struct _iobuf {
char *_ptr;
int   _cnt;
char *_base;
int   _flag;
int   _file;
int   _charbuf;
int   _bufsiz;
char *_tmpfname;
};
typedef struct _iobuf FILE;
#define _FILE_DEFINED
#endif

#ifndef stdout
#define stdout (FILE*)(1)
#endif

int __cdecl fprintf(FILE * stream, const char * format, ...);

#ifdef  __cplusplus
}
#endif
