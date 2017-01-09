global render
global  multiplyMat4
global  multiplyMatVec4
global  updateRotation
global  updatePosition
global  normalizeVert
section .data
const_fp_half   dd  0.5
const_fp_180    dd  180.0

section .text

multiplyMat4:
push        ebp
mov         ebp, esp
mov         eax, dword [ebp + 08h]   ;eax <- destination
mov         ebx, dword [ebp + 0Ch]   ;ebx <- matrixA
mov         ecx, dword [ebp + 10h]   ;ecx <- matrixB

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

mov         eax, dword [ebp + 08h]   ;eax <- destination
mov         edx, dword [ebp + 0Ch]   ;ebx <- matrixA
mov         ecx, dword [ebp + 10h]   ;ecx <- vector

movups      xmm1, [ecx]

movups      xmm0, [edx]
dpps        xmm0, xmm1, 0xFF
extractps   [eax], xmm0, 0x01

movups      xmm0, [edx + 10h]
dpps        xmm0, xmm1, 0xFF
extractps   [eax + 04h], xmm0, 0x01

movups      xmm0, [edx + 20h]
dpps        xmm0, xmm1, 0xFF
extractps   [eax + 08h], xmm0, 0x01

movups      xmm0, [edx + 30h]
dpps        xmm0, xmm1, 0xFF
extractps   [eax + 0Ch], xmm0, 0x01

mov         esp, ebp
pop         ebp
ret

updateRotation:
push        ebp
mov         ebp, esp

mov         eax, dword [ebp + 08h] ;matrix addr
mov         ebx, dword [ebp + 0Ch] ;rotation addr

sub         esp, 18h ;6*4 for sin cos

fld         dword [ebx + 00h]
fldpi
fmulp
fdiv        dword [const_fp_180]
fsincos
fstp        dword [ebp - 04h] ; cos(x)
fstp        dword [ebp - 08h] ; sin(x)

fld         dword [ebx + 04h]
fldpi
fmulp
fdiv        dword [const_fp_180]
fsincos
fstp        dword [ebp - 0Ch] ; cos(y)
fstp        dword [ebp - 10h] ; sin(y)

fld         dword [ebx + 08h]
fldpi
fmulp
fdiv        dword [const_fp_180]
fsincos
fstp        dword [ebp - 14h] ; cos(z)
fstp        dword [ebp - 18h] ; sin(z)

;#############################################################################################
;# cos(y)cos(z)   sin(x)sin(y)cos(z) - cos(x)sin(z)   cos(x)sin(y)cos(z) + sin(x)sin(z)  0   #
;# cos(y)sin(z)   sin(x)sin(y)sin(z) + cos(x)cos(z)   cos(x)sin(y)sin(z) - sin(x)cos(z)  0   #
;#   -sin(y)	          sin(x)cos(y)                          cos(x)cos(y)             0   #
;#      0                       0                                     0                  1   #
;#############################################################################################

fld         dword [ebp - 0Ch] ;cos(y)
fmul        dword [ebp - 14h] ;*cos(z)
fstp        dword [eax + 00h]

fld         dword [ebp - 08h] ; sin(x)
fmul        dword [ebp - 10h] ;*sin(y)
fmul        dword [ebp - 14h] ;*cos(z)
fld         dword [ebp - 04h] ; cos(x)
fmul        dword [ebp - 18h] ;*sin(z)
fsubp
fstp        dword [eax + 04h]

fld         dword [ebp - 04h] ; cos(x)
fmul        dword [ebp - 10h] ;*sin(y)
fmul        dword [ebp - 14h] ;*cos(z)
fld         dword [ebp - 08h] ; sin(x)
fmul        dword [ebp - 18h] ;*sin(z)
faddp
fstp        dword [eax + 08h]

mov         dword [eax + 0Ch], 0 ; 0

;# cos(y)sin(z)   sin(x)sin(y)sin(z) + cos(x)cos(z)   cos(x)sin(y)sin(z) - sin(x)cos(z)  0   #

fld         dword [ebp - 0Ch] ;cos(y)
fmul        dword [ebp - 18h] ;*sin(z)
fstp        dword [eax + 10h]

fld         dword [ebp - 08h] ; sin(x)
fmul        dword [ebp - 10h] ;*sin(y)
fmul        dword [ebp - 18h] ;*sin(z)
fld         dword [ebp - 04h] ; cos(x)
fmul        dword [ebp - 14h] ;*cos(z)
faddp
fstp        dword [eax + 14h]

fld         dword [ebp - 04h] ; cos(x)
fmul        dword [ebp - 10h] ;*sin(y)
fmul        dword [ebp - 18h] ;*sin(z)
fld         dword [ebp - 08h] ; sin(x)
fmul        dword [ebp - 14h] ;*cos(z)
fsubp
fstp        dword [eax + 18h]

mov         dword [eax + 1Ch], 0 ; 0

;#   -sin(y)	          sin(x)cos(y)                          cos(x)cos(y)            0   #    
fld         dword [ebp - 10h] ;sin(y)
fchs
fstp        dword [eax + 20h]

