EXTERN Callback:NEAR
EXTERNDEF __imp_RtlCaptureContext:QWORD

.code

bridge PROC	
    mov gs:[2e0h], rsp
    mov gs:[2d8h], r10 
	
	mov r10, rcx
	sub rsp, 4d0h
	and rsp, -10h
	mov rcx, rsp
	call __imp_RtlCaptureContext
	
	sub rsp, 20h
	call callback
	
	int 3
bridge ENDP

END