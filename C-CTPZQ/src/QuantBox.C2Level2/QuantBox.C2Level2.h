// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 QUANTBOXC2CTP_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// QUANTBOXC2CTP_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。
#ifndef _QUANTBOXC2LEVEL2_H_
#define _QUANTBOXC2LEVEL2_H_

#ifdef QUANTBOXC2LEVEL2_EXPORTS
#define QUANTBOXC2LEVEL2_API __declspec(dllexport)
#else
#define QUANTBOXC2LEVEL2_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "include\Level2\ThostFtdcLevel2UserApiStruct.h"

_USING_LEVEL2_NS_


//用于分隔输入的合列表，与前置机地址列表，所以不能出现“:”一类的
#define _QUANTBOXC2LEVEL2_SEPS_ ",;"

//连接状态枚举
enum ConnectionStatus
{
	E_uninit,		//未初始化
	E_inited,		//已经初始化
	E_unconnected,	//连接已经断开
	E_connecting,	//连接中
	E_connected,	//连接成功
	E_authing,		//授权中
	E_authed,		//授权成功
	E_logining,		//登录中
	E_logined,		//登录成功
	E_confirming,	//结算单确认中
	E_confirmed,	//已经确认
	E_conn_max		//最大值
};

//回调函数类型定义（为编写方便，按字母排序）
typedef void (__stdcall *fnOnConnect)(void* pApi,CThostFtdcRspUserLoginField *pRspUserLogin,ConnectionStatus result);//连接后的结果状态
typedef void (__stdcall *fnOnDisconnect)(void* pApi,CThostFtdcRspInfoField *pRspInfo,ConnectionStatus step);//出错时所处的状态
typedef void (__stdcall *fnOnRspError)(void* pApi,CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void (__stdcall *fnOnRtnLevel2MarketData)(void* pLevel2Api,CThostFtdcLevel2MarketDataField *pLevel2MarketData);
typedef void (__stdcall *fnOnRtnNGTSIndex)(void* pLevel2Api,CThostFtdcNGTSIndexField *pNGTSIndex);

//创建接收消息队列，支持响应行情和交易
QUANTBOXC2LEVEL2_API void* __stdcall CTP_CreateMsgQueue();

//向消息队列注册回调函数
QUANTBOXC2LEVEL2_API void __stdcall CTP_RegOnConnect(void* pMsgQueue,fnOnConnect pCallback);
QUANTBOXC2LEVEL2_API void __stdcall CTP_RegOnDisconnect(void* pMsgQueue,fnOnDisconnect pCallback);
QUANTBOXC2LEVEL2_API void __stdcall CTP_RegOnRspError(void* pMsgQueue,fnOnRspError pCallback);
QUANTBOXC2LEVEL2_API void __stdcall CTP_RegOnRtnLevel2MarketData(void* pMsgQueue,fnOnRtnLevel2MarketData pCallback);
QUANTBOXC2LEVEL2_API void __stdcall CTP_RegOnRtnNGTSIndex(void* pMsgQueue,fnOnRtnNGTSIndex pCallback);

//释放消息队列
QUANTBOXC2LEVEL2_API void __stdcall CTP_ReleaseMsgQueue(void* pMsgQueue);
//清空消息队列内容
QUANTBOXC2LEVEL2_API void __stdcall CTP_ClearMsgQueue(void* pMsgQueue);

//处理消息队列第一条记录
QUANTBOXC2LEVEL2_API bool __stdcall CTP_ProcessMsgQueue(void* pMsgQueue);
//开启队列回调主动推送线程
QUANTBOXC2LEVEL2_API void __stdcall CTP_StartMsgQueue(void* pMsgQueue);
//停止队列回调主动推送线程
QUANTBOXC2LEVEL2_API void __stdcall CTP_StopMsgQueue(void* pMsgQueue);

//行情接口=======================================

//创建行情接口
QUANTBOXC2LEVEL2_API void* __stdcall L2_CreateLevel2Api();
//将消息队列注册到行情接口上
QUANTBOXC2LEVEL2_API void __stdcall L2_RegMsgQueue2Level2Api(void* pLevel2Api,void* pMsgQueue);
//连接
QUANTBOXC2LEVEL2_API void __stdcall L2_Connect(
	void* pLevel2Api,
	const char* szPath,
	const char* szAddresses,
	const char* szBrokerId,
	const char* szInvestorId,
	const char* szPassword);

//订阅合约，多个合约以“,”分隔
QUANTBOXC2LEVEL2_API void __stdcall L2_SubscribeLevel2MarketData(void* pLevel2Api,const char* szInstrumentIDs,const char* szExchageID);
QUANTBOXC2LEVEL2_API void __stdcall L2_SubscribeNGTSIndex(void* pLevel2Api,const char* szInstrumentIDs,const char* szExchageID);
//取消订阅，多个合约以“,”分隔
QUANTBOXC2LEVEL2_API void __stdcall L2_UnSubscribeLevel2MarketData(void* pLevel2Api,const char* szInstrumentIDs,const char* szExchageID);
QUANTBOXC2LEVEL2_API void __stdcall L2_UnSubscribeNGTSIndex(void* pLevel2Api,const char* szInstrumentIDs,const char* szExchageID);
//断开连接
QUANTBOXC2LEVEL2_API void __stdcall L2_Disconnect(void* pLevel2Api);
//释放行情接口
QUANTBOXC2LEVEL2_API void __stdcall L2_ReleaseLevel2Api(void* pLevel2Api);


#ifdef __cplusplus
}
#endif


#endif//end of _QUANTBOXC2CTP_H_