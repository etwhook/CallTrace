
# CallTrace

Simple Utility Tool To Trace System Calls Using Instrumention Callbacks.


## What Does It Do?
CallTrace is served as a **DLL** file that can be injected into processes, Upon injection it will first allocate a console to later display information about the syscalls then call **NtSetInformationProcess** with a nirvana structure that will setup the instrumention callback which will allow us to catch transitions from kernelmode to usermode.

## Acknowledgements

- [Instrumention Callbacks - Deputaion](https://github.com/Deputation/instrumentation_callbacks)
- [Detecting Manual Syscalls from User Mode - Winternl](https://winternl.com/detecting-manual-syscalls-from-user-mode/)
