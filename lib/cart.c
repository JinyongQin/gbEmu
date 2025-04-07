#include "cart.h"
#include "emu.h"
/*
$00	0	No RAM
$01	–	Unused 12
$02	8 KiB	1 bank
$03	32 KiB	4 banks of 8 KiB each
$04	128 KiB	16 banks of 8 KiB each
$05	64 KiB	8 banks of 8 KiB each
*/
extern Emulator *emu;
static const int CART_RAM_SIZE[] = {
    [0x00] = 0,
    [0x02] = 8,
    [0x03] = 32,
    [0x04] = 128,
    [0x05] = 64,
};

static const char *
    ROM_TYPES[] = {
        "ROM ONLY",
        "MBC1",
        "MBC1+RAM",
        "MBC1+RAM+BATTERY",
        "0x04 ???",
        "MBC2",
        "MBC2+BATTERY",
        "0x07 ???",
        "ROM+RAM 1",
        "ROM+RAM+BATTERY 1",
        "0x0A ???",
        "MMM01",
        "MMM01+RAM",
        "MMM01+RAM+BATTERY",
        "0x0E ???",
        "MBC3+TIMER+BATTERY",
        "MBC3+TIMER+RAM+BATTERY 2",
        "MBC3",
        "MBC3+RAM 2",
        "MBC3+RAM+BATTERY 2",
        "0x14 ???",
        "0x15 ???",
        "0x16 ???",
        "0x17 ???",
        "0x18 ???",
        "MBC5",
        "MBC5+RAM",
        "MBC5+RAM+BATTERY",
        "MBC5+RUMBLE",
        "MBC5+RUMBLE+RAM",
        "MBC5+RUMBLE+RAM+BATTERY",
        "0x1F ???",
        "MBC6",
        "0x21 ???",
        "MBC7+SENSOR+RUMBLE+RAM+BATTERY",
};

static const char *LIC_CODE[0xA5] = {
    [0x00] = "None",
    [0x01] = "Nintendo R&D1",
    [0x08] = "Capcom",
    [0x13] = "Electronic Arts",
    [0x18] = "Hudson Soft",
    [0x19] = "b-ai",
    [0x20] = "kss",
    [0x22] = "pow",
    [0x24] = "PCM Complete",
    [0x25] = "san-x",
    [0x28] = "Kemco Japan",
    [0x29] = "seta",
    [0x30] = "Viacom",
    [0x31] = "Nintendo",
    [0x32] = "Bandai",
    [0x33] = "Ocean/Acclaim",
    [0x34] = "Konami",
    [0x35] = "Hector",
    [0x37] = "Taito",
    [0x38] = "Hudson",
    [0x39] = "Banpresto",
    [0x41] = "Ubi Soft",
    [0x42] = "Atlus",
    [0x44] = "Malibu",
    [0x46] = "angel",
    [0x47] = "Bullet-Proof",
    [0x49] = "irem",
    [0x50] = "Absolute",
    [0x51] = "Acclaim",
    [0x52] = "Activision",
    [0x53] = "American sammy",
    [0x54] = "Konami",
    [0x55] = "Hi tech entertainment",
    [0x56] = "LJN",
    [0x57] = "Matchbox",
    [0x58] = "Mattel",
    [0x59] = "Milton Bradley",
    [0x60] = "Titus",
    [0x61] = "Virgin",
    [0x64] = "LucasArts",
    [0x67] = "Ocean",
    [0x69] = "Electronic Arts",
    [0x70] = "Infogrames",
    [0x71] = "Interplay",
    [0x72] = "Broderbund",
    [0x73] = "sculptured",
    [0x75] = "sci",
    [0x78] = "THQ",
    [0x79] = "Accolade",
    [0x80] = "misawa",
    [0x83] = "lozc",
    [0x86] = "Tokuma Shoten Intermedia",
    [0x87] = "Tsukuda Original",
    [0x91] = "Chunsoft",
    [0x92] = "Video system",
    [0x93] = "Ocean/Acclaim",
    [0x95] = "Varie",
    [0x96] = "Yonezawa/s’pal",
    [0x97] = "Kaneko",
    [0x99] = "Pack in soft",
    [0xA4] = "Konami (Yu-Gi-Oh!)"};

