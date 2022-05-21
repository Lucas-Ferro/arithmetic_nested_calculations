.org 0x00
rjmp start

.include "macros.inc" ; Biblioteca de contas

start:
LDI R16, 1
LDI R26, 8
passa_pra_iee R16, R26
PUSH R17
PUSH R27
LDI R16, 1
LDI R26, 2
passa_pra_iee R16, R26
POP R26
POP R16
sum R26, R27, R16, R17
LDI R25, 0

PUSH R16
PUSH R26
LDI R16, 4
LDI R26, 0
passa_pra_iee R16, R26
PUSH R17
PUSH R27
POP R26
POP R16
POP R27
POP R17
mult R26, R27, R16, R17
LDI R25, 0

PUSH R16
PUSH R26
LDI R16, 3
LDI R26, 0
passa_pra_iee R16, R26
PUSH R17
PUSH R27
POP R26
POP R16
POP R27
POP R17
sum R26, R27, R16, R17
LDI R25, 0

separa_inteiro_decimal R26, R16
mostra_resultado_botao

;Expected output = 15
; --------------- PROGRAM END ---------------

LDI R16, 5
LDI R26, 0
passa_pra_iee R16, R26
PUSH R17
PUSH R27
LDI R16, 2
LDI R26, 3
passa_pra_iee R16, R26
POP R26
POP R16
sum R26, R27, R16, R17
LDI R25, 0

PUSH R16
PUSH R26
LDI R16, 10
LDI R26, 5
passa_pra_iee R16, R26
PUSH R17
PUSH R27
POP R26
POP R16
POP R27
POP R17
subtraction R26, R27, R16, R17
LDI R25, 0

PUSH R16
PUSH R26
LDI R16, 3
LDI R26, 0
passa_pra_iee R16, R26
PUSH R17
PUSH R27
POP R26
POP R16
POP R27
POP R17
mult R26, R27, R16, R17
LDI R25, 0

PUSH R16
PUSH R26
LDI R16, 12
LDI R26, 4
passa_pra_iee R16, R26
PUSH R17
PUSH R27
POP R26
POP R16
POP R27
POP R17
div R26, R27, R16, R17
LDI R25, 0

separa_inteiro_decimal R26, R16
mostra_resultado_display

;Expected output = 1.29167
; --------------- PROGRAM END ---------------

LDI R16, 5
LDI R26, 0
passa_pra_iee R16, R26
PUSH R17
PUSH R27
LDI R16, 2
LDI R26, 0
passa_pra_iee R16, R26
POP R26
POP R16
sum R26, R27, R16, R17
LDI R25, 0

PUSH R16
PUSH R26
LDI R16, 10
LDI R26, 0
passa_pra_iee R16, R26
PUSH R17
PUSH R27
POP R26
POP R16
POP R27
POP R17
subtraction R26, R27, R16, R17
LDI R25, 0

PUSH R16
PUSH R26
LDI R16, 3
LDI R26, 0
passa_pra_iee R16, R26
PUSH R17
PUSH R27
POP R26
POP R16
POP R27
POP R17
mult R26, R27, R16, R17
LDI R25, 0

PUSH R16
PUSH R26
LDI R16, 12
LDI R26, 0
passa_pra_iee R16, R26
PUSH R17
PUSH R27
POP R26
POP R16
POP R27
POP R17
resto R26, R27, R16, R17
LDI R25, 0

PUSH R16
PUSH R26
LDI R16, 5
LDI R26, 0
passa_pra_iee R16, R26
PUSH R17
PUSH R27
POP R26
POP R16
POP R27
POP R17
subtraction R26, R27, R16, R17
LDI R25, 0

PUSH R16
PUSH R26
LDI R16, 2
LDI R26, 1
passa_pra_iee R16, R26
PUSH R17
PUSH R27
POP R26
POP R16
POP R27
POP R17
potencia R26, R27, R16, R17
LDI R25, 0

separa_inteiro_decimal R26, R16
mostra_resultado_display

;Expected output = 4.41
; --------------- PROGRAM END ---------------

LDI R16, 5
LDI R26, 0
passa_pra_iee R16, R26
PUSH R17
PUSH R27
LDI R16, 2
LDI R26, 3
passa_pra_iee R16, R26
POP R26
POP R16
sum R26, R27, R16, R17
LDI R25, 0

PUSH R16
PUSH R26
LDI R16, 10
LDI R26, 5
passa_pra_iee R16, R26
PUSH R17
PUSH R27
POP R26
POP R16
POP R27
POP R17
subtraction R26, R27, R16, R17
LDI R25, 0

PUSH R16
PUSH R26
LDI R16, 3
LDI R26, 0
passa_pra_iee R16, R26
PUSH R17
PUSH R27
POP R26
POP R16
POP R27
POP R17
mult R26, R27, R16, R17
LDI R25, 0

PUSH R16
PUSH R26
LDI R16, 12
LDI R26, 4
passa_pra_iee R16, R26
PUSH R17
PUSH R27
POP R26
POP R16
POP R27
POP R17
div R26, R27, R16, R17
LDI R25, 0

PUSH R16
PUSH R26
LDI R16, 2
LDI R26, 0
passa_pra_iee R16, R26
PUSH R17
PUSH R27
POP R26
POP R16
POP R27
POP R17
sum R26, R27, R16, R17
LDI R25, 0

PUSH R16
PUSH R26
LDI R16, 1
LDI R26, 2
passa_pra_iee R16, R26
POP R26
POP R16
subtraction R26, R27, R16, R17
LDI R25, 1

PUSH R16
PUSH R26
LDI R16, 3
LDI R26, 0
passa_pra_iee R16, R26
PUSH R17
PUSH R27
POP R26
POP R16
POP R27
POP R17
mult R26, R27, R16, R17
LDI R25, 1

separa_inteiro_decimal R26, R16
mostra_resultado_temp

;Expected output = -6.275
; --------------- PROGRAM END ---------------

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
  pop R16
  ret

; --------------- CODE END ---------------
