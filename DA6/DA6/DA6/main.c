/*
 * DA6.c
 *
 * Created: 5/4/2019 9:58:14 AM
 * Author : acate
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>			
#include <util/delay.h>	
#include <math.h>			
#include <stdlib.h>			
#include <stdio.h>			
#include "MPU6050_def.h"	
#include "i2c_master.h"		
#include "uart.h"			

#define MPU6050_WRITE 0xD0
#define MPU6050_READ 0xD1

float Acc_x;
float Acc_y;
float Acc_z;
float Gyro_x;
float Gyro_y;
float Gyro_z;

void init_uart(uint16_t baudrate){
	uint16_t UBRR_val = (F_CPU/16)/(baudrate-1);
	UBRR0H = UBRR_val >> 8;
	UBRR0L = UBRR_val;
	UCSR0B |= (1<<TXEN0) | (1<<RXEN0) | (1<<RXCIE0); 
	UCSR0C |= (1<<USBS0) | (3<<UCSZ00); 
}

void uart_putc(unsigned char c){
	while(!(UCSR0A & (1<<UDRE0)));	
	UDR0 = c;						
}

void uart_puts(char *s){
	while(*s){
		uart_putc(*s);
		s++;
	}
}

void init_MPU6050(void){
	_delay_ms(150);				
	i2c_start(MPU6050_WRITE);	
	i2c_write(SMPLRT_DIV);		
	i2c_write(0x07);			
	i2c_stop();
	i2c_start(MPU6050_WRITE);
	i2c_write(PWR_MGMT_1);
	i2c_write(0x01); // PLL with X axis gyroscope reference
	i2c_stop();
	i2c_start(MPU6050_WRITE);
	i2c_write(CONFIG); 
	i2c_write(0x00);
	i2c_stop();
	i2c_start(MPU6050_WRITE);
	i2c_write(GYRO_CONFIG); 
	i2c_write(0x18);		
	i2c_stop();
	i2c_start(MPU6050_WRITE);
	i2c_write(INT_ENABLE); // DATA_RDY_EN = 1
	i2c_write(0x01);
	i2c_stop();
}

void getreading(void){

	i2c_start(MPU6050_WRITE);
	i2c_write(ACCEL_XOUT_H); // set pointer
	i2c_stop();
	i2c_start(MPU6050_READ);
	Acc_x = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	i2c_stop();
	
	i2c_start(MPU6050_WRITE);
	i2c_write(ACCEL_YOUT_H); // set pointer
	i2c_stop();
	i2c_start(MPU6050_READ);
	Acc_y = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	i2c_stop();
	
	i2c_start(MPU6050_WRITE);
	i2c_write(ACCEL_ZOUT_H); // set pointer
	i2c_stop();
	i2c_start(MPU6050_READ);
	Acc_z = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	i2c_stop();
	
	i2c_start(MPU6050_WRITE);
	i2c_write(GYRO_XOUT_H); // set pointer
	i2c_stop();
	i2c_start(MPU6050_READ);
	Gyro_x = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	i2c_stop();
	
	i2c_start(MPU6050_WRITE);
	i2c_write(GYRO_YOUT_H); // set pointer
	i2c_stop();
	i2c_start(MPU6050_READ);
	Gyro_y = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	i2c_stop();
	
	i2c_start(MPU6050_WRITE);
	i2c_write(GYRO_ZOUT_H); // set pointer
	i2c_stop();
	i2c_start(MPU6050_READ);
	Gyro_z = (((int)i2c_read_ack()<<8) | (int)i2c_read_ack());
	i2c_stop();
}

int main(void){
	char buffer[20], float_[10];
	float Xa;
	float Ya;
	float Za;
	float Xg;
	float Yg;
	float Zg;
	init_uart(9600);
	i2c_init();
	init_MPU6050();
	
	while(1){
		getreading();
		Xa = Acc_x/16384.0;				
		Ya = Acc_y/16384.0;
		Za = Acc_z/16384.0;
		Xg = Gyro_x/16.4;
		Yg = Gyro_y/16.4;
		Zg = Gyro_z/16.4;
		dtostrf( Xa, 3, 2, float_ );	
		sprintf(buffer,"%s Xa, ",float_);
		USART_SendString(buffer);

		dtostrf( Ya, 3, 2, float_ );
		sprintf(buffer,"%s Ya, ",float_);
		USART_SendString(buffer);
		
		dtostrf( Za, 3, 2, float_ );
		sprintf(buffer,"%s Za, ",float_);
		USART_SendString(buffer);
		
		dtostrf( Xg, 3, 2, float_ );
		sprintf(buffer,"%s Xg, ",float_);
		USART_SendString(buffer);
		
		dtostrf( Yg, 3, 2, float_ );
		sprintf(buffer,"%s Yg, ",float_);
		USART_SendString(buffer);
		
		dtostrf( Zg, 3, 2, float_ );
		sprintf(buffer,"%s Zg, ",float_);
		USART_SendString(buffer);
		
		USART_SendString("\r\n");
		_delay_ms(1000);
	}
	
	return 0;
}
