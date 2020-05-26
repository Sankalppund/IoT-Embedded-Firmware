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
 *Auther: Sankalp Pund (This code was developed with the help of team members)
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
#include "mesh_sizes.h"
#include "src/state_machine.h"
#include "src/i2c_interrupt.h"
#include "src/sensor.h"


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
		.sleep.flags = SLEEP_FLAGS_DEEP_SLEEP_ENABLE,
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

#define FRIEND_TIMER_HANDLE (0x30)

#define LIGHT_STATE_HANDLE (0x40)

#define PROXIMITY_LEVEL_HANDLE (0x50)

#define QUEUE_LEN (2)

#define POLL_TIMEOUT (5*1000)

#define NET_KEY_INDEX (0)

#define LIGHT_KEY (0x4001)

#define PROXIMITY_KEY (0x4016)

#define INCLUDE_LOG_DEBUG 1

#define INCLUDE_LOG_INFO 1

extern uint32_t SetupLoadPowerManagement_flag;

extern uint32_t StartI2CWriteTransfer_flag;

extern uint32_t StartI2CReadTransfer_flag;

extern uint32_t SetupLuxTimer_flag;

extern uint32_t LuxCalculation_flag;

extern uint32_t Button_flag ;

extern uint32_t Button_flag_1;

extern uint32_t SetupLoadPowerManagementProx_flag;

extern uint32_t StartI2CWriteTransfer_flag_Prox;

extern uint32_t SetupProxTimer_flag;

extern uint16_t final_data;

extern uint16_t prox_val;

extern uint8_t PB0;

extern uint8_t PB1;

uint8_t tran_param=0;

uint8_t tran_param_1=0;

uint16_t address_value;

char device_Value[32];

uint8_t connection_handle=0xFF;

uint16_t maxDistance=0;

uint16_t maxLight=0;


/// Number of active Bluetooth connections
static uint8 num_connections = 0;

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

void request_cl_1(uint16_t model_id,
		uint16_t element_index,
		uint16_t client_addr,
		uint16_t server_addr,
		uint16_t appkey_index,
		const struct mesh_generic_request *req,
		uint32_t transition_ms,
		uint16_t delay_ms,
		uint8_t request_flags);

void change_ser_1(uint16_t model_id,
		uint16_t element_index,
		const struct mesh_generic_state *current,
		const struct mesh_generic_state *target,
		uint32_t remaining_ms);

void gatt_db_device_addr(bd_addr *);
void get_address();
void LPN_INIT();
void Models_INIT();
void Node_INIT();
void PD_Store(uint16_t, uint16_t);
uint16_t PD_Load(uint16_t);
void PD_Display();
void publish_light_data();
void publish_proximity_data(uint16_t);

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

	gecko_bgapi_classes_init_client_lpn();


	// Initialize coexistence interface. Parameters are taken from HAL config.
	gecko_initCoexHAL();

}


/*
 * Function Name - handle_gecko_event()
 * Description - This function is a bluetooth mesh event handler
 * Inputs - command packet type and event id.
 * Return Value - none.
 */

