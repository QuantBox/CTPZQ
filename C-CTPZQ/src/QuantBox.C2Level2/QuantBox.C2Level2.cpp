// QuantBox.C2Level2.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "QuantBox.C2Level2.h"

#include "Level2UserApi.h"
#include "Level2MsgQueue.h"

inline CLevel2MsgQueue* CTP_GetQueue(void* pMsgQueue)
{
	return static_cast<CLevel2MsgQueue*>(pMsgQueue);
}

inline CLevel2UserApi* L2_GetApi(void* pLevel2Api)
{
	return static_cast<CLevel2UserApi*>(pLevel2Api);
}

QUANTBOXC2LEVEL2_API void* __stdcall CTP_CreateMsgQueue()
{
	return new CLevel2MsgQueue();
}

QUANTBOXC2LEVEL2_API void __stdcall CTP_ReleaseMsgQueue(void* pMsgQueue)
{
	if(pMsgQueue)
	{
		delete CTP_GetQueue(pMsgQueue);
	}
}

QUANTBOXC2LEVEL2_API void __stdcall CTP_RegOnConnect(void* pMsgQueue,fnOnConnect pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2LEVEL2_API void __stdcall CTP_RegOnDisconnect(void* pMsgQueue,fnOnDisconnect pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2LEVEL2_API void __stdcall CTP_RegOnRspError(void* pMsgQueue,fnOnRspError pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2LEVEL2_API void __stdcall CTP_RegOnRtnLevel2MarketData(void* pMsgQueue,fnOnRtnLevel2MarketData pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2LEVEL2_API void __stdcall CTP_RegOnRtnNGTSIndex(void* pMsgQueue,fnOnRtnNGTSIndex pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2LEVEL2_API bool __stdcall CTP_ProcessMsgQueue(void* pMsgQueue)
{
	if(pMsgQueue)
	{
		return CTP_GetQueue(pMsgQueue)->Process();
	}
	return false;
}

QUANTBOXC2LEVEL2_API void __stdcall CTP_ClearMsgQueue(void* pMsgQueue)
{
	if(pMsgQueue)
	{
		return CTP_GetQueue(pMsgQueue)->Clear();
	}
}

QUANTBOXC2LEVEL2_API void __stdcall CTP_StartMsgQueue(void* pMsgQueue)
{
	if(pMsgQueue)
	{
		return CTP_GetQueue(pMsgQueue)->StartThread();
	}
}

QUANTBOXC2LEVEL2_API void __stdcall CTP_StopMsgQueue(void* pMsgQueue)
{
	if(pMsgQueue)
	{
		return CTP_GetQueue(pMsgQueue)->StopThread();
	}
}

QUANTBOXC2LEVEL2_API void* __stdcall L2_CreateLevel2Api()
{
	return new CLevel2UserApi();
}

QUANTBOXC2LEVEL2_API void __stdcall L2_RegMsgQueue2Level2Api(void* pLevel2Api,void* pMsgQueue)
{
	if(pLevel2Api)
	{
		L2_GetApi(pLevel2Api)->RegisterMsgQueue((CLevel2MsgQueue*)pMsgQueue);
	}
}

QUANTBOXC2LEVEL2_API void __stdcall L2_Connect(void* pLevel2Api,
	const char* szPath,
	const char* szAddresses,
	const char* szBrokerId,
	const char* szInvestorId,
	const char* szPassword)
{
	if(pLevel2Api
		&&szPath
		&&szAddresses
		&&szBrokerId
		&&szInvestorId
		&&szPassword)
	{
		L2_GetApi(pLevel2Api)->Connect(szPath,szAddresses,szBrokerId,szInvestorId,szPassword);
	}
}

QUANTBOXC2LEVEL2_API void __stdcall L2_Disconnect(void* pLevel2Api)
{
	if(pLevel2Api)
	{
		L2_GetApi(pLevel2Api)->Disconnect();
	}
}

QUANTBOXC2LEVEL2_API void __stdcall L2_SubscribeLevel2MarketData(void* pLevel2Api,const char* szInstrumentIDs,const char* szExchageID)
{
	if(pLevel2Api
		&&szInstrumentIDs)
	{
		L2_GetApi(pLevel2Api)->SubscribeLevel2MarketData(szInstrumentIDs,szExchageID);
	}
}

QUANTBOXC2LEVEL2_API void __stdcall L2_SubscribeNGTSIndex(void* pLevel2Api,const char* szInstrumentIDs,const char* szExchageID)
{
	if(pLevel2Api
		&&szInstrumentIDs)
	{
		L2_GetApi(pLevel2Api)->SubscribeNGTSIndex(szInstrumentIDs,szExchageID);
	}
}

QUANTBOXC2LEVEL2_API void __stdcall L2_UnSubscribeLevel2MarketData(void* pLevel2Api,const char* szInstrumentIDs,const char* szExchageID)
{
	if(pLevel2Api
		&&szInstrumentIDs)
	{
		L2_GetApi(pLevel2Api)->UnSubscribeLevel2MarketData(szInstrumentIDs,szExchageID);
	}
}

QUANTBOXC2LEVEL2_API void __stdcall L2_UnSubscribeNGTSIndex(void* pLevel2Api,const char* szInstrumentIDs,const char* szExchageID)
{
	if(pLevel2Api
		&&szInstrumentIDs)
	{
		L2_GetApi(pLevel2Api)->UnSubscribeNGTSIndex(szInstrumentIDs,szExchageID);
	}
}

QUANTBOXC2LEVEL2_API void __stdcall L2_ReleaseLevel2Api(void* pLevel2Api)
{
	if(pLevel2Api)
	{
		delete L2_GetApi(pLevel2Api);
	}
}