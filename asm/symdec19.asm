%include "stud_io.inc"
global	_start

section .data
xten dw 10

section .bss
buf resb 128

section	.text
_start:	mov	ecx, 0 	; хранение результата
	mov	ebx, 0	; временный регистр
	PRINT	"Hello write text:"	
again:	GETCHAR
	cmp	ax, 0	; если конец файла то заканчиваем считывание
	jl	end	; знаковое если меньше
	inc	ecx	; увеличиваем счётчик символов на 1
	jmp	again	; возвращаемся к получению символа
end:	PUTCHAR	10	; переводим строку
	PRINT	"Count symbols:"
	mov	eax, ecx; переносим количество символов для деления
	;PUTCHAR	al	; попробуем напечатать количество Верно количество совпадает
	mov 	ecx, 0	; счетчик массива
	mov 	edi, buf
getdec:	cdq		; расширяем делеимое
	mov	ebx, [xten]
	div	ebx	; делим на 10	
	add	dx, '0'	; получаем символ числа	
	mov	[edi], dx	; сохраняем символ остатка в массив
	inc	edi	; увеличиваем адрес
	inc	ecx	; увеличиваем счетчик
	; mov	ecx, eax; переносим частное на хранение
	cmp	eax, 0	; если остаток от деления равен нулю то завершаем
	je	print
	jmp	getdec	; если нет то повторяем цикл
print:	dec	ecx	; уменьшаем ecx
	mov	edi, buf ; адрес массива в EDI
	add	edi, ecx ; переходим в конец массива
	mov	ax, [edi] ; получаем симаол из массива
	PUTCHAR	al	; печатаем символ
	cmp	ecx, 0
	jl	final
	jmp 	print
final:	PUTCHAR	10
	FINISH

