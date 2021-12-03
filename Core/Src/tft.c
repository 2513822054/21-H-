#include "tft.h"

//收到指令值和指令单元
uint8_t R_onedata,R_alldata[256];
uint32_t R_place;
tft_control controlbuf;



//文本控件发送内容
/*
 * screen 画面id
 * id 控件id
 * text  发送字符串地址
 * len 长度
 */
void tft_text(uint8_t screen,uint8_t id,uint8_t *text,uint8_t len)
{
	uint8_t head[7]={0xEE,0xB1,0x10,0x00,screen,0x00,id};
	uint8_t end[4]={0xFF,0xFC,0xFF,0xFF};
	HAL_UART_Transmit(TFT_SEND, head, 7, 5);
	HAL_UART_Transmit(TFT_SEND, text, len, 5);
	HAL_UART_Transmit(TFT_SEND, end, 4, 5);


}

//文本控件发送整数
/*
 * screen 画面id
 * id 控件id
 * num  发送的整数
 */
void tft_text_int(uint8_t screen,uint8_t id,int num)
{
	uint8_t head[7]={0xEE,0xB1,0x10,0x00,screen,0x00,id};
	uint8_t end[4]={0xFF,0xFC,0xFF,0xFF};
	uint8_t	body[11];
	sprintf(body,"%d",num);

	HAL_UART_Transmit(TFT_SEND, head, 7, 5);
	HAL_UART_Transmit(TFT_SEND, body, 11, 5);
	HAL_UART_Transmit(TFT_SEND, end, 4, 5);
}

//文本控件发送浮点
/*
 * screen 画面id
 * id 控件id
 * num  发送浮点数字
 */
void tft_text_float_6(uint8_t screen,uint8_t id,float num)
{
	uint8_t head[7]={0xEE,0xB1,0x10,0x00,screen,0x00,id};
	uint8_t end[4]={0xFF,0xFC,0xFF,0xFF};
	uint8_t	body[13];
	sprintf(body,"%.6f",num);
	HAL_UART_Transmit(TFT_SEND, head, 7, 5);
	HAL_UART_Transmit(TFT_SEND, body,13, 5);
	HAL_UART_Transmit(TFT_SEND, end, 4, 5);

	WIFI_Transmit_Data(head,10);
	WIFI_Transmit_Data(body,13);
	WIFI_Transmit_Data(end,4);
}
//文本控件发送浮点
/*
 * screen 画面id
 * id 控件id
 * num  发送浮点数字
 */
void tft_text_float_3(uint8_t screen,uint8_t id,float num)
{
	uint8_t head[7]={0xEE,0xB1,0x10,0x00,screen,0x00,id};
	uint8_t end[4]={0xFF,0xFC,0xFF,0xFF};
	uint8_t	body[11];
	sprintf(body,"%.3f",num);
	HAL_UART_Transmit(TFT_SEND, head, 7, 5);
	HAL_UART_Transmit(TFT_SEND, body,11, 5);
	HAL_UART_Transmit(TFT_SEND, end, 4, 5);

	WIFI_Transmit_Data(head,10);
	WIFI_Transmit_Data(body,11);
	WIFI_Transmit_Data(end,4);
}


void tft_text_u8_bit(uint8_t screen,uint8_t id,uint8_t num)
{
	uint8_t head[7]={0xEE,0xB1,0x10,0x00,screen,0x00,id};
	uint8_t end[4]={0xFF,0xFC,0xFF,0xFF};
	uint8_t	body[10]="0000 0000\0";

	uint32_t i;
	for(i=0;i<4;i++)
	{
		if(num>>i & 0x01)
		{
			body[8-i] = '1';
		}
		if(num>>(i+4) & 0x01)
		{
			body[3-i] = '1';
		}
	}

	HAL_UART_Transmit(TFT_SEND, head, 7, 5);
	HAL_UART_Transmit(TFT_SEND, body,10, 5);
	HAL_UART_Transmit(TFT_SEND, end, 4, 5);
}


//发送图表内容
/*
 * screen 画面id
 * id 控件id
 * text  发送字符串地址
 * len 长度
 * channel 通道
 */
void tft_form(uint8_t screen,uint8_t id,uint8_t *text,uint16_t len,uint8_t channel)
{
	uint8_t head[10]={0xEE,0xB1,0x32,0x00,screen,0x00,id,channel,(uint8_t)(len>>8),(uint8_t)(len)};
	uint8_t end[4]={0xFF,0xFC,0xFF,0xFF};
	HAL_UART_Transmit(TFT_SEND, head, 10, 5);
	HAL_UART_Transmit(TFT_SEND, text, len, 20);
	HAL_UART_Transmit(TFT_SEND, end, 4, 5);

	WIFI_Transmit_Data(head,10);
	WIFI_Transmit_Data(text,len);
	WIFI_Transmit_Data(end,4);
}


