
global  multiplyMat4
global  multiplyMatVec4
global  updateRotation
global  updatePosition
global  normalizeVert

section .data
const_fp_half   dd  0.5

section .text

multiplyMat4:
push        ebp
mov         ebp, esp
mov         eax, dword [esp + 08h]   ;eax <- destination
mov         ebx, dword [esp + 0Ch]   ;ebx <- matrixA
mov         ecx, dword [esp + 10h]   ;ecx <- matrixB

insertps    xmm1, [ecx + 00h], 0x0E			;0b--ppzzzz load first column of B into xmm1
insertps    xmm1, [ecx + 10h], 0x10           
insertps    xmm1, [ecx + 20h], 0x20           
insertps    xmm1, [ecx + 30h], 0x30           

insertps    xmm2, [ecx + 04h], 0x0E			;0b--ppzzzz load second column of B into xmm1
insertps    xmm2, [ecx + 14h], 0x10           
insertps    xmm2, [ecx + 24h], 0x20           
insertps    xmm2, [ecx + 34h], 0x30           

insertps    xmm3, [ecx + 08h], 0x0E			;0b--ppzzzz load third column of B into xmm1
insertps    xmm3, [ecx + 18h], 0x10           
insertps    xmm3, [ecx + 28h], 0x20           
insertps    xmm3, [ecx + 38h], 0x30           

insertps    xmm4, [ecx + 0Ch], 0x0E			;0b--ppzzzz load fourth column of B into xmm1
insertps    xmm4, [ecx + 1Ch], 0x10           
insertps    xmm4, [ecx + 2Ch], 0x20           
insertps    xmm4, [ecx + 3Ch], 0x30           

movups      xmm0, [ebx + 00h]              ;load first row of A into xmm0

movups		xmm5, xmm0
dpps        xmm5, xmm1, 0xFF
extractps   [eax + 00h], xmm5, 0x01

movups		xmm5, xmm0
dpps        xmm5, xmm2, 0xFF
extractps   [eax + 04h], xmm5, 0x01

movups		xmm5, xmm0
dpps        xmm5, xmm3, 0xFF
extractps   [eax + 08h], xmm5, 0x01

movups		xmm5, xmm0
dpps        xmm5, xmm4, 0xFF
extractps   [eax + 0Ch], xmm5, 0x01

movups      xmm0, [ebx + 10h]              ;load second row of A into xmm0

movups		xmm5, xmm0
dpps        xmm5, xmm1, 0xFF
extractps   [eax + 10h], xmm5, 0x01

movups		xmm5, xmm0
dpps        xmm5, xmm2, 0xFF
extractps   [eax + 14h], xmm5, 0x01

movups		xmm5, xmm0
dpps        xmm5, xmm3, 0xFF
extractps   [eax + 18h], xmm5, 0x01

movups		xmm5, xmm0
dpps        xmm5, xmm4, 0xFF
extractps   [eax + 1Ch], xmm5, 0x01

movups      xmm0, [ebx + 20h]              ;load first row of A into xmm0

movups		xmm5, xmm0
dpps        xmm5, xmm1, 0xFF
extractps   [eax + 20h], xmm5, 0x01

movups		xmm5, xmm0
dpps        xmm5, xmm2, 0xFF
extractps   [eax + 24h], xmm5, 0x01

movups		xmm5, xmm0
dpps        xmm5, xmm3, 0xFF
extractps   [eax + 28h], xmm5, 0x01

movups		xmm5, xmm0
dpps        xmm5, xmm4, 0xFF
extractps   [eax + 2Ch], xmm5, 0x01

movups      xmm0, [ebx + 30h]              ;load first row of A into xmm0

movups		xmm5, xmm0
dpps        xmm5, xmm1, 0xFF
extractps   [eax + 30h], xmm5, 0x01

movups		xmm5, xmm0
dpps        xmm5, xmm2, 0xFF
extractps   [eax + 34h], xmm5, 0x01

movups		xmm5, xmm0
dpps        xmm5, xmm3, 0xFF
extractps   [eax + 38h], xmm5, 0x01

movups		xmm5, xmm0
dpps        xmm5, xmm4, 0xFF
extractps   [eax + 3Ch], xmm5, 0x01

mov     esp, ebp
pop     ebp
ret

multiplyMatVec4:
push        ebp
mov         ebp, esp

mov         eax, dword [esp + 08h]   ;eax <- destination
mov         ebx, dword [esp + 0Ch]   ;ebx <- matrixA
mov         ecx, dword [esp + 10h]   ;ecx <- vector

movups      xmm1, [ecx]

movups      xmm0, [ebx]
dpps        xmm0, xmm1, 0xFF
extractps   [eax], xmm0, 0x01

movups      xmm0, [ebx + 10h]
dpps        xmm0, xmm1, 0xFF
extractps   [eax + 04h], xmm0, 0x01

movups      xmm0, [ebx + 20h]
dpps        xmm0, xmm1, 0xFF
extractps   [eax + 08h], xmm0, 0x01

movups      xmm0, [ebx + 30h]
dpps        xmm0, xmm1, 0xFF
extractps   [eax + 0Ch], xmm0, 0x01

mov         esp, ebp
pop         ebp
ret

updateRotation:
push        ebp
mov         ebp, esp

mov         esp, ebp
pop         ebp
ret

updatePosition:
push        ebp
mov         ebp, esp

mov         esp, ebp
pop         ebp
ret

normalizeVert:
push        ebp
mov         ebp, esp
;W-NORMALIZE
mov         eax, dword [esp + 08h]  ;vector addr to eax

fld         dword [eax + 0Ch]
fabs
fstp        dword [eax + 0Ch]

fld         dword [eax + 00h]      ;load x to st(0)
fdiv        dword [eax + 0Ch]
fstp        dword [eax + 00h]

fld         dword [eax + 04h]      ;load y to st(0)
fdiv        dword [eax + 0Ch]
fstp        dword [eax + 04h]

fld         dword [eax + 04h]      ;load z to st(0)
fdiv        dword [eax + 0Ch]
fstp        dword [eax + 04h]

fld         dword [eax + 0Ch]      ;load w to st(0)
fdiv        dword [eax + 0Ch]
fstp        dword [eax + 0Ch]
; CLIP TO VIEWPORT
fld1        
fadd        dword [eax + 00h]      ;1 + x
fmul        dword [esp + 0Ch]      ;* width
fmul        dword [const_fp_half]
fstp        dword [eax + 00h]

fld1        
fadd        dword [eax + 04h]      ;1 + y
fmul        dword [esp + 10h]      ;* height
fmul        dword [const_fp_half]
fstp        dword [eax + 04h]

fld1        
fadd        dword [eax + 08h]      ;1 + z
fmul        dword [const_fp_half]
fstp        dword [eax + 08h]

mov         esp, ebp
pop         ebp
ret

render:
