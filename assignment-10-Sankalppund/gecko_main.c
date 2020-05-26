/***************************************************************************//**
 * @file
 * @brief Silicon Labs BT Mesh Empty Example Project
 * This example demonstrates the bare minimum needed for a Blue Gecko BT Mesh C application.
 * The application starts unprovisioned Beaconing after boot
 *******************************************************************************
 * # License
 * <b>Copyright 2018 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 *
 *Auther: Sankalp Pund
 *Date: 11/09/2019
 ******************************************************************************/

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
#include <gecko_configuration.h>
#include <mesh_sizes.h>

/* Libraries containing default Gecko configuration values */
#include "em_emu.h"
#include "em_cmu.h"
#include <em_gpio.h>

/* Device initialization header */
#include "hal-config.h"

#if defined(HAL_CONFIG)
#include "bsphalconfig.h"
#else
#include "bspconfig.h"
#endif
#include "src/ble_mesh_device_type.h"
#include "mesh_generic_model_capi_types.h"
#include "src/display.h"
#include "mesh_lib.h"
#include "src/log.h"
#include "src/gpio.h"
#include "src/gecko_ble_errors.h"

/***********************************************************************************************//**
 * @addtogroup Application
 * @{
 **************************************************************************************************/

/***********************************************************************************************//**
 * @addtogroup app
 * @{
 **************************************************************************************************/

// bluetooth stack heap
#define MAX_CONNECTIONS 2

uint8_t bluetooth_stack_heap[DEFAULT_BLUETOOTH_HEAP(MAX_CONNECTIONS) + BTMESH_HEAP_SIZE + 1760];

// Bluetooth advertisement set configuration
//
// At minimum the following is required:
// * One advertisement set for Bluetooth LE stack (handle number 0)
// * One advertisement set for Mesh data (handle number 1)
// * One advertisement set for Mesh unprovisioned beacons (handle number 2)
// * One advertisement set for Mesh unprovisioned URI (handle number 3)
// * N advertisement sets for Mesh GATT service advertisements
// (one for each network key, handle numbers 4 .. N+3)
//
#define MAX_ADVERTISERS (4 + MESH_CFG_MAX_NETKEYS)

static gecko_bluetooth_ll_priorities linklayer_priorities = GECKO_BLUETOOTH_PRIORITIES_DEFAULT;

// bluetooth stack configuration
extern const struct bg_gattdb_def bg_gattdb_data;

// Flag for indicating DFU Reset must be performed
uint8_t boot_to_dfu = 0;

const gecko_configuration_t config =
{
		.bluetooth.max_connections = MAX_CONNECTIONS,
		.bluetooth.max_advertisers = MAX_ADVERTISERS,
		.bluetooth.heap = bluetooth_stack_heap,
		.bluetooth.heap_size = sizeof(bluetooth_stack_heap) - BTMESH_HEAP_SIZE,
		.bluetooth.sleep_clock_accuracy = 100,
		.bluetooth.linklayer_priorities = &linklayer_priorities,
		.gattdb = &bg_gattdb_data,
		.btmesh_heap_size = BTMESH_HEAP_SIZE,
#if (HAL_PA_ENABLE)
		.pa.config_enable = 1, // Set this to be a valid PA config
#if defined(FEATURE_PA_INPUT_FROM_VBAT)
		.pa.input = GECKO_RADIO_PA_INPUT_VBAT, // Configure PA input to VBAT
#else
		.pa.input = GECKO_RADIO_PA_INPUT_DCDC,
#endif // defined(FEATURE_PA_INPUT_FROM_VBAT)
#endif // (HAL_PA_ENABLE)
		.max_timers = 16,
};

void handle_gecko_event(uint32_t evt_id, struct gecko_cmd_packet *evt);
void mesh_native_bgapi_init(void);
bool mesh_bgapi_listener(struct gecko_cmd_packet *evt);


#define TIMER_HANDLE_RESET (0x20)


#define INCLUDE_LOG_DEBUG 1
#define INCLUDE_LOG_INFO 1


extern uint32_t Button_flag ;

extern uint8_t PB0;

uint8_t tran_param=0;

uint16_t address_value;

char device_Value[20];

/*Function Prototype*/

void request_cl(uint16_t model_id,
		uint16_t element_index,
		uint16_t client_addr,
		uint16_t server_addr,
		uint16_t appkey_index,
		const struct mesh_generic_request *req,
		uint32_t transition_ms,
		uint16_t delay_ms,
		uint8_t request_flags);

