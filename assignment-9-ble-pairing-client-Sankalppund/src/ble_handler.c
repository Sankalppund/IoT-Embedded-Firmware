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
#include "em_gpio.h"
#include "infrastructure.h"

int Val_of_RSSI = 0;

uint8_t connection_value = 0;

uint32_t passkey_value=0;

uint32_t passkey_data=0;

uint8_t passkey_handle =0;

uint8_t bonding_handle =0;

uint8_t value_id_connection=0;

int connection_flag = 0;

char passkey[20]={0};

uint8_t PB0 = 0;

int SM_FLAG =0;

extern Button_flag;

uint32_t srv = 0;
uint32_t srv_2=0;
uint32_t chr = 0;
uint32_t chr_2 = 0;
//uint8_t service_handle[16];
uint32_t service_handle[2];
uint16_t chr_handle[2];
uint8_t *TempBuff;
uint8_t temperature[5];
float x =0;
uint8_t conn_handle;

int temp_Ser=0;
int temp_ser_1=0;

// Temperature Measurement characteristic UUID defined by Bluetooth SIG
const uint8_t temp_UUID_data[2] = {0x1C,0x2A};

const uint8_t custom_UUID_data[16] = {0x89,0x62,0x13,0x2d,0x2a,0x65,0xec,0x87,0x3e,0x43,0xc8,0x38,0x02,0x00,0x00,0x00};

// Health Thermometer service UUID defined by Bluetooth SIG
const uint8_t thr_service_UUID_data[2] = {0x09, 0x18};

const uint8_t custom_service_UUID_data[16] = {0x89,0x62,0x13,0x2d,0x2a,0x65,0xec,0x87,0x3e,0x43,0xc8,0x38,0x01,0x00,0x00,0x00};


/************************************
 *
 * Function name: ble_event_handler()
 *
 * return: none.
 *
 * reference:soc-smartphone SDK example.
 *
 ***************************************/

