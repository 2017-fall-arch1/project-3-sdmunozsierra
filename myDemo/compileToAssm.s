;; Play a dummy Notes
		.data
count:.word 0; count = 0

		.text
JT:	.word case0
		.word case1
		.word case2
		.word case3
		.word case4

		.global startSong
startSong:
		mov &count, r12
		add #1, r12
		mov JT(r12), r0

case0:
		mov #300, r12
		call #buzzer_set_period; buzzer_set_period(300)
		mov #900, r12
		call #buzzer_set_period; buzzer_set_period(900)
		add #1, &count; count++
		jmp break

case1:
		mov #400, r12
		call #buzzer_set_period; buzzer_set_period(400)
		mov #800, r12
		call #buzzer_set_period; buzzer_set_period(800)
		add #1, &count; count++
		jmp break
case2:
		mov #500, r12
		call #buzzer_set_period ;buzzer_set_period(500)
		mov #700, r12
		call #buzzer_set_period ;buzzer_set_period(700)
		add #1, &count	;count++
		jmp break
case3:
		mov #600, r12
		call #buzzer_set_period ;buzzer_set_period(600)
		mov #500, r12
		call #buzzer_set_period ;buzzer_set_period(500)
		add #1, &count	;count++
		jmp break

case4:
		mov #700, r12
		call #buzzer_set_period ;buzzer_set_period(700)
		mov #400, r12
		call #buzzer_set_period ;buzzer_set_period(400)
		add #1, &count	;count++
		jmp break
break:
		pop r0
