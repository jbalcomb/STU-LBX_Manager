.8086                   ;Target processor.
.MODEL LARGE, C         ;Use the flat memory model. Use C calling conventions

dseg            segment byte public 'DATA' use16

ORG 922Ah
addr_Battle_unit        dd 0

ORG 9EC2h
addr_Unit_1009_20h      dd 0

dseg            ends

ovr116          segment byte public 'CODE' use16
                assume cs:ovr116
                assume es:nothing, ss:nothing, ds:dseg

ORG 1CB1h

; Attributes: bp-based frame

Experience_bonus proc far ; CODE XREF: ___61J UNIT_battledat_combine+169p

unitNr=	word ptr  6
ptrBattle_Unit=	dword ptr  8

si_Level = si
                push	bp
                mov	bp, sp
                push	si_Level
                push	di
                mov	di, [bp+unitNr]
                mov	ax, di
                mov	cl, 5
                shl	ax, cl
                les	bx, addr_Unit_1009_20h
                add	bx, ax
                mov	al, es:[bx+0Ch]
                cbw
                mov	si_Level, ax	; level
                mov	ax, di
                mov	cl, 5
                shl	ax, cl
                les	bx, addr_Unit_1009_20h
                add	bx, ax
                mov	ax, es:[bx+1Ah]
                mov	dx, es:[bx+18h]
                and	dx, 0
                and	ax, 100h	; heroism
                or	dx, ax
                jnz	short loc_9917F

                les	bx, [bp+ptrBattle_Unit]
                mov	ax, es:[bx+3Ch]
                mov	dx, es:[bx+3Ah]
                and	dx, 0
                and	ax, 100h
                or	dx, ax
                jz	short loc_99187

loc_9917F:		; CODE XREF: Experience_bonus+37j
                cmp	si_Level, 3	; heroism
                jge	short loc_99187

                mov	si_Level, 3

loc_99187:		; CODE XREF: Experience_bonus+4Cj Experience_bonus+51j
                mov	ax, di
                mov	cl, 5
                shl	ax, cl
                les	bx, addr_Unit_1009_20h
                add	bx, ax
                cmp	byte ptr es:[bx+6], -1 ; hero?

;; BEGIN CHANGED CODE
                jmp     after_unit_table

BONUS_RECORD_SIZE = 16h
                ;  Me Ra    Sh Th Df Re Mv                 CFg       Hp Sc Tr Fg Cs Ga
unit_table      db +0,+0, 0, 0, 0, 0,+0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0  ; Lvl 1
                db +1,+1, 0, 0, 0, 0,+1, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0  ; Lvl 2
                db +1,+1, 0, 0, 0,+1,+2, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0  ; Lvl 3
                db +2,+2, 0, 0,+1,+1,+3, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0  ; Lvl 4
                db +2,+2, 0, 0,+2,+2,+4, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0  ; Lvl 5
                db +3,+3, 0, 0,+3,+2,+5, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0  ; Lvl 6

after_unit_table:
                jmp     after_tables            ; // Intermediate jump
                
                ;  Me Ra    Sh Th Df Re Mv                 CFg       Hp Sc Tr Fg Cs Ga
hero_table      db +0,+0, 0, 0, 0, 0,+0, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0  ; Lvl 1
                db +1,+1, 0, 0, 0,+1,+1, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0  ; Lvl 2
                db +2,+2, 0, 0,+1,+1,+2, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0  ; Lvl 3
                db +3,+3, 0, 0,+1,+2,+3, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0  ; Lvl 4
                db +4,+4, 0, 0,+1,+2,+4, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0  ; Lvl 5
                db +5,+5, 0, 0,+2,+3,+5, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0  ; Lvl 6
                db +6,+6, 0, 0,+2,+3,+6, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0  ; Lvl 7
                db +7,+7, 0, 0,+2,+4,+7, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0  ; Lvl 8
                db +8,+8, 0, 0,+3,+4,+8, 0,  0, 0, 0, 0, 0, 0, 0, 0,  0, 0, 0, 0, 0, 0  ; Lvl 9

after_tables:
                jg      short hero_identified   ; if (regular unit)

                mov     bx, OFFSET unit_table   ;     bonus = unit_table;
                jmp     short process_level
                
hero_identified:                                ; else
                mov     bx, OFFSET hero_table   ;     bonus = hero_table;

process_level:
                push    ds                      ; SAVE DS
                
                mov     ax, si_Level
                mov     dx, BONUS_RECORD_SIZE
                imul    dx
                add     bx, ax                  ; bonus += level * BONUS_RECORD_SIZE;
                
                les     di, [bp+ptrBattle_Unit]

                push    cs
                pop     ds
                mov     si, bx
                
                cld
                xor     cx, cx                  ; for (int i = 0; i < BONUS_RECORD_SIZE; ++i)
for_ability:
                mov     bl, es:[di]                     
                cmp     bl, 0                   ;     if (battleUnit[i] > 0
                ja      positive_ability
                jb      negative_ability
                cmp     cx, 4                   ;         || i == TO_HIT)
                je      positive_ability
                jmp     short skip_ability

positive_ability:
                lodsb                           ;         battleUnit[i] += bonus[i];
                add     al, bl
                stosb
                jmp     short fi_ability
                
negative_ability:                               ;     else if (battleUnit[i] < 0)   // Gaze modifier (versus Poison)
                lodsb                           ;         battleUnit[i] -= bonus[i];
                neg     al
                add     al, bl
                stosb
                jmp     short fi_ability
                
;;flag_ability:
;;                lodsw                           ;         battleUnit[i] |= bonus[i];
;;                or      ax, bx
;;                stosw
;;                jmp     short fi_ability

skip_ability:
                inc     si
                inc     di

fi_ability:
                inc     cx
                cmp     cx, BONUS_RECORD_SIZE
                jl      for_ability

endfor_ability:
                pop     ds                      ; RESTORE DS

                jmp     loc_993EE
                
;; END CHANGED CODE

ORG 1F6Eh
loc_993EE:		   ; CODE XREF:	Experience_bonus:loc_9932Dj
			   ; Experience_bonus+28Dj
			   ; Experience_bonus+2B5j
                pop	   di
                pop	   si_Level
                pop	   bp
                retf

Experience_bonus endp

                ;; ...

ovr116          ends

                end 