void ble_event_handler(struct gecko_cmd_packet*evt)
{
	gecko_update(evt);

	switch(BGLIB_MSG_ID(evt->header))
	{
	case gecko_evt_system_boot_id:


		/* This command can be used to delete all bonding information and whitelist from Persistent Store. */

		gecko_cmd_sm_delete_bondings();

		//LOG_INFO("In Case of boot ID, Response for sm_delete_bondings is %d ",LOG1->result);

		/* This command can be used to configure security requirements and I/O capabilities of the system.*/

		struct gecko_msg_sm_configure_rsp_t*LOG2=gecko_cmd_sm_configure(FLAG,sm_io_capability_displayyesno);

		LOG_INFO("In Case of boot ID, Response for sm_configure is %d ",LOG2->result);

		/* This command can be used to set whether the device should accept new bondings. By default,
		 * the device does not accept new bondings. */

		struct gecko_msg_sm_set_bondable_mode_rsp_t*LOG3=gecko_cmd_sm_set_bondable_mode(BONDABLE_VALUE);

		LOG_INFO("In Case of boot ID, Response for sm_set_bondable_mode is %d ",LOG3->result);

		/* This command can be used to set the advertising timing parameters of the given advertising set.
		This setting will take effect on the next advertising enabling.*/

		struct gecko_msg_le_gap_set_advertise_timing_rsp_t*LOG4=gecko_cmd_le_gap_set_advertise_timing(0,INTERVAL_MIN,INTERVAL_MAX,DURATION,MAX_EVENTS);

		LOG_INFO("In Case of boot ID, Response for le_gap_set_advertise_timing is %d ",LOG4->result);

		/* This command can be used to start the advertising of the given advertising
        /* set with specified discoverable and connectable modes.*/

		struct gecko_msg_le_gap_start_advertising_rsp_t*LOG5=gecko_cmd_le_gap_start_advertising(0,le_gap_general_discoverable, le_gap_connectable_scannable);

		LOG_INFO("In Case of boot ID, Response for le_gap_start_advertising is %d ",LOG5->result);

		displayPrintf(DISPLAY_ROW_NAME, "Server");

		displayPrintf(DISPLAY_ROW_BTADDR2, "Advertising");

		ble_address();



		break;

	case gecko_evt_le_connection_closed_id:

		/* This command forces radio to idle state and allows device to sleep.*/

		gecko_cmd_system_halt(HALT_VALUE);

		/*This command can be used to set the global maximum TX power for Bluetooth.*/

		struct gecko_msg_system_set_tx_power_rsp_t*LOG7=gecko_cmd_system_set_tx_power(POWER_VALUE);

		LOG_INFO("In Case of closed ID, Response for gecko_cmd_system_set_tx_power %d ",LOG7->set_power);

		/* This command forces radio to idle state and allows device to sleep.*/

		struct gecko_msg_system_halt_rsp_t*LOG8=gecko_cmd_system_halt(RESUME_VALUE);

		LOG_INFO("In Case of closed ID, Response for gecko_cmd_system_halt %d ",LOG8->result);

		/*This command can be used to start the advertising of the given advertising set with specified discoverable and connectable modes.*/

		struct gecko_msg_le_gap_start_advertising_rsp_t*LOG9=gecko_cmd_le_gap_start_advertising(1,le_gap_general_discoverable, le_gap_connectable_scannable);

		LOG_INFO("In Case of closed ID, Response for le_gap_start_advertising %d ",LOG9->result);

		struct gecko_msg_sm_delete_bondings_rsp_t*LOG10=gecko_cmd_sm_delete_bondings();

		LOG_INFO("In Case of closed ID, Response for sm_delete_bondings %d ",LOG10->result);

		connection_flag = 0;
		SM_FLAG = 0;

		displayPrintf(DISPLAY_ROW_CONNECTION, "Disconnect");
		displayPrintf(DISPLAY_ROW_TEMPVALUE,"");
		displayPrintf(DISPLAY_ROW_BTADDR2, "Advertising");
		displayPrintf(DISPLAY_ROW_ACTION,"");

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

		displayPrintf(DISPLAY_ROW_BTADDR2, "");

		displayPrintf(DISPLAY_ROW_ACTION,"");

		connection_value = evt->data.evt_le_connection_opened.connection;

		/* This command can be used to request a change in the connection parameters of a Bluetooth  connection. */

		struct gecko_msg_le_connection_set_timing_parameters_rsp_t*LOG11=gecko_cmd_le_connection_set_timing_parameters(connection_value,MIN_INTERVAL,MAX_INTERVAL,LATENCY,TIMEOUT,MIN_CE_LENGTH,MIN_CE_LENGTH);

		LOG_INFO("In Case of closed ID, Response for sm_delete_bondings %d ",LOG11->result);

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
		if((evt->data.evt_system_external_signal.extsignals && Button_flag==0x11)!=0)
		{
			PB0_state();

			Button_flag = 0;
		}


		break;

	case gecko_evt_gatt_server_characteristic_status_id:

		if(evt->data.evt_gatt_server_characteristic_status.status_flags == gatt_server_confirmation)
		{
			/* This command can be used to get the latest RSSI value of a Bluetooth connection.*/

			struct gecko_msg_le_connection_get_rssi_rsp_t* LOG15=gecko_cmd_le_connection_get_rssi(evt->data.evt_gatt_server_characteristic_status.connection);

			LOG_INFO("In Case of gatt_server_characteristic_status, Response for le_connection_get_rssi %d ",LOG15->result);
		}
		break;

	case gecko_evt_hardware_soft_timer_id:

		displayUpdate();

		break;


	case gecko_evt_sm_confirm_passkey_id:

		displayPrintf(DISPLAY_ROW_ACTION,"Confirm with PB0");

		passkey_handle = evt->data.evt_sm_confirm_passkey.connection;

		passkey_value = evt->data.evt_sm_passkey_display.passkey;

		passkey_data=evt->data.evt_sm_confirm_passkey.passkey;

		snprintf(passkey, sizeof(passkey),"%lu",passkey_data);

		displayPrintf(DISPLAY_ROW_PASSKEY,passkey);


		/* This command can be used to enhance the security of a connection to current security requirements. On an unencrypted connection,
		   this will encrypt the connection and will also perform bonding if requested by both devices. On an encrypted connection,
		   this will cause the connection re-encrypted. */

		//struct gecko_msg_sm_increase_security_rsp_t*LOG12=gecko_cmd_sm_increase_security(connection_value);

		//LOG_INFO("In Case of sm_confirm_passkey, Response for sm_increase_security %d ",LOG12->result);

		while(GPIO_PinInGet(gpioPortF,6)!=0)
		{
			displayPrintf(DISPLAY_ROW_ACTION,"PB0 released.");
		}

		displayPrintf(DISPLAY_ROW_ACTION,"PB0 Pressed.");

		displayPrintf(DISPLAY_ROW_PASSKEY,"");

		/* This command can be used for accepting or rejecting reported confirm value.*/

		struct gecko_msg_sm_passkey_confirm_rsp_t*LOG13=gecko_cmd_sm_passkey_confirm(passkey_handle, CONFIRM);

		LOG_INFO("In Case of sm_confirm_passkey, Response for sm_passkey_confirm %d ",LOG13->result);

		break;

	case gecko_evt_sm_bonded_id:

		displayPrintf(DISPLAY_ROW_ACTION,"Bonded");

		SM_FLAG=1;   // Flag set to measure temperature only after bonding.

		break;

	case gecko_evt_sm_bonding_failed_id:

		displayPrintf(DISPLAY_ROW_ACTION,"Bonding Failed");

		break;

	case gecko_evt_sm_confirm_bonding_id:

		/*This command can be used for accepting or rejecting bonding request. */

		gecko_cmd_sm_bonding_confirm(evt->data.evt_sm_confirm_bonding.connection, CONFIRM);


		break;
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

		/*gecko_cmd_sm_delete_bondings*/

		struct gecko_msg_sm_delete_bondings_rsp_t*LOG18=gecko_cmd_sm_delete_bondings(1);

		LOG_INFO("In Case of Boot ID, Response for sm_delete_bondings %d ",LOG18->result);

		/*This command can be used to configure security requirements and I/O capabilities of the system.*/

		struct gecko_msg_sm_configure_rsp_t*LOG26=gecko_cmd_sm_configure(FLAG,sm_io_capability_displayyesno);

		LOG_INFO("In Case of Boot ID, Response for sm_configure %d ",LOG26->result);

		/*This command can be used to set whether the device should accept new bondings. By default, the device does not accept new bondings.*/

		struct gecko_msg_sm_set_bondable_mode_rsp_t*LOG27=gecko_cmd_sm_set_bondable_mode(1);

		LOG_INFO("In Case of Boot ID, Response for set_bondable_mode %d ",LOG27->result);

		/*  This command can be used to set the timing parameters of the specified PHYs.*/

		struct gecko_msg_le_gap_set_discovery_timing_rsp_t*LOG28=gecko_cmd_le_gap_set_discovery_timing(PHYS, SCAN_INTERVAL, SCAN_WINDOW);

		LOG_INFO("In Case of Boot ID, Response for gap_set_discovery_timing %d ",LOG28->result);

		/*  This command can be used to set the scan type of the specified PHYs. If the device is currently scanning for advertising devices on the PHYs, new parameters will take effect when the scanning is restarted */

		struct gecko_msg_le_gap_set_discovery_type_rsp_t*LOG29=gecko_cmd_le_gap_set_discovery_type(PHYS, SCAN_TYPE);

		LOG_INFO("In Case of Boot ID, Response for gecko_cmd_le_gap_set_discovery_type %d ",LOG29->result);

		/*This command can be used to connect an advertising device with the specified initiating PHY. */

		struct gecko_msg_le_gap_connect_rsp_t*LOG30=gecko_cmd_le_gap_connect(server_Addr, le_gap_address_type_public, INITIATING_PHY);        // le_gap enumerations

		LOG_INFO("In Case of Boot ID, Response for gecko_cmd_le_gap_connect %d ",LOG30->result);

		/*This command can be used to request a change in the connection parameters of a Bluetooth  connection.*/

		struct gecko_msg_le_connection_set_timing_parameters_rsp_t*LOG31=gecko_cmd_le_connection_set_timing_parameters(connection_value,60,60,3,900,0,0);

		LOG_INFO("In Case of Boot ID, Response for gecko_cmd_le_connection_set_timing_parameters %d ",LOG31->result);

		/* This command can be used to start the GAP discovery procedure to scan for advertising devices on the specified scanning PHY, that is to perform a device discovery.*/

		struct gecko_msg_le_gap_start_discovery_rsp_t* res1 = gecko_cmd_le_gap_start_discovery(le_gap_phy_1m, le_gap_discover_generic);

		LOG_INFO("Response for start_discovery is %d ",res1->result);

		displayPrintf(DISPLAY_ROW_NAME, "Client");

		ble_address();

		displayPrintf(DISPLAY_ROW_BTADDR2,"%x:%x:%x:%x:%x:%x",server_Addr.addr[5],server_Addr.addr[4],server_Addr.addr[3],server_Addr.addr[2],server_Addr.addr[1],server_Addr.addr[0]);

		displayPrintf(DISPLAY_ROW_PASSKEY, "Discovering");

		break;

	case gecko_evt_le_connection_opened_id:                               // This event indicates that a new connection was opened

		LOG_INFO("Open Id");

		displayPrintf(DISPLAY_ROW_PASSKEY, "Handling Indications");

		connection_value = evt->data.evt_le_connection_opened.connection;

		/*This command can be used to discover all the primary services of a remote GATT database.*/

		struct gecko_msg_gatt_discover_primary_services_rsp_t*LOG57=gecko_cmd_gatt_discover_primary_services(connection_value);

		LOG_INFO("Response for discover_primary_services_by_uuid 57********* %d",LOG57->result);

		break;


	case gecko_evt_le_connection_closed_id:                          //This event indicates that a connection was closed.

		/* This command can be used to start the GAP discovery procedure to scan for advertising devices on the specified scanning PHY, that is to perform a device discovery. */

		gecko_cmd_le_gap_start_discovery(le_gap_phy_1m, le_gap_discover_generic);                 //  le_gap_phy_type enum & le_gap_discover_mode enum

		/*This command can be used to connect an advertising device with the specified initiating PHY. */

		gecko_cmd_le_gap_connect(server_Addr, le_gap_address_type_public, INITIATING_PHY);

		displayPrintf(DISPLAY_ROW_PASSKEY, "Discovering");
		displayPrintf(DISPLAY_ROW_TEMPVALUE,"");
		displayPrintf(DISPLAY_ROW_ACTION,"");

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

		value_id_connection = evt->data.evt_gatt_characteristic_value.connection;

		/* This command must be used to send a characteristic confirmation to a remote GATT server after receiving an indication.*/

		gecko_cmd_gatt_send_characteristic_confirmation(value_id_connection);

		TempBuff=&evt->data.evt_gatt_characteristic_value.value.data[0];

		if(evt->data.evt_gatt_characteristic_value.characteristic == chr_handle[0])
		{


		   LOG_INFO("Value of state is=%d",*TempBuff);

		   x = gattUint32ToFloat(TempBuff);

		   displayPrintf(DISPLAY_ROW_TEMPVALUE,"Temp=%.2f",x);

		   LOG_INFO("Temperature Value=%.2f",x);

		}

		if(evt->data.evt_gatt_characteristic_value.characteristic == chr_handle[1])
		{
			value_id_connection = evt->data.evt_gatt_characteristic_value.connection;

			gecko_cmd_gatt_send_characteristic_confirmation(value_id_connection);

			TempBuff=&evt->data.evt_gatt_characteristic_value.value.data[0];

		        if(*TempBuff==0)
				{
					displayPrintf(DISPLAY_ROW_ACTION,"Button Released");
				}
				if(*TempBuff==1)
				{
					displayPrintf(DISPLAY_ROW_ACTION,"Button Pressed");
				}

		}

		break;



	case gecko_evt_gatt_service_id:                               //This event indicates that a GATT service in the remote GATT database was discovered.

		/* Comparing the set UUIDs and generated UUIDs from primary services in open id*/

		if (memcmp(evt->data.evt_gatt_service.uuid.data, thr_service_UUID_data, 2) == 0)
		{
			srv=1;

			LOG_INFO("Temperature service id");

			service_handle[0] = evt->data.evt_gatt_service.service;

			LOG_INFO("Service value is:%x",service_handle[0]);


		}


		if (memcmp(evt->data.evt_gatt_service.uuid.data, custom_service_UUID_data, 16) == 0)
		{
			srv_2=1;

			LOG_INFO("custom service id");

			service_handle[1] = evt->data.evt_gatt_service.service;

			LOG_INFO("custom Service value is***************************************** %x",service_handle[1]);
		}



		break;


	case gecko_evt_gatt_characteristic_id:                      //This event indicates that a GATT characteristic in the remote GATT database was discovered.

		LOG_INFO("Inside characteristics id");


		if (memcmp(evt->data.evt_gatt_characteristic.uuid.data, temp_UUID_data, 2) == 0)
		{

			chr=1;

			chr_handle[0] = evt->data.evt_gatt_characteristic.characteristic;

			LOG_INFO("chara value is :%x",chr_handle[0]);
		}


		if (memcmp(evt->data.evt_gatt_characteristic.uuid.data, custom_UUID_data, 16) == 0)
		{

			LOG_INFO("inside custom characteristics");

			chr_2=1;

			chr_handle[1] = evt->data.evt_gatt_characteristic.characteristic;

			LOG_INFO("custom char value is:%x",chr_handle[1]);
		}

		break;


	case gecko_evt_gatt_procedure_completed_id:               //This event indicates that the current GATT procedure has been completed successfully or that it has failed with an error.

		LOG_INFO("Inside procedure completed");

		if(evt->data.evt_gatt_procedure_completed.result!=0)
		{
			LOG_INFO("Gatt procedure failed: %d",evt->data.evt_gatt_procedure_completed.result);
		}
		else
		{
			LOG_INFO("Gatt procedure successful");
		}
		if(srv==1)
		{
			srv=0;

			LOG_INFO("Inside srv_1 and service handle is %x",service_handle[0]);

			LOG_INFO("Service handle value inside procedure complete=%x",service_handle[0]);

		  /*This command can be used to discover all the characteristics of the specified GATT service in a remote GATT database having the specified UUID.
			This command generates a unique gatt_characteristic event for every discovered characteristic having the specified UUID.
			Received "gatt_procedure_completed" event indicates that this GATT procedure has successfully completed or failed with error.*/

			struct gecko_msg_gatt_discover_characteristics_by_uuid_rsp_t* res5=gecko_cmd_gatt_discover_characteristics_by_uuid(connection_value, service_handle[0], UUID_LEN,(const uint8_t*)temp_UUID_data);

   		    LOG_INFO("Response for discover_characteristics_by_uuid %d ",res5->result);

   		    break;
		}

        if(srv_2==1)
         {
        	srv_2=0;

        	LOG_INFO("Inside srv_2 and service handle is %x",service_handle[1]);

		    struct gecko_msg_gatt_discover_characteristics_by_uuid_rsp_t* res500=gecko_cmd_gatt_discover_characteristics_by_uuid(connection_value, service_handle[1], UUID_LENGTH,(const uint8_t*)custom_UUID_data);

            LOG_INFO("Response for discover_characteristics_by_uuid_2 %d ",res500->result);

            break;
         }

		if(chr==1)
		{
			chr=0;

			/* This command can be used to enable or disable the notifications and indications being sent from a remote GATT server. This procedure discovers a
			   characteristic client configuration descriptor and writes the related configuration flags to a remote GATT database.
			   A received "gatt_procedure_completed" event indicates that this GATT procedure has successfully completed or that is has failed with an error. */

			struct gecko_msg_gatt_set_characteristic_notification_rsp_t* res6=gecko_cmd_gatt_set_characteristic_notification(connection_value, chr_handle[0], gatt_indication);    // gatt_indication=gatt_client_config_flag

			LOG_INFO("Response for set_characteristic_notification_rsp %d ",res6->result);

			break;
		}
       if(chr_2==1)
        {
            chr_2=0;

			/* This command can be used to enhance the security of a connection to current security requirements. On an unencrypted connection,
			   this will encrypt the connection and will also perform bonding if requested by both devices. On an encrypted connection,
			   this will cause the connection re-encrypted. */

			struct gecko_msg_sm_increase_security_rsp_t*LOG12=gecko_cmd_sm_increase_security(connection_value);

			LOG_INFO("In Case of procedure_completed_id, Response for sm_increase_security %d ",LOG12->result);

			struct gecko_msg_gatt_set_characteristic_notification_rsp_t* res699=gecko_cmd_gatt_set_characteristic_notification(connection_value, chr_handle[1], gatt_indication);    // gatt_indication=gatt_client_config_flag

			LOG_INFO("Response for set_characteristic_notification_rsp %d ",res699->result);

			break;
		}



	case gecko_evt_sm_confirm_bonding_id:


		bonding_handle=evt->data.evt_sm_confirm_bonding.connection;

		gecko_cmd_sm_bonding_confirm(bonding_handle,1);

		break;

	case gecko_evt_sm_confirm_passkey_id:

		displayPrintf(DISPLAY_ROW_ACTION,"Confirm with PB0");

		passkey_handle = evt->data.evt_sm_confirm_passkey.connection;

		passkey_value = evt->data.evt_sm_passkey_display.passkey;

		passkey_data=evt->data.evt_sm_confirm_passkey.passkey;

		snprintf(passkey, sizeof(passkey),"%lu",passkey_data);

		displayPrintf(DISPLAY_ROW_PASSKEY,passkey);


		while(GPIO_PinInGet(gpioPortF,6)!=0)
		{
			displayPrintf(DISPLAY_ROW_ACTION,"PB0 released.");
		}

		displayPrintf(DISPLAY_ROW_ACTION,"PB0 Pressed.");

		displayPrintf(DISPLAY_ROW_PASSKEY,"");

		/* This command can be used for accepting or rejecting reported confirm value.*/

		struct gecko_msg_sm_passkey_confirm_rsp_t*LOG19=gecko_cmd_sm_passkey_confirm(passkey_handle, CONFIRM);

		LOG_INFO("In Case of sm_confirm_passkey, Response for sm_passkey_confirm %d ",LOG19->result);

		break;

	case gecko_evt_sm_bonded_id:

		displayPrintf(DISPLAY_ROW_ACTION,"Bonded");

		break;

	case gecko_evt_sm_bonding_failed_id:

		displayPrintf(DISPLAY_ROW_ACTION,"Bonding Failed");

		gecko_cmd_le_gap_set_discovery_timing(PHYS, SCAN_INTERVAL, SCAN_WINDOW);

		gecko_cmd_le_gap_set_discovery_type(PHYS, SCAN_TYPE);

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



/**
 * Function Name: PB0_state();
 *
 * This function is used to send current state value of Button PB0 when pressed to remote GATT Client.
 *
 * Return: None
 */
void PB0_state()
{

	uint8_t BTN_BUFFER[1]={0}; 		    // Buffer to store the data  - Button status and Flags.

	uint8_t *p=BTN_BUFFER; 		// Pointer to store the BTN_BUFFER.

	UINT8_TO_BITSTREAM(p, PB0);        // Macros for type conversion

	/* This command can be used to send notifications or indications to one or more remote GATT clients */

	struct gecko_msg_gatt_server_send_characteristic_notification_rsp_t* abc=gecko_cmd_gatt_server_send_characteristic_notification(0xFF, gattdb_button_state,VALUE_LENGTH,BTN_BUFFER);
	LOG_INFO("Response for gecko_msg_gatt_server_send_characteristic_notification_rsp_t %d ",abc->result);


}

