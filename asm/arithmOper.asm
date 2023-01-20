; 1) Реализовать подпрограммы перевода строки в число 
;    и арифметической операции
; 2) Сделать работу программы как с целочисленными переменными 
;    так и с вещественными
; 3) Начало подпрограммы и конец надо сделать в макросе
;    (может в заранее скомпилированном)
; 4) выложить исходники в гит

; CDECL имеем право портить EAX, EDX, ECX
;  все остальные регистры либо не трогать или сохранять и 
;  восстанавливать перед выходом из функции
; %include "stud_io.inc" ; будем использовать только написанное
%include "sysmacro.inc"

global	_start
; Инициализированные данные
section .data
xone dd 1
xten dd 10
fone dd 1.0
ften dd 10.0
msgPutArithm db	'Enter an arithmetic expression:', 0
msgIsInt  db 	'Receive int value', 0
msgIsFloat db	'Receive float value', 0
msgErrArifm db  'Error of arifmecit operation', 0
msgErrStrToInt db	'Error of convert string to integer', 0
msgErrFloatOper db 	'Error of float operation', 0
msgPrintInt db	'Print integer:', 0
msgAnswer db 	'Answer:', 0
msgThis db	'This', 0

valFloat1 dd 43.123
valFloat2 dd 1.3
valFloat10k dd 10000.0
; Не инициализированные данные
section .bss
buf resb 512
buflen equ $-buf
strVal resb 128
strValLen equ $-strVal

section	.text
_start:	
	;pcall printFloat, valFloat1
	pcall writeString, msgPutArithm
; считываем строку
	push dword buflen
	push dword buf		; кладем параметр в стек
	call getString		; вызваем подпрограмму
	add esp, 8		; очищаем стек от параметров
; передаем строку на обработку
	push dword buf
	call arithm
	add esp, 4
; Конец
	call quit

; печать символа перевода строки
writeLN:
	sub esp, 4 		; выделяем память под символ
	mov edx, esp		; получаем адрес памяти
	mov al, 10		; заносим число в регистр
	mov [edx], al		; заносим число из регистра в память
	kernel 4, 1, edx, 1	; системный вызов вывода в поток 
	add esp, 4		;  стандартного вывода
	ret			; завершаем

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
	call writeLN
	mov esp, ebp
	pop ebp
	ret

; подпрограмма для перевода числа в строку
; получает: число(arg1) и адрес буфера строки(arg2)
; длина буфера должна быть не меньше 128 байт
intToStr:
	push ebp
	mov ebp, esp
	push esi
	push edi
	push ebx
	mov edi, [arg2]		; адрес текущей позиции в строке
	add edi, 64		; оставим 64 символа чтобы 
				; инвертировать полученную строку 
				; с конца в начало
	mov eax, [arg1] 	; переносим делимое в eax
	mov ecx, 0		; счетчик количесва символов в строке
.again:	cdq			; расширяем делимое eax
	mov ebx, 10		; заносим 10
	div ebx			; делим на 10
	add dl, '0'		; получаем символ числа
	mov [edi], dl		; сохраняем символ остатка в массив
	inc edi			; увеличиваем адрес	
	inc ecx
	cmp eax, 0		; если ноль то строка закончилась
	je .end			;
	jmp .again		; если больше 0 то делим дальше
.end:	mov eax, [arg2]		; в начало стрики будем кидать конец
.invert:dec ecx
	dec edi			; начинаем с конца
	mov dl, [edi]		; получаем символ	
	mov [eax], dl		; помещаем его в ответ
	inc eax
	cmp ecx, 0		; если строка не кончилась
	jne .invert		; то продолжаем перенос
	mov bl, 0
	mov [eax], bl		; помечаем конец строки и выходим
.quit:	pop ebx
	pop edi
	pop esi
	mov esp, ebp
	pop ebp
	ret

; Печатаем число в буфер вывода
; arg1 - целое число
printInt:
	push ebp
	mov ebp, esp
; переводим число в строку	
	pcall intToStr, [arg1], strVal
; печатаем строку
	pcall writeString, msgPrintInt 
	pcall writeString, strVal
.quit:	mov esp, ebp
	pop ebp
	ret

; ставим точку в конец строки
; arg1 - строка
pushPointToEnd:
	push ebp
	mov ebp, esp
	push edi
	mov edi, [arg1]
	mov al, 0
.again:	cmp [edi], al
	je .end
	inc edi
	jmp .again
.end:	mov al, '.'
	mov [edi], al
	inc edi
	mov al, 0
	mov [edi], al
.quit:	pop edi
	mov esp, ebp
	pop ebp
	ret

