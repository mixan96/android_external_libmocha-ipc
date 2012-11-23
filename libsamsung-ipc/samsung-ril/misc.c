/**
 * This file is part of samsung-ril.
 *
 * Copyright (C) 2010-2011 Joerie de Gram <j.de.gram@gmail.com>
 * Copyright (C) 2011 Paul Kocialkowski <contact@oaulk.fr>
 *
 * samsung-ril is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * samsung-ril is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with samsung-ril.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#define LOG_TAG "RIL-MISC"
#include <utils/Log.h>

#include "samsung-ril.h"
#include "util.h"

void ril_request_get_imei(RIL_Token t)
{
	if(cached_imei[0] != 0x00)
		RIL_onRequestComplete(t, RIL_E_SUCCESS, cached_imei, sizeof(cached_imei));
	else
		ril_state.tokens.get_imei = t;
}

void ril_request_get_imeisv(RIL_Token t)
{
	//TODO: return it immediately?
	ril_state.tokens.get_imeisv = t;
	/*
	if(ril_state.tokens.get_imeisv != 0) {
		RIL_onRequestComplete(ril_state.tokens.get_imeisv, RIL_E_SUCCESS, imeisv, sizeof(char *));
		ril_state.tokens.get_imeisv = 0;
	}
	*/
}

void ril_request_baseband_version(RIL_Token t)
{

	if(ril_state.radio_state != RADIO_STATE_OFF) {	
		RIL_onRequestComplete(t, RIL_E_SUCCESS, cached_sw_version, sizeof(cached_sw_version));
	} else {		
		ril_state.tokens.baseband_version = t;
	}
}


/**
 * In: RIL_REQUEST_GET_IMSI
 *   Get the SIM IMSI
 *   Only valid when radio state is "RADIO_STATE_SIM_READY"
 *
 * Out: IPC_MISC_ME_IMSI
 *   Requests ME's IMSI
 */
void ril_request_get_imsi(RIL_Token t)
{

	//ipc_send_get(IPC_MISC_ME_IMSI, reqGetId(t));
}


/*
void ipc_misc_time_info(struct ipc_message_info *info)
{
	struct ipc_misc_time_info *nitz = (struct ipc_misc_time_info*) info->data;
	char str[128];

	sprintf(str, "%02u/%02u/%02u,%02u:%02u:%02u+%02d,%02d",
		nitz->year, nitz->mon, nitz->day, nitz->hour, nitz->min, nitz->sec, nitz->tz, 0);

	RIL_onUnsolicitedResponse(RIL_UNSOL_NITZ_TIME_RECEIVED, str, strlen(str) + 1);
}
*/