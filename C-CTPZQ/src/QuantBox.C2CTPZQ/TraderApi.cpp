#include "StdAfx.h"
#include "TraderApi.h"
#include "CTPZQMsgQueue.h"
#include "include\toolkit.h"
#include "include\Lock.h"

CTraderApi::CTraderApi(void)
{
	m_msgQueue = NULL;
	m_status = E_uninit;
	m_lRequestID = 0;

	m_hThread = NULL;
	m_bRunning = false;

	InitializeCriticalSection(&m_csList);
	InitializeCriticalSection(&m_csMap);
	InitializeCriticalSection(&m_csOrderRef);
}


CTraderApi::~CTraderApi(void)
{
	Disconnect();

	DeleteCriticalSection(&m_csList);
	DeleteCriticalSection(&m_csMap);
	DeleteCriticalSection(&m_csOrderRef);
}

void CTraderApi::StopThread()
{
	//停止发送线程
	m_bRunning = false;
	WaitForSingleObject(m_hThread,INFINITE);
	CloseHandle(m_hThread);
	m_hThread = NULL;
}

void CTraderApi::RegisterMsgQueue(CCTPZQMsgQueue* pMsgQueue)
{
	m_msgQueue = pMsgQueue;
}

bool CTraderApi::IsErrorRspInfo(CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)   
{
	bool bRet = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if(bRet)
	{
		if(m_msgQueue)
			m_msgQueue->Input_OnRspError(this,pRspInfo,nRequestID,bIsLast);
	}
	return bRet;
}

bool CTraderApi::IsErrorRspInfo(CZQThostFtdcRspInfoField *pRspInfo)   
{
	bool bRet = ((pRspInfo) && (pRspInfo->ErrorID != 0));

	return bRet;
}

void CTraderApi::Connect(const string& szPath,
		const string& szAddresses,
		const string& szBrokerId,
		const string& szInvestorId,
		const string& szPassword,
		ZQTHOST_TE_RESUME_TYPE nResumeType,
		const string& szUserProductInfo,
		const string& szAuthCode)
{
	m_szBrokerId = szBrokerId;
	m_szInvestorId = szInvestorId;
	m_szPassword = szPassword;
	m_szUserProductInfo = szUserProductInfo;
	m_szAuthCode = szAuthCode;

	char *pszPath = new char[szPath.length()+20];
	sprintf(pszPath,"%s\\Td\\",szPath.c_str());
	makedirs(pszPath);

	m_pApi = CZQThostFtdcTraderApi::CreateFtdcTraderApi(pszPath);
	delete[] pszPath;

	m_status = E_inited;
	if(m_msgQueue)
		m_msgQueue->Input_OnConnect(this,NULL,m_status);
	
	if (m_pApi)
	{
		m_pApi->RegisterSpi(this);
		
		//添加地址
		size_t len = szAddresses.length()+1;
		char* buf = new char[len];
		strncpy(buf,szAddresses.c_str(),len);

		char* token = strtok(buf, _QUANTBOXC2CTPZQ_SEPS_);
		while(token)
		{
			if (strlen(token)>0)
			{
				m_pApi->RegisterFront(token);
			}
			token = strtok( NULL, _QUANTBOXC2CTPZQ_SEPS_);
		}
		delete[] buf;

		m_pApi->SubscribePublicTopic(nResumeType);
		m_pApi->SubscribePrivateTopic(nResumeType);
		
		//初始化连接
		m_pApi->Init();
		m_status = E_connecting;
		if(m_msgQueue)
			m_msgQueue->Input_OnConnect(this,NULL,m_status);
	}
}

void CTraderApi::Disconnect()
{
	m_status = E_unconnected;
	if(m_pApi)
	{
		m_pApi->RegisterSpi(NULL);
		m_pApi->Release();
		m_pApi = NULL;

		if(m_msgQueue)
			m_msgQueue->Input_OnDisconnect(this,NULL,m_status);
	}

	StopThread();

	m_lRequestID = 0;//由于线程已经停止，没有必要用原子操作了

	ReleaseRequestListBuf();
	ReleaseRequestMapBuf();
}

CTraderApi::SRequest* CTraderApi::MakeRequestBuf(RequestType type)
{
	SRequest *pRequest = new SRequest;
	if (NULL == pRequest)
		return NULL;

	memset(pRequest,0,sizeof(SRequest));
	pRequest->type = type;

	return pRequest;
}

