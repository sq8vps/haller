#include "lan.h"
#include "tcp.h"
#include "udp.h"
#include "stm32h7xx_hal.h"
#include "lwip.h"
#include <string.h>

#define TCP_CONN_TIMEOUT 3000 //TCP connection retry timeout in ms

struct udp_pcb *udp = NULL;
struct tcp_pcb *tcp = NULL;
ip_addr_t remoteIp;

struct Lan_state_s
{
	uint8_t tcpConnected : 1;
	uint8_t tcpConnWaiting : 1;
	uint8_t tcpTxInProgress : 1;
	uint8_t tcpTxFailed : 1;
	uint8_t tcpTxSuccessful : 1;
	uint32_t tcpConnTimeout;
};

struct Lan_state_s Lan_state;

static void (*rxCallback)(uint8_t *buf, uint16_t *len) = NULL;

static void lan_tcpInit(void);

static err_t lan_tcpReceived(void *arg, struct tcp_pcb *tpcb, struct pbuf *p, err_t err)
{
	if(rxCallback != NULL)
		rxCallback((uint8_t*)p->payload, &(p->len));
	tcp_recved(tpcb, p->len);
	return ERR_OK;
}

static void lan_udpReceived(void *arg, struct udp_pcb *pcb, struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
	if(rxCallback != NULL)
		rxCallback((uint8_t*)p->payload, &(p->len));
	pbuf_free(p);
}

static err_t lan_tcpConnected(void *arg, struct tcp_pcb *tpcb, err_t err)
{
	Lan_state.tcpConnected = 1;
	Lan_state.tcpConnWaiting = 0;
	Lan_state.tcpConnTimeout = 0;
	return ERR_OK;
}

static void lan_tcpError(void *arg, err_t err)
{
	if(err != ERR_OK) //restart connection on every error
	{
		Lan_state.tcpConnWaiting = 0;
		Lan_state.tcpConnected = 0;
		lan_tcpInit();
	}
}

static err_t lan_tcpSent(void *arg, struct tcp_pcb *tpcb, u16_t len)
{
	Lan_state.tcpTxInProgress = 0;
	Lan_state.tcpTxSuccessful = 1;
	return ERR_OK;
}

static void lan_tcpInit(void)
{
	//if(tcp != NULL)
	//	return;

	tcp = tcp_new();
	if(tcp == NULL)
	{
		/**
		 * Fatal error, cannot allocate TCP structure
		 * TODO: should we force the reset?
		 **/
		NVIC_SystemReset();
		while(1);;
	}

	tcp_bind(tcp, IP_ANY_TYPE, TCP_PORT_LOCAL); //bind to local port
	tcp_recv(tcp, &lan_tcpReceived); //set callbacks
	tcp_err(tcp, &lan_tcpError);
	tcp_sent(tcp, &lan_tcpSent);
}

static void lan_udpInit(void)
{
	//if(udp != NULL)
	//	return;

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
	udp_recv(udp, &lan_udpReceived, 0); //set callback
}

static void lan_tcpConnect(void)
{
	if(tcp == NULL)
		lan_tcpInit();

	Lan_state.tcpConnected = 0;
	Lan_state.tcpConnWaiting = 1;
	tcp_connect(tcp, &remoteIp, TCP_PORT_REMOTE, &lan_tcpConnected);
}



Lan_err_t Lan_sendTcp(uint8_t *buf, uint16_t len)
{
	if(tcp == NULL)
		return LAN_PCB_NOT_INITIALIZED;

	if(Lan_state.tcpConnected == 0)
		return LAN_NOT_CONNECTED;

	if(len == 0)
		return LAN_OK;


	if(tcp_write(tcp, buf, len, TCP_WRITE_FLAG_COPY) != ERR_OK)
		return LAN_LL_ERR;

	if(tcp_output(tcp) != ERR_OK)
			return LAN_LL_ERR;

	Lan_state.tcpTxFailed = 0;
	Lan_state.tcpTxSuccessful = 0;
	Lan_state.tcpTxInProgress = 1;

	return LAN_OK;
}

Lan_err_t Lan_sendUdp(uint8_t *buf, uint16_t len)
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

void Lan_init(void (*rxCallbackFun)(uint8_t*, uint16_t*))
{
	memset(&Lan_state, 0, sizeof(Lan_state));
	rxCallback = rxCallbackFun;
	IP4_ADDR(&remoteIp, REMOTE_IP_1, REMOTE_IP_2, REMOTE_IP_3, REMOTE_IP_4); //prepare remote IP

	MX_LWIP_Refresh();

	lan_udpInit(); //initialize UDP
	//lan_tcpInit(); //initialize TCP
}
