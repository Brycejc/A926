#ifndef _GSF_NET_API_H
#define _GSF_NET_API_H

#include <sys/time.h>
#include <sys/socket.h>

/*
* Copyright (c) 2010
* All rights reserved.
* 
* filename:gsf_net_api.h
* 
* func:���ļ�ʵ����linux�� socket��װ
* 
* author : gsf
* createdate: 2010-10-10
*
* modify list: 
* 
*/

#ifdef __cplusplus
extern "C" {
#endif

#define LISTENQ 1024
#define GSF_MAX_BLOCK_SEND_TIME 3
#define GSF_MAX_BLOCK_RECV_TIME 3

#define GSF_DEFAULT_RECV_TIMEOUT 500
#define GSF_DEFAULT_SEND_TIMEOUT 1000

#define GSF_DEFAULT_LOCAL_TIME	2

#define GSF_RETURN_FAIL -1
#define GSF_RETURN_OK    0

typedef enum _GSF_SOCK_TYPE_
{
    GSF_TCP_SOCK         = 1,
    GSF_UDP_SOCK        ,
    GSF_LOCAL_TCP_SOCK  ,
    GSF_LOCAL_UDP_SOCK  ,
}GSF_SOCK_TYPE;
	
//////////////////////////////////////////////////////////////////////////
//
//socket ��������
//
//////////////////////////////////////////////////////////////////////////

//=================================================================
//����: gsf_create_sock 
//
//����:����һ��socket
//
//iType :������socket������ 1:tcp 2:udp 3:����ͨѶtcp sock
//		  4.����ͨѶudp sock ʹ��af_unixЭ����
//
//����ֵ���ɹ�����socket ,������GSF_RETURN_FAIL
//==================================================================
int gsf_create_sock(GSF_SOCK_TYPE iType);

//=================================================================
//����: gsf_close_socket 
//
//����:�ر�ָ����socket
//
//GSFd : ��Ҫ�رյ�socket 	
//����ֵ����ʱ����GSF_RETURN_OK,������GSF_RETURN_FAIL
//=================================================================
int gsf_close_socket(int *fd);

//=================================================================
//����: gsf_get_sock_ip 
//
//����: ȡ��socket��ip
//
//sock:�׽���
//
//����ֵ���ɹ�����ip ,������GSF_RETURN_FAIL
//=================================================================
int gsf_get_sock_ip(int sock);

//=================================================================
//����: gsf_get_sock_port 
//
//����:ȡ��socket��port
//
//sock:�׽���
//
//����ֵ���ɹ�����port ,������GSF_RETURN_FAIL
//=================================================================
int gsf_get_sock_port(int sock);

//=================================================================
//����: gsf_tcp_noblock_connect 
//
//����:tcp��������ʽ���ӷ����
//
//����:
//localHost:���ض˵���������ip��ַ
//localServ:���ض˵ķ������ƻ����Ƕ˿�
//
//dstHost: ����˵���������ip��ַ
//dstServ: ����˵ķ������ƻ����Ƕ˿�
//timeout_ms: ���ӷ�������ʱʱ��(��λ����)
//
//����ֵ�����ӳɹ���������ʹ�õ�socket, ���򷵻�GSF_RETURN_FAIL
//=================================================================	
int gsf_tcp_noblock_connect(const char *localHost, const char *localServ, 
					const char *dstHost, const char *dstServ, int timeout_ms);

//=================================================================
//����: gsf_tcp_noblock_recv 
//
//����:tcp������socket���պ���
//
//����:
//  sockfd:���յ�socket
//  rcvBuf:���յĻ�����
//  bufSize:���յĻ�������С
//  rcvSize:�ƶ����յ��ֽ���, ��0��ָ�����յ��ֽ���
//  timeOut:���ճ�ʱʱ��(��λ����)		
//
//����ֵ:�ɹ����ؽ��յ����ֽ���, ���򷵻�GSF_RETURN_FAIL
//=================================================================
int gsf_tcp_noblock_recv(int sockfd, unsigned char *rcvBuf, int bufSize, int rcvSize, 
						int timeOut);
//=================================================================
//����: gsf_tcp_noblock_send 
//
//����:tcp��������ʽ����
//
//����:
// hSock:��Ҫ�������ݵ�socket
// pbuf:��Ҫ���͵�����
// size: ��Ҫ���͵����ݵ��ֽ���
// pBlock: �ж���·�ǲ��ǶϿ� (��������timeout����β��ɹ�����Ϊ����Ͽ�)
// timeOut: ���ͳ�ʱʱ��(��λ����)
//
//����ֵ:�ɹ����ط��͵��ַ���, ���򷵻�GSF_RETURN_FAIL
//=================================================================
int gsf_tcp_noblock_send(int hSock, unsigned char *pbuf, int size, int *pBlock, int timeOut);

//=================================================================
//����: gsf_tcp_noblock_mtu_send 
//
//����:tcp��������ʽ����ÿ�η���mtu�ֽ�����
//
//����:
// hSock:��Ҫ�������ݵ�socket
// pbuf: ��Ҫ���͵�����
// size: ��Ҫ���͵����ݵ��ֽ���
// mtu:  ÿ�η��͵�����ֽ���
//����ֵ:�ɹ����ط��͵��ַ���, ���򷵻�GSF_RETURN_FAIL
//=================================================================
int gsf_tcp_noblock_mtu_send(int hSock, unsigned char *pbuf,int size, int mtu);

//=================================================================
//����: gsf_select 
//
//����:����fd_array�����е�socket
//
//fd_array: socket ����
//fd_num:   socket �ĸ���
//fd_type:  bit 1 Ϊ 1 �������socket �Ƿ�ɶ�
//	 	  bit 2 Ϊ 1 �������socket �Ƿ��д
//		  bit 3 Ϊ 1 �������socket �Ƿ����쳣
//	
//����ֵ����ʱ����GSF_RETURN_OK,������GSF_RETURN_FAIL������
//		����пɶ��ķ��ظ�socketֵ|0x10000
//		�п�д���ظ�socketֵ|0x20000
//		���쳣���ظ�socketֵ|0x40000
//=================================================================
int gsf_select(int *fd_array, int fd_num, int fd_type, int time_out);

//=================================================================
//����: gsf_ip_n2a 
//
//����:����IP ת�� ���ʮ���� IP
//
//ip    ����: ����IP(������)
//ourIp ���: ���ʮ���� IP����
//len   ����: ���ʮ���� IP�����С
//	
//����ֵ:�ɹ�����GSF_RETURN_OK,���򷵻�GSF_RETURN_FAIL
//=================================================================	
unsigned long gsf_ip_n2a(unsigned long ip, char *ourIp, int len);

//=================================================================
//����: gsf_ip_a2n 
//
//����:���ʮ���� IP(��������) ת�� ����IP  
//
//ip    ����: ���ʮ���� IP����
//	
//����ֵ:�ɹ���������IP(������),���򷵻�GSF_RETURN_FAIL
//=================================================================	
unsigned long gsf_ip_a2n(char *szIp);

//=================================================================
//����: gsf_Ip_N2A 
//
//����:gsf_itoa ��windows ��itoaһ�� ����ת���ַ���
//
//n:��ֵ
//
//s:�ִ�
//
//����ֵ:��
//=================================================================
void   gsf_itoa(int   n,   char   s[]);

/*
SO_LINGER

   ��ѡ��ָ������close���������ӵ�Э����β�������TCP����
   ȱʡclose�������������أ���������ݲ������׽ӿڻ�������
   ��ϵͳ�����Ž���Щ���ݷ��͸��Է���
SO_LINGERѡ�������ı��ȱʡ���á�ʹ�����½ṹ��
struct linger {
     int l_onoff; // 0 = off, nozero = on 
     int l_linger; // linger time 
};

���������������

l_onoffΪ0�����ѡ��رգ�l_linger��ֵ�����ԣ�����ȱʡ�����close�������أ� 

l_onoffΪ��0��l_lingerΪ0�����׽ӿڹر�ʱTCPز�����ӣ�
TCP�������������׽ӿڷ��ͻ������е��κ����ݲ�����һ��RST���Է���
������ͨ�����ķ�����ֹ���У��������TIME_WAIT״̬�� 

l_onoff Ϊ��0��l_lingerΪ��0�����׽ӿڹر�ʱ�ں˽�����һ��ʱ�䣨��l_linger��������
����׽ӿڻ��������Բ������ݣ����̽�����˯��״̬��
ֱ ����a���������ݷ������ұ��Է�ȷ�ϣ�
֮�������������ֹ���У������ַ��ʼ���Ϊ0����b���ӳ�ʱ�䵽��
��������£�Ӧ�ó�����close�ķ���ֵ�Ƿǳ���Ҫ�ģ�
��������ݷ����겢��ȷ��ǰʱ�䵽��
close������EWOULDBLOCK�������׽ӿڷ��ͻ������е��κ����ݶ���ʧ��
close�ĳɹ����ؽ��������Ƿ��͵����ݣ���FIN�����ɶԷ�TCPȷ�ϣ�
�������ܸ������ǶԷ�Ӧ�ý����Ƿ��Ѷ������ݡ�
����׽ӿ���Ϊ�������ģ��������ȴ�close�� �ɡ� 


l_linger�ĵ�λ������ʵ�֣�4.4BSD�����䵥λ��ʱ�ӵδ𣨰ٷ�֮һ�룩��
��Posix.1g�涨��λΪ�롣

*/

unsigned short cal_chksum(unsigned short *addr, int len);


#ifdef __cplusplus
}
#endif 

#endif