void change_ser(uint16_t model_id,
		uint16_t element_index,
		const struct mesh_generic_state *current,
		const struct mesh_generic_state *target,
		uint32_t remaining_ms);


/**
 * See light switch app.c file definition
 */
void gecko_bgapi_classes_init_server_friend(void)
{
	gecko_bgapi_class_dfu_init();
	gecko_bgapi_class_system_init();
	gecko_bgapi_class_le_gap_init();
	gecko_bgapi_class_le_connection_init();
	//gecko_bgapi_class_gatt_init();
	gecko_bgapi_class_gatt_server_init();
	gecko_bgapi_class_hardware_init();
	gecko_bgapi_class_flash_init();
	gecko_bgapi_class_test_init();
	//gecko_bgapi_class_sm_init();
	//mesh_native_bgapi_init();
	gecko_bgapi_class_mesh_node_init();
	//gecko_bgapi_class_mesh_prov_init();
	gecko_bgapi_class_mesh_proxy_init();
	gecko_bgapi_class_mesh_proxy_server_init();
	//gecko_bgapi_class_mesh_proxy_client_init();
	//gecko_bgapi_class_mesh_generic_client_init();
	gecko_bgapi_class_mesh_generic_server_init();
	//gecko_bgapi_class_mesh_vendor_model_init();
	//gecko_bgapi_class_mesh_health_client_init();
	//gecko_bgapi_class_mesh_health_server_init();
	//gecko_bgapi_class_mesh_test_init();
	gecko_bgapi_class_mesh_lpn_init();
	//gecko_bgapi_class_mesh_friend_init();
}


/**
 * See main function list in soc-btmesh-switch project file
 */
void gecko_bgapi_classes_init_client_lpn(void)
{
	gecko_bgapi_class_dfu_init();
	gecko_bgapi_class_system_init();
	gecko_bgapi_class_le_gap_init();
	gecko_bgapi_class_le_connection_init();
	//gecko_bgapi_class_gatt_init();
	gecko_bgapi_class_gatt_server_init();
	gecko_bgapi_class_hardware_init();
	gecko_bgapi_class_flash_init();
	gecko_bgapi_class_test_init();
	//gecko_bgapi_class_sm_init();
	//mesh_native_bgapi_init();
	gecko_bgapi_class_mesh_node_init();
	//gecko_bgapi_class_mesh_prov_init();
	gecko_bgapi_class_mesh_proxy_init();
	gecko_bgapi_class_mesh_proxy_server_init();
	//gecko_bgapi_class_mesh_proxy_client_init();
	gecko_bgapi_class_mesh_generic_client_init();
	//gecko_bgapi_class_mesh_generic_server_init();
	//gecko_bgapi_class_mesh_vendor_model_init();
	//gecko_bgapi_class_mesh_health_client_init();
	//gecko_bgapi_class_mesh_health_server_init();
	//gecko_bgapi_class_mesh_test_init();
	gecko_bgapi_class_mesh_lpn_init();
	//gecko_bgapi_class_mesh_friend_init();

}
void gecko_main_init()
{
	// Initialize device
	initMcu();
	// Initialize board
	initBoard();
	// Initialize application
	initApp();

	// Minimize advertisement latency by allowing the advertiser to always
	// interrupt the scanner.
	linklayer_priorities.scan_max = linklayer_priorities.adv_min + 1;

	gecko_stack_init(&config);

	if( DeviceUsesClientModel() ) {
		gecko_bgapi_classes_init_client_lpn();
	} else {
		gecko_bgapi_classes_init_server_friend();
	}

	// Initialize coexistence interface. Parameters are taken from HAL config.
	gecko_initCoexHAL();

}


/*
 * Function Name - handle_gecko_event()
 * Description - This function bluetooth mesh event handler
 * Inputs - command packet type and event id.
 * Return Value - none.
 */

