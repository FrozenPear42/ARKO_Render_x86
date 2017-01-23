global render

section .data
const_fp_half     dd  0.5
const_fp_180      dd  180.0
f_far             dd  500.0
f_near            dd  0.1

extern MVPMatrix
extern projectionMatrix
extern rotationMatrix
extern translationMatrix

section .text

multiplyMat4:
push        rbp
mov         rbp, rsp

insertps    xmm1, [rdx + 00h], 0x0E			;0b--ppzzzz load first column of B into xmm1
insertps    xmm1, [rdx + 10h], 0x10
insertps    xmm1, [rdx + 20h], 0x20
insertps    xmm1, [rdx + 30h], 0x30

insertps    xmm2, [rdx + 04h], 0x0E			;0b--ppzzzz load second column of B into xmm1
insertps    xmm2, [rdx + 14h], 0x10
insertps    xmm2, [rdx + 24h], 0x20
insertps    xmm2, [rdx + 34h], 0x30

insertps    xmm3, [rdx + 08h], 0x0E			;0b--ppzzzz load third column of B into xmm1
insertps    xmm3, [rdx + 18h], 0x10
insertps    xmm3, [rdx + 28h], 0x20
insertps    xmm3, [rdx + 38h], 0x30

insertps    xmm4, [rdx + 0Ch], 0x0E			;0b--ppzzzz load fourth column of B into xmm1
insertps    xmm4, [rdx + 1Ch], 0x10
insertps    xmm4, [rdx + 2Ch], 0x20
insertps    xmm4, [rdx + 3Ch], 0x30

xorps       xmm0,  xmm0
movups      xmm8,  [rsi + 00h]              ;load first row of A into xmm0
movups      xmm9,  xmm8
movups      xmm10, xmm8
movups      xmm11, xmm8
dpps        xmm8,  xmm1, 0xF1
dpps        xmm9,  xmm2, 0xF2
dpps        xmm10, xmm3, 0xF4
dpps        xmm11, xmm4, 0xF8
orps        xmm0,  xmm8
orps        xmm0,  xmm9
orps        xmm0,  xmm10
orps        xmm0,  xmm11
movups      [rdi + 00h], xmm0

xorps       xmm0,  xmm0
movups      xmm8,  [rsi + 10h]              ;load first row of A into xmm0
movups      xmm9,  xmm8
movups      xmm10, xmm8
movups      xmm11, xmm8
dpps        xmm8,  xmm1, 0xF1
dpps        xmm9,  xmm2, 0xF2
dpps        xmm10, xmm3, 0xF4
dpps        xmm11, xmm4, 0xF8
orps        xmm0,  xmm8
orps        xmm0,  xmm9
orps        xmm0,  xmm10
orps        xmm0,  xmm11
movups      [rdi + 10h], xmm0

xorps       xmm0,  xmm0
movups      xmm8,  [rsi + 20h]              ;load first row of A into xmm0
movups      xmm9,  xmm8
movups      xmm10, xmm8
movups      xmm11, xmm8
dpps        xmm8,  xmm1, 0xF1
dpps        xmm9,  xmm2, 0xF2
dpps        xmm10, xmm3, 0xF4
dpps        xmm11, xmm4, 0xF8
orps        xmm0,  xmm8
orps        xmm0,  xmm9
orps        xmm0,  xmm10
orps        xmm0,  xmm11
movups      [rdi + 20h], xmm0

xorps       xmm0,  xmm0
movups      xmm8,  [rsi + 30h]              ;load first row of A into xmm0
movups      xmm9,  xmm8
movups      xmm10, xmm8
movups      xmm11, xmm8
dpps        xmm8,  xmm1, 0xF1
dpps        xmm9,  xmm2, 0xF2
dpps        xmm10, xmm3, 0xF4
dpps        xmm11, xmm4, 0xF8
orps        xmm0,  xmm8
orps        xmm0,  xmm9
orps        xmm0,  xmm10
orps        xmm0,  xmm11
movups      [rdi + 30h], xmm0

pop         rbp
ret

multiplyMatVec4:
push        rbp
mov         rbp, rsp

xorps       xmm0, xmm0

movups      xmm1, [rdx]
movups      xmm2, [rsi]
movups      xmm3, [rsi + 10h]
movups      xmm4, [rsi + 20h]
movups      xmm5, [rsi + 30h]

dpps        xmm2, xmm1, 0xF1
dpps        xmm3, xmm1, 0xF2
dpps        xmm4, xmm1, 0xF4
dpps        xmm5, xmm1, 0xF8

orps        xmm0, xmm2
orps        xmm0, xmm3
orps        xmm0, xmm4
orps        xmm0, xmm5
movups      [rdi], xmm0

pop         rbp
ret

;float projectionMatrix[16] = {
;        1 / (std::tan(fov / 2) * (width / height)), 0, 0, 0,
;        0, 1 / std::tan(fov / 2), 0, 0,
;        0, 0, (far + near) / (far - near), -2 * far * near / (far - near),
;        0, 0, 1, 0
;};
; matrix - rdi
; fov - xmm0
; width - xmm1
; height - xmm2

