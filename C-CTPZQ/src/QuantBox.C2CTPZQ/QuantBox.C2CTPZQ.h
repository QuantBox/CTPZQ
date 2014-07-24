// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� QUANTBOXC2CTP_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// QUANTBOXC2CTP_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�
#ifndef _QUANTBOXC2CTPZQ_H_
#define _QUANTBOXC2CTPZQ_H_

#ifdef QUANTBOXC2CTPZQ_EXPORTS
#define QUANTBOXC2CTPZQ_API __declspec(dllexport)
#else
#define QUANTBOXC2CTPZQ_API __declspec(dllimport)
#endif

#ifdef __cplusplus
extern "C" {
#endif

#include "include\CTPZQ\ThostFtdcUserApiStructSSE.h"


//���ڷָ�����ĺ��б���ǰ�û���ַ�б����Բ��ܳ��֡�:��һ���
#define _QUANTBOXC2CTPZQ_SEPS_ ",;"

//����״̬ö��
enum ConnectionStatus
{
	E_uninit,		//δ��ʼ��
	E_inited,		//�Ѿ���ʼ��
	E_unconnected,	//�����Ѿ��Ͽ�
	E_connecting,	//������
	E_connected,	//���ӳɹ�
	E_authing,		//��Ȩ��
	E_authed,		//��Ȩ�ɹ�
	E_logining,		//��¼��
	E_logined,		//��¼�ɹ�
	E_confirming,	//���㵥ȷ����
	E_confirmed,	//�Ѿ�ȷ��
	E_conn_max		//���ֵ
};

//�ص��������Ͷ��壨Ϊ��д���㣬����ĸ����
typedef void (__stdcall *fnOnConnect)(void* pApi,CZQThostFtdcRspUserLoginField *pRspUserLogin,ConnectionStatus result);//���Ӻ�Ľ��״̬
typedef void (__stdcall *fnOnDisconnect)(void* pApi,CZQThostFtdcRspInfoField *pRspInfo,ConnectionStatus step);//����ʱ������״̬
typedef void (__stdcall *fnOnErrRtnOrderAction)(void* pTraderApi,CZQThostFtdcOrderActionField *pOrderAction, CZQThostFtdcRspInfoField *pRspInfo);
typedef void (__stdcall *fnOnErrRtnOrderInsert)(void* pTraderApi,CZQThostFtdcInputOrderField *pInputOrder, CZQThostFtdcRspInfoField *pRspInfo);
typedef void (__stdcall *fnOnRspError)(void* pApi,CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void (__stdcall *fnOnRspOrderAction)(void* pTraderApi,CZQThostFtdcInputOrderActionField *pInputOrderAction, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void (__stdcall *fnOnRspOrderInsert)(void* pTraderApi,CZQThostFtdcInputOrderField *pInputOrder, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void (__stdcall *fnOnRspQryDepthMarketData)(void* pTraderApi,CZQThostFtdcDepthMarketDataField *pDepthMarketData, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void (__stdcall *fnOnRspQryInstrument)(void* pTraderApi,CZQThostFtdcInstrumentField *pInstrument, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void (__stdcall *fnOnRspQryInstrumentCommissionRate)(void* pTraderApi,CZQThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void (__stdcall *fnOnRspQryInstrumentMarginRate)(void* pTraderApi,CZQThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void (__stdcall *fnOnRspQryInvestorPosition)(void* pTraderApi,CZQThostFtdcInvestorPositionField *pInvestorPosition, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void (__stdcall *fnOnRspQryInvestorPositionDetail)(void* pTraderApi,CZQThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void (__stdcall *fnOnRspQryOrder)(void* pTraderApi,CZQThostFtdcOrderField *pOrder, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void (__stdcall *fnOnRspQryTrade)(void* pTraderApi,CZQThostFtdcTradeField *pTrade, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void (__stdcall *fnOnRspQryTradingAccount)(void* pTraderApi,CZQThostFtdcTradingAccountField *pTradingAccount, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
typedef void (__stdcall *fnOnRtnDepthMarketData)(void* pMdUserApi,CZQThostFtdcDepthMarketDataField *pDepthMarketData);
typedef void (__stdcall *fnOnRtnInstrumentStatus)(void* pTraderApi,CZQThostFtdcInstrumentStatusField *pInstrumentStatus);
typedef void (__stdcall *fnOnRtnOrder)(void* pTraderApi,CZQThostFtdcOrderField *pOrder);
typedef void (__stdcall *fnOnRtnTrade)(void* pTraderApi,CZQThostFtdcTradeField *pTrade);

//����������Ϣ���У�֧����Ӧ����ͽ���
QUANTBOXC2CTPZQ_API void* __stdcall CTP_CreateMsgQueue();

//����Ϣ����ע��ص�����
QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnConnect(void* pMsgQueue,fnOnConnect pCallback);
QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnDisconnect(void* pMsgQueue,fnOnDisconnect pCallback);
QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnErrRtnOrderAction(void* pMsgQueue,fnOnErrRtnOrderAction pCallback);
QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnErrRtnOrderInsert(void* pMsgQueue,fnOnErrRtnOrderInsert pCallback);
QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspError(void* pMsgQueue,fnOnRspError pCallback);
QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspOrderAction(void* pMsgQueue,fnOnRspOrderAction pCallback);
QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspOrderInsert(void* pMsgQueue,fnOnRspOrderInsert pCallback);
QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspQryDepthMarketData(void* pMsgQueue,fnOnRspQryDepthMarketData pCallback);
QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspQryInstrument(void* pMsgQueue,fnOnRspQryInstrument pCallback);
QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspQryInstrumentCommissionRate(void* pMsgQueue,fnOnRspQryInstrumentCommissionRate pCallback);
QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspQryInstrumentMarginRate(void* pMsgQueue,fnOnRspQryInstrumentMarginRate pCallback);
QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspQryInvestorPosition(void* pMsgQueue,fnOnRspQryInvestorPosition pCallback);
QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspQryInvestorPositionDetail(void* pMsgQueue,fnOnRspQryInvestorPositionDetail pCallback);
QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspQryOrder(void* pMsgQueue,fnOnRspQryOrder pCallback);
QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspQryTrade(void* pMsgQueue,fnOnRspQryTrade pCallback);
QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspQryTradingAccount(void* pMsgQueue,fnOnRspQryTradingAccount pCallback);
QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRtnDepthMarketData(void* pMsgQueue,fnOnRtnDepthMarketData pCallback);
QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRtnInstrumentStatus(void* pMsgQueue,fnOnRtnInstrumentStatus pCallback);
QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRtnOrder(void* pMsgQueue,fnOnRtnOrder pCallback);
QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRtnTrade(void* pMsgQueue,fnOnRtnTrade pCallback);

//�ͷ���Ϣ����
QUANTBOXC2CTPZQ_API void __stdcall CTP_ReleaseMsgQueue(void* pMsgQueue);
//�����Ϣ��������
QUANTBOXC2CTPZQ_API void __stdcall CTP_ClearMsgQueue(void* pMsgQueue);

//������Ϣ���е�һ����¼
QUANTBOXC2CTPZQ_API bool __stdcall CTP_ProcessMsgQueue(void* pMsgQueue);
//�������лص����������߳�
QUANTBOXC2CTPZQ_API void __stdcall CTP_StartMsgQueue(void* pMsgQueue);
//ֹͣ���лص����������߳�
QUANTBOXC2CTPZQ_API void __stdcall CTP_StopMsgQueue(void* pMsgQueue);

//����ӿ�=======================================

//��������ӿ�
QUANTBOXC2CTPZQ_API void* __stdcall MD_CreateMdApi();
//����Ϣ����ע�ᵽ����ӿ���
QUANTBOXC2CTPZQ_API void __stdcall MD_RegMsgQueue2MdApi(void* pMdUserApi,void* pMsgQueue);
//����
QUANTBOXC2CTPZQ_API void __stdcall MD_Connect(
	void* pMdUserApi,
	const char* szPath,
	const char* szAddresses,
	const char* szBrokerId,
	const char* szInvestorId,
	const char* szPassword);

//���ĺ�Լ�������Լ�ԡ�,���ָ�
QUANTBOXC2CTPZQ_API void __stdcall MD_Subscribe(void* pMdUserApi,const char* szInstrumentIDs,const char* szExchageID);
//ȡ�����ģ������Լ�ԡ�,���ָ�
QUANTBOXC2CTPZQ_API void __stdcall MD_Unsubscribe(void* pMdUserApi,const char* szInstrumentIDs,const char* szExchageID);
//�Ͽ�����
QUANTBOXC2CTPZQ_API void __stdcall MD_Disconnect(void* pMdUserApi);
//�ͷ�����ӿ�
QUANTBOXC2CTPZQ_API void __stdcall MD_ReleaseMdApi(void* pMdUserApi);

//���׽ӿ�=======================================
QUANTBOXC2CTPZQ_API void* __stdcall TD_CreateTdApi();
//����Ϣ����ע�ᵽ���׽ӿ���
QUANTBOXC2CTPZQ_API void __stdcall TD_RegMsgQueue2TdApi(void* pTraderApi,void* pMsgQueue);
//����
QUANTBOXC2CTPZQ_API void __stdcall TD_Connect(
	void* pTraderApi,
	const char* szPath,
	const char* szAddresses,
	const char* szBrokerId,
	const char* szInvestorId,
	const char* szPassword,
	ZQTHOST_TE_RESUME_TYPE nResumeType,
	const char* szUserProductInfo,
	const char* szAuthCode);

//����
QUANTBOXC2CTPZQ_API int __stdcall TD_SendOrder(
	void* pTraderApi,
	int OrderRef,
	const char* szInstrument,
	const char* szExchage,
	TZQThostFtdcDirectionType Direction,
	const char* szCombOffsetFlag,
	const char* szCombHedgeFlag,
	TZQThostFtdcVolumeType VolumeTotalOriginal,
	double LimitPrice,
	TZQThostFtdcOrderPriceTypeType OrderPriceType,
	TZQThostFtdcTimeConditionType TimeCondition,
	TZQThostFtdcContingentConditionType ContingentCondition,
	double StopPrice,
	TZQThostFtdcVolumeConditionType VolumeCondition);

//����
QUANTBOXC2CTPZQ_API void __stdcall TD_CancelOrder(void* pTraderApi,CZQThostFtdcOrderField *pOrder);

//�Ͽ�����
QUANTBOXC2CTPZQ_API void __stdcall TD_Disconnect(void* pTraderApi);
//�ͷ�����ӿ�
QUANTBOXC2CTPZQ_API void __stdcall TD_ReleaseTdApi(void* pTraderApi);
//���ۺϳֲ�
QUANTBOXC2CTPZQ_API void __stdcall TD_ReqQryInvestorPosition(void* pTraderApi,const char* szInstrumentId);
//��ֲ���ϸ
QUANTBOXC2CTPZQ_API void __stdcall TD_ReqQryInvestorPositionDetail(void* pTraderApi,const char* szInstrumentId);
//���ʽ��˺�
QUANTBOXC2CTPZQ_API void __stdcall TD_ReqQryTradingAccount(void* pTraderApi);
//���Լ
QUANTBOXC2CTPZQ_API void __stdcall TD_ReqQryInstrument(void* pTraderApi,const char* szInstrumentId);
//��������
QUANTBOXC2CTPZQ_API void __stdcall TD_ReqQryInstrumentCommissionRate(void* pTraderApi,const char* szInstrumentId);
//�鱣֤��
//QUANTBOXC2CTPZQ_API void __stdcall TD_ReqQryInstrumentMarginRate(void* pTraderApi,const char* szInstrumentId);
//���������
QUANTBOXC2CTPZQ_API void __stdcall TD_ReqQryDepthMarketData(void* pTraderApi,const char* szInstrumentId);

#ifdef __cplusplus
}
#endif


#endif//end of _QUANTBOXC2CTP_H_