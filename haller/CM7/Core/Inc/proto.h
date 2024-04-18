#ifndef INC_PROTO_H_
#define INC_PROTO_H_

#include <stdint.h>

int ProtoRegister(uint8_t id, void(*callback)(void *buffer, uint8_t len));

void ProtoParse(void *buffer, uint16_t len);

#endif /* INC_PROTO_H_ */
