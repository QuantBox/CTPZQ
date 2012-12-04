
namespace QuantBox.CSharp2CTPZQ
{
    //自己定义的
    public enum ConnectionStatus
    {
        E_uninit,		//未初始化
        E_inited,		//已经初始化
        E_unconnected,	//连接已经断开
        E_connecting,	//连接中
        E_connected,	//连接成功
        E_authing,		//授权中
        E_authed,		//授权成功
        E_logining,		//登录中
        E_logined,		//登录成功
        E_confirming,	//确认中
        E_confirmed,	//已经确认
        E_conn_max		//最大值
    };

    public enum ZQTHOST_TE_RESUME_TYPE
    {
        ZQTHOST_TERT_RESTART = 0,
        ZQTHOST_TERT_RESUME,
        ZQTHOST_TERT_QUICK
    };

    //枚举不能以数字开头，所以3DES被改成了DES3
    /// <summary>
    /// TFtdcFBTEncryModeType是一个加密方式类型
    /// </summary>
    public enum TZQThostFtdcFBTEncryModeType : byte
    {
        /// <summary>
        /// 不加密
        /// </summary>
        NoEncry = (byte)'0',

        /// <summary>
        /// DES
        /// </summary>
        DES = (byte)'1',

        /// <summary>
        /// 3DES
        /// </summary>
        DES3 = (byte)'2'
    }

    //原文件是直接将'102001'放到char类型中，转换后不行了，在实际使用时得注意此处
    /// <summary>
    /// TFtdcVirementTradeCodeType是一个银期交易代码枚举类型
    /// </summary>
    public enum TZQThostFtdcVirementTradeCodeType : byte
    {
        /// <summary>
        /// 银行发起银行转期货
        /// </summary>
        BankLaunchBankToBroker = (byte)'0',//'102001',

        /// <summary>
        /// 期货发起银行转期货
        /// </summary>
        BrokerLaunchBankToBroker = (byte)'1',//'202001',

        /// <summary>
        /// 银行发起期货转银行
        /// </summary>
        BankLaunchBrokerToBank = (byte)'2',//'102002',

        /// <summary>
        /// 期货发起期货转银行
        /// </summary>
        BrokerLaunchBrokerToBank = (byte)'3'//'202002'
    }

    /// <summary>
    /// TFtdcDirectionType是一个买卖方向类型
    /// </summary>
    public enum TZQThostFtdcDirectionType:byte
    {
	    /// <summary>
	    /// 买
	    /// </summary>
	    Buy = (byte)'0',

	    /// <summary>
	    /// 卖
	    /// </summary>
	    Sell = (byte)'1',

	    /// <summary>
	    /// ETF申购
	    /// </summary>
	    ETFPur = (byte)'2',

	    /// <summary>
	    /// ETF赎回
	    /// </summary>
	    ETFRed = (byte)'3',

	    /// <summary>
	    /// 融资买入
	    /// </summary>
	    MarginTrade = (byte)'4',

	    /// <summary>
	    /// 融券卖出
	    /// </summary>
	    ShortSell = (byte)'5',

	    /// <summary>
	    /// 卖券还款
	    /// </summary>
	    RepayMargin = (byte)'6',

	    /// <summary>
	    /// 买券还券
	    /// </summary>
	    RepayStock = (byte)'7',

	    /// <summary>
	    /// 直接还款
	    /// </summary>
	    DirectRepayMargin = (byte)'8',

	    /// <summary>
	    /// 直接还券
	    /// </summary>
	    DirectRepayStock = (byte)'9',

	    /// <summary>
	    /// 担保品划转入信用账户
	    /// </summary>
	    TransferSecuritiesIn = (byte)'a',//'10'

	    /// <summary>
	    /// 担保品划转出信用账户
	    /// </summary>
	    TransferSecuritiesOut = (byte)'b',//'11'

	    /// <summary>
	    /// 现金替代，只用作回报
	    /// </summary>
	    CashIn = (byte)'c'//'12'
    }
}
