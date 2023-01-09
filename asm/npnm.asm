%include "stud_io.inc"
global	_start

section	.text
_start:	mov	cx, 0 	; для знака минус
	mov	bx, 0	; для знака плюс
	PRINT	"Hello print symbols:"	
again:	GETCHAR
	cmp	al, -1
	je	end
	cmp	al, 45	; сравниваем со знаком минус
	je	minus
	cmp	al, 43	; сравниваем со знаком плюс
	je	plus
	jmp	again	; если нмчего не найдено запрашиваем символ
minus:	inc	cx
	jmp	again
plus:	inc	bx
	jmp	again
end:	PUTCHAR	10	; переводим строку
	mov	eax, 0
	mov	ax, bx	; умножаем полученное количество звезл
	mul	cx
	mov	ecx, eax
	cmp	ecx, 0	; если звезд которые надо печатать ноль то выходим
	je	final
star:	PUTCHAR 42	; печатаем звездочки пока не закончатся
	loop 	star
final:	PUTCHAR	10
	FINISH