void handle_gecko_event(uint32_t evt_id, struct gecko_cmd_packet *evt)
{
	switch (evt_id) {

	/*This event indicates the device has started and the radio is ready.
	 * This event carries the firmware build number and other SW and HW
     *identification codes.*/

	case gecko_evt_system_boot_id:

		if(GPIO_PinInGet(gpioPortF,6)==0|| GPIO_PinInGet(gpioPortF,7)==0)
		{

			LOG_INFO("Entering Factory Data Reset");

			displayPrintf(DISPLAY_ROW_ACTION,"Factory Reset");

			/*This command can be used to erase all PS keys and their corresponding values.*/
			BTSTACK_CHECK_RESPONSE(gecko_cmd_flash_ps_erase_all());

			/*This command can be used to start a software timer. Multiple concurrent timers can be running simultaneously.*/
			BTSTACK_CHECK_RESPONSE(gecko_cmd_hardware_set_soft_timer(65600,TIMER_HANDLE_RESET,1));

		}
		else
		{
			LOG_INFO("Entering Normal Boot ID");


			/*Initializes the Mesh stack in Node role.*/
			BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_node_init());

			get_address();

			displayPrintf(DISPLAY_ROW_ACTION,"");
		}

		break;

		/*This event indicates that a new connection was opened*/

	case gecko_evt_le_connection_opened_id:


		LOG_INFO("Inside Connection Open ID");

		connection_handle = evt->data.evt_le_connection_opened.connection;

		displayPrintf(DISPLAY_ROW_CONNECTION, "CONNECTED");

		gecko_cmd_mesh_lpn_deinit(); //Turn of LPN Feature

		displayPrintf(DISPLAY_ROW_CONNECTION, "LPN OFF");

		num_connections++;

		break;

		/*This event indicates that the soft timer has lapsed.*/

	case gecko_evt_hardware_soft_timer_id:


		switch(evt->data.evt_hardware_soft_timer.handle)
		{

		case TIMER_HANDLE:

			/* Function to prevent charge buildup within the Liquid Crystal Cells.*/

			displayUpdate();

			break;

		case TIMER_HANDLE_RESET:


			/* This command can be used to reset the system.*/

			gecko_cmd_system_reset(0);

			break;

		case FRIEND_TIMER_HANDLE:

			LOG_INFO("Searching for Friend Node...");

			/*Establish a friendship. Once a friendship has been established the node can start saving power.*/

			BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_lpn_establish_friendship(0));

			break;

		case LIGHT_STATE_HANDLE:

			LOG_INFO("LIGHT STATE HANDLE");

			if(final_data>maxLight)
			{
				maxLight = final_data;

				/*Storing the Persistant Data*/
				PD_Store(LIGHT_KEY,maxLight);
			}

			/* Publish ON OFF state of LIGHT to Friend*/
			publish_light_data();


			break;


		case PROXIMITY_LEVEL_HANDLE:

			LOG_INFO("PROXIMITY LEVEL HANDLE");

			if(prox_val>maxDistance)
			{
				maxDistance = prox_val;

				/*Storing the Persistant Data*/
				PD_Store(PROXIMITY_KEY,maxDistance);
			}

			/* Publish LEVEL state of Proximity Sensor to Friend*/
			publish_proximity_data(prox_val);

		}

		break;

		/*Node initialized and operational*/

		case gecko_evt_mesh_node_initialized_id:

			LOG_INFO("NODE INITIALIZED.");

			/*Initialize generic server models */
			BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_generic_client_init());

			if (!evt->data.evt_mesh_node_initialized.provisioned)
			{

				// The Node is now initialized, start unprovisioned Beaconing using PB-ADV and PB-GATT Bearers
				BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_node_start_unprov_beaconing(0x3));

				displayPrintf(DISPLAY_ROW_PASSKEY,"Unprovisioned");

				LOG_INFO("Device is Unprovisioned.");

			}

			if (evt->data.evt_mesh_node_initialized.provisioned)
			{
				displayPrintf(DISPLAY_ROW_PASSKEY,"Provisioned");

				LOG_INFO("Device is Provisioned.");

				Node_INIT();

			}
			break;

			/*Provisioner has started provisioning this Node*/

		case gecko_evt_mesh_node_provisioning_started_id:

			LOG_INFO("Started Provisioning");

			displayPrintf(DISPLAY_ROW_PASSKEY,"Provisioning");

			break;

			/*The Node has received provisioning data (address allocation and a network key) from the Provisioner*/

		case gecko_evt_mesh_node_provisioned_id:

			LOG_INFO("Provisioning Successful");

			displayPrintf(DISPLAY_ROW_PASSKEY,"Provisioned");

			Node_INIT();

			break;

			/*Provisioning the Node has failed.*/

		case gecko_evt_mesh_node_provisioning_failed_id:

			LOG_INFO("gecko_evt_mesh_node_provisioning_failed_id");
			displayPrintf(DISPLAY_ROW_PASSKEY,"Provision Fail");

			break;


		/*This event is received when a Configuration Client has deployed a new network or application key to the node.*/

		case gecko_evt_mesh_node_key_added_id:

			LOG_INFO("Received new %s key with index %x\r\n", evt->data.evt_mesh_node_key_added.type == 0 ? "network" : "application",
					evt->data.evt_mesh_node_key_added.index);

			break;

		/*State change request sent by a client model.
		 *This may be generated either because of a request directly to this model, or a request
		 *sent to an address which is subscribed to by the model.*/

		case gecko_evt_mesh_generic_server_client_request_id:

			LOG_INFO("_generic_server_client_request_id");

			LOG_INFO("Model ID 0x%x\r\n", evt->data.evt_mesh_generic_server_client_request.model_id);

			/*Process a generic server event*/
			mesh_lib_generic_server_event_handler(evt);

			break;

			/*Cached model state changed*/

		case gecko_evt_mesh_generic_server_state_changed_id:
			LOG_INFO("gecko_evt_mesh_generic_server_state_changed_id");

			/*Process a generic server event*/
			mesh_lib_generic_server_event_handler(evt);

			break;

			/* This event notifies that a remote Configuration Client has changed the configuration of a local model.*/

		case gecko_evt_mesh_node_model_config_changed_id:

			LOG_INFO("Model Configuration Changed");


			break;

			/*Provisioner has instructed the node to reset itself.*/

		case gecko_evt_mesh_node_reset_id:

			LOG_INFO("gecko_evt_mesh_node_reset_id");

			/*start a software timer*/
			BTSTACK_CHECK_RESPONSE(gecko_cmd_hardware_set_soft_timer(65600,TIMER_HANDLE_RESET,1));

			break;

			/*This event indicates external signals have been received.*/

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
			if((evt->data.evt_system_external_signal.extsignals && SetupLuxTimer_flag==0x04)!=0)
			{

				event_status = SetupLuxTimer;

				SetupLuxTimer_flag = 0;

			}
			if((evt->data.evt_system_external_signal.extsignals && StartI2CReadTransfer_flag==0x08)!=0)
			{

				event_status = StartI2CReadTransfer;

				StartI2CReadTransfer_flag = 0;
			}
			if((evt->data.evt_system_external_signal.extsignals && LuxCalculation_flag==0x10)!=0)
			{

				event_status = LuxCalculation;

				LuxCalculation_flag = 0;
			}

			if((evt->data.evt_system_external_signal.extsignals && Button_flag==0x11)!=0)
			{

				LOG_INFO("Inside button external event");

				Button_flag = 0;

			}

			if((evt->data.evt_system_external_signal.extsignals && Button_flag_1==0x12)!=0)
			{

				Button_flag_1 = 0;

				PD_Display();


			}
			if((evt->data.evt_system_external_signal.extsignals && SetupLoadPowerManagementProx_flag==0x13)!=0)
			{

				event_status = SetupLoadPowerManagement;

				SetupLoadPowerManagementProx_flag = 0;


			}
			if((evt->data.evt_system_external_signal.extsignals && StartI2CWriteTransfer_flag_Prox==0x14)!=0)
			{

				event_status = StartI2CWriteTransfer;

				StartI2CWriteTransfer_flag_Prox = 0;


			}
			if((evt->data.evt_system_external_signal.extsignals && SetupProxTimer_flag==0x15)!=0)
			{

				event_status = SetupLuxTimer;

				SetupProxTimer_flag = 0;

			}

			break;

			/*This event indicates that a connection was closed*/

		case gecko_evt_le_connection_closed_id:

			/* Check if need to boot to dfu mode */

			if (boot_to_dfu)
			{

				/* Enter to DFU OTA mode */

				gecko_cmd_system_reset(2);
			}

			LOG_INFO("evt:conn closed, reason 0x%x\r\n", evt->data.evt_le_connection_closed.reason);

			connection_handle=0xFF;

			if(num_connections>0)
			{
				if(--num_connections == 0)
				{
					displayPrintf(DISPLAY_ROW_CONNECTION,"");

					LPN_INIT();
				}
			}

			break;

			/*This event indicates that a remote GATT client is attempting to write a value of an attribute in to the local GATT database*/

		case gecko_evt_gatt_server_user_write_request_id:

			LOG_INFO("Inside Write Request ID");

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

			/*Indication that a friendship has been established*/

		case gecko_evt_mesh_lpn_friendship_established_id:

			LOG_INFO("Friendship Established");

			displayPrintf(DISPLAY_ROW_ACTION,"Friendship Est.");

			gecko_cmd_hardware_set_soft_timer(32800*5,LIGHT_STATE_HANDLE,0);

			gecko_cmd_hardware_set_soft_timer(32800*5,PROXIMITY_LEVEL_HANDLE,0);


			break;

			/*Indication that friendship establishment failed*/

		case gecko_evt_mesh_lpn_friendship_failed_id:

			LOG_INFO("Friendship Failed");

			displayPrintf(DISPLAY_ROW_ACTION,"Friendship Failed");

			gecko_cmd_hardware_set_soft_timer(65600,FRIEND_TIMER_HANDLE,1);

			break;

			/*Indication that a friendship that was successfully established has been terminated*/

		case gecko_evt_mesh_lpn_friendship_terminated_id:

			LOG_INFO("Friendship Terminated");

			displayPrintf(DISPLAY_ROW_ACTION,"Friendship Term.");

			if(num_connections==0)
			{
				gecko_cmd_hardware_set_soft_timer(65600,FRIEND_TIMER_HANDLE,1);
			}

			break;

	}
}

