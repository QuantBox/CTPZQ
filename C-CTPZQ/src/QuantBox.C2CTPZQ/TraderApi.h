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
	//�������ݰ�����
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

	//�������ݰ��ṹ��
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
	//���ݰ������߳�
	friend DWORD WINAPI SendThread(LPVOID lpParam);
	void RunInThread();
	void StopThread();

	//ָ�����ݰ����ͣ����ɶ�Ӧ���ݰ�
	SRequest * MakeRequestBuf(RequestType type);
	//������������������
	void ReleaseRequestListBuf();
	//����ѷ����������
	void ReleaseRequestMapBuf();
	//���ָ�����������ָ����
	void ReleaseRequestMapBuf(int nRequestID);
	//��ӵ��Ѿ��������
	void AddRequestMapBuf(int nRequestID,SRequest* pRequest);
	//��ӵ������Ͱ�����
	void AddToSendQueue(SRequest * pRequest);

	void ReqAuthenticate();
	void ReqUserLogin();
	//void ReqSettlementInfoConfirm();

	//����Ƿ����
	bool IsErrorRspInfo(CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);//����Ϣ���������Ϣ
	bool IsErrorRspInfo(CZQThostFtdcRspInfoField *pRspInfo);//�������Ϣ

	//����
	virtual void OnFrontConnected();
	virtual void OnFrontDisconnected(int nReason);

	//��֤
	virtual void OnRspAuthenticate(CZQThostFtdcRspAuthenticateField *pRspAuthenticateField, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspUserLogin(CZQThostFtdcRspUserLoginField *pRspUserLogin, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//virtual void OnRspSettlementInfoConfirm(CZQThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//�µ�
	virtual void OnRspOrderInsert(CZQThostFtdcInputOrderField *pInputOrder, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnErrRtnOrderInsert(CZQThostFtdcInputOrderField *pInputOrder, CZQThostFtdcRspInfoField *pRspInfo);

	//����
	virtual void OnRspOrderAction(CZQThostFtdcInputOrderActionField *pInputOrderAction, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnErrRtnOrderAction(CZQThostFtdcOrderActionField *pOrderAction, CZQThostFtdcRspInfoField *pRspInfo);
	
	//�����ر�
	virtual void OnRspQryOrder(CZQThostFtdcOrderField *pOrder, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnOrder(CZQThostFtdcOrderField *pOrder);
	
	//�ɽ��ر�
	virtual void OnRspQryTrade(CZQThostFtdcTradeField *pTrade, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnTrade(CZQThostFtdcTradeField *pTrade);

	//��λ
	virtual void OnRspQryInvestorPosition(CZQThostFtdcInvestorPositionField *pInvestorPosition, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQryInvestorPositionDetail(CZQThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQryInvestorPositionCombineDetail(CZQThostFtdcInvestorPositionCombineDetailField *pInvestorPositionCombineDetail, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast) {};

	//�ʽ�
	virtual void OnRspQryTradingAccount(CZQThostFtdcTradingAccountField *pTradingAccount, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	//��Լ��������
	virtual void OnRspQryInstrument(CZQThostFtdcInstrumentField *pInstrument, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	//virtual void OnRspQryInstrumentMarginRate(CZQThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRspQryInstrumentCommissionRate(CZQThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	
	//��ѯ������Ӧ
	virtual void OnRspQryDepthMarketData(CZQThostFtdcDepthMarketDataField *pDepthMarketData, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);

	//����
	virtual void OnRspError(CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast);
	virtual void OnRtnInstrumentStatus(CZQThostFtdcInstrumentStatusField *pInstrumentStatus);

private:
	ConnectionStatus			m_status;				//����״̬
	atomic<int>					m_lRequestID;			//����ID,�ñ�������
	
	CZQThostFtdcRspUserLoginField m_RspUserLogin;			//���صĵ�¼�ɹ���Ӧ��Ŀǰ���ô��ڳ�Ա���б�����������

	mutex						m_csOrderRef;
	int							m_nMaxOrderRef;			//�������ã��������ֱ�������������

	CZQThostFtdcTraderApi*		m_pApi;					//����API
	CCTPZQMsgQueue*				m_msgQueue;				//��Ϣ����ָ��

	string						m_szPath;				//���������ļ���·��
	set<string>					m_arrAddresses;			//��������ַ
	string						m_szBrokerId;			//����ID
	string						m_szInvestorId;			//Ͷ����ID
	string						m_szPassword;			//����
	string						m_szUserProductInfo;	//��Ʒ��Ϣ
	string						m_szAuthCode;			//��֤��

	int							m_nSleep;
	bool						m_bRunning;
	HANDLE						m_hThread;

	mutex						m_csList;
	list<SRequest*>				m_reqList;				//�������������

	mutex						m_csMap;
	map<int,SRequest*>			m_reqMap;				//�ѷ��������
};