updateProjection:
push        rbp
mov         rbp, rsp
sub         rsp, 4

movss       dword [rbp - 04h], xmm2
fld         dword [rbp - 04h]       ; height
movss       dword [rbp - 04h], xmm1
fld         dword [rbp - 04h]       ; width
fdivp                               ; 1/(width/height)

movss       dword [rbp - 04h], xmm0
fld         dword [rbp - 04h] ; fov
fldpi
fmulp
fld         dword [const_fp_180]
fdivp

fld         dword [const_fp_half]
fmulp
fptan
fdivrp
fst         dword [rdi + 14h]
fmulp
fstp        dword [rdi + 00h]

fld         dword [f_near]
fld         dword [f_far]
faddp
fld         dword [f_far]
fld         dword [f_near]
fsubp
fdivp
fstp        dword [rdi + 28h]


fld         dword [f_far]
fld         dword [f_near]
fsub
fld         dword [const_fp_half]
fmulp
fld         dword [f_far]
fld         dword [f_near]
fmulp
fdivrp
fchs
fstp        dword [rdi + 2Ch]

fld1
fstp        dword [rdi + 38h]

add         rsp, 4
pop         rbp
ret

updateRotation:
push        rbp
mov         rbp, rsp

sub         rsp, 18h ;6*4 for sin cos

fld         dword [rsi + 00h]
fldpi
fmulp
fdiv        dword [const_fp_180]
fsincos
fstp        dword [rbp - 04h] ; cos(x)
fstp        dword [rbp - 08h] ; sin(x)

fld         dword [rsi + 04h]
fldpi
fmulp
fdiv        dword [const_fp_180]
fsincos
fstp        dword [rbp - 0Ch] ; cos(y)
fstp        dword [rbp - 10h] ; sin(y)

fld         dword [rsi + 08h]
fldpi
fmulp
fdiv        dword [const_fp_180]
fsincos
fstp        dword [rbp - 14h] ; cos(z)
fstp        dword [rbp - 18h] ; sin(z)

;#############################################################################################
;# cos(y)cos(z)   sin(x)sin(y)cos(z) - cos(x)sin(z)   cos(x)sin(y)cos(z) + sin(x)sin(z)  0   #
;# cos(y)sin(z)   sin(x)sin(y)sin(z) + cos(x)cos(z)   cos(x)sin(y)sin(z) - sin(x)cos(z)  0   #
;#   -sin(y)	          sin(x)cos(y)                          cos(x)cos(y)             0   #
;#      0                       0                                     0                  1   #
;#############################################################################################

fld         dword [rbp - 0Ch] ;cos(y)
fmul        dword [rbp - 14h] ;*cos(z)
fstp        dword [rdi + 00h]

fld         dword [rbp - 08h] ; sin(x)
fmul        dword [rbp - 10h] ;*sin(y)
fmul        dword [rbp - 14h] ;*cos(z)
fld         dword [rbp - 04h] ; cos(x)
fmul        dword [rbp - 18h] ;*sin(z)
fsubp
fstp        dword [rdi + 04h]

fld         dword [rbp - 04h] ; cos(x)
fmul        dword [rbp - 10h] ;*sin(y)
fmul        dword [rbp - 14h] ;*cos(z)
fld         dword [rbp - 08h] ; sin(x)
fmul        dword [rbp - 18h] ;*sin(z)
faddp
fstp        dword [rdi + 08h]

mov         dword [rdi + 0Ch], 0 ; 0

;# cos(y)sin(z)   sin(x)sin(y)sin(z) + cos(x)cos(z)   cos(x)sin(y)sin(z) - sin(x)cos(z)  0   #

fld         dword [rbp - 0Ch] ;cos(y)
fmul        dword [rbp - 18h] ;*sin(z)
fstp        dword [rdi + 10h]

fld         dword [rbp - 08h] ; sin(x)
fmul        dword [rbp - 10h] ;*sin(y)
fmul        dword [rbp - 18h] ;*sin(z)
fld         dword [rbp - 04h] ; cos(x)
fmul        dword [rbp - 14h] ;*cos(z)
faddp
fstp        dword [rdi + 14h]

fld         dword [rbp - 04h] ; cos(x)
fmul        dword [rbp - 10h] ;*sin(y)
fmul        dword [rbp - 18h] ;*sin(z)
fld         dword [rbp - 08h] ; sin(x)
fmul        dword [rbp - 14h] ;*cos(z)
fsubp
fstp        dword [rdi + 18h]

mov         dword [rdi + 1Ch], 0 ; 0

;#   -sin(y)	          sin(x)cos(y)                          cos(x)cos(y)            0   #    
fld         dword [rbp - 10h] ;sin(y)
fchs
fstp        dword [rdi + 20h]

fld         dword [rbp - 08h] ; sin(x)
fmul        dword [rbp - 0Ch] ;*cos(y)
fstp        dword [rdi + 24h]

