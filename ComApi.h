#ifndef COMAPI_H
#define COMAPI_H

#include "ace/OS.h"
#include "define.h"
#include "BufferAccess.h"
#include "mystruct.h"

//通讯规约(组包解包)接口基类
class ComApi
{
public:
	ComApi();
	~ComApi();
	virtual unsigned char calc_check_sum(const unsigned char *p_data, unsigned short data_len);
	virtual void CharToBcd(char*Cbuf,byte*Bbuf,int*blen);
	virtual void PutBCD(CBufferAccess &ba,char*key);
	virtual void Put32BCD(CBufferAccess &ba,char*key);

	virtual int get_random_number(int rand_max_val);
	virtual int CP56Time2a_Convert(char*sbuf,time_t  time_val);
	virtual int ChargeDev_7BytesTime_Convert(char*sbuf,time_t  time_val);
	virtual unsigned int Make_Total_Packet(CBufferAccess &ba,CBufferAccess &ba_data);
	
//各功能码组包、解包
	virtual bool Pre_Parser(char*buf,int len);
 
//以下都是虚函数,不同通讯规约版本可以基于此基类自己实现以下相应函数功能
	virtual int Make_Login(CBufferAccess &ba_all,char*charge_id,unsigned char charge_type,unsigned char charge_mode);
	virtual int Parser_Login_ACK(char*buf,int len);

	virtual int Make_Heart(CBufferAccess &ba_all,unsigned char state_A,unsigned char state_B, unsigned char state_charge);
	virtual int Parser_Heart_ACK(char*buf,int len);
	
	virtual int Parser_Time_Sync(char*buf,int len,Charge_Time &my_chargetime);
	virtual int Make_Time_ACK(CBufferAccess &ba_all,Charge_Time &my_chargetime);

	virtual int Parser_PriceMode_Down(char*buf,int len, Charge_PriceMode &my_chargeMode);
	virtual int Make_PriceMode_Ack(CBufferAccess &ba_all);

	virtual int Make_CardReq(CBufferAccess &ba_all,char*card_no,char*card_passwd);
	
	virtual int Make_ChargeReq(CBufferAccess &ba_all,Charge_Order &chargeOrder,char*charge_id);
	virtual int Parser_Charge_ACK(char*buf,int len, Charge_Order &chargeOrder);

	virtual int Make_BMS(CBufferAccess &ba_all,Charge_Order &chargeOrder);
	virtual int Make_ChargeRecord(CBufferAccess &ba_all,Charge_Order &chargeOrder);
	virtual int Make_ChargeRealData(CBufferAccess &ba_all,Charge_Order &chargeOrder);
	virtual int Parser_BMS_ACK(char*buf,int len, Charge_Order &chargeOrder);

public:
	unsigned char prot_type;
	char com_desc[256];//规约描述
	int frame_ByteSeq;

private:
	
};
#endif
