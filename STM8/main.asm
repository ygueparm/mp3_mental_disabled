;--------------------------------------------------------
; File Created by SDCC : free open source ISO C Compiler
; Version 4.5.0 #15242 (Linux)
;--------------------------------------------------------
	.module main
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _main
	.globl _TIM4_UPD_OVF_IRQHandler
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area DATA
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area INITIALIZED
_tick_ms:
	.ds 4
_lastStateNext:
	.ds 1
_btnStateNext:
	.ds 1
_debounceNext:
	.ds 4
_lastActionNext:
	.ds 4
_lastStateVolD:
	.ds 1
_btnStateVolD:
	.ds 1
_debounceVolD:
	.ds 4
_lastActionVolD:
	.ds 4
;--------------------------------------------------------
; Stack segment in internal ram
;--------------------------------------------------------
	.area SSEG
__start__stack:
	.ds	1

;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area DABS (ABS)

; default segment ordering for linker
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area CONST
	.area INITIALIZER
	.area CODE

;--------------------------------------------------------
; interrupt vector
;--------------------------------------------------------
	.area HOME
__interrupt_vect:
	int s_GSINIT ; reset
	int 0x000000 ; trap
	int 0x000000 ; int0
	int 0x000000 ; int1
	int 0x000000 ; int2
	int 0x000000 ; int3
	int 0x000000 ; int4
	int 0x000000 ; int5
	int 0x000000 ; int6
	int 0x000000 ; int7
	int 0x000000 ; int8
	int 0x000000 ; int9
	int 0x000000 ; int10
	int 0x000000 ; int11
	int 0x000000 ; int12
	int 0x000000 ; int13
	int 0x000000 ; int14
	int 0x000000 ; int15
	int 0x000000 ; int16
	int 0x000000 ; int17
	int 0x000000 ; int18
	int 0x000000 ; int19
	int 0x000000 ; int20
	int 0x000000 ; int21
	int 0x000000 ; int22
	int _TIM4_UPD_OVF_IRQHandler ; int23
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area GSINIT
	call	___sdcc_external_startup
	tnz	a
	jreq	__sdcc_init_data
	jp	__sdcc_program_startup
__sdcc_init_data:
; stm8_genXINIT() start
	ldw x, #l_DATA
	jreq	00002$
00001$:
	clr (s_DATA - 1, x)
	decw x
	jrne	00001$
00002$:
	ldw	x, #l_INITIALIZER
	jreq	00004$
00003$:
	ld	a, (s_INITIALIZER - 1, x)
	ld	(s_INITIALIZED - 1, x), a
	decw	x
	jrne	00003$
00004$:
; stm8_genXINIT() end
	.area GSFINAL
	jp	__sdcc_program_startup
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME
	.area HOME
__sdcc_program_startup:
	jp	_main
;	return from main will return to caller
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE
;	main.c: 94: void TIM4_UPD_OVF_IRQHandler(void) __interrupt(23)
;	-----------------------------------------
;	 function TIM4_UPD_OVF_IRQHandler
;	-----------------------------------------
_TIM4_UPD_OVF_IRQHandler:
;	main.c: 96: TIM4_SR &= ~0x01;   /* acquitter le flag UIF */
	bres	0x5344, #0
;	main.c: 97: tick_ms++;
	ldw	x, _tick_ms+2
	ldw	y, _tick_ms+0
	incw	x
	jrne	00103$
	incw	y
00103$:
	ldw	_tick_ms+2, x
	ldw	_tick_ms+0, y
;	main.c: 98: }
	iret
;	main.c: 104: static uint32_t millis(void)
;	-----------------------------------------
;	 function millis
;	-----------------------------------------
_millis:
;	main.c: 107: disableInterrupts();
	sim
;	main.c: 108: val = tick_ms;
	ldw	x, _tick_ms+2
	ldw	y, _tick_ms+0
;	main.c: 109: enableInterrupts();
	rim
;	main.c: 110: return val;
;	main.c: 111: }
	ret
;	main.c: 117: static void delay_ms(uint16_t ms)
;	-----------------------------------------
;	 function delay_ms
;	-----------------------------------------
_delay_ms:
	sub	sp, #14
	ldw	(0x0d, sp), x
