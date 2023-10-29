define xDir 0x1000
define yDir 0x1001

; ball travels down and to the right
load # 1
store xDir
store yDir

label loop
; draw ball; color corresponds to X position
load ballX
label ballX 1
label ballY 2
store 0x4000

; move ball
load ballX
add xDir
store ballX
load ballY
add yDir
store ballY

; check if we hit the bottom
load ballY
sub # 0xff
jnz nobottom
load # -1
store yDir
label nobottom

; check if we hit the top
load ballY
sub # 0x40
jnz notop
load # 1
store yDir
label notop

; check if we hit the left
load ballX
jnz noleft
load # 1
store xDir
label noleft

; check if we hit the right
load ballX
sub # 0xff
jnz noright
load # -1
store xDir
label noright

nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop
nop

jmp loop