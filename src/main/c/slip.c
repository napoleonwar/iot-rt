#include <stdio.h>
#include <fcntl.h>

#define END 0xc0
#define ESC 0xdb
#define ESC_END 0xdc
#define ESC_ESC 0xdd

void tpip_print(unsigned char buf[], int len) {
  
  printf("One packet:\n");
  for (int i=0; i<len; ++i) {
    printf("%02x ", buf[i]);
    if (i%4 == 3) printf("\n");
  }
  printf("\n\n");
}

void tpip_slip(char *dev) {

  int fd = open(dev, O_RDONLY);
  // O_NDELAY returns 0 when no character available, maybe useful for the real polling thing
  printf("%s %d\n", dev, fd);

  unsigned char buf[2000];
  unsigned char c;
  int is_esc = 0;
  int cnt = 0;
  while(read(fd, &c, 1) == 1) {
    
    // printf("%02x %d %d \n", c, is_esc, cnt);
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


int main(int argc, char *argv[]) {

  if (argc != 2) return -1;
  tpip_slip(argv[1]);
  return 0;
}
