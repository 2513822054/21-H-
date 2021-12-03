#include "ESP8266.h"
#include <stdio.h>
/*
指令大全


系统设置
AT+RST                                        重启
AT+RESTORE									    恢复出场设置
AT+UART=115200,8,1,0,0                        串口配置(串口率,数据位,停止位,校验位,流控)
AT+CWMODE?                                    查看WIFI模式
AT+CWMODE=1                                   设置WIFI模式，1：客户端，2：服务端，3：通用
ATE1                                          打开回显功能
ATE0										    关闭回显功能

AT模式  1
AT+CWJAP="APNAME","APPASSWORD"                加入AP
AT+CWLAP                                      列出可用AP
AT+CWQAP                                      退出当前AP
AT+CWAUTOCONN=1                               设置STA开机自动连接到WIFI
AT+CIPSTA="192.168.1.109"                     设置STA IP地址
AT+CIPSTAMAC="18:fe:16:98:d3:7b"              设置STA MAC地址
AT+CIPSTA?                                    查看客户端IP信息，1，3模式有效
AT+CIPSTATUS                                  获取连接状态，status:2已连接 AP，获得 IP 地址,4未连接 AP
⭐⭐⭐⭐⭐⭐⭐⭐⭐
AT+CIPSTART="TCP","192.168.72.1",7210


AT模式2
AT+CWSAP="NikoWIFI","1234567890",6,4,4,0      设置AP参数  <ssid>,<pwd>,<chl>,<ecn><max><SSDI>
                                              ecn:加密方式    （0：OPEN，1：WEP，2：WPA_SK,3：WPA2_PSK，4：WPA_WPA2_PSK）通用是4
                                              chl通道号
                                              max最大连接数
                                              SSDI广播
AT+CIPAP="192.168.1.109"                      设置AP   IP地址
AT+CIPAPMAC="28:fe:31:98:d3:7b"               设置AP  MAC地址
AT+CIPAP?                                     查看客户端IP信息，2，3模式有效
AT+CIPSERVER=<status>,<port>				     配置服务端，status，1：打开，0：关闭。port：端口号
AT+CWLIF									     查看已接入设备的IP，IP在前，MAC在后
AT+CIPSTO=<sec>								     设置服务器超时时间，0为永不超时，最大值为7200s，单位为秒

配置服务端步骤：
1、AT+CIPSTO=0       AT+CWMODE=2 关闭回显功能设置成服务端
2、AT+CWSAP="NikoWIFI","1234567890",6,4 设置服务端参数
3、AT+CIPAP="192.168.1.129"       设置服务端IP地址
4、AT+CIPMUX=1                    设置多路连接
5、AT+CIPSERVER=1,8089            打开服务端口
当有设备链接后会有
0,CONNECT  （连接）
1,CONNECT  （连接）
2,CONNECT FAIL  （断开连接）
1,CLOSED        （关闭）
可由此确认设备链接路数
有设备连接后，可以使用
AT+CIPSEND=<channel>,<len>    来发送信息      (通道号)（长度）
接收信号格式：
+IPD,2,102:  （+IPD固定，2表示通道，102表示发送长度）
 */

uint8_t statusTx=0;   //发送状态0：没有发送任务    1：开启发送任务，等待发送返回    2：可以开始发送
uint8_t textRx[256];
uint8_t textTx[256];
uint8_t textBuf;      //暂存变量
uint32_t textPlace=0;   //接收位置
uint8_t APch=0;    //哪一路连上了TCP server     0000 0000 后四位1表示连接上了  例如 03  表示0路 1路连接上
uint8_t APstatus=0;  //配置状态   每完成一步返回OK该值+1   当主机配置完成后该值为6
uint8_t APname[16]="NikoWifi",APpassword[16]="123454321";
uint16_t APnameLen = 8,APpasswordLen=9,portNum =8089;


