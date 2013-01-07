#include "StdAfx.h"
#include "CTPZQMsgQueue.h"

void CCTPZQMsgQueue::Clear()
{
	SMsgItem* pItem = NULL;
	//清空队列
	while(m_queue.dequeue(pItem))
	{
		_Output(pItem);
		delete pItem;
	}
}

bool CCTPZQMsgQueue::Process()
{
	SMsgItem* pItem = NULL;
	if(m_queue.dequeue(pItem))
	{
		_Output(pItem);
		delete pItem;
		return true;
	}
	return false;
}

void CCTPZQMsgQueue::StartThread()
{
	if (NULL == m_hThread)
	{
		m_bRunning = true;
		m_hThread = CreateThread(NULL,0,ProcessThread,this,0,NULL); 
	}
}

void CCTPZQMsgQueue::StopThread()
{
	//停止线程
	m_bRunning = false;
	WaitForSingleObject(m_hThread,INFINITE);
	CloseHandle(m_hThread);
	m_hThread = NULL;
}

DWORD WINAPI ProcessThread(LPVOID lpParam)
{
	CCTPZQMsgQueue* pMsgQueue = reinterpret_cast<CCTPZQMsgQueue *>(lpParam);
	if (pMsgQueue)
		pMsgQueue->RunInThread();
	return 0;
}

void CCTPZQMsgQueue::RunInThread()
{
	m_nSleep = 1;
	while (m_bRunning)
	{
		if(Process())
		{
			//成功处理了一个
			m_nSleep = 1;
		}
		else
		{
			//失败表示队列为空，等待一会再来取为好
			m_nSleep *= 2;
			m_nSleep %= 255;//不超过N毫秒
			Sleep(m_nSleep);
		}
	}

	//清理线程
	CloseHandle(m_hThread);
	m_hThread = NULL;
	m_bRunning = false;
}

void CCTPZQMsgQueue::_Input(SMsgItem* pMsgItem)
{
	//由于只内部调用，所以不再检查指针是否有效
	m_queue.enqueue(pMsgItem);
}

void CCTPZQMsgQueue::_Output(SMsgItem* pMsgItem)
{
	//内部调用，不判断指针是否有效
	switch(pMsgItem->type)
	{
	case E_fnOnConnect:
		Output_OnConnect(pMsgItem);
		break;
	case E_fnOnDisconnect:
		Output_OnDisconnect(pMsgItem);
		break;
	case E_fnOnErrRtnOrderAction:
		Output_OnErrRtnOrderAction(pMsgItem);
		break;
	case E_fnOnErrRtnOrderInsert:
		Output_OnErrRtnOrderInsert(pMsgItem);
		break;
	case E_fnOnRspError:
		Output_OnRspError(pMsgItem);
		break;
	case E_fnOnRspOrderAction:
		Output_OnRspOrderAction(pMsgItem);
		break;
	case E_fnOnRspOrderInsert:
		Output_OnRspOrderInsert(pMsgItem);
		break;
	case E_fnOnRspQryDepthMarketData:
		Output_OnRspQryDepthMarketData(pMsgItem);
		break;
	case E_fnOnRspQryInstrument:
		Output_OnRspQryInstrument(pMsgItem);
		break;
	case E_fnOnRspQryInstrumentCommissionRate:
		Output_OnRspQryInstrumentCommissionRate(pMsgItem);
		break;
	case E_fnOnRspQryInstrumentMarginRate:
		Output_OnRspQryInstrumentMarginRate(pMsgItem);
		break;
	case E_fnOnRspQryInvestorPosition:
		Output_OnRspQryInvestorPosition(pMsgItem);
		break;
	case E_fnOnRspQryInvestorPositionDetail:
		Output_OnRspQryInvestorPositionDetail(pMsgItem);
		break;
	case E_fnOnRspQryOrder:
		Output_OnRspQryOrder(pMsgItem);
		break;
	case E_fnOnRspQryTrade:
		Output_OnRspQryTrade(pMsgItem);
		break;
	case E_fnOnRspQryTradingAccount:
		Output_OnRspQryTradingAccount(pMsgItem);
		break;
	case E_fnOnRtnDepthMarketData:
		Output_OnRtnDepthMarketData(pMsgItem);
		break;
	case E_fnOnRtnOrder:
		Output_OnRtnOrder(pMsgItem);
		break;
	case E_fnOnRtnTrade:
		Output_OnRtnTrade(pMsgItem);
		break;
	default:
		_ASSERT(false);
		break;
	}
}

