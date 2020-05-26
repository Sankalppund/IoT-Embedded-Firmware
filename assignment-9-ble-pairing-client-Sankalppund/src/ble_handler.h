/*
 * ble_handler.h
 *
 *  Created on: Oct 18, 2019
 *      Author: sanka
 */

#ifndef SRC_BLE_HANDLER_H_
#define SRC_BLE_HANDLER_H_

#define PHYS (5)               // 5: 1M PHY and Coded PHY (Ref: https://docs.silabs.com/bluetooth/latest/le-gap)
#define SCAN_INTERVAL (0x10)   // Default value: 10 ms (Ref: https://docs.silabs.com/bluetooth/latest/le-gap)
#define SCAN_WINDOW (0x10)     // Default value: 10 ms (Ref: https://docs.silabs.com/bluetooth/latest/le-gap)
#define SCAN_TYPE (1)          // 0: Passive Scanning & 1: Active Scanning
#define INITIATING_PHY (0x01)
#define UUID_LEN (2)
#define UUID_LENGTH (16)          // for client MITM Implementation, to access service and charaxcteristics.
#define INTERVAL_MIN (400)
#define INTERVAL_MAX (400)
#define DURATION (0)
#define MAX_EVENTS (0)
#define MIN_INTERVAL (60)
#define MAX_INTERVAL (60)
#define LATENCY (3)
#define TIMEOUT (900)
#define MIN_CE_LENGTH (0)
#define MAX_CE_LENGTH (0)
#define CONFIRM (1)
#define FLAG (0x0F)
#define BTN_FLAG (0x00)
#define VALUE_LENGTH (1)
#define BONDABLE_VALUE (1)
#define HALT_VALUE (1)
#define POWER_VALUE (0)
#define RESUME_VALUE (0)

//bd_addr server_Addr = {.addr={0x60,0xf2,0xb5,0x57,0x0b,0x00}}; // set the server address here

bd_addr server_Addr = {.addr={0x43,0x62,0x08,0x6f,0x0d,0x00}}; // set the server address here

//bd_addr server_Addr = {.addr={0x6e,0xf2,0xb5,0x57,0x0b,0x00}}; // set the server address here


float gattUint32ToFloat(const uint8_t *);
void ble_event_handler(struct gecko_cmd_packet*);
void ble_client(struct gecko_cmd_packet*);
bool gecko_update(struct gecko_cmd_packet*);
void PB0_state();

#endif /* SRC_BLE_HANDLER_H_ */