; преобразует тип double в строку
; arg1(8 байт) - один double записанный во две 4 байтные 
; ячейки, arg3 - адрес строки на вывод
floatToStr:
	push ebp
	mov ebp, esp
	sub esp, 4 		; выделяем память под локальную переменную
	push esi
	push edi
	push ebx
	mov edi, [arg2]		; адрес текущей позиции в строке
	fld dword [arg1] 	; переносим делимое в ST
	fist dword [local1]	; преобразуем из ST0 в знаковое целое
	pcall intToStr, [local1], edi	; преобразуем целую часть в строку
; поставим точку в конец
	pcall pushPointToEnd, edi
; преобразуем 5 знаков после запятой в строку
	; найти конец строки и передать его функции
	mov al, 0
.again:	cmp [edi], al
	je .findEnd
	inc edi
	jmp .again
.findEnd:
	fild dword [local1]	; заносим в стек сопроцесоора целое число
				;  до запятой
	fsub			; оставляем только после запятой
	fmul dword [valFloat10k]	; умножаем на 10000 выделяем вещественную часть числа
	fist dword [local1]	
	pcall intToStr, [local1], edi 	; печатаем в строку
.quit:	mov esp, ebp
	pop ebp
	ret

; Печатаем число с плавающей точкой из буфера
; arg1 - число с плавающей точкой
printFloat:
	push ebp
	mov ebp, esp
; переводим число в строку
	pcall floatToStr, [arg1], strVal
	pcall writeString, strVal
.quit: 	mov esp, ebp
	pop ebp
	ret

; подпрограмма перевода строки в целое число
; получает: строку(arg1)
; выдает в eax успешность(ответ количество прочитанных символов числа) 
;  операции, в буфере число, 0 - мусор
; edx - результат
strToInt:
	push ebp
	mov ebp, esp
	sub esp, 4		; буфер под локальную переменную
				; состояния преобразования строки	
	push ebx		; сохраняем для умножения
	push edi
	mov [local1], dword 0	; по умолчанию ставим что в локальной
				; находится мусор
	xor eax, eax		; обнуляем число для вывода результата
	mov edi, [arg1]		; адрес текущей позиции в строке
	xor edx, edx
.again:	xor ebx, ebx	
	mov bl, [edi]		; получаем символ
	cmp bl, '0'		; если меньше нуля или больше 9 то
	jb .end			; выдаем результат
	cmp bl, '9'		; заканчиваем считать
	ja .end
	mov [local1], dword 1	; если нашли символ числа то говорим
				; что на выходе должен быть результат
	sub bl, '0'		; получаем цифру в десятичном виде
	mul dword [xten] 	; умножаем на 10 предыдущее число
	jc .error		; ПРОВЕРИТЬ работу, если произошло переполнение
				;  то выходим и выводим ошибку
	add eax, ebx		; и складываем с полученным
	jc .error
	inc edi			; возвращаемся для дальнейшей обработки
	jmp .again	
.end:	mov edx, eax
	jmp .quit

.error: mov [local1], dword 0	; заносим ошибку в локальную переменную
	pcall writeString, msgErrStrToInt
.quit:	mov eax, [local1]	; в eax выводим количество прочитанных 
				; символов
	pop edi
	pop ebx			; восстанавливаем значения и выходим
	mov esp, ebp
	pop ebp	
	ret

; преобразуем строку значение типа float
; получает: строку(arg1)
; выдает в eax успешность(ответ количество прочитанных символов числа) 
;  операции, в буфере число, 0 - мусор
; edx - результат
strToFloat:
	push ebp
	mov ebp, esp
	sub esp, 8 
	push ebx
	pcall strToInt, [arg1]	; заносим в сопроцессор
	cmp eax, 0		; значение числа 
	je .error		; до
	mov [local1], edx
	fild dword [local1]		; запятой
	; находим позицию точки до знака операции или конца строки
	mov eax, [arg1]
.again:	mov bl, '+'	; если 
	cmp [eax], bl	; найден знак
	je .end		; операции 
	mov bl, '-'	; или 
	cmp [eax], bl	; конец
	je .end		; то завершаем
	mov bl, '*'	
	cmp [eax], bl
	je .end
	mov bl, 0
	cmp [eax], bl
	je .end
	mov bl, '.'	; если 
	cmp [eax], bl	; найдена точка
	je .point_find	; или запятая
	mov bl, ','	; то проебразуем дробную часть
	cmp [eax], bl	;
	je .point_find	;
	inc eax
	jmp .again
.point_find:
	inc eax
	pcall strToInt, eax	; пребразуем дробную часть
	cmp eax, 0
	je .error
	mov [local2], edx
	fild dword [local2]	; заносим дробную часть в сопроцессор
	cmp edx, 1		; если получили 1 то пропускаем цикл деления 
	je .end_one		; (непонятная ошибка со сравнением)
	cmp edx, 0		; если ноль то результат получен 
	je .end_div		; пропускаем
