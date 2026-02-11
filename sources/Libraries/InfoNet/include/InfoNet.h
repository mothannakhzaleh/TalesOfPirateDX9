
/* * * * * * * * * * * * * * * * * * * *
*
*      信息服务网络消息解析
*                  by Jampe
*
* * * * * * * * * * * * * * * * * * * */


#if !defined __INFONET_H__
#define __INFONET_H__


#include <time.h>


#define     INFO_DEFAULT                0       //  信息返回默认
#define     INFO_SUCCESS                1       //  信息返回成功
#define     INFO_FAILED                 2       //  信息返回失败

#define     INFO_LOGIN                  1000    //  登陆请求
#define     INFO_REQUEST_ACCOUNT        1010    //  获取帐号信息
#define     INFO_EXCEPTION_SERVICE      1900    //  拒绝服务
#define     INFO_REQUEST_STORE          2001    //  获取商城列表
#define     INFO_REQUEST_AFFICHE        2002    //  获取公告列表
#define     INFO_STORE_BUY              2011    //  购买道具
#define     INFO_EXCHANGE_MONEY         2012    //  兑换代币
#define     INFO_REGISTER_VIP           2013    //  购买VIP
#define     INFO_REQUEST_HISTORY        2021    //  获取交易记录
#define     INFO_SND_GM_MAIL            4500    //  发送GM邮件
#define     INFO_RCV_GM_MAIL            4501    //  收取GM回复


#define     INFO_MAN_DEFAULT            INFO_DEFAULT    //  管理默认值
#define     INFO_MAN_SUCCESS            10              //  管理消息返回成功
#define     INFO_MAN_FAILED             11              //  管理消息返回失败

#define     INFO_MAN_ADD                0               //  管理添加
#define     INFO_MAN_MODIFY             1               //  管理修改
#define     INFO_MAN_DEL                2               //  管理删除

#define     INFO_MAN_ITEM               6000            //  商品管理
#define     INFO_MAN_CLASS              6001            //  分类管理
#define     INFO_MAN_AFFICHE            6002            //  公告管理
#define     INFO_MAN_ALL_ITEM           6003            //  管理所有未上架的商品
#define     INFO_GET_GM_MAIL            6500            //  收取GM邮件
#define     INFO_RPY_GM_MAIL            6501            //  回复GM邮件
#define     INFO_MAIL_QUEUE             6502            //  邮件列表数

#define     INFO_MAN_VIP                7000            //  VIP管理
#define     INFO_MAN_RATE               7001            //  修改汇率



#define     INFO_TYPE_HISTORY           0               //  交易记录
#define     INFO_TYPE_EXCHANGE          1               //  兑换记录
#define     INFO_TYPE_VIP               2               //  VIP购买记录

#define     INFO_CHECK_LIVE             0               //  检测在线

typedef unsigned char       _byte;


//  帐户信息
typedef struct _RoleInfo_
{
    long        actID;          //	帐号编号
    char        actName[21];    //	帐号名称
    long        chaID;          //	角色编号
    char        chaName[17];    //	角色名称
    long        moBean;         //	摩豆数量
    long        rplMoney;       //	代币数量
    long        vip;            //  VIP种类(20-50)，0为非vip
}RoleInfo, *pRoleInfo;



//  交易记录
typedef struct _HistoryInfo_
{
    time_t      tradeTime;      //	交易时间
    long        comID;          //	商品编号
    char        comName[21];    //	商品名称
    int         tradeResult;    //  结果        0 成功，1 错误的商品价格, 2 余额不足, 3 扣费失败
    long        tradeMoney;     //	交易金额
}HistoryInfo, *pHistoryInfo;



//  公告信息
typedef struct _AfficheInfo_
{
    long        affiID;         //	公告编号
    char        affiTitle[64];  //	公告标题
    char        comID[33];      //	连接商品编号
    time_t      affiTime;       //  发布时间
}AfficheInfo, *pAfficheInfo;



//  分类信息
typedef struct _ClassInfo_
{
    short        clsID;        //	分类编号
    char         clsName[17];   //	分类名称
    short        parentID;      //	父分类编号
}ClassInfo, *pClassInfo;



//  道具信息
typedef struct _ItemInfo_
{
    short       itemID;         //	道具编号
    _byte       itemNum;        //	道具个数
    _byte       itemFlute;      //	凹槽数
    short       itemAttrID[5];  //	五个属性编号
    short       itemAttrVal[5]; //	五个属性值
}ItemInfo, *pItemInfo;



