define input 0x3fff
define directionX 0x1000
define directionY 0x1001
define length 0x1002 ; length is double actual snake length; it stores how many bytes the snake array is
define newTmp 0x1003
define index 0x1004
define drawIndex 0x1005

; we reserve 100 bytes for the snake array; this gives us a max length of 50. 0x1007-0x106b

define delayOuter 0x106c

; init length
load # 8
store length

; init snake
load # 0x07
store snakeInit0
load # 0
label snakeInit0 1
store 0x1000 ; placeholder

load # 0x07
add # 2
store snakeInit1
load # 1
label snakeInit1 1
store 0x1000 ; placeholder

load # 0x07
add # 4
store snakeInit2
load # 2
label snakeInit2 1
store 0x1000 ; placeholder

load # 0x07
add # 6
store snakeInit3
load # 3
label snakeInit3 1
store 0x1000 ; placeholder

; init direction
load # 0
store directionX
load # 1
store directionY

; init apple
load # 15
store appleX
load # 86
store appleY

label mainLoop

; input code
load input
and # 0x01
jnz up
load input
and # 0x02
jnz down
load input
and # 0x04
jnz left
load input
and # 0x08
jnz right
jmp inputDone

label up
load # 0
store directionX
load # -1
store directionY
jmp inputDone

label down
load # 0
store directionX
load # 1
store directionY
jmp inputDone

label left
load # -1
store directionX
load # 0
store directionY
jmp inputDone

label right
load # 1
store directionX
load # 0
store directionY

label inputDone

; is snake touching apple
load 0x1007
sub appleX
jnz notTouching
load 0x1008
sub appleY
sub # 0xc0
jnz notTouching

; add new snake unit
load # 0x07
add length
sub # 2
store snakeNew0
label snakeNew0 1
load 0x1000 ; placeholder
sub directionX
store newTmp
load # 0x07
add length
store snakeNew1
load newTmp
label snakeNew1 1
store 0x1000 ; placeholder

load # 0x07
add length
sub # 1
store snakeNew2
label snakeNew2 1
load 0x1000 ; placeholder
sub directionX
store newTmp
load # 0x07
add length
add # 1
store snakeNew3
load newTmp
label snakeNew3 1
store 0x1000 ; placeholder

; increase length
load length
add # 2
store length

; move apple
load appleX
add # 1
store appleX
load appleY
add # 1
store appleY

label notTouching
; move snake

; move snake units
load length
sub # 2
store index

label moveLoop

load # 0x07
add index
sub # 2
store snakeMove0
label snakeMove0 1
load 0x1000 ; placeholder
store newTmp

load # 0x07
add index
store snakeMove1
load newTmp
label snakeMove1 1
store 0x1000 ; placeholder

load # 0x07
add index
sub # 1
store snakeMove3
label snakeMove3 1
load 0x1000 ; placeholder
store newTmp

load # 0x07
add index
add # 1
store snakeMove4
load newTmp
label snakeMove4 1
store 0x1000 ; placeholder

load index
sub # 2
store index

jnz moveLoop

; set new head position
load 0x1007
add directionX
store 0x1007
load 0x1008
add directionY
store 0x1008

; clear background
load # 0x40
store highByte
label clearBackground
load # 0
store lowByte
label clearRow
load # 0xff ; white
label lowByte 1
label highByte 2
store 0 ; placeholder
load lowByte
add # 1
store lowByte
sub # 50
jnz clearRow
load highByte
add # 1
store highByte
sub # 114
jnz clearBackground

; draw apple
load # 0xc0 ; red
label appleX 1
label appleY 2
store 0 ; placeholder

; draw snake
load # 0
store drawIndex
label drawLoop

load # 0x07
add drawIndex
store snakeDraw0
label snakeDraw0 1
load 0x1000 ; placeholder
store drawSnakeLow

load # 0x07
add drawIndex
add # 1
store snakeDraw1
label snakeDraw1 1
load 0x1000 ; placeholder
add # 0x40
store drawSnakeHigh

load # 0x38 ; green
label drawSnakeLow 1
label drawSnakeHigh 2
store 0 ; placeholder

load drawIndex
add # 2
store drawIndex

load length
sub drawIndex
jnz drawLoop

; delay to reduce flickering
load # 0x80
store delayOuter
label outerLoop
load # 0xff
label delayLoop
sub # 1
jnz delayLoop
load delayOuter
sub # 1
store delayOuter
jnz outerLoop

jmp mainLoop