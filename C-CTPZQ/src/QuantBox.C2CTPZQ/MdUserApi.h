#pragma once

#include "QuantBox.C2CTPZQ.h"
#include "include\CTPZQ\ThostFtdcMdApiSSE.h"

#include <set>
#include <map>
#include <string>

using namespace std;

class CCTPZQMsgQueue;

class CMdUserApi :
	public CZQThostFtdcMdSpi
{
public:
	CMdUserApi(void);
	virtual ~CMdUserApi(void);

	void RegisterMsgQueue(CCTPZQMsgQueue* pMsgQueue);

	void Connect(const string& szPath,
		const string& szAddresses,
		const string& szBrokerId,
		const string& szInvestorId,
		const string& szPassword);
	void Disconnect();

	void Subscribe(const string& szInstrumentIDs,const string& szExchageID);
	void Unsubscribe(const string& szInstrumentIDs,const string& szExchageID);

private:
	//订阅行情
	void Subscribe(const set<string>& instrumentIDs,const string& szExchageID);
	//登录请求
	void ReqUserLogin();

	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);
	virtual void OnRspUserLogin(CZQThostFtdcRspUserLoginField *pRspUserLogin, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspError(CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspSubMarketData(CZQThostFtdcSpecificInstrumentField *pSpecificInstrument, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspUnSubMarketData(CZQThostFtdcSpecificInstrumentField *pSpecificInstrument, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketData(CZQThostFtdcDepthMarketDataField *pDepthMarketData);

	//检查是否出错
	bool IsErrorRspInfo(CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);//将出错消息送到消息队列
	bool IsErrorRspInfo(CZQThostFtdcRspInfoField *pRspInfo);//不送出错消息

private:
	CRITICAL_SECTION			m_csMapInstrumentIDs;

	ConnectionStatus			m_status;				//连接状态
	int							m_nRequestID;			//请求ID，每次请求前自增

	map<string,set<string> >	m_mapInstrumentIDs;		//正在订阅的合约
	CZQThostFtdcMdApi*			m_pApi;					//行情API
	CCTPZQMsgQueue*				m_msgQueue;				//消息队列指针

	string						m_szPath;				//生成配置文件的路径
	set<string>					m_arrAddresses;			//服务器地址
	string						m_szBrokerId;			//期商ID
	string						m_szInvestorId;			//投资者ID
	string						m_szPassword;			//密码
};