/*
 * Function Name - request_cl
 * Description - This is callback function for mesh_lib_generic_server_register_handler
 * @param model_id Model that is being published
 * @param element_index Element where the model resides
 * @param transaction_id Transaction ID, used for messages that are defined
 * to contain one and ignored for others
 * @param req Request structure
 * @param transition_ms Requested transition time in milliseconds,
 * or zero for immediate state transition
 * @param delay_ms Requested delay before starting transition or
 * setting the immediate value, in milliseconds
 * @param request_flags Request flags
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

}

/*
 * Function Name - change_ser
 * Description - Server state change handler function for generic server model
 * @param model_id Model that generated the event
 * @param element_index Element where the model resides
 * @param current Current model state
 * @param target Target model state, towards which model is moving;
 * in case there is no state transition ongoing a NULL pointer will
 * be given
 * @param remaining_ms Time remaining for transition in milliseconds;
 * in case there is no state transition ongoing a value of zero will
 * be given
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
 * Function Name - request_cl_1
 * Description - This is callback function for mesh_lib_generic_server_register_handler
 * @param model_id Model that is being published
 * @param element_index Element where the model resides
 * @param transaction_id Transaction ID, used for messages that are defined
 * to contain one and ignored for others
 * @param req Request structure
 * @param transition_ms Requested transition time in milliseconds,
 * or zero for immediate state transition
 * @param delay_ms Requested delay before starting transition or
 * setting the immediate value, in milliseconds
 * @param request_flags Request flags
 * Return Value - none
 */

