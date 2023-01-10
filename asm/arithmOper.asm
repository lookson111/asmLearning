; 1) Реализовать подпрограммы перевода строки в число 
;    и арифметической операции
; 2) Сделать работу программы как с целочисленными переменными 
;    так и с вещественными
; 3) Начало подпрограммы и конец надо сделать в макросе
;    (может в заранее скомпилированном)
; 4) выложить исходники в гит
; %include "stud_io.inc" ; будем использовать только написанное
%include "sysmacro.inc"

global	_start
; Инициализированные данные
section .data
xten dw 10
; Не инициализированные данные
section .bss
buf resb 512
buflen equ $-buf

section	.text
_start:	
; считываем строку
;	pcall getString buf
	push dword buflen
	push dword buf		; кладем параметр в стек
	call getString		; вызваем подпрограмму
	add esp, 8		; очищаем стек от параметров
	
	push dword buf
	call writeString
	add esp, 4
; Преобразуем строку в числа, знак операции, тип числа
;	pcall getOperParam v1, v2, typeOp, typeV1, typeV2
; передаем строку в продпрограмму расчета
;	pcall getOperResult v1, v2, typeOp, typeV1, typeV2, ans, type
; выводим результат арифметической операции
;	pcall ans, type
; Конец
	call writeLN
	call quit

; печать символа перевода строки
writeLN:
	sub esp, 4
	mov edi, esp
	mov al, 10
	mov [edi], al
	kernel 4, 1, edi, 1
	add esp, 4
	ret

; завершение программы 
quit:
	kernel 1, 0

; подпрограмма чтения строки в буфер
; arg1 - буфер, arg2 - длина
getString:
	push ebp		; оргинизуем стековый фрейм
				; помещаем ebp сразу после адреса возврата 
	mov ebp, esp		; сохраняем ebp будет указывать на свой 
				; адрес в стеке
				; адрес возврата будет в [ebp+4]
	sub esp, 4		; локальная переменная
	push esi		; сохраняем регистры
	push edi
	
	xor ecx, ecx		; обнуляем счетчик символов
	mov edx, [ebp+8]	; адрес буфера в EDX
.again:	inc ecx			; увеличиваем счетчик
	cmp ecx, [ebp+12]	; если больше длины буфера то 
	jae .quit		;  выходим
	push ecx		;
	push edx
	kernel 3, 0, edx, 1	; читаем один символ с потока в
	pop edx			; буфер edx
	pop ecx
	cmp eax, 1
	jne .quit		; все прочитали выходим
	mov al, [edx]
	cmp al, 10		; если обнаружен конец строки
	je  .quit		; то выходим
	inc edx			; если нет то увеличиваем счетчик 
	jmp .again		; и продолжаем цикл заного

.quit:	mov [edx], byte 0	; заносим в edx ограничительный ноль
	pop edi			; возвращаем
	pop esi
	mov esp, ebp		; завершение подпрограммы
	pop ebp
	ret

; подпрограмма вычисляющая длину строки
; arg1 - адрес строки, eax - возвращает длину
strLen:
	push ebp
	mov ebp, esp
	
	xor eax, eax 		; обнуляем eax
	mov edx, [arg1]		; получаем адрес строки
.lp:	cmp byte [eax+edx], 0	; если обнаружен конец строки 
	jz .quit		; то выходим
	inc eax			; если нет то инкрементируем адрес
	jmp short .lp
.quit:	mov esp, ebp
	pop ebp
	ret
	

; подпрограмма записи строки в поток стандартного вывода
; arg1 - буфер
writeString:
	push ebp
	mov ebp, esp
	
	push dword [arg1]	; вызываем функцию нахождения длины
	call strLen		; строки
	add esp, 4		; результат в eax
	
	kernel 4, 1, [arg1], eax; записываем строку в поток станд вывода
	mov esp, ebp
	pop ebp
	ret


; подпрограмма для перевода числа в строку
; получает: число(arg1) и адрес буфера строки(arg2)
; CDECL имеем право портить EBP ESI EDI EAX
intToString:
	push ebp
	mov ebp, esp
	
	mov esi, [arg1]		; храним число в регистре
	mov edi, [arg2]		; адрес текущей позиции в строке
	
.again:	mov eax, [edi]		; получаем символ
	cmp ax, 0		; если ноль то строка закончилась
	jl .end			;
.end:
.quit:	mov esp, ebp	
	pop ebp
	ret

