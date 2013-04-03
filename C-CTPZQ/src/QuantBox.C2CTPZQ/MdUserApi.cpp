#include "StdAfx.h"
#include "MdUserApi.h"
#include "CTPZQMsgQueue.h"
#include "include\toolkit.h"
#include "include\Lock.h"

#include <iostream>
using namespace std;

CMdUserApi::CMdUserApi(void)
{
	m_msgQueue = NULL;
	m_status = E_uninit;
	m_nRequestID = 0;

	InitializeCriticalSection(&m_csMapInstrumentIDs);
}

CMdUserApi::~CMdUserApi(void)
{
	Disconnect();

	DeleteCriticalSection(&m_csMapInstrumentIDs);
}

void CMdUserApi::RegisterMsgQueue(CCTPZQMsgQueue* pMsgQueue)
{
	m_msgQueue = pMsgQueue;
}

bool CMdUserApi::IsErrorRspInfo(CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)   
{
	bool bRet = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if(bRet)
	{
		if(m_msgQueue)
			m_msgQueue->Input_OnRspError(this,pRspInfo,nRequestID,bIsLast);
	}
	return bRet;
}

bool CMdUserApi::IsErrorRspInfo(CZQThostFtdcRspInfoField *pRspInfo)   
{
	bool bRet = ((pRspInfo) && (pRspInfo->ErrorID != 0));

	return bRet;
}

void CMdUserApi::Connect(const string& szPath,
		const string& szAddresses,
		const string& szBrokerId,
		const string& szInvestorId,
		const string& szPassword)
{
	m_szBrokerId = szBrokerId;
	m_szInvestorId = szInvestorId;
	m_szPassword = szPassword;
	
	char *pszPath = new char[szPath.length()+20];
	sprintf(pszPath,"%s\\Md\\",szPath.c_str());
	makedirs(pszPath);

	m_pApi = CZQThostFtdcMdApi::CreateFtdcMdApi(pszPath,(szAddresses.find("udp://") != szAddresses.npos));
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
				char * pch = strstr(token,"udp://");
				if(pch)
				{
					strncpy (pch,"tcp://",6);
				}
				m_pApi->RegisterFront(token);
			}
			token = strtok( NULL, _QUANTBOXC2CTPZQ_SEPS_);
		}
		delete[] buf;
		
		//初始化连接
		m_pApi->Init();
		m_status = E_connecting;
		if(m_msgQueue)
			m_msgQueue->Input_OnConnect(this,NULL,m_status);
	}
}

void CMdUserApi::ReqUserLogin()
{
	if (NULL == m_pApi)
		return;

	CZQThostFtdcReqUserLoginField request = {0};
	
	strncpy(request.BrokerID, m_szBrokerId.c_str(),sizeof(TZQThostFtdcBrokerIDType));
	strncpy(request.UserID, m_szInvestorId.c_str(),sizeof(TZQThostFtdcInvestorIDType));
	strncpy(request.Password, m_szPassword.c_str(),sizeof(TZQThostFtdcPasswordType));
	
	//只有这一处用到了m_nRequestID，没有必要每次重连m_nRequestID都从0开始
	m_pApi->ReqUserLogin(&request,++m_nRequestID);
	m_status = E_logining;
	if(m_msgQueue)
		m_msgQueue->Input_OnConnect(this,NULL,m_status);
}

void CMdUserApi::Disconnect()
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
}

void CMdUserApi::Subscribe(const string& szInstrumentIDs,const string& szExchageID)
{
	if(NULL == m_pApi)
		return;

	vector<char*> vct;

	size_t len = szInstrumentIDs.length()+1;
	char* buf = new char[len];
	strncpy(buf,szInstrumentIDs.c_str(),len);

	len = szExchageID.length()+1;
	char* pExchageID = new char[len];
	strncpy(pExchageID,szExchageID.c_str(),len);

	CLock cl(&m_csMapInstrumentIDs);

	set<string> _setInstrumentIDs;
	map<string,set<string> >::iterator it = m_mapInstrumentIDs.find(szExchageID);
	if (it!=m_mapInstrumentIDs.end())
	{
		_setInstrumentIDs = it->second;
	}

	char* token = strtok(buf, _QUANTBOXC2CTPZQ_SEPS_);
	while(token)
	{
		size_t l = strlen(token);
		if (l>0)
		{
			//合约已经存在，不用再订阅，但多次订阅也没关系
			_setInstrumentIDs.insert(token);//记录此合约进行订阅
			vct.push_back(token);
		}
		token = strtok( NULL, _QUANTBOXC2CTPZQ_SEPS_);
	}

	m_mapInstrumentIDs[szExchageID] = _setInstrumentIDs;
	
	if(vct.size()>0)
	{
		//转成字符串数组
		char** pArray = new char*[vct.size()];
		for (size_t j = 0; j<vct.size(); ++j)
		{
			pArray[j] = vct[j];
		}

		//订阅
		m_pApi->SubscribeMarketData(pArray,(int)vct.size(),pExchageID);

		delete[] pArray;
	}

	//释放内存
	delete[] buf;
	delete[] pExchageID;
}

void CMdUserApi::Subscribe(const set<string>& instrumentIDs,const string& szExchageID)
{
	if(NULL == m_pApi)
		return;

	string szInstrumentIDs;
	for(set<string>::iterator i=instrumentIDs.begin();i!=instrumentIDs.end();++i)
	{
		szInstrumentIDs.append(*i);
		szInstrumentIDs.append(";");
	}

	if (szInstrumentIDs.length()>1)
	{
		Subscribe(szInstrumentIDs,szExchageID);
	}
}

