#pragma once

#include "QuantBox.C2CTPZQ.h"
#include "include\CTPZQ\ThostFtdcTraderApiSSE.h"

#include <set>
#include <list>
#include <map>
#include <string>
#include <mutex>
#include <atomic>

using namespace std;

class CCTPZQMsgQueue;

class CTraderApi :
	public CZQThostFtdcTraderSpi
{
	//请求数据包类型
	enum RequestType
	{
		E_ReqAuthenticateField,
		E_ReqUserLoginField,
		//E_SettlementInfoConfirmField,
		E_QryInstrumentField,
		E_InputOrderField,
		E_InputOrderActionField,
		E_QryTradingAccountField,
		E_QryInvestorPositionField,
		E_QryInvestorPositionDetailField,
		E_QryInstrumentCommissionRateField,
		//E_QryInstrumentMarginRateField,
		E_QryDepthMarketDataField,
	};

	//请求数据包结构体
	struct SRequest
	{
		RequestType type;
		union{
			CZQThostFtdcReqAuthenticateField				ReqAuthenticateField;
			CZQThostFtdcReqUserLoginField					ReqUserLoginField;
			CZQThostFtdcSettlementInfoConfirmField			SettlementInfoConfirmField;
			CZQThostFtdcQryDepthMarketDataField				QryDepthMarketDataField;
			CZQThostFtdcQryInstrumentField					QryInstrumentField;
			CZQThostFtdcQryInstrumentCommissionRateField	QryInstrumentCommissionRateField;
			CZQThostFtdcQryInstrumentMarginRateField		QryInstrumentMarginRateField;
			CZQThostFtdcQryInvestorPositionField			QryInvestorPositionField;
			CZQThostFtdcQryInvestorPositionDetailField		QryInvestorPositionDetailField;
			CZQThostFtdcQryTradingAccountField				QryTradingAccountField;
			CZQThostFtdcInputOrderField						InputOrderField;
			CZQThostFtdcInputOrderActionField				InputOrderActionField;
		};
	};

public:
	CTraderApi(void);
	virtual ~CTraderApi(void);

	void RegisterMsgQueue(CCTPZQMsgQueue* pMsgQueue);

	void Connect(const string& szPath,
		const string& szAddresses,
		const string& szBrokerId,
		const string& szInvestorId,
		const string& szPassword,
		ZQTHOST_TE_RESUME_TYPE nResumeType,
		const string& szUserProductInfo,
		const string& szAuthCode);
	void Disconnect();

	int ReqOrderInsert(
		int OrderRef,
		const string& szInstrumentId,
		const string& szExchangeId,
		TZQThostFtdcDirectionType Direction,
		const TZQThostFtdcCombOffsetFlagType CombOffsetFlag,
		const TZQThostFtdcCombHedgeFlagType CombHedgeFlag,
		TZQThostFtdcVolumeType VolumeTotalOriginal,
		TZQThostFtdcStockPriceType LimitPrice,
		TZQThostFtdcOrderPriceTypeType OrderPriceType,
		TZQThostFtdcTimeConditionType TimeCondition,
		TZQThostFtdcContingentConditionType ContingentCondition,
		TZQThostFtdcPriceType StopPrice,
		TZQThostFtdcVolumeConditionType VolumeCondition);
	int ReqOrderAction(CZQThostFtdcOrderField *pOrder);

	void ReqQryTradingAccount();
	void ReqQryInvestorPosition(const string& szInstrumentId);
	void ReqQryInvestorPositionDetail(const string& szInstrumentId);
	void ReqQryInstrument(const string& szInstrumentId);
	void ReqQryInstrumentCommissionRate(const string& szInstrumentId);
	//void ReqQryInstrumentMarginRate(const string& szInstrumentId);
	void ReqQryDepthMarketData(const string& szInstrumentId);

private:
	//数据包发送线程
	friend DWORD WINAPI SendThread(LPVOID lpParam);
	void RunInThread();
	void StopThread();

	//指定数据包类型，生成对应数据包
	SRequest * MakeRequestBuf(RequestType type);
	//清除将发送请求包队列
	void ReleaseRequestListBuf();
	//清除已发送请求包池
	void ReleaseRequestMapBuf();
	//清除指定请求包池中指定包
	void ReleaseRequestMapBuf(int nRequestID);
	//添加到已经请求包池
	void AddRequestMapBuf(int nRequestID,SRequest* pRequest);
	//添加到将发送包队列
	void AddToSendQueue(SRequest * pRequest);

	void ReqAuthenticate();
	void ReqUserLogin();
	//void ReqSettlementInfoConfirm();

	//检查是否出错
	bool IsErrorRspInfo(CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);//向消息队列输出信息
	bool IsErrorRspInfo(CZQThostFtdcRspInfoField *pRspInfo);//不输出信息

	//连接
	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);

	//认证
	virtual void OnRspAuthenticate(CZQThostFtdcRspAuthenticateField *pRspAuthenticateField, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspUserLogin(CZQThostFtdcRspUserLoginField *pRspUserLogin, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//virtual void OnRspSettlementInfoConfirm(CZQThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//下单
	virtual void OnRspOrderInsert(CZQThostFtdcInputOrderField *pInputOrder, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnErrRtnOrderInsert(CZQThostFtdcInputOrderField *pInputOrder, CZQThostFtdcRspInfoField *pRspInfo);

	//撤单
	virtual void OnRspOrderAction(CZQThostFtdcInputOrderActionField *pInputOrderAction, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnErrRtnOrderAction(CZQThostFtdcOrderActionField *pOrderAction, CZQThostFtdcRspInfoField *pRspInfo);
	
	//报单回报
	virtual void OnRspQryOrder(CZQThostFtdcOrderField *pOrder, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnOrder(CZQThostFtdcOrderField *pOrder);
	
	//成交回报
	virtual void OnRspQryTrade(CZQThostFtdcTradeField *pTrade, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnTrade(CZQThostFtdcTradeField *pTrade);

	//仓位
	virtual void OnRspQryInvestorPosition(CZQThostFtdcInvestorPositionField *pInvestorPosition, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQryInvestorPositionDetail(CZQThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQryInvestorPositionCombineDetail(CZQThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	//资金
	virtual void OnRspQryTradingAccount(CZQThostFtdcTradingAccountField *pTradingAccount, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	//合约、手续费
	virtual void OnRspQryInstrument(CZQThostFtdcInstrumentField *pInstrument, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//virtual void OnRspQryInstrumentMarginRate(CZQThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQryInstrumentCommissionRate(CZQThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	//查询行情响应
	virtual void OnRspQryDepthMarketData(CZQThostFtdcDepthMarketDataField *pDepthMarketData, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//其它
	virtual void OnRspError(CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnInstrumentStatus(CZQThostFtdcInstrumentStatusField *pInstrumentStatus);

private:
	ConnectionStatus			m_status;				//连接状态
	atomic<int>					m_lRequestID;			//请求ID,得保持自增
	
	CZQThostFtdcRspUserLoginField m_RspUserLogin;			//返回的登录成功响应，目前利用此内成员进行报单所属区分

	mutex						m_csOrderRef;
	int							m_nMaxOrderRef;			//报单引用，用于区分报单，保持自增

	CZQThostFtdcTraderApi*		m_pApi;					//交易API
	CCTPZQMsgQueue*				m_msgQueue;				//消息队列指针

	string						m_szPath;				//生成配置文件的路径
	set<string>					m_arrAddresses;			//服务器地址
	string						m_szBrokerId;			//期商ID
	string						m_szInvestorId;			//投资者ID
	string						m_szPassword;			//密码
	string						m_szUserProductInfo;	//产品信息
	string						m_szAuthCode;			//认证码

	int							m_nSleep;
	bool						m_bRunning;
	HANDLE						m_hThread;

	mutex						m_csList;
	list<SRequest*>				m_reqList;				//将发送请求队列

	mutex						m_csMap;
	map<int,SRequest*>			m_reqMap;				//已发送请求池
};