; подпрограмма перевода строки в целое число
; получает: строку(arg1) и адрес буфера(arg2) куда положить число
; выдает в eax успешность(ответ количество прочитанных символов числа) 
;  операции, в буфере число, 0 - мусор
strToInt:
	push ebp
	mov ebp, esp
	sub esp, 4		; буфер под локальную переменную
				; состояния преобразования строки	
	push ebx
	push edx		; сохраняем для умножения

	mov [local1], dword 0	; по умолчанию ставим что в локальной
				; находится мусор
	xor eax, eax		; обнуляем число для вывода результата
	mov edi, [arg2]		; адрес текущей позиции в строке
	xor ebx, ebx	
	xor edx, edx
.again:	mov bl, [edi]		; получаем символ
	cmp bl, '0'		; если меньше нуля или больше 9 то
	jl .end			; выдаем результат
	cmp bl, '9'		; заканчиваем считать
	jg .end
	inc dword [local1]	; если нашли символ числа то говорим
				; что на выходе должен быть результат
	sub bl, '0'		; получаем цифру в десятичном виде
	mul dword [xten]	; умножаем на 10 предыдущее число
	jc .error		; ПРОВЕРИТЬ работу, если произошло переполнение
				;  то выходим и выводим ошибку
	add eax, ebx		; и складываем с полученным
	jc .error
	inc edi			; возвращаемся для дальнейшей обработки
	jmp .again	
.end:	mov edi, [arg2]		; получаем адрес куда положить ответ
	mov [edi], eax		; кладем ответ по полученному адресу	
	jmp .quit

.error: mov [local1], dword 0	; заносим ошибку в локальную переменную
.quit:	mov eax, [local1]	; в eax выводим количество прочитанных 
				; символов
	pop edx			; восстанавливаем значения и выходим
	pop ebx
	mov esp, ebp
	pop ebp	
	ret

; определяем что содержится в строке целое число или вещественное
; если целое то выдаем в eax 0 если вещественное то 1
; на вход волучаем адрес строки
valueType:
	ret

; тип операции сложение вычитание умножение деление
; получаем адрес строки 
strToTypeOper:
	ret

section .data
putArithm db	'Enter an arithmetic expression', 0

; получаем на вход адрес строки выполняем арифметическое действие в выводим
; arg1 адрес строки 
arithm:			; Начало подпрограммы арифметического
			; действия на вход должна подаваться строка
			; на выходе получаем результат
	push ebp	; организуем стековый фрейм
	mov ebp, esp	; заносим адрес возврата в ebp т.к esp будет
			;  использоваться в подпрограмме
	sub esp, 4	; локальная переменная I будет по адресу 
			; [ebp-4]
	push esi	; Сохраняем регистры ESI и EDI (EAX всё равно 
	push edi	; изменится)

	mov esi, [ebp+8]; загружаем параметры: адреса строки 
	mov edi, [ebp+12];

; Сначала определяем тип чисел поступивших на вход

; если целый то выполняем операции над целыми

; если вещественные то над вещественными

.quit:
	pop edi
	pop esi	
	mov esp, ebp
	pop ebp
	ret








;_start:	mov	ecx, 0 	; хранение результата
;	mov	ebx, 0	; временный регистр
;	mov	ebx, 0	; временный регистр
;	PRINT	"Hello write text:"	
;again:	GETCHAR
;	cmp	ax, 0	; если конец файла то заканчиваем считывание
;	jl	end	; знаковое если меньше
;	inc	ecx	; увеличиваем счётчик символов на 1
;	jmp	again	; возвращаемся к получению символа
;;end:	PUTCHAR	10	; переводим строку
;	PRINT	"Count symbols:"
;	mov	eax, ecx; переносим количество символов для деления
;	;PUTCHAR	al	; попробуем напечатать количество Верно количество совпадает
;	mov 	ecx, 0	; счетчик массива
;	mov 	edi, buf
;getdec:	cdq		; расширяем делеимое
;	mov	ebx, [xten]
;	div	ebx	; делим на 10	
;	add	dx, '0'	; получаем символ числа	
;	mov	[edi], dx	; сохраняем символ остатка в массив
;	inc	edi	; увеличиваем адрес
;	inc	ecx	; увеличиваем счетчик
;	; mov	ecx, eax; переносим частное на хранение
;	cmp	eax, 0	; если остаток от деления равен нулю то завершаем
;	je	print
;	jmp	getdec	; если нет то повторяем цикл
;print:	dec	ecx	; уменьшаем ecx
;	mov	edi, buf ; адрес массива в EDI
;	add	edi, ecx ; переходим в конец массива
;	mov	ax, [edi] ; получаем симаол из массива
;	PUTCHAR	al	; печатаем символ
;	cmp	ecx, 0
;	jl	final
;	jmp 	print
;final:	PUTCHAR	10
;	FINISH