void CTraderApi::ReleaseRequestListBuf()
{
	CLock cl(&m_csList);
	while (!m_reqList.empty())
	{
		SRequest * pRequest = m_reqList.front();
		delete pRequest;
		m_reqList.pop_front();
	}
}

void CTraderApi::ReleaseRequestMapBuf()
{
	CLock cl(&m_csMap);
	for (map<int,SRequest*>::iterator it=m_reqMap.begin();it!=m_reqMap.end();++it)
	{
		delete (*it).second;
	}
	m_reqMap.clear();
}

void CTraderApi::ReleaseRequestMapBuf(int nRequestID)
{
	CLock cl(&m_csMap);
	map<int,SRequest*>::iterator it = m_reqMap.find(nRequestID);
	if (it!=m_reqMap.end())
	{
		delete it->second;
		m_reqMap.erase(nRequestID);
	}
}

void CTraderApi::AddRequestMapBuf(int nRequestID,SRequest* pRequest)
{
	if(NULL == pRequest)
		return;

	CLock cl(&m_csMap);
	map<int,SRequest*>::iterator it = m_reqMap.find(nRequestID);
	if (it!=m_reqMap.end())
	{
		SRequest* p = it->second;
		if(pRequest != p)//如果实际上指的是同一内存，不再插入
		{
			delete p;
			m_reqMap[nRequestID] = pRequest;
		}
	}
}

DWORD WINAPI SendThread(LPVOID lpParam)
{
	CTraderApi* pTrade = reinterpret_cast<CTraderApi *>(lpParam);
	if (pTrade)
		pTrade->RunInThread();
	return 0;
}

void CTraderApi::AddToSendQueue(SRequest * pRequest)
{
	if (NULL == pRequest)
		return;

	CLock cl(&m_csList);
	bool bFind = false;
	//目前不去除相同类型的请求，即没有对大量同类型请求进行优化
	//for (list<SRequest*>::iterator it = m_reqList.begin();it!= m_reqList.end();++it)
	//{
	//	if (pRequest->type == (*it)->type)
	//	{
	//		bFind = true;
	//		break;
	//	}
	//}

	if (!bFind)
		m_reqList.push_back(pRequest);

	if (NULL == m_hThread
		&&!m_reqList.empty())
	{
		m_bRunning = true;
		m_hThread = CreateThread(NULL,0,SendThread,this,0,NULL); 
	}
}


void CTraderApi::RunInThread()
{
	int iRet = 0;

	while (!m_reqList.empty()&&m_bRunning)
	{
		SRequest * pRequest = m_reqList.front();
		long lRequest = InterlockedIncrement(&m_lRequestID);
		switch(pRequest->type)
		{
		case E_ReqAuthenticateField:
			iRet = m_pApi->ReqAuthenticate(&pRequest->ReqAuthenticateField,lRequest);
			break;
		case E_ReqUserLoginField:
			iRet = m_pApi->ReqUserLogin(&pRequest->ReqUserLoginField,lRequest);
			break;
		//case E_SettlementInfoConfirmField:
		//	iRet = m_pApi->ReqSettlementInfoConfirm(&pRequest->SettlementInfoConfirmField,lRequest);
		//	break;
		case E_QryInstrumentField:
			iRet = m_pApi->ReqQryInstrument(&pRequest->QryInstrumentField,lRequest);
			break;
		case E_QryTradingAccountField:
			iRet = m_pApi->ReqQryTradingAccount(&pRequest->QryTradingAccountField,lRequest);
			break;
		case E_QryInvestorPositionField:
			iRet = m_pApi->ReqQryInvestorPosition(&pRequest->QryInvestorPositionField,lRequest);
			break;
		case E_QryInstrumentCommissionRateField:
			iRet = m_pApi->ReqQryInstrumentCommissionRate(&pRequest->QryInstrumentCommissionRateField,lRequest);
			break;
		//case E_QryInstrumentMarginRateField:
		//	iRet = m_pApi->ReqQryInstrumentMarginRate(&pRequest->QryInstrumentMarginRateField,lRequest);
		//	break;
		case E_QryDepthMarketDataField:
			iRet = m_pApi->ReqQryDepthMarketData(&pRequest->QryDepthMarketDataField,lRequest);
			break;
		default:
			_ASSERT(FALSE);
			break;
		}

		if (0 == iRet)
		{
			//返回成功，填加到已发送池
			m_nSleep = 1;
			AddRequestMapBuf(lRequest,pRequest);

			CLock cl(&m_csList);
			m_reqList.pop_front();
		}
		else
		{
			//失败，按2的幂进行延时，但不超过1s
			m_nSleep *= 2;
			m_nSleep %= 1000;
		}
		Sleep(m_nSleep);
	}

	//清理线程
	CloseHandle(m_hThread);
	m_hThread = NULL;
	m_bRunning = false;
}

