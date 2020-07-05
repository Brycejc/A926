#ifndef __sadp_pu_api_h__
#define __sadp_pu_api_h__

#include "NET_DEV_STRUCT.H"
/*
 * �豸���Ͷ���
 */

enum _SADP_DEV_TYPE_E
 {
    /* --------- IPC --------------- */
      SADP_DEV_IPC_PRO       = 0x0001	// רҵ����
    , SADP_DEV_IPC_SHARK	 = 0x1001	// ҡͷ��
	, SADP_DEV_IPC_POCKET	 = 0x2001	// ��Ƭ��
	, SADP_DEV_IPDOME		 = 0x3001	// ������
    /* --------- DVR --------------- */
    , SADP_DEV_DVR_3531      = 0x4001    //DVR 3531 16-D1(960)
    , SADP_DEV_SDI_3531      = 0x4101    //DVR 3531 04,08, HD-SDI
    /* --------- NVR --------------- */
    , SADP_DEV_NVR_3531      = 0x5001    //NVR 3531 09, 16, 25, 32 720P/1080P
    , SADP_DEV_NVR_3520D     = 0x5101    //NVR 3520D 04, 09, 16 720P/1080P
    , SADP_DEV_NVR_3520D_NC  = 0x5102    //NVR 3520D 04, with NC
    , SADP_DEV_NVR_3521      = 0x5201    //NVR 3521 04, 09, 16 720P/1080P
    /* --------- DEC --------------- */
    , SADP_DEV_DECC_3531     = 0x6001    //DEC CARD 720P 8in, 2out;
    , SADP_DEV_DECB_3531     = 0x6101    //DEC BOX  720P 8in, 2out;
    /*-----------------------------------*/
};
typedef enum _SADP_DEV_TYPE_E SADP_DEV_TYPE_E;

/*
 * �鲥���ò���ID����
 */

enum _SADP_MSG_ID_E
{
      SADP_MSG_ID_DEV_INFO      = 0x0001     /*  �豸����  NET_DEV_SADPINFO   */
    , SADP_MSG_ID_NET_CFG                    /*  ��������  NET_DEV_MODIFY_NET */
};
typedef enum _SADP_MSG_ID_E SADP_MSG_ID_E;

/*-----------------------------------*/

#define NET_DEV_ETHCFG_HTON(ethcfg) do { \
    /* none */\
}while(0)

#define NET_DEV_ETHCFG_NTOH(ethcfg) do { \
    /* none */\
}while(0)

#define NET_DEV_WIFINIC_HTON(wifinic) do { \
    (wifinic)->EnableWifi = htonl((wifinic)->EnableWifi);\
}while(0)

#define NET_DEV_WIFINIC_NTOH(wifinic) do { \
    (wifinic)->EnableWifi = ntohl((wifinic)->EnableWifi);\
}while(0)

#define NET_DEV_SERVICECFG_HTON(servicecfg) do { \
    (servicecfg)->MsgPort  = htons((servicecfg)->MsgPort );\
    (servicecfg)->RtspPort = htons((servicecfg)->RtspPort);\
    (servicecfg)->HttpPort = htons((servicecfg)->HttpPort);\
    (servicecfg)->RtmpPort = htons((servicecfg)->RtmpPort);\
}while(0)

#define NET_DEV_SERVICECFG_NTOH(servicecfg) do { \
    (servicecfg)->MsgPort  = ntohs((servicecfg)->MsgPort );\
    (servicecfg)->RtspPort = ntohs((servicecfg)->RtspPort);\
    (servicecfg)->HttpPort = ntohs((servicecfg)->HttpPort);\
    (servicecfg)->RtmpPort = ntohs((servicecfg)->RtmpPort);\
}while(0)
/*-----------------------------------*/

