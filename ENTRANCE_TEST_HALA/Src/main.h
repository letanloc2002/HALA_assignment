/*
 * main.h
 *
 *  Created on: Dec 29, 2024
 *      Author: Loc
 */
#include <stdint.h>
#ifndef MAIN_H_
#define MAIN_H_

//
#define RCC_AHB1ENR		(*(volatile uint32_t*)0x40023830)
#define GPIOA_MODER		(*(volatile uint32_t*)0x40020000)
#define GPIOA_OTYPER	(*(volatile uint32_t*)0x40020004)
#define GPIOA_ODR		(*(volatile uint32_t*)0x40020014)
//
#define RCC_CR			(*(volatile uint32_t*)0x40023800)
#define RCC_CFGR		(*(volatile uint32_t*)0x40023808)
//
#define RCC_APB1ENR		(*(volatile uint32_t*)0x40023840)
#define TIM2_CR1		(*(volatile uint32_t*)0x40000000)
#define TIM2_PSC		(*(volatile uint32_t*)0x40000028)
#define TIM2_ARR		(*(volatile uint32_t*)0x4000002C)
#define TIM2_DIER		(*(volatile uint32_t*)0x4000000C)
#define TIM2_SR			(*(volatile uint32_t*)0x40000010)
#define TIM2_EGR		(*(volatile uint32_t*)0x40000014)
#define NVIC_ISER0		(*(volatile uint32_t*)0xE000E100)


void HSI_Init(void)
{
	RCC_CR |= (1<<0);
	RCC_CFGR &= ~(0x3<<21);
}
void Timer2_Init(uint16_t prescaler, uint16_t period)
{
	RCC_APB1ENR |= (1<<0);
	TIM2_PSC = prescaler - 1;      // Set prescaler
	TIM2_ARR = period - 1;         // Set auto-reload value
	TIM2_CR1 	|= (1 << 0);          // Enable Timer
	//TIM2_EGR	 |= (1<<0);
	TIM2_DIER	 |= (1 << 0);         // Enable update interrupt (UIE)
	NVIC_ISER0	|= (1 << 28);       // Enable interrupt in NVIC for Timer2
}

void Timer2_Start() {
    TIM2_CR1 |= (1 << 0);          // Enable Timer2
}
void Timer2_Stop() {
    TIM2_CR1 &= ~(1 << 0);         // Disable Timer2
}
volatile uint32_t timerDelayCounter = 0;

void Delay_ms(uint32_t ms) {
    timerDelayCounter = ms;
    while (timerDelayCounter > 0);
}

// ISR for Timer2 (in startup file)
void TIM2_IRQHandler(void)
{
    if (TIM2_SR & (1 << 0))
    {
    	// Check update interrupt flag
        TIM2_SR &= ~(1 << 0);      // Clear interrupt flag
        if (timerDelayCounter > 0)
        {
            timerDelayCounter--;
        }
    }
}
void GPIOA_Pin5_Init(void)
{
	RCC_AHB1ENR |= (1<<0);
	GPIOA_MODER |= (1<<10);
	GPIOA_OTYPER &= ~(1<<5);
	GPIOA_ODR 	&=	~(1<<5);
}

void GPIOA_Pin5_Toggle(void)
{
	GPIOA_ODR ^= (1<<5);
}

#endif /* MAIN_H_ */