void CTraderApi::OnFrontConnected()
{
	m_status =  E_connected;
	if(m_msgQueue)
		m_msgQueue->Input_OnConnect(this,NULL,m_status);

	//连接成功后自动请求认证或登录
	if(m_szAuthCode.length()>0
		&&m_szUserProductInfo.length()>0)
	{
		//填了认证码就先认证
		ReqAuthenticate();
	}
	else
	{
		ReqUserLogin();
	}
}

void CTraderApi::OnFrontDisconnected(int nReason)
{
	m_status = E_unconnected;
	CZQThostFtdcRspInfoField RspInfo;
	//构造出来的错误消息，为了统一出错信息
	RspInfo.ErrorID = nReason;
	GetOnFrontDisconnectedMsg(&RspInfo);

	if(m_msgQueue)
		m_msgQueue->Input_OnDisconnect(this,&RspInfo,m_status);
}

void CTraderApi::ReqAuthenticate()
{
	if (NULL == m_pApi)
		return;

	SRequest* pRequest = MakeRequestBuf(E_ReqAuthenticateField);
	if (pRequest)
	{
		m_status = E_authing;
		if(m_msgQueue)
			m_msgQueue->Input_OnConnect(this,NULL,m_status);

		CZQThostFtdcReqAuthenticateField& body = pRequest->ReqAuthenticateField;

		strncpy(body.BrokerID, m_szBrokerId.c_str(),sizeof(TZQThostFtdcBrokerIDType));
		strncpy(body.UserID, m_szInvestorId.c_str(),sizeof(TZQThostFtdcInvestorIDType));
		strncpy(body.UserProductInfo,m_szUserProductInfo.c_str(),sizeof(TZQThostFtdcProductInfoType));
		strncpy(body.AuthCode,m_szAuthCode.c_str(),sizeof(TZQThostFtdcAuthCodeType));

		AddToSendQueue(pRequest);
	}
}

void CTraderApi::OnRspAuthenticate(CZQThostFtdcRspAuthenticateField *pRspAuthenticateField, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!IsErrorRspInfo(pRspInfo)
		&&pRspAuthenticateField)
	{
		m_status = E_authed;
		if(m_msgQueue)
			m_msgQueue->Input_OnConnect(this,NULL,m_status);

		ReqUserLogin();
	}
	else
	{
		m_status = E_connected;
		if(m_msgQueue)
			m_msgQueue->Input_OnDisconnect(this,pRspInfo,E_authing);
	}

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}

void CTraderApi::ReqUserLogin()
{
	if (NULL == m_pApi)
		return;

	SRequest* pRequest = MakeRequestBuf(E_ReqUserLoginField);
	if (pRequest)
	{
		m_status = E_logining;
		if(m_msgQueue)
			m_msgQueue->Input_OnConnect(this,NULL,m_status);

		CZQThostFtdcReqUserLoginField& body = pRequest->ReqUserLoginField;

		strncpy(body.BrokerID, m_szBrokerId.c_str(),sizeof(TZQThostFtdcBrokerIDType));
		strncpy(body.UserID, m_szInvestorId.c_str(),sizeof(TZQThostFtdcInvestorIDType));
		strncpy(body.Password, m_szPassword.c_str(),sizeof(TZQThostFtdcPasswordType));
		strncpy(body.UserProductInfo,m_szUserProductInfo.c_str(),sizeof(TZQThostFtdcProductInfoType));

		AddToSendQueue(pRequest);
	}
}

void CTraderApi::OnRspUserLogin(CZQThostFtdcRspUserLoginField *pRspUserLogin, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!IsErrorRspInfo(pRspInfo)
		&&pRspUserLogin)
	{
		m_status = E_logined;
		if(m_msgQueue)
			m_msgQueue->Input_OnConnect(this,pRspUserLogin,m_status);
		
		memcpy(&m_RspUserLogin,pRspUserLogin,sizeof(CZQThostFtdcRspUserLoginField));
		m_nMaxOrderRef = atol(pRspUserLogin->MaxOrderRef);
		//ReqSettlementInfoConfirm();
	}
	else
	{
		m_status = E_authed;
		if(m_msgQueue)
			m_msgQueue->Input_OnDisconnect(this,pRspInfo,E_logining);
	}

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}

