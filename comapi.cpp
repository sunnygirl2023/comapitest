#include "stdafx.h"
//#include <Windows.h>
#include "comapi.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

ComApi::ComApi()
{
	prot_type=0;
	memset(com_desc,0,sizeof(com_desc));
	frame_ByteSeq=CHARGER_ByteSeq;//采用大端字节序

	//char CP56Time2a_sbuf[KEYID_LEN]={0};
	//int t1=time(0);
	//ChargeDev_7BytesTime_Convert(CP56Time2a_sbuf,t1);
}

ComApi::~ComApi()
{

}



//校验和
unsigned char  ComApi::calc_check_sum(const unsigned char *p_data, unsigned short data_len)
{
	unsigned short  i;
	unsigned char  sum =0;

	for (i =0; i < data_len; i++)
	{
		sum += p_data[i];
	}
	return sum;
}





//https://blog.csdn.net/xqhrs232/article/details/9198989
//将十进制数/字符串转换为BCD码（将字符存入内存）
//字符串转成BCD码
void ComApi::CharToBcd(char*Cbuf,byte*Bbuf,int*blen)
{
	int i=0;
	int j=0;
	int tem_len=0;
	byte tem_h=0;
	byte tem_l=0;
	tem_len=(int)strlen(Cbuf);
	for(i=0;i<tem_len;i++)
	{
		switch(Cbuf[i])
		{
			case '0':
			case '1':
				Cbuf[i]=((Cbuf[i]-0x30)&0x0f);
				break;
			case 'A':
			case 'a':
				Cbuf[i]=0x0A;
				break;
			case 'B':
			case 'b':
				Cbuf[i]=0x0B;
				break;
			case 'C':
			case 'c':
				Cbuf[i]=0x0C;
				break;
			case 'D':
			case 'd':
				Cbuf[i]=0x0D;
				break;
			case 'E':
			case 'e':
				Cbuf[i]=0x0E;
				break;
			case 'F':
			case 'f':
				Cbuf[i]=0x0F;
				break;
			default:
				break;
		}
	}
	
	if((tem_len%2)==0)
	{
		for(i=0,j=0;i<tem_len;i++,j++)
		{
			tem_h=(byte)(Cbuf[i]<<4)&0xf0;
			tem_l=(byte)(Cbuf[++i]&0x0f);
			Bbuf[j]=tem_h|tem_l;
		}
		*blen=(tem_len/2);
	}
	else
	{
		if(tem_len==1)
		{
			Bbuf[0]=(byte)(Cbuf[tem_len-1]&0x0f);
			*blen=tem_len;
		}
		else
		{
			for(i=0,j=0;i<tem_len-1;i++,j++)
			{
				tem_h=(byte)(Cbuf[i]<<4)&0xf0;
				tem_l=(byte)(Cbuf[++i]&0x0f);
				Bbuf[j]=tem_h|tem_l;
			}
			Bbuf[tem_len/2]=(byte)(Cbuf[tem_len-1]&0x0f);
			*blen=(tem_len/2)+1;
		}
	}
}

//char[16]--> 8Byte BCD
void ComApi::PutBCD(CBufferAccess &ba,char*key)
{
	char temp[KEYID_LEN+1]={0};
	memcpy(temp,key,KEYID_LEN);
	unsigned char Bbuf[64]={0};
	int blen=0;
	CharToBcd(temp,Bbuf,&blen);
	ba.PutBinary(Bbuf,blen);
}

void ComApi::Put32BCD(CBufferAccess &ba,char*key)
{
	char temp[KEYID_LEN*2+1]={0};
	memcpy(temp,key,KEYID_LEN*2);
	unsigned char Bbuf[64]={0};
	int blen=0;
	CharToBcd(temp,Bbuf,&blen);
	ba.PutBinary(Bbuf,blen);
}


//报文帧格式
// 域名			字节数		说  明
// 	起始域		1		起始字节，固定为0x68
// 	数据长度	2		功能码与数据字总节数
// 	功能码		1		见“功能码”小节
// 	数据域		n		数据域，不同的功能码对应不同的数据定义
// 	检验位		1		功能码与数据与累加和
// 	结束语		1		结束字节，固定为0x16
// 	注：本协议中所有字段采用大端模式发送，即先发送高字节，再发送低字节。
unsigned int ComApi::Make_Total_Packet(CBufferAccess &ba,CBufferAccess &ba_data)
{
	ba.SetByteSeq(frame_ByteSeq);
	ba.PutByte(FRAME_START);
	unsigned short len=ba_data.GetRealLength();
	ba.PutShort(len);
	ba.PutBinary(ba_data.GetPtr(),ba_data.GetRealLength());
	unsigned char crc=calc_check_sum((unsigned char*)(ba_data.GetPtr()),ba_data.GetRealLength());
	ba.PutByte(crc);
	ba.PutByte(FRAME_END);
	return ba.GetRealLength();
}