// static bool cart_is_mbc1(Cart *cart)
// {
//     BETWEEN(cart->header->cartridge_type, 1, 3);
// }

int print_rom_info(Cart *cart)
{
    CartHeader header = cart_get_header(cart);
    printf("\tTitle     : %s\n", header.title);
    printf("\tROM Type  : %s\n", ROM_TYPES[header.cartridge_type]);
    printf("\tROM Size  : %dKB\n", 32 << header.rom_size);
    printf("\tRAM Size  : %dKB\n", CART_RAM_SIZE[header.ram_size]);
    printf("\tLicensee  : %s\n", LIC_CODE[header.old_licensee_code]);
    printf("\tChecksum  : 0x%X (%s)\n", header.checksum,
           (cart->check_passed ? "PASSED" : "FAIL"));

    return 0;
}

int cart_load_rom(Cart *cart)
{
    if (cart->rom_fp == NULL)
    {
        printf("Error: rom file not opened\n");
        return -1;
    }
    // 1. get rom file size
    fseek(cart->rom_fp, 0, SEEK_END);     // 设置文件指针到文件末尾，偏移量为0（字节）
    cart->rom_size = ftell(cart->rom_fp); // 获取文件指针的当前位置，即文件大小（字节数）
    // 2. read rom file to memory
    fseek(cart->rom_fp, 0, SEEK_SET); // rewind(rom_fd);
    // alloc rom data
    cart->rom_data = (uint8_t *)malloc(cart->rom_size);
    if (cart->rom_data == NULL)
    {
        printf("Error: could not allocate memory for rom data\n");
        return -1;
    }
    fread(cart->rom_data, 1, cart->rom_size, cart->rom_fp);
    // 3. load header ,header -> 0x100-0x14F
    cart->header = (CartHeader *)(cart->rom_data + 0x100);
    // 4. check header
    uint8_t checksum = 0;
    for (int i = 0x134; i <= 0x14C; i++)
    {
        checksum = checksum - cart->rom_data[i] - 1;
    }
    if (checksum != cart->header->checksum)
    {
        printf("Error: header checksum failed\n");
        return -1;
    }
    cart->check_passed = true;
    cart->loaded = true;

    print_rom_info(cart);

    return 0;
}

inline static int remove_rom(Cart *cart)
{
    // remove header
    if (cart->header != NULL)
    {
        cart->header = NULL;
    }
    // remove rom file
    if (cart->rom_fp != NULL)
    {
        fclose(cart->rom_fp);
        cart->rom_fp = NULL;
    }
    cart->rom_size = 0;
    cart->loaded = false;
    return 0;
}

Cart *cart_create()
{
    Cart *cart = (Cart *)malloc(sizeof(Cart));
    if (cart == NULL)
    {
        printf("Error: could not allocate memory for cart\n");
        return NULL;
    }
    cart->rom_fp = NULL;
    cart->header = NULL;
    cart->rom_data = NULL;
    cart->rom_size = 0;
    return cart;
}

int cart_destroy(Cart *cart)
{
    if (cart == NULL)
    {
        return -1;
    }
    // remove rom
    remove_rom(cart);
    return 0;
}

CartHeader cart_get_header(Cart *cart)
{
    return *(cart->header);
}

uint8_t cart_read(uint16_t addr)
{
    return emu->cart.rom_data[addr];
}

void cart_write(uint16_t addr, uint8_t val)
{
    printf("%s: cart type unsupport write\n", __func__);
}

uint8_t exram_read(uint16_t addr)
{
    NO_IMPL;
}

void exram_write(uint16_t addr, uint8_t val)
{
    NO_IMPL;
}