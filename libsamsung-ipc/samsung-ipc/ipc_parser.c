/**
 * This file is part of libsamsung-ipc.
 *
 * Copyright (C)
 *				 2011-2012 KB <kbjetdroid@gmail.com>
 *				 2011-2012 Dominik Marszk <dmarszk@gmail.com>
 *
 *
 * libsamsung-ipc is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libsamsung-ipc is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libsamsung-ipc.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdlib.h>
#include <stdio.h>


#include <drv.h>
#include <tapi.h>
#include <fm.h>
#include <sim.h>
#include <radio.h>

#include "ipc_private.h"

#define LOG_TAG "RIL-IPC-PARSER"
#include <utils/Log.h>



void ipc_parse_boot(struct ipc_client *client, struct modem_io *ipc_frame)
{
	DEBUG_I("Inside ipc_parse_boot\n");
	int retval, count;
	struct drvPacketHeader *rx_header;
	struct drvRequest tx_packet;

	struct modem_io request;
    void *frame;
    uint8_t *payload;
    int frame_length;

    struct fifoPacketHeader *ipc;

    char data[4][0x04] = {
    			  {0x06,0,0,0},
    };

    static struct modem_io sysDataToCP [1] = {
    		{
    			.magic = 0xCAFECAFE,
    			.cmd = 0x96,
    			.datasize = 0x04,
    		},
    };

	DEBUG_I("Frame header = 0x%x\n Frame type = 0x%x\n Frame length = 0x%x\n", ipc_frame->magic, ipc_frame->cmd, ipc_frame->datasize);

	ipc_hex_dump(client, ipc_frame->data, ipc_frame->datasize);

	for(count = 0; count < 1; count++)
	{
		payload = malloc(sysDataToCP[count].datasize);

		memcpy(payload, data[count], sysDataToCP[count].datasize);

		request.magic = sysDataToCP[count].magic;
		request.cmd = sysDataToCP[count].cmd;
		request.datasize = sysDataToCP[count].datasize;

		request.data = payload;

		ipc_client_send(client, &request);

	}
    DEBUG_I("Inside ipc_parse_boot leaving\n");

}

void ipc_parse_dbg_level(struct ipc_client *client, struct modem_io *ipc_frame)
{
	DEBUG_I("Inside ipc_parse_dbg_level\n");
	int retval, count;
	struct drvPacketHeader *rx_header;
	struct drvRequest tx_packet;

	struct modem_io request;
    void *frame;
    uint8_t *payload;
    int32_t frame_length;

    struct fifoPacketHeader *ipc;

    char data[4][0x33] = {
    			  {0x01,0,0,0},
    			  {0x0B,0,0,0,1,0,0,0},
    			  {0x02,0,0,0,0,0,0,0,0x11,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,'3','7','3','2','4','6','1','1','2',0,0,0,0,0,0,0,0},
    			  {0x0C,0,0,0,'t','e','s','t',0x20,'F','W',0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    };

    static struct modem_io sysDataToCP [4] = {
    		{
    			.magic = 0xCAFECAFE,
    			.cmd = 0x2E,
    			.datasize = 0x04,
    			//.data = data1,
    		},
    		{
    			.magic = 0xCAFECAFE,
    			.cmd = 0x0F,
    			.datasize = 0x08,
    			//.data = data2,
    		},
    		{
    			.magic = 0xCAFECAFE,
    			.cmd = 0x96,
    			.datasize = 0x33,
    			//.data = data3,
    		},
    		{
    			.magic = 0xCAFECAFE,
    			.cmd = 0x0F,
    			.datasize = 0x24,
    			//.data = data4,
    		},
    };

	DEBUG_I("Frame header = 0x%x\n Frame type = 0x%x\n Frame length = 0x%x\n", ipc_frame->magic, ipc_frame->cmd, ipc_frame->datasize);

	ipc_hex_dump(client, ipc_frame->data, ipc_frame->datasize);

	for(count = 0; count < 4; count++)
	{
		DEBUG_I("sending frame no. %d\n", count);
		payload = malloc(sysDataToCP[count].datasize);

		memcpy(payload, data[count], sysDataToCP[count].datasize);
		DEBUG_I("Copying data %d\n", count);

		request.magic = sysDataToCP[count].magic;
		request.cmd = sysDataToCP[count].cmd;
		request.datasize = sysDataToCP[count].datasize;

		request.data = payload;
		DEBUG_I("Before sending\n");

		ipc_client_send(client, &request);
		DEBUG_I("sent frame no. %d\n", count);

	}
    DEBUG_I("Inside ipc_parse_dbg_level leaving\n");

}

void ipc_parse_dbg(struct ipc_client *client, struct modem_io *ipc_frame)
{
	ipc_client_log(client, "AMSS debugstring - %s\n", (char *)(ipc_frame->data));
}


void ipc_dispatch(struct ipc_client *client, struct modem_io *ipc_frame)
{
	int32_t ret;
	switch(ipc_frame->cmd)
    {
        case FIFO_PKT_FILE:
        	ipc_parse_fm(client, ipc_frame);
			/*
        	if (ret)
        	{
        		modem_send_tapi_init(ipc_frame);
        		sim_atk_open(0);
        		sim_open_to_modem(0);
        	}*/
        break;
        case FIFO_PKT_DVB_H_DebugLevel:
            //ipc_parse_dbg_level(client, ipc_frame);
        break;
        case FIFO_PKT_BOOT:
            //ipc_parse_boot(client, ipc_frame);
        break;
        case FIFO_PKT_DRV:
        	ipc_parse_drv(client, ipc_frame);
        break;
        case FIFO_PKT_DEBUG:
        	ipc_parse_dbg(client, ipc_frame);
        break;
        case FIFO_PKT_TAPI:
        	ipc_parse_tapi(client, ipc_frame);
        break;
        case FIFO_PKT_SIM:
        	ipc_parse_sim(client, ipc_frame);
        break;
        default :
        	DEBUG_I("Packet type 0x%x not yet handled\n", ipc_frame->cmd);
        	DEBUG_I("Frame header = 0x%x\n Frame type = 0x%x\n Frame length = 0x%x\n", 
			ipc_frame->magic, ipc_frame->cmd, ipc_frame->datasize);
        	ipc_hex_dump(client, ipc_frame->data, ipc_frame->datasize);

    }
}