//串口接收存储一个字节函数
void WIFI_Receive_One()
{
	textRx[textPlace] = textBuf;
	//接收返回信号
	if( textPlace >= 3)
	{
		//返回OK？
		if( textRx[textPlace-3] == 79 && textRx[textPlace-2] == 75  && textRx[textPlace-1] == 13  && textRx[textPlace] == 10 )
		{
			WIFI_Receive_Handle(1);
			return;
		}
		//返回ERROR
		else if(textPlace >= 6 && textRx[textPlace-6] == 69 && textRx[textPlace-5] == 82  && textRx[textPlace-4] == 82  && textRx[textPlace-3] == 79 && textRx[textPlace-2] == 82 && textRx[textPlace-1] == 13 && textRx[textPlace] == 10)
		{
			WIFI_Receive_Handle(2);
			return;
		}
		//接收连接服务器或者断开服务器的信息
		else if(textRx[0] == 48 || textRx[0] == 49 || textRx[0] == 50 || textRx[0] == 51)
		{
			if(textRx[textPlace-1] == 0x0D && textRx[textPlace] == 0x0A && textRx[1] == 0x2C)
			{
				//接收完毕，返回信息
				WIFI_Receive_Handle(3);
				return;
			}
		}
		//接收从机发送过来的信息
		else if(textRx[0] == 0x0D && textRx[1] == 0x0A && textRx[2] == 0x2B && textRx[3] == 0x49 && textRx[4] == 0x50 && textRx[5] == 0x44 && textRx[6] == 0x2C && textRx[textPlace-1] == 0x0D && textRx[textPlace] == 0x0A)
		{
			WIFI_Receive_Handle(4);
			return;
		}
		else if(textRx[0] == 0x2B && textRx[1] == 0x49 && textRx[2] == 0x50 && textRx[3] == 0x44 && textRx[4] == 0x2C && textRx[textPlace-1] == 0x0D && textRx[textPlace] == 0x0A)
		{
			WIFI_Receive_Handle(4);
			return;
		}
		else if(textRx[textPlace-1] == 0x0D && textRx[textPlace] == 0x0A)
		{
			WIFI_Receive_Handle(5);
			return;
		}
	}

	//接收下一个字符
	textPlace++;
	HAL_UART_Receive_IT(WIFI_UART, (uint8_t *)&textBuf, 1);

}

//串口接收处理函数
/*
 * status 表示
 * 0表示信息未接收完
 * 1表示AT指令返回 OK
 * 2表示AT指令返回ERROR
 * 3表示设备连接状态信息
 * 4表示设备发来的信息
 * 5表示已结束但是无法识别的信息（垃圾信息，重新计数）
 * 6表示接收到发送信息的信号   （0x4F 0x4B 0x0D 0x0A 0x3E 0x20）
 */
