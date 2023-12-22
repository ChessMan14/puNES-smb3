/*
 *  Copyright (C) 2010-2023 Fabio Cavallo (aka FHorse)
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

#include "input/standard_controller.h"
#include "info.h"
#include "conf.h"
#include "nes.h"
#include "gui.h"
#include "vs_system.h"
#include "video/gfx.h"
#include "tas.h"

INLINE static void input_turbo_buttons_standard_controller(_port *prt);

void input_wr_standard_controller(BYTE nidx, const BYTE *value, BYTE nport) {
	if ((nes[nidx].c.input.r4016 & 0x01) || ((*value) & 0x01)) {
		nes[nidx].c.input.pindex[nport] = 0;
	}
}
void input_rd_standard_controller(BYTE nidx, BYTE *value, BYTE nport, BYTE shift) {
	(*value) = !!port[nport].data.treated[nes[nidx].c.input.pindex[nport]] << shift;
	// Se $4016 e' a 1 leggo solo lo stato del primo pulsante
	// del controller. Quando verra' scritto 0 nel $4016
	// riprendero' a leggere lo stato di tutti i pulsanti.
	if (!(nes[nidx].c.input.r4016 & 0x01) && (++nes[nidx].c.input.pindex[nport] >= sizeof(port[nport].data.raw))) {
		nes[nidx].c.input.pindex[nport] = 0;
	}
}

void input_add_event_standard_controller(BYTE index) {
	js_jdev_read_port(&jsp[index], &port[index]);
	input_turbo_buttons_standard_controller(&port[index]);
}
BYTE input_decode_event_standard_controller(BYTE mode, BYTE autorepeat, DBWORD event, BYTE type, _port *prt) {
	_input_lfud_standard_controller axes;

	if (tas.type || (autorepeat && (type == KEYBOARD))) {
		return (EXIT_OK);
	}
	input_rotate_standard_controller(&axes);
	if (event == prt->input[type][BUT_A]) {
		if (!prt->turbo[TURBOA].active) {
			input_data_set_standard_controller(BUT_A, mode, prt);
		}
		return (EXIT_OK);
	} else if (event == prt->input[type][BUT_B]) {
		if (!prt->turbo[TURBOB].active) {
			input_data_set_standard_controller(BUT_B, mode, prt);
		}
		return (EXIT_OK);
	} else if (event == prt->input[type][SELECT]) {
		input_data_set_standard_controller(SELECT, mode, prt);
		return (EXIT_OK);
	} else if (event == prt->input[type][START]) {
		input_data_set_standard_controller(START, mode, prt);
		return (EXIT_OK);
	} else if (event == prt->input[type][UP]) {
		input_data_set_standard_controller(axes.up, mode, prt);
		input_updown_leftright_standard_controller(UP, axes.up, axes.down, prt);
		return (EXIT_OK);
	} else if (event == prt->input[type][DOWN]) {
		input_data_set_standard_controller(axes.down, mode, prt);
		input_updown_leftright_standard_controller(DOWN, axes.down, axes.up, prt);
		return (EXIT_OK);
	} else if (event == prt->input[type][LEFT]) {
		input_data_set_standard_controller(axes.left, mode, prt);
		input_updown_leftright_standard_controller(LEFT, axes.left, axes.right, prt);
		return (EXIT_OK);
	} else if (event == prt->input[type][RIGHT]) {
		input_data_set_standard_controller(axes.right, mode, prt);
		input_updown_leftright_standard_controller(RIGHT, axes.right, axes.left, prt);
		return (EXIT_OK);
	} else if (event == prt->input[type][TRB_A]) {
		prt->turbo[TURBOA].mode = mode;
		if (mode == PRESSED) {
			prt->turbo[TURBOA].active = TRUE;
		}
		return (EXIT_OK);
	} else if (event == prt->input[type][TRB_B]) {
		prt->turbo[TURBOB].mode = mode;
		if (mode == PRESSED) {
			prt->turbo[TURBOB].active = TRUE;
		}
		return (EXIT_OK);
	}
	return (EXIT_ERROR);
}

void input_rd_standard_controller_vs(BYTE nidx, BYTE *value, BYTE nport, BYTE shift) {
	BYTE index = nes[nidx].c.input.pindex[nport];
	BYTE protection = FALSE;
	BYTE np = nport;

	if (index == START) {
		index = SELECT;
		protection = vs_system.special_mode.type == VS_SM_Ice_Climber;
	} else if (index == SELECT) {
		index = START;
	} else if (info.mapper.expansion == EXP_VS_1P_R4017) {
		np ^= 0x01;
	}
	(*value) = (protection ? PRESSED : !!port[nport].data.treated[index]) << shift;
	// Se $4016 e' a 1 leggo solo lo stato del primo pulsante
	// del controller. Quando verra' scritto 0 nel $4016
	// riprendero' a leggere lo stato di tutti i pulsanti.
	if (!(nes[nidx].c.input.r4016 & 0x01) && (++nes[nidx].c.input.pindex[nport] >= sizeof(port[nport].data.raw))) {
		nes[nidx].c.input.pindex[nport] = 0;
	}
}

void input_data_set_standard_controller(BYTE index, BYTE value, _port *prt) {
	prt->data.raw[index] = value;
	if (index < 8) {
		prt->data.treated[index] = value;
	}
}
void input_rotate_standard_controller(_input_lfud_standard_controller *lfud) {
	lfud->left = LEFT;
	lfud->right = RIGHT;
	lfud->up = UP;
	lfud->down = DOWN;

	if ((cfg->screen_rotation | cfg->hflip_screen) && cfg->input_rotation) {
		switch (cfg->screen_rotation) {
			default:
			case ROTATE_0:
				if (cfg->hflip_screen) {
					emu_invert_bytes(&lfud->left, &lfud->right);
				}
				break;
			case ROTATE_90:
				emu_invert_bytes(&lfud->left, &lfud->down);
				emu_invert_bytes(&lfud->right, &lfud->up);
				if (!cfg->hflip_screen) {
					emu_invert_bytes(&lfud->up, &lfud->down);
				}
				break;
			case ROTATE_180:
				emu_invert_bytes(&lfud->up, &lfud->down);
				if (!cfg->hflip_screen) {
					emu_invert_bytes(&lfud->left, &lfud->right);
				}
				break;
			case ROTATE_270:
				emu_invert_bytes(&lfud->left, &lfud->up);
				emu_invert_bytes(&lfud->right, &lfud->down);
				if (!cfg->hflip_screen) {
					emu_invert_bytes(&lfud->up, &lfud->down);
				}
				break;
		}
	}
}
void input_updown_leftright_standard_controller(BYTE index, BYTE src, BYTE opposite, _port *prt) {
	BYTE *axis = NULL;

	if (cfg->input.permit_updown_leftright) {
		return;
	}
	if ((index == LEFT) || (index == RIGHT)) {
		axis = &prt->permit.left_or_right;
	} else if ((index == UP) || (index == DOWN)) {
		axis = &prt->permit.up_or_down;
	} else {
		return;
	}
	if ((*axis) == FALSE) {
		(*axis) = prt->data.raw[src] ? src : opposite;
	} else if (((*axis) == src) && (prt->data.raw[src] == RELEASED)) {
		(*axis) = prt->data.raw[opposite] ? opposite : FALSE;
		prt->data.treated[opposite] = prt->data.raw[opposite];
	}
	if ((*axis) == src) {
		prt->data.treated[opposite] = RELEASED;
	} else if ((*axis) == opposite) {
		prt->data.treated[src] = RELEASED;
	}
}

INLINE static void input_turbo_buttons_standard_controller(_port *prt) {
	if ((prt->turbo[TURBOA].mode == PRESSED) || prt->turbo[TURBOA].active) {
		if (++prt->turbo[TURBOA].counter == prt->turbo[TURBOA].frequency) {
			input_data_set_standard_controller(BUT_A, PRESSED, prt);
			prt->turbo[TURBOA].active = TRUE;
		} else if (prt->turbo[TURBOA].counter > prt->turbo[TURBOA].frequency) {
			input_data_set_standard_controller(BUT_A, RELEASED, prt);
			prt->turbo[TURBOA].active = FALSE;
			prt->turbo[TURBOA].counter = 0;
		}
	}
	if ((prt->turbo[TURBOB].mode == PRESSED) || prt->turbo[TURBOB].active) {
		if (++prt->turbo[TURBOB].counter == prt->turbo[TURBOB].frequency) {
			input_data_set_standard_controller(BUT_B, PRESSED, prt);
			prt->turbo[TURBOB].active = TRUE;
		} else if (prt->turbo[TURBOB].counter > prt->turbo[TURBOB].frequency) {
			input_data_set_standard_controller(BUT_B, RELEASED, prt);
			prt->turbo[TURBOB].active = FALSE;
			prt->turbo[TURBOB].counter = 0;
		}
	}
}
