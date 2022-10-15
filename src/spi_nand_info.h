/*
 * Copyright (c) 2022, YuzukiTsuru <GloomyGhost@GloomyGhost.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * See README and LICENSE for more details.
 */

//
// Created by gloom on 2022/9/27.
//

#ifndef YFEL_SPI_NAND_INFO_H
#define YFEL_SPI_NAND_INFO_H

#include <QObject>

typedef struct spinand_info {
    QString name;
    struct {
        uint8_t val[4];
        uint8_t len;
    } id;
    uint32_t page_size = 0;
    uint32_t spare_size = 0;
    uint32_t pages_per_block = 0;
    uint32_t blocks_per_die = 0;
    uint32_t planes_per_die = 0;
    uint32_t ndies = 0;
    uint64_t nand_size = 0;
} spinand_info_t;

typedef struct spinand_pdata {
    spinand_info_t info;
    uint32_t swap_buf = 0;
    uint32_t swap_len = 0;
    uint32_t cmd_len = 0;
} spinand_pdata_t;

enum SPI_NAND_OPS {
    OPCODE_RDID = 0x9f,
    OPCODE_GET_FEATURE = 0x0f,
    OPCODE_SET_FEATURE = 0x1f,
    OPCODE_FEATURE_PROTECT = 0xa0,
    OPCODE_FEATURE_CONFIG = 0xb0,
    OPCODE_FEATURE_STATUS = 0xc0,
    OPCODE_READ_PAGE_TO_CACHE = 0x13,
    OPCODE_READ_PAGE_FROM_CACHE = 0x03,
    OPCODE_WRITE_ENABLE = 0x06,
    OPCODE_BLOCK_ERASE = 0xd8,
    OPCODE_PROGRAM_LOAD = 0x02,
    OPCODE_PROGRAM_EXEC = 0x10,
    OPCODE_RESET = 0xff,
};

