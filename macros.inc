.macro config_geral
	ldi	R16, 0b11111111 ; Set PDx as Output
	out	DDRD, R16

	ldi	R16, 0b00000000 ; Enable pull-up resistor
	out	PORTD, R16

	LDI R16, 0b11100111    ; 0x is to indicate that the following number is in Hexa
	OUT DDRB, R16    ; Set the DDR Register

	LDI R16, 0b00011000
	OUT PORTB, R16

	ldi	R16, 0b10111111 ; Set PDx as Output
	out	DDRC, R16

	ldi	R16, 0b01000000 ; Enable pull-up resistor
	out	PORTC, R16

	nop
.endm

.macro display_sem_ponto
	mov     R16, @0
	call    pega_tabela
	lds     R16, PORTB
	andi    R16, 0xC0
	out     PORTB, R16
	out     PORTD, R17
	sbi     PORTB,@1
.endm

.macro display_com_ponto
	mov     R16, @0
	call    pega_tabela
	ori		R17, 0b10000000
	lds     R16, PORTB
	andi    R16, 0xC0
	out     PORTB, R16
	out     PORTD, R17
	sbi     PORTB, @1
.endm

.macro mostra_resultado_display
	cpi R20, 10
	brsh div ;se for maior que 10, divide em dezena e unidade
	rjmp not_div
	
	div: ;divide
		ldi R19, 10
		sub R20, R19 ;R20 - D2
		ldi R22, 1 ;R22 - D1
		rjmp config

	not_div: ;nao divide
		ldi R22, 0 ;R22 - D1
	
	config:
		config_geral
		pause_i_ms 250
	
	loop_display:
		push R16
		display_sem_ponto R22, 2
		pause_i_ms 5
		pop R16

		push R16
		display_com_ponto R20, 1
		pause_i_ms 5
		pop R16

		push R16
		display_sem_ponto R21, 0
		pause_i_ms 5
		pop R16

		sbis PINB,3
		rjmp endmacro
		rjmp loop_display

	endmacro:
		pause_i_ms 5
		cbi	PORTB,0
		cbi PORTB,5
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
		
.endm

.macro mostra_resultado_botao
	cpi R20, 10
	brsh div ;se for maior que 10, divide em dezena e unidade
	rjmp not_div
	
	div: ;divide
		ldi R19, 10
		sub R20, R19
		ldi R22, 1
		rjmp config

	not_div: ;nao divide
		ldi R22, 0

	config:
		config_geral
		pause_i_ms 250
		cpi R22, 0
		breq loop_2

	loop_sinal:
		cpi R30,1
		breq sinal
		rjmp loop_1
	sinal:
		ldi R16, 0b00100000
		out PORTB, R16
	loop_1:
		sbis PINB, 3
		rjmp loop_2
		out PORTC, R22 ;primeiro inteiro
		rjmp loop_1
	loop_2:
		sbis PINB, 3
		rjmp loop_3
		out PORTC, R20 ;segundo inteiro
		rjmp loop_2
	loop_3:
		sbis PINB, 4
		rjmp fim
		ori R21, 0b00010000
		out PORTC, R21 ;decimal com virgula acesa
		rjmp loop_3
	fim:
		ldi R16, 0b00000000
		out PORTC, R16
		ldi R16, 0b00100000
		out PORTC, R16
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
.endm

.macro mostra_resultado_temp
	cpi R20, 10
	brsh div ;se for maior que 10, divide em dezena e unidade
	rjmp not_div
	
	div: ;divide
		ldi R19, 10
		sub R20, R19
		ldi R22, 1
		rjmp config

	not_div: ;nao divide
		ldi R22, 0

	config:
		config_geral
		pause_i_ms 250

	loop_sinal:
		cpi R30,1
		breq sinal
		rjmp leds
	sinal:
		ldi R16, 0b00100000
		out PORTB, R16
		pause_i_ms 250
		pause_i_ms 250
	leds:
		out PORTC, R22 ;primeiro inteiro
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250

		out PORTC, R20 ;segundo inteiro
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250

		ldi R23, 0b00010000
		out PORTC, R23 ;virgula
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250

		ori R21, 0b00010000
		out PORTC, R21 ;decimal com virgula acesa
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
	fim:
		ldi R16, 0b00000000
		out PORTB, R16
		ldi R16, 0b00100000
		out PORTC, R16
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
.endm
