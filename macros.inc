rjmp start_macros
.include "delays.inc"

start_macros:

.macro arruma_expoente ; são enviados registradores com mantiças e com expoentes
	; @0 e @1 apresentam os expoentes
	;@2 e @3 apresentam as mantiças
	; @0 - @2			@1- @3

	cp @0,@1
	brlo altera_exp_0
	cp @1, @0
	brlo altera_exp_1
	rjmp continua_calculo

	altera_exp_0: ; @0 valor tem expoente menor
		sub @1,@0 ; pega a diferença entre eles e taca em @1
		add @0,@1 ; adiciona a diferença pra @0 ficar igual

	ajusta_registrador_0:
		lsr @2 ; faz shift pra direita
		dec @1
		cpi @1, 0
		brne ajusta_registrador_0

		mov @1,@0 ; coloca o valor certo em @1 novamente depois de ajustar
		rjmp continua_calculo

	altera_exp_1: ; @1 valor tem expoente menor

		sub @0,@1 ; pega a diferença entre eles e taca em @0
		add @1,@0 ; adiciona a diferença pra @1 ficar igual

	ajusta_registrador_1:

		lsr @3 ; faz shift pra direita
		dec @0
		cpi @0, 0
		brne ajusta_registrador_1

		mov @0,@1 ; coloca o valor certo em @0 novamente depois de ajustar

	continua_calculo:
	
.endm

.macro decimal_correspondente
		PUSH R24
		PUSH R25
		PUSH R30
		mov R25, @0
		ldi R23, 0
		cpi R25, 3 ; valor >= 8
		breq busca_valor_decimal_3_mantissas
		cpi R25, 2 ; valor >= 4
		breq busca_valor_decimal_4_mantissas
		cpi R25, 1 ; valor >= 2
		breq busca_valor_decimal_5_mantissas
busca_valor_decimal_6_mantissas:
		push R23
		call tabela_1
		cp R23, R21
		breq seta_decimal
		mov R24, R21
		cp R24, R23
		brsh valor_aproximado_6_mantissas
		pop R23 ; se chegou aqui já pegou o valor mais proximo
		rjmp seta_decimal_aproximado
valor_aproximado_6_mantissas:
		POP R23
		mov R30, R23
		inc R23
		rjmp busca_valor_decimal_6_mantissas
busca_valor_decimal_5_mantissas:
		push R23
		call tabela_menor_4
		cp R23, R21
		breq seta_decimal
		mov R24, R21
		cp R24, R23
		brsh valor_aproximado_5_mantissas
		pop R23 ; se chegou aqui já pegou o valor mais proximo
		rjmp seta_decimal_aproximado
valor_aproximado_5_mantissas:
		POP R23
		mov R30, R23
		inc R23
		rjmp busca_valor_decimal_5_mantissas
busca_valor_decimal_4_mantissas:
		push R23
		call tabela_entre_4_e_7
		cp R23, R21
		breq seta_decimal
		mov R24, R21
		cp R24, R23
		brsh valor_aproximado_4_mantissas
		pop R23 ; se chegou aqui já pegou o valor mais proximo
		rjmp seta_decimal_aproximado
valor_aproximado_4_mantissas:
		POP R23
		mov R30, R23
		inc R23
		rjmp busca_valor_decimal_4_mantissas
busca_valor_decimal_3_mantissas:
		push R23
		call tabela_maior_que_7
		cp R23, R21
		breq seta_decimal
		mov R24, R21
		cp R24, R23
		brsh valor_aproximado_3_mantissas
		pop R23 ; se chegou aqui já pegou o valor mais proximo
		rjmp seta_decimal_aproximado
valor_aproximado_3_mantissas:
		POP R23
		mov R30, R23
		inc R23
		rjmp busca_valor_decimal_3_mantissas
seta_decimal_aproximado:
		mov R21, R23
		rjmp fim_separacao
seta_decimal:
		POP R23
		mov R21, R23