;	main.c: 119: uint32_t start = millis();
	call	_millis
	ldw	(0x03, sp), x
	ldw	(0x01, sp), y
;	main.c: 120: while ((millis() - start) < ms) {
00101$:
	call	_millis
	ldw	(0x0b, sp), x
	ldw	(0x09, sp), y
	ldw	x, (0x0b, sp)
	subw	x, (0x03, sp)
	ldw	(0x07, sp), x
	ld	a, (0x0a, sp)
	sbc	a, (0x02, sp)
	ld	(0x06, sp), a
	ld	a, (0x09, sp)
	sbc	a, (0x01, sp)
	ldw	y, (0x0d, sp)
	ldw	(0x0b, sp), y
	clrw	x
	ldw	(0x09, sp), x
	push	a
	ldw	x, (0x08, sp)
	cpw	x, (0x0c, sp)
	ld	a, (0x07, sp)
	sbc	a, (0x0b, sp)
	pop	a
	sbc	a, (0x09, sp)
	jrc	00101$
;	main.c: 123: }
	addw	sp, #14
	ret
;	main.c: 155: static void pin_low(uint8_t is_track)
;	-----------------------------------------
;	 function pin_low
;	-----------------------------------------
_pin_low:
	push	a
	ld	(0x01, sp), a
;	main.c: 158: PC_ODR &= ~(1 << KEY_TRACK_BIT);
	ld	a, 0x500a
;	main.c: 157: if (is_track) {
	tnz	(0x01, sp)
	jreq	00102$
;	main.c: 158: PC_ODR &= ~(1 << KEY_TRACK_BIT);
	and	a, #0xef
	ld	0x500a, a
;	main.c: 159: PC_DDR |=  (1 << KEY_TRACK_BIT);
	bset	0x500c, #4
	jra	00104$
00102$:
;	main.c: 161: PC_ODR &= ~(1 << KEY_VOLDOWN_BIT);
	and	a, #0xf7
	ld	0x500a, a
;	main.c: 162: PC_DDR |=  (1 << KEY_VOLDOWN_BIT);
	bset	0x500c, #3
00104$:
;	main.c: 164: }
	pop	a
	ret
;	main.c: 166: static void pin_hiz(uint8_t is_track)
;	-----------------------------------------
;	 function pin_hiz
;	-----------------------------------------
_pin_hiz:
	push	a
	ld	(0x01, sp), a
;	main.c: 169: PC_DDR &= ~(1 << KEY_TRACK_BIT);
	ld	a, 0x500c
;	main.c: 168: if (is_track) {
	tnz	(0x01, sp)
	jreq	00102$
;	main.c: 169: PC_DDR &= ~(1 << KEY_TRACK_BIT);
	and	a, #0xef
	ld	0x500c, a
;	main.c: 170: PC_CR1 &= ~(1 << KEY_TRACK_BIT);
	bres	0x500d, #4
	jra	00104$
00102$:
;	main.c: 172: PC_DDR &= ~(1 << KEY_VOLDOWN_BIT);
	and	a, #0xf7
	ld	0x500c, a
;	main.c: 173: PC_CR1 &= ~(1 << KEY_VOLDOWN_BIT);
	bres	0x500d, #3
00104$:
;	main.c: 175: }
	pop	a
	ret
;	main.c: 177: static void send_pulse(uint8_t is_track, uint16_t duration_ms)
;	-----------------------------------------
;	 function send_pulse
;	-----------------------------------------
_send_pulse:
;	main.c: 179: pin_low(is_track);
	push	a
	pushw	x
	call	_pin_low
	popw	x
;	main.c: 180: delay_ms(duration_ms);
	call	_delay_ms
	pop	a
;	main.c: 181: pin_hiz(is_track);
;	main.c: 182: }
	jp	_pin_hiz
