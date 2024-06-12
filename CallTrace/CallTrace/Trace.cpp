#include "Trace.h"

DWORD tlsValue;

t_NtSetInformationProcess fnNtSetInformationProcess = (t_NtSetInformationProcess)GetProcAddress(GetModuleHandleA("NTDLL"), "NtSetInformationProcess");

extern "C" void Callback(CONTEXT* ctx);
extern "C" void bridge();

bool* GetThreadDataPointer() {
	void* thread_data = nullptr;
	bool* data_pointer = nullptr;

	thread_data = TlsGetValue(tlsValue);

	if (thread_data == nullptr) {
		thread_data = reinterpret_cast<void*>(LocalAlloc(LPTR, 256));

		if (thread_data == nullptr) {
			return nullptr;
		}

		RtlZeroMemory(thread_data, 256);


		if (!TlsSetValue(tlsValue, thread_data)) {
			return nullptr;
		}
	}

	data_pointer = reinterpret_cast<bool*>(thread_data);

	return data_pointer;
}

bool SetThreadHandlingSyscall(bool value) {
	if (auto data_pointer = GetThreadDataPointer()) {
		*data_pointer = value;
		return true;
	}

	return false;
}

bool IsThreadHandlingSyscall() {
	if (auto data_pointer = GetThreadDataPointer()) {
		return *data_pointer;
	}

	return false;
}


LPCSTR GetFunctionNameFromAddress(PVOID funcAddr) {
	PVOID symbolBuffer = malloc(sizeof(SYMBOL_INFO) + MAX_SYM_NAME);

	RtlZeroMemory(symbolBuffer, sizeof(symbolBuffer));

	PSYMBOL_INFO symbolInfo = (PSYMBOL_INFO)symbolBuffer;
	symbolInfo->SizeOfStruct = sizeof(SYMBOL_INFO);
	symbolInfo->MaxNameLen = MAX_SYM_NAME;

	if (!SymFromAddr(GetCurrentProcess(), (DWORD64)funcAddr, NULL, symbolInfo)) {
		return NULL;
	}

	return symbolInfo->Name;
}

VOID Callback(PCONTEXT ctx) {
	uint64_t currentTeb = (uint64_t)NtCurrentTeb();

	ctx->Rip = *(uint64_t*)(currentTeb + 0x02d8);
	ctx->Rsp = *(uint64_t*)(currentTeb + 0x02e0);
	ctx->Rcx = ctx->R10;


	if (IsThreadHandlingSyscall()) {
		RtlRestoreContext(ctx, nullptr);
	}

	if (!SetThreadHandlingSyscall(true)) {
		RtlRestoreContext(ctx, nullptr);
	}

	PVOID returnAddress = (PVOID)ctx->Rip;
	DWORD returnValue = (DWORD)ctx->Rax;

	LPCSTR funcName = GetFunctionNameFromAddress(returnAddress);

	if (funcName) {
		printf("[*] %s ( 0x%p ) Returned ( %ld )\n", funcName, returnAddress, returnValue);
	}

	SetThreadHandlingSyscall(false);
	RtlRestoreContext(ctx, nullptr);
}



VOID InitInstrumentionCallback() {

	PROCESS_INSTRUMENTATION_CALLBACK_INFORMATION nirvana;

	nirvana.Callback = (PVOID)(ULONG_PTR)bridge;
	nirvana.Reserved = 0;
	nirvana.Version = 0;

	tlsValue = TlsAlloc();

	NTSTATUS setIcStatus = fnNtSetInformationProcess(
		GetCurrentProcess(),
		(PROCESS_INFORMATION_CLASS)PROCESS_INFO_CLASS_INSTRUMENTATION,
		&nirvana,
		sizeof(nirvana));

	printf("[*] NtSetInformationProcess ( 0x%x )\n", setIcStatus);
}