/*
void CTraderApi::ReqSettlementInfoConfirm()
{
	if (NULL == m_pApi)
		return;

	SRequest* pRequest = MakeRequestBuf(E_SettlementInfoConfirmField);
	if (pRequest)
	{
		m_status = E_confirming;
		if(m_msgQueue)
			m_msgQueue->Input_OnConnect(this,NULL,m_status);

		CZQThostFtdcSettlementInfoConfirmField& body = pRequest->SettlementInfoConfirmField;

		strncpy(body.BrokerID, m_szBrokerId.c_str(),sizeof(TZQThostFtdcBrokerIDType));
		strncpy(body.InvestorID, m_szInvestorId.c_str(),sizeof(TZQThostFtdcInvestorIDType));

		AddToSendQueue(pRequest);
	}
}

void CTraderApi::OnRspSettlementInfoConfirm(CZQThostFtdcSettlementInfoConfirmField *pSettlementInfoConfirm, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!IsErrorRspInfo(pRspInfo)
		&&pSettlementInfoConfirm)
	{
		m_status = E_confirmed;
		if(m_msgQueue)
			m_msgQueue->Input_OnConnect(this,NULL,m_status);
	}
	else
	{
		m_status = E_logined;
		if(m_msgQueue)
			m_msgQueue->Input_OnDisconnect(this,pRspInfo,E_confirming);
	}

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}
*/

int CTraderApi::ReqOrderInsert(
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
	TZQThostFtdcPriceType StopPrice)
{
	if (NULL == m_pApi)
		return 0;

	SRequest* pRequest = MakeRequestBuf(E_InputOrderField);
	if (NULL == pRequest)
		return 0;

	CZQThostFtdcInputOrderField& body = pRequest->InputOrderField;
	
	strncpy(body.BrokerID, m_RspUserLogin.BrokerID,sizeof(TZQThostFtdcBrokerIDType));
	strncpy(body.InvestorID, m_RspUserLogin.UserID,sizeof(TZQThostFtdcInvestorIDType));
	
	body.MinVolume = 1;
	body.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	body.IsAutoSuspend = 0;
	body.UserForceClose = 0;

	//合约
	strncpy(body.InstrumentID, szInstrumentId.c_str(),sizeof(TZQThostFtdcInstrumentIDType));
	//交易所，新加
	strncpy(body.ExchangeID, szExchangeId.c_str(),sizeof(TZQThostFtdcExchangeIDType));
	//买卖
	body.Direction = Direction;
	//开平
	memcpy(body.CombOffsetFlag,CombOffsetFlag,sizeof(TZQThostFtdcCombOffsetFlagType));
	
	//价格
	body.OrderPriceType = OrderPriceType;
	//body.LimitPrice = LimitPrice;
	strncpy(body.LimitPrice, LimitPrice,sizeof(TZQThostFtdcStockPriceType));
	
	//数量
	body.VolumeTotalOriginal = VolumeTotalOriginal;
	//投保
	memcpy(body.CombHedgeFlag,CombHedgeFlag,sizeof(TZQThostFtdcCombHedgeFlagType));
	
	//各条件
	body.VolumeCondition = THOST_FTDC_VC_AV;
	body.TimeCondition = TimeCondition;
	body.ContingentCondition = ContingentCondition;
	body.StopPrice = StopPrice;

	int nRet = 0;
	{
		//可能报单太快，m_nMaxOrderRef还没有改变就提交了
		CLock cl(&m_csOrderRef);

		nRet = m_nMaxOrderRef;
		sprintf(body.OrderRef,"%d",nRet);
		++m_nMaxOrderRef;

		//不保存到队列，而是直接发送
		long lRequest = InterlockedIncrement(&m_lRequestID);
		int n = m_pApi->ReqOrderInsert(&pRequest->InputOrderField,lRequest);
	}
	delete pRequest;//用完后直接删除

	//如何定位报单，用报单引用实际上并不靠谱
	//如重新开软件、条件单，同时别的软件下单三个可能性要处理
	return nRet;
}

void CTraderApi::OnRspOrderInsert(CZQThostFtdcInputOrderField *pInputOrder, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspOrderInsert(this,pInputOrder,pRspInfo,nRequestID,bIsLast);
}