;	main.c: 188: static uint8_t button_fell(uint8_t reading,
;	-----------------------------------------
;	 function button_fell
;	-----------------------------------------
_button_fell:
	sub	sp, #15
	ld	(0x0f, sp), a
	ldw	(0x0d, sp), x
;	main.c: 193: uint32_t now = millis();
	call	_millis
	ldw	(0x03, sp), x
	ldw	(0x01, sp), y
;	main.c: 195: if (reading != *lastState)
	ldw	x, (0x0d, sp)
	ld	a, (x)
;	main.c: 196: *debounceTime = now;
	ldw	x, (0x12, sp)
;	main.c: 195: if (reading != *lastState)
	cp	a, (0x0f, sp)
	jreq	00102$
;	main.c: 196: *debounceTime = now;
	ldw	y, (0x03, sp)
	ldw	(0x2, x), y
	ldw	y, (0x01, sp)
	ldw	(x), y
00102$:
;	main.c: 199: if ((now - *debounceTime) > DEBOUNCE_DELAY) {
	ldw	y, x
	ldw	y, (0x2, y)
	ldw	(0x07, sp), y
	ldw	x, (x)
	ldw	y, (0x03, sp)
	subw	y, (0x07, sp)
	ldw	(0x0b, sp), y
	ld	a, (0x02, sp)
	pushw	x
	sbc	a, (2, sp)
	popw	x
	ld	(0x0a, sp), a
	ld	a, (0x01, sp)
	pushw	x
	sbc	a, (1, sp)
	popw	x
	ld	(0x09, sp), a
	ldw	x, #0x0032
	cpw	x, (0x0b, sp)
	clr	a
	sbc	a, (0x0a, sp)
	clr	a
	sbc	a, (0x09, sp)
	jrnc	00108$
;	main.c: 200: if (reading != *currentState) {
	ldw	x, (0x14, sp)
	ld	a, (x)
	cp	a, (0x0f, sp)
	jreq	00108$
;	main.c: 201: *currentState = reading;
	ld	a, (0x0f, sp)
	ld	(x), a
;	main.c: 202: if (*currentState == 0) {
	tnz	(0x0f, sp)
	jrne	00108$
;	main.c: 203: *lastState = reading;
	ldw	x, (0x0d, sp)
	ld	a, (0x0f, sp)
	ld	(x), a
;	main.c: 204: return 1;
	ld	a, #0x01
	jra	00109$
00108$:
;	main.c: 209: *lastState = reading;
	ldw	x, (0x0d, sp)
	ld	a, (0x0f, sp)
	ld	(x), a
;	main.c: 210: return 0;
	clr	a
00109$:
;	main.c: 211: }
	ldw	x, (16, sp)
	addw	sp, #21
	jp	(x)
;	main.c: 217: static uint8_t can_trigger(uint32_t *lastActionTime, uint16_t cooldown)
;	-----------------------------------------
;	 function can_trigger
;	-----------------------------------------
_can_trigger:
	sub	sp, #12
;	main.c: 220: if ((millis() - *lastActionTime) >= cooldown)
	pushw	x
	call	_millis
	ldw	(0x05, sp), x
	popw	x
	ld	a, (0x3, x)
	ld	(0x08, sp), a
	ld	a, (0x2, x)
	ld	(0x07, sp), a
	ldw	x, (x)
	ldw	(0x05, sp), x
	ldw	x, (0x03, sp)
	subw	x, (0x07, sp)
	ld	a, yl
	sbc	a, (0x06, sp)
	ld	(0x0a, sp), a
	ld	a, yh
	sbc	a, (0x05, sp)
	ldw	y, (0x0f, sp)
	ldw	(0x07, sp), y
	clr	(0x06, sp)
	clr	(0x05, sp)
	push	a
	cpw	x, (0x08, sp)
	ld	a, (0x0b, sp)
	sbc	a, (0x07, sp)
	pop	a
	sbc	a, (0x05, sp)
	jrc	00102$
;	main.c: 221: return 1;
	ld	a, #0x01
;	main.c: 222: return 0;
	.byte 0x21
00102$:
	clr	a
00103$:
;	main.c: 223: }
	ldw	x, (13, sp)
	addw	sp, #16
	jp	(x)
;	main.c: 233: static void tim4_init(void)
;	-----------------------------------------
;	 function tim4_init
;	-----------------------------------------
_tim4_init:
;	main.c: 235: TIM4_CR1  = 0x00;
	mov	0x5340+0, #0x00
;	main.c: 236: TIM4_PSCR = 0x03;   /* prescaler /8 (2^3)           */
	mov	0x5347+0, #0x03
;	main.c: 237: TIM4_ARR  = 62;     /* auto-reload : compte de 0→62 */
	mov	0x5348+0, #0x3e
;	main.c: 238: TIM4_IER  = 0x01;   /* interruption débordement     */
	mov	0x5343+0, #0x01
;	main.c: 239: TIM4_SR   = 0x00;   /* acquitter flag résiduel      */
	mov	0x5344+0, #0x00
;	main.c: 240: TIM4_CR1  = 0x01;   /* démarrer (CEN=1)             */
	mov	0x5340+0, #0x01
;	main.c: 241: }
	ret
;	main.c: 247: static void hw_init(void)
;	-----------------------------------------
;	 function hw_init
;	-----------------------------------------
_hw_init:
;	main.c: 250: CLK_CKDIVR = 0x1B;
	mov	0x50c6+0, #0x1b
;	main.c: 258: PA_ODR &= ~((1 << 1) | (1 << 2) | (1 << 3));
	ld	a, 0x5000
	and	a, #0xf1
	ld	0x5000, a
;	main.c: 259: PA_DDR |=   (1 << 1) | (1 << 2) | (1 << 3);
	ld	a, 0x5002
	or	a, #0x0e
	ld	0x5002, a
;	main.c: 260: PA_CR1 |=   (1 << 1) | (1 << 2) | (1 << 3);
	ld	a, 0x5003
	or	a, #0x0e
	ld	0x5003, a
;	main.c: 261: PA_CR2 &=  ~((1 << 1) | (1 << 2) | (1 << 3));  /* pas d'IRQ externe */
	ld	a, 0x5004
	and	a, #0xf1
	ld	0x5004, a
;	main.c: 263: PD_ODR &= ~(1 << 6);
	bres	0x5010, #6
;	main.c: 264: PD_DDR |=  (1 << 6);
	bset	0x5013, #6
;	main.c: 265: PD_CR1 |=  (1 << 6);
	bset	0x5014, #6
;	main.c: 266: PD_CR2 &= ~(1 << 6);
	bres	0x5015, #6
;	main.c: 272: PD_DDR &= ~(1 << BTN_NEXT_BIT);
	bres	0x5013, #5
;	main.c: 273: PD_CR1 |=  (1 << BTN_NEXT_BIT);
	bset	0x5014, #5
;	main.c: 274: PD_CR2 &= ~(1 << BTN_NEXT_BIT);
	bres	0x5015, #5
;	main.c: 280: PC_DDR &= ~((1 << BTN_VOLD_BIT) | (1 << KEY_VOLDOWN_BIT) | (1 << KEY_TRACK_BIT));
	ld	a, 0x500c
	and	a, #0xc7
	ld	0x500c, a
;	main.c: 281: PC_CR1 |=  (1 << BTN_VOLD_BIT);
	bset	0x500d, #5
;	main.c: 282: PC_CR1 &= ~((1 << KEY_VOLDOWN_BIT) | (1 << KEY_TRACK_BIT));
	ld	a, 0x500d
	and	a, #0xe7
	ld	0x500d, a
;	main.c: 283: PC_CR2 &= ~((1 << BTN_VOLD_BIT) | (1 << KEY_VOLDOWN_BIT) | (1 << KEY_TRACK_BIT));
	ld	a, 0x500e
	and	a, #0xc7
	ld	0x500e, a
;	main.c: 286: tim4_init();
	call	_tim4_init
;	main.c: 289: enableInterrupts();
	rim
;	main.c: 290: }
	ret
