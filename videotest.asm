color = 1000

label loop ; loop = 0000
load color ; 02 00 10
store 4000(addressLow,addressHigh) ; 07 00 40 (addressLow = 0004)(addressHigh = 0005)
add #1 ; 03 01
store color ; 07 00 10
load addressLow ; 02 04 00
add #1 ; 03 01
store addressLow ; 07 04 00
jnz loop ; 09 00 00
load addressHigh ; 02 05 00
add #1 ; 03 01
store addressHigh ; 07 05 00
load color ; 02 00 10
add #1 ; 03 01
store color ; 07 00 10
load addressHigh ; 02 05 00
jnz loop ; 09 00 00
load color ; 02 00 10
add #1 ; 03 01
store color ; 07 00 10
jmp loop ; 08 00 00