void request_cl_1(uint16_t model_id,
		uint16_t element_index,
		uint16_t client_addr,
		uint16_t server_addr,
		uint16_t appkey_index,
		const struct mesh_generic_request *req,
		uint32_t transition_ms,
		uint16_t delay_ms,
		uint8_t request_flags)
{

}

/*
 * Function Name - change_ser_1
 * Description - Server state change handler function for generic server model
 * @param model_id Model that generated the event
 * @param element_index Element where the model resides
 * @param current Current model state
 * @param target Target model state, towards which model is moving;
 * in case there is no state transition ongoing a NULL pointer will
 * be given
 * @param remaining_ms Time remaining for transition in milliseconds;
 * in case there is no state transition ongoing a value of zero will
 * be given
 * Return Value -none
 */

void change_ser_1(uint16_t model_id,
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

	sprintf(device_Value, "5823Sub%02x:%02x", ble_address-> addr[1], ble_address-> addr[0]);
	displayPrintf(DISPLAY_ROW_NAME, "%s",device_Value);

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

/*
 * Function Name - LPN_INIT()
 * Description - This function initializes the low power node.
 * Inputs - none
 * Return Value - none
 */
void LPN_INIT()
{
	LOG_INFO("Initializing the Low power functionality");

	BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_lpn_init());

	BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_lpn_configure(QUEUE_LEN, POLL_TIMEOUT));

	BTSTACK_CHECK_RESPONSE(gecko_cmd_mesh_lpn_establish_friendship(NET_KEY_INDEX));

	displayPrintf(DISPLAY_ROW_CONNECTION, "Low Power Node ON");

}

/*
 * Function Name - Models_INIT()
 * Description - This function provides Register handler functions for a client model.
 * Inputs - none
 * Return Value - none
 */

void Models_INIT()
{
	/*Register handler functions for a server model*/
	errorcode_t error_code;
	error_code=mesh_lib_generic_server_register_handler(MESH_GENERIC_ON_OFF_SERVER_MODEL_ID,0,request_cl,change_ser);
	LOG_INFO(" mesh_lib_generic_server_register_handler for Model no. 1:%d ",error_code);


	errorcode_t error_code_1;
	error_code_1=mesh_lib_generic_server_register_handler(MESH_GENERIC_LEVEL_SERVER_MODEL_ID,0,request_cl_1,change_ser_1);
	LOG_INFO(" mesh_lib_generic_server_register_handler for model no. 2:%d ",error_code_1);
}