fld         dword [rbp - 04h] ; cos(x)
fmul        dword [rbp - 0Ch] ;*cos(z)
fstp        dword [rdi + 28h]

mov         dword [rdi + 2Ch], 0 ; 0

; 0 0 0 1

mov         dword [rdi + 30h], 0 ; 0
mov         dword [rdi + 34h], 0 ; 0
mov         dword [rdi + 38h], 0 ; 0
fld1
fstp         dword [rdi + 3Ch]   ; 1

add         rsp, 18h
pop         rbp
ret

updatePosition:
push        rbp
mov         rbp, rsp

fld1
fst         dword [rdi + 00h]
mov         dword [rdi + 04h], 0
mov         dword [rdi + 08h], 0
mov         eax, dword [rsi + 00h]
mov         dword [rdi + 0Ch], eax

mov         dword [rdi + 10h], 0
fst         dword [rdi + 14h]
mov         dword [rdi + 18h], 0
mov         eax, dword [rsi + 04h]
mov         dword [rdi + 1Ch], eax

mov         dword [rdi + 20h], 0
mov         dword [rdi + 24h], 0
fst         dword [rdi + 28h]
mov         eax, dword [rsi + 08h]
mov         dword [rdi + 2Ch], eax

mov         dword [rdi + 30h], 0
mov         dword [rdi + 34h], 0
mov         dword [rdi + 38h], 0
fstp        dword [rdi + 3Ch]

pop         rbp
ret

normalizeVert:
push        rbp
mov         rbp, rsp
sub         rsp, 4
;W-NORMALIZE

fld         dword [rdi + 0Ch]
fabs
fstp        dword [rdi + 0Ch]

fld         dword [rdi + 00h]      ;load x to st(0)
fdiv        dword [rdi + 0Ch]
fstp        dword [rdi + 00h]

fld         dword [rdi + 04h]      ;load y to st(0)
fdiv        dword [rdi + 0Ch]
fstp        dword [rdi + 04h]

fld         dword [rdi + 08h]      ;load z to st(0)
fdiv        dword [rdi + 0Ch]
fstp        dword [rdi + 08h]

fld         dword [rdi + 0Ch]      ;load w to st(0)
fdiv        dword [rdi + 0Ch]
fstp        dword [rdi + 0Ch]
; CLIP TO VIEWPORT
fld1        
fadd        dword [rdi + 00h]      ;1 + x
movss       dword [rbp - 04h], xmm0
fmul        dword [rbp - 04h]      ;* width
fmul        dword [const_fp_half]
fstp        dword [rdi + 00h]

fld1
fadd        dword [rdi + 04h]      ;1 + y
movss       dword [rbp - 04h], xmm1
fmul        dword [rbp - 04h]      ;* height
fmul        dword [const_fp_half]
fstp        dword [rdi + 04h]

fld1        
fadd        dword [rdi + 08h]      ;1 + z
fmul        dword [const_fp_half]
fstp        dword [rdi + 08h]

mov         rsp, rbp
pop         rbp
ret

; int render(float* outputVerts, float* vertices, int nVecs,       ;rdi, rsi, rdx
;             float* rotation, float* position, int rotationFlag,  ;rcx, r8 , r9
;             float fov, float width. float height,                ;xmm0, xmm1, xmm2
;             );
render:
push        rbp
mov         rbp, rsp

movss       xmm14, xmm1 ;width
movss       xmm15, xmm2 ;height

push        rdi
push        rsi
push        rdx
push        rcx

; args in xmm0, xmm1, xmm2
mov         rdi,  qword projectionMatrix  ;push matrix
call        updateProjection

;create rotation and translation matrices
mov         rdi, qword rotationMatrix    ; rotationMatrix
pop         rsi                          ; rotation
call        updateRotation

mov         rdi, qword translationMatrix ; translationMatrix
mov         rsi, r8                      ; translation
call        updatePosition

mov         rdi, qword MVPMatrix
mov         rsi, qword translationMatrix
mov         rdx, qword rotationMatrix
call        multiplyMat4

mov         rdi, qword MVPMatrix
mov         rsi, qword projectionMatrix
mov         rdx, qword MVPMatrix
call        multiplyMat4

mov         rcx, qword [rbp - 18h]       ;nVerts
sub         rcx, 1
computeVerts:
mov         rdi, qword [rbp - 08h]       ;out vertices
lea         rdi, [rdi + 8*rcx]
lea         rdi, [rdi + 8*rcx]
mov         rsi, qword MVPMatrix         ;MVP
mov         rdx, qword [rbp - 10h]       ;source vertices
lea         rdx, [rdx + 8*rcx]
lea         rdx, [rdx + 8*rcx]
call        multiplyMatVec4 ;dest, mat, vec

mov         rdi, qword [rbp - 08h]       ;out vertices
lea         rdi, [rdi + 8*rcx]
lea         rdi, [rdi + 8*rcx]
movss       xmm0, xmm14
movss       xmm1, xmm15
call        normalizeVert
dec         rcx
cmp         rcx, 0
jge         computeVerts

mov         rsp, rbp
pop         rbp
ret
