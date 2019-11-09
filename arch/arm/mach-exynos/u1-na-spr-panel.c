/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/ld9040.h>

#define SLEEPMSEC		0x1000
#define ENDDEF			0x2000
#define	DEFMASK			0xFF00
#define COMMAND_ONLY		0xFE
#define DATA_ONLY		0xFF


static const unsigned short SEQ_USER_SETTING[] = {
	0xF0, 0x5A,

	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short SEQ_DISPCTL[] = {
	0xF2, 0x02,

	DATA_ONLY, 0x06,
	DATA_ONLY, 0x0A,
	DATA_ONLY, 0x10,
	DATA_ONLY, 0x10,
	ENDDEF, 0x00
};

static const unsigned short SEQ_GTCON[] = {
	0xF7, 0x09,

	ENDDEF, 0x00
};

static const unsigned short SEQ_PANEL_CONDITION[] = {
	0xF8, 0x05,
	DATA_ONLY, 0x5E,
	DATA_ONLY, 0x96,
	DATA_ONLY, 0x6B,
	DATA_ONLY, 0x7D,
	DATA_ONLY, 0x0D,
	DATA_ONLY, 0x3F,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x32,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x07,
	DATA_ONLY, 0x05,
	DATA_ONLY, 0x1F,
	DATA_ONLY, 0x1F,
	DATA_ONLY, 0x1F,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x00,
	ENDDEF, 0x00
};

static const unsigned short SEQ_PWR_CTRL[] = {
	0xF4, 0x0A,

	DATA_ONLY, 0x87,
	DATA_ONLY, 0x25,
	DATA_ONLY, 0x6A,
	DATA_ONLY, 0x44,
	DATA_ONLY, 0x02,
	ENDDEF, 0x00
};

static const unsigned short SEQ_SLPOUT[] = {
	0x11, COMMAND_ONLY,
	SLEEPMSEC, 120,
	ENDDEF, 0x00
};

static const unsigned short SEQ_SLPIN[] = {
	0x10, COMMAND_ONLY,
	ENDDEF, 0x00
};

static const unsigned short SEQ_DISPON[] = {
	0x29, COMMAND_ONLY,
	ENDDEF, 0x00
};

static const unsigned short SEQ_DISPOFF[] = {
	0x28, COMMAND_ONLY,
	ENDDEF, 0x00
};

static const unsigned short SEQ_ACL_ON[] = {
	0xC0, 0x01,

	ENDDEF, 0x00
};

static const unsigned short SEQ_ACL_OFF[] = {
	0xC0, 0x00,

	ENDDEF, 0x00
};

static const unsigned short SEQ_ACL_40P[] = {
	0xC1, 0x4D,

	DATA_ONLY, 0x96, DATA_ONLY, 0x1D, DATA_ONLY, 0x00, DATA_ONLY, 0x00,
	DATA_ONLY, 0x01, DATA_ONLY, 0xDF, DATA_ONLY, 0x00, DATA_ONLY, 0x00,
	DATA_ONLY, 0x03, DATA_ONLY, 0x1F, DATA_ONLY, 0x00, DATA_ONLY, 0x00,
	DATA_ONLY, 0x00, DATA_ONLY, 0x00, DATA_ONLY, 0x00, DATA_ONLY, 0x01,
	DATA_ONLY, 0x06, DATA_ONLY, 0x11, DATA_ONLY, 0x1A, DATA_ONLY, 0x20,
	DATA_ONLY, 0x25, DATA_ONLY, 0x29, DATA_ONLY, 0x2D, DATA_ONLY, 0x30,
	DATA_ONLY, 0x33, DATA_ONLY, 0x35,

	0xC0, 0x01,

	ENDDEF, 0x00
};

static const unsigned short SEQ_ACL_50P[] = {
	0xC1, 0x4D,

	DATA_ONLY, 0x96, DATA_ONLY, 0x1D, DATA_ONLY, 0x00,	DATA_ONLY, 0x00,
	DATA_ONLY, 0x01, DATA_ONLY, 0xDF, DATA_ONLY, 0x00,	DATA_ONLY, 0x00,
	DATA_ONLY, 0x03, DATA_ONLY, 0x1F, DATA_ONLY, 0x00,	DATA_ONLY, 0x00,
	DATA_ONLY, 0x00, DATA_ONLY, 0x00, DATA_ONLY, 0x00,	DATA_ONLY, 0x01,
	DATA_ONLY, 0x08, DATA_ONLY, 0x16, DATA_ONLY, 0x22,	DATA_ONLY, 0x2B,
	DATA_ONLY, 0x31, DATA_ONLY, 0x37, DATA_ONLY, 0x3B,	DATA_ONLY, 0x3F,
	DATA_ONLY, 0x43, DATA_ONLY, 0x46,

	0xC0, 0x01,

	ENDDEF, 0x00
};

static const unsigned short *ACL_cutoff_set[] = {
	SEQ_ACL_OFF,
	SEQ_ACL_40P,
	SEQ_ACL_50P,
};


static const unsigned short SEQ_ELVSS_ON[] = {
	0xB1, 0x0F,

	DATA_ONLY, 0x00,
	DATA_ONLY, 0x16,
	ENDDEF, 0x00
};

/* ELVSS set for SM2 4.52" LCD */
static const unsigned short SEQ_SM2_ELVSS_45[] = {
	0xB2, 0x14,
	DATA_ONLY, 0x14,
	DATA_ONLY, 0x14,
	DATA_ONLY, 0x14,
	ENDDEF, 0x00
};

static const unsigned short SEQ_SM2_ELVSS_38[] = {
	0xB2, 0x1B,
	DATA_ONLY, 0x1B,
	DATA_ONLY, 0x1B,
	DATA_ONLY, 0x1B,
	ENDDEF, 0x00
};

static const unsigned short SEQ_SM2_ELVSS_35[] = {
	0xB2, 0x1E,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0x1E,
	ENDDEF, 0x00
};

static const unsigned short SEQ_SM2_ELVSS_31[] = {
	0xB2, 0x22,
	DATA_ONLY, 0x22,
	DATA_ONLY, 0x22,
	DATA_ONLY, 0x22,
	ENDDEF, 0x00
};

static const unsigned short *SEQ_SM2_ELVSS_set[] = {
	SEQ_SM2_ELVSS_31,
	SEQ_SM2_ELVSS_35,
	SEQ_SM2_ELVSS_38,
	SEQ_SM2_ELVSS_45,
};

/* 160cd for SM2 4.52" LCD : default */
static const unsigned short SEQ_SM2_GAMMA_SET1[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xB1,
	DATA_ONLY, 0xC5,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x75,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB0,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xB0,
	DATA_ONLY, 0xC2,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x8C,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xB4,
	DATA_ONLY, 0xAD,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x98,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short SEQ_GAMMA_CTRL[] = {
	0xFB, 0x02,

	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_300[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xAB,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x9D,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB2,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xA8,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB2,
	DATA_ONLY, 0xB1,
	DATA_ONLY, 0xA6,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xC8,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_290[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xAB,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x9A,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xA8,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB4,
	DATA_ONLY, 0xB0,
	DATA_ONLY, 0xA6,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xC5,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_280[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xB6,
	DATA_ONLY, 0xAC,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x97,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xB6,
	DATA_ONLY, 0xA9,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xB1,
	DATA_ONLY, 0xA7,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xC1,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_270[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xB6,
	DATA_ONLY, 0xAC,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x94,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xB6,
	DATA_ONLY, 0xA9,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xB0,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB4,
	DATA_ONLY, 0xB0,
	DATA_ONLY, 0xA7,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xBE,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_260[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xAB,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x92,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xA9,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xAD,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB4,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xA6,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xBB,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_250[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xAC,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x8F,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xAA,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xAA,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB4,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xA8,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xB7,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_240[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xAD,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x8C,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB2,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xAA,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xA7,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xB4,
	DATA_ONLY, 0xA8,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xB4,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_230[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xAD,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x8A,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB2,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xAC,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xA4,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xA9,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xB1,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_220[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xAF,
	DATA_ONLY, 0xC0,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x87,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB2,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xAD,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xA1,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xB2,
	DATA_ONLY, 0xAA,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xAE,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_210[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xB0,
	DATA_ONLY, 0xC0,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x84,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB2,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xAD,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x9E,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xAB,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xAA,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_200[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xB0,
	DATA_ONLY, 0xC2,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x81,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB2,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xAD,
	DATA_ONLY, 0xC0,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x9A,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xB2,
	DATA_ONLY, 0xAB,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xA6,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_190[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xB0,
	DATA_ONLY, 0xC3,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x7E,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB2,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xAD,
	DATA_ONLY, 0xC0,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x97,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xAB,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xA3,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_180[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xB0,
	DATA_ONLY, 0xC3,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x7B,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB0,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xAF,
	DATA_ONLY, 0xC0,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x93,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB6,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xAC,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x9F,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_170[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xB1,
	DATA_ONLY, 0xC4,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x78,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB0,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xB0,
	DATA_ONLY, 0xC2,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x8F,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB6,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xAD,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x9B,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_160[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xB1,
	DATA_ONLY, 0xC5,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x75,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB0,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xB0,
	DATA_ONLY, 0xC2,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x8C,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xB4,
	DATA_ONLY, 0xAD,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x98,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_150[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xB1,
	DATA_ONLY, 0xC6,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x72,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xAE,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xB1,
	DATA_ONLY, 0xC3,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x88,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB6,
	DATA_ONLY, 0xB6,
	DATA_ONLY, 0xAD,
	DATA_ONLY, 0xC2,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x94,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_140[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xC6,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x6E,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xAE,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xB1,
	DATA_ONLY, 0xC4,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x84,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB6,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xAE,
	DATA_ONLY, 0xC3,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x8F,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_130[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xB2,
	DATA_ONLY, 0xC7,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x6B,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xAE,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0xB1,
	DATA_ONLY, 0xC5,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x80,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xAD,
	DATA_ONLY, 0xC4,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x8B,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_120[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xB4,
	DATA_ONLY, 0xC6,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x68,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xAC,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xC5,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x7C,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xB0,
	DATA_ONLY, 0xC3,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x87,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_110[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB6,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xC8,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x64,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xAC,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xB4,
	DATA_ONLY, 0xC7,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x77,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xB1,
	DATA_ONLY, 0xC5,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x82,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_100[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB6,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xB6,
	DATA_ONLY, 0xC8,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x60,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xAC,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xC7,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x73,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xB2,
	DATA_ONLY, 0xC6,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x7D,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_90[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB6,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xC9,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x5C,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xAC,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xB6,
	DATA_ONLY, 0xC8,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x6E,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xB2,
	DATA_ONLY, 0xC7,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x78,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_80[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xCA,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x57,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xAC,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xCA,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x68,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xC8,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x72,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_70[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xCC,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x52,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xAC,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xCB,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x63,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xB4,
	DATA_ONLY, 0xCA,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x6C,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_60[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xAA,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xCD,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x4C,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xA2,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xCD,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x5C,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xB6,
	DATA_ONLY, 0xCB,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x65,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_50[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xA9,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xCD,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x47,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xA2,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xCD,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x56,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xB6,
	DATA_ONLY, 0xCB,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x5F,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_40[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xA6,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xD0,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x3F,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xA2,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0xCF,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x4E,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xC0,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xCD,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x56,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_22_30_dimming[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xA3,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xCF,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x38,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xA2,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0xD0,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x45,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xC4,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xCD,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x4D,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_300[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC0,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xB1,
	DATA_ONLY, 0xC3,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x9D,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xAF,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xB6,
	DATA_ONLY, 0xAE,
	DATA_ONLY, 0xC0,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xC8,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_290[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC0,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xB1,
	DATA_ONLY, 0xC3,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x9A,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xAF,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xAD,
	DATA_ONLY, 0xC0,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xC5,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_280[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xB2,
	DATA_ONLY, 0xC4,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x97,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xB0,
	DATA_ONLY, 0xC2,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xAE,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xC1,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_270[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xC5,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x94,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xB0,
	DATA_ONLY, 0xC3,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xB0,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xAE,
	DATA_ONLY, 0xC2,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xBE,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_260[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC0,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xB2,
	DATA_ONLY, 0xC6,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x92,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xB0,
	DATA_ONLY, 0xC4,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xAD,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xAE,
	DATA_ONLY, 0xC3,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xBB,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_250[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xC6,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x8F,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xB1,
	DATA_ONLY, 0xC4,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xAA,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xAF,
	DATA_ONLY, 0xC3,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xB7,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_240[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xB4,
	DATA_ONLY, 0xC6,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x8C,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xB2,
	DATA_ONLY, 0xC4,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xA7,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xB0,
	DATA_ONLY, 0xC3,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xB4,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_230[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xC6,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x8A,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xC4,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xA4,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xB1,
	DATA_ONLY, 0xC3,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xB1,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_220[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xC7,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x87,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xC5,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xA1,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xB1,
	DATA_ONLY, 0xC4,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xAE,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_210[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xC8,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x84,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xC6,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x9E,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xB1,
	DATA_ONLY, 0xC5,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xAA,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_200[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xB6,
	DATA_ONLY, 0xC9,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x81,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0xB4,
	DATA_ONLY, 0xC7,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x9A,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xB2,
	DATA_ONLY, 0xC6,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xA6,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_190[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC2,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xCA,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x7E,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xC7,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x97,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xB3,
	DATA_ONLY, 0xC6,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0xA3,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_180[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC2,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xCA,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x7B,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xB6,
	DATA_ONLY, 0xC7,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x93,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xB4,
	DATA_ONLY, 0xC6,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x9F,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_170[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC2,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xCB,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x78,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xB6,
	DATA_ONLY, 0xC9,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x8F,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xB4,
	DATA_ONLY, 0xC8,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x9B,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_160[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC3,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xCC,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x75,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xC9,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x8C,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xC8,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x98,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_150[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC3,
	DATA_ONLY, 0xC0,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xCC,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x72,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xC2,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xC9,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x88,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xB4,
	DATA_ONLY, 0xC8,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x94,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_140[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xCC,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x6E,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xC2,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xCA,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x84,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xC9,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x8F,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_130[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC2,
	DATA_ONLY, 0xC2,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xCD,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x6B,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xC3,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xCB,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x80,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0xB6,
	DATA_ONLY, 0xCA,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x8B,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_120[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC2,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xCD,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x68,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xC2,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xCC,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x7C,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xCB,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x87,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_110[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC3,
	DATA_ONLY, 0xC2,
	DATA_ONLY, 0xBB,
	DATA_ONLY, 0xCE,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x64,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xC4,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xCD,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x77,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xB7,
	DATA_ONLY, 0xCB,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x82,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_100[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC2,
	DATA_ONLY, 0xC4,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xCD,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x60,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xC4,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xCD,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x73,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xCC,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x7D,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_90[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC2,
	DATA_ONLY, 0xC4,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xCE,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x5C,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xC6,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xCE,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x6E,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xB9,
	DATA_ONLY, 0xCD,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x78,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_80[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0xC4,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xD0,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x57,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB8,
	DATA_ONLY, 0xC7,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0xD0,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x68,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xC0,
	DATA_ONLY, 0xBA,
	DATA_ONLY, 0xCF,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x72,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_70[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC0,
	DATA_ONLY, 0xC4,
	DATA_ONLY, 0xC0,
	DATA_ONLY, 0xD1,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x52,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xC7,
	DATA_ONLY, 0xC0,
	DATA_ONLY, 0xD0,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x63,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xCF,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x6C,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_60[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC0,
	DATA_ONLY, 0xC5,
	DATA_ONLY, 0xC0,
	DATA_ONLY, 0xD4,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x4C,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xC7,
	DATA_ONLY, 0xC0,
	DATA_ONLY, 0xD3,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x5C,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xC0,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0xBC,
	DATA_ONLY, 0xD2,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x65,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_50[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xC0,
	DATA_ONLY, 0xC6,
	DATA_ONLY, 0xC2,
	DATA_ONLY, 0xD2,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x47,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xC6,
	DATA_ONLY, 0xC3,
	DATA_ONLY, 0xD3,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x56,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0xD1,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x5F,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_40[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xBD,
	DATA_ONLY, 0xC7,
	DATA_ONLY, 0xC3,
	DATA_ONLY, 0xD6,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x3F,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xC7,
	DATA_ONLY, 0xC4,
	DATA_ONLY, 0xD5,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x4E,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xC3,
	DATA_ONLY, 0xC2,
	DATA_ONLY, 0xBF,
	DATA_ONLY, 0xD4,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x56,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};

static const unsigned short ld9040_sm2_19_30_dimming[] = {
	0xF9, 0x1D,
	DATA_ONLY, 0xBE,
	DATA_ONLY, 0xC8,
	DATA_ONLY, 0xC4,
	DATA_ONLY, 0xD6,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x38,
	DATA_ONLY, 0x1C,
	DATA_ONLY, 0xB5,
	DATA_ONLY, 0xC7,
	DATA_ONLY, 0xC7,
	DATA_ONLY, 0xD6,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x45,
	DATA_ONLY, 0x1E,
	DATA_ONLY, 0xC7,
	DATA_ONLY, 0xC4,
	DATA_ONLY, 0xC1,
	DATA_ONLY, 0xD4,
	DATA_ONLY, 0x00,
	DATA_ONLY, 0x4D,
	0xFB, 0x02,
	DATA_ONLY, 0x5A,
	ENDDEF, 0x00
};


/* SM2 4.52" GAMMA 2.2 Table */
static const unsigned short *psm2_22Gamma_set[] = {
	ld9040_sm2_22_30_dimming,
	ld9040_sm2_22_40,
	ld9040_sm2_22_70,
	ld9040_sm2_22_90,
	ld9040_sm2_22_100,
	ld9040_sm2_22_110,
	ld9040_sm2_22_120,
	ld9040_sm2_22_130,
	ld9040_sm2_22_140,
	ld9040_sm2_22_150,
	ld9040_sm2_22_160,
	ld9040_sm2_22_170,
	ld9040_sm2_22_180,
	ld9040_sm2_22_190,
	ld9040_sm2_22_200,
	ld9040_sm2_22_210,
	ld9040_sm2_22_220,
	ld9040_sm2_22_230,
	ld9040_sm2_22_240,
	ld9040_sm2_22_250,
	ld9040_sm2_22_260,
	ld9040_sm2_22_270,
	ld9040_sm2_22_280,
	ld9040_sm2_22_290,
	ld9040_sm2_22_300,
};


/* SM2 4.52" GAMMA 1.9 Table */
static const unsigned short *psm2_19Gamma_set[] = {
	ld9040_sm2_19_30_dimming,
	ld9040_sm2_19_40,
	ld9040_sm2_19_70,
	ld9040_sm2_19_90,
	ld9040_sm2_19_100,
	ld9040_sm2_19_110,
	ld9040_sm2_19_120,
	ld9040_sm2_19_130,
	ld9040_sm2_19_140,
	ld9040_sm2_19_150,
	ld9040_sm2_19_160,
	ld9040_sm2_19_170,
	ld9040_sm2_19_180,
	ld9040_sm2_19_190,
	ld9040_sm2_19_200,
	ld9040_sm2_19_210,
	ld9040_sm2_19_220,
	ld9040_sm2_19_230,
	ld9040_sm2_19_240,
	ld9040_sm2_19_250,
	ld9040_sm2_19_260,
	ld9040_sm2_19_270,
	ld9040_sm2_19_280,
	ld9040_sm2_19_290,
	ld9040_sm2_19_300,
};


struct ld9040_panel_data u1_panel_data = {
	.seq_user_set = SEQ_USER_SETTING,
	.seq_displayctl_set = SEQ_DISPCTL,
	.seq_gtcon_set = SEQ_GTCON,
	.seq_panelcondition_set = SEQ_PANEL_CONDITION,
	.seq_pwrctl_set = SEQ_PWR_CTRL,
	.display_on = SEQ_DISPON,
	.display_off = SEQ_DISPOFF,
	.sleep_in = SEQ_SLPIN,
	.sleep_out = SEQ_SLPOUT,
	.gamma19_table = psm2_19Gamma_set,
	.gamma22_table = psm2_22Gamma_set,
	.acl_table = ACL_cutoff_set,
	.elvss_table = SEQ_SM2_ELVSS_set,
	.acl_on = SEQ_ACL_ON,
	.elvss_on = SEQ_ELVSS_ON,
	.lcdtype = LCDTYPE_SM2_A1,
};
