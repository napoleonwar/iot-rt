#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

#include "config.h"

#define END 0xc0
#define ESC 0xdb
#define ESC_END 0xdc
#define ESC_ESC 0xdd

#define MAX 2000

static int fd;

unsigned char buf[MAX];
int is_esc = 0;
int cnt = 0;



int tpip_slip_init(char *str) {

  fd = open(str, O_RDONLY);
  // O_NDELAY returns 0 when no character available, maybe useful for the real polling thing
  printf("SLIP open: %s %d\n", str, fd);

  return 0;
}

void tpip_slip_run() {
  // peridic stuff
  printf("hello\n");
}

int tpip_slip_rxfull() {
  // 
  return 0;
}

int tpip_slip_rxread(unsigned char buf[]) {
  return 123;
}

int tpip_slip_txempty() {
  return 0;
}

void tpip_slip_txwrite(unsigned char buf[], int len) {

}

// below is stuff that needs to be changed and other names.
// Don't look at it now
void tpip_print(unsigned char buf[], int len) {
  
  printf("One packet:\n");
  for (int i=0; i<len; ++i) {
    printf("%02x ", buf[i]);
    if (i%4 == 3) printf("\n");
  }
  printf("\n\n");
}


void tpip_slip(char *dev) {

  unsigned char c;

  LL_INIT(dev);

/*
  for (;;) {
    if (read(fd, &c, 1) == 1) {
      printf("%c", c);
    }
  }
*/
for(;;) {
  while(read(fd, &c, 1) == 1) {
    
    if (is_esc) {
      if (c == ESC_ESC) {
        buf[cnt++] = ESC;
      } else if (c == ESC_END) {
        buf[cnt++] = END;
      }
      is_esc = 0;
    } else if (c == ESC) {
      is_esc = 1;
    } else if (c == END) {
      tpip_print(buf, cnt);
      cnt = 0;
    } else {
      buf[cnt++] = c;
    }
    if (cnt == 2000) cnt = 0;
  }
}
}