void CTraderApi::OnErrRtnOrderInsert(CZQThostFtdcInputOrderField *pInputOrder, CZQThostFtdcRspInfoField *pRspInfo)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnErrRtnOrderInsert(this,pInputOrder,pRspInfo);
}

void CTraderApi::OnRtnTrade(CZQThostFtdcTradeField *pTrade)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRtnTrade(this,pTrade);
}

void CTraderApi::ReqOrderAction(CZQThostFtdcOrderField *pOrder)
{
	if (NULL == m_pApi)
		return;
	
	SRequest* pRequest = MakeRequestBuf(E_InputOrderActionField);
	if (NULL == pRequest)
		return;
	
	CZQThostFtdcInputOrderActionField& body = pRequest->InputOrderActionField;

	///经纪公司代码
	strncpy(body.BrokerID, pOrder->BrokerID,sizeof(TZQThostFtdcBrokerIDType));
	///投资者代码
	strncpy(body.InvestorID, pOrder->InvestorID,sizeof(TZQThostFtdcInvestorIDType));
	///报单引用
	strncpy(body.OrderRef, pOrder->OrderRef,sizeof(TZQThostFtdcOrderRefType));
	///前置编号
	body.FrontID = pOrder->FrontID;
	///会话编号
	body.SessionID = pOrder->SessionID;
	///交易所代码
	strncpy(body.ExchangeID,pOrder->ExchangeID,sizeof(TZQThostFtdcExchangeIDType));
	///报单编号
	//strncpy(body.OrderSysID,pOrder->OrderSysID,sizeof(TZQThostFtdcOrderSysIDType));
	///操作标志
	body.ActionFlag = THOST_FTDC_AF_Delete;
	///合约代码
	strncpy(body.InstrumentID, pOrder->InstrumentID,sizeof(TZQThostFtdcInstrumentIDType));
	
	long lRequest = InterlockedIncrement(&m_lRequestID);
	m_pApi->ReqOrderAction(&pRequest->InputOrderActionField,lRequest);
	delete pRequest;
}

void CTraderApi::OnRspOrderAction(CZQThostFtdcInputOrderActionField *pInputOrderAction, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspOrderAction(this,pInputOrderAction,pRspInfo,nRequestID,bIsLast);
}

void CTraderApi::OnErrRtnOrderAction(CZQThostFtdcOrderActionField *pOrderAction, CZQThostFtdcRspInfoField *pRspInfo)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnErrRtnOrderAction(this,pOrderAction,pRspInfo);
}

void CTraderApi::OnRtnOrder(CZQThostFtdcOrderField *pOrder)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRtnOrder(this,pOrder);
}

void CTraderApi::ReqQryTradingAccount()
{
	if (NULL == m_pApi)
		return;
	
	SRequest* pRequest = MakeRequestBuf(E_QryTradingAccountField);
	if (NULL == pRequest)
		return;
	
	CZQThostFtdcQryTradingAccountField& body = pRequest->QryTradingAccountField;
	
	strncpy(body.BrokerID, m_RspUserLogin.BrokerID,sizeof(TZQThostFtdcBrokerIDType));
	strncpy(body.InvestorID, m_RspUserLogin.UserID,sizeof(TZQThostFtdcInvestorIDType));
	
	AddToSendQueue(pRequest);
}

void CTraderApi::OnRspQryTradingAccount(CZQThostFtdcTradingAccountField *pTradingAccount, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspQryTradingAccount(this,pTradingAccount,pRspInfo,nRequestID,bIsLast);

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}

void CTraderApi::ReqQryInvestorPosition(const string& szInstrumentId)
{
	if (NULL == m_pApi)
		return;
	
	SRequest* pRequest = MakeRequestBuf(E_QryInvestorPositionField);
	if (NULL == pRequest)
		return;
	
	CZQThostFtdcQryInvestorPositionField& body = pRequest->QryInvestorPositionField;
	
	strncpy(body.BrokerID, m_RspUserLogin.BrokerID,sizeof(TZQThostFtdcBrokerIDType));
	strncpy(body.InvestorID, m_RspUserLogin.UserID,sizeof(TZQThostFtdcInvestorIDType));
	strncpy(body.InstrumentID,szInstrumentId.c_str(),sizeof(TZQThostFtdcInstrumentIDType));
	
	AddToSendQueue(pRequest);
}

