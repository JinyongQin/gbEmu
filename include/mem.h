#pragma once
#include "common.h"
// mam map:
/*
Start	End	    Description	Notes
0000	3FFF	16 KiB ROM bank 00	From cartridge, usually a fixed bank
4000	7FFF	16 KiB ROM Bank 01–NN	From cartridge, switchable bank via mapper (if any)
8000	9FFF	8 KiB Video RAM (VRAM)	In CGB mode, switchable bank 0/1
A000	BFFF	8 KiB External RAM	From cartridge, switchable bank if any
C000	CFFF	4 KiB Work RAM (WRAM)
D000	DFFF	4 KiB Work RAM (WRAM)	In CGB mode, switchable bank 1–7
E000	FDFF	Echo RAM (mirror of C000–DDFF)	Nintendo says use of this area is prohibited.
FE00	FE9F	Object attribute memory (OAM)
FEA0	FEFF	Not Usable	Nintendo says use of this area is prohibited.
FF00	FF7F	I/O Registers
FF80	FFFE	High RAM (HRAM)
FFFF	FFFF	Interrupt Enable register (IE)
*/
#define MEM_SIZE 0x10000 // 64KB
// rom:
#define ROM_BANK_SIZE 0x4000 // 16KB

#define ROM_BANK_OO_START 0x0000 // 16KB ROM bank 00
#define ROM_BANK_OO_END 0x3FFF   // 16KB ROM bank 00

#define ROM_BANK_01_START 0x4000 // 16KB ROM bank 01
#define ROM_BANK_01_END 0x7FFF   // 16KB ROM bank 01
// vram:
#define VRAM_SIZE 0x2000 // 8KB

#define VRAM_START 0x8000 // 8KB Video RAM (VRAM)
#define VRAM_END 0x9FFF   // 8KB Video RAM (VRAM)
// external ram:
#define EX_RAM_SIZE 0x2000 // 8KB

#define EX_RAM_START 0xA000 // 8KB External RAM
#define EX_RAM_END 0xBFFF   // 8KB External RAM
// wram:
#define WRAM_SIZE 0x1000 // 4KB

#define WRAM_START 0xC000 // 4KB Work RAM (WRAM)
#define WRAM_END 0xCFFF   // 4KB Work RAM (WRAM)
// wram bank:
#define WRAM_BANK_SIZE 0x1000 // 4KB Work RAM (WRAM) bank

#define WRAM_BANK_START 0xD000 // 4KB Work RAM (WRAM) bank
#define WRAM_BANK_END 0xDFFF   // 4KB Work RAM (WRAM) bank
// echo ram:
#define ECHO_RAM_SIZE 0x1E00

#define ECHO_RAM_START 0xE000 // Echo RAM (mirror of C000–DDFF)
#define ECHO_RAM_END 0xFDFF   // Echo RAM (mirror of C000–DDFF)
// oam:
#define OAM_SIZE 0x00A0 // 160 bytes

#define OAM_START 0xFE00 // Object attribute memory (OAM)
#define OAM_END 0xFE9F   // Object attribute memory (OAM)
// prohibited:
#define PROHIBITED_SIZE 0x0060 // 96 bytes

#define PROHIBITED_START 0xFEA0 // Not Usable
#define PROHIBITED_END 0xFEFF   // Not Usable
// io:
#define IO_SIZE 0x80 // 128 bytes

#define IO_START 0xFF00 // I/O Registers
#define IO_END 0xFF7F   // I/O Registers
// hram:
#define HRAM_SIZE 0x7F // 127 bytes

#define HRAM_START 0xFF80 // High RAM (HRAM)
#define HRAM_END 0xFFFE   // High RAM (HRAM)
// ie:
#define IE_SIZE 0x01

#define IE_START 0xFFFF // Interrupt Enable register (IE)
#define IE_END 0xFFFF   // Interrupt Enable register (IE)

#define BLOCK_SIZE 128                    // 每个块大小
#define NUM_BLOCKS (0x10000 / BLOCK_SIZE) // 块（0x0000-0xFFFF）

#define WAVE_RAM_SIZE 0x10 // 16 bytes

uint8_t memory_read(uint16_t addr);
void memory_write(uint16_t addr, uint8_t val);