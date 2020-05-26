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

bd_addr server_Addr = {.addr={0x60,0xf2,0xb5,0x57,0x0b,0x00}}; // set the server address here

float gattUint32ToFloat(const uint8_t *);
void ble_event_handler(struct gecko_cmd_packet*);
void ble_client(struct gecko_cmd_packet*);
bool gecko_update(struct gecko_cmd_packet*);

#endif /* SRC_BLE_HANDLER_H_ */