;	main.c: 296: void main(void)
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
;	main.c: 298: hw_init();
	call	_hw_init
;	main.c: 300: lastStateNext = BTN_NEXT_READ();
	ld	a, 0x5011
	swap	a
	srl	a
	and	a, #0x01
	ld	_lastStateNext+0, a
;	main.c: 301: btnStateNext  = lastStateNext;
	mov	_btnStateNext+0, _lastStateNext+0
;	main.c: 302: lastStateVolD = BTN_VOLD_READ();
	ld	a, 0x500b
	swap	a
	srl	a
	and	a, #0x01
	ld	_lastStateVolD+0, a
;	main.c: 303: btnStateVolD  = lastStateVolD;
	mov	_btnStateVolD+0, _lastStateVolD+0
;	main.c: 305: debounceNext   = millis();
	call	_millis
	ldw	_debounceNext+2, x
	ldw	_debounceNext+0, y
;	main.c: 306: debounceVolD   = millis();
	call	_millis
	ldw	_debounceVolD+2, x
	ldw	_debounceVolD+0, y
;	main.c: 308: delay_ms(MP3_BOOT_DELAY);
	ldw	x, #0x03e8
	call	_delay_ms
;	main.c: 313: lastActionNext = millis() - COOLDOWN_NEXT;
	call	_millis
	subw	x, #0x0fa0
	jrnc	00149$
	decw	y
