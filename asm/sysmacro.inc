; Макросы для упрощения программирования
; 1) объявление подпрограммы с переменным числом параметров
; 2) Макрос запуска системных вызовов

; Однострочные макросы для доступа к переменным процедурыЖ
%define arg1 ebp+8
%define arg2 ebp+12
%define arg3 ebp+16
%define local1 ebp-4
%define local2 ebp-8
%define local3 ebp-12
%define arg(n) ebp+(4*n)+4
%define local(n) ebp-(4*n)

; Макрос вызова подпрограммы
; вызывается pcall myproc, eax, myvar, 27
%macro pcall 1-* 	; от одног
  %rep %0 - 1 		; цикл по всем параметрам кроме первого
    %rotate -1 		; последний параметр становится %1
	push dword %1	; заносим параметры в стек
  %endrep
  %rotate -1		; адрес процедуры становиться %1
	call %1		; вызываем подпрограмму
	add esp, (%0 - 1) * 4	; убираем параметры из стека
%endmacro

; макрос записи значения типа double в стек
%macro doublePush 1
	mov eax, %1
	add eax, 4
	push dword [eax]
	push dword [%1]
%endmacro

%define FREADONLY 000h
%ifdef OS_FREEBSD
%define FCREATE 200h
%elifdef OS_LINUX
%define FREATE	040h
%endif


; write - 4 - standart out is 1
; exit  - 1
; read  - 3 - std in is 0
; open  - 5

 
; Макрос системных вызовов
%macro kernel 1-*
%ifdef OS_FREEBSD
  %rep %0
    %rotate -1
	push dword %1
  %endrep
	mov eax, [esp]
	int 80h
	jnc %%ok
	mov ecx, eax
	mov eax, -1
	jmp short %%q
  %%ok:	xor ecx, ecx
  %%q:	add esp, %0 * 4
%elifdef OS_LINUX
  %if %0 > 1
	push ebx
    %if %0 > 4
	push esi
	push edi
	push ebp
    %endif
  %endif
  %rep %0
    %rotate -1
	push dword %1
  %endrep
	pop eax
  %if %0 > 1
	pop ebx
    %if %0 > 2
	pop ecx
      %if %0 > 3
	pop edx
        %if %0 > 4
	pop esi
          %if %0 > 5
	pop edi
	    %if %0 > 6
	pop ebp
              %if %0 > 7
                %error "Can't do Linux syscall with 7+ params"
              %endif
            %endif
          %endif
        %endif
      %endif
    %endif
  %endif
	int 80h
	mov ecx, eax
	and ecx, 0fffff000h
	cmp ecx, 0fffff000h
	jne %%ok
	mov ecx, eax
	neg ecx
	mov eax, -1
	jmp short %%q
  %%ok:	xor ecx, ecx
  %%q:
  %if %0 > 1
    %if %0 > 4
	pop ebp
	pop edi
	pop esi
    %endif
	pop ebx
  %endif
%else
%error "Please define either OS_LINUX or OS_FREEBSD"
%endif
%endmacro  

