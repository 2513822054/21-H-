#ifndef __TFT_H
#define __TFT_H
#include "main.h"
#include <stdio.h>
//此两行define需要根据单片机设置的发送和接收的串口分别设置，发送和接收最好不要用一个串口，接收串口要配置中断，只需将tft_receive函数放入中断服务函数中即可
#define TFT_SEND &huart3
#define TFT_RECEIVE &huart3

extern uint8_t R_onedata,R_alldata[256];

//接收指令   //如果返回值是0则说明暂时没有接收到完整数据
//1说明返回指令有效    会将指令信息存入结构体 tft_control内
uint8_t tft_receive();
uint8_t tft_manage();//处理指令

void tft_button_press(uint8_t screenid,uint8_t id);
void tft_button_lose(uint8_t screenid,uint8_t id);


void tft_text(uint8_t screen,uint8_t id,uint8_t *text,uint8_t len);//发送文本控件内容
void tft_text_float_6(uint8_t screen,uint8_t id,float num);      //文本控件发送浮点
void tft_text_float_3(uint8_t screen,uint8_t id,float num);      //文本控件发送浮点
void tft_text_int(uint8_t screen,uint8_t id,int num);          //文本控件发送32位整数
void tft_form(uint8_t screen,uint8_t id,uint8_t *text,uint16_t len,uint8_t channel);//发送图标内容   曲线表
void tft_form_delete(uint8_t screen,uint8_t id,uint8_t channel);//曲线空间删除指定通道数据
void tft_selection(uint8_t screen,uint8_t id,uint8_t index);//设置滑动选择控件值
void tft_slideblock(uint8_t screen,uint8_t id,uint16_t index);    //设置滑块控件值
void tft_progress(uint8_t screen,uint8_t id,uint16_t index);    //设置进度条控件值
void tft_buzzer(uint8_t index);                                 //设置蜂鸣器响的时间
void tft_text_u8_bit(uint8_t screen,uint8_t id,uint8_t num);
typedef struct
{
	//界面号
	uint8_t screenid;
	//控件号
	uint8_t controlid;
	//功能
	/*
	 *10  按钮
	 *1B 滑动选择控件
	 */
	uint8_t controltype;
	//状态数值
	uint16_t data;
}tft_control;

extern tft_control controlbuf;

#endif
