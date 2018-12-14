/*
 * main.h
 *
 *  Created on: 13 Mar 2013
 *      Author: Smit
 */
#define Acc_Cal_Enable BIT0
#define Mag_Cal_Enable BIT1
#define Gyr_Cal_Enable BIT2
#define Press_Cal_Enable BIT3

typedef struct {
unsigned char Acc_Cal[3];
unsigned char Mag_Cal[6];
int Gyr_Cal[3];
int Press_Cal;
unsigned char Cal_Enable;
}_Sensor_Calibration_Value;

typedef union {
struct {
int Acc[3];
int Mag[3];
int Gyr[3];
float Press;
}Data;
unsigned char cVal[20];
}_Sensor_Value;

typedef union {
struct {
unsigned long Sicaklik,nem,Pil_Voltaji;
}Data;
unsigned char cVal[12];
}_LMP_ADC_Value;

typedef union {
struct {
unsigned int _16bit;
}Data;
unsigned char cVal[2];
}Convert_16Bit;

typedef union {
struct {
unsigned long _32bit;
}Data;
unsigned char cVal[4];
}Convert_32Bit;
