using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace QuantBox.CSharp2CTPZQ
{
    public class TraderApiWrapper : IDisposable
    {
        public event OnConnectHander OnConnect;
        public event OnDisconnectHander OnDisconnect;
        public event OnErrRtnOrderActionHander OnErrRtnOrderAction;
        public event OnErrRtnOrderInsertHander OnErrRtnOrderInsert;
        public event OnRspErrorHander OnRspError;
        public event OnRspOrderActionHander OnRspOrderAction;
        public event OnRspOrderInsertHander OnRspOrderInsert;
        public event OnRspQryDepthMarketDataHander OnRspQryDepthMarketData;
        public event OnRspQryInstrumentHander OnRspQryInstrument;
        public event OnRspQryInstrumentCommissionRateHander OnRspQryInstrumentCommissionRate;
        public event OnRspQryInstrumentMarginRateHander OnRspQryInstrumentMarginRate;
        public event OnRspQryInvestorPositionHander OnRspQryInvestorPosition;
        public event OnRspQryInvestorPositionDetailHander OnRspQryInvestorPositionDetail;
        public event OnRspQryOrderHander OnRspQryOrder;
        public event OnRspQryTradeHander OnRspQryTrade;
        public event OnRspQryTradingAccountHander OnRspQryTradingAccount;
        public event OnRtnOrderHander OnRtnOrder;
        public event OnRtnTradeHander OnRtnTrade;

        private readonly fnOnConnect _fnOnConnect_Holder;
        private readonly fnOnDisconnect _fnOnDisconnect_Holder;
        private readonly fnOnErrRtnOrderAction _fnOnErrRtnOrderAction_Holder;
        private readonly fnOnErrRtnOrderInsert _fnOnErrRtnOrderInsert_Holder;
        private readonly fnOnRspError _fnOnRspError_Holder;
        private readonly fnOnRspOrderAction _fnOnRspOrderAction_Holder;
        private readonly fnOnRspOrderInsert _fnOnRspOrderInsert_Holder;
        private readonly fnOnRspQryDepthMarketData _fnOnRspQryDepthMarketData_Holder;
        private readonly fnOnRspQryInstrument _fnOnRspQryInstrument_Holder;
        private readonly fnOnRspQryInstrumentCommissionRate _fnOnRspQryInstrumentCommissionRate_Holder;
        //private readonly fnOnRspQryInstrumentMarginRate _fnOnRspQryInstrumentMarginRate_Holder;
        private readonly fnOnRspQryInvestorPosition _fnOnRspQryInvestorPosition_Holder;
        private readonly fnOnRspQryInvestorPositionDetail _fnOnRspQryInvestorPositionDetail_Holder;
        private readonly fnOnRspQryOrder _fnOnRspQryOrder_Holder;
        private readonly fnOnRspQryTrade _fnOnRspQryTrade_Holder;
        private readonly fnOnRspQryTradingAccount _fnOnRspQryTradingAccount_Holder;
        private readonly fnOnRtnOrder _fnOnRtnOrder_Holder;
        private readonly fnOnRtnTrade _fnOnRtnTrade_Holder;

        private readonly object _lockTd = new object();
        private readonly object _lockMsgQueue = new object();

        private IntPtr m_pTdApi = IntPtr.Zero;
        private IntPtr m_pMsgQueue = IntPtr.Zero;
        private volatile bool _bTdConnected;

        private bool disposed;

        private string szPath;
        private string szAddresses;
        private string szBrokerId;
        private string szInvestorId;
        private string szPassword;
        private string szUserProductInfo;
        private string szAuthCode;
        private ZQTHOST_TE_RESUME_TYPE nResumeType;

        public TraderApiWrapper()
        {
            _fnOnConnect_Holder = OnConnect_callback;
            _fnOnDisconnect_Holder = OnDisconnect_callback;
            _fnOnErrRtnOrderAction_Holder = OnErrRtnOrderAction_callback;
            _fnOnErrRtnOrderInsert_Holder = OnErrRtnOrderInsert_callback;
            _fnOnRspError_Holder = OnRspError_callback;
            _fnOnRspOrderAction_Holder = OnRspOrderAction_callback;
            _fnOnRspOrderInsert_Holder = OnRspOrderInsert_callback;
            _fnOnRspQryDepthMarketData_Holder = OnRspQryDepthMarketData_callback;
            _fnOnRspQryInstrument_Holder = OnRspQryInstrument_callback;
            _fnOnRspQryInstrumentCommissionRate_Holder = OnRspQryInstrumentCommissionRate_callback;
            //_fnOnRspQryInstrumentMarginRate_Holder = OnRspQryInstrumentMarginRate_callback;
            _fnOnRspQryInvestorPosition_Holder = OnRspQryInvestorPosition_callback;
            _fnOnRspQryInvestorPositionDetail_Holder = OnRspQryInvestorPositionDetail_callback;
            _fnOnRspQryOrder_Holder = OnRspQryOrder_callback;
            _fnOnRspQryTrade_Holder = OnRspQryTrade_callback;
            _fnOnRspQryTradingAccount_Holder = OnRspQryTradingAccount_callback;
            _fnOnRtnOrder_Holder = OnRtnOrder_callback;
            _fnOnRtnTrade_Holder = OnRtnTrade_callback;
        }

        //Implement IDisposable.
        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        protected virtual void Dispose(bool disposing)
        {
            if (!disposed)
            {
                if (disposing)
                {
                    // Free other state (managed objects).
                }
                // Free your own state (unmanaged objects).
                // Set large fields to null.
                Disconnect();
                disposed = true;
            }
            //base.Dispose(disposing);
        }

        // Use C# destructor syntax for finalization code.
        ~TraderApiWrapper()
        {
            // Simply call Dispose(false).
            Dispose(false);
        }

        public void Connect(string szPath, string szAddresses,
            string szBrokerId, string szInvestorId, string szPassword,
            ZQTHOST_TE_RESUME_TYPE nResumeType,
            string szUserProductInfo, string szAuthCode)
        {
            this.szPath = szPath;
            this.szAddresses = szAddresses;
            this.szBrokerId = szBrokerId;
            this.szInvestorId = szInvestorId;
            this.szPassword = szPassword;
            this.nResumeType = nResumeType;
            this.szUserProductInfo = szUserProductInfo;
            this.szAuthCode = szAuthCode;

            Disconnect_TD();
            Connect_MsgQueue();
            Connect_TD();
        }

        public void Disconnect()
        {
            Disconnect_TD();
            Disconnect_MsgQueue();
        }

        //建立行情
        private void Connect_TD()
        {
            lock (_lockTd)
            {
                if (null == m_pTdApi || IntPtr.Zero == m_pTdApi)
                {
                    m_pTdApi = TraderApi.TD_CreateTdApi();
                    TraderApi.CTP_RegOnErrRtnOrderAction(m_pMsgQueue, _fnOnErrRtnOrderAction_Holder);
                    TraderApi.CTP_RegOnErrRtnOrderInsert(m_pMsgQueue, _fnOnErrRtnOrderInsert_Holder);
                    TraderApi.CTP_RegOnRspOrderAction(m_pMsgQueue, _fnOnRspOrderAction_Holder);
                    TraderApi.CTP_RegOnRspOrderInsert(m_pMsgQueue, _fnOnRspOrderInsert_Holder);
                    TraderApi.CTP_RegOnRspQryDepthMarketData(m_pMsgQueue, _fnOnRspQryDepthMarketData_Holder);
                    TraderApi.CTP_RegOnRspQryInstrument(m_pMsgQueue, _fnOnRspQryInstrument_Holder);
                    TraderApi.CTP_RegOnRspQryInstrumentCommissionRate(m_pMsgQueue, _fnOnRspQryInstrumentCommissionRate_Holder);
                    //TraderApi.CTP_RegOnRspQryInstrumentMarginRate(m_pMsgQueue, _fnOnRspQryInstrumentMarginRate_Holder);
                    TraderApi.CTP_RegOnRspQryInvestorPosition(m_pMsgQueue, _fnOnRspQryInvestorPosition_Holder);
                    TraderApi.CTP_RegOnRspQryOrder(m_pMsgQueue, _fnOnRspQryOrder_Holder);
                    TraderApi.CTP_RegOnRspQryTrade(m_pMsgQueue, _fnOnRspQryTrade_Holder);
                    TraderApi.CTP_RegOnRspQryTradingAccount(m_pMsgQueue, _fnOnRspQryTradingAccount_Holder);
                    TraderApi.CTP_RegOnRtnOrder(m_pMsgQueue, _fnOnRtnOrder_Holder);
                    TraderApi.CTP_RegOnRtnTrade(m_pMsgQueue, _fnOnRtnTrade_Holder);
                    TraderApi.TD_RegMsgQueue2TdApi(m_pTdApi, m_pMsgQueue);
                    TraderApi.TD_Connect(m_pTdApi, szPath, szAddresses,
                        szBrokerId, szInvestorId, szPassword,
                        nResumeType,
                        szUserProductInfo, szAuthCode);
                }
            }
        }

        private void Disconnect_TD()
        {
            lock (_lockTd)
            {
                if (null != m_pTdApi && IntPtr.Zero != m_pTdApi)
                {
                    TraderApi.TD_RegMsgQueue2TdApi(m_pTdApi, IntPtr.Zero);
                    TraderApi.TD_ReleaseTdApi(m_pTdApi);
                    m_pTdApi = IntPtr.Zero;
                }
                _bTdConnected = false;
            }
        }

        private void Connect_MsgQueue()
        {
            lock (_lockMsgQueue)
            {
                if (null == m_pMsgQueue || IntPtr.Zero == m_pMsgQueue)
                {
                    m_pMsgQueue = CommApi.CTP_CreateMsgQueue();

                    CommApi.CTP_RegOnConnect(m_pMsgQueue, _fnOnConnect_Holder);
                    CommApi.CTP_RegOnDisconnect(m_pMsgQueue, _fnOnDisconnect_Holder);
                    CommApi.CTP_RegOnRspError(m_pMsgQueue, _fnOnRspError_Holder);

                    //由底层启动线程
                    CommApi.CTP_StartMsgQueue(m_pMsgQueue);
                }
            }
        }

        private void Disconnect_MsgQueue()
        {
            lock (_lockMsgQueue)
            {
                if (null != m_pMsgQueue && IntPtr.Zero != m_pMsgQueue)
                {
                    //停止底层线程
                    CommApi.CTP_StopMsgQueue(m_pMsgQueue);

                    CommApi.CTP_ReleaseMsgQueue(m_pMsgQueue);
                    m_pMsgQueue = IntPtr.Zero;
                }
            }
        }

        public int SendOrder(
            string szInstrument,
            string szExchange,
            TZQThostFtdcDirectionType Direction,
            string szCombOffsetFlag,
            string szCombHedgeFlag,
            int VolumeTotalOriginal,
            string LimitPrice,
            TZQThostFtdcOrderPriceTypeType OrderPriceType,
            TZQThostFtdcTimeConditionType TimeCondition,
            TZQThostFtdcContingentConditionType ContingentCondition,
            double StopPrice)
        {
            if (null == m_pTdApi || IntPtr.Zero == m_pTdApi)
            {
                return 0;
            }

            return TraderApi.TD_SendOrder(
               m_pTdApi,
               szInstrument,
               szExchange,
               Direction,
               szCombOffsetFlag,
               szCombHedgeFlag,
               VolumeTotalOriginal,
               LimitPrice,
               OrderPriceType,
               TimeCondition,
               ContingentCondition,
               StopPrice);
        }

        public void CancelOrder(ref CZQThostFtdcOrderField pOrder)
        {
            if (null == m_pTdApi || IntPtr.Zero == m_pTdApi)
            {
                return;
            }

            TraderApi.TD_CancelOrder(m_pTdApi, ref pOrder);
        }

        private void OnConnect_callback(IntPtr pApi, ref CZQThostFtdcRspUserLoginField pRspUserLogin, ConnectionStatus result)
        {
            _bTdConnected = (ConnectionStatus.E_confirmed == result);

            if (null != OnConnect)
            {
                OnConnect(this, new OnConnectArgs(pApi, ref pRspUserLogin, result));
            }
        }

        private void OnDisconnect_callback(IntPtr pApi, ref CZQThostFtdcRspInfoField pRspInfo, ConnectionStatus step)
        {
            if (null != OnDisconnect)
            {
                OnDisconnect(this, new OnDisconnectArgs(pApi, ref pRspInfo, step));
            }
        }

        private void OnErrRtnOrderAction_callback(IntPtr pTraderApi, ref CZQThostFtdcOrderActionField pOrderAction, ref CZQThostFtdcRspInfoField pRspInfo)
        {
            if (null != OnErrRtnOrderAction)
            {
                OnErrRtnOrderAction(this, new OnErrRtnOrderActionArgs(pTraderApi, ref pOrderAction, ref pRspInfo));
            }
        }

        private void OnErrRtnOrderInsert_callback(IntPtr pTraderApi, ref CZQThostFtdcInputOrderField pInputOrder, ref CZQThostFtdcRspInfoField pRspInfo)
        {
            if (null != OnErrRtnOrderInsert)
            {
                OnErrRtnOrderInsert(this, new OnErrRtnOrderInsertArgs(pTraderApi, ref pInputOrder, ref pRspInfo));
            }
        }

        private void OnRspError_callback(IntPtr pApi, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            if (null != OnRspError)
            {
                OnRspError(this, new OnRspErrorArgs(pApi, ref pRspInfo, nRequestID, bIsLast));
            }
        }

        private void OnRspOrderAction_callback(IntPtr pTraderApi, ref CZQThostFtdcInputOrderActionField pInputOrderAction, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            if (null != OnRspOrderAction)
            {
                OnRspOrderAction(this, new OnRspOrderActionArgs(pTraderApi, ref pInputOrderAction, ref pRspInfo, nRequestID, bIsLast));
            }
        }

        private void OnRspOrderInsert_callback(IntPtr pTraderApi, ref CZQThostFtdcInputOrderField pInputOrder, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            if (null != OnRspOrderInsert)
            {
                OnRspOrderInsert(this, new OnRspOrderInsertArgs(pTraderApi, ref pInputOrder, ref pRspInfo, nRequestID, bIsLast));
            }
        }

        private void OnRspQryDepthMarketData_callback(IntPtr pTraderApi, ref CZQThostFtdcDepthMarketDataField pDepthMarketData, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            if (null != OnRspQryDepthMarketData)
            {
                OnRspQryDepthMarketData(this, new OnRspQryDepthMarketDataArgs(pTraderApi, ref pDepthMarketData, ref pRspInfo, nRequestID, bIsLast));
            }
        }

        private void OnRspQryInstrument_callback(IntPtr pTraderApi, ref CZQThostFtdcInstrumentField pInstrument, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            if (null != OnRspQryInstrument)
            {
                OnRspQryInstrument(this, new OnRspQryInstrumentArgs(pTraderApi, ref pInstrument, ref pRspInfo, nRequestID, bIsLast));
            }
        }

        private void OnRspQryInstrumentCommissionRate_callback(IntPtr pTraderApi, ref CZQThostFtdcInstrumentCommissionRateField pInstrumentCommissionRate, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            if (null != OnRspQryInstrumentCommissionRate)
            {
                OnRspQryInstrumentCommissionRate(this, new OnRspQryInstrumentCommissionRateArgs(pTraderApi, ref pInstrumentCommissionRate, ref pRspInfo, nRequestID, bIsLast));
            }
        }

        //private void OnRspQryInstrumentMarginRate_callback(IntPtr pTraderApi, ref CZQThostFtdcInstrumentMarginRateField pInstrumentMarginRate, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        //{
        //    if (null != OnRspQryInstrumentMarginRate)
        //    {
        //        OnRspQryInstrumentMarginRate(this, new OnRspQryInstrumentMarginRateArgs(pTraderApi, ref pInstrumentMarginRate, ref pRspInfo, nRequestID, bIsLast));
        //    }
        //}

        private void OnRspQryInvestorPosition_callback(IntPtr pTraderApi, ref CZQThostFtdcInvestorPositionField pInvestorPosition, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            if (null != OnRspQryInvestorPosition)
            {
                OnRspQryInvestorPosition(this, new OnRspQryInvestorPositionArgs(pTraderApi, ref pInvestorPosition, ref pRspInfo, nRequestID, bIsLast));
            }
        }

        private void OnRspQryInvestorPositionDetail_callback(IntPtr pTraderApi, ref CZQThostFtdcInvestorPositionDetailField pInvestorPositionDetail, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            if (null != OnRspQryInvestorPositionDetail)
            {
                OnRspQryInvestorPositionDetail(this, new OnRspQryInvestorPositionDetailArgs(pTraderApi, ref pInvestorPositionDetail, ref pRspInfo, nRequestID, bIsLast));
            }
        }

        private void OnRspQryOrder_callback(IntPtr pTraderApi, ref CZQThostFtdcOrderField pOrder, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            if (null != OnRspQryOrder)
            {
                OnRspQryOrder(this, new OnRspQryOrderArgs(pTraderApi, ref pOrder, ref pRspInfo, nRequestID, bIsLast));
            }
        }

        private void OnRspQryTrade_callback(IntPtr pTraderApi, ref CZQThostFtdcTradeField pTrade, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            if (null != OnRspQryTrade)
            {
                OnRspQryTrade(this, new OnRspQryTradeArgs(pTraderApi, ref pTrade, ref pRspInfo, nRequestID, bIsLast));
            }
        }

        private void OnRspQryTradingAccount_callback(IntPtr pTraderApi, ref CZQThostFtdcTradingAccountField pTradingAccount, ref CZQThostFtdcRspInfoField pRspInfo, int nRequestID, bool bIsLast)
        {
            if (null != OnRspQryTradingAccount)
            {
                OnRspQryTradingAccount(this, new OnRspQryTradingAccountArgs(pTraderApi, ref pTradingAccount, ref pRspInfo, nRequestID, bIsLast));
            }
        }

        private void OnRtnOrder_callback(IntPtr pTraderApi, ref CZQThostFtdcOrderField pOrder)
        {
            if (null != OnRtnOrder)
            {
                OnRtnOrder(this, new OnRtnOrderArgs(pTraderApi, ref pOrder));
            }
        }

        private void OnRtnTrade_callback(IntPtr pTraderApi, ref CZQThostFtdcTradeField pTrade)
        {
            if (null != OnRtnTrade)
            {
                OnRtnTrade(this, new OnRtnTradeArgs(pTraderApi, ref pTrade));
            }
        }
    }
}
