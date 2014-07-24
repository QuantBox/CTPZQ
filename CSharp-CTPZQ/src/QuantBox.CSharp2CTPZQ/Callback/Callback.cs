using QuantBox.Library;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace QuantBox.CSharp2CTPZQ.Callback
{
    public delegate void fnOnConnect(object sender, IntPtr pApi, ref CThostFtdcRspUserLoginField pRspUserLogin, ConnectionStatus result);
    public delegate void fnOnDisconnect(object sender, IntPtr pApi, ref CThostFtdcRspInfoField pRspInfo, ConnectionStatus step);
    public delegate void fnOnErrRtnOrderAction(object sender, IntPtr pTraderApi, ref CThostFtdcOrderActionField pOrderAction, ref CThostFtdcRspInfoField pRspInfo);
    public delegate void fnOnErrRtnOrderInsert(object sender, IntPtr pTraderApi, ref CThostFtdcInputOrderField pInputOrder, ref CThostFtdcRspInfoField pRspInfo);
    public delegate void fnOnRspError(object sender, IntPtr pApi, ref CThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    public delegate void fnOnRspOrderAction(object sender, IntPtr pTraderApi, ref CThostFtdcInputOrderActionField pInputOrderAction, ref CThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    public delegate void fnOnRspOrderInsert(object sender, IntPtr pTraderApi, ref CThostFtdcInputOrderField pInputOrder, ref CThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    public delegate void fnOnRspQryDepthMarketData(object sender, IntPtr pTraderApi, ref CThostFtdcDepthMarketDataField pDepthMarketData, ref CThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    public delegate void fnOnRspQryInstrument(object sender, IntPtr pTraderApi, ref CThostFtdcInstrumentField pInstrument, ref CThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    public delegate void fnOnRspQryInstrumentCommissionRate(object sender, IntPtr pTraderApi, ref CThostFtdcInstrumentCommissionRateField pInstrumentCommissionRate, ref CThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    //public delegate void fnOnRspQryInstrumentMarginRate(IntPtr pTraderApi, ref CThostFtdcInstrumentMarginRateField pInstrumentMarginRate, ref CThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    public delegate void fnOnRspQryInvestorPosition(object sender, IntPtr pTraderApi, ref CThostFtdcInvestorPositionField pInvestorPosition, ref CThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    public delegate void fnOnRspQryInvestorPositionDetail(object sender, IntPtr pTraderApi, ref CThostFtdcInvestorPositionDetailField pInvestorPositionDetail, ref CThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    public delegate void fnOnRspQryOrder(object sender, IntPtr pTraderApi, ref CThostFtdcOrderField pOrder, ref CThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    public delegate void fnOnRspQryTrade(object sender, IntPtr pTraderApi, ref CThostFtdcTradeField pTrade, ref CThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    public delegate void fnOnRspQryTradingAccount(object sender, IntPtr pTraderApi, ref CThostFtdcTradingAccountField pTradingAccount, ref CThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    public delegate void fnOnRtnDepthMarketData(object sender, IntPtr pMdUserApi, ref CThostFtdcDepthMarketDataField pDepthMarketData);
    public delegate void fnOnRtnInstrumentStatus(object sender, IntPtr pTraderApi, ref CThostFtdcInstrumentStatusField pInstrumentStatus);
    public delegate void fnOnRtnOrder(object sender, IntPtr pTraderApi, ref CThostFtdcOrderField pOrder);
    public delegate void fnOnRtnTrade(object sender, IntPtr pTraderApi, ref CThostFtdcTradeField pTrade);
}