void CTraderApi::OnRspQryInvestorPosition(CZQThostFtdcInvestorPositionField *pInvestorPosition, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspQryInvestorPosition(this,pInvestorPosition,pRspInfo,nRequestID,bIsLast);

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}

void CTraderApi::ReqQryInstrument(const string& szInstrumentId)
{
	if (NULL == m_pApi)
		return;
	
	SRequest* pRequest = MakeRequestBuf(E_QryInstrumentField);
	if (NULL == pRequest)
		return;
	
	CZQThostFtdcQryInstrumentField& body = pRequest->QryInstrumentField;
	
	strncpy(body.InstrumentID,szInstrumentId.c_str(),sizeof(TZQThostFtdcInstrumentIDType));
	
	AddToSendQueue(pRequest);
}

void CTraderApi::OnRspQryInstrument(CZQThostFtdcInstrumentField *pInstrument, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspQryInstrument(this,pInstrument,pRspInfo,nRequestID,bIsLast);

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}

void CTraderApi::ReqQryInstrumentCommissionRate(const string& szInstrumentId)
{
	if (NULL == m_pApi)
		return;

	SRequest* pRequest = MakeRequestBuf(E_QryInstrumentCommissionRateField);
	if (NULL == pRequest)
		return;

	CZQThostFtdcQryInstrumentCommissionRateField& body = pRequest->QryInstrumentCommissionRateField;

	strncpy(body.BrokerID, m_RspUserLogin.BrokerID,sizeof(TZQThostFtdcBrokerIDType));
	strncpy(body.InvestorID, m_RspUserLogin.UserID,sizeof(TZQThostFtdcInvestorIDType));
	strncpy(body.InstrumentID,szInstrumentId.c_str(),sizeof(TZQThostFtdcInstrumentIDType));

	AddToSendQueue(pRequest);
}

void CTraderApi::OnRspQryInstrumentCommissionRate(CZQThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspQryInstrumentCommissionRate(this,pInstrumentCommissionRate,pRspInfo,nRequestID,bIsLast);

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}
/*
void CTraderApi::ReqQryInstrumentMarginRate(const string& szInstrumentId)
{
	if (NULL == m_pApi)
		return;

	SRequest* pRequest = MakeRequestBuf(E_QryInstrumentMarginRateField);
	if (NULL == pRequest)
		return;

	CZQThostFtdcQryInstrumentMarginRateField& body = pRequest->QryInstrumentMarginRateField;

	strncpy(body.BrokerID, m_RspUserLogin.BrokerID,sizeof(TZQThostFtdcBrokerIDType));
	strncpy(body.InvestorID, m_RspUserLogin.UserID,sizeof(TZQThostFtdcInvestorIDType));
	strncpy(body.InstrumentID,szInstrumentId.c_str(),sizeof(TZQThostFtdcInstrumentIDType));

	AddToSendQueue(pRequest);
}

void CTraderApi::OnRspQryInstrumentMarginRate(CZQThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspQryInstrumentMarginRate(this,pInstrumentMarginRate,pRspInfo,nRequestID,bIsLast);

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}
*/

void CTraderApi::ReqQryDepthMarketData(const string& szInstrumentId)
{
	if (NULL == m_pApi)
		return;

	SRequest* pRequest = MakeRequestBuf(E_QryDepthMarketDataField);
	if (NULL == pRequest)
		return;

	CZQThostFtdcQryDepthMarketDataField& body = pRequest->QryDepthMarketDataField;

	strncpy(body.InstrumentID,szInstrumentId.c_str(),sizeof(TZQThostFtdcInstrumentIDType));

	AddToSendQueue(pRequest);
}

void CTraderApi::OnRspQryDepthMarketData(CZQThostFtdcDepthMarketDataField *pDepthMarketData, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspQryDepthMarketData(this,pDepthMarketData,pRspInfo,nRequestID,bIsLast);

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}

void CTraderApi::OnRspError(CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspError(this,pRspInfo,nRequestID,bIsLast);

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}

void CTraderApi::OnRspQryOrder(CZQThostFtdcOrderField *pOrder, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspQryOrder(this,pOrder,pRspInfo,nRequestID,bIsLast);

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}


void CTraderApi::OnRspQryTrade(CZQThostFtdcTradeField *pTrade, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspQryTrade(this,pTrade,pRspInfo,nRequestID,bIsLast);

	if (bIsLast)
		ReleaseRequestMapBuf(nRequestID);
}