void CCTPZQMsgQueue::Input_OnConnect(void* pApi,CZQThostFtdcRspUserLoginField *pRspUserLogin,ConnectionStatus result)
{
	SMsgItem* pItem = new SMsgItem;
	if(pItem)
	{
		memset(pItem,0,sizeof(SMsgItem));
		pItem->type = E_fnOnConnect;
		pItem->pApi = pApi;
		pItem->Status = result;

		if(pRspUserLogin)
			pItem->RspUserLogin = *pRspUserLogin;

		_Input(pItem);
	}
}

void CCTPZQMsgQueue::Input_OnDisconnect(void* pApi,CZQThostFtdcRspInfoField *pRspInfo,ConnectionStatus step)
{
	SMsgItem* pItem = new SMsgItem;
	if(pItem)
	{
		memset(pItem,0,sizeof(SMsgItem));
		pItem->type = E_fnOnDisconnect;
		pItem->pApi = pApi;
		pItem->Status = step;

		if(pRspInfo)
			pItem->RspInfo = *pRspInfo;

		_Input(pItem);
	}
}

void CCTPZQMsgQueue::Input_OnRspError(void* pApi,CZQThostFtdcRspInfoField* pRspInfo,int nRequestID,bool bIsLast)
{
	if(NULL==pRspInfo)
		return;

	SMsgItem* pItem = new SMsgItem;
	if(pItem)
	{
		pItem->type = E_fnOnRspError;
		pItem->pApi = pApi;
		pItem->nRequestID = nRequestID;
		pItem->bIsLast = bIsLast;
		pItem->RspInfo = *pRspInfo;

		_Input(pItem);
	}
}

void CCTPZQMsgQueue::Input_OnRtnDepthMarketData(void* pMdApi,CZQThostFtdcDepthMarketDataField *pDepthMarketData)
{
	if(NULL == pDepthMarketData)
		return;

	SMsgItem* pItem = new SMsgItem;
	if(pItem)
	{
		pItem->type = E_fnOnRtnDepthMarketData;
		pItem->pApi = pMdApi;
		pItem->DepthMarketData = *pDepthMarketData;

		_Input(pItem);
	}
}

void CCTPZQMsgQueue::Input_OnRtnOrder(void* pTraderApi,CZQThostFtdcOrderField *pOrder)
{
	if(NULL == pOrder)
		return;

	SMsgItem* pItem = new SMsgItem;
	if(pItem)
	{
		pItem->type = E_fnOnRtnOrder;
		pItem->pApi = pTraderApi;
		pItem->Order = *pOrder;

		_Input(pItem);
	}
}

void CCTPZQMsgQueue::Input_OnRtnTrade(void* pTraderApi,CZQThostFtdcTradeField *pTrade)
{
	if(NULL == pTrade)
		return;

	SMsgItem* pItem = new SMsgItem;
	if(pItem)
	{
		pItem->type = E_fnOnRtnTrade;
		pItem->pApi = pTraderApi;
		pItem->Trade = *pTrade;

		_Input(pItem);
	}
}

void CCTPZQMsgQueue::Input_OnRspOrderInsert(void* pTraderApi,CZQThostFtdcInputOrderField *pInputOrder, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(NULL == pInputOrder
		&&NULL == pRspInfo)
		return;

	SMsgItem* pItem = new SMsgItem;
	if(pItem)
	{
		memset(pItem,0,sizeof(SMsgItem));
		pItem->type = E_fnOnRspOrderInsert;
		pItem->pApi = pTraderApi;
		pItem->nRequestID = nRequestID;
		pItem->bIsLast = bIsLast;

		if(pInputOrder)
			pItem->InputOrder = *pInputOrder;
		if(pRspInfo)
			pItem->RspInfo = *pRspInfo;

		_Input(pItem);
	}
}

void CCTPZQMsgQueue::Input_OnRspQryInstrument(void* pTraderApi,CZQThostFtdcInstrumentField *pInstrument, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(NULL == pInstrument
		&&NULL == pRspInfo)
		return;

	SMsgItem* pItem = new SMsgItem;
	if(pItem)
	{
		memset(pItem,0,sizeof(SMsgItem));
		pItem->type = E_fnOnRspQryInstrument;
		pItem->pApi = pTraderApi;
		pItem->nRequestID = nRequestID;
		pItem->bIsLast = bIsLast;

		if(pInstrument)
			pItem->Instrument = *pInstrument;
		if(pRspInfo)
			pItem->RspInfo = *pRspInfo;

		_Input(pItem);
	}
}