void handle_gecko_event(uint32_t evt_id, struct gecko_cmd_packet *evt)
{
	switch (evt_id) {
	case gecko_evt_system_boot_id:

		if(GPIO_PinInGet(gpioPortF,6)==0|| GPIO_PinInGet(gpioPortF,7)==0)
		{
			displayPrintf(DISPLAY_ROW_ACTION,"Factory Reset");

			/*This command can be used to erase all PS keys and their corresponding values.*/
			BTSTACK_CHECK_RESPONSE(gecko_cmd_flash_ps_erase_all());

			BTSTACK_CHECK_RESPONSE(gecko_cmd_hardware_set_soft_timer(65600,TIMER_HANDLE_RESET,1));
		}
		else
		{
			BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_node_init());

			get_address();

			displayPrintf(DISPLAY_ROW_ACTION,"");
		}

		break;


	case gecko_evt_le_connection_opened_id:

		LOG_INFO("gecko_evt_le_connection_opened_id");

		displayPrintf(DISPLAY_ROW_CONNECTION, "CONNECTED");

		break;


	case gecko_evt_hardware_soft_timer_id:


		switch(evt->data.evt_hardware_soft_timer.handle)
		{
		case TIMER_HANDLE:

			displayUpdate();

			break;

		case TIMER_HANDLE_RESET:


			/* This command can be used to reset the system.*/

		    gecko_cmd_system_reset(0);

			break;

		}


		break;


		case gecko_evt_mesh_node_initialized_id:

			if (!evt->data.evt_mesh_node_initialized.provisioned) {

				// The Node is now initialized, start unprovisioned Beaconing using PB-ADV and PB-GATT Bearers
				BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_node_start_unprov_beaconing(0x3));
			}

			if (evt->data.evt_mesh_node_initialized.provisioned) {


#if DEVICE_USES_BLE_MESH_CLIENT_MODEL

				/* Initialize generic client models */
				BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_generic_client_init());
#endif

#if DEVICE_USES_BLE_MESH_SERVER_MODEL

				/*Initialize generic server models */
				BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_generic_server_init());

#endif


#if DEVICE_IS_ONOFF_PUBLISHER

				/*Initialize Mesh helper library*/
				mesh_lib_init(malloc,free,8);

#else
				/*Initialize Mesh helper library*/
				mesh_lib_init(malloc,free,9);

				/*Register handler functions for a server model*/
				errorcode_t error_code;
				error_code=mesh_lib_generic_server_register_handler(MESH_GENERIC_ON_OFF_SERVER_MODEL_ID,0,request_cl,change_ser);
				LOG_INFO(" mesh_lib_generic_server_register_handler:%d ",error_code);
				mesh_lib_generic_server_publish(MESH_GENERIC_ON_OFF_SERVER_MODEL_ID , 0, mesh_generic_state_on_off);

				/*Update server state without sending a response to a client request*/
				error_code = mesh_lib_generic_server_update(MESH_GENERIC_ON_OFF_SERVER_MODEL_ID, 0, NULL, NULL, 0);
				LOG_INFO(" mesh_lib_generic_server_update:%d ",error_code);

				/*Publish current server state*/
				errorcode_t error_code_1;
				error_code_1=mesh_lib_generic_server_publish(MESH_GENERIC_ON_OFF_SERVER_MODEL_ID,0, mesh_generic_state_on_off);
				LOG_INFO(" mesh_lib_generic_server_publish:%d ",error_code_1);
