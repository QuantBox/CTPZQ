#pragma once

#include "QuantBox.C2Level2.h"
#include "include\Level2\ThostFtdcLevel2UserApi.h"

#include <set>
#include <map>
#include <string>

using namespace std;
_USING_LEVEL2_NS_

class CLevel2MsgQueue;

class CLevel2UserApi:public CThostFtdcLevel2UserSpi
{
public:
	CLevel2UserApi(void);
	virtual ~CLevel2UserApi(void);

	void RegisterMsgQueue(CLevel2MsgQueue* pMsgQueue);

	void Connect(const string& szPath,
		const string& szAddresses,
		const string& szBrokerId,
		const string& szInvestorId,
		const string& szPassword);
	void Disconnect();

	void SubscribeLevel2MarketData(const string& szInstrumentIDs,const string& szExchageID);
	void UnSubscribeLevel2MarketData(const string& szInstrumentIDs,const string& szExchageID);
	void SubscribeNGTSIndex(const string& szInstrumentIDs,const string& szExchageID);
	void UnSubscribeNGTSIndex(const string& szInstrumentIDs,const string& szExchageID);

private:
	//订阅行情
	void SubscribeLevel2MarketData(const set<string>& instrumentIDs,const string& szExchageID);
	void SubscribeNGTSIndex(const set<string>& instrumentIDs,const string& szExchageID);

	//登录请求
	void ReqUserLogin();

	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField *pRspUserLogin, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspError(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	///Level2行情
	virtual void OnRspSubLevel2MarketData(CThostFtdcSpecificSecurityField *pSpecificSecurity, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspUnSubLevel2MarketData(CThostFtdcSpecificSecurityField *pSpecificSecurity, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnLevel2MarketData(CThostFtdcLevel2MarketDataField *pLevel2MarketData);
	///Level2指数行情
	virtual void OnRspSubNGTSIndex(CThostFtdcSpecificSecurityField *pSpecificSecurity, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspUnSubNGTSIndex(CThostFtdcSpecificSecurityField *pSpecificSecurity, CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnNGTSIndex(CThostFtdcNGTSIndexField *pNGTSIndex);

private:
	//检查是否出错
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);//将出错消息送到消息队列
	bool IsErrorRspInfo(CThostFtdcRspInfoField *pRspInfo);//不送出错消息

private:
	ConnectionStatus			m_status;				//连接状态
	int							m_nRequestID;			//请求ID，每次请求前自增

	map<string,set<string> >	m_mapSecurityIDs;		//正在订阅的合约
	map<string,set<string> >	m_mapIndexIDs;			//正在订阅的合约

	CThostFtdcLevel2UserApi*	m_pApi;					//行情API
	CLevel2MsgQueue*			m_msgQueue;				//消息队列指针

	string						m_szPath;				//生成配置文件的路径
	set<string>					m_arrAddresses;			//服务器地址
	string						m_szBrokerId;			//期商ID
	string						m_szInvestorId;			//投资者ID
	string						m_szPassword;			//密码
};

