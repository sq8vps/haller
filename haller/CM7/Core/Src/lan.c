#include "lan.h"
#include "tcp.h"
#include "udp.h"
#include "stm32h7xx_hal.h"
#include "lwip.h"
#include <string.h>

static struct udp_pcb *udp = NULL;
static ip_addr_t remoteIp;

static void (*LanRxCallback)(void *buf, uint16_t len) = NULL;

static void LanUdpRxCallback(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	if(LanRxCallback != NULL)
		LanRxCallback(p->payload, p->len);
	pbuf_free(p);
}

static void LanUdpInit(void)
{

	udp = udp_new(); //initialize UDP structure
	if(udp == NULL)
	{
		/**
		 * Fatal error, cannot allocate UDP structure
		 * TODO: should we force the reset?
		 **/
		NVIC_SystemReset();
		while(1);;
	}


	udp_bind(udp, IP_ANY_TYPE, UDP_PORT_LOCAL); //bind to local port
	udp_recv(udp, &LanUdpRxCallback, 0); //set callback
}

LanError LanSendUdp(void *buf, uint16_t len)
{
	if(udp == NULL)
		return LAN_PCB_NOT_INITIALIZED;

	struct pbuf *b = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_RAM);
	memcpy(b->payload, buf, len);
	if(udp_sendto(udp, b, &remoteIp, UDP_PORT_REMOTE) != ERR_OK)
	{
		pbuf_free(b);
		return LAN_LL_ERR;
	}
	pbuf_free(b);
	return LAN_OK;
}

void LanInit(void (*rxCallback)(void*, uint16_t))
{
	LanRxCallback = rxCallback;
	IP4_ADDR(&remoteIp, REMOTE_IP_1, REMOTE_IP_2, REMOTE_IP_3, REMOTE_IP_4); //prepare remote IP

	LanUdpInit(); //initialize UDP
}
