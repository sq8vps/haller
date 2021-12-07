#ifndef INC_PROTO_H_
#define INC_PROTO_H_

#include <stdint.h>

typedef enum
{
	MODULE_NONE = 0x00,
	MODULE_STEVAL = 0x02,
} Module_id;

/**
 * @brief Parse incoming packet and pass data to appropriate module handler
 * @param *buf Packet body
 * @param *len Packet length
 */
void Proto_parse(uint8_t *buf, uint16_t *len);

#endif /* INC_PROTO_H_ */