#endif


			}
			break;

		case gecko_evt_mesh_node_provisioning_started_id:

			LOG_INFO("gecko_evt_mesh_node_provisioning_started_id");
			displayPrintf(DISPLAY_ROW_ACTION,"Provisioning");

			break;

		case gecko_evt_mesh_node_provisioned_id:

			LOG_INFO("gecko_evt_mesh_node_provisioned_id");
			displayPrintf(DISPLAY_ROW_ACTION,"Provisioned");

			break;

		case gecko_evt_mesh_node_provisioning_failed_id:

			LOG_INFO("gecko_evt_mesh_node_provisioning_failed_id");
			displayPrintf(DISPLAY_ROW_ACTION,"Provision Fail");

			break;


		case gecko_evt_mesh_generic_server_client_request_id:

			LOG_INFO("_generic_server_client_request_id");

			/*Process a generic server event*/
			mesh_lib_generic_server_event_handler(evt);

			break;

		case gecko_evt_mesh_generic_server_state_changed_id:
			LOG_INFO("gecko_evt_mesh_generic_server_state_changed_id");

			/*Process a generic server event*/
			mesh_lib_generic_server_event_handler(evt);

			break;

		case gecko_evt_mesh_node_reset_id:

			LOG_INFO("gecko_evt_mesh_node_reset_id");

			BTSTACK_CHECK_RESPONSE(gecko_cmd_hardware_set_soft_timer(65600,TIMER_HANDLE_RESET,1));

			break;


		case gecko_evt_system_external_signal_id:

			if((evt->data.evt_system_external_signal.extsignals && Button_flag==0x11)!=0)
			{
				LOG_INFO("Inside button external event");

				Button_flag = 0;

				struct mesh_generic_request req;

				uint16 publish_request;

				req.kind= mesh_generic_request_on_off;

				if (!PB0)
				{
					req.on_off=0x01;
				}
				else
				{
					req.on_off=0x00;
				}
				publish_request = mesh_lib_generic_client_publish(MESH_GENERIC_ON_OFF_CLIENT_MODEL_ID,0,tran_param,&req,0,0,0);

				LOG_INFO("Response for mesh_lib_generic_client_publish : %d",publish_request);

				tran_param++;

			}

			break;


		case gecko_evt_le_connection_closed_id:
			/* Check if need to boot to dfu mode */
			if (boot_to_dfu) {
				/* Enter to DFU OTA mode */
				gecko_cmd_system_reset(2);
			}
			break;

		case gecko_evt_gatt_server_user_write_request_id:

			LOG_INFO("gecko_evt_gatt_server_user_write_request_id ");

			if (evt->data.evt_gatt_server_user_write_request.characteristic == gattdb_ota_control) {
				/* Set flag to enter to OTA mode */
				boot_to_dfu = 1;
				/* Send response to Write Request */
				gecko_cmd_gatt_server_send_user_write_response(
						evt->data.evt_gatt_server_user_write_request.connection,
						gattdb_ota_control,
						bg_err_success);

				/* Close connection to enter to DFU OTA mode */
				gecko_cmd_le_connection_close(evt->data.evt_gatt_server_user_write_request.connection);
			}
			break;
		default:
			break;
	}
}

/*
 * Function Name - request_cl
 * Description - Client request handler function for generic server model. Passed as argument in mesh_lib_generic_server_register_handler
 * Inputs - none
 * Return Value - none
 */

void request_cl(uint16_t model_id,
		uint16_t element_index,
		uint16_t client_addr,
		uint16_t server_addr,
		uint16_t appkey_index,
		const struct mesh_generic_request *req,
		uint32_t transition_ms,
		uint16_t delay_ms,
		uint8_t request_flags)
{
	LOG_INFO("Request value is: %d", req->on_off);

	if(req->on_off == 0x00)
	{
		displayPrintf(DISPLAY_ROW_ACTION,"BUTTON PRESSED");
	}

	if(req->on_off == 0x01)
	{
		displayPrintf(DISPLAY_ROW_ACTION,"BUTTON RELEASED");
	}
}

/*
 * Function Name - change_ser
 * Description - Server state change handler function for generic server model.  Passed as argument in mesh_lib_generic_server_register_handler
 * Inputs - none
 * Return Value -none
 */

void change_ser(uint16_t model_id,
		uint16_t element_index,
		const struct mesh_generic_state *current,
		const struct mesh_generic_state *target,
		uint32_t remaining_ms)
{

}

/*
 * Function Name - gatt_db_device_addr
 * Description - This function gives publisher and subscriber address.
 * Inputs - address
 * Return Value - none
 */

void gatt_db_device_addr(bd_addr *ble_address)
{
	displayPrintf(DISPLAY_ROW_BTADDR, " %x:%x:%x:%x:%x:%x", ble_address-> addr[5], ble_address-> addr[4], ble_address-> addr[3], ble_address-> addr[2], ble_address-> addr[1], ble_address-> addr[0]);

	address_value = gecko_cmd_gatt_server_write_attribute_value(gattdb_device_name, 0, strlen(device_Value), (uint8 *)device_Value)->result;

#if(DEVICE_IS_ONOFF_PUBLISHER ==1)

	sprintf(device_Value, "5823Pub%02x:%02x", ble_address->addr[1], ble_address->addr[0]);
	displayPrintf(DISPLAY_ROW_NAME, "%s", device_Value);

#else

	sprintf(device_Value, "5823Sub%02x:%02x", ble_address->addr[1], ble_address->addr[0]);
	displayPrintf(DISPLAY_ROW_NAME, "%s", device_Value);

#endif

}

/*
 * Function Name - get_address
 * Description - gets the address of using gatt_db_device_addr();
 * Inputs - none
 * Return Value - none
 */

void get_address()
{
	struct gecko_msg_system_get_bt_address_rsp_t* address_result = gecko_cmd_system_get_bt_address();
	gatt_db_device_addr(&address_result->address);
}
