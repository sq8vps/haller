#include "proto.h"
#include <stddef.h>

#define PROTO_MAX_ENTRIES 10

struct ProtoPacket
{
	uint8_t id;
	uint8_t size;
	uint8_t payload[];
} __attribute__ ((packed));

struct ProtoEntry
{
	uint8_t id;
	void (*callback)(void *buffer, uint8_t len);
}
static ProtoEntryTable[PROTO_MAX_ENTRIES];
static uint32_t ProtoEntryTableCount = 0;

int ProtoRegister(uint8_t id, void(*callback)(void *buffer, uint8_t len))
{
	if(ProtoEntryTableCount >= PROTO_MAX_ENTRIES)
		return -1;

	ProtoEntryTable[ProtoEntryTableCount].id = id;
	ProtoEntryTable[ProtoEntryTableCount].callback = callback;
	ProtoEntryTableCount++;
	return 0;
}

void ProtoParse(void *buffer, uint16_t len)
{
	if(len < sizeof(struct ProtoPacket))
		return;

	struct ProtoPacket *p = buffer;

	if(p->size > (len - sizeof(struct ProtoPacket)))
		return;

	for(uint32_t i = 0; i < ProtoEntryTableCount; i++)
	{
		if(ProtoEntryTable[i].id == p->id)
		{
			if(NULL != ProtoEntryTable[i].callback)
				ProtoEntryTable[i].callback(p->payload, p->size);
			return;
		}
	}
}