/* NET_DEV_SADPINFO */
#define NET_DEV_SADPINFO_HTON(sadp) do { \
    (sadp)->ProtolType = htons((sadp)->ProtolType);\
    NET_DEV_ETHCFG_HTON(&(sadp)->EthCfg);\
    NET_DEV_WIFINIC_HTON(&(sadp)->WifiNIC);\
    NET_DEV_SERVICECFG_HTON(&(sadp)->ServiceCfg);\
}while(0)

#define NET_DEV_SADPINFO_NTOH(sadp) do { \
    (sadp)->ProtolType = ntohs((sadp)->ProtolType);\
    NET_DEV_ETHCFG_NTOH(&(sadp)->EthCfg);\
    NET_DEV_WIFINIC_NTOH(&(sadp)->WifiNIC);\
    NET_DEV_SERVICECFG_NTOH(&(sadp)->ServiceCfg);\
}while(0)

/*-----------------------------------*/

/* NET_DEV_MODIFY_NET */
#define NET_DEV_MODIFY_NET_HTON(modi) do { \
    (modi)->ProtolType = htons((modi)->ProtolType);\
    NET_DEV_ETHCFG_HTON(&(modi)->EthCfg);\
    NET_DEV_WIFINIC_HTON(&(modi)->WifiNIC);\
    NET_DEV_SERVICECFG_HTON(&(modi)->ServiceCfg);\
}while(0)

#define NET_DEV_MODIFY_NET_NTOH(modi) do { \
    (modi)->ProtolType = ntohs((modi)->ProtolType);\
    NET_DEV_ETHCFG_NTOH(&(modi)->EthCfg);\
    NET_DEV_WIFINIC_NTOH(&(modi)->WifiNIC);\
    NET_DEV_SERVICECFG_NTOH(&(modi)->ServiceCfg);\
}while(0)

/*-----------------------------------*/

/*
  *   SADP_MSG_SGET_E  ������cu������Ϣ�����û��ǻ�ȡ
  */
typedef enum _SADP_SGET_DIR{
      SADP_MSG_SET = 0X0001                  //����
    , SADP_MSG_GET                           //��ȡ
}SADP_MSG_SGET_E;

/*
 * sadp_pu_parm_t
 * pu  �˻ص�����
 * 
 */

typedef struct _sadp_pu_parm_s {
        int  id;                            //SADP_MSG_ID_E;       
        int  args;                          //SADP_MSG_SGET_E
        char user[NET_DEV_USERNAME_LEN];    //user name
        char pass[NET_DEV_PASSWORD_LEN];    //password
        char eth_name[NET_DEV_ETH_NAME_LEN];//eth_name;
        int  *size;                         //SADP_MSG_SET:����Ľṹ���С.    
                                            //SADP_MSG_GET:�ܴ�������󳤶�
        char *data;                         //SADP_MSG_SET:����Ľṹ���ַ
                                            //SADP_MSG_GET:�����ṹ����䵽�����ַ
}sadp_pu_parm_t;

/*
 * pu �˵Ļص��ṹ�壬���ڴ���cu��������Ϣ
 *
 * callback �ķ���ֵ:
                        �ɹ�:SADP_NOTIFY_ERR_0
                        ʧ��:SADP_NOTIFY_ERR_FAILED
 */

typedef struct _sadp_pu_notify_s {
    void *user_arg;
    int (*callback)(struct _sadp_pu_notify_s *handle
            , sadp_pu_parm_t *parm);
}sadp_pu_notify_t;

/*
 *  pu �˵ĳ�ʼ���ͷ���ʼ��
 *  addr ��������֪�ĵ�ַ, һ��Ĭ��Ϊ"0"����
 *  port ��������֪�Ķ˿�,  һ��Ĭ��Ϊ0  ����
 *  sadp_pu_notify_t �ص��ṹ���ַ
 */

int sadp_pu_init(char *addr
            , unsigned int port
            , sadp_pu_notify_t *msg_cb);

int sadp_pu_uninit(void);


#endif //__sadp_pu_api_h__