fim_separacao:
		POP R30
		POP R25
		POP R24
.endm

.macro separa_inteiro_decimal
	; @0 -> expoente @1 -> Valor resultado
	; R20 -> tem a parte inteira 
	; R21 -> tem a parte decimal
	PUSH R31
	mov R31, @0
	cpi R31, 0
	brne comeca_separar
	POP R31
	ldi R20, 0
	ldi R21, 0
	rjmp final_separa
comeca_separar:
	POP R31
	PUSH R30
	PUSH R23
	PUSH @0
	ldi R18, 6 ; 8 - 2 -> número de bits - posições fora da mantissa 00, ou 01,
	ldi R23, 3 
	sub @0, R23 ; subtrai o bias de 3
	sub R18, @0 ; subtrai o expoente, o resultado é quantos shifts serão feitos pra obter o decimal
	PUSH @1
	separa_inteiro:
		lsr @1
		dec R18
		cpi R18, 0
		brne separa_inteiro
		mov R20, @1
		POP @1
		PUSH @1
		ldi R18, 2
		add R18, @0 ; soma 2 ao expoente
	separa_decimal:
		lsl @1
		dec R18
		cpi R18, 0
		brne separa_decimal

		ldi R18,2
		add R18,@0
	ajusta_decimal:
		lsr @1
		dec R18
		cpi R18, 0
		brne ajusta_decimal
		mov R21, @1 ; decimal está em R21
		POP @1

		decimal_correspondente @0 ;decimal ajustado está em R21

		POP @0
		POP R23
		POP R30
final_separa:
.endm



tabela_3_mantissas:
    .db 0x0, 0x0, 0x1, 0x2, 0x3, 0x4, 0x4, 0x5, 0x6, 0x7

tabela_4_mantissas:
    .db 0x0, 0x1, 0x3, 0x4, 0x6, 0x8, 0x9, 0xB, 0xC, 0xE

tabela_5_mantissas:
    .db 0x0, 0x3, 0x6, 0x9, 0xC, 0x10, 0x13, 0x16, 0x19, 0x1C

tabela_6_mantissas:
    .db 0x0, 0x6, 0xC, 0x13, 0x19, 0x20, 0x26, 0x2C, 0x33, 0x39

tabela_1:
	; Precisa multiplicar por 2 pra pegar as instruções individualmente e não de duas 
	; LOW são os 8 bits menos significativos
	ldi ZL,LOW(tabela_6_mantissas*2) 
	; HIGH são os 8 bits mais significativos
	ldi ZH,HIGH(tabela_6_mantissas*2) 

	add ZL, R23
	ldi R23, 0
	; adc soma os valores MAIS o valor na flag carry
	adc ZH, R23

	; lpm pega da memória
	lpm R23, Z ; Z = ZL e ZH
	ret

tabela_menor_4:
	; Precisa multiplicar por 2 pra pegar as instruções individualmente e não de duas 
	; LOW são os 8 bits menos significativos
	ldi ZL,LOW(tabela_5_mantissas*2) 
	; HIGH são os 8 bits mais significativos
	ldi ZH,HIGH(tabela_5_mantissas*2) 

	add ZL, R23
	ldi R23, 0
	; adc soma os valores MAIS o valor na flag carry
	adc ZH, R23

	; lpm pega da memória
	lpm R23, Z ; Z = ZL e ZH
	ret

tabela_entre_4_e_7:
	; Precisa multiplicar por 2 pra pegar as instruções individualmente e não de duas 
	; LOW são os 8 bits menos significativos
	ldi ZL,LOW(tabela_4_mantissas*2) 
	; HIGH são os 8 bits mais significativos
	ldi ZH,HIGH(tabela_4_mantissas*2) 

	add ZL, R23
	ldi R23, 0
	; adc soma os valores MAIS o valor na flag carry
	adc ZH, R23

	; lpm pega da memória
	lpm R23, Z ; Z = ZL e ZH
	ret

