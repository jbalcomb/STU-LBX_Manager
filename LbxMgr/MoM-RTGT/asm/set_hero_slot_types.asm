;.586	           ;Target processor.  Use instructions for Pentium class machines
.MODEL LARGE, C	 ;Use the flat memory model. Use C calling conventions
;.STACK	         ;Define a stack segment of 1KB (Not required for this example)
;.DATA	          ;Create a near data segment.  Local variables are declared after
	               ;this directive (Not required for this example)
.CODE	          ;Indicates the start of a code segment.

	; int __cdecl __far set_hero_slot_types(int unitType, int hero_slot_types)
	set_hero_slot_types      proc far       ; CODE XREF: j_set_hero_slot_types

	unitType                 = word ptr  6
	hero_slot_types          = word ptr  8

	offset_hero_type = 01ABh
	si_hero_slot_types = si
	cx_unitType = cx
	
	; ovr130:0000
	push   bp
	mov    bp, sp
	push   si_hero_slot_types
	mov    cx_unitType, [bp+unitType]
	mov    si_hero_slot_types, [bp+hero_slot_types]
	mov    ax, cx_unitType
	mov    dx, 24h
	imul   dx
	mov    bx, ax

	; ovr130:0013
	mov   al, offset_hero_type[bx]   ; AX = 0-3 as before, or 6 + P + 6 * (Q + 6 * R), where P,Q,R are the slot-types - 1
	xor   ah, ah
	cmp   ax, 6
	jge   new_method

	inc   ax
	mov   [si], ax        ; Set slot1 to 1-4 (Sword, Bow, Sword/Staff, Staff)
	mov   dx, 5
	mov   [si+2], dx      ; Set slot2 to 5 (Armor)
	inc   dx
	mov   [si+4], dx      ; Set slot3 to 6 (Ring)
	cmp   al, 4
	jnz   done
	mov   [si+2], dx      ; If AX is 4 (Wizard), overrule slot2 to 6 (Ring)
	jmp   short done
	
new_method:
	mov   cl, 6
	sub   al, cl          ; AX = P + 6 * (Q + 6 * R), where P,Q,R are the slot-types - 1
	cwd
	idiv  cl              ; AH = P, AL = Q + 6 * R
	mov   dl, ah
	inc   dx
	mov   [si], dx        ; Set slot 1 to <P>
	xor   ah, ah
	idiv  cl              ; AH = Q, AL = R
	mov   dl, ah
	inc   dx
	mov   [si+2], dx      ; Set slot 2 to <Q>
	mov   dl, al
	inc   dx
	mov   [si+4], dx      ; Set slot 3 to <R>

done:
	pop   si
	pop   bp
	retf

	; nops
	endp

slots	dw	0
	dw	0
	dw	0

main	proc far

	push offset slots
	push 11
	call set_hero_slot_types
	add sp, 6

	endp

	end main
	
