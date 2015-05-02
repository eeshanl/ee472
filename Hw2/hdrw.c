#include "hdrw.h"

#define PORTABASE = 0x0AFF;
#define PORTADIR = 0x0AFE;
#define PORTBBASE = 0x0BFF;
#define PORTBDIR = 0x0BFE;

void clockPulse();

void HDWrite(void* ptr, unsigned char ch) {
  void* pad = PORTADIR;
  void* pbd = PORTBDIR;
  *pad = 0xFF;
  *pad = 0x03;
  clockPulse();


}

void clockPulse() {
  // ldr 0x0BFF, =0x11 @clock pulse
  // ldr 0x0BFF, =0x10
  void* pbb = PORTBBASE;
  *pbb = 0x11;
  *pbb = 0x10;

}

unsigned char HDRead(void* ptr) {

}