tabela_maior_que_7:
	; Precisa multiplicar por 2 pra pegar as instruções individualmente e não de duas 
	; LOW são os 8 bits menos significativos
	ldi ZL,LOW(tabela_3_mantissas*2) 
	; HIGH são os 8 bits mais significativos
	ldi ZH,HIGH(tabela_3_mantissas*2) 

	add ZL, R23
	ldi R23, 0
	; adc soma os valores MAIS o valor na flag carry
	adc ZH, R23

	; lpm pega da memória
	lpm R23, Z ; Z = ZL e ZH
	ret

; resultado em @0
; @0 -> @2    @1 -> @3
; @0 e @1 -> Expoentes @2 e @3 -> Valores
; (R16/R26) / (R17/R27)
.macro div ; dividendo SEMPRE menor
	PUSH @2
	PUSH @3
	PUSH @0
	PUSH @1
	ldi R19, 0
loop_div_v1_v2_parte_inteira:
	subtraction @0, @1, @2, @3 ; subtrai os valores e já deixa o resultado normalizado
	normaliza_valor @3, @1
	sbrc R29, 2 ; se a flag de negativo não estiver setada pular proxima instrução
	rjmp div_v1_v2_parte_decimal
	inc R19
	mov R0, @2 ; guardo quanto sobrou
	mov R1, @0 ; quanto expoente de quanto sobrou
	rjmp loop_div_v1_v2_parte_inteira
div_v1_v2_parte_decimal: ; parte inteira em R19
	POP @1
	POP @0
	POP @3
	POP @2
	PUSH @2
	PUSH @3
	PUSH R19
	ldi R30, 0 ; passo o resto pra R30 -> só tem resto se for as duas inteiras
	PUSH R30
	separa_inteiro_decimal R1, R0 ; pego a parte inteira e decimal do valor que sobrou
	cpi R20, 0
	breq conta_e_exata
	rjmp conta_nao_exata
conta_e_exata:
	mov R28, R20
	rjmp transforma_pra_iee
conta_nao_exata:
	POP R30
	mov R30, R20 ; passo o resto pra R30
	PUSH R30
	PUSH R20
	PUSH R21
	separa_inteiro_decimal @0, @2
	cpi R21, 0 ; verifica parte decimal
	in R29, SREG
	POP R21
	POP R20
	sbrs R29, 1 ; se a flag Zero estiver setada pula a proxima instrução
	rjmp conta_continua

	PUSH R20
	PUSH R21
	separa_inteiro_decimal @1, @3
	cpi R21, 0 ; verifica parte decimal
	in R29, SREG
	POP R21
	POP R20
	sbrs R29, 1 ; se a flag Zero estiver setada pula a proxima instrução
	rjmp conta_continua

	; os dois valores são decimais, logo a conta já acabou
	rjmp transforma_pra_iee

conta_continua:
	POP R30
	ldi R30, 0
	PUSH R30
	ldi R19, 10
	mul R20, R19 ; multiplico a parte inteira por 10
	mov R19, R0 ; passo o resultado pra R4
	add R19, R21 ; somo a parte decimal do resto
	separa_inteiro_decimal @1, @3
	ldi R31, 0
loop_div_v1_v2_parte_decimal:
	sub R19, R20
	in R29, SREG
	sbrc R29, 2 ; se a flag de negativo não estiver setada pular proxima instrução
	rjmp div_v1_v2_parte_decimal_parte_final
	cpi R31, 9
	breq seta_limite
	inc R31
	mov R23, R19 ; guardo quanto sobrou
	rjmp loop_div_v1_v2_parte_decimal
seta_limite:
	rjmp terminou_decimal
div_v1_v2_parte_decimal_parte_final:
	cp R23, R20
	brlo terminou_decimal
	cpi R21, 2
	brlo terminou_decimal
	ldi R19, 10
	mul R23, R19 ; multiplico a parte inteira por 10
	mov R23, R0
	ldi R28, 0