.divz:
	fidiv dword [xten]	; если нет то делим на 10 и повторяем
	fcom dword [fone]	; сравниваем
	fstsw ax		; заносим флаги в AX
	sahf			;  и оттуда - во FLAGS
	jae .divz		; если 1 больше то мы перевели в дробную
	;mov dword [local1], 10
	;jmp .divz
.end_one:
	fdiv dword [ften]
.end_div:
	fiadd dword [local1]		; складаваем дробную и целую часть
.end:
	fstp dword [local1]
	mov edx, [local1]
	jmp .quit
.error:
.quit:	pop edi
	mov esp, ebp
	pop ebp
	ret
; определяем что содержится в строке целое число или вещественное
; если целое то выдаем в eax 0 если вещественное то 1
; на вход волучаем адрес строки
typeArithmStr:
	push ebp
	mov ebp, esp
	push edi
	mov edi, [arg1] 	; получаем адрес строки полученной для 
				; анализа
	xor edx, edx		; хранится тип полученного числа по умолчанию целое
.again: mov al, [edi]
	cmp al, 0		; если конец 
	je .quit		;  завершаем
	cmp al, '.'		; если найдена точки или 
	je .isFlt		;  запятая
	cmp al, ','		;  то получено вещественное число
	je .isFlt		;  и выдаем 1 на выход
	inc edi	
	jmp .again
.isFlt	mov edx, 1
	jmp .quit
.quit:	mov edi, edx		; сохраняем тип числа
	cmp edx, 1		; если вещественно число
	je .printFloat		; то печатаем сообщнеие что число вещественное
	push dword msgIsInt	; иначе печатаем сообщение что
	jmp .printMsgType	; число целое
.printFloat:
	push dword msgIsFloat
.printMsgType:
	call writeString	; вызываем печать
	add esp, 4
	mov eax, edi		; восстанвливаем
	pop edi
	mov esp, ebp
	pop ebp
	ret

; тип операции сложение вычитание умножение деление
; получаем адрес строки arg1
; выдаем если '+' - 1, '-' - 2, '*' - 3, '/' - 4
; если знак не определен то выдаем 0 в eax
; в edx выдадим номер позиции символа
strToTypeOper:
	push ebp
	mov ebp, esp
	push edi
	mov edi, [arg1] 	; получаем адрес строки полученной для 
				; анализа
	xor edx, edx		; хранится тип полученного числа по умолчанию целое
.again: mov al, [edi]
	cmp al, 0		; если конец 
	je .quit		;  завершаем
	cmp al, '+'		; найдем тип
	je .if_plus		; операции
	cmp al, '-'		; которую необходимо выполнить
	je .if_minus
	cmp al, '*'
	je .if_mul
	cmp al, '/'
	je .if_div
	inc edi
	jmp .again
.if_plus:
	mov edx, 1
	jmp .quit
.if_minus:
	mov edx, 2
	jmp .quit
.if_mul:
	mov edx, 3
	jmp .quit
.if_div:
	mov edx, 4
	jmp .quit
.quit:	mov eax, edx		; сохраняем результат в eax 
	mov edx, edi		; сохраняем в edx позицию знака в
	sub edx, [arg1]		;  строке и выводим результат
	pop edi			; восстанавливаем регистры и покидаем
	mov esp, ebp		; функцию
	pop ebp
	ret

; Арифметическая операция над целыми числами
operInt:
	push ebp
	mov ebp, esp
	sub esp, 24 		; выделяем память под локальные переменные
	push edi		; сохраняем edi т.к. будем его изменять
; получаем тип операции сложение вычитание умножение деление
	pcall strToTypeOper, [arg1]
	mov [local1], eax	; сохраняем тип операции в
	mov [local2], edx	; сохраняем номер позиции знака
	cmp eax, 0		; обработка ошибки 
	je .error
; преобразуем первое число из текста в байт код
	pcall strToInt, [arg1]
	cmp eax, 0		; обработка ошибки
	je .error
	mov [local3], edx	; сохраняем результат в стек
; преобразуем второе число из текста в байт код
	mov eax, [arg1]		; получим адрес второго числа
	add eax, [local2]	; и передадим его подпрограмме
	inc eax			; пропускаем знак операции
	pcall strToInt, eax
	cmp eax, 0
	je .error
	mov [local(4)], edx

	mov eax, [local1]
	cmp eax, 1
	jne .not_plus
; код для плюса
	mov eax, [local(3)]	; получаем первое число 
	add eax, [local(4)]	; складываем оба числа
	jmp .ansToStr 
