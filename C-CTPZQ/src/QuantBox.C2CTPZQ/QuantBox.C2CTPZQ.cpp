// QuantBox.C2CTP.cpp : ���� DLL Ӧ�ó���ĵ���������
//

#include "stdafx.h"
#include "QuantBox.C2CTPZQ.h"

#include "MdUserApi.h"
#include "TraderApi.h"
#include "CTPZQMsgQueue.h"

inline CCTPZQMsgQueue* CTP_GetQueue(void* pMsgQueue)
{
	return static_cast<CCTPZQMsgQueue*>(pMsgQueue);
}

inline CMdUserApi* MD_GetApi(void* pMdUserApi)
{
	return static_cast<CMdUserApi*>(pMdUserApi);
}

inline CTraderApi* TD_GetApi(void* pTraderApi)
{
	return static_cast<CTraderApi*>(pTraderApi);
}


QUANTBOXC2CTPZQ_API void* __stdcall CTP_CreateMsgQueue()
{
	return new CCTPZQMsgQueue();
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_ReleaseMsgQueue(void* pMsgQueue)
{
	if(pMsgQueue)
	{
		delete CTP_GetQueue(pMsgQueue);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnConnect(void* pMsgQueue,fnOnConnect pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnDisconnect(void* pMsgQueue,fnOnDisconnect pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnErrRtnOrderAction(void* pMsgQueue,fnOnErrRtnOrderAction pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnErrRtnOrderInsert(void* pMsgQueue,fnOnErrRtnOrderInsert pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspError(void* pMsgQueue,fnOnRspError pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspOrderAction(void* pMsgQueue,fnOnRspOrderAction pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspOrderInsert(void* pMsgQueue,fnOnRspOrderInsert pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspQryDepthMarketData(void* pMsgQueue,fnOnRspQryDepthMarketData pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspQryInstrument(void* pMsgQueue,fnOnRspQryInstrument pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspQryInstrumentCommissionRate(void* pMsgQueue,fnOnRspQryInstrumentCommissionRate pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspQryInstrumentMarginRate(void* pMsgQueue,fnOnRspQryInstrumentMarginRate pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspQryInvestorPosition(void* pMsgQueue,fnOnRspQryInvestorPosition pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspQryInvestorPositionDetail(void* pMsgQueue,fnOnRspQryInvestorPositionDetail pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspQryOrder(void* pMsgQueue,fnOnRspQryOrder pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspQryTrade(void* pMsgQueue,fnOnRspQryTrade pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRspQryTradingAccount(void* pMsgQueue,fnOnRspQryTradingAccount pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRtnDepthMarketData(void* pMsgQueue,fnOnRtnDepthMarketData pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRtnInstrumentStatus(void* pMsgQueue,fnOnRtnInstrumentStatus pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRtnOrder(void* pMsgQueue,fnOnRtnOrder pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_RegOnRtnTrade(void* pMsgQueue,fnOnRtnTrade pCallback)
{
	if(pMsgQueue)
	{
		CTP_GetQueue(pMsgQueue)->RegisterCallback(pCallback);
	}
}

QUANTBOXC2CTPZQ_API bool __stdcall CTP_ProcessMsgQueue(void* pMsgQueue)
{
	if(pMsgQueue)
	{
		return CTP_GetQueue(pMsgQueue)->Process();
	}
	return false;
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_ClearMsgQueue(void* pMsgQueue)
{
	if(pMsgQueue)
	{
		return CTP_GetQueue(pMsgQueue)->Clear();
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_StartMsgQueue(void* pMsgQueue)
{
	if(pMsgQueue)
	{
		return CTP_GetQueue(pMsgQueue)->StartThread();
	}
}

QUANTBOXC2CTPZQ_API void __stdcall CTP_StopMsgQueue(void* pMsgQueue)
{
	if(pMsgQueue)
	{
		return CTP_GetQueue(pMsgQueue)->StopThread();
	}
}

QUANTBOXC2CTPZQ_API void* __stdcall MD_CreateMdApi()
{
	return new CMdUserApi();
}

QUANTBOXC2CTPZQ_API void __stdcall MD_RegMsgQueue2MdApi(void* pMdUserApi,void* pMsgQueue)
{
	if(pMdUserApi)
	{
		MD_GetApi(pMdUserApi)->RegisterMsgQueue((CCTPZQMsgQueue*)pMsgQueue);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall MD_Connect(void* pMdUserApi,
	const char* szPath,
	const char* szAddresses,
	const char* szBrokerId,
	const char* szInvestorId,
	const char* szPassword)
{
	if(pMdUserApi
		&&szPath
		&&szAddresses
		&&szBrokerId
		&&szInvestorId
		&&szPassword)
	{
		MD_GetApi(pMdUserApi)->Connect(szPath,szAddresses,szBrokerId,szInvestorId,szPassword);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall MD_Disconnect(void* pMdUserApi)
{
	if(pMdUserApi)
	{
		MD_GetApi(pMdUserApi)->Disconnect();
	}
}

QUANTBOXC2CTPZQ_API void __stdcall MD_Subscribe(void* pMdUserApi,const char* szInstrumentIDs,const char* szExchageID)
{
	if(pMdUserApi
		&&szInstrumentIDs)
	{
		MD_GetApi(pMdUserApi)->Subscribe(szInstrumentIDs,szExchageID);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall MD_Unsubscribe(void* pMdUserApi,const char* szInstrumentIDs,const char* szExchageID)
{
	if(pMdUserApi
		&&szInstrumentIDs)
	{
		MD_GetApi(pMdUserApi)->Unsubscribe(szInstrumentIDs,szExchageID);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall MD_ReleaseMdApi(void* pMdUserApi)
{
	if(pMdUserApi)
	{
		delete MD_GetApi(pMdUserApi);
	}
}

QUANTBOXC2CTPZQ_API void* __stdcall TD_CreateTdApi()
{
	return new CTraderApi();
}

QUANTBOXC2CTPZQ_API void __stdcall TD_RegMsgQueue2TdApi(void* pTraderApi,void* pMsgQueue)
{
	if(pTraderApi)
	{
		TD_GetApi(pTraderApi)->RegisterMsgQueue((CCTPZQMsgQueue*)pMsgQueue);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall TD_Connect(
	void* pTraderApi,
	const char* szPath,
	const char* szAddresses,
	const char* szBrokerId,
	const char* szInvestorId,
	const char* szPassword,
	ZQTHOST_TE_RESUME_TYPE nResumeType,
	const char* szUserProductInfo,
	const char* szAuthCode)
{
	if(pTraderApi
		&&szPath
		&&szAddresses
		&&szBrokerId
		&&szInvestorId
		&&szPassword)
	{
		if(szUserProductInfo&&szAuthCode)
			TD_GetApi(pTraderApi)->Connect(szPath,szAddresses,szBrokerId,szInvestorId,szPassword,nResumeType,szUserProductInfo,szAuthCode);
		else
			TD_GetApi(pTraderApi)->Connect(szPath,szAddresses,szBrokerId,szInvestorId,szPassword,nResumeType,"","");
	}
}

QUANTBOXC2CTPZQ_API int __stdcall TD_SendOrder(
	void* pTraderApi,
	int OrderRef,
	const char* szInstrument,
	const char* szExchange,
	TZQThostFtdcDirectionType Direction,
	const char* szCombOffsetFlag,
	const char* szCombHedgeFlag,
	TZQThostFtdcVolumeType VolumeTotalOriginal,
	double LimitPrice,
	TZQThostFtdcOrderPriceTypeType OrderPriceType,
	TZQThostFtdcTimeConditionType TimeCondition,
	TZQThostFtdcContingentConditionType ContingentCondition,
	double StopPrice,
	TZQThostFtdcVolumeConditionType VolumeCondition)
{
	if(pTraderApi
		&&szInstrument
		&&szCombOffsetFlag
		&&szCombHedgeFlag)
	{
		TZQThostFtdcStockPriceType price;
		sprintf(price, "%f", LimitPrice);

		return TD_GetApi(pTraderApi)->ReqOrderInsert(
			OrderRef,
			szInstrument,
			szExchange,
			Direction,
			szCombOffsetFlag,
			szCombHedgeFlag,
			VolumeTotalOriginal,
			price,
			OrderPriceType,
			TimeCondition,
			ContingentCondition,
			StopPrice,
			VolumeCondition);
	}
	return 0;
}

QUANTBOXC2CTPZQ_API void __stdcall TD_CancelOrder(void* pTraderApi,CZQThostFtdcOrderField *pOrder)
{
	if(pTraderApi)
	{
		TD_GetApi(pTraderApi)->ReqOrderAction(pOrder);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall TD_Disconnect(void* pTraderApi)
{
	if(pTraderApi)
	{
		TD_GetApi(pTraderApi)->Disconnect();
	}
}

QUANTBOXC2CTPZQ_API void __stdcall TD_ReleaseTdApi(void* pTraderApi)
{
	if(pTraderApi)
	{
		delete TD_GetApi(pTraderApi);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall TD_ReqQryInvestorPosition(void* pTraderApi,const char* szInstrumentId)
{
	if(pTraderApi)
	{
		TD_GetApi(pTraderApi)->ReqQryInvestorPosition(NULL==szInstrumentId?"":szInstrumentId);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall TD_ReqQryInvestorPositionDetail(void* pTraderApi,const char* szInstrumentId)
{
	if(pTraderApi)
	{
		TD_GetApi(pTraderApi)->ReqQryInvestorPositionDetail(NULL==szInstrumentId?"":szInstrumentId);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall TD_ReqQryTradingAccount(void* pTraderApi)
{
	if(pTraderApi)
	{
		TD_GetApi(pTraderApi)->ReqQryTradingAccount();
	}
}

QUANTBOXC2CTPZQ_API void __stdcall TD_ReqQryInstrument(void* pTraderApi,const char* szInstrumentId)
{
	if(pTraderApi)
	{
		TD_GetApi(pTraderApi)->ReqQryInstrument(NULL==szInstrumentId?"":szInstrumentId);
	}
}

QUANTBOXC2CTPZQ_API void __stdcall TD_ReqQryInstrumentCommissionRate(void* pTraderApi,const char* szInstrumentId)
{
	if(pTraderApi)
	{
		TD_GetApi(pTraderApi)->ReqQryInstrumentCommissionRate(szInstrumentId);
	}
}

//QUANTBOXC2CTPZQ_API void __stdcall TD_ReqQryInstrumentMarginRate(void* pTraderApi,const char* szInstrumentId)
//{
//	if(pTraderApi)
//	{
//		TD_GetApi(pTraderApi)->ReqQryInstrumentMarginRate(szInstrumentId);
//	}
//}

QUANTBOXC2CTPZQ_API void __stdcall TD_ReqQryDepthMarketData(void* pTraderApi,const char* szInstrumentId)
{
	if(pTraderApi)
	{
		TD_GetApi(pTraderApi)->ReqQryDepthMarketData(szInstrumentId);
	}
}