//设置滑动选择控件值
/*
 *index 设置的索引号
 */
void tft_selection(uint8_t screen,uint8_t id,uint8_t index)
{
	uint8_t all[12]={0xEE,0xB1,0x10,0x00,screen,0x00,id,index,0xFF,0xFC,0xFF,0xFF};
	HAL_UART_Transmit(TFT_SEND, all, 12, 5);
}




void tft_button_press(uint8_t screenid,uint8_t id)
{
	uint8_t all[15]={0xEE,0xB1,0x10,0x00,screenid,0x00,id,0x01,0xFF,0xFC,0xFF,0xFF};
	HAL_UART_Transmit(TFT_SEND, all, 12, 5);

	WIFI_Transmit_Data(all,12);

}
void tft_button_lose(uint8_t screenid,uint8_t id)
{
	uint8_t all[15]={0xEE,0xB1,0x10,0x00,screenid,0x00,id,0x00,0xFF,0xFC,0xFF,0xFF};
	HAL_UART_Transmit(TFT_SEND, all, 12, 5);

	WIFI_Transmit_Data(all,12);
}

//设置滑块控件值
/*
 *index 设置的值
 */
void tft_slideblock(uint8_t screen,uint8_t id,uint16_t index)
{
	uint8_t all[15]={0xEE,0xB1,0x10,0x00,screen,0x00,id,0x00,0x00,(uint8_t)(index>>8),(uint8_t)(index),0xFF,0xFC,0xFF,0xFF};
	HAL_UART_Transmit(TFT_SEND, all, 15, 5);
}

 //设置进度条控件值
void tft_progress(uint8_t screen,uint8_t id,uint16_t index)
{
	uint8_t all[15]={0xEE,0xB1,0x10,0x00,screen,0x00,id,0x00,0x00,(uint8_t)(index>>8),(uint8_t)(index),0xFF,0xFC,0xFF,0xFF};
	HAL_UART_Transmit(TFT_SEND, all, 15, 5);
}
  //设置蜂鸣器响的时间   单位10ms
void tft_buzzer(uint8_t index)
{
	uint8_t all[7]={0xEE,0x61,index,0xFF,0xFC,0xFF,0xFF};
	HAL_UART_Transmit(TFT_SEND, all, 7, 5);
}

//曲线空间删除指定通道数据
void tft_form_delete(uint8_t screen,uint8_t id,uint8_t channel)
{
	uint8_t all[12]={0xEE,0xB1,0x33,0x00,screen,0x00,id,channel,0xFF,0xFC,0xFF,0xFF};
	HAL_UART_Transmit(TFT_SEND, all, 12, 5);
}


















uint32_t i;
//接收指令
uint8_t tft_receive()
{
	uint8_t state;
	R_alldata[R_place]=R_onedata;

	if(R_onedata==0xEE){
	R_place++;
	}
	else if(R_place>=1)R_place++;
	if(R_place>5){
		if(R_alldata[R_place-1]==0xFF&&R_alldata[R_place-2]==0xFF&&R_alldata[R_place-3]==0xFC&&R_alldata[R_place-4]==0xFF)
		{
			state=tft_manage();
			for(i=0;i<=255;i++)R_alldata[i]=0;
			R_place=0;
		}
	}
	R_onedata = 0;
	HAL_UART_Receive_IT(TFT_RECEIVE, (uint8_t *)&R_onedata, 1);
	return state;
}


/*
 *    返回值：
 */
uint8_t tft_manage()
{
	//按钮状态或者滑动选择控件状态
	if(R_alldata[1]==0xB1 && R_alldata[2]==0x11)
	{


		controlbuf.screenid = R_alldata[4];
		controlbuf.controlid = R_alldata[6];
		controlbuf.controltype = R_alldata[7];
//		controlbuf.controltype = R_alldata[8];
//		controlbuf.data = R_alldata[9];
		//按钮控件
		if(R_alldata[7]==0x10)
		{
			controlbuf.data = R_alldata[9];
		}
		//滑动选择控件
		else if(R_alldata[7]==0x1B)
		{
			controlbuf.data = R_alldata[8];

		}
		//滑块控件  或进度条控件  或仪表控件
		else if(R_alldata[7]==0x13 || R_alldata[7]==12 || R_alldata[7]==14)
		{
			controlbuf.data =( (uint16_t)R_alldata[10]<<8 | (uint16_t)R_alldata[11]);
		}

		return 1;

	}




	return 0;
}