.not_plus:
	cmp eax, 2
	jne .not_minus
; код для минуса
	mov eax, [local(3)]	; переносим первое числа в eax
	sub eax, [local(4)]	; вычитаем из первого второе
	jmp .ansToStr
.not_minus:
	cmp eax, 3
	jne .not_mul
; код для умножения
	mov eax, [local(3)]
	mul dword [local(4)]
	jmp .ansToStr
.not_mul:
	cmp eax, 4
	jne .not_div
; код для деления
	mov eax, [local(3)]
	mov edx, 0
	div dword [local(4)]
	jmp .ansToStr
.not_div:
; ничего не найдено необходимо выдать ошибку и выйти
	jmp .print_ans
.ansToStr:
	pcall intToStr, eax, buf
.print_ans:
	pcall writeString, msgAnswer
	pcall writeString, buf
	jmp .quit
.error:
	pcall writeString, msgErrArifm
.quit:
	pop edi
	mov esp, ebp
	pop ebp
	ret

; Арифметическия операция над числами с плавающей точкой
operFloat:
	push ebp
	mov ebp, esp
	sub esp, 64 		; выделяем память под локальные переменные
	push edi		; сохраняем edi т.к. будем его изменять
; получаем тип операции сложение вычитание умножение деление
	pcall strToTypeOper, [arg1]
	mov [local1], eax	; сохраняем тип операции в
	mov [local2], edx	; сохраняем номер позиции знака
	cmp eax, 0		; обработка ошибки 
	je .error
; преобразуем первое число из текста в байт код
	pcall strToFloat, [arg1]
	cmp eax, 0		; обработка ошибки
	je .error
	mov [local(3)], edx	; сохраняем результат в стек
	pcall printFloat, [local(3)]
	;pcall writeString, msgThis
	;pcall printFloat, [local(3)] 
	;pcall writeString, msgThis
; преобразуем второе число из текста в байт код
	mov eax, [arg1]		; получим адрес второго числа
	add eax, [local2]	; и передадим его подпрограмме
	inc eax			; пропускаем знак операции
	pcall strToFloat, eax
	cmp eax, 0
	je .error
	mov [local(4)], edx
	pcall printFloat, [local(4)]
	fld dword [local(3)]	; получаем первое число 
	mov eax, [local1]
	cmp eax, 1
	jne .not_plus
; код для плюса
	fadd dword [local(4)]	; складываем оба числа
	jmp .ansToStr 
.not_plus:
	cmp eax, 2
	jne .not_minus
; код для минуса
	fsub dword [local(4)]	; вычитаем из первого второе
	jmp .ansToStr
.not_minus:
	cmp eax, 3
	jne .not_mul
; код для умножения
	fmul dword [local(4)]
	jmp .ansToStr
.not_mul:
	cmp eax, 4
	jne .not_div
; код для деления
	pcall writeString, msgThis
	fdiv dword [local(4)]
	jmp .ansToStr
.not_div:
.error:
	pcall writeString, msgErrFloatOper
	jmp .quit
.ansToStr:	
	fst dword [local(3)]	; выталкиваем результат в память
	pcall writeString, msgAnswer
	pcall printFloat, [local(3)] ; печатаем результат
.quit:
	pop edi
	mov esp, ebp
	pop ebp
	ret

; получаем на вход адрес строки выполняем арифметическое действие в выводим
; arg1 адрес строки 
arithm:			; Начало подпрограммы арифметического
			; действия на вход должна подаваться строка
			; на выходе получаем результат
	push ebp	; организуем стековый фрейм
	mov ebp, esp	; заносим адрес возврата в ebp т.к esp будет
			;  использоваться в подпрограмме
	sub esp, 12	; локальная переменная I будет по адресу 
			; [ebp-4]
	push esi	; Сохраняем регистры ESI и EDI (EAX всё равно 
	push edi	; изменится)
; Сначала определяем тип чисел поступивших на вход
	pcall typeArithmStr, [arg1]
	mov [local1], eax	; сохраняем тип чисел в esi
; получаем тип операции сложение вычитание умножение деление
	pcall strToTypeOper, [arg1]
	mov [local2], eax	; сохраняем тип операции в edi
	mov [local3], edx	; сохраняем номер позиции знака
; если число целое то производим операцию и получаем строковый ответ
	mov eax, [local1]
	cmp eax, 0
	jne .float_oper
	pcall operInt, [arg1]	; запускаем расчет целых
	jmp .quit
; если вещественные то над вещественными
.float_oper:
	pcall operFloat, [arg1]	; запускаем расчет вещественного числа
; выводим результат
.quit:	pop edi
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
