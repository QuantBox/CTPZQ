using System;

namespace QuantBox.CSharp2CTPZQ
{
    public delegate void fnOnConnect(IntPtr pApi, ref CZQThostFtdcRspUserLoginField pRspUserLogin, ConnectionStatus result);
    public delegate void fnOnDisconnect(IntPtr pApi, ref CZQThostFtdcRspInfoField pRspInfo, ConnectionStatus step);
    public delegate void fnOnErrRtnOrderAction(IntPtr pTraderApi, ref CZQThostFtdcOrderActionField pOrderAction, ref CZQThostFtdcRspInfoField pRspInfo);
    public delegate void fnOnErrRtnOrderInsert(IntPtr pTraderApi, ref CZQThostFtdcInputOrderField pInputOrder, ref CZQThostFtdcRspInfoField pRspInfo);
    public delegate void fnOnRspError(IntPtr pApi, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    public delegate void fnOnRspOrderAction(IntPtr pTraderApi, ref CZQThostFtdcInputOrderActionField pInputOrderAction, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    public delegate void fnOnRspOrderInsert(IntPtr pTraderApi, ref CZQThostFtdcInputOrderField pInputOrder, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    public delegate void fnOnRspQryDepthMarketData(IntPtr pTraderApi,ref CZQThostFtdcDepthMarketDataField pDepthMarketData,ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    public delegate void fnOnRspQryInstrument(IntPtr pTraderApi, ref CZQThostFtdcInstrumentField pInstrument, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    public delegate void fnOnRspQryInstrumentCommissionRate(IntPtr pTraderApi, ref CZQThostFtdcInstrumentCommissionRateField pInstrumentCommissionRate, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    //public delegate void fnOnRspQryInstrumentMarginRate(IntPtr pTraderApi, ref CZQThostFtdcInstrumentMarginRateField pInstrumentMarginRate, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    public delegate void fnOnRspQryInvestorPosition(IntPtr pTraderApi, ref CZQThostFtdcInvestorPositionField pInvestorPosition, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    public delegate void fnOnRspQryOrder(IntPtr pTraderApi, ref CZQThostFtdcOrderField pOrder, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    public delegate void fnOnRspQryTrade(IntPtr pTraderApi, ref CZQThostFtdcTradeField pTrade, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    public delegate void fnOnRspQryTradingAccount(IntPtr pTraderApi, ref CZQThostFtdcTradingAccountField pTradingAccount, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast);
    public delegate void fnOnRtnDepthMarketData(IntPtr pMdUserApi, ref CZQThostFtdcDepthMarketDataField pDepthMarketData);
    public delegate void fnOnRtnOrder(IntPtr pTraderApi, ref CZQThostFtdcOrderField pOrder);
    public delegate void fnOnRtnTrade(IntPtr pTraderApi, ref CZQThostFtdcTradeField pTrade);
}
