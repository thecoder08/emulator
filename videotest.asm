define color 0x1000
define startingColor 0x1001

label loop
load color
label addressLow 1 ; label with an offset; allows modification of code during runtime
label addressHigh 2
store 0x4000 ; (addressLow = 0004)(addressHigh = 0005)
add # 1
store color
load addressLow
add # 1
store addressLow
jnz loop

; if addressLow overflowed, increase addressHigh. Also increase color to make diagonal lines
load color
add # 1
store color
load addressHigh
add # 1
store addressHigh
jnz loop

; if addressHigh overflowed, reset it. Also increase startingColor and set color to it to make movement
load # 0x40
store addressHigh
load startingColor
add # 1
store startingColor
store color
jmp loop