void CMdUserApi::Unsubscribe(const string& szInstrumentIDs,const string& szExchageID)
{
	if(NULL == m_pApi)
		return;

	vector<char*> vct;
	size_t len = szInstrumentIDs.length()+1;
	char* buf = new char[len];
	strncpy(buf,szInstrumentIDs.c_str(),len);

	len = szExchageID.length()+1;
	char* pExchageID = new char[len];
	strncpy(pExchageID,szExchageID.c_str(),len);

	CLock cl(&m_csMapInstrumentIDs);

	set<string> _setInstrumentIDs;
	map<string,set<string> >::iterator it = m_mapInstrumentIDs.find(szExchageID);
	if (it!=m_mapInstrumentIDs.end())
	{
		_setInstrumentIDs = it->second;
	}

	char* token = strtok(buf, _QUANTBOXC2CTPZQ_SEPS_);
	while(token)
	{
		size_t l = strlen(token);
		if (l>0)
		{
			//合约已经不存在，不用再取消订阅，但多次取消也没什么关系
			_setInstrumentIDs.erase(token);
			vct.push_back(token);
		}
		token = strtok( NULL, _QUANTBOXC2CTPZQ_SEPS_);
	}

	m_mapInstrumentIDs[szExchageID] = _setInstrumentIDs;
	
	if(vct.size()>0)
	{
		//转成字符串数组
		char** pArray = new char*[vct.size()];
		for (size_t j = 0; j<vct.size(); ++j)
		{
			pArray[j] = vct[j];
		}

		//订阅
		m_pApi->UnSubscribeMarketData(pArray,(int)vct.size(),pExchageID);

		delete[] pArray;
	}

	//释放内存
	delete[] buf;
	delete[] pExchageID;
}

void CMdUserApi::OnFrontConnected()
{
	m_status =  E_connected;
	if(m_msgQueue)
		m_msgQueue->Input_OnConnect(this,NULL,m_status);

	//连接成功后自动请求登录
	ReqUserLogin();
}

void CMdUserApi::OnFrontDisconnected(int nReason)
{
	m_status = E_unconnected;
	CZQThostFtdcRspInfoField RspInfo;
	//连接失败返回的信息是拼接而成，主要是为了统一输出
	RspInfo.ErrorID = nReason;
	GetOnFrontDisconnectedMsg(&RspInfo);
	
	if(m_msgQueue)
		m_msgQueue->Input_OnDisconnect(this,&RspInfo,m_status);
}

void CMdUserApi::OnRspUserLogin(CZQThostFtdcRspUserLoginField *pRspUserLogin, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if (!IsErrorRspInfo(pRspInfo)
		&&pRspUserLogin)
	{
		m_status = E_logined;
		if(m_msgQueue)
			m_msgQueue->Input_OnConnect(this,pRspUserLogin,m_status);
		
		//有可能断线了，本处是断线重连后重新订阅
		map<string,set<string> > mapOld = m_mapInstrumentIDs;//记下上次订阅的合约
		
		for(map<string,set<string> >::iterator i=mapOld.begin();i!=mapOld.end();++i)
		{
			string strkey = i->first;
			set<string> setValue = i->second;

			Subscribe(setValue,strkey);//订阅
		}
	}
	else
	{
		m_status = E_authed;
		if(m_msgQueue)
			m_msgQueue->Input_OnDisconnect(this,pRspInfo,E_logining);
	}
}

void CMdUserApi::OnRspError(CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRspError(this,pRspInfo,nRequestID,bIsLast);
}

void CMdUserApi::OnRspSubMarketData(CZQThostFtdcSpecificInstrumentField *pSpecificInstrument, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//在模拟平台可能这个函数不会触发，所以要自己维护一张已经订阅的合约列表
	if(!IsErrorRspInfo(pRspInfo,nRequestID,bIsLast)
		&&pSpecificInstrument)
	{
		CLock cl(&m_csMapInstrumentIDs);

		set<string> _setInstrumentIDs;
		map<string,set<string> >::iterator it = m_mapInstrumentIDs.find(pSpecificInstrument->ExchangeID);
		if (it!=m_mapInstrumentIDs.end())
		{
			_setInstrumentIDs = it->second;
		}

		_setInstrumentIDs.insert(pSpecificInstrument->InstrumentID);
		m_mapInstrumentIDs[pSpecificInstrument->ExchangeID] = _setInstrumentIDs;
	}
}

void CMdUserApi::OnRspUnSubMarketData(CZQThostFtdcSpecificInstrumentField *pSpecificInstrument, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	//模拟平台可能这个函数不会触发
	if(!IsErrorRspInfo(pRspInfo,nRequestID,bIsLast)
		&&pSpecificInstrument)
	{
		CLock cl(&m_csMapInstrumentIDs);

		set<string> _setInstrumentIDs;
		map<string,set<string> >::iterator it = m_mapInstrumentIDs.find(pSpecificInstrument->ExchangeID);
		if (it!=m_mapInstrumentIDs.end())
		{
			_setInstrumentIDs = it->second;
		}
		_setInstrumentIDs.erase(pSpecificInstrument->InstrumentID);
		m_mapInstrumentIDs[pSpecificInstrument->ExchangeID] = _setInstrumentIDs;
	}
}

//行情回调，得保证此函数尽快返回
void CMdUserApi::OnRtnDepthMarketData(CZQThostFtdcDepthMarketDataField *pDepthMarketData)
{
	if(m_msgQueue)
		m_msgQueue->Input_OnRtnDepthMarketData(this,pDepthMarketData);
}