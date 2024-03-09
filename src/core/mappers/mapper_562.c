/*
 *  Copyright (C) 2010-2024 Fabio Cavallo (aka FHorse)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#include <string.h>
#include "mappers.h"
#include "save_slot.h"

INLINE static void prg_fix_562(void);
INLINE static void chr_fix_562(void);
INLINE static void mirroring_fix_562(void);

const BYTE bios[0x1000] = {
	0xF6, 0x60, 0x20, 0xD3, 0xE7, 0xF0, 0x06, 0x20, 0xD3, 0xE7, 0x20, 0xF5, 0xE7, 0xA2, 0x01, 0xB5,
	0xF5, 0xA8, 0x55, 0xF7, 0x35, 0xF5, 0x95, 0xF5, 0x94, 0xF7, 0xCA, 0x10, 0xF2, 0x60, 0xA9, 0x08,
	0x08, 0xA0, 0x00, 0x18, 0x65, 0x00, 0x85, 0x00, 0xA9, 0x00, 0x65, 0x01, 0x85, 0x01, 0x28, 0xC6,
	0x02, 0x60, 0xA2, 0x08, 0xB0, 0x0A, 0xB1, 0x00, 0x8D, 0x07, 0x20, 0xC8, 0xCA, 0xD0, 0xF5, 0x60,
	0xAD, 0x07, 0x20, 0x91, 0x00, 0xB0, 0xF4, 0xA5, 0x03, 0xA2, 0x08, 0xB0, 0x07, 0x8D, 0x07, 0x20,
	0xCA, 0xD0, 0xF8, 0x60, 0xAD, 0x07, 0x20, 0xB0, 0xF7, 0xA2, 0x08, 0xA5, 0x03, 0x51, 0x00, 0x8D,
	0x07, 0x20, 0xC8, 0xCA, 0xD0, 0xF5, 0x60, 0x85, 0x04, 0x86, 0x02, 0x84, 0x03, 0x20, 0xFC, 0xE3,
	0xAD, 0x02, 0x20, 0xA5, 0xFF, 0x29, 0xFB, 0x85, 0xFF, 0x8D, 0x00, 0x20, 0xA4, 0x03, 0x8C, 0x06,
	0x20, 0xA5, 0x04, 0x29, 0xF0, 0x8D, 0x06, 0x20, 0xA9, 0x00, 0x85, 0x03, 0xA5, 0x04, 0x29, 0x0F,
	0x4A, 0x90, 0x02, 0xC6, 0x03, 0x4A, 0x90, 0x03, 0xAE, 0x07, 0x20, 0xA8, 0xF0, 0x15, 0x88, 0xF0,
	0x20, 0x88, 0xF0, 0x29, 0x88, 0x20, 0x59, 0xE8, 0xA0, 0x00, 0x20, 0x32, 0xE8, 0x20, 0x1E, 0xE8,
	0xD0, 0xF3, 0x60, 0x20, 0x32, 0xE8, 0x20, 0x32, 0xE8, 0xA9, 0x10, 0x20, 0x20, 0xE8, 0xD0, 0xF3,
	0x60, 0x20, 0x32, 0xE8, 0x20, 0x47, 0xE8, 0x20, 0x1E, 0xE8, 0xD0, 0xF5, 0x60, 0x20, 0x47, 0xE8,
	0x20, 0x32, 0xE8, 0x20, 0x1E, 0xE8, 0xD0, 0xF5, 0x60, 0xAE, 0x00, 0x78, 0x9A, 0xAD, 0x11, 0x44,
	0x29, 0xFC, 0x09, 0x06, 0x8D, 0x11, 0x44, 0xA2, 0x00, 0xBD, 0x00, 0x70, 0x95, 0x00, 0xBD, 0x00,
	0x71, 0x9D, 0x00, 0x01, 0xE8, 0xD0, 0xF2, 0xAD, 0x13, 0x44, 0x8D, 0x11, 0x44, 0xAD, 0x10, 0x44,
	0x29, 0xFC, 0x8D, 0x10, 0x44, 0xAD, 0x02, 0x20, 0x10, 0xFB, 0xAD, 0x02, 0x20, 0x10, 0xFB, 0x68,
	0xA8, 0x68, 0xAA, 0x68, 0x28, 0x6C, 0xFA, 0xFF, 0x08, 0x48, 0xAD, 0x10, 0x44, 0x29, 0x03, 0xD0,
	0x33, 0xAD, 0x10, 0x44, 0x09, 0x02, 0x8D, 0x10, 0x44, 0xAD, 0x11, 0x44, 0x8D, 0x13, 0x44, 0x09,
	0x04, 0x29, 0xFC, 0x8D, 0x11, 0x44, 0xAD, 0x06, 0x60, 0x10, 0x0B, 0x8A, 0x48, 0x98, 0x48, 0xBA,
	0x8E, 0x00, 0x78, 0x4C, 0x03, 0x60, 0xAD, 0x13, 0x44, 0x8D, 0x11, 0x44, 0xAD, 0x10, 0x44, 0x29,
	0xFC, 0x8D, 0x10, 0x44, 0xAD, 0x10, 0x44, 0x29, 0xBF, 0x8D, 0x10, 0x44, 0x29, 0x03, 0xC9, 0x02,
	0xF0, 0x05, 0x68, 0x28, 0x6C, 0xFA, 0xFF, 0x68, 0x28, 0x4C, 0x64, 0xE1, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xA9, 0x00, 0x85, 0xFC, 0x85, 0xFD, 0x20, 0x3A, 0xE1, 0x20, 0x73, 0xE3, 0xBA, 0xEE, 0x20, 0x22,
	0xE4, 0x20, 0x73, 0xE3, 0xF6, 0xEE, 0x20, 0xA3, 0xEE, 0x20, 0x3A, 0xE1, 0x20, 0x4E, 0xEC, 0x20,
	0x22, 0xE4, 0x20, 0x73, 0xE3, 0xCE, 0xEE, 0x20, 0x73, 0xE3, 0x07, 0xEF, 0x20, 0xA3, 0xEE, 0x20,
	0x3A, 0xE1, 0x20, 0x67, 0xEC, 0x20, 0x7B, 0xEC, 0x20, 0x22, 0xE4, 0x20, 0x73, 0xE3, 0xE2, 0xEE,
	0x20, 0x73, 0xE3, 0x18, 0xEF, 0x20, 0x99, 0xEE, 0x20, 0x5C, 0xE3, 0x4C, 0x00, 0xEC, 0x20, 0xD4,
	0xED, 0x20, 0x8C, 0xEC, 0xEA, 0xEA, 0xEA, 0x20, 0x42, 0xE0, 0xA5, 0x06, 0x85, 0x94, 0x85, 0x95,
	0x20, 0x01, 0xED, 0x20, 0xA7, 0xE2, 0x60, 0x20, 0xD4, 0xED, 0x20, 0xAD, 0xEE, 0x20, 0xA0, 0xEC,
	0xA5, 0x95, 0x85, 0x06, 0x20, 0x6E, 0xED, 0x20, 0xA7, 0xE2, 0x60, 0x20, 0xD4, 0xED, 0x20, 0xDC,
	0xEC, 0xA5, 0x95, 0x85, 0x06, 0x20, 0xA1, 0xED, 0x20, 0xA7, 0xE2, 0x60, 0x20, 0xE2, 0xED, 0xA0,
	0x00, 0x20, 0x12, 0xE3, 0x99, 0x00, 0x07, 0xC8, 0xC0, 0x37, 0xD0, 0xF5, 0x20, 0xCD, 0xE1, 0x60,
	0x20, 0xB4, 0xE2, 0xA5, 0xFA, 0x29, 0x2B, 0x8D, 0x25, 0x40, 0xA9, 0x00, 0x8D, 0x24, 0x40, 0xA0,
	0xC5, 0x20, 0x2C, 0xE1, 0xA0, 0x56, 0x20, 0x2C, 0xE1, 0xA9, 0x01, 0x20, 0x37, 0xE3, 0xA0, 0x00,
	0xB9, 0x00, 0x07, 0x20, 0x12, 0xE3, 0xC8, 0xC0, 0x37, 0xD0, 0xF5, 0x20, 0xF0, 0xE1, 0xA9, 0x02,
	0x20, 0x37, 0xE3, 0xA5, 0x94, 0x20, 0x12, 0xE3, 0x20, 0xF0, 0xE1, 0x60, 0x20, 0xE2, 0xED, 0xA0,
	0x00, 0x20, 0x12, 0xE3, 0xA2, 0x22, 0xD9, 0x00, 0x07, 0x20, 0x2B, 0xE2, 0xC8, 0xC0, 0x37, 0xD0,
	0xF0, 0x20, 0xCD, 0xE1, 0x20, 0x42, 0xE0, 0xA5, 0x06, 0xA2, 0x23, 0xC5, 0x94, 0x20, 0x2B, 0xE2,
	0x60, 0xA9, 0x03, 0x20, 0x16, 0xE3, 0xA0, 0x00, 0x20, 0x12, 0xE3, 0x99, 0xA0, 0x00, 0x20, 0x61,
	0xEE, 0xC8, 0xC0, 0x0F, 0xD0, 0xF2, 0x20, 0xCD, 0xE1, 0xA9, 0x04, 0x20, 0x16, 0xE3, 0x20, 0x89,
	0xEE, 0x90, 0x09, 0x20, 0x12, 0xE3, 0x20, 0x61, 0xEE, 0x4C, 0x1E, 0xED, 0x20, 0xCD, 0xE1, 0xC6,
	0x06, 0xD0, 0xCE, 0x60, 0x20, 0x10, 0xEE, 0xC9, 0x03, 0xD0, 0x32, 0xA0, 0x00, 0x20, 0x12, 0xE3,
	0x99, 0xA0, 0x00, 0x20, 0x61, 0xEE, 0xC8, 0xC0, 0x0F, 0xD0, 0xF2, 0x20, 0x22, 0xEE, 0x20, 0x10,
	0xEE, 0xC9, 0x04, 0xD0, 0x18, 0x20, 0x89, 0xEE, 0x90, 0x09, 0x20, 0x12, 0xE3, 0x20, 0x61, 0xEE,
	0x4C, 0x55, 0xED, 0x20, 0x22, 0xEE, 0xD0, 0x05, 0xE6, 0x95, 0x4C, 0x34, 0xED, 0x60, 0xA9, 0x03,
	0x20, 0x37, 0xE3, 0xA0, 0x00, 0x20, 0x68, 0xEE, 0x99, 0xA0, 0x00, 0x20, 0x12, 0xE3, 0xC8, 0xC0,
	0x0F, 0xD0, 0xF2, 0x20, 0xF0, 0xE1, 0xA9, 0x04, 0x20, 0x37, 0xE3, 0x20, 0x89, 0xEE, 0x90, 0x09,
	0x20, 0x68, 0xEE, 0x20, 0x12, 0xE3, 0x4C, 0x8B, 0xED, 0x20, 0xF0, 0xE1, 0xC6, 0x06, 0xD0, 0xCE,
	0x60, 0xA9, 0x03, 0x20, 0x16, 0xE3, 0xA0, 0x00, 0x20, 0x12, 0xE3, 0x99, 0xA0, 0x00, 0x20, 0x56,
	0xEE, 0xC8, 0xC0, 0x0F, 0xD0, 0xF2, 0x20, 0xCD, 0xE1, 0xA9, 0x04, 0x20, 0x16, 0xE3, 0x20, 0x89,
	0xEE, 0x90, 0x09, 0x20, 0x12, 0xE3, 0x20, 0x56, 0xEE, 0x4C, 0xBE, 0xED, 0x20, 0xCD, 0xE1, 0xC6,
	0x06, 0xD0, 0xCE, 0x60, 0xA9, 0x00, 0x85, 0x90, 0x8D, 0xFF, 0x43, 0x85, 0x88, 0xA9, 0x60, 0x85,
	0x89, 0x60, 0x20, 0xB4, 0xE2, 0xA0, 0xC5, 0x20, 0x2C, 0xE1, 0xA0, 0x46, 0x20, 0x2C, 0xE1, 0xA9,
	0x01, 0x20, 0x16, 0xE3, 0x60, 0xA0, 0x0F, 0xB9, 0x00, 0x07, 0xC9, 0xB0, 0xD0, 0x11, 0xC8, 0xC0,
	0x12, 0xD0, 0xF4, 0xB9, 0x00, 0x07, 0xC9, 0xB1, 0xB0, 0x05, 0xA2, 0x06, 0x4C, 0x2D, 0xE2, 0x60,
	0xA0, 0x05, 0x20, 0x2C, 0xE1, 0xA5, 0xFA, 0x09, 0x40, 0x85, 0xFA, 0x8D, 0x25, 0x40, 0x20, 0x03,
	0xE3, 0x60, 0x20, 0x12, 0xE3, 0xAD, 0x30, 0x40, 0x29, 0x40, 0xD0, 0x29, 0xA5, 0xFA, 0x09, 0x10,
	0x85, 0xFA, 0x8D, 0x25, 0x40, 0x20, 0x12, 0xE3, 0xAD, 0x30, 0x40, 0x29, 0x10, 0xD0, 0x16, 0xA5,
	0xFA, 0x29, 0x2F, 0x09, 0x04, 0x85, 0xFA, 0x8D, 0x25, 0x40, 0xA2, 0x01, 0xAD, 0x32, 0x40, 0x4A,
	0x20, 0x28, 0xE2, 0xA9, 0x00, 0x60, 0xA2, 0x00, 0xC1, 0x88, 0xF0, 0x10, 0xA2, 0x26, 0x4C, 0x2D,
	0xE2, 0xA2, 0x00, 0x81, 0x88, 0x4C, 0x6C, 0xEE, 0xA2, 0x00, 0xA1, 0x88, 0xE6, 0x88, 0xD0, 0x18,
	0xE6, 0x89, 0xA6, 0x89, 0xE0, 0xE0, 0x90, 0x10, 0xA2, 0x60, 0x86, 0x89, 0x48, 0x18, 0xA5, 0x90,
	0x69, 0x10, 0x85, 0x90, 0x8D, 0xFF, 0x43, 0x68, 0x60, 0xA5, 0xAC, 0x38, 0xE9, 0x01, 0x85, 0xAC,
	0xB0, 0x06, 0xA5, 0xAD, 0xE9, 0x00, 0x85, 0xAD, 0x60, 0x20, 0x76, 0xE4, 0x20, 0x8C, 0xE1, 0x20,
	0x5E, 0xE1, 0x60, 0x20, 0x99, 0xEE, 0x20, 0x5C, 0xE3, 0x20, 0x64, 0xE3, 0x60, 0xAD, 0x32, 0x40,
	0x29, 0x04, 0xD0, 0x01, 0x60, 0xA2, 0x07, 0x4C, 0x2D, 0xE2, 0x3F, 0x00, 0x10, 0x0F, 0x2A, 0x0F,
	0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0x3F, 0x00,
	0x10, 0x0F, 0x26, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
	0x0F, 0xFF, 0x3F, 0x00, 0x10, 0x0F, 0x22, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F,
	0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0xFF, 0x21, 0xE8, 0x0D, 0x12, 0x17, 0x1C, 0x0E, 0x1B, 0x1D, 0x24,
	0x1C, 0x18, 0x1E, 0x1B, 0x0C, 0x0E, 0xFF, 0x21, 0xE8, 0x0D, 0x12, 0x17, 0x1C, 0x0E, 0x1B, 0x1D,
	0x24, 0x1D, 0x0A, 0x1B, 0x10, 0x0E, 0x1D, 0xFF, 0x21, 0xEC, 0x07, 0x0C, 0x18, 0x19, 0x22, 0x24,
	0x18, 0x14, 0xFF, 0xA9, 0x00, 0x8D, 0xFF, 0x43, 0x8D, 0x0C, 0x44, 0x8D, 0x02, 0x01, 0xA9, 0x80,
	0x8D, 0xFF, 0x42, 0xA9, 0x03, 0x8D, 0x11, 0x44, 0xA9, 0x83, 0x8D, 0x10, 0x44, 0xD0, 0x2C, 0x78,
	0xA9, 0x00, 0x8D, 0x00, 0x20, 0x8D, 0x01, 0x20, 0x8D, 0x0C, 0x44, 0xA2, 0x03, 0xAD, 0x02, 0x20,
	0x10, 0xFB, 0xCA, 0xD0, 0xF8, 0x2C, 0x10, 0x44, 0x30, 0x09, 0x70, 0x07, 0xAD, 0x10, 0x44, 0x29,
	0x1F, 0x10, 0x18, 0xAD, 0x10, 0x44, 0x29, 0x1F, 0x8D, 0x12, 0x44, 0xAD, 0x10, 0x44, 0x29, 0xFC,
	0x09, 0x02, 0x8D, 0x10, 0x44, 0x6C, 0xF6, 0xFF, 0xAD, 0x12, 0x44, 0x8D, 0x10, 0x44, 0x8D, 0x10,
	0x44, 0x29, 0x03, 0xC9, 0x02, 0xF0, 0x09, 0xA2, 0xFF, 0x9A, 0xE8, 0x8A, 0xA8, 0x6C, 0xFC, 0xFF,
	0x20, 0xBE, 0xE6, 0x4C, 0x3F, 0xE6, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
	0xFF, 0x4C, 0xD9, 0x48, 0x4C, 0x18, 0x49, 0x4C, 0x23, 0x4F, 0x4C, 0x3F, 0x4F, 0x4C, 0x78, 0x4F
};

struct _m562 {
	BYTE prg[4];
	BYTE latch;
	struct _m562_chr {
		BYTE lock;
		BYTE reg1k[8];
		BYTE reg8k;
		BYTE lastbank;
	} chr;
	struct _m562_mode {
		BYTE m1;
		BYTE m2;
		BYTE tgd;
	} mode;
	struct _m562_irq {
		WORD target;
		WORD counter;
	} irq;
	struct _m562_fds {
		BYTE control;
		SWORD counter;
	} fds;
} m562;

void map_init_562(void) {
	EXTCL_AFTER_MAPPER_INIT(562);
	EXTCL_CPU_INIT_PC(562);
	EXTCL_CPU_WR_MEM(562);
	EXTCL_CPU_RD_MEM(562);
	EXTCL_SAVE_MAPPER(562);
	EXTCL_WR_CHR(562);
	EXTCL_RD_CHR(562);
	EXTCL_CPU_EVERY_CYCLE(562);
	map_internal_struct_init((BYTE *)&m562, sizeof(m562));

	if (info.reset >= HARD) {
		memset(&m562, 0x00, sizeof(m562));

		m562.mode.m1 = (info.mapper.submapper << 5) | (info.mapper.mirroring == MIRRORING_VERTICAL ? 0x01: 0x11) | 0x06;
		m562.mode.m2 = 0x03;
		m562.mode.tgd = 0x03;

		m562.prg[0] = 0x1C;
		m562.prg[1] = 0x1D;
		m562.prg[2] = 0x1E;
		m562.prg[3] = 0x1F;

		m562.chr.reg1k[0] = 0;
		m562.chr.reg1k[1] = 1;
		m562.chr.reg1k[2] = 2;
		m562.chr.reg1k[3] = 3;
		m562.chr.reg1k[4] = 4;
		m562.chr.reg1k[5] = 5;
		m562.chr.reg1k[6] = 6;
		m562.chr.reg1k[7] = 7;
		m562.chr.lock = FALSE;
	}

	memset(&m562.irq, 0x00, sizeof(m562.irq));
	m562.irq.counter = 0xFFFF;

	info.mapper.extend_wr = TRUE;
}
void extcl_after_mapper_init_562(void) {
	if ((info.reset == CHANGE_ROM) || (info.reset == POWER_UP)) {
		if (chrrom_size()) {
			if (!chrrom_size() || (vram_size(0) < chrrom_size())) {
				vram_set_ram_size(0, chrrom_size());
				vram_init();
			}
			memcpy(vram_pnt(0), chrrom_pnt(), chrrom_size());
		}
	}
	prg_fix_562();
	chr_fix_562();
	mirroring_fix_562();
}
void extcl_cpu_init_pc_562(BYTE nidx) {
	if (info.reset >= HARD) {
		if (miscrom_size() >= 4) {
			WORD address = 0x7000;
			WORD init = 0x7003;
			size_t size = 512;
			BYTE *data = miscrom_pnt();

			if (miscrom_size() != size) {
				address = (miscrom_byte(1) << 8) | miscrom_byte(0);
				init = (miscrom_byte(3) << 8) | miscrom_byte(2);
				size = miscrom_size() - 4;
				data = miscrom_pnt_byte(4);
			}

			if (address < 0x2000) {
				memcpy(ram_pnt_byte(nidx, address & 0x1FFF), data, size);
			} else {
				memcpy(wram_pnt_byte(address & 0x1FFF), data, size);
			}

			if (init) {
				// JSR init
				ram_wr(nidx, 0x700, 0x20);
				ram_wr(nidx, 0x701, init & 0xFF);
				ram_wr(nidx, 0x702, init >> 8);

				// JMP ($FFFC)
				ram_wr(nidx, 0x703, 0x6C);
				ram_wr(nidx, 0x704, 0xFC);
				ram_wr(nidx, 0x705, 0xFF);

				nes[nidx].c.cpu.PC.w = 0x700;
			}
		}
		r4015.value &= 0xBF;
		nes[nidx].c.irq.high &= ~APU_IRQ;
	}
}
void extcl_cpu_wr_mem_562(BYTE nidx, WORD address, BYTE value) {
	if ((address >= 0x4000) && (address <= 0x4FFF)) {
		switch (address) {
			case 0x4024:
				nes[nidx].c.irq.high &= ~EXT_IRQ;
				return;
			case 0x4025:
				m562.fds.control = value;
				if (m562.fds.control & 0x42) {
					m562.fds.counter = 0;
				}
				nes[nidx].c.irq.high &= ~EXT_IRQ;
				return;
			case 0x42FC:
			case 0x42FD:
			case 0x42FE:
			case 0x42FF:
				m562.mode.m1 = (value & 0xF0) | (address & 0x03);
				if (m562.mode.m1 >= 0x80) {
					m562.chr.lock = FALSE;
				}
				prg_fix_562();
				chr_fix_562();
				mirroring_fix_562();
				return;
			case 0x43FC:
			case 0x43FD:
			case 0x43FE:
			case 0x43FF:
				m562.mode.m2 = (value & 0xF0) | (address & 0x03);
				m562.chr.reg8k = value & 0x03;
				prg_fix_562();
				chr_fix_562();
				return;
			case 0x4400:
			case 0x4401:
			case 0x4402:
			case 0x4403:
			case 0x4404:
			case 0x4405:
			case 0x4406:
			case 0x4407:
				m562.chr.reg1k[address & 0x07] = value;
				chr_fix_562();
				return;
			case 0x440C:
				m562.irq.target = (m562.irq.target & 0x00FF) | (value << 8);
				if (!(value & 0x80)) {
					m562.irq.counter = 0x8000;
				}
				nes[nidx].c.irq.high &= ~EXT_IRQ;
				return;
			case 0x440D:
				m562.irq.target = (m562.irq.target & 0xFF00) | value;
				nes[nidx].c.irq.high &= ~EXT_IRQ;
				return;
			case 0x4411:
				m562.mode.tgd = value;
				prg_fix_562();
				chr_fix_562();
				return;
			default:
				return;
		}
	} else if (address >= 0x8000) {
		if (m562.mode.m1 & 0x02) {
			m562.latch = value;
			switch (m562.mode.m1 >> 5) {
				case 0:
				case 2:
					m562.chr.reg8k = 0;
					break;
				case 1:
				case 4:
				case 5:
					m562.chr.reg8k = value & 0x03;
					break;
				case 3:
					m562.chr.reg8k = (value & 0x30) >> 4;
					break;
				default:
					break;
			}
			m562.prg[(address >> 13) & 0x03] = value >> 2;
			prg_fix_562();
			chr_fix_562();
		}
	}
}
BYTE extcl_cpu_rd_mem_562(BYTE nidx, WORD address, UNUSED(BYTE openbus)) {
	if ((address >= 0x4000) && (address <= 0x4FFF)) {
		switch(address) {
			case 0x4400:
			case 0x4401:
			case 0x4402:
			case 0x4403:
			case 0x4404:
			case 0x4405:
			case 0x4406:
			case 0x4407:
				return (m562.chr.reg1k[address & 0x07]);
			case 0x4408:
			case 0x4409:
			case 0x440A:
			case 0x440B:
				return ((m562.prg[address & 0x03] << 2) | (m562.latch & 0x03));
			case 0x440C:
				return (m562.irq.counter >> 8);
			case 0x440D:
				return (m562.irq.counter & 0xFF);
			case 0x4411:
				return (m562.mode.tgd);
			case 0x4415:
				return (m562.mode.m1);
			case 0x4420:
				return (m562.chr.reg1k[m562.chr.lastbank]);
			default:
				return (address & 0x0800 ? bios[address & 0x07FF] : wram_rd(nidx, address));
		}
	}
	return (wram_rd(nidx, address));
}
BYTE extcl_save_mapper_562(BYTE mode, BYTE slot, FILE *fp) {
	save_slot_ele(mode, slot, m562.prg);
	save_slot_ele(mode, slot, m562.latch);
	save_slot_ele(mode, slot, m562.chr.lock);
	save_slot_ele(mode, slot, m562.chr.reg1k);
	save_slot_ele(mode, slot, m562.chr.reg8k);
	save_slot_ele(mode, slot, m562.chr.lastbank);
	save_slot_ele(mode, slot, m562.mode.m1);
	save_slot_ele(mode, slot, m562.mode.m2);
	save_slot_ele(mode, slot, m562.mode.tgd);
	save_slot_ele(mode, slot, m562.irq.target);
	save_slot_ele(mode, slot, m562.irq.counter);
	save_slot_ele(mode, slot, m562.fds.control);
	save_slot_ele(mode, slot, m562.fds.counter);
	return (EXIT_OK);
}
void extcl_wr_chr_562(BYTE nidx, WORD address, BYTE value) {
	if ((m562.mode.m1 >= 0xA0) && !(m562.mode.m1 & 0x01)) {
		m562.chr.lock = (m562.mode.m1 & 0x10);
	}
	m562.chr.lastbank = address >> 10;
	chr_wr(nidx, address, value);
}
BYTE extcl_rd_chr_562(BYTE nidx, WORD address) {
	m562.chr.lastbank = address >> 10;
	return (chr_rd(nidx, address));
}
void extcl_cpu_every_cycle_562(BYTE nidx) {
	m562.fds.counter += 3;
	while ((m562.fds.counter >= 448) && (m562.fds.control & 0x80)) {
		m562.fds.counter -= 448;
		nes[nidx].c.irq.high |= EXT_IRQ;
	}
	if (m562.irq.target & 0x8000) {
		if ((m562.irq.counter == m562.irq.target) && (m562.irq.counter != 0xFFFF)) {
			nes[nidx].c.irq.high |= EXT_IRQ;
		} else {
			m562.irq.counter++;
		}
	}
}

INLINE static void prg_fix_562(void) {
	BYTE wr = !(m562.mode.m1 & 0x02);

	if (m562.mode.tgd & 0x80) {
		memmap_auto_wp_8k(0, MMCPU(0x8000), m562.prg[0], TRUE, wr);
		memmap_auto_wp_8k(0, MMCPU(0xA000), m562.prg[1], TRUE, wr);
		memmap_auto_wp_8k(0, MMCPU(0xC000), m562.prg[2], TRUE, wr);
		memmap_auto_wp_8k(0, MMCPU(0xE000), m562.prg[3], TRUE, wr);
	} else if (!(m562.mode.m2 & 0x01)) {
		memmap_auto_wp_8k(0, MMCPU(0x8000), (((m562.mode.m2 & 0x40) >> 2) | (m562.prg[0] & 0x0F)), TRUE, wr);
		memmap_auto_wp_8k(0, MMCPU(0xA000), (((m562.mode.m2 & 0x40) >> 2) | (m562.prg[1] & 0x0F)), TRUE, wr);
		memmap_auto_wp_8k(0, MMCPU(0xC000), (m562.prg[2] & 0x0F), TRUE, wr);
		memmap_auto_wp_8k(0, MMCPU(0xE000), (m562.prg[3] & 0x0F), TRUE, wr);
	} else {
		switch (m562.mode.m1 >> 5) {
			case 0:
				memmap_auto_wp_16k(0, MMCPU(0x8000), (m562.latch & 0x07), TRUE, wr);
				memmap_auto_wp_16k(0, MMCPU(0xC000), 0x07, TRUE, wr);
				return;
			case 1:
				memmap_auto_wp_16k(0, MMCPU(0x8000), ((m562.latch & 0x3C) >> 2), TRUE, wr);
				memmap_auto_wp_16k(0, MMCPU(0xC000), 0x07, TRUE, wr);
				return;
			case 2:
				memmap_auto_wp_16k(0, MMCPU(0x8000), (m562.latch & 0x0F), TRUE, wr);
				memmap_auto_wp_16k(0, MMCPU(0xC000), 0x0F, TRUE, wr);
				return;
			case 3:
				memmap_auto_wp_16k(0, MMCPU(0x8000), 0x0F, TRUE, wr);
				memmap_auto_wp_16k(0, MMCPU(0xC000), (m562.latch & 0x0F), TRUE, wr);
				return;
			case 4:
				memmap_auto_wp_32k(0, MMCPU(0x8000), ((m562.latch & 0x30) >> 4), TRUE, wr);
				return;
			case 5:
				memmap_auto_wp_32k(0, MMCPU(0x8000), 0x03, TRUE, wr);
				return;
			case 6:
				memmap_auto_wp_8k(0, MMCPU(0x8000), (m562.latch & 0x0F), TRUE, wr);
				memmap_auto_wp_8k(0, MMCPU(0xA000), (m562.latch >> 4), TRUE, wr);
				memmap_auto_wp_16k(0, MMCPU(0xC000), 0x07, TRUE, wr);
				return;
			case 7:
				memmap_auto_wp_8k(0, MMCPU(0x8000), (m562.latch & 0x0E), TRUE, wr);
				memmap_auto_wp_8k(0, MMCPU(0xA000), ((m562.latch >> 4) | 0x01), TRUE, wr);
				memmap_auto_wp_16k(0, MMCPU(0xC000), 0x07, TRUE, wr);
				return;
		}
	}
}
INLINE static void chr_fix_562(void) {
	WORD bank[8];

	if (m562.mode.tgd & 0x40) {
		bank[0] = m562.chr.reg1k[0];
		bank[1] = m562.chr.reg1k[1];
		bank[2] = m562.chr.reg1k[2];
		bank[3] = m562.chr.reg1k[3];
		bank[4] = m562.chr.reg1k[4];
		bank[5] = m562.chr.reg1k[5];
		bank[6] = m562.chr.reg1k[6];
		bank[7] = m562.chr.reg1k[7];
	} else {
		bank[0] = m562.chr.reg8k << 3;
		bank[1] = bank[0] | 0x01;
		bank[2] = bank[0] | 0x02;
		bank[3] = bank[0] | 0x03;
		bank[4] = bank[0] | 0x04;
		bank[5] = bank[0] | 0x05;
		bank[6] = bank[0] | 0x06;
		bank[7] = bank[0] | 0x07;
	}
	{
		BYTE enabled = !(((m562.mode.m1 & 0xE1) >= 0x81) || m562.chr.lock);

		memmap_vram_wp_1k(0, MMPPU(0x0000), bank[0], TRUE, enabled);
		memmap_vram_wp_1k(0, MMPPU(0x0400), bank[1], TRUE, enabled);
		memmap_vram_wp_1k(0, MMPPU(0x0800), bank[2], TRUE, enabled);
		memmap_vram_wp_1k(0, MMPPU(0x0C00), bank[3], TRUE, enabled);
		memmap_vram_wp_1k(0, MMPPU(0x1000), bank[4], TRUE, enabled);
		memmap_vram_wp_1k(0, MMPPU(0x1400), bank[5], TRUE, enabled);
		memmap_vram_wp_1k(0, MMPPU(0x1800), bank[6], TRUE, enabled);
		memmap_vram_wp_1k(0, MMPPU(0x1C00), bank[7], TRUE, enabled);
	}
}
INLINE static void mirroring_fix_562(void) {
	switch (m562.mode.m1 & 0x11) {
		case 0x00:
			mirroring_SCR0(0);
			return;
		case 0x01:
			mirroring_V(0);
			return;
		case 0x10:
			mirroring_SCR1(0);
			return;
		case 0x11:
			mirroring_H(0);
			return;
	}
}