00149$:
	ldw	_lastActionNext+2, x
	ldw	_lastActionNext+0, y
;	main.c: 314: lastActionVolD = millis() - COOLDOWN_VOLD;
	call	_millis
	subw	x, #0x07d0
	jrnc	00150$
	decw	y
00150$:
	ldw	_lastActionVolD+2, x
	ldw	_lastActionVolD+0, y
00110$:
;	main.c: 323: &lastStateNext, &debounceNext, &btnStateNext)) {
	ldw	x, #_lastStateNext+0
;	main.c: 322: if (button_fell(BTN_NEXT_READ(),
	ld	a, 0x5011
	swap	a
	srl	a
	and	a, #0x01
	push	#<(_btnStateNext+0)
	push	#((_btnStateNext+0) >> 8)
	push	#<(_debounceNext+0)
	push	#((_debounceNext+0) >> 8)
	call	_button_fell
	tnz	a
	jreq	00104$
;	main.c: 324: if (can_trigger(&lastActionNext, COOLDOWN_NEXT)) {
	push	#0xa0
	push	#0x0f
	ldw	x, #(_lastActionNext+0)
	call	_can_trigger
	tnz	a
	jreq	00104$
;	main.c: 325: send_pulse(1, PULSE_NEXT);
	ldw	x, #0x00fa
	ld	a, #0x01
	call	_send_pulse
;	main.c: 326: lastActionNext = millis();
	call	_millis
	ldw	_lastActionNext+2, x
	ldw	_lastActionNext+0, y
00104$:
;	main.c: 332: &lastStateVolD, &debounceVolD, &btnStateVolD)) {
	ldw	x, #_lastStateVolD+0
;	main.c: 331: if (button_fell(BTN_VOLD_READ(),
	ld	a, 0x500b
	swap	a
	srl	a
	and	a, #0x01
	push	#<(_btnStateVolD+0)
	push	#((_btnStateVolD+0) >> 8)
	push	#<(_debounceVolD+0)
	push	#((_debounceVolD+0) >> 8)
	call	_button_fell
	tnz	a
	jreq	00108$
;	main.c: 333: if (can_trigger(&lastActionVolD, COOLDOWN_VOLD)) {
	push	#0xd0
	push	#0x07
	ldw	x, #(_lastActionVolD+0)
	call	_can_trigger
	tnz	a
	jreq	00108$
;	main.c: 334: send_pulse(0, PULSE_VOL_DOWN);
	ldw	x, #0x05dc
	clr	a
	call	_send_pulse
;	main.c: 335: lastActionVolD = millis();
	call	_millis
	ldw	_lastActionVolD+2, x
	ldw	_lastActionVolD+0, y
00108$:
;	main.c: 339: delay_ms(5);
	ldw	x, #0x0005
	call	_delay_ms
	jra	00110$
;	main.c: 341: }
	ret
	.area CODE
	.area CONST
	.area INITIALIZER
__xinit__tick_ms:
	.byte #0x00, #0x00, #0x00, #0x00	; 0
__xinit__lastStateNext:
	.db #0x01	; 1
__xinit__btnStateNext:
	.db #0x01	; 1
__xinit__debounceNext:
	.byte #0x00, #0x00, #0x00, #0x00	; 0
__xinit__lastActionNext:
	.byte #0x00, #0x00, #0x00, #0x00	; 0
__xinit__lastStateVolD:
	.db #0x01	; 1
__xinit__btnStateVolD:
	.db #0x01	; 1
__xinit__debounceVolD:
	.byte #0x00, #0x00, #0x00, #0x00	; 0
__xinit__lastActionVolD:
	.byte #0x00, #0x00, #0x00, #0x00	; 0
	.area CABS (ABS)