void CCTPZQMsgQueue::Input_OnRspQryInstrumentMarginRate(void* pTraderApi,CZQThostFtdcInstrumentMarginRateField *pInstrumentMarginRate, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(NULL == pInstrumentMarginRate
		&&NULL == pRspInfo)
		return;

	SMsgItem* pItem = new SMsgItem;
	if(pItem)
	{
		memset(pItem,0,sizeof(SMsgItem));
		pItem->type = E_fnOnRspQryInstrumentMarginRate;
		pItem->pApi = pTraderApi;
		pItem->nRequestID = nRequestID;
		pItem->bIsLast = bIsLast;

		if(pInstrumentMarginRate)
			pItem->InstrumentMarginRate = *pInstrumentMarginRate;
		if(pRspInfo)
			pItem->RspInfo = *pRspInfo;

		_Input(pItem);
	}
}

void CCTPZQMsgQueue::Input_OnRspQryInstrumentCommissionRate(void* pTraderApi,CZQThostFtdcInstrumentCommissionRateField *pInstrumentCommissionRate, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(NULL == pInstrumentCommissionRate
		&&NULL == pRspInfo)
		return;

	SMsgItem* pItem = new SMsgItem;
	if(pItem)
	{
		memset(pItem,0,sizeof(SMsgItem));
		pItem->type = E_fnOnRspQryInstrumentCommissionRate;
		pItem->pApi = pTraderApi;
		pItem->nRequestID = nRequestID;
		pItem->bIsLast = bIsLast;

		if(pInstrumentCommissionRate)
			pItem->InstrumentCommissionRate = *pInstrumentCommissionRate;
		if(pRspInfo)
			pItem->RspInfo = *pRspInfo;

		_Input(pItem);
	}
}

void CCTPZQMsgQueue::Input_OnRspQryInvestorPosition(void* pTraderApi,CZQThostFtdcInvestorPositionField *pInvestorPosition, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(NULL == pInvestorPosition
		&&NULL == pRspInfo)
		return;

	SMsgItem* pItem = new SMsgItem;
	if(pItem)
	{
		memset(pItem,0,sizeof(SMsgItem));
		pItem->type = E_fnOnRspQryInvestorPosition;
		pItem->pApi = pTraderApi;
		pItem->nRequestID = nRequestID;
		pItem->bIsLast = bIsLast;

		if(pInvestorPosition)
			pItem->InvestorPosition = *pInvestorPosition;
		if(pRspInfo)
			pItem->RspInfo = *pRspInfo;

		_Input(pItem);
	}
}

void CCTPZQMsgQueue::Input_OnRspQryInvestorPositionDetail(void* pTraderApi,CZQThostFtdcInvestorPositionDetailField *pInvestorPositionDetail, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(NULL == pInvestorPositionDetail
		&&NULL == pRspInfo)
		return;

	SMsgItem* pItem = new SMsgItem;
	if(pItem)
	{
		memset(pItem,0,sizeof(SMsgItem));
		pItem->type = E_fnOnRspQryInvestorPositionDetail;
		pItem->pApi = pTraderApi;
		pItem->nRequestID = nRequestID;
		pItem->bIsLast = bIsLast;

		if(pInvestorPositionDetail)
			pItem->InvestorPositionDetail = *pInvestorPositionDetail;
		if(pRspInfo)
			pItem->RspInfo = *pRspInfo;

		_Input(pItem);
	}
}

void CCTPZQMsgQueue::Input_OnErrRtnOrderInsert(void* pTraderApi,CZQThostFtdcInputOrderField *pInputOrder, CZQThostFtdcRspInfoField *pRspInfo)
{
	if(NULL == pInputOrder
		&&NULL == pRspInfo)
		return;

	SMsgItem* pItem = new SMsgItem;
	if(pItem)
	{
		memset(pItem,0,sizeof(SMsgItem));
		pItem->type = E_fnOnErrRtnOrderInsert;
		pItem->pApi = pTraderApi;
		if(pInputOrder)
			pItem->InputOrder = *pInputOrder;
		if(pRspInfo)
			pItem->RspInfo = *pRspInfo;

		_Input(pItem);
	}
}

