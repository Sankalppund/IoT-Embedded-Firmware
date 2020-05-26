/*
 * ble_handler.c
 *
 *  Created on: Oct 18, 2019
 *      Author: sankalp Pund
 * ############################################################################
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *#############################################################################
 * * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 *
 *Reference: Lecture Slides
 *Reference: https://docs.silabs.com/bluetooth/latest/
 */

/* Board headers */
#include "init_mcu.h"
#include "init_board.h"
#include "init_app.h"
#include "ble-configuration.h"
#include "board_features.h"

/* Bluetooth stack headers */
#include "bg_types.h"
#include "native_gecko.h"
#include "gatt_db.h"

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"

/* Device initialization header */
#include "hal-config.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif

#include "em_device.h"
#include "em_chip.h"
#include "gpio.h"
#ifndef MAX_CONNECTIONS
#define MAX_CONNECTIONS 4
#endif

#include"Letimer0.h"
#include"Sleep.h"

//Logging headers
#define INCLUDE_LOG_DEBUG 1
#define INCLUDE_LOG_INFO 1
#include"log.h"
#include "em_core.h"
#include "i2c_interrupt.h"
#include "main.h"
#include "display.h"
#include "stdio.h"
#include "state_machine.h"
#include "ble_handler.h"
#include "math.h"

int Val_of_RSSI = 0;

uint8_t connection_value = 0;

int connection_flag = 0;

uint32_t srv = 0;
uint32_t chr = 0;
uint32_t service_handle;
uint32_t chr_handle;
uint8_t *TempBuff;
uint8_t temperature[5];
float x =0;

// Temperature Measurement characteristic UUID defined by Bluetooth SIG
const uint8_t temp_UUID_data[2] = {0x1C,0x2A};

// Health Thermometer service UUID defined by Bluetooth SIG
const uint8_t thr_service_UUID_data[2] = {0x09, 0x18};


/************************************
 *
 * Function name: ble_event_handler()
 *
 * return: none.
 *
 * reference:soc-smartphone SDK example.
 ***************************************/

void ble_event_handler(struct gecko_cmd_packet*evt)
{
	gecko_update(evt);

	switch(BGLIB_MSG_ID(evt->header))
	{
	case gecko_evt_system_boot_id:

		gecko_cmd_le_gap_set_advertise_timing(0,400,400,0,0);

		gecko_cmd_le_gap_start_advertising(0,le_gap_general_discoverable, le_gap_connectable_scannable);

		displayPrintf(DISPLAY_ROW_NAME, "Sankalp Gecko Server");

		displayPrintf(DISPLAY_ROW_BTADDR, "Advertising!!!");

		ble_address();


		break;

	case gecko_evt_le_connection_closed_id:

		gecko_cmd_system_halt(1);
		gecko_cmd_system_set_tx_power(0);
		gecko_cmd_system_halt(0);

		gecko_cmd_le_gap_start_advertising(1,le_gap_general_discoverable, le_gap_connectable_scannable);

		connection_flag = 0;

		displayPrintf(DISPLAY_ROW_CONNECTION, "Disconnect");
		displayPrintf(DISPLAY_ROW_TEMPVALUE,"");

		break;

	case gecko_evt_le_connection_rssi_id:

		Val_of_RSSI = evt->data.evt_le_connection_rssi.rssi;

		if(Val_of_RSSI > -35)
		{
			gecko_cmd_system_set_tx_power(-260);
		}
		else if( -45 < Val_of_RSSI && Val_of_RSSI <= -35)
		{
			gecko_cmd_system_set_tx_power(-200);
		}
		else if( -55 < Val_of_RSSI && Val_of_RSSI <= -45)
		{
			gecko_cmd_system_set_tx_power(-150);
		}
		else if( -65 < Val_of_RSSI && Val_of_RSSI <= -55)
		{
			gecko_cmd_system_set_tx_power(-50);
		}
		else if( -75 < Val_of_RSSI && Val_of_RSSI <= -65)
		{
			gecko_cmd_system_set_tx_power(0);
		}
		else if( -85 < Val_of_RSSI && Val_of_RSSI <= -75)
		{
			gecko_cmd_system_set_tx_power(50);
		}
		else if(Val_of_RSSI <= -85)
		{
			gecko_cmd_system_set_tx_power(80);
		}

		break;

	case gecko_evt_le_connection_opened_id:

		displayPrintf(DISPLAY_ROW_CONNECTION, "Connected");

		connection_value = evt->data.evt_le_connection_opened.connection;

		gecko_cmd_le_connection_set_timing_parameters(connection_value,60,60,3,900,0,0);

		connection_flag = 1;

		break;

	case gecko_evt_system_external_signal_id:

		if((evt->data.evt_system_external_signal.extsignals && SetupLoadPowerManagement_flag==0x01)!=0)
		{
			event_status = SetupLoadPowerManagement;

			SetupLoadPowerManagement_flag = 0;

		}
		if((evt->data.evt_system_external_signal.extsignals && StartI2CWriteTransfer_flag==0x02)!=0)
		{
			event_status = StartI2CWriteTransfer;

			StartI2CWriteTransfer_flag = 0;

		}
		if((evt->data.evt_system_external_signal.extsignals && SetupTempTimer_flag==0x04)!=0)
		{

			event_status = SetupTempTimer;

			SetupTempTimer_flag = 0;

		}
		if((evt->data.evt_system_external_signal.extsignals && StartI2CReadTransfer_flag==0x08)!=0)
		{

			event_status = StartI2CReadTransfer;

			StartI2CReadTransfer_flag = 0;
		}
		if((evt->data.evt_system_external_signal.extsignals && TemperatureCalculation_flag==0x10)!=0)
		{
			event_status = TemperatureCalculation;

			TemperatureCalculation_flag = 0;
		}


		break;

	case gecko_evt_gatt_server_characteristic_status_id:

		if(evt->data.evt_gatt_server_characteristic_status.status_flags == gatt_server_confirmation)
		{
			gecko_cmd_le_connection_get_rssi(evt->data.evt_gatt_server_characteristic_status.connection);
		}
		break;

	case gecko_evt_hardware_soft_timer_id:

		displayUpdate();

	}
}