static const spinand_info_t spinand_info[] = {
        /* Winbond */
        {"W25N512GV",       {.val = {0xef, 0xaa, 0x20}, .len = 3}, 2048, 64,  64,  512,  1, 1},
        {"W25N01GV",        {.val = {0xef, 0xaa, 0x21}, .len = 3}, 2048, 64,  64,  1024, 1, 1},
        {"W25M02GV",        {.val = {0xef, 0xab, 0x21}, .len = 3}, 2048, 64,  64,  1024, 1, 2},
        {"W25N02KV",        {.val = {0xef, 0xaa, 0x22}, .len = 3}, 2048, 128, 64,  2048, 1, 1},

        /* Gigadevice */
        {"GD5F1GQ4UAWxx",   {.val = {0xc8, 0x10}, .len = 2},       2048, 64,  64,  1024, 1, 1},
        {"GD5F1GQ5UExxG",   {.val = {0xc8, 0x51}, .len = 2},       2048, 128, 64,  1024, 1, 1},
        {"GD5F1GQ4UExIG",   {.val = {0xc8, 0xd1}, .len = 2},       2048, 128, 64,  1024, 1, 1},
        {"GD5F1GQ4UExxH",   {.val = {0xc8, 0xd9}, .len = 2},       2048, 64,  64,  1024, 1, 1},
        {"GD5F1GQ4xAYIG",   {.val = {0xc8, 0xf1}, .len = 2},       2048, 64,  64,  1024, 1, 1},
        {"GD5F2GQ4UExIG",   {.val = {0xc8, 0xd2}, .len = 2},       2048, 128, 64,  2048, 1, 1},
        {"GD5F2GQ5UExxH",   {.val = {0xc8, 0x32}, .len = 2},       2048, 64,  64,  2048, 1, 1},
        {"GD5F2GQ4xAYIG",   {.val = {0xc8, 0xf2}, .len = 2},       2048, 64,  64,  2048, 1, 1},
        {"GD5F4GQ4UBxIG",   {.val = {0xc8, 0xd4}, .len = 2},       4096, 256, 64,  2048, 1, 1},
        {"GD5F4GQ4xAYIG",   {.val = {0xc8, 0xf4}, .len = 2},       2048, 64,  64,  4096, 1, 1},
        {"GD5F2GQ5UExxG",   {.val = {0xc8, 0x52}, .len = 2},       2048, 128, 64,  2048, 1, 1},
        {"GD5F4GQ4UCxIG",   {.val = {0xc8, 0xb4}, .len = 2},       4096, 256, 64,  2048, 1, 1},
        {"GD5F4GQ4RCxIG",   {.val = {0xc8, 0xa4}, .len = 2},       4096, 256, 64,  2048, 1, 1},

        /* Macronix */
        {"MX35LF1GE4AB",    {.val = {0xc2, 0x12}, .len = 2},       2048, 64,  64,  1024, 1, 1},
        {"MX35LF1G24AD",    {.val = {0xc2, 0x14}, .len = 2},       2048, 128, 64,  1024, 1, 1},
        {"MX31LF1GE4BC",    {.val = {0xc2, 0x1e}, .len = 2},       2048, 64,  64,  1024, 1, 1},
        {"MX35LF2GE4AB",    {.val = {0xc2, 0x22}, .len = 2},       2048, 64,  64,  2048, 1, 1},
        {"MX35LF2G24AD",    {.val = {0xc2, 0x24}, .len = 2},       2048, 128, 64,  2048, 1, 1},
        {"MX35LF2GE4AD",    {.val = {0xc2, 0x26}, .len = 2},       2048, 128, 64,  2048, 1, 1},
        {"MX35LF2G14AC",    {.val = {0xc2, 0x20}, .len = 2},       2048, 64,  64,  2048, 1, 1},
        {"MX35LF4G24AD",    {.val = {0xc2, 0x35}, .len = 2},       4096, 256, 64,  2048, 1, 1},
        {"MX35LF4GE4AD",    {.val = {0xc2, 0x37}, .len = 2},       4096, 256, 64,  2048, 1, 1},

        /* Micron */
        {"MT29F1G01AAADD",  {.val = {0x2c, 0x12}, .len = 2},       2048, 64,  64,  1024, 1, 1},
        {"MT29F1G01ABAFD",  {.val = {0x2c, 0x14}, .len = 2},       2048, 128, 64,  1024, 1, 1},
        {"MT29F2G01AAAED",  {.val = {0x2c, 0x9f}, .len = 2},       2048, 64,  64,  2048, 2, 1},
        {"MT29F2G01ABAGD",  {.val = {0x2c, 0x24}, .len = 2},       2048, 128, 64,  2048, 2, 1},
        {"MT29F4G01AAADD",  {.val = {0x2c, 0x32}, .len = 2},       2048, 64,  64,  4096, 2, 1},
        {"MT29F4G01ABAFD",  {.val = {0x2c, 0x34}, .len = 2},       4096, 256, 64,  2048, 1, 1},
        {"MT29F4G01ADAGD",  {.val = {0x2c, 0x36}, .len = 2},       2048, 128, 64,  2048, 2, 2},
        {"MT29F8G01ADAFD",  {.val = {0x2c, 0x46}, .len = 2},       4096, 256, 64,  2048, 1, 2},

        /* Toshiba */
        {"TC58CVG0S3HRAIG", {.val = {0x98, 0xc2}, .len = 2},       2048, 128, 64,  1024, 1, 1},
        {"TC58CVG1S3HRAIG", {.val = {0x98, 0xcb}, .len = 2},       2048, 128, 64,  2048, 1, 1},
        {"TC58CVG2S0HRAIG", {.val = {0x98, 0xcd}, .len = 2},       4096, 256, 64,  2048, 1, 1},
        {"TC58CVG0S3HRAIJ", {.val = {0x98, 0xe2}, .len = 2},       2048, 128, 64,  1024, 1, 1},
        {"TC58CVG1S3HRAIJ", {.val = {0x98, 0xeb}, .len = 2},       2048, 128, 64,  2048, 1, 1},
        {"TC58CVG2S0HRAIJ", {.val = {0x98, 0xed}, .len = 2},       4096, 256, 64,  2048, 1, 1},
        {"TH58CVG3S0HRAIJ", {.val = {0x98, 0xe4}, .len = 2},       4096, 256, 64,  4096, 1, 1},

        /* Esmt */
        {"F50L512M41A",     {.val = {0xc8, 0x20}, .len = 2},       2048, 64,  64,  512,  1, 1},
        {"F50L1G41A",       {.val = {0xc8, 0x21}, .len = 2},       2048, 64,  64,  1024, 1, 1},
        {"F50L1G41LB",      {.val = {0xc8, 0x01}, .len = 2},       2048, 64,  64,  1024, 1, 1},
        {"F50L2G41LB",      {.val = {0xc8, 0x0a}, .len = 2},       2048, 64,  64,  1024, 1, 2},

        /* Fison */
        {"CS11G0T0A0AA",    {.val = {0x6b, 0x00}, .len = 2},       2048, 128, 64,  1024, 1, 1},
        {"CS11G0G0A0AA",    {.val = {0x6b, 0x10}, .len = 2},       2048, 128, 64,  1024, 1, 1},
        {"CS11G0S0A0AA",    {.val = {0x6b, 0x20}, .len = 2},       2048, 64,  64,  1024, 1, 1},
        {"CS11G1T0A0AA",    {.val = {0x6b, 0x01}, .len = 2},       2048, 128, 64,  2048, 1, 1},
        {"CS11G1S0A0AA",    {.val = {0x6b, 0x21}, .len = 2},       2048, 64,  64,  2048, 1, 1},
        {"CS11G2T0A0AA",    {.val = {0x6b, 0x02}, .len = 2},       2048, 128, 64,  4096, 1, 1},
        {"CS11G2S0A0AA",    {.val = {0x6b, 0x22}, .len = 2},       2048, 64,  64,  4096, 1, 1},

        /* Etron */
        {"EM73B044VCA",     {.val = {0xd5, 0x01}, .len = 2},       2048, 64,  64,  512,  1, 1},
        {"EM73C044SNB",     {.val = {0xd5, 0x11}, .len = 2},       2048, 120, 64,  1024, 1, 1},
        {"EM73C044SNF",     {.val = {0xd5, 0x09}, .len = 2},       2048, 128, 64,  1024, 1, 1},
        {"EM73C044VCA",     {.val = {0xd5, 0x18}, .len = 2},       2048, 64,  64,  1024, 1, 1},
        {"EM73C044SNA",     {.val = {0xd5, 0x19}, .len = 2},       2048, 64,  128, 512,  1, 1},
        {"EM73C044VCD",     {.val = {0xd5, 0x1c}, .len = 2},       2048, 64,  64,  1024, 1, 1},
        {"EM73C044SND",     {.val = {0xd5, 0x1d}, .len = 2},       2048, 64,  64,  1024, 1, 1},
        {"EM73D044SND",     {.val = {0xd5, 0x1e}, .len = 2},       2048, 64,  64,  2048, 1, 1},
        {"EM73C044VCC",     {.val = {0xd5, 0x22}, .len = 2},       2048, 64,  64,  1024, 1, 1},
        {"EM73C044VCF",     {.val = {0xd5, 0x25}, .len = 2},       2048, 64,  64,  1024, 1, 1},
        {"EM73C044SNC",     {.val = {0xd5, 0x31}, .len = 2},       2048, 128, 64,  1024, 1, 1},
        {"EM73D044SNC",     {.val = {0xd5, 0x0a}, .len = 2},       2048, 120, 64,  2048, 1, 1},
        {"EM73D044SNA",     {.val = {0xd5, 0x12}, .len = 2},       2048, 128, 64,  2048, 1, 1},
        {"EM73D044SNF",     {.val = {0xd5, 0x10}, .len = 2},       2048, 128, 64,  2048, 1, 1},
        {"EM73D044VCA",     {.val = {0xd5, 0x13}, .len = 2},       2048, 128, 64,  2048, 1, 1},
        {"EM73D044VCB",     {.val = {0xd5, 0x14}, .len = 2},       2048, 64,  64,  2048, 1, 1},
        {"EM73D044VCD",     {.val = {0xd5, 0x17}, .len = 2},       2048, 128, 64,  2048, 1, 1},
        {"EM73D044VCH",     {.val = {0xd5, 0x1b}, .len = 2},       2048, 64,  64,  2048, 1, 1},
        {"EM73D044SND",     {.val = {0xd5, 0x1d}, .len = 2},       2048, 64,  64,  2048, 1, 1},
        {"EM73D044VCG",     {.val = {0xd5, 0x1f}, .len = 2},       2048, 64,  64,  2048, 1, 1},
        {"EM73D044VCE",     {.val = {0xd5, 0x20}, .len = 2},       2048, 64,  64,  2048, 1, 1},
        {"EM73D044VCL",     {.val = {0xd5, 0x2e}, .len = 2},       2048, 128, 64,  2048, 1, 1},
        {"EM73D044SNB",     {.val = {0xd5, 0x32}, .len = 2},       2048, 128, 64,  2048, 1, 1},
        {"EM73E044SNA",     {.val = {0xd5, 0x03}, .len = 2},       4096, 256, 64,  2048, 1, 1},
        {"EM73E044SND",     {.val = {0xd5, 0x0b}, .len = 2},       4096, 240, 64,  2048, 1, 1},
        {"EM73E044SNB",     {.val = {0xd5, 0x23}, .len = 2},       4096, 256, 64,  2048, 1, 1},
        {"EM73E044VCA",     {.val = {0xd5, 0x2c}, .len = 2},       4096, 256, 64,  2048, 1, 1},
        {"EM73E044VCB",     {.val = {0xd5, 0x2f}, .len = 2},       2048, 128, 64,  4096, 1, 1},
        {"EM73F044SNA",     {.val = {0xd5, 0x24}, .len = 2},       4096, 256, 64,  4096, 1, 1},
        {"EM73F044VCA",     {.val = {0xd5, 0x2d}, .len = 2},       4096, 256, 64,  4096, 1, 1},
        {"EM73E044SNE",     {.val = {0xd5, 0x0e}, .len = 2},       4096, 256, 64,  4096, 1, 1},
        {"EM73C044SNG",     {.val = {0xd5, 0x0c}, .len = 2},       2048, 120, 64,  1024, 1, 1},
        {"EM73D044VCN",     {.val = {0xd5, 0x0f}, .len = 2},       2048, 64,  64,  2048, 1, 1},

        /* Elnec */
        {"FM35Q1GA",        {.val = {0xe5, 0x71}, .len = 2},       2048, 64,  64,  1024, 1, 1},

        /* Paragon */
        {"PN26G01A",        {.val = {0xa1, 0xe1}, .len = 2},       2048, 128, 64,  1024, 1, 1},
        {"PN26G02A",        {.val = {0xa1, 0xe2}, .len = 2},       2048, 128, 64,  2048, 1, 1},

        /* Ato */
        {"ATO25D1GA",       {.val = {0x9b, 0x12}, .len = 2},       2048, 64,  64,  1024, 1, 1},

        /* Heyang */
        {"HYF1GQ4U",        {.val = {0xc9, 0x51}, .len = 2},       2048, 128, 64,  1024, 1, 1},
        {"HYF2GQ4U",        {.val = {0xc9, 0x52}, .len = 2},       2048, 128, 64,  2048, 1, 1},
};

#endif //YFEL_SPI_NAND_INFO_H