loop_div_v1_v2_parte_decimal_final:
	sub R23, R21
	in R29, SREG
	sbrc R29, 2 ; se a flag de negativo não estiver setada pular proxima instrução
	rjmp transforma_pra_iee
	inc R28
	rjmp loop_div_v1_v2_parte_decimal_final
terminou_decimal:
	mov R28, R31
transforma_pra_iee: ; PARTE INTEIRA em R19 e PARTE DECIMAL em R28
	POP R30
	POP R19
	POP @3
	POP @2
	PUSH R30
	passa_pra_iee R19, R28 ; vai retornar o resultado em R17 e R27
	POP R30
	mov R16, R17
	mov R26, R27
.endm

.macro mult_velha
	separa_inteiro_decimal @0, @2
	mov R23, R20 ; parte inteira do 1 valor
	mov R24, R21 ; parte decimal do 1 valor
	separa_inteiro_decimal @1, @3
	cpi R21, 0
	breq primeiro_inteiro
	rjmp segundo_tipo_mult
primeiro_inteiro:
	mov R25, @2
	mov R28, @0
	dec R20
	cpi R20, 0
	in R29, SREG
	sbrc R29, 1 ; se a flag Zero não estiver setada pula a proxima instrução
	rjmp final_mult
loop_primeiro_inteiro:
	arruma_expoente @0, R28, @2, R25
	sum @2, R25, @0
	dec R20
	cpi R20, 0
	in R29, SREG
	sbrc R29, 1 ; se a flag Zero não estiver setada pula a proxima instrução
	rjmp final_mult
	rjmp loop_primeiro_inteiro
segundo_tipo_mult:
	mov R25, @2
	mov R28, @0
	dec R21
	cpi R21, 0
	in R29, SREG
	sbrc R29, 1 ; se a flag Zero não estiver setada pula a proxima instrução
	rjmp segunda_parte
loop_segundo_tipo:
	arruma_expoente @0, R28, @2, R25
	sum @2, R25, @0
	normaliza_valor R25, R28 ; @2 expoente
	dec R21
	cpi R21, 0
	breq segunda_parte
	rjmp loop_segundo_tipo
segunda_parte:
	PUSH @0 ; guardo primeira parte da multiplicação
	PUSH @2
	mov @2, R25
	mov @0, R28
	dec R20
	cpi R20, 0
	in R29, SREG
	sbrc R29, 1 ; se a flag Zero não estiver setada pula a proxima instrução
	rjmp terceira_parte
loop_segundo_parte:
	arruma_expoente @0, R28, @2, R25
	sum @2, R25, @0
	normaliza_valor R25, R28 ; @2 expoente
	dec R20
	cpi R20, 0
	breq terceira_parte
	rjmp loop_segundo_parte
terceira_parte:
	mov R25, @2
	mov R28, @0
	POP @2
	POP @0
	separa_inteiro_decimal @0, @2
	mov @2, R20
	separa_inteiro_decimal R28, R25
	add @2, R21 ; soma parte inteira do primeiro com parte decimal do segundo
	cpi @2, 10
	brsh ajuste_terceira_parte
	rjmp final_mult_segundo_tipo
ajuste_terceira_parte:
	ldi R29, 10
	sub @2, R29 ; parte decimal do resultado
	inc R20 ; acrescenta em 1 a parte inteira do resultado
final_mult_segundo_tipo:
	passa_pra_iee R20, @2 ; vai retornar o resultado em R17 e R27
	mov @0, R27
	mov @2, R17
final_mult:
.endm

.macro sum_velha ; Resultado em @0
	add @0,@1
	in R29, SREG
	normaliza_valor @0, @2 ; @2 expoente
.endm

.macro sum ; Resultado em @0
	separa_inteiro_decimal @0, @2
	mov R24, R20
	mov R25, R21
	separa_inteiro_decimal @1, @3
	add R24, R20
	add R25, R21
	cpi R25, 10
	brsh ajusta_soma
	rjmp fim_soma