/*******************************************
 *
 * Function name: ble_client()
 *
 * return: none.
 *
 * reference:soc-thermometer-client SDK example.
 ************************************************/

void ble_client(struct gecko_cmd_packet*evt)
{

	switch(BGLIB_MSG_ID(evt->header))
	{

	case gecko_evt_system_boot_id:                                // This event indicates the device has started and the radio is ready. This event carries the firmware build number and other SW and HW identification codes.

		LOG_INFO("\n\n Boot Id");

		gecko_cmd_le_gap_set_discovery_timing(PHYS, SCAN_INTERVAL, SCAN_WINDOW);

		gecko_cmd_le_gap_set_discovery_type(PHYS, SCAN_TYPE);

		gecko_cmd_le_gap_connect(server_Addr, le_gap_address_type_public, INITIATING_PHY);        // le_gap enumerations

		gecko_cmd_le_connection_set_timing_parameters(connection_value,60,60,3,900,0,0);

		struct gecko_msg_le_gap_start_discovery_rsp_t* res1 = gecko_cmd_le_gap_start_discovery(le_gap_phy_1m, le_gap_discover_generic);

		LOG_INFO("Response for start_discovery is %d ",res1->result);

		displayPrintf(DISPLAY_ROW_NAME, "Client");

		ble_address();

		displayPrintf(DISPLAY_ROW_BTADDR2,"%x:%x:%x:%x:%x:%x",server_Addr.addr[5],server_Addr.addr[4],server_Addr.addr[3],server_Addr.addr[2],server_Addr.addr[1],server_Addr.addr[0]);

		displayPrintf(DISPLAY_ROW_PASSKEY, "Discovering");

		break;

	case gecko_evt_le_connection_opened_id:                               // This event indicates that a new connection was opened

		displayPrintf(DISPLAY_ROW_PASSKEY, "Handling Indications");

		connection_value = evt->data.evt_le_connection_opened.connection;

		struct gecko_msg_gatt_discover_primary_services_by_uuid_rsp_t* res2=gecko_cmd_gatt_discover_primary_services_by_uuid(connection_value, UUID_LEN, (const uint8_t*)thr_service_UUID_data);

		LOG_INFO("Response for discover_primary_services_by_uuid %d ",res2->result);

		break;


	case gecko_evt_le_connection_closed_id:                          //This event indicates that a connection was closed.

		gecko_cmd_le_gap_start_discovery(le_gap_phy_1m, le_gap_discover_generic);                 //  le_gap_phy_type enum & le_gap_discover_mode enum

		displayPrintf(DISPLAY_ROW_PASSKEY, "Discovering");
		displayPrintf(DISPLAY_ROW_TEMPVALUE,"");

		break;


	case gecko_evt_gatt_server_characteristic_status_id:             //This event indicates either that a local Client Characteristic Configuration descriptor has been changed by the remote GATT client, or that a confirmation from the remote GATT client was received upon a successful reception of the indication.

		if(evt->data.evt_gatt_server_characteristic_status.status_flags == gatt_server_confirmation)
		{
			struct gecko_msg_le_connection_get_rssi_rsp_t* res3 = gecko_cmd_le_connection_get_rssi(evt->data.evt_gatt_server_characteristic_status.connection);

			LOG_INFO("Response for gecko_cmd_le_connection_get_rssi%d ",res3->result);
		}
		break;

	case gecko_evt_hardware_soft_timer_id:                          //This event indicates that the soft timer has lapsed.

		displayUpdate();

		break;

	case gecko_evt_gatt_characteristic_value_id:                     //This event indicates that the value of one or several characteristics in the remote GATT server was received.


		gecko_cmd_gatt_send_characteristic_confirmation(connection_value);



		TempBuff=&evt->data.evt_gatt_characteristic_value.value.data[0];

		x = gattUint32ToFloat(TempBuff);

		displayPrintf(DISPLAY_ROW_TEMPVALUE,"Temp=%.2f",x);


		break;



	case gecko_evt_gatt_service_id:                               //This event indicates that a GATT service in the remote GATT database was discovered.

		srv=1;

		service_handle = evt->data.evt_gatt_service.service;

		break;


	case gecko_evt_gatt_characteristic_id:                      //This event indicates that a GATT characteristic in the remote GATT database was discovered.

		chr=1;

		chr_handle = evt->data.evt_gatt_characteristic.characteristic;

		break;


	case gecko_evt_gatt_procedure_completed_id:               //This event indicates that the current GATT procedure has been completed successfully or that it has failed with an error.

		if(srv==1)
		{
			srv=0;

			struct gecko_msg_gatt_discover_characteristics_by_uuid_rsp_t* res5=gecko_cmd_gatt_discover_characteristics_by_uuid(connection_value, service_handle, UUID_LEN,(const uint8_t*)temp_UUID_data);

			LOG_INFO("Response for discover_characteristics_by_uuid %d ",res5->result);
		}

		if(chr==1)
		{
			chr=0;

			struct gecko_msg_gatt_set_characteristic_notification_rsp_t* res6=gecko_cmd_gatt_set_characteristic_notification(connection_value, chr_handle, gatt_indication);    // gatt_indication=gatt_client_config_flag

			LOG_INFO("Response for set_characteristic_notification_rsp %d ",res6->result);
		}

		break;

	}

}

/**
 * Function Name: gattUint32ToFloat();
 *
 * @return a float value based on a UINT32 value written by FLT_TO_UINT32 and
 * UINT32_TO_BITSTREAM
 * @param value_start_little_endian is a pointer to the first byte of the float
 * which is represented in UINT32 format from FLT_TO_UINT32/UINT32_TO_BITSTREAM
 *
 * Reference:https://docs.google.com/document/d/11evaTGs1zuboJqPjHNCLha2jTSCaHxmRvG1ugGQCay8/edit(Assignment-7 document)
 */


float gattUint32ToFloat(const uint8_t *value_start_little_endian)
{
	int8_t exponent = (int8_t)value_start_little_endian[4];
	uint32_t mantissa = value_start_little_endian[1] +
			(((uint32_t)value_start_little_endian[2]) << 8) +
			(((uint32_t)value_start_little_endian[3]) << 16);

	return (float)mantissa*pow(10,exponent);
}
