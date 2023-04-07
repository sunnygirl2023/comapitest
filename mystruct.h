#ifndef MYSTRUCT_H
#define MYSTRUCT_H


//后台配置结构体
typedef struct _Charge_Houtai
{
	char server_ip[32];
	int server_port;
}Charge_Houtai;

#define KEYID_LEN	16

//充电桩设备信息结构体
typedef struct _Charge_Info
{
	char charge_id[KEYID_LEN+1];			//桩ID
	char charge_name[64];		//桩别名
	float charge_p;				//功率
	unsigned char charge_type;	//充电桩类型:直流桩、交流桩
	unsigned char charge_mode;	//充电模式
	
	char com_version[64];		//通讯规约版本
	unsigned char userType;		//桩用户的类型：card or vin
	char user_id[16];			//桩用户 "绑定用户卡号或白名单VIN

	unsigned char login_model_val;
}Charge_Info;



//保存充电桩当前时间
typedef struct _Charge_Time
{
	unsigned char sec;
	unsigned char min;
	unsigned char hour;
	unsigned char day;
	unsigned char mon;
	unsigned short year;
}Charge_Time;


typedef struct _Charge_PricePeriod
{
	unsigned char starttime_hour;
	unsigned char starttime_min;
	unsigned char endtime_hour;
	unsigned char endtime_min;
	float price;	//电价	4Byte	BIN码	
	unsigned char period_flag;//标志	1Byte	BIN码	1峰，2t谷，3尖，4平
}Charge_PricePeriod;

#define MAX_PERIOD	10		//最大时段数
typedef struct _Charge_PriceMode
{
	float price_server;			//服务费单价
	unsigned char period_num;	//时段数
	Charge_PricePeriod PricePeriod[MAX_PERIOD];
}Charge_PriceMode;



typedef struct _Charge_Status
{
	//A枪状态	1Byte	BIN码	00-无 01-空闲 02-充电 03-故障 04-待机 05-已排队 06-维护 07-已预约
	// 		B枪状态	1Byte	BIN码	同上
	// 		充电机状态	1Byte	BIN码	01-故障02-待机03-工作
	unsigned char state_a;
	unsigned char state_b;
	unsigned char state_charge;
}Charge_Status;


typedef struct _Charge_Card
{
	char card_no[KEYID_LEN+1];
	char card_pw[KEYID_LEN+1];
}Charge_Card;


// 	枪号	BIN码	1Byte	01-A枪 02-B枪 03-AB枪
// 	卡编号	BCD码	8Byte	账户卡号
// 	业务标识	BCD码	 16Byte	平台生成的订单编号
// 	响应结果	BIN码	1Byte	01-同意 02-拒绝
typedef struct _Charge_Order
{
	char charge_no;
	char card_no[KEYID_LEN+1];//卡编号
	char card_no_bcd[KEYID_LEN+1];//卡编号BCD
	char order_bcd[KEYID_LEN+1];//平台生成的订单编号
	char order_answer;
	char isvip;
	char vin[KEYID_LEN+2];
	time_t  order_starttime;
	time_t  order_stoptime;
	float balance;
}Charge_Order;


#endif
