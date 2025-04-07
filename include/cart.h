#pragma once

#include "common.h"

// cartridge (gameboy rom)
typedef struct CartHeader
{
    uint8_t entry_point[4];          // 0x100-0x103
    uint8_t nintendo_logo[48];       // 0x104-0x133
    char title[16];                  // 0x134-0x143
    uint16_t new_licensee_code;      // 0x144-0x145
    uint8_t sgb_flag;                // 0x146
    uint8_t cartridge_type;          // 0x147
    uint8_t rom_size;                // 0x148
    uint8_t ram_size;                // 0x149
    uint8_t destination_code;        // 0x14A
    uint8_t old_licensee_code;       // 0x14B
    uint8_t mask_rom_version_number; // 0x14C
    uint8_t checksum;                // 0x14D
    uint16_t global_checksum;        // 0x14E-0x14F
} CartHeader;

typedef struct Cart
{
    FILE *rom_fp;

    CartHeader *header; // rom header: if loaded, header points to rom_data + 0x100

    uint8_t *rom_data; // rom data
    uint32_t rom_size; // rom size
    bool loaded;       // rom loaded
    bool check_passed;

} Cart;

Cart *cart_create();
int cart_load_rom(Cart *cart);
int cart_destroy(Cart *cart);

int print_rom_info(Cart *cart);

CartHeader cart_get_header(Cart *cart);

uint8_t cart_read(uint16_t addr);
void cart_write(uint16_t addr, uint8_t val);
uint8_t exram_read(uint16_t addr);
void exram_write(uint16_t addr, uint8_t val);