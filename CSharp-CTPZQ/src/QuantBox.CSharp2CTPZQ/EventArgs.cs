using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace QuantBox.CSharp2CTPZQ
{
    //通用
    public class OnConnectArgs : EventArgs
    {
        public readonly IntPtr pApi;
        public readonly CZQThostFtdcRspUserLoginField pRspUserLogin;
        public readonly ConnectionStatus result;
        public OnConnectArgs(IntPtr pApi, ref CZQThostFtdcRspUserLoginField pRspUserLogin, ConnectionStatus result)
        {
            this.pApi = pApi;
            this.pRspUserLogin = pRspUserLogin;
            this.result = result;
        }
    }

    public class OnDisconnectArgs : EventArgs
    {
        public readonly IntPtr pApi;
        public readonly CZQThostFtdcRspInfoField pRspInfo;
        public readonly ConnectionStatus step;
        public OnDisconnectArgs(IntPtr pApi, ref CZQThostFtdcRspInfoField pRspInfo, ConnectionStatus step)
        {
            this.pApi = pApi;
            this.pRspInfo = pRspInfo;
            this.step = step;
        }
    }

    public class OnRspErrorArgs : EventArgs
    {
        public readonly IntPtr pApi;
        public readonly CZQThostFtdcRspInfoField pRspInfo;
        public readonly int nRequestID;
        public readonly bool bIsLast;
        public OnRspErrorArgs(IntPtr pApi, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            this.pApi = pApi;
            this.pRspInfo = pRspInfo;
            this.nRequestID = nRequestID;
            this.bIsLast = bIsLast;
        }
    }


    //行情
    public class OnRtnDepthMarketDataArgs : EventArgs
    {
        public readonly IntPtr pMdUserApi;
        public readonly CZQThostFtdcDepthMarketDataField pDepthMarketData;
        public OnRtnDepthMarketDataArgs(IntPtr pMdUserApi, ref CZQThostFtdcDepthMarketDataField pDepthMarketData)
        {
            this.pMdUserApi = pMdUserApi;
            this.pDepthMarketData = pDepthMarketData;
        }
    }

    //交易
    public class OnErrRtnOrderActionArgs : EventArgs
    {
        public readonly IntPtr pTraderApi;
        public readonly CZQThostFtdcOrderActionField pOrderAction;
        public readonly CZQThostFtdcRspInfoField pRspInfo;
        public OnErrRtnOrderActionArgs(IntPtr pTraderApi, ref CZQThostFtdcOrderActionField pOrderAction, ref CZQThostFtdcRspInfoField pRspInfo)
        {
            this.pTraderApi = pTraderApi;
            this.pOrderAction = pOrderAction;
            this.pRspInfo = pRspInfo;
        }
    }

    public class OnErrRtnOrderInsertArgs : EventArgs
    {
        public readonly IntPtr pTraderApi;
        public readonly CZQThostFtdcInputOrderField pInputOrder;
        public readonly CZQThostFtdcRspInfoField pRspInfo;
        public OnErrRtnOrderInsertArgs(IntPtr pTraderApi, ref CZQThostFtdcInputOrderField pInputOrder, ref CZQThostFtdcRspInfoField pRspInfo)
        {
            this.pTraderApi = pTraderApi;
            this.pInputOrder = pInputOrder;
            this.pRspInfo = pRspInfo;
        }
    }

    public class OnRspOrderActionArgs : EventArgs
    {
        public readonly IntPtr pTraderApi;
        public readonly CZQThostFtdcInputOrderActionField pInputOrderAction;
        public readonly CZQThostFtdcRspInfoField pRspInfo;
        public readonly int nRequestID;
        public readonly bool bIsLast;
        public OnRspOrderActionArgs(IntPtr pTraderApi, ref CZQThostFtdcInputOrderActionField pInputOrderAction, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            this.pTraderApi = pTraderApi;
            this.pInputOrderAction = pInputOrderAction;
            this.pRspInfo = pRspInfo;
            this.nRequestID = nRequestID;
            this.bIsLast = bIsLast;
        }
    }

    public class OnRspOrderInsertArgs : EventArgs
    {
        public readonly IntPtr pTraderApi;
        public readonly CZQThostFtdcInputOrderField pInputOrder;
        public readonly CZQThostFtdcRspInfoField pRspInfo;
        public readonly int nRequestID;
        public readonly bool bIsLast;
        public OnRspOrderInsertArgs(IntPtr pTraderApi, ref CZQThostFtdcInputOrderField pInputOrder, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            this.pTraderApi = pTraderApi;
            this.pInputOrder = pInputOrder;
            this.pRspInfo = pRspInfo;
            this.nRequestID = nRequestID;
            this.bIsLast = bIsLast;
        }
    }

    public class OnRspQryDepthMarketDataArgs : EventArgs
    {
        public readonly IntPtr pTraderApi;
        public readonly CZQThostFtdcDepthMarketDataField pDepthMarketData;
        public readonly CZQThostFtdcRspInfoField pRspInfo;
        public readonly int nRequestID;
        public readonly bool bIsLast;
        public OnRspQryDepthMarketDataArgs(IntPtr pTraderApi, ref CZQThostFtdcDepthMarketDataField pDepthMarketData, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            this.pTraderApi = pTraderApi;
            this.pDepthMarketData = pDepthMarketData;
            this.pRspInfo = pRspInfo;
            this.nRequestID = nRequestID;
            this.bIsLast = bIsLast;
        }
    }

    public class OnRspQryInstrumentArgs : EventArgs
    {
        public readonly IntPtr pTraderApi;
        public readonly CZQThostFtdcInstrumentField pInstrument;
        public readonly CZQThostFtdcRspInfoField pRspInfo;
        public readonly int nRequestID;
        public readonly bool bIsLast;
        public OnRspQryInstrumentArgs(IntPtr pTraderApi, ref CZQThostFtdcInstrumentField pInstrument, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            this.pTraderApi = pTraderApi;
            this.pInstrument = pInstrument;
            this.pRspInfo = pRspInfo;
            this.nRequestID = nRequestID;
            this.bIsLast = bIsLast;
        }
    }

    public class OnRspQryInstrumentCommissionRateArgs : EventArgs
    {
        public readonly IntPtr pTraderApi;
        public readonly CZQThostFtdcInstrumentCommissionRateField pInstrumentCommissionRate;
        public readonly CZQThostFtdcRspInfoField pRspInfo;
        public readonly int nRequestID;
        public readonly bool bIsLast;
        public OnRspQryInstrumentCommissionRateArgs(IntPtr pTraderApi, ref CZQThostFtdcInstrumentCommissionRateField pInstrumentCommissionRate, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            this.pTraderApi = pTraderApi;
            this.pInstrumentCommissionRate = pInstrumentCommissionRate;
            this.pRspInfo = pRspInfo;
            this.nRequestID = nRequestID;
            this.bIsLast = bIsLast;
        }
    }

    public class OnRspQryInstrumentMarginRateArgs : EventArgs
    {
        public readonly IntPtr pTraderApi;
        public readonly CZQThostFtdcInstrumentMarginRateField pInstrumentMarginRate;
        public readonly CZQThostFtdcRspInfoField pRspInfo;
        public readonly int nRequestID;
        public readonly bool bIsLast;
        public OnRspQryInstrumentMarginRateArgs(IntPtr pTraderApi, ref CZQThostFtdcInstrumentMarginRateField pInstrumentMarginRate, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            this.pTraderApi = pTraderApi;
            this.pInstrumentMarginRate = pInstrumentMarginRate;
            this.pRspInfo = pRspInfo;
            this.nRequestID = nRequestID;
            this.bIsLast = bIsLast;
        }
    }

    public class OnRspQryInvestorPositionArgs : EventArgs
    {
        public readonly IntPtr pTraderApi;
        public readonly CZQThostFtdcInvestorPositionField pInvestorPosition;
        public readonly CZQThostFtdcRspInfoField pRspInfo;
        public readonly int nRequestID;
        public readonly bool bIsLast;
        public OnRspQryInvestorPositionArgs(IntPtr pTraderApi, ref CZQThostFtdcInvestorPositionField pInvestorPosition, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            this.pTraderApi = pTraderApi;
            this.pInvestorPosition = pInvestorPosition;
            this.pRspInfo = pRspInfo;
            this.nRequestID = nRequestID;
            this.bIsLast = bIsLast;
        }
    }

    public class OnRspQryInvestorPositionDetailArgs : EventArgs
    {
        public readonly IntPtr pTraderApi;
        public readonly CZQThostFtdcInvestorPositionDetailField pInvestorPositionDetail;
        public readonly CZQThostFtdcRspInfoField pRspInfo;
        public readonly int nRequestID;
        public readonly bool bIsLast;
        public OnRspQryInvestorPositionDetailArgs(IntPtr pTraderApi, ref CZQThostFtdcInvestorPositionDetailField pInvestorPositionDetail, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            this.pTraderApi = pTraderApi;
            this.pInvestorPositionDetail = pInvestorPositionDetail;
            this.pRspInfo = pRspInfo;
            this.nRequestID = nRequestID;
            this.bIsLast = bIsLast;
        }
    }

    public class OnRspQryOrderArgs : EventArgs
    {
        public readonly IntPtr pTraderApi;
        public readonly CZQThostFtdcOrderField pOrder;
        public readonly CZQThostFtdcRspInfoField pRspInfo;
        public readonly int nRequestID;
        public readonly bool bIsLast;
        public OnRspQryOrderArgs(IntPtr pTraderApi, ref CZQThostFtdcOrderField pOrder, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            this.pTraderApi = pTraderApi;
            this.pOrder = pOrder;
            this.pRspInfo = pRspInfo;
            this.nRequestID = nRequestID;
            this.bIsLast = bIsLast;
        }
    }

    public class OnRspQryTradeArgs : EventArgs
    {
        public readonly IntPtr pTraderApi;
        public readonly CZQThostFtdcTradeField pTrade;
        public readonly CZQThostFtdcRspInfoField pRspInfo;
        public readonly int nRequestID;
        public readonly bool bIsLast;
        public OnRspQryTradeArgs(IntPtr pTraderApi, ref CZQThostFtdcTradeField pTrade, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            this.pTraderApi = pTraderApi;
            this.pTrade = pTrade;
            this.pRspInfo = pRspInfo;
            this.nRequestID = nRequestID;
            this.bIsLast = bIsLast;
        }
    }

    public class OnRspQryTradingAccountArgs : EventArgs
    {
        public readonly IntPtr pTraderApi;
        public readonly CZQThostFtdcTradingAccountField pTradingAccount;
        public readonly CZQThostFtdcRspInfoField pRspInfo;
        public readonly int nRequestID;
        public readonly bool bIsLast;
        public OnRspQryTradingAccountArgs(IntPtr pTraderApi, ref CZQThostFtdcTradingAccountField pTradingAccount, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            this.pTraderApi = pTraderApi;
            this.pTradingAccount = pTradingAccount;
            this.pRspInfo = pRspInfo;
            this.nRequestID = nRequestID;
            this.bIsLast = bIsLast;
        }
    }

    public class OnRtnOrderArgs : EventArgs
    {
        public readonly IntPtr pTraderApi;
        public readonly CZQThostFtdcOrderField pOrder;
        public OnRtnOrderArgs(IntPtr pTraderApi, ref CZQThostFtdcOrderField pOrder)
        {
            this.pTraderApi = pTraderApi;
            this.pOrder = pOrder;
        }
    }

    public class OnRtnTradeArgs : EventArgs
    {
        public readonly IntPtr pTraderApi;
        public readonly CZQThostFtdcTradeField pTrade;
        public OnRtnTradeArgs(IntPtr pTraderApi, ref CZQThostFtdcTradeField pTrade)
        {
            this.pTraderApi = pTraderApi;
            this.pTrade = pTrade;
        }
    }
}