void WIFI_Receive_Handle(uint8_t status)
{

	//测试用
	//HAL_UART_Transmit(&huart3, textRx, textPlace+1, 5);

	//如果返回的是OK
	if(status == 1 && statusTx == 0)
	{

		//如果不是服务器配置的最后一步则进入下一步配置   否则说明配置已经完成
		textPlace = 0;
		if(APstatus<6)
		{
			APstatus++;
			WIFI_TCP_Server_Init(0);
			HAL_UART_Receive_IT(WIFI_UART, (uint8_t *)&textBuf, 1);
			return;
		}
	}
	else if(status == 3)
	{
		//0连接信息
		if(textRx[0] == 48)
		{
			if(textRx[2]==0x43 && textRx[3]==0x4F && textRx[4]==0x4E && textRx[5]==0x4E && textRx[6]==0x45 && textRx[7]==0x43 && textRx[8]==0x54 && textRx[9]==0x0D)
			{
				//0路连上
				APch = APch | 0x01;
			}
			else
			{
				//0路断开
				APch = APch & 0xFE;
			}
		}
		//1连接信息
		else if(textRx[0] == 49)
		{
			if(textRx[2]==0x43 && textRx[3]==0x4F && textRx[4]==0x4E && textRx[5]==0x4E && textRx[6]==0x45 && textRx[7]==0x43 && textRx[8]==0x54 && textRx[9]==0x0D)
			{
				//1路连上
				APch = APch | 0x02;
			}
			else
			{
				//1路断开
				APch = APch & 0xFD;
			}
		}
		//2连接信息
		else if(textRx[0] == 50)
		{
			if(textRx[2]==0x43 && textRx[3]==0x4F && textRx[4]==0x4E && textRx[5]==0x4E && textRx[6]==0x45 && textRx[7]==0x43 && textRx[8]==0x54 && textRx[9]==0x0D)
			{
				//2路连上
				APch = APch | 0x04;
			}
			else
			{
				//2路断开
				APch = APch & 0xFB;
			}
		}
		//3连接信息
		else if(textRx[0] == 51)
		{
			if(textRx[2]==0x43 && textRx[3]==0x4F && textRx[4]==0x4E && textRx[5]==0x4E && textRx[6]==0x45 && textRx[7]==0x43 && textRx[8]==0x54 && textRx[9]==0x0D)
			{
				//3路连上
				APch = APch | 0x08;
			}
			else
			{
				//3路断开
				APch = APch & 0xF7;
			}
		}
	}
	//处理从机发来的信息
	else if(status == 4)
	{
		uint16_t mark=0,flag=0,sign=0,len=0;
		uint8_t chl;
		for(mark=0;mark<10;mark++)
		{
			if(textRx[mark] == 0x2C)break;
		}
		//记录通道
		chl = textRx[mark+1] - 48;
		mark += 2;
		for(flag=1;flag<5;flag++)
		{
			if(textRx[mark+flag] == 0x3A)break;
		}
		//计算len
		for(sign=1;sign<flag;sign++)
		{
			if(sign==1)len+=(textRx[mark+flag-sign]-48);
			if(sign==2)len+=(textRx[mark+flag-sign]-48)*10;
			if(sign==3)len+=(textRx[mark+flag-sign]-48)*100;
		}
		//将数据传给数据接收函数
		WIFI_Receive_Data(&textRx[mark+flag+1],chl,len);
	}
	else if(status)
	{

	}
	textPlace = 0;
	HAL_UART_Receive_IT(WIFI_UART, (uint8_t *)&textBuf, 1);
}

//接收数据函数，将处理数据的内容写在此函数内
void WIFI_Receive_Data(uint8_t *pdata,uint8_t chl,uint16_t len)
{

	//测试用
	//HAL_UART_Transmit(&huart3, pdata, len, 5);

}

