                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ISO C Compiler
                                      3 ; Version 4.5.0 #15242 (Linux)
                                      4 ;--------------------------------------------------------
                                      5 	.module main
                                      6 	
                                      7 ;--------------------------------------------------------
                                      8 ; Public variables in this module
                                      9 ;--------------------------------------------------------
                                     10 	.globl _main
                                     11 	.globl _TIM4_UPD_OVF_IRQHandler
                                     12 ;--------------------------------------------------------
                                     13 ; ram data
                                     14 ;--------------------------------------------------------
                                     15 	.area DATA
                                     16 ;--------------------------------------------------------
                                     17 ; ram data
                                     18 ;--------------------------------------------------------
                                     19 	.area INITIALIZED
      000001                         20 _tick_ms:
      000001                         21 	.ds 4
      000005                         22 _lastStateNext:
      000005                         23 	.ds 1
      000006                         24 _btnStateNext:
      000006                         25 	.ds 1
      000007                         26 _debounceNext:
      000007                         27 	.ds 4
      00000B                         28 _lastActionNext:
      00000B                         29 	.ds 4
      00000F                         30 _lastStateVolD:
      00000F                         31 	.ds 1
      000010                         32 _btnStateVolD:
      000010                         33 	.ds 1
      000011                         34 _debounceVolD:
      000011                         35 	.ds 4
      000015                         36 _lastActionVolD:
      000015                         37 	.ds 4
                                     38 ;--------------------------------------------------------
                                     39 ; Stack segment in internal ram
                                     40 ;--------------------------------------------------------
                                     41 	.area SSEG
      000019                         42 __start__stack:
      000019                         43 	.ds	1
                                     44 
                                     45 ;--------------------------------------------------------
                                     46 ; absolute external ram data
                                     47 ;--------------------------------------------------------
                                     48 	.area DABS (ABS)
                                     49 
                                     50 ; default segment ordering for linker
                                     51 	.area HOME
                                     52 	.area GSINIT
                                     53 	.area GSFINAL
                                     54 	.area CONST
                                     55 	.area INITIALIZER
                                     56 	.area CODE
                                     57 
                                     58 ;--------------------------------------------------------
                                     59 ; interrupt vector
                                     60 ;--------------------------------------------------------
                                     61 	.area HOME
      008000                         62 __interrupt_vect:
      008000 82 00 80 6B             63 	int s_GSINIT ; reset
      008004 82 00 00 00             64 	int 0x000000 ; trap
      008008 82 00 00 00             65 	int 0x000000 ; int0
      00800C 82 00 00 00             66 	int 0x000000 ; int1
      008010 82 00 00 00             67 	int 0x000000 ; int2
      008014 82 00 00 00             68 	int 0x000000 ; int3
      008018 82 00 00 00             69 	int 0x000000 ; int4
      00801C 82 00 00 00             70 	int 0x000000 ; int5
      008020 82 00 00 00             71 	int 0x000000 ; int6
      008024 82 00 00 00             72 	int 0x000000 ; int7
      008028 82 00 00 00             73 	int 0x000000 ; int8
      00802C 82 00 00 00             74 	int 0x000000 ; int9
      008030 82 00 00 00             75 	int 0x000000 ; int10
      008034 82 00 00 00             76 	int 0x000000 ; int11
      008038 82 00 00 00             77 	int 0x000000 ; int12
      00803C 82 00 00 00             78 	int 0x000000 ; int13
      008040 82 00 00 00             79 	int 0x000000 ; int14
      008044 82 00 00 00             80 	int 0x000000 ; int15
      008048 82 00 00 00             81 	int 0x000000 ; int16
      00804C 82 00 00 00             82 	int 0x000000 ; int17
      008050 82 00 00 00             83 	int 0x000000 ; int18
      008054 82 00 00 00             84 	int 0x000000 ; int19
      008058 82 00 00 00             85 	int 0x000000 ; int20
      00805C 82 00 00 00             86 	int 0x000000 ; int21
      008060 82 00 00 00             87 	int 0x000000 ; int22
      008064 82 00 80 A9             88 	int _TIM4_UPD_OVF_IRQHandler ; int23
                                     89 ;--------------------------------------------------------
                                     90 ; global & static initialisations
                                     91 ;--------------------------------------------------------
                                     92 	.area HOME
                                     93 	.area GSINIT
                                     94 	.area GSFINAL
                                     95 	.area GSINIT
      00806B CD 83 4B         [ 4]   96 	call	___sdcc_external_startup
      00806E 4D               [ 1]   97 	tnz	a
      00806F 27 03            [ 1]   98 	jreq	__sdcc_init_data
      008071 CC 80 68         [ 2]   99 	jp	__sdcc_program_startup
      008074                        100 __sdcc_init_data:
                                    101 ; stm8_genXINIT() start
      008074 AE 00 00         [ 2]  102 	ldw x, #l_DATA
      008077 27 07            [ 1]  103 	jreq	00002$
      008079                        104 00001$:
      008079 72 4F 00 00      [ 1]  105 	clr (s_DATA - 1, x)
      00807D 5A               [ 2]  106 	decw x
      00807E 26 F9            [ 1]  107 	jrne	00001$
      008080                        108 00002$:
      008080 AE 00 18         [ 2]  109 	ldw	x, #l_INITIALIZER
      008083 27 09            [ 1]  110 	jreq	00004$
      008085                        111 00003$:
      008085 D6 80 90         [ 1]  112 	ld	a, (s_INITIALIZER - 1, x)
      008088 D7 00 00         [ 1]  113 	ld	(s_INITIALIZED - 1, x), a
      00808B 5A               [ 2]  114 	decw	x
      00808C 26 F7            [ 1]  115 	jrne	00003$
      00808E                        116 00004$:
                                    117 ; stm8_genXINIT() end
                                    118 	.area GSFINAL
      00808E CC 80 68         [ 2]  119 	jp	__sdcc_program_startup
                                    120 ;--------------------------------------------------------
                                    121 ; Home
                                    122 ;--------------------------------------------------------
                                    123 	.area HOME
                                    124 	.area HOME
      008068                        125 __sdcc_program_startup:
      008068 CC 82 78         [ 2]  126 	jp	_main
                                    127 ;	return from main will return to caller
                                    128 ;--------------------------------------------------------
                                    129 ; code
                                    130 ;--------------------------------------------------------
                                    131 	.area CODE
                                    132 ;	main.c: 94: void TIM4_UPD_OVF_IRQHandler(void) __interrupt(23)
                                    133 ;	-----------------------------------------
                                    134 ;	 function TIM4_UPD_OVF_IRQHandler
                                    135 ;	-----------------------------------------
      0080A9                        136 _TIM4_UPD_OVF_IRQHandler:
                                    137 ;	main.c: 96: TIM4_SR &= ~0x01;   /* acquitter le flag UIF */
      0080A9 72 11 53 44      [ 1]  138 	bres	0x5344, #0
                                    139 ;	main.c: 97: tick_ms++;
      0080AD CE 00 03         [ 2]  140 	ldw	x, _tick_ms+2
      0080B0 90 CE 00 01      [ 2]  141 	ldw	y, _tick_ms+0
      0080B4 5C               [ 1]  142 	incw	x
      0080B5 26 02            [ 1]  143 	jrne	00103$
      0080B7 90 5C            [ 1]  144 	incw	y
      0080B9                        145 00103$:
      0080B9 CF 00 03         [ 2]  146 	ldw	_tick_ms+2, x
      0080BC 90 CF 00 01      [ 2]  147 	ldw	_tick_ms+0, y
                                    148 ;	main.c: 98: }
      0080C0 80               [11]  149 	iret
                                    150 ;	main.c: 104: static uint32_t millis(void)
                                    151 ;	-----------------------------------------
                                    152 ;	 function millis
                                    153 ;	-----------------------------------------
      0080C1                        154 _millis:
                                    155 ;	main.c: 107: disableInterrupts();
      0080C1 9B               [ 1]  156 	sim
                                    157 ;	main.c: 108: val = tick_ms;
      0080C2 CE 00 03         [ 2]  158 	ldw	x, _tick_ms+2
      0080C5 90 CE 00 01      [ 2]  159 	ldw	y, _tick_ms+0
                                    160 ;	main.c: 109: enableInterrupts();
      0080C9 9A               [ 1]  161 	rim
                                    162 ;	main.c: 110: return val;
                                    163 ;	main.c: 111: }
      0080CA 81               [ 4]  164 	ret
                                    165 ;	main.c: 117: static void delay_ms(uint16_t ms)
                                    166 ;	-----------------------------------------
                                    167 ;	 function delay_ms
                                    168 ;	-----------------------------------------
      0080CB                        169 _delay_ms:
      0080CB 52 0E            [ 2]  170 	sub	sp, #14
      0080CD 1F 0D            [ 2]  171 	ldw	(0x0d, sp), x
                                    172 ;	main.c: 119: uint32_t start = millis();
      0080CF CD 80 C1         [ 4]  173 	call	_millis
      0080D2 1F 03            [ 2]  174 	ldw	(0x03, sp), x
      0080D4 17 01            [ 2]  175 	ldw	(0x01, sp), y
                                    176 ;	main.c: 120: while ((millis() - start) < ms) {
      0080D6                        177 00101$:
      0080D6 CD 80 C1         [ 4]  178 	call	_millis
      0080D9 1F 0B            [ 2]  179 	ldw	(0x0b, sp), x
      0080DB 17 09            [ 2]  180 	ldw	(0x09, sp), y
      0080DD 1E 0B            [ 2]  181 	ldw	x, (0x0b, sp)
      0080DF 72 F0 03         [ 2]  182 	subw	x, (0x03, sp)
      0080E2 1F 07            [ 2]  183 	ldw	(0x07, sp), x
      0080E4 7B 0A            [ 1]  184 	ld	a, (0x0a, sp)
      0080E6 12 02            [ 1]  185 	sbc	a, (0x02, sp)
      0080E8 6B 06            [ 1]  186 	ld	(0x06, sp), a
      0080EA 7B 09            [ 1]  187 	ld	a, (0x09, sp)
      0080EC 12 01            [ 1]  188 	sbc	a, (0x01, sp)
      0080EE 16 0D            [ 2]  189 	ldw	y, (0x0d, sp)
      0080F0 17 0B            [ 2]  190 	ldw	(0x0b, sp), y
      0080F2 5F               [ 1]  191 	clrw	x
      0080F3 1F 09            [ 2]  192 	ldw	(0x09, sp), x
      0080F5 88               [ 1]  193 	push	a
      0080F6 1E 08            [ 2]  194 	ldw	x, (0x08, sp)
      0080F8 13 0C            [ 2]  195 	cpw	x, (0x0c, sp)
      0080FA 7B 07            [ 1]  196 	ld	a, (0x07, sp)
      0080FC 12 0B            [ 1]  197 	sbc	a, (0x0b, sp)
      0080FE 84               [ 1]  198 	pop	a
      0080FF 12 09            [ 1]  199 	sbc	a, (0x09, sp)
      008101 25 D3            [ 1]  200 	jrc	00101$
                                    201 ;	main.c: 123: }
      008103 5B 0E            [ 2]  202 	addw	sp, #14
      008105 81               [ 4]  203 	ret
                                    204 ;	main.c: 155: static void pin_low(uint8_t is_track)
                                    205 ;	-----------------------------------------
                                    206 ;	 function pin_low
                                    207 ;	-----------------------------------------
      008106                        208 _pin_low:
      008106 88               [ 1]  209 	push	a
      008107 6B 01            [ 1]  210 	ld	(0x01, sp), a
                                    211 ;	main.c: 158: PC_ODR &= ~(1 << KEY_TRACK_BIT);
      008109 C6 50 0A         [ 1]  212 	ld	a, 0x500a
                                    213 ;	main.c: 157: if (is_track) {
      00810C 0D 01            [ 1]  214 	tnz	(0x01, sp)
      00810E 27 0B            [ 1]  215 	jreq	00102$
                                    216 ;	main.c: 158: PC_ODR &= ~(1 << KEY_TRACK_BIT);
      008110 A4 EF            [ 1]  217 	and	a, #0xef
      008112 C7 50 0A         [ 1]  218 	ld	0x500a, a
                                    219 ;	main.c: 159: PC_DDR |=  (1 << KEY_TRACK_BIT);
      008115 72 18 50 0C      [ 1]  220 	bset	0x500c, #4
      008119 20 09            [ 2]  221 	jra	00104$
      00811B                        222 00102$:
                                    223 ;	main.c: 161: PC_ODR &= ~(1 << KEY_VOLDOWN_BIT);
      00811B A4 F7            [ 1]  224 	and	a, #0xf7
      00811D C7 50 0A         [ 1]  225 	ld	0x500a, a
                                    226 ;	main.c: 162: PC_DDR |=  (1 << KEY_VOLDOWN_BIT);
      008120 72 16 50 0C      [ 1]  227 	bset	0x500c, #3
      008124                        228 00104$:
                                    229 ;	main.c: 164: }
      008124 84               [ 1]  230 	pop	a
      008125 81               [ 4]  231 	ret
                                    232 ;	main.c: 166: static void pin_hiz(uint8_t is_track)
                                    233 ;	-----------------------------------------
                                    234 ;	 function pin_hiz
                                    235 ;	-----------------------------------------
      008126                        236 _pin_hiz:
      008126 88               [ 1]  237 	push	a
      008127 6B 01            [ 1]  238 	ld	(0x01, sp), a
                                    239 ;	main.c: 169: PC_DDR &= ~(1 << KEY_TRACK_BIT);
      008129 C6 50 0C         [ 1]  240 	ld	a, 0x500c
                                    241 ;	main.c: 168: if (is_track) {
      00812C 0D 01            [ 1]  242 	tnz	(0x01, sp)
      00812E 27 0B            [ 1]  243 	jreq	00102$
                                    244 ;	main.c: 169: PC_DDR &= ~(1 << KEY_TRACK_BIT);
      008130 A4 EF            [ 1]  245 	and	a, #0xef
      008132 C7 50 0C         [ 1]  246 	ld	0x500c, a
                                    247 ;	main.c: 170: PC_CR1 &= ~(1 << KEY_TRACK_BIT);
      008135 72 19 50 0D      [ 1]  248 	bres	0x500d, #4
      008139 20 09            [ 2]  249 	jra	00104$
      00813B                        250 00102$:
                                    251 ;	main.c: 172: PC_DDR &= ~(1 << KEY_VOLDOWN_BIT);
      00813B A4 F7            [ 1]  252 	and	a, #0xf7
      00813D C7 50 0C         [ 1]  253 	ld	0x500c, a
                                    254 ;	main.c: 173: PC_CR1 &= ~(1 << KEY_VOLDOWN_BIT);
      008140 72 17 50 0D      [ 1]  255 	bres	0x500d, #3
      008144                        256 00104$:
                                    257 ;	main.c: 175: }
      008144 84               [ 1]  258 	pop	a
      008145 81               [ 4]  259 	ret
                                    260 ;	main.c: 177: static void send_pulse(uint8_t is_track, uint16_t duration_ms)
                                    261 ;	-----------------------------------------
                                    262 ;	 function send_pulse
                                    263 ;	-----------------------------------------
      008146                        264 _send_pulse:
                                    265 ;	main.c: 179: pin_low(is_track);
      008146 88               [ 1]  266 	push	a
      008147 89               [ 2]  267 	pushw	x
      008148 CD 81 06         [ 4]  268 	call	_pin_low
      00814B 85               [ 2]  269 	popw	x
                                    270 ;	main.c: 180: delay_ms(duration_ms);
      00814C CD 80 CB         [ 4]  271 	call	_delay_ms
      00814F 84               [ 1]  272 	pop	a
                                    273 ;	main.c: 181: pin_hiz(is_track);
                                    274 ;	main.c: 182: }
      008150 CC 81 26         [ 2]  275 	jp	_pin_hiz
                                    276 ;	main.c: 188: static uint8_t button_fell(uint8_t reading,
                                    277 ;	-----------------------------------------
                                    278 ;	 function button_fell
                                    279 ;	-----------------------------------------
      008153                        280 _button_fell:
      008153 52 0F            [ 2]  281 	sub	sp, #15
      008155 6B 0F            [ 1]  282 	ld	(0x0f, sp), a
      008157 1F 0D            [ 2]  283 	ldw	(0x0d, sp), x
                                    284 ;	main.c: 193: uint32_t now = millis();
      008159 CD 80 C1         [ 4]  285 	call	_millis
      00815C 1F 03            [ 2]  286 	ldw	(0x03, sp), x
      00815E 17 01            [ 2]  287 	ldw	(0x01, sp), y
                                    288 ;	main.c: 195: if (reading != *lastState)
      008160 1E 0D            [ 2]  289 	ldw	x, (0x0d, sp)
      008162 F6               [ 1]  290 	ld	a, (x)
                                    291 ;	main.c: 196: *debounceTime = now;
      008163 1E 12            [ 2]  292 	ldw	x, (0x12, sp)
                                    293 ;	main.c: 195: if (reading != *lastState)
      008165 11 0F            [ 1]  294 	cp	a, (0x0f, sp)
      008167 27 07            [ 1]  295 	jreq	00102$
                                    296 ;	main.c: 196: *debounceTime = now;
      008169 16 03            [ 2]  297 	ldw	y, (0x03, sp)
      00816B EF 02            [ 2]  298 	ldw	(0x2, x), y
      00816D 16 01            [ 2]  299 	ldw	y, (0x01, sp)
      00816F FF               [ 2]  300 	ldw	(x), y
      008170                        301 00102$:
                                    302 ;	main.c: 199: if ((now - *debounceTime) > DEBOUNCE_DELAY) {
      008170 90 93            [ 1]  303 	ldw	y, x
      008172 90 EE 02         [ 2]  304 	ldw	y, (0x2, y)
      008175 17 07            [ 2]  305 	ldw	(0x07, sp), y
      008177 FE               [ 2]  306 	ldw	x, (x)
      008178 16 03            [ 2]  307 	ldw	y, (0x03, sp)
      00817A 72 F2 07         [ 2]  308 	subw	y, (0x07, sp)
      00817D 17 0B            [ 2]  309 	ldw	(0x0b, sp), y
      00817F 7B 02            [ 1]  310 	ld	a, (0x02, sp)
      008181 89               [ 2]  311 	pushw	x
      008182 12 02            [ 1]  312 	sbc	a, (2, sp)
      008184 85               [ 2]  313 	popw	x
      008185 6B 0A            [ 1]  314 	ld	(0x0a, sp), a
      008187 7B 01            [ 1]  315 	ld	a, (0x01, sp)
      008189 89               [ 2]  316 	pushw	x
      00818A 12 01            [ 1]  317 	sbc	a, (1, sp)
      00818C 85               [ 2]  318 	popw	x
      00818D 6B 09            [ 1]  319 	ld	(0x09, sp), a
      00818F AE 00 32         [ 2]  320 	ldw	x, #0x0032
      008192 13 0B            [ 2]  321 	cpw	x, (0x0b, sp)
      008194 4F               [ 1]  322 	clr	a
      008195 12 0A            [ 1]  323 	sbc	a, (0x0a, sp)
      008197 4F               [ 1]  324 	clr	a
      008198 12 09            [ 1]  325 	sbc	a, (0x09, sp)
      00819A 24 17            [ 1]  326 	jrnc	00108$
                                    327 ;	main.c: 200: if (reading != *currentState) {
      00819C 1E 14            [ 2]  328 	ldw	x, (0x14, sp)
      00819E F6               [ 1]  329 	ld	a, (x)
      00819F 11 0F            [ 1]  330 	cp	a, (0x0f, sp)
      0081A1 27 10            [ 1]  331 	jreq	00108$
                                    332 ;	main.c: 201: *currentState = reading;
      0081A3 7B 0F            [ 1]  333 	ld	a, (0x0f, sp)
      0081A5 F7               [ 1]  334 	ld	(x), a
                                    335 ;	main.c: 202: if (*currentState == 0) {
      0081A6 0D 0F            [ 1]  336 	tnz	(0x0f, sp)
      0081A8 26 09            [ 1]  337 	jrne	00108$
                                    338 ;	main.c: 203: *lastState = reading;
      0081AA 1E 0D            [ 2]  339 	ldw	x, (0x0d, sp)
      0081AC 7B 0F            [ 1]  340 	ld	a, (0x0f, sp)
      0081AE F7               [ 1]  341 	ld	(x), a
                                    342 ;	main.c: 204: return 1;
      0081AF A6 01            [ 1]  343 	ld	a, #0x01
      0081B1 20 06            [ 2]  344 	jra	00109$
      0081B3                        345 00108$:
                                    346 ;	main.c: 209: *lastState = reading;
      0081B3 1E 0D            [ 2]  347 	ldw	x, (0x0d, sp)
      0081B5 7B 0F            [ 1]  348 	ld	a, (0x0f, sp)
      0081B7 F7               [ 1]  349 	ld	(x), a
                                    350 ;	main.c: 210: return 0;
      0081B8 4F               [ 1]  351 	clr	a
      0081B9                        352 00109$:
                                    353 ;	main.c: 211: }
      0081B9 1E 10            [ 2]  354 	ldw	x, (16, sp)
      0081BB 5B 15            [ 2]  355 	addw	sp, #21
      0081BD FC               [ 2]  356 	jp	(x)
                                    357 ;	main.c: 217: static uint8_t can_trigger(uint32_t *lastActionTime, uint16_t cooldown)
                                    358 ;	-----------------------------------------
                                    359 ;	 function can_trigger
                                    360 ;	-----------------------------------------
      0081BE                        361 _can_trigger:
      0081BE 52 0C            [ 2]  362 	sub	sp, #12
                                    363 ;	main.c: 220: if ((millis() - *lastActionTime) >= cooldown)
      0081C0 89               [ 2]  364 	pushw	x
      0081C1 CD 80 C1         [ 4]  365 	call	_millis
      0081C4 1F 05            [ 2]  366 	ldw	(0x05, sp), x
      0081C6 85               [ 2]  367 	popw	x
      0081C7 E6 03            [ 1]  368 	ld	a, (0x3, x)
      0081C9 6B 08            [ 1]  369 	ld	(0x08, sp), a
      0081CB E6 02            [ 1]  370 	ld	a, (0x2, x)
      0081CD 6B 07            [ 1]  371 	ld	(0x07, sp), a
      0081CF FE               [ 2]  372 	ldw	x, (x)
      0081D0 1F 05            [ 2]  373 	ldw	(0x05, sp), x
      0081D2 1E 03            [ 2]  374 	ldw	x, (0x03, sp)
      0081D4 72 F0 07         [ 2]  375 	subw	x, (0x07, sp)
      0081D7 90 9F            [ 1]  376 	ld	a, yl
      0081D9 12 06            [ 1]  377 	sbc	a, (0x06, sp)
      0081DB 6B 0A            [ 1]  378 	ld	(0x0a, sp), a
      0081DD 90 9E            [ 1]  379 	ld	a, yh
      0081DF 12 05            [ 1]  380 	sbc	a, (0x05, sp)
      0081E1 16 0F            [ 2]  381 	ldw	y, (0x0f, sp)
      0081E3 17 07            [ 2]  382 	ldw	(0x07, sp), y
      0081E5 0F 06            [ 1]  383 	clr	(0x06, sp)
      0081E7 0F 05            [ 1]  384 	clr	(0x05, sp)
      0081E9 88               [ 1]  385 	push	a
      0081EA 13 08            [ 2]  386 	cpw	x, (0x08, sp)
      0081EC 7B 0B            [ 1]  387 	ld	a, (0x0b, sp)
      0081EE 12 07            [ 1]  388 	sbc	a, (0x07, sp)
      0081F0 84               [ 1]  389 	pop	a
      0081F1 12 05            [ 1]  390 	sbc	a, (0x05, sp)
      0081F3 25 03            [ 1]  391 	jrc	00102$
                                    392 ;	main.c: 221: return 1;
      0081F5 A6 01            [ 1]  393 	ld	a, #0x01
                                    394 ;	main.c: 222: return 0;
      0081F7 21                     395 	.byte 0x21
      0081F8                        396 00102$:
      0081F8 4F               [ 1]  397 	clr	a
      0081F9                        398 00103$:
                                    399 ;	main.c: 223: }
      0081F9 1E 0D            [ 2]  400 	ldw	x, (13, sp)
      0081FB 5B 10            [ 2]  401 	addw	sp, #16
      0081FD FC               [ 2]  402 	jp	(x)
                                    403 ;	main.c: 233: static void tim4_init(void)
                                    404 ;	-----------------------------------------
                                    405 ;	 function tim4_init
                                    406 ;	-----------------------------------------
      0081FE                        407 _tim4_init:
                                    408 ;	main.c: 235: TIM4_CR1  = 0x00;
      0081FE 35 00 53 40      [ 1]  409 	mov	0x5340+0, #0x00
                                    410 ;	main.c: 236: TIM4_PSCR = 0x03;   /* prescaler /8 (2^3)           */
      008202 35 03 53 47      [ 1]  411 	mov	0x5347+0, #0x03
                                    412 ;	main.c: 237: TIM4_ARR  = 62;     /* auto-reload : compte de 0→62 */
      008206 35 3E 53 48      [ 1]  413 	mov	0x5348+0, #0x3e
                                    414 ;	main.c: 238: TIM4_IER  = 0x01;   /* interruption débordement     */
      00820A 35 01 53 43      [ 1]  415 	mov	0x5343+0, #0x01
                                    416 ;	main.c: 239: TIM4_SR   = 0x00;   /* acquitter flag résiduel      */
      00820E 35 00 53 44      [ 1]  417 	mov	0x5344+0, #0x00
                                    418 ;	main.c: 240: TIM4_CR1  = 0x01;   /* démarrer (CEN=1)             */
      008212 35 01 53 40      [ 1]  419 	mov	0x5340+0, #0x01
                                    420 ;	main.c: 241: }
      008216 81               [ 4]  421 	ret
                                    422 ;	main.c: 247: static void hw_init(void)
                                    423 ;	-----------------------------------------
                                    424 ;	 function hw_init
                                    425 ;	-----------------------------------------
      008217                        426 _hw_init:
                                    427 ;	main.c: 250: CLK_CKDIVR = 0x1B;
      008217 35 1B 50 C6      [ 1]  428 	mov	0x50c6+0, #0x1b
                                    429 ;	main.c: 258: PA_ODR &= ~((1 << 1) | (1 << 2) | (1 << 3));
      00821B C6 50 00         [ 1]  430 	ld	a, 0x5000
      00821E A4 F1            [ 1]  431 	and	a, #0xf1
      008220 C7 50 00         [ 1]  432 	ld	0x5000, a
                                    433 ;	main.c: 259: PA_DDR |=   (1 << 1) | (1 << 2) | (1 << 3);
      008223 C6 50 02         [ 1]  434 	ld	a, 0x5002
      008226 AA 0E            [ 1]  435 	or	a, #0x0e
      008228 C7 50 02         [ 1]  436 	ld	0x5002, a
                                    437 ;	main.c: 260: PA_CR1 |=   (1 << 1) | (1 << 2) | (1 << 3);
      00822B C6 50 03         [ 1]  438 	ld	a, 0x5003
      00822E AA 0E            [ 1]  439 	or	a, #0x0e
      008230 C7 50 03         [ 1]  440 	ld	0x5003, a
                                    441 ;	main.c: 261: PA_CR2 &=  ~((1 << 1) | (1 << 2) | (1 << 3));  /* pas d'IRQ externe */
      008233 C6 50 04         [ 1]  442 	ld	a, 0x5004
      008236 A4 F1            [ 1]  443 	and	a, #0xf1
      008238 C7 50 04         [ 1]  444 	ld	0x5004, a
                                    445 ;	main.c: 263: PD_ODR &= ~(1 << 6);
      00823B 72 1D 50 10      [ 1]  446 	bres	0x5010, #6
                                    447 ;	main.c: 264: PD_DDR |=  (1 << 6);
      00823F 72 1C 50 13      [ 1]  448 	bset	0x5013, #6
                                    449 ;	main.c: 265: PD_CR1 |=  (1 << 6);
      008243 72 1C 50 14      [ 1]  450 	bset	0x5014, #6
                                    451 ;	main.c: 266: PD_CR2 &= ~(1 << 6);
      008247 72 1D 50 15      [ 1]  452 	bres	0x5015, #6
                                    453 ;	main.c: 272: PD_DDR &= ~(1 << BTN_NEXT_BIT);
      00824B 72 1B 50 13      [ 1]  454 	bres	0x5013, #5
                                    455 ;	main.c: 273: PD_CR1 |=  (1 << BTN_NEXT_BIT);
      00824F 72 1A 50 14      [ 1]  456 	bset	0x5014, #5
                                    457 ;	main.c: 274: PD_CR2 &= ~(1 << BTN_NEXT_BIT);
      008253 72 1B 50 15      [ 1]  458 	bres	0x5015, #5
                                    459 ;	main.c: 280: PC_DDR &= ~((1 << BTN_VOLD_BIT) | (1 << KEY_VOLDOWN_BIT) | (1 << KEY_TRACK_BIT));
      008257 C6 50 0C         [ 1]  460 	ld	a, 0x500c
      00825A A4 C7            [ 1]  461 	and	a, #0xc7
      00825C C7 50 0C         [ 1]  462 	ld	0x500c, a
                                    463 ;	main.c: 281: PC_CR1 |=  (1 << BTN_VOLD_BIT);
      00825F 72 1A 50 0D      [ 1]  464 	bset	0x500d, #5
                                    465 ;	main.c: 282: PC_CR1 &= ~((1 << KEY_VOLDOWN_BIT) | (1 << KEY_TRACK_BIT));
      008263 C6 50 0D         [ 1]  466 	ld	a, 0x500d
      008266 A4 E7            [ 1]  467 	and	a, #0xe7
      008268 C7 50 0D         [ 1]  468 	ld	0x500d, a
                                    469 ;	main.c: 283: PC_CR2 &= ~((1 << BTN_VOLD_BIT) | (1 << KEY_VOLDOWN_BIT) | (1 << KEY_TRACK_BIT));
      00826B C6 50 0E         [ 1]  470 	ld	a, 0x500e
      00826E A4 C7            [ 1]  471 	and	a, #0xc7
      008270 C7 50 0E         [ 1]  472 	ld	0x500e, a
                                    473 ;	main.c: 286: tim4_init();
      008273 CD 81 FE         [ 4]  474 	call	_tim4_init
                                    475 ;	main.c: 289: enableInterrupts();
      008276 9A               [ 1]  476 	rim
                                    477 ;	main.c: 290: }
      008277 81               [ 4]  478 	ret
                                    479 ;	main.c: 296: void main(void)
                                    480 ;	-----------------------------------------
                                    481 ;	 function main
                                    482 ;	-----------------------------------------
      008278                        483 _main:
                                    484 ;	main.c: 298: hw_init();
      008278 CD 82 17         [ 4]  485 	call	_hw_init
                                    486 ;	main.c: 300: lastStateNext = BTN_NEXT_READ();
      00827B C6 50 11         [ 1]  487 	ld	a, 0x5011
      00827E 4E               [ 1]  488 	swap	a
      00827F 44               [ 1]  489 	srl	a
      008280 A4 01            [ 1]  490 	and	a, #0x01
      008282 C7 00 05         [ 1]  491 	ld	_lastStateNext+0, a
                                    492 ;	main.c: 301: btnStateNext  = lastStateNext;
      008285 55 00 05 00 06   [ 1]  493 	mov	_btnStateNext+0, _lastStateNext+0
                                    494 ;	main.c: 302: lastStateVolD = BTN_VOLD_READ();
      00828A C6 50 0B         [ 1]  495 	ld	a, 0x500b
      00828D 4E               [ 1]  496 	swap	a
      00828E 44               [ 1]  497 	srl	a
      00828F A4 01            [ 1]  498 	and	a, #0x01
      008291 C7 00 0F         [ 1]  499 	ld	_lastStateVolD+0, a
                                    500 ;	main.c: 303: btnStateVolD  = lastStateVolD;
      008294 55 00 0F 00 10   [ 1]  501 	mov	_btnStateVolD+0, _lastStateVolD+0
                                    502 ;	main.c: 305: debounceNext   = millis();
      008299 CD 80 C1         [ 4]  503 	call	_millis
      00829C CF 00 09         [ 2]  504 	ldw	_debounceNext+2, x
      00829F 90 CF 00 07      [ 2]  505 	ldw	_debounceNext+0, y
                                    506 ;	main.c: 306: debounceVolD   = millis();
      0082A3 CD 80 C1         [ 4]  507 	call	_millis
      0082A6 CF 00 13         [ 2]  508 	ldw	_debounceVolD+2, x
      0082A9 90 CF 00 11      [ 2]  509 	ldw	_debounceVolD+0, y
                                    510 ;	main.c: 308: delay_ms(MP3_BOOT_DELAY);
      0082AD AE 03 E8         [ 2]  511 	ldw	x, #0x03e8
      0082B0 CD 80 CB         [ 4]  512 	call	_delay_ms
                                    513 ;	main.c: 313: lastActionNext = millis() - COOLDOWN_NEXT;
      0082B3 CD 80 C1         [ 4]  514 	call	_millis
      0082B6 1D 0F A0         [ 2]  515 	subw	x, #0x0fa0
      0082B9 24 02            [ 1]  516 	jrnc	00149$
      0082BB 90 5A            [ 2]  517 	decw	y
      0082BD                        518 00149$:
      0082BD CF 00 0D         [ 2]  519 	ldw	_lastActionNext+2, x
      0082C0 90 CF 00 0B      [ 2]  520 	ldw	_lastActionNext+0, y
                                    521 ;	main.c: 314: lastActionVolD = millis() - COOLDOWN_VOLD;
      0082C4 CD 80 C1         [ 4]  522 	call	_millis
      0082C7 1D 07 D0         [ 2]  523 	subw	x, #0x07d0
      0082CA 24 02            [ 1]  524 	jrnc	00150$
      0082CC 90 5A            [ 2]  525 	decw	y
      0082CE                        526 00150$:
      0082CE CF 00 17         [ 2]  527 	ldw	_lastActionVolD+2, x
      0082D1 90 CF 00 15      [ 2]  528 	ldw	_lastActionVolD+0, y
      0082D5                        529 00110$:
                                    530 ;	main.c: 323: &lastStateNext, &debounceNext, &btnStateNext)) {
      0082D5 AE 00 05         [ 2]  531 	ldw	x, #_lastStateNext+0
                                    532 ;	main.c: 322: if (button_fell(BTN_NEXT_READ(),
      0082D8 C6 50 11         [ 1]  533 	ld	a, 0x5011
      0082DB 4E               [ 1]  534 	swap	a
      0082DC 44               [ 1]  535 	srl	a
      0082DD A4 01            [ 1]  536 	and	a, #0x01
      0082DF 4B 06            [ 1]  537 	push	#<(_btnStateNext+0)
      0082E1 4B 00            [ 1]  538 	push	#((_btnStateNext+0) >> 8)
      0082E3 4B 07            [ 1]  539 	push	#<(_debounceNext+0)
      0082E5 4B 00            [ 1]  540 	push	#((_debounceNext+0) >> 8)
      0082E7 CD 81 53         [ 4]  541 	call	_button_fell
      0082EA 4D               [ 1]  542 	tnz	a
      0082EB 27 1F            [ 1]  543 	jreq	00104$
                                    544 ;	main.c: 324: if (can_trigger(&lastActionNext, COOLDOWN_NEXT)) {
      0082ED 4B A0            [ 1]  545 	push	#0xa0
      0082EF 4B 0F            [ 1]  546 	push	#0x0f
      0082F1 AE 00 0B         [ 2]  547 	ldw	x, #(_lastActionNext+0)
      0082F4 CD 81 BE         [ 4]  548 	call	_can_trigger
      0082F7 4D               [ 1]  549 	tnz	a
      0082F8 27 12            [ 1]  550 	jreq	00104$
                                    551 ;	main.c: 325: send_pulse(1, PULSE_NEXT);
      0082FA AE 00 FA         [ 2]  552 	ldw	x, #0x00fa
      0082FD A6 01            [ 1]  553 	ld	a, #0x01
      0082FF CD 81 46         [ 4]  554 	call	_send_pulse
                                    555 ;	main.c: 326: lastActionNext = millis();
      008302 CD 80 C1         [ 4]  556 	call	_millis
      008305 CF 00 0D         [ 2]  557 	ldw	_lastActionNext+2, x
      008308 90 CF 00 0B      [ 2]  558 	ldw	_lastActionNext+0, y
      00830C                        559 00104$:
                                    560 ;	main.c: 332: &lastStateVolD, &debounceVolD, &btnStateVolD)) {
      00830C AE 00 0F         [ 2]  561 	ldw	x, #_lastStateVolD+0
                                    562 ;	main.c: 331: if (button_fell(BTN_VOLD_READ(),
      00830F C6 50 0B         [ 1]  563 	ld	a, 0x500b
      008312 4E               [ 1]  564 	swap	a
      008313 44               [ 1]  565 	srl	a
      008314 A4 01            [ 1]  566 	and	a, #0x01
      008316 4B 10            [ 1]  567 	push	#<(_btnStateVolD+0)
      008318 4B 00            [ 1]  568 	push	#((_btnStateVolD+0) >> 8)
      00831A 4B 11            [ 1]  569 	push	#<(_debounceVolD+0)
      00831C 4B 00            [ 1]  570 	push	#((_debounceVolD+0) >> 8)
      00831E CD 81 53         [ 4]  571 	call	_button_fell
      008321 4D               [ 1]  572 	tnz	a
      008322 27 1E            [ 1]  573 	jreq	00108$
                                    574 ;	main.c: 333: if (can_trigger(&lastActionVolD, COOLDOWN_VOLD)) {
      008324 4B D0            [ 1]  575 	push	#0xd0
      008326 4B 07            [ 1]  576 	push	#0x07
      008328 AE 00 15         [ 2]  577 	ldw	x, #(_lastActionVolD+0)
      00832B CD 81 BE         [ 4]  578 	call	_can_trigger
      00832E 4D               [ 1]  579 	tnz	a
      00832F 27 11            [ 1]  580 	jreq	00108$
                                    581 ;	main.c: 334: send_pulse(0, PULSE_VOL_DOWN);
      008331 AE 05 DC         [ 2]  582 	ldw	x, #0x05dc
      008334 4F               [ 1]  583 	clr	a
      008335 CD 81 46         [ 4]  584 	call	_send_pulse
                                    585 ;	main.c: 335: lastActionVolD = millis();
      008338 CD 80 C1         [ 4]  586 	call	_millis
      00833B CF 00 17         [ 2]  587 	ldw	_lastActionVolD+2, x
      00833E 90 CF 00 15      [ 2]  588 	ldw	_lastActionVolD+0, y
      008342                        589 00108$:
                                    590 ;	main.c: 339: delay_ms(5);
      008342 AE 00 05         [ 2]  591 	ldw	x, #0x0005
      008345 CD 80 CB         [ 4]  592 	call	_delay_ms
      008348 20 8B            [ 2]  593 	jra	00110$
                                    594 ;	main.c: 341: }
      00834A 81               [ 4]  595 	ret
                                    596 	.area CODE
                                    597 	.area CONST
                                    598 	.area INITIALIZER
      008091                        599 __xinit__tick_ms:
      008091 00 00 00 00            600 	.byte #0x00, #0x00, #0x00, #0x00	; 0
      008095                        601 __xinit__lastStateNext:
      008095 01                     602 	.db #0x01	; 1
      008096                        603 __xinit__btnStateNext:
      008096 01                     604 	.db #0x01	; 1
      008097                        605 __xinit__debounceNext:
      008097 00 00 00 00            606 	.byte #0x00, #0x00, #0x00, #0x00	; 0
      00809B                        607 __xinit__lastActionNext:
      00809B 00 00 00 00            608 	.byte #0x00, #0x00, #0x00, #0x00	; 0
      00809F                        609 __xinit__lastStateVolD:
      00809F 01                     610 	.db #0x01	; 1
      0080A0                        611 __xinit__btnStateVolD:
      0080A0 01                     612 	.db #0x01	; 1
      0080A1                        613 __xinit__debounceVolD:
      0080A1 00 00 00 00            614 	.byte #0x00, #0x00, #0x00, #0x00	; 0
      0080A5                        615 __xinit__lastActionVolD:
      0080A5 00 00 00 00            616 	.byte #0x00, #0x00, #0x00, #0x00	; 0
                                    617 	.area CABS (ABS)
