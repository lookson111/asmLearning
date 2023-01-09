%include "stud_io.inc"
global	_start

section .data
xten dw 10

section	.text
_start:	mov	cx, 0 	; хранение результата
	mov	bx, 0	; временный регистр
	PRINT	"Hello write dec number:"	
again:	GETCHAR
	cmp	ax, '0'
	jl	end	; знаковое если меньше
	cmp	ax, '9'	; 
	jg	end	; знаковое если больше
	sub	ax, '0'	; получаем из символа число
	mov	ebx, eax; переносим полученно число во временный
			; регистр
	mov	eax, ecx; переносим наш результат в ax для умножения
	mov	cx, 10	; переносим 10 для умножения
	mul	cx	; умножаем на 10 наш результат
	add	eax, ebx; складываем итог с введенным числом
	mov 	ecx, eax; полученный результат переносим в место хранения
	jmp	again	; возвращаемся к получению символа
end:	PUTCHAR	10	; переводим строку
	cmp	ecx, 0	; если звезд которые надо печатать ноль то выходим
	je	final
star:	PUTCHAR '*'	; печатаем звездочки пока не закончатся
	loop 	star
final:	PUTCHAR	10
	FINISH

