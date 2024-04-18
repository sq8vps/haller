#ifndef INC_LAN_H_
#define INC_LAN_H_

#include <stdint.h>

#define UDP_PORT_LOCAL 25565
#define UDP_PORT_REMOTE 25565

#define REMOTE_IP_1 192
#define REMOTE_IP_2 168
#define REMOTE_IP_3 69
#define REMOTE_IP_4 1

typedef enum
{
	LAN_OK = 0,
	LAN_LL_ERR,
	LAN_PCB_NOT_INITIALIZED,
} LanError;

LanError LanSendUdp(void *buf, uint16_t len);

void LanInit(void (*rxCallback)(void *buf, uint16_t size));

#endif /* INC_LAN_H_ */
