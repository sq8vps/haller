#ifndef INC_LAN_H_
#define INC_LAN_H_

#include <stdint.h>

#define UDP_PORT_LOCAL 25562
#define UDP_PORT_REMOTE 25563
#define TCP_PORT_LOCAL 25564
#define TCP_PORT_REMOTE 25565
#define REMOTE_IP_1 10
#define REMOTE_IP_2 69
#define REMOTE_IP_3 69
#define REMOTE_IP_4 70

typedef enum
{
	LAN_OK,
	LAN_NOT_CONNECTED,
	LAN_LL_ERR,
	LAN_PCB_NOT_INITIALIZED,

} Lan_err_t;

/**
 * @brief Send TCP packet
 * @param *buf Data to send
 * @param len Data length
 * @return LAN_OK or error code
 */
Lan_err_t Lan_sendTcp(uint8_t *buf, uint16_t len);

/**
 * @brief Send UDP packet
 * @param *buf Data to send
 * @param len Data length
 * @return LAN_OK or error code
 */
Lan_err_t Lan_sendUdp(uint8_t *buf, uint16_t len);

/**
 * @brief Initialize LAN module
 * @param *rxCallbackFun Callback function for received data
 */
void Lan_init(void (*rxCallbackFun)(uint8_t*, uint16_t*));

/**
 * @brief Refresh LAN timeouts
 * @attention Must be polled in main loop
 */
void Lan_refresh(void);

#endif /* INC_LAN_H_ */
