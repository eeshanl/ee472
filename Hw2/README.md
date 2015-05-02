# Build a BIOS

Your company maintains a small, proprietary PC-like box that includes a hard drive and a video card.  Its BIOS is long overdue for a refresh and many of the original engineers have left, retired, etc.

Your project lead has taken a look at the specs and leftover docs and given you an outline to help make fixing this mess easier:

## Part 1: Hard Drive IO
Convert an old hard drive driver to C.  In the zip file you will find hdr.s, hdw.s, and hdrw.h.  These files represent the old way your company interfaced the hard drive of your product.  C would be more maintainable and readable (and these days probably create faster, better code most times), so you've been asked to write a new hdrw.c file, replacing both hdr.s and hdw.s.
You don't need the write operation for the bios, but while we're fixing the driver we might as well get both things done.
(Heads up on the .s files! Our assembly looks a lot like ARM but it uses a generic "ldr" op where the first argument is always the destination and the second is always the source.  This operation can work on registers or data.)

Notes:
You will be writing hdrw.c.  Search-and-replace might come in handy for replacing ugly addresses with nice identifiers for #define statements.

You may want to replace the 16-bit void* with two 8-bit chars for clarity.

## Part 2: Video Output Driver
The video card has a text-mode fallback, which is the only thing we should bother supporting during boot.  The OS can worry about 1080p.

The video card is attached to Port C.  At startup, Port C should be held at 0x00 until you're ready to send text.  Once ready, change Port C to 0x02 for about a hundred instructions to enter text-mode fallback.  You can then enter any ASCII code in sequence and it will show up on the screen.  For example writing hello to the screen could be done as:
unsigned char* PC = 0xCFF;
*PC = 'h';
*PC = 'e';
*PC = 'l';
*PC = 'l';
*PC = 'o';

For this project, you'll want a function that can take a string as an argument and put it to the screen.  Easy enough, we just haven't gotten around to it yet.

## Part 3: BIOS Code
The old BIOS now only exists as hex codes, as the original assembly BIOS code was lost.  On the bright side this should provide you with a nice clean slate to work from!

The BIOS should use the HD driver to scan the first 16 characters of the hard drive (addresses 0x0000-0x000F) and, if all 16 are ASCII (numbers, characters and spaces only), print that to the display.

If non-ASCII characters are present, it should print an error message and halt (loop forever).  If the boot string is valid, the BIOS should copy the first 2kB (0x0010-0x0810) to main memory starting at address 0x0F00.  The BIOS should then create a void function pointer and set it to 0x0F00 and execute it.  (The OS image is actually only 1.2k right now but ideally it should be a few OS iterations before we need more space.)

After that line in the C code, theoretically the BIOS should never execute another line, but please put an error message and while(1) loop after that in case the OS should decide to exit for some reason.
