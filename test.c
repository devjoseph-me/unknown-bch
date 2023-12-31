#include <stdio.h>
#include <string.h>
#include "bch.h"

unsigned char target_zero[] = {
    0xee, 0xab, 0xf9, 0x62, 0x63, 0xa3, 0xcb, 0x9e, 0x09, 0xd5, 0x38, 0xf4,
    0x4b, 0x4a, 0xda, 0x0e, 0x64, 0x66, 0xb2, 0x66, 0x6a, 0xba, 0x9f, 0xec,
    0x1a, 0xae, 0x3a, 0xaa, 0xe7, 0x2a
};

unsigned char rb_zero[] = {
    0x77, 0x5d, 0xf9, 0x64, 0x6c, 0x5c, 0x3d, 0x97, 0x09, 0xba, 0xc1, 0xf2,
    0x2d, 0x25, 0xb5, 0x07, 0x62, 0x66, 0xd4, 0x66, 0x65, 0xd5, 0x9f, 0x73,
    0x85, 0x57, 0xc5, 0x55, 0x7e, 0x45
};

unsigned char target_ff[] = {
    0x7b, 0x32, 0x01, 0xd7, 0x63, 0x6f, 0x38, 0xf1, 0xc7, 0x84, 0xa1, 0xab,
    0x8d, 0xbe, 0x3f, 0x71, 0x84, 0x82, 0x71, 0x99, 0x08, 0x9f, 0xac, 0x7b,
    0x52, 0xab, 0xef, 0xa3, 0x1e, 0xf3
};

unsigned char rb_ff[] = {
    0xed, 0xc4, 0x08, 0xbe, 0x6c, 0x6f, 0xc1, 0xf8, 0x3e, 0x12, 0x58, 0x5d,
    0x1b, 0xd7, 0xcf, 0xe8, 0x12, 0x14, 0xe8, 0x99, 0x01, 0x9f, 0x53, 0xed,
    0xa4, 0x5d, 0x7f, 0x5c, 0x87, 0xfc
};

void compute_bch(const unsigned char *buff, int size, int poly, bool swa, unsigned char *ecc) {
  struct bch_control *c = bch_init(15, 16, poly, swa);
  if (c) {
    memset(ecc, 0, 30);
    bch_encode(c, buff, size, ecc);
  }
}

void xor_bytes(unsigned char *out, const unsigned char *a, const unsigned char * b, int size) {
  int i;
  for (i=0;i<size;i++) {
    out[i] = a[i]^b[i];
  }
}

void dumpbin(unsigned char* ecc, int size) {
  for (int i = 0; i < size; i++) {
    printf("%02x ", ecc[i]);
  }
  printf("\n");
}

int main() {
  unsigned char buff[2048 + 32];
  unsigned char out[30];
  unsigned char ecc1[30];
  memset(buff, 0xff, sizeof(buff));

  // ================================================================================
  // FILL 00
  memset(buff, 0x00, 2048);
  compute_bch(buff, 2048 + 32, 0x8003, true, ecc1);

  printf("FILLED 00) DUMP:");
  dumpbin(ecc1, 30);

  // should rb_zero == ecc1
  if (memcmp(rb_zero, ecc1, 30) != 0) {
    printf("SOMETHING WRONG\n");
  }

  xor_bytes(out, ecc1, target_zero, 30);
  printf("DIFF FROM TARGET) DUMP:");
  dumpbin(out, 30);

  // ================================================================================
  // FILL FF
  memset(buff, 0xff, 2048);
  compute_bch(buff, 2048 + 32, 0x8003, true, ecc1);

  printf("FILLED FF) DUMP:");
  dumpbin(ecc1, 30);

  // should rb_ff == ecc1
  if (memcmp(rb_ff, ecc1, 30) != 0) {
    printf("SOMETHING WRONG\n");
  }

  xor_bytes(out, ecc1, target_ff, 30);
  printf("DIFF FROM TARGET) DUMP:");
  dumpbin(out, 30);

  return 0;
}