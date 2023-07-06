// Copyright 2023 Christopher Briscoe.  All rights reserved.

#ifndef FONT_H
#define FONT_H

#include "env.h"

// since max width of the font is 5, we could reduce the size of memory by
// doing some bit shifting magic on the encoding and decoding
// bytes 1-7 are the font, byte 8 is the character width
// 64 characters, 8 bytes each
const byte font[65][8] PROGMEM = {
    0b00000000,  // (space) ascii 32
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    1,
    0b10000000,  // ! ascii 33
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b00000000,
    0b10000000,
    1,
    0b10100000,  // " ascii 34
    0b10100000,
    0b10100000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    3,
    0b01010000,  // # ascii 35
    0b01010000,
    0b11111000,
    0b01010000,
    0b11111000,
    0b01010000,
    0b01010000,
    5,
    0b00100000,  // $ ascii 36
    0b01111000,
    0b10000000,
    0b01110000,
    0b00001000,
    0b11110000,
    0b00100000,
    5,
    0b00000000,  // % ascii 37
    0b11001000,
    0b11010000,
    0b00100000,
    0b01011000,
    0b10011000,
    0b00000000,
    5,
    0b01000000,  // & ascii 38
    0b10100000,
    0b10100000,
    0b01000000,
    0b10101000,
    0b10010000,
    0b01101000,
    5,
    0b10000000,  // ' ascii 39
    0b10000000,
    0b10000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    1,
    0b01000000,  // ( ascii 40
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b01000000,
    2,
    0b10000000,  // ) ascii 41
    0b11000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b10000000,
    2,
    0b00000000,  // * ascii 42
    0b00000000,
    0b10100000,
    0b01000000,
    0b10100000,
    0b00000000,
    0b00000000,
    5,
    0b00000000,  // + ascii 43
    0b00000000,
    0b01000000,
    0b11100000,
    0b01000000,
    0b00000000,
    0b00000000,
    3,
    0b00000000,  // , ascii 44
    0b00000000,
    0b00000000,
    0b00000000,
    0b11000000,
    0b01000000,
    0b10000000,
    2,
    0b00000000,  // - ascii 45
    0b00000000,
    0b00000000,
    0b11100000,
    0b00000000,
    0b00000000,
    0b00000000,
    3,
    0b00000000,  // . ascii 46
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b11000000,
    0b11000000,
    2,
    0b00000000,  // / ascii 47
    0b00000000,
    0b00100000,
    0b01000000,
    0b10000000,
    0b00000000,
    0b00000000,
    3,
    0b01110000,  // 0 ascii 48
    0b10001000,
    0b11001000,
    0b10101000,
    0b10011000,
    0b10001000,
    0b01110000,
    5,
    0b00100000,  // 1 ascii 49
    0b01100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b01110000,
    3,
    0b01110000,  // 2 ascii 50
    0b10001000,
    0b00010000,
    0b00100000,
    0b01000000,
    0b10000000,
    0b11111000,
    5,
    0b11110000,  // 3 ascii 51
    0b00001000,
    0b00001000,
    0b01110000,
    0b00001000,
    0b00001000,
    0b11110000,
    5,
    0b00010000,  // 4 ascii 52
    0b00110000,
    0b01010000,
    0b11111000,
    0b00010000,
    0b00010000,
    0b00010000,
    5,
    0b11111000,  // 5 ascii 53
    0b10000000,
    0b10000000,
    0b11110000,
    0b00001000,
    0b00001000,
    0b11110000,
    5,
    0b01110000,  // 6 ascii 54
    0b10000000,
    0b10000000,
    0b11110000,
    0b10001000,
    0b10001000,
    0b01110000,
    5,
    0b11111000,  // 7 ascii 55
    0b10001000,
    0b00001000,
    0b00010000,
    0b00010000,
    0b00100000,
    0b00100000,
    5,
    0b01110000,  // 8 ascii 56
    0b10001000,
    0b10001000,
    0b01110000,
    0b10001000,
    0b10001000,
    0b01110000,
    5,
    0b01110000,  // 9 ascii 57
    0b10001000,
    0b10001000,
    0b01110000,
    0b00001000,
    0b00001000,
    0b01110000,
    5,
    0b00000000,  // : ascii 58
    0b11000000,
    0b11000000,
    0b00000000,
    0b11000000,
    0b11000000,
    0b00000000,
    2,
    0b00000000,  // ; ascii 59
    0b11000000,
    0b11000000,
    0b00000000,
    0b11000000,
    0b01000000,
    0b10000000,
    2,
    0b00000000,  // < ascii 60
    0b00100000,
    0b01000000,
    0b10000000,
    0b01000000,
    0b00100000,
    0b00000000,
    3,
    0b00000000,  // = ascii 61
    0b00000000,
    0b11100000,
    0b00000000,
    0b11100000,
    0b00000000,
    0b00000000,
    3,
    0b00000000,  // > ascii 62
    0b10000000,
    0b01000000,
    0b00100000,
    0b01000000,
    0b10000000,
    0b00000000,
    3,
    0b01110000,  // ? ascii 63
    0b10001000,
    0b00010000,
    0b00100000,
    0b00100000,
    0b00000000,
    0b00100000,
    5,
    0b01110000,  // @ ascii 64
    0b10001000,
    0b10111000,
    0b10101000,
    0b10111000,
    0b10000000,
    0b01111000,
    5,
    0b00100000,  // A ascii 65
    0b01010000,
    0b10001000,
    0b11111000,
    0b10001000,
    0b10001000,
    0b10001000,
    5,
    0b11100000,  // B ascii 66
    0b10010000,
    0b10010000,
    0b11110000,
    0b10001000,
    0b10001000,
    0b11110000,
    5,
    0b01111000,  // C ascii 67
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b01111000,
    5,
    0b11100000,  // D ascii 68
    0b10010000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b10010000,
    0b11100000,
    5,
    0b11111000,  // E ascii 69
    0b10000000,
    0b10000000,
    0b11110000,
    0b10000000,
    0b10000000,
    0b11111000,
    5,
    0b11111000,  // F ascii 70
    0b10000000,
    0b10000000,
    0b11110000,
    0b10000000,
    0b10000000,
    0b10000000,
    5,
    0b01111000,  // G ascii 71
    0b10000000,
    0b10000000,
    0b10111000,
    0b10001000,
    0b10001000,
    0b01111000,
    5,
    0b10001000,  // H ascii 72
    0b10001000,
    0b10001000,
    0b11111000,
    0b10001000,
    0b10001000,
    0b10001000,
    5,
    0b11111000,  // I ascii 73
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b11111000,
    5,
    0b00111000,  // J ascii 74
    0b00001000,
    0b00001000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b01110000,
    5,
    0b10001000,  // K ascii 75
    0b10010000,
    0b10100000,
    0b11000000,
    0b10100000,
    0b10010000,
    0b10001000,
    5,
    0b10000000,  // L ascii 76
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b11111000,
    5,
    0b10001000,  // M ascii 77
    0b11011000,
    0b11011000,
    0b10101000,
    0b10101000,
    0b10001000,
    0b10001000,
    5,
    0b10001000,  // N ascii 78
    0b10001000,
    0b11001000,
    0b10101000,
    0b10011000,
    0b10001000,
    0b10001000,
    5,
    0b01110000,  // O ascii 79
    0b10001000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b01110000,
    5,
    0b11110000,  // P ascii 80
    0b10001000,
    0b10001000,
    0b11110000,
    0b10000000,
    0b10000000,
    0b10000000,
    5,
    0b01110000,  // Q ascii 81
    0b10001000,
    0b10001000,
    0b10001000,
    0b10101000,
    0b10010000,
    0b01101000,
    5,
    0b11110000,  // R ascii 82
    0b10001000,
    0b10001000,
    0b11110000,
    0b10010000,
    0b10001000,
    0b10011000,
    5,
    0b01111000,  // S ascii 83
    0b10000000,
    0b10000000,
    0b01110000,
    0b00001000,
    0b00001000,
    0b11110000,
    5,
    0b11111000,  // T ascii 84
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    5,
    0b10001000,  // U ascii 85
    0b10001000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b01110000,
    5,
    0b10001000,  // V ascii 86
    0b10001000,
    0b10001000,
    0b10001000,
    0b10001000,
    0b01010000,
    0b00100000,
    5,
    0b10001000,  // W ascii 87
    0b10001000,
    0b10001000,
    0b10101000,
    0b10101000,
    0b01010000,
    0b01010000,
    5,
    0b10001000,  // X ascii 88
    0b10001000,
    0b01010000,
    0b00100000,
    0b01001000,
    0b10000100,
    0b10000100,
    5,
    0b10001000,  // Y ascii 89
    0b10001000,
    0b01010000,
    0b01010000,
    0b00100000,
    0b00100000,
    0b00100000,
    5,
    0b11111000,  // Z ascii 90
    0b00001000,
    0b00010000,
    0b00100000,
    0b01000000,
    0b10000000,
    0b11111000,
    5,
    0b11100000,  // [ ascii 91
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b10000000,
    0b11100000,
    3,
    0b00000000,  // \ ascii 92
    0b10000000,
    0b01000000,
    0b00100000,
    0b00010000,
    0b00001000,
    0b00000000,
    5,
    0b11100000,  // ] ascii 93
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b11100000,
    3,
    0b00100000,  // ^ ascii 94
    0b01010000,
    0b10001000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    5,
    0b00000000,  // _ ascii 95
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b11111000,
    5,
    0b10000000,  // ` ascii 96
    0b01000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    2,
};

#endif