#pragma once

#include "QuantBox.C2Level2.h"
#include "include\LockFreeQ.h"

class CLevel2MsgQueue
{
	//响应队列中可能出现的消息类型（按字母排序）
	enum EnumMsgType
	{
		E_fnOnConnect,	
		E_fnOnDisconnect,
		E_fnOnRspError,
		E_fnOnRtnLevel2MarketData,
		E_fnOnRtnNGTSIndex,
	};

	struct SMsgItem
	{
		EnumMsgType							type;			//消息类型
		void*								pApi;			//指向类对象的指针
		CThostFtdcRspInfoField				RspInfo;		//响应信息
		bool								bIsLast;		//是否最后一个包
		union{
			int								nRequestID;
			ConnectionStatus				Status;
		};
		union{
			CThostFtdcLevel2MarketDataField			Level2MarketData;
			CThostFtdcNGTSIndexField				NGTSIndex;
			CThostFtdcRspUserLoginField				RspUserLogin;
		};
	};

public:
	CLevel2MsgQueue(void)
	{
		m_nSleep = 1;
		m_hThread = NULL;
		m_bRunning = false;

		//回调函数地址指针
		m_fnOnConnect = NULL;
		m_fnOnDisconnect = NULL;
		m_fnOnRspError = NULL;
		m_fnOnRtnLevel2MarketData = NULL;
		m_fnOnRtnNGTSIndex = NULL;
	}
	virtual ~CLevel2MsgQueue(void)
	{
		StopThread();
		Clear();
	}

public:
	//清空队列
	void Clear();

	//可以由外部发起，顺序处理队列触发回调函数
	bool Process();

	//由内部启动线程，内部主动调用Process触发回调
	void StartThread();
	void StopThread();

	//将外部的函数地址注册到队列(按字母排序)
	void RegisterCallback(fnOnConnect pCallback){m_fnOnConnect = pCallback;}
	void RegisterCallback(fnOnDisconnect pCallback){m_fnOnDisconnect = pCallback;}
	void RegisterCallback(fnOnRspError pCallback){m_fnOnRspError = pCallback;}
	void RegisterCallback(fnOnRtnLevel2MarketData pCallback){m_fnOnRtnLevel2MarketData = pCallback;}
	void RegisterCallback(fnOnRtnNGTSIndex pCallback){m_fnOnRtnNGTSIndex = pCallback;}

	//响应结果处理后入队列(按字母排序)
	void Input_OnConnect(void* pApi,CThostFtdcRspUserLoginField *pRspUserLogin,ConnectionStatus result);
	void Input_OnDisconnect(void* pApi,CThostFtdcRspInfoField *pRspInfo,ConnectionStatus step);
	void Input_OnRspError(void* pApi,CThostFtdcRspInfoField* pRspInfo,int nRequestID,bool bIsLast);
	void Input_OnRtnLevel2MarketData(void* pLevel2Api,CThostFtdcLevel2MarketDataField *pLevel2MarketData);
	void Input_OnRtnNGTSIndex(void* pLevel2Api,CThostFtdcNGTSIndexField *pNGTSIndex);
private:
	friend DWORD WINAPI ProcessThread(LPVOID lpParam);
	void RunInThread();

	//响应结果直接入队列
	void _Input(SMsgItem* pMsgItem);
	//队列中的消息判断分发
	void _Output(SMsgItem* pMsgItem);

	//响应输出
	void Output_OnConnect(SMsgItem* pItem)
	{
		if(m_fnOnConnect)
			(*m_fnOnConnect)(pItem->pApi,&pItem->RspUserLogin,pItem->Status);
	}
	void Output_OnDisconnect(SMsgItem* pItem)
	{
		if(m_fnOnDisconnect)
			(*m_fnOnDisconnect)(pItem->pApi,&pItem->RspInfo,pItem->Status);
	}
	void Output_OnRspError(SMsgItem* pItem)
	{
		if(m_fnOnRspError)
			(*m_fnOnRspError)(pItem->pApi,&pItem->RspInfo,pItem->nRequestID,pItem->bIsLast);
	}
	void Output_OnRtnLevel2MarketData(SMsgItem* pItem)
	{
		if(m_fnOnRtnLevel2MarketData)
			(*m_fnOnRtnLevel2MarketData)(pItem->pApi,&pItem->Level2MarketData);
	}
	void Output_OnRtnNGTSIndex(SMsgItem* pItem)
	{
		if(m_fnOnRtnNGTSIndex)
			(*m_fnOnRtnNGTSIndex)(pItem->pApi,&pItem->NGTSIndex);
	}
private:
	int							m_nSleep;
	bool						m_bRunning;
	HANDLE						m_hThread;

	MSQueue<SMsgItem*>			m_queue;			//响应队列

	//回调函数指针（按字母排序）
	fnOnConnect							m_fnOnConnect;
	fnOnDisconnect						m_fnOnDisconnect;
	fnOnRspError						m_fnOnRspError;
	fnOnRtnLevel2MarketData				m_fnOnRtnLevel2MarketData;
	fnOnRtnNGTSIndex					m_fnOnRtnNGTSIndex;
};

