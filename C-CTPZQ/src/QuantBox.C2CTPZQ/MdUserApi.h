#pragma once

#include "QuantBox.C2CTPZQ.h"
#include "include\CTPZQ\ThostFtdcMdApiSSE.h"

#include <set>
#include <string>
#include <atomic>
#include <mutex>
#include <map>

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
	//��������
	void Subscribe(const set<string>& instrumentIDs,const string& szExchageID);
	//��¼����
	void ReqUserLogin();

	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);
	virtual void OnRspUserLogin(CZQThostFtdcRspUserLoginField *pRspUserLogin, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspError(CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspSubMarketData(CZQThostFtdcSpecificInstrumentField *pSpecificInstrument, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspUnSubMarketData(CZQThostFtdcSpecificInstrumentField *pSpecificInstrument, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnDepthMarketData(CZQThostFtdcDepthMarketDataField *pDepthMarketData);

	//����Ƿ����
	bool IsErrorRspInfo(CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);//��������Ϣ�͵���Ϣ����
	bool IsErrorRspInfo(CZQThostFtdcRspInfoField *pRspInfo);//���ͳ�����Ϣ

private:
	mutex						m_csMapInstrumentIDs;

	ConnectionStatus			m_status;				//����״̬
	atomic<int>					m_nRequestID;			//����ID��ÿ������ǰ����

	map<string,set<string> >	m_mapInstrumentIDs;		//���ڶ��ĵĺ�Լ
	CZQThostFtdcMdApi*			m_pApi;					//����API
	CCTPZQMsgQueue*				m_msgQueue;				//��Ϣ����ָ��

	string						m_szPath;				//���������ļ���·��
	set<string>					m_arrAddresses;			//��������ַ
	string						m_szBrokerId;			//����ID
	string						m_szInvestorId;			//Ͷ����ID
	string						m_szPassword;			//����
};