ajusta_soma:
	ldi R28, 10
	sub R25, R28
	inc R24
fim_soma:
	mov R16, R24
	mov R26, R25
	passa_pra_iee R16, R26
	mov @2, R17
	mov @0, R27
.endm

.macro subtraction_velha ; Resultado em @0
	sub @0,@1
	in R29, SREG
	normaliza_valor @0, @2 ; @2 expoente
.endm 

.macro subtraction ; Resultado em @0
	PUSH R24
	PUSH R25
	PUSH R20
	PUSH R21
	PUSH R28
	PUSH R17
	PUSH R27
	separa_inteiro_decimal @0, @2
	mov R24, R20
	mov R25, R21
	separa_inteiro_decimal @1, @3
	sub R24, R20
	in R29, SREG
	cp R25, R21
	brlo ajusta_sub
	sub R25, R21
	rjmp fim_sub
ajusta_sub:
	ldi R28, 10
	add R25, R28
	sub R25, R21
	dec R24
fim_sub:
	mov R16, R24
	mov R26, R25
	passa_pra_iee R16, R26
	mov @2, R17
	mov @0, R27
	POP R27
	POP R17
	POP R28
	POP R21
	POP R20
	POP R25
	POP R24
.endm


.macro normaliza_valor ;@0 valor @1 expoente deste valor
	PUSH R20
	PUSH R21
	PUSH R22
	sbrc @0, 7 ; bit mais significante em 1, fazer shift pra direita e aumenta em 1 seu expoente
	rjmp aumenta_expoente
	ldi R20, 6
	sbrs @0, 6 ; pula proxima instrucao se está no padrão normalizado 01,
	rjmp diminui_expoente
	rjmp fim_ajuste
aumenta_expoente:
	lsr @0
	inc @1
	rjmp fim_ajuste
diminui_expoente:
	ldi R21, 0b00100000
loop_diminui_expoente:
	dec R20
	mov R22, @0
	and R22, R21
	cpi R22, 0
	brne calcula_shifts_necessarios
	lsr R21
	cpi R20, 3
	breq fim_ajuste
	rjmp loop_diminui_expoente
calcula_shifts_necessarios:
	ldi R21, 6
	sub R21, R20
faz_shifts_necessarios:
	lsl @0
	dec @1
	dec R21
	cpi R21, 0
	brne faz_shifts_necessarios
fim_ajuste:
	POP R22
	POP R21
	POP R20
.endm

.macro passa_pra_iee ;@0 valor inteiro @1 valor decimal ; VAI passar o expoente em R27 e o valor em R17
	cpi @0, 0
	brne comeca_passar
	ldi R27, 0
	ldi R17, 0
	rjmp final_passa_pra_iee
comeca_passar:
	mov R23, @1
	cpi @0, 8 ; valor >= 8
	brsh passa_valor_decimal_3_mantissas
	cpi @0, 4 ; valor >= 4
	brsh passa_valor_decimal_4_mantissas
	cpi @0, 2 ; valor >= 2
	brsh passa_valor_decimal_5_mantissas
passa_valor_decimal_6_mantissas:
	ldi @0, 0b10000000
	ldi R24, 2
	call tabela_1
	rjmp ajusta_valor
passa_valor_decimal_5_mantissas:
	lsl @0
	lsl @0
	lsl @0
	lsl @0
	lsl @0
	lsl @0
	ldi R24, 3
	call tabela_menor_4
	rjmp ajusta_valor
passa_valor_decimal_4_mantissas:
	lsl @0
	lsl @0
	lsl @0
	lsl @0
	lsl @0
	ldi R24, 4
	call tabela_entre_4_e_7
	rjmp ajusta_valor
passa_valor_decimal_3_mantissas:
	lsl @0
	lsl @0
	lsl @0
	lsl @0
	ldi R24, 5
	call tabela_maior_que_7
	rjmp ajusta_valor
ajusta_valor: ; parte decimal em R23, inteira em @0 e expoente em R24(com bias)
	lsl R23
	or @0, R23
	mov R27, R24
	mov R17, @0
	normaliza_valor R17, R27 ; @2 expoente
final_passa_pra_iee:
.endm

.macro resto
	
	div @0, @1, @2, @3

	; RESULTADO DO RESTO EM R30, converter pra iee e depois separar inteiro e decimal

	mov R17, R30
	ldi R27, 0 ; parte decimal é zero
	
	passa_pra_iee R17, R27
	mov R26, R27
	mov R16, R17
	
.endm

;@0 -> @2    @1 -> @3
;@0 e @1 -> Expoentes @2 e @3 -> Valores
; (R16/R16) ** (R17,R27)
.macro potencia
	
    separa_inteiro_decimal @1, @3
	PUSH R20
	mov @3, R20; parte inteira
	ldi @1, 0 ; parte decimal vai ser 0, transformando ele em inteiro
	passa_pra_iee @3, @1 ; retorna mantissa em R17 e expoente em R27
	mov R25, @2
	mov R28, @0
	POP R20
	cpi R20, 0
	in R29, SREG
	sbrc R29, 1 ; se a flag Zero não estiver setada pula a proxima instrução
	rjmp fim_potencia_zero
	cpi R20, 1
	in R29, SREG
	sbrc R29, 1 ; se a flag Zero não estiver setada pula a proxima instrução
	rjmp fim_potencia
loop_potencia:
	PUSH R20
	PUSH R25
	PUSH R28
	mult @0, R28, @2, R25
	POP R28
	POP R25
	POP R20
	dec R20
	cpi R20, 1
	in R29, SREG
	sbrs R29, 1 ; se a flag Zero estiver setada pula a proxima instrução
	rjmp loop_potencia
fim_potencia_zero:
	;
fim_potencia:
	; resultado vai estar em @0 e @2
.endm 


;@0 -> @2    @1 -> @3
;@0 e @1 -> Expoentes @2 e @3 -> Valores
.macro mult ; (R16/R16) * (R17,R27)

	separa_inteiro_decimal @0, @2
	mov R23, R20 ; parte inteira do 1 valor
	mov R24, R21 ; parte decimal do 1 valor
	separa_inteiro_decimal @1, @3
	cpi R21, 0
	breq primeiro_inteiro
	rjmp segundo_tipo_mult
primeiro_inteiro:
	mul R20, R23
	mov R23, R0
	mul R20, R24
	mov R24, R0
	cpi R24, 9
	in R29, SREG
	sbrc R29, 2 ; se a flag Negativa não estiver setada pula a proxima instrução
	rjmp final_com_inteiro
	PUSH R23
	pega_parte_inteira_e_decimal R24 ; volta valor separado em parte x 10 e restante em R20 e R21
	POP R23
	add R23, R20
	mov R16, R23
	mov R26, R21
	rjmp final_mult
segundo_tipo_mult:
	PUSH R20
	mul R23, R21 ; multiplico parte decimal pela parte inteira
	mov R25, R0 ; repasso o resultado
	mul R24, R21
	mov R28, R0
	cpi R28, 9
	in R29, SREG
	sbrc R29, 2 ; se a flag Negativa não estiver setada pula a proxima instrução
	rjmp continua_ajuste_parte_1
	PUSH R20
	PUSH R21
	PUSH R25
	pega_parte_inteira_e_decimal R28 ; volta valor separado em parte x 10 e restante em R20 e R21
	POP R25
	add R25, R20
	POP R21
	POP R20
continua_ajuste_parte_1:
	mov R30, R25
	ldi R31, 0
	POP R20 ; parte inteira do valor multiplicando
	mul R23, R20
	mov R25, R0 ; repasso o resultado
	mul R24, R20
	mov R28, R0
	cpi R28, 9
	in R29, SREG
	sbrc R29, 2 ; se a flag Negativa não estiver setada pula a proxima instrução
	rjmp continua_ajuste_parte_2
	inc R25
	ldi R29, 10
	sub R28, R29
