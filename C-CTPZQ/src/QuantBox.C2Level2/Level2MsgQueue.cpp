#include "StdAfx.h"
#include "Level2MsgQueue.h"

void CLevel2MsgQueue::Clear()
{
	SMsgItem* pItem = NULL;
	//清空队列
	while(m_queue.dequeue(pItem))
	{
		_Output(pItem);
		delete pItem;
	}
}

bool CLevel2MsgQueue::Process()
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

void CLevel2MsgQueue::StartThread()
{
	if (NULL == m_hThread)
	{
		m_bRunning = true;
		m_hThread = CreateThread(NULL,0,ProcessThread,this,0,NULL); 
	}
}

void CLevel2MsgQueue::StopThread()
{
	//停止线程
	m_bRunning = false;
	WaitForSingleObject(m_hThread,INFINITE);
	CloseHandle(m_hThread);
	m_hThread = NULL;
}

DWORD WINAPI ProcessThread(LPVOID lpParam)
{
	CLevel2MsgQueue* pMsgQueue = reinterpret_cast<CLevel2MsgQueue *>(lpParam);
	if (pMsgQueue)
		pMsgQueue->RunInThread();
	return 0;
}

void CLevel2MsgQueue::RunInThread()
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

void CLevel2MsgQueue::_Input(SMsgItem* pMsgItem)
{
	//由于只内部调用，所以不再检查指针是否有效
	m_queue.enqueue(pMsgItem);
}

void CLevel2MsgQueue::_Output(SMsgItem* pMsgItem)
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
	case E_fnOnRspError:
		Output_OnRspError(pMsgItem);
		break;
	case E_fnOnRtnLevel2MarketData:
		Output_OnRtnLevel2MarketData(pMsgItem);
		break;
	case E_fnOnRtnNGTSIndex:
		Output_OnRtnNGTSIndex(pMsgItem);
		break;
	default:
		_ASSERT(false);
		break;
	}
}

void CLevel2MsgQueue::Input_OnConnect(void* pApi,CThostFtdcRspUserLoginField *pRspUserLogin,ConnectionStatus result)
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

void CLevel2MsgQueue::Input_OnDisconnect(void* pApi,CThostFtdcRspInfoField *pRspInfo,ConnectionStatus step)
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

void CLevel2MsgQueue::Input_OnRspError(void* pApi,CThostFtdcRspInfoField* pRspInfo,int nRequestID,bool bIsLast)
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

void CLevel2MsgQueue::Input_OnRtnLevel2MarketData(void* pLevel2Api,CThostFtdcLevel2MarketDataField *pLevel2MarketData)
{
	if(NULL == pLevel2MarketData)
		return;

	SMsgItem* pItem = new SMsgItem;
	if(pItem)
	{
		pItem->type = E_fnOnRtnLevel2MarketData;
		pItem->pApi = pLevel2Api;
		pItem->Level2MarketData = *pLevel2MarketData;

		_Input(pItem);
	}
}

void CLevel2MsgQueue::Input_OnRtnNGTSIndex(void* pLevel2Api,CThostFtdcNGTSIndexField *pNGTSIndex)
{
	if(NULL == pNGTSIndex)
		return;

	SMsgItem* pItem = new SMsgItem;
	if(pItem)
	{
		pItem->type = E_fnOnRtnNGTSIndex;
		pItem->pApi = pNGTSIndex;
		pItem->NGTSIndex = *pNGTSIndex;

		_Input(pItem);
	}
}