//  商品信息
typedef struct _StoreInfo_
{
    long        comID;          //	商品编号
    char        comName[21];    //	商品名称
    long        comClass;       //	商品分类
    time_t      comTime;        //	商品上架时间
    long        comPrice;       //	价格
    char        comRemark[129]; //	商品备注
    int         comNumber;      //  商品剩余个数。-1不限制，0下架
    time_t      comExpire;      //  商品到期时间。
    _byte       itemNum;        //  道具个数
    _byte       isHot;          //  是否热卖
    time_t      beginTime;      //  上架时间
}StoreInfo, *pStoreInfo;



//  商品结构
typedef struct _StoreStruct_
{
    StoreInfo   store_head;     //  商品信息头
    pItemInfo   item_body;      //  道具信息体
}StoreStruct, *pStoreStruct;



//  登陆信息
typedef struct _LoginInfo_
{
    _byte       id;             //  服务器编号
    char        ip[16];         //  登陆IP
    char        pwd[33];        //  验证码
    _byte       ban;            //  是否黑名单
}LoginInfo, *pLoginInfo;


//  VIP信息
typedef struct _VIPInfo_
{
    int         vipID;          //  VIP编号(类型，范围在20-50之间)
    int         vipPrice;       //  VIP价格
}VIPInfo, *pVIPInfo;


//  Mail信息
typedef struct _MailInfo_
{
    long id;                    //  邮件编号
    char title[33];				//	邮件标题
    char description[513];		//	邮件正文
    long actID;				    //	发送者帐号ID
    char actName[64];			//	发送者帐号名称
    long chaID;				    //	发送者角色ID
    char chaName[64];			//	发送者角色名称
    time_t sendTime;			//	发送时间
}MailInfo, *pMailInfo;


//  消息头
typedef struct _NetMessageHead_
{
    unsigned char msgChk[4];    //  有效网络消息标识码为 0x4A, 0x4D, 0x50, 0x53
    long msgID;                 //  消息编号
    long subID;                 //  消息子编号
    long long msgOrder;         //  消息握手
    long msgSection;            //  小区编号
    long msgExtend;             //  扩展字段
    long msgSize;               //  消息体长度
    long msgEncSize;            //  加密后长度
}NetMessageHead, *pNetMessageHead;



//  网络消息
typedef struct _NetMessage_
{
    NetMessageHead      msgHead;    //  消息头
    void*               msgBody;    //  消息体
}NetMessage, *pNetMessage;


#define PNM_SUCCESS             0           //  解释成功
#define PNM_FAILED              1           //  错误的协议
#define PNM_UNKNOWN             2           //  未知协议
#define PNM_EXCEPTION           3           //  异常


//  获取消息头
extern int GetMessageHead(pNetMessageHead mh, unsigned char* msg, int size);

//  解析消息
extern int PeekNetMessage(pNetMessage* nm, unsigned char** buf, long& bufsize, void** tmpbuf, long& tmpsize);

//  释放消息
extern void FreeNetMessage(pNetMessage msg);

//  生成消息
extern bool BuildNetMessage(pNetMessage nm, long msgID, long subID, long extend, long section, unsigned char* body, long size);

//  获取密钥
extern _byte* GetInfoKey();

//  获取密码(尽限GameServer使用)
extern _byte* GetPassword();

class InfoThread;
//  客户端接口
class InfoNetBase
{
public:
    InfoNetBase();
    virtual ~InfoNetBase();
    virtual bool RunInfoService(const char* ip, short port);  //  运行客户端服务
    virtual bool StopInfoService(); //  停止客户端服务
    virtual bool PostInfoSend(long msgID, long subID, long extend, long section, unsigned char* body, long size, bool resnd = false);   //  发送消息
    virtual bool PostInfoSend(pNetMessage msg, bool resnd = false);     //  发送消息
    virtual bool IsConnect();

public:
    virtual void OnConnect(bool result) = 0;            //  连接时响应
    virtual void OnNetMessage(pNetMessage msg) = 0;     //  用户接口协议，msg由用户手工释放。
    virtual void OnResend(pNetMessage msg) = 0;         //  重发完成后，自动释放msg。
    virtual void OnDisconnect() = 0;        //  断开时响应

protected:
    virtual bool Connect();
    virtual bool Disconnect();      //  断开
    virtual bool PostInfoRecv();    //  投递接收
    friend class InfoThread;

protected:
    void*       m_client;
    void*       m_list;
    int         m_state;
    char        m_ip[260];
    short       m_port;
};

#ifndef __INFO_NET__
    #pragma comment(lib, "InfoNet.lib")
#endif

#endif
