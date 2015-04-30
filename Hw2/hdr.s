@Read HD.  Have to clock in an address then change direction.
@PortA is 8 bit data, PortB is sys clock (b0) and direction (b1).
@PortA Base: 0x0AFF
@PortA Dir: 0x0AFE
@PORTB Base: 0x0BFF
@PortB Dir: 0x0BFE
.global HDRead

@make b0 and b1 outputs.
ldr 0x0BFE, =0x03
ldr 0x0AFE, =0xFF

@Address is 16-bit, takes two writes, bitbanged in.
@Thankfully the compiler puts void* results in r25 and r24
ldr 0x0BFF, =0x11 @clock pulse
ldr 0x0BFF, =0x10

ldr 0x0AFF, =0xCC @Special command, indicates a read is comming.

ldr 0x0BFF, =0x11 @clock pulse
ldr 0x0BFF, =0x10

ldr 0x0AFF, r25

ldr 0x0BFF, =0x11 @clock pulse
ldr 0x0BFF, =0x10

ldr 0x0AFF, r24

@Data will show up on the next clock pulse, better change the port dir!
ldr 0x0AFE, =0x00

ldr 0x0BFF, =0x11 @clock pulse
ldr 0x0BFF, =0x10

ldr r24, 0x0AFF
