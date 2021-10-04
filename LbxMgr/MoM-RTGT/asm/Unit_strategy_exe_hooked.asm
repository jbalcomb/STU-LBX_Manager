.8086                   ;Target processor.
.MODEL LARGE, C         ;Use the flat memory model. Use C calling conventions

dseg            segment byte public 'DATA' use16

ORG 922Ah
addr_Battle_unit        dd 0

ORG 9294h
baseptr                 dw 0
ORG 9296h
bait                    dw 0

dseg            ends

ovr114          segment byte public 'CODE' use16
                assume cs:ovr114
                assume es:nothing, ss:nothing, ds:dseg

ORG 0344h

Unit_strategy_exe       proc far       ; CODE XREF: ___69
                                       ; Army_autocombat+2E0
                                       ; Army_autocombat+3A2

parm1                    = word ptr -0Ah
parm2                    = word ptr -8
target_unit_at_gate      = word ptr -6
casting_failed           = word ptr -4
targetID_in_gate_OR_moves_left= word ptr -2
battleUnitID             = word ptr  6
target_xpos              = word ptr  8
target_ypos              = word ptr  0Ah

si_battleUnitID = si
di_target_xpos = di
                ;; Original
                push    bp
                mov     bp, sp
                sub     sp, 0Ah
                push    si
                push    di

                ;; Added
                mov     ax, bp    
                mov     [baseptr], ax               ; Make stack variables accessible
                mov     ax, 1                       ;; The "hook"
                mov     [bait], ax
loc_wait_external_AI:
                mov     ax, [bait]
                or      ax, ax
                jnz     loc_wait_external_AI

                xor     ax, ax
                mov     [baseptr], ax               ; Restore dseg:9294

                ;; Original
                mov     si_battleUnitID, [bp+battleUnitID]
                mov     di_target_xpos, [bp+target_xpos]
                or      di_target_xpos, di_target_xpos
                jnz     short loc_92521

                cmp     [bp+target_ypos], 0
                jnz     short loc_92521

                mov     ax, si_battleUnitID
                mov     dx, 6Eh ; 'n'
                imul    dx
                les     bx, addr_Battle_unit
                add     bx, ax
                mov     al, es:[bx+43h]
                cbw
                mov     dx, 6Eh ; 'n'
                imul    dx
                les     bx, addr_Battle_unit
                add     bx, ax
                mov     di_target_xpos, es:[bx+44h]
                mov     [bp+target_ypos], ax
                
                ;; Jump to regular continuation of code (could also be NOP's)
                jmp     loc_92521

                ;; Original
ORG 03A1h
loc_92521:                              ; CODE XREF: Unit_strategy_exe+10
                                        ; Unit_strategy_exe+16
                push   si_battleUnitID ; battleUnitID
                ;; call   j_choose_next_unit_for_action
                ;; ...

Unit_strategy_exe       endp

                ;; ...

ovr114          ends

                end 