void CCTPZQMsgQueue::Input_OnRspOrderAction(void* pTraderApi,CZQThostFtdcInputOrderActionField *pInputOrderAction, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(NULL == pInputOrderAction
		&&NULL == pRspInfo)
		return;

	SMsgItem* pItem = new SMsgItem;
	if(pItem)
	{
		memset(pItem,0,sizeof(SMsgItem));
		pItem->type = E_fnOnRspOrderAction;
		pItem->pApi = pTraderApi;
		pItem->nRequestID = nRequestID;
		pItem->bIsLast = bIsLast;

		if(pInputOrderAction)
			pItem->InputOrderAction = *pInputOrderAction;
		if(pRspInfo)
			pItem->RspInfo = *pRspInfo;

		_Input(pItem);
	}
}

void CCTPZQMsgQueue::Input_OnErrRtnOrderAction(void* pTraderApi,CZQThostFtdcOrderActionField *pOrderAction, CZQThostFtdcRspInfoField *pRspInfo)
{
	if(NULL == pOrderAction
		&&NULL == pRspInfo)
		return;

	SMsgItem* pItem = new SMsgItem;
	if(pItem)
	{
		memset(pItem,0,sizeof(SMsgItem));
		pItem->type = E_fnOnErrRtnOrderAction;
		pItem->pApi = pTraderApi;
		if(pOrderAction)
			pItem->OrderAction = *pOrderAction;
		if(pRspInfo)
			pItem->RspInfo = *pRspInfo;

		_Input(pItem);
	}
}

void CCTPZQMsgQueue::Input_OnRspQryOrder(void* pTraderApi,CZQThostFtdcOrderField *pOrder, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(NULL == pOrder
		&&NULL == pRspInfo)
		return;

	SMsgItem* pItem = new SMsgItem;
	if(pItem)
	{
		memset(pItem,0,sizeof(SMsgItem));
		pItem->type = E_fnOnRspQryOrder;
		pItem->pApi = pTraderApi;
		pItem->nRequestID = nRequestID;
		pItem->bIsLast = bIsLast;

		if(pOrder)
			pItem->Order = *pOrder;
		if(pRspInfo)
			pItem->RspInfo = *pRspInfo;


		_Input(pItem);
	}
}

void CCTPZQMsgQueue::Input_OnRspQryTrade(void* pTraderApi,CZQThostFtdcTradeField *pTrade, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(NULL == pTrade
		&&NULL == pRspInfo)
		return;

	SMsgItem* pItem = new SMsgItem;
	if(pItem)
	{
		memset(pItem,0,sizeof(SMsgItem));
		pItem->type = E_fnOnRspQryTrade;
		pItem->pApi = pTraderApi;
		pItem->nRequestID = nRequestID;
		pItem->bIsLast = bIsLast;
		if(pTrade)
			pItem->Trade = *pTrade;
		if(pRspInfo)
			pItem->RspInfo = *pRspInfo;

		_Input(pItem);
	}
}

void CCTPZQMsgQueue::Input_OnRspQryTradingAccount(void* pTraderApi,CZQThostFtdcTradingAccountField *pTradingAccount, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(NULL == pTradingAccount
		&&NULL == pRspInfo)
		return;

	SMsgItem* pItem = new SMsgItem;
	if(pItem)
	{
		memset(pItem,0,sizeof(SMsgItem));
		pItem->type = E_fnOnRspQryTradingAccount;
		pItem->pApi = pTraderApi;
		pItem->nRequestID = nRequestID;
		pItem->bIsLast = bIsLast;
		if(pTradingAccount)
			pItem->TradingAccount = *pTradingAccount;
		if(pRspInfo)
			pItem->RspInfo = *pRspInfo;

		_Input(pItem);
	}
}

void CCTPZQMsgQueue::Input_OnRspQryDepthMarketData(void* pTraderApi,CZQThostFtdcDepthMarketDataField *pDepthMarketData, CZQThostFtdcRspInfoField *pRspInfo, int nRequestID, bool bIsLast)
{
	if(NULL == pDepthMarketData
		&&NULL == pRspInfo)
		return;

	SMsgItem* pItem = new SMsgItem;
	if(pItem)
	{
		memset(pItem,0,sizeof(SMsgItem));
		pItem->type = E_fnOnRspQryDepthMarketData;
		pItem->pApi = pTraderApi;
		pItem->nRequestID = nRequestID;
		pItem->bIsLast = bIsLast;
		if(pDepthMarketData)
			pItem->DepthMarketData = *pDepthMarketData;
		if(pRspInfo)
			pItem->RspInfo = *pRspInfo;

		_Input(pItem);
	}
}