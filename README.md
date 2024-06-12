
# CallTrace

Simple Utility Tool To Trace System Calls Using Instrumention Callbacks.


## What Does It Do?
CallTrace is served as a **DLL** file that can be injected into processes, Upon injection it will first allocate a console to later display information about the syscalls then call **NtSetInformationProcess** with a nirvana structure that will setup the instrumention callback which will allow us to catch transitions from kernelmode to usermode.

## Taking it further
This is a **very** simple example of using instrumention callbacks which has been knowing for years now, However, Instrumention callbacks by themselves are not that useful for analyzing the actual **parameters** for these system calls, You only get the **Rip** (Return address) and **Rax** (Return value, most of the times it's an **NTSTATUS** code), So what if you want to analyze these parameters? There are mainly 2 methods you can use here, One is applying a **trampoline patch** to redirect the flow of the system call to a custom handler function, Second method being you can place an **int 3** (0xCC) and register a **VEH** (Vectored Exception Handler) from which you get access to a **CONTEXT** structure which has all the registers you may need.

## Acknowledgements

- [Instrumention Callbacks - Deputaion](https://github.com/Deputation/instrumentation_callbacks)
- [Detecting Manual Syscalls from User Mode - Winternl](https://winternl.com/detecting-manual-syscalls-from-user-mode/)
