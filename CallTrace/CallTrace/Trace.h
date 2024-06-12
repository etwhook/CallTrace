#pragma once

#include<iostream>
#include<windows.h>
#include<winternl.h>
#include<DbgHelp.h>

#define PROCESS_INFO_CLASS_INSTRUMENTATION 40

typedef struct _PROCESS_INSTRUMENTATION_CALLBACK_INFORMATION
{
	ULONG Version;
	ULONG Reserved;
	PVOID Callback;

} PROCESS_INSTRUMENTATION_CALLBACK_INFORMATION, * PPROCESS_INSTRUMENTATION_CALLBACK_INFORMATION;

typedef NTSTATUS(NTAPI* t_NtSetInformationProcess) (

	HANDLE               ProcessHandle,
	PROCESS_INFORMATION_CLASS ProcessInformationClass,
	PVOID                ProcessInformation,
	ULONG                ProcessInformationLength

);

VOID InitInstrumentionCallback();