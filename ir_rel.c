/*  Copyright (C) 2013 Louis Marchand <prog@tioui.com>
 *  Base on the ir_ptr and acc plugins from L. Donnie Smith <donnie.smith@gatech.edu>
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
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#include "wmplugin.h"

#define MAX_AXES_VALUE 32
#define MIN_AXES_VALUE -32

#define X_VAL_MULTIPLIER -1
#define Y_VAL_MULTIPLIER 1

cwiid_wiimote_t *wiimote;

static struct wmplugin_info info;
static struct wmplugin_data data;

wmplugin_info_t wmplugin_info;
wmplugin_init_t wmplugin_init;
wmplugin_exec_t wmplugin_exec;

struct wmplugin_info *wmplugin_info() {
	static unsigned char info_init = 0;

	if (!info_init) {
		info.button_count = 0;
		info.axis_count = 2;
		info.axis_info[0].name = "X";
		info.axis_info[0].type = WMPLUGIN_REL;
		info.axis_info[0].max  =  MAX_AXES_VALUE;
		info.axis_info[0].min  = MIN_AXES_VALUE;
		info.axis_info[0].fuzz = 0;
		info.axis_info[0].flat = 0;
		info.axis_info[1].name = "Y";
		info.axis_info[1].type = WMPLUGIN_REL;
		info.axis_info[1].max  =  MAX_AXES_VALUE;
		info.axis_info[1].min  = MIN_AXES_VALUE;
		info.axis_info[1].fuzz = 0;
		info.axis_info[1].flat = 0;
		info.param_count = 0;
		info_init = 1;
	}
	return &info;
}

int wmplugin_init(int id, cwiid_wiimote_t *arg_wiimote)
{
	wiimote = arg_wiimote;

	data.buttons = 0;

	data.axes[0].valid = 1;
	data.axes[1].valid = 1;

	if (wmplugin_set_rpt_mode(id, CWIID_RPT_IR)) {
		return -1;
	}

	return 0;
}


struct wmplugin_data *wmplugin_exec(int mesg_count, union cwiid_mesg mesg[])
{
	static char src_valid[CWIID_IR_SRC_COUNT];
	static uint16_t src_pos_x[CWIID_IR_SRC_COUNT];
	static uint16_t src_pos_y[CWIID_IR_SRC_COUNT];

	int i;
	int diff_count = 0;
	int diff_x_avg = 0;
	int diff_y_avg = 0;
	__s32 x_val,y_val;	

	struct cwiid_ir_mesg *ir_mesg;

	ir_mesg = NULL;
	for (i=0; i < mesg_count; i++) {
		if (mesg[i].type == CWIID_MESG_IR) {
			ir_mesg = &mesg[i].ir_mesg;
		}
	}
	if (!ir_mesg) {
		return NULL;
	}
	for(i=0;i<CWIID_IR_SRC_COUNT;i++){
		if(ir_mesg->src[i].valid && src_valid[i]){
			diff_x_avg=diff_x_avg+(ir_mesg->src[i].pos[0]-src_pos_x[i]);
			diff_y_avg=diff_y_avg+(ir_mesg->src[i].pos[1]-src_pos_y[i]);

			diff_count=diff_count+1;
		}
	}
	for(i=0;i<CWIID_IR_SRC_COUNT;i++){
		src_valid[i] = ir_mesg->src[i].valid;
		src_pos_x[i] = ir_mesg->src[i].pos[0];
		src_pos_y[i] = ir_mesg->src[i].pos[1];
	}
	if(diff_count!=0){
		x_val=(diff_x_avg*X_VAL_MULTIPLIER)/diff_count;
		y_val=(diff_y_avg*Y_VAL_MULTIPLIER)/diff_count;
		// printf("Value: (%d,%d).\n",x_val,y_val);

		if(x_val>MAX_AXES_VALUE){
			x_val=MAX_AXES_VALUE;
		}
		if(y_val>MAX_AXES_VALUE){
			y_val=MAX_AXES_VALUE;
		}
		if(x_val<MIN_AXES_VALUE){
			x_val=MIN_AXES_VALUE;
		}
		if(y_val<MIN_AXES_VALUE){
			y_val=MIN_AXES_VALUE;
		}
		data.axes[0].value = x_val;
		data.axes[1].value = y_val;
	}else{
		data.axes[0].value = 0;
		data.axes[1].value = 0;
	}

	return &data;
}
