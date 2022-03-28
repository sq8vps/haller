#ifndef INC_PROTO_H_
#define INC_PROTO_H_

#include <stdint.h>
#include "CommunicationCodes.h"

/**
 * @brief Parse incoming packet and pass data to appropriate module handler
 * @param *buf Packet body
 * @param *len Packet length
 */
void Proto_parse(uint8_t *buf, uint16_t *len);


/**
 * @brief Send packet
 * @param id Module ID
 * @param data Packet body
 * @param len Packet length (number of WORDS!)
 */
void Proto_send(enum Command id, uint8_t *data, uint16_t len);

#endif /* INC_PROTO_H_ */