fld         dword [ebp - 08h] ; sin(x)
fmul        dword [ebp - 0Ch] ;*cos(y)
fstp        dword [eax + 24h]

fld         dword [ebp - 04h] ; cos(x)
fmul        dword [ebp - 0Ch] ;*cos(z)
fstp        dword [eax + 28h]

mov         dword [eax + 2Ch], 0 ; 0

; 0 0 0 1

mov         dword [eax + 30h], 0 ; 0
mov         dword [eax + 34h], 0 ; 0
mov         dword [eax + 38h], 0 ; 0
fld1
fstp         dword [eax + 3Ch]   ; 1



mov         esp, ebp
pop         ebp
ret

updatePosition:
push        ebp
mov         ebp, esp

mov         eax, dword [esp + 08h] ;matrix addr
mov         ebx, dword [esp + 0Ch] ;position addr

fld1

fst         dword [eax + 00h]
mov         dword [eax + 04h], 0
mov         dword [eax + 08h], 0
mov         ecx, dword [ebx + 00h]
mov         dword [eax + 0Ch], ecx

mov         dword [eax + 10h], 0
fst         dword [eax + 14h]
mov         dword [eax + 18h], 0
mov         ecx, dword [ebx + 04h]
mov         dword [eax + 1Ch], ecx

mov         dword [eax + 20h], 0
mov         dword [eax + 24h], 0
fst         dword [eax + 28h]
mov         ecx, dword [ebx + 08h]
mov         dword [eax + 2Ch], ecx

mov         dword [eax + 30h], 0
mov         dword [eax + 34h], 0
mov         dword [eax + 38h], 0
fstp        dword [eax + 3Ch]

mov         esp, ebp
pop         ebp
ret

normalizeVert:
push        ebp
mov         ebp, esp
;W-NORMALIZE
mov         eax, dword [ebp + 08h]  ;vector addr to eax
fld         dword [eax + 0Ch]
fabs
fstp        dword [eax + 0Ch]

fld         dword [eax + 00h]      ;load x to st(0)
fdiv        dword [eax + 0Ch]
fstp        dword [eax + 00h]

fld         dword [eax + 04h]      ;load y to st(0)
fdiv        dword [eax + 0Ch]
fstp        dword [eax + 04h]

fld         dword [eax + 08h]      ;load z to st(0)
fdiv        dword [eax + 0Ch]
fstp        dword [eax + 08h]

fld         dword [eax + 0Ch]      ;load w to st(0)
fdiv        dword [eax + 0Ch]
fstp        dword [eax + 0Ch]
; CLIP TO VIEWPORT
fld1        
fadd        dword [eax + 00h]      ;1 + x
fmul        dword [ebp + 0Ch]      ;* width
fmul        dword [const_fp_half]
fstp        dword [eax + 00h]

fld1        
fadd        dword [eax + 04h]      ;1 + y
fmul        dword [ebp + 10h]      ;* height
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
push        ebp
mov         ebp, esp

; int render(float* outputVerts, float* verts, int nVecs,                                                   ;08h, 0Ch, 10h
;             float* rotation, float* position, int rotationFlag,                                           ;14h, 18h, 1Ch
;             float fov, float width. float height,                                                         ;20h, 24h, 28h
;             float* rotationMatrix, float* translationMatrix, float* projectionMatrix, float* MVPMatrix    ;2Ch, 30h, 34h, 38h
;             );

;create rotation and translation matrices
push        dword [ebp + 14h] ; push rotation
push        dword [ebp + 2Ch] ; push rotationMatrix
call        updateRotation
add         esp, 8

push        dword [ebp + 18h] ; push translation
push        dword [ebp + 30h] ; push translationMatrix
call        updatePosition
add         esp, 8
push        dword [ebp + 2Ch] ;rotationMatrix
push        dword [ebp + 30h] ;translationMatrix
push        dword [ebp + 38h] ;MVPMatrix
call        multiplyMat4
add         esp, 0Ch

push        dword [ebp + 38h] ;MVPMatrix 
push        dword [ebp + 34h] ;projectionMatrix
push        dword [ebp + 38h] ;MVPMatrix
call        multiplyMat4
add         esp, 0Ch

mov         ecx, dword [ebp + 10h] ; nVerts
sub         ecx, 1
computeVerts:
push        ecx

mov         eax, dword [ebp + 0Ch] ; source verts
mov         ebx, ecx
lsl         ebx, 4
add         eax, ebx

push        eax
push        dword [ebp + 38h] ; MVP
mov         eax,  dword [ebp + 08h]  ; res verts
add         eax, ebx
push        eax

call        multiplyMatVec4
add         esp, 0Ch

push        dword [ebp + 28h]
push        dword [ebp + 24h]
mov         eax, dword [ebp + 08h] ; res verts
add         eax, ebx
push        eax
call        normalizeVert
add         esp, 0Ch

pop         ecx
dec         ecx
cmp         ecx, 0
jge        computeVerts

mov         esp, ebp
pop         ebp
ret