//配置服务端步骤：
//1、AT+CWMODE=2 设置成服务端
//2、AT+CWSAP="NikoWIFI","1234567890",6,4,4,0 设置服务端参数
//3、AT+CIPAP="192.168.1.129"       设置服务端IP地址
//4、AT+CIPMUX=1                    设置多路连接
//5、AT+CIPSERVER=1,8089            打开服务端口
//6、AT+CIPSTO=0                    设置永不超时
//当有设备链接后会有
//0,CONNECT  （连接）
//1,CONNECT  （连接）
//2,CONNECT FAIL  （断开连接）
//1,CLOSED        （关闭）
//可由此确认设备链接路数
//有设备连接后，可以使用
//AT+CIPSEND=<channel>,<len>    来发送信息      (通道号)（长度）
//接收信号格式：
//+IPD,2,102:  （+IPD固定，2表示通道，102表示发送长度）
void WIFI_TCP_Server_Init(uint8_t isRST)
{
	if(isRST==1)APstatus=0;
	//根据配置进度发送不同指令
	if(APstatus==0)
	{
		//
		HAL_UART_Transmit(WIFI_UART, "ATE0\r\n", 6, 5);
		HAL_Delay(5);
		//发送指令
		HAL_UART_Transmit(WIFI_UART, "AT+CWMODE=2\r\n", 13, 5);
		//开启串口中断等待回复
		HAL_UART_Receive_IT(WIFI_UART, (uint8_t *)&textBuf, 1);
	}
	else if(APstatus==1)
	{
		//发送指令
		//HAL_UART_Transmit(WIFI_UART, "AT+CWSAP=\"NikoWWWW\",\"1234567890\",6,4\r\n", 38, 5);
//		HAL_UART_Transmit(&huart3, "AT+CWSAP=\"", 10, 5);
//		HAL_UART_Transmit(&huart3, APname, APnameLen, 5);
//		HAL_UART_Transmit(&huart3, "\",\"", 3, 5);
//		HAL_UART_Transmit(&huart3, APpassword, APpasswordLen, 5);
//		HAL_UART_Transmit(&huart3, "\",6,4\r\n", 7, 5);

		HAL_UART_Transmit(WIFI_UART, "AT+CWSAP=\"", 10, 5);
		HAL_UART_Transmit(WIFI_UART, APname, APnameLen, 5);
		HAL_UART_Transmit(WIFI_UART, "\",\"", 3, 5);
		HAL_UART_Transmit(WIFI_UART, APpassword, APpasswordLen, 5);
		HAL_UART_Transmit(WIFI_UART, "\",6,4\r\n", 7, 5);
		//开启串口中断等待回复
		HAL_UART_Receive_IT(WIFI_UART, (uint8_t *)&textBuf, 1);
	}
	else if(APstatus==2)
	{
		//发送指令
		HAL_UART_Transmit(WIFI_UART, "AT+CIPAP=\"192.168.1.166\"\r\n", 26, 5);
		//开启串口中断等待回复
		HAL_UART_Receive_IT(WIFI_UART, (uint8_t *)&textBuf, 1);
	}
	else if(APstatus==3)
	{
		//发送指令
		HAL_UART_Transmit(WIFI_UART, "AT+CIPMUX=1\r\n", 13, 5);
		//开启串口中断等待回复
		HAL_UART_Receive_IT(WIFI_UART, (uint8_t *)&textBuf, 1);
	}
	else if(APstatus==4)
	{
		//发送指令
		HAL_UART_Transmit(WIFI_UART, "AT+CIPSERVER=1,8089\r\n", 21, 5);
		//开启串口中断等待回复
		HAL_UART_Receive_IT(WIFI_UART, (uint8_t *)&textBuf, 1);
	}
	else if(APstatus==5)
	{
		//发送指令
		HAL_UART_Transmit(WIFI_UART, "AT+CIPSTO=0\r\n", 13, 5);
		//开启串口中断等待回复
		HAL_UART_Receive_IT(WIFI_UART, (uint8_t *)&textBuf, 1);
	}
}

//所有通道发送消息,最高发送240个字节
uint8_t WIFI_Transmit_Data(uint8_t * p,uint8_t len)
{
	//if(APstatus!=6)return 0;
	//判断是否有通道连接上
	if(APch)
	{
		uint32_t countcount=0;
		uint8_t mark;
		uint8_t digit;
		uint8_t lenCH[3];
		len+=2;
		if(len<10)digit = 1;
		else if(len<100)digit = 2;
		else digit =3;
		if (len>240)len=240;
		//处理发送数据
		sprintf(lenCH,"%d",(int)(len));
		for(mark=0;mark<4;mark++)
		{
			if((APch>>mark)&0x01)
			{
				uint8_t channelCH;
				mark+=48;
				HAL_UART_Transmit(WIFI_UART, "AT+CIPSEND=", 11, 5);
				HAL_UART_Transmit(WIFI_UART, &mark, 1, 5);
				HAL_UART_Transmit(WIFI_UART, ",", 1, 5);
				HAL_UART_Transmit(WIFI_UART, lenCH, digit, 5);
				HAL_UART_Transmit(WIFI_UART, "\r\n", 2, 5);
				//HAL_Delay(1);
				while(countcount++<2999);
				HAL_UART_Transmit(WIFI_UART, p, len-2, 5);
				HAL_UART_Transmit(WIFI_UART, "\r\n", 2, 5);
				//while(countcount++<1999999);
			}
		}
		return 1;
	}
	else return 0;


}


