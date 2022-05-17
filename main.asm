.org 0x00
	rjmp start

.include "minha_lib.inc"
.include "macros.inc"
.include "delays.inc"

start:
	ldi R20, 8 ;inteiro
	ldi R21, 2 ;decimal
	mostra_resultado_display

	ldi R20, 6 ;inteiro
	ldi R21, 5 ;decimal
	mostra_resultado_botao

	ldi R20, 12 ;inteiro
	ldi R21, 3 ;decimal
	ldi R30, 1
	mostra_resultado_temp

	final_cod:
		rjmp final_cod

tabela:
	.db	0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F

pega_tabela:
	push	R16
		ldi		ZL, LOW(tabela<<1)
		ldi		ZH, HIGH(tabela<<1)
		add		ZL,R16
		ldi		R16,0
		adc		ZH,R16
		lpm		R17,Z
	pop		R16
	ret