/*
 * Function Name - Node_INIT()
 * Description - This function Initialize Mesh helper library.
 * Inputs - none
 * Return Value - none
 */

void Node_INIT()
{

	/*Initialize Mesh helper library*/
	mesh_lib_init(malloc,free,8);

	/*Initialize the Low Power Functionality*/
	LPN_INIT();
}

/*
 * Function Name - PD_Store()
 * Description - This function is used to store persistant data in PS key.
 * Inputs - uint16_t sensor key,uint16_t value to be stored
 * Return Value - none
 */

void PD_Store(uint16_t Sensor_key, uint16_t SensorVal)
{
	uint8_t* Sen_data;

	uint8_t  S_size = sizeof(SensorVal);

	Sen_data = &SensorVal;

	BTSTACK_CHECK_RESPONSE(gecko_cmd_flash_ps_save(Sensor_key, S_size,Sen_data));

	displayPrintf(DISPLAY_ROW_MAX_1,"");

	displayPrintf(DISPLAY_ROW_MAX_2,"");
}

/*
 * Function Name - PD_Load()
 * Description - This function is used to retrieve some value stored in persistant data in PS key.
 * Inputs - uint16_t sensor key
 * Return Value - none
 */

uint16_t PD_Load(uint16_t Sensor_key)
{
	uint16_t PD_Data;

	struct gecko_msg_flash_ps_load_rsp_t*instance=gecko_cmd_flash_ps_load(Sensor_key);

	memcpy(&PD_Data,&instance->value.data,instance->value.len);

	LOG_INFO("Stored Persistant data is %d",PD_Data);

	return PD_Data;
}

/*
 * Function Name - PD_Display()
 * Description - This function is used to display Persistant data by retrieve some value stored in persistant data in PS key.
 * Inputs - none
 * Return Value - none
 */

void PD_Display()
{
	uint16_t PD_LUX;
	uint16_t PD_PROXIMITY;

	PD_LUX = PD_Load(LIGHT_KEY);

	PD_PROXIMITY = PD_Load(PROXIMITY_KEY);

	displayPrintf(DISPLAY_ROW_MAX_1,"PD_LUX:%d",PD_LUX);

	displayPrintf(DISPLAY_ROW_MAX_2,"PD_PROX:%d",PD_PROXIMITY);

	LOG_INFO("PD_LUX:%d",PD_LUX);

	LOG_INFO("PD_PROX:%d",PD_PROXIMITY);
}

/*
 * Function Name - publish_light_data()
 * Description - This function is used to publish on_off model data to server.
 * Inputs - none
 * Return Value - none
 */

void publish_light_data()
{
	struct mesh_generic_request req;

	uint16_t publish_state_request;

	req.kind= mesh_generic_request_on_off;

	LOG_INFO("Light Value is %d",final_data);

	if (final_data>LUX_THRESHOLD)
	{
		LOG_INFO("Sending ON OFF Request 0x01");
		req.on_off=0x01;
	}
	else
	{
		LOG_INFO("Sending ON OFF Request 0x00");

		req.on_off=0x00;
	}
	publish_state_request = mesh_lib_generic_client_publish(MESH_GENERIC_ON_OFF_CLIENT_MODEL_ID,0,tran_param,&req,0,0,0);

	LOG_INFO("Response for mesh_lib_generic_client_publish : %d",publish_state_request);

	tran_param++;
}

/*
 * Function Name - publish_proximity_data
 * Description - This function is used to publish level model data to server.
 * Inputs - uint16_t data to be published
 * Return Value - none
 */

void publish_proximity_data(uint16_t proximity_distance)
{
	struct mesh_generic_state requ;

	uint16_t publish_level_request;

	requ.kind= mesh_generic_request_level;

	LOG_INFO("Publishing Proximity Distance");

	if (proximity_distance>PROX_THRESHOLD)
	{
		LOG_INFO("Sending level request 10");
		requ.level.level=10;
	}
	if(proximity_distance<PROX_THRESHOLD)
	{
		LOG_INFO("Sending level request 0");
		requ.level.level=1;
	}
	publish_level_request = mesh_lib_generic_client_publish(MESH_GENERIC_LEVEL_CLIENT_MODEL_ID,0,tran_param_1,&requ,0,0,0);

	LOG_INFO("Response for mesh_lib_generic_client_publish : %d",publish_level_request);

	tran_param_1++;
}
