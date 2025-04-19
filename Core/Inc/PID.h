#ifndef __PID_H
#define __PID_H

typedef struct
{
	float Err;			  //����ƫ��ֵ
	float Err_last;		  //������һ��ƫ��ֵ
    float Kpa,Kpb;
	float Ki,Kd,gkd;		  //������������֡�΢��
	float Integral;       // ������
	float Integral_Last;  // ��һ�λ���
	float PID_Out;		  //�����������
	
	float Out_Limit;      // ����޷�
}PID_InitTypeDef;

extern PID_InitTypeDef Turn_PID;

//PID��ʼ��
void PID_Init(PID_InitTypeDef *PID_Struct, float Kpa,float Kpb,float Kd,float gkd,float Out_Limit);
//PID����
void PID_Calculate(PID_InitTypeDef *PID_Struct, float Exp_Val, float Act_Val);

//***********************************************************����ʽPID**************************************************//
typedef struct{
    float Kp;
    float Ki;
    float Kd;
	
    float error;
    float last_error;
    float last_last_error;
    float last_out;
	
    float P_Out,I_Out,D_Out;		//����PID�����
    float out;
    float outmax;
    float outmin;
	
    float Integral_Limit;
    unsigned char use_lowpass_filter;
    float lowpass_filter_factor;
	
} PID_Incremental;

extern PID_Incremental Left_Wheel;
extern PID_Incremental Right_Wheel;

void PID_Incremental_Init(PID_Incremental *pid, float Kp, float Ki, float Kd,float Oout_Limit,unsigned char use_lowpass_filter);
float PID_Incremental_Calc(PID_Incremental *pid, float setpoint, float input_value);

#endif