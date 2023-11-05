define pixels 0xf6
define pixelsStart 0xf7

label loop

; store pixels in vidmem
load pixels
label vidmem 1
store 0xf8

; multiply pixels by 2, shifting to the left
load pixels
add pixels
store pixels

; wait
load # 0xff
label wait
sub # 1
jnz wait
load # 0xff
label wait2
sub # 1
jnz wait2

; go to next line
load vidmem
add # 1
store vidmem

; draw next line if not at bottom
jnz loop

; increase pixelsStart, set pixels to it
load pixelsStart
add # 1
store pixelsStart
store pixels

; reset line to top
load # 0xf8
store vidmem

jmp loop