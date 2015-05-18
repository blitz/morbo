/* -*- Mode: C -*- */

#include <util.h>
#include <version.h>
#include <serial.h>
#include <realmode.h>
#include <bda.h>

extern char _binary_standalone_hermes_int_start[];
extern char _binary_standalone_hermes_int_end[];
extern char _binary_standalone_hermes_int_size[];

struct int_header {
  uint16_t entry_point;
  uint16_t continue_offset;
  uint16_t serial_port;
} __attribute__((packed));

extern char _binary_standalone_hermes_rm_start[];
extern char _binary_standalone_hermes_rm_end[];
extern char _binary_standalone_hermes_rm_size[];

int
main(uint32_t magic, struct mbi *mbi)
{
  serial_init();

  printf("\nHermes %s\n", version_str);
  printf("Blame Julian Stecklina <js@alien8.de> for bugs.\n\n");

  struct bios_data_area *bda = get_bios_data_area();
  printf("Low memory: %uKB\n", bda->low_memory);

  uint32_t *ivt = (uint32_t *)(0);

  // Convince the compiler that ivt is not a null pointer.
  asm ("" : "+rm" (ivt));

  // We can only claim full kilobytes.
  size_t int_size = (((size_t)_binary_standalone_hermes_int_size) + 0x3FF) & ~0x3FF;
  printf("We need to claim %zuKB.\n", int_size >> 10);

  bda->low_memory -= int_size >> 10;

  struct int_header *intercept = (struct int_header *)(bda->low_memory * 1024);
  memcpy(intercept, _binary_standalone_hermes_int_start, (size_t)_binary_standalone_hermes_int_size);

  intercept->serial_port = bda->com_port[0];
  uint32_t *continue_vector = (uint32_t *)((char *)intercept + intercept->continue_offset);
  *continue_vector = ivt[0x10];

  printf("intercept %p continue %08x port %04x\n", intercept, *continue_vector, intercept->serial_port);

  ivt[0x10] = (((uintptr_t)intercept >> 4) << 16) | intercept->entry_point;

  memcpy((char *)0x7c00, _binary_standalone_hermes_rm_start, (size_t)_binary_standalone_hermes_rm_size);
  enable_real_mode(0, 0x7c00);

  // Never reached.
  return 0;
}