continua_ajuste_parte_2:
	add R30, R28 ; soma parte inteira do primeiro com parte decimal
	pega_parte_inteira_e_decimal R30 ; volta valor separado em parte x 10 e restante em R20 e R21
	add R25, R20 ; soma parte inteira do segundo com o que foi passado da op anterior
	mov R16, R25
	mov R26, R21
	rjmp final_mult
final_com_inteiro:
	mov R16, R23
	mov R26, R24
final_mult:
	passa_pra_iee R16, R26 ; vai retornar o resultado em R17 e R27
	mov R26, R27
	mov R16, R17
.endm

.macro pega_parte_inteira_e_decimal
	
	cpi @0, 10
	brsh retira_valores
	ldi R20, 0
	mov R21, @0
	rjmp fim_pega_parte_inteira_e_decimal
retira_valores:
	ldi R29, 10
	ldi R31, 0
loop_retira_valores:
	sub @0, R29
	inc R31
	cpi @0, 10
	brsh loop_retira_valores
	mov R20, R31
	mov R21, @0
fim_pega_parte_inteira_e_decimal:

.endm

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
	andi    R16, 0b00011000
	out     PORTB, R16
	out     PORTD, R17
	sbi     PORTB,@1
.endm

.macro display_com_ponto
	mov     R16, @0
	call    pega_tabela
	ori		R17, 0b10000000
	lds     R16, PORTB
	andi    R16, 0b00011000
	out     PORTB, R16
	out     PORTD, R17
	sbi     PORTB, @1
.endm

.macro mostra_resultado_display
	cpi R20, 10
	brsh div_display ;se for maior que 10, divide em dezena e unidade
	rjmp not_div_display
	
	div_display: ;divide
		ldi R19, 10
		sub R20, R19 ;R20 - D2
		ldi R22, 1 ;R22 - D1
		rjmp config

	not_div_display: ;nao divide
		ldi R22, 0 ;R22 - D1
	
	config:
		config_geral
		pause_i_ms 250
	
	loop_sinal:
		cpi R25,1
		breq sinal
		rjmp loop_display
	sinal:
		sbi PORTB, 5
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
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
		rjmp end_display
		rjmp loop_display

	end_display:
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
	brsh div_botao ;se for maior que 10, divide em dezena e unidade
	rjmp not_div_botao
	
	div_botao: ;divide
		ldi R19, 10
		sub R20, R19
		ldi R22, 1
		rjmp config

	not_div_botao: ;nao divide
		ldi R22, 0

	config:
		config_geral
		pause_i_ms 250
		cpi R22, 0
		breq loop_2

	loop_sinal:
		cpi R25,1
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
		sbis PINB, 4
		rjmp loop_3
		out PORTC, R20 ;segundo inteiro
		rjmp loop_2
	loop_3:
		sbis PINB, 3
		rjmp loop_4
		ldi R24, 0b00010000
		out PORTC, R24 ;decimal com virgula acesa
		rjmp loop_3
	loop_4:
		sbis PINB, 4
		rjmp end_botao
		ori R21, 0b00010000
		out PORTC, R21 ;decimal com virgula acesa
		rjmp loop_4
	end_botao:
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
	brsh div_temp ;se for maior que 10, divide em dezena e unidade
	rjmp not_div_temp
	
	div_temp: ;divide
		ldi R19, 10
		sub R20, R19
		ldi R22, 1
		rjmp config

	not_div_temp: ;nao divide
		ldi R22, 0

	config:
		config_geral
		pause_i_ms 250

	loop_sinal:
		cpi R25,1
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
	end_temp:
		ldi R16, 0b00000000
		out PORTB, R16
		ldi R16, 0b00100000
		out PORTC, R16
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
		pause_i_ms 250
.endm
