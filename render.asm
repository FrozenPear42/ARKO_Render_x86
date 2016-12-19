section text

global  matrix_multiply
global  render

extern rotationMatrix       ;double 16
extern translationMatrix    ;double 16
extern projectionMatrix     ;double 16
extern MVPMatrix            ;double 16
extern verts                ;double 
extern vertsResult


multiplyMat4:
RET

multiplyMatVec4:
RET

matrix_multiply:
;PUSH    ebp
;MOV     ebp, esp


;MOV     esp, ebp
;POP     ebp
RET

render:
