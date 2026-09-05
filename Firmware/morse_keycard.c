#include "ch32fun.h"

#include <stdint.h>

#define DOT_CHANNEL  1
#define DASH_CHANNEL 2
#define LED_PIN      PB0

#define DOT_TIME_MS       120
#define DASH_TIME_MS      (3 * DOT_TIME_MS)
#define POLL_TIME_MS      5
#define CALIBRATION_SAMPLES 32

static uint16_t dot_baseline;
static uint16_t dash_baseline;

static uint16_t touch_sample(uint8_t channel)
{
	ADC1->RSQR3 = channel;
	TKey1->IDATAR1 = 0x10;
	TKey1->RDATAR = 0x08;
	while (!(ADC1->STATR & ADC_FLAG_EOC))
		;

	return (uint16_t)TKey1->RDATAR;
}

static void touch_init(void)
{
	RCC->APB2PCENR |= RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOA |
	                    RCC_APB2Periph_GPIOB | RCC_APB2Periph_ADC1;
	RCC->CFGR0 |= 3U << 14;

	funPinMode(PA1, GPIO_CFGLR_IN_ANALOG);
	funPinMode(PA2, GPIO_CFGLR_IN_ANALOG);
	funPinMode(LED_PIN, GPIO_CFGLR_OUT_10Mhz_PP);
	funDigitalWrite(LED_PIN, FUN_LOW);

	RCC->APB2PRSTR |= RCC_APB2Periph_ADC1;
	RCC->APB2PRSTR &= ~RCC_APB2Periph_ADC1;

	TKey1->SAMPTR2 = (ADC_SampleTime_7Cycles5 << (3 * DOT_CHANNEL)) |
	                  (ADC_SampleTime_7Cycles5 << (3 * DASH_CHANNEL));
	ADC1->RSQR1 = 0;
	ADC1->RSQR2 = 0;
	ADC1->CTLR2 |= ADC_ADON;

	ADC1->CTLR2 |= ADC_RSTCAL;
	while (ADC1->CTLR2 & ADC_RSTCAL)
		;
	ADC1->CTLR2 |= ADC_CAL;
	while (ADC1->CTLR2 & ADC_CAL)
		;

	TKey1->CTLR1 |= ADC_BUFEN | (1U << 24);
}

static void calibrate(void)
{
	uint32_t dot_total = 0;
	uint32_t dash_total = 0;

	for (uint8_t i = 0; i < CALIBRATION_SAMPLES; ++i) {
		dot_total += touch_sample(DOT_CHANNEL);
		dash_total += touch_sample(DASH_CHANNEL);
		Delay_Ms(POLL_TIME_MS);
	}

	dot_baseline = dot_total / CALIBRATION_SAMPLES;
	dash_baseline = dash_total / CALIBRATION_SAMPLES;
}

static int is_touched(uint16_t value, uint16_t baseline)
{
	return value < baseline - baseline / 4;
}

static void pulse(uint16_t duration_ms)
{
	funDigitalWrite(LED_PIN, FUN_HIGH);
	Delay_Ms(duration_ms);
	funDigitalWrite(LED_PIN, FUN_LOW);
}

static void wait_for_release(void)
{
	uint8_t released_samples = 0;

	while (released_samples < 3) {
		uint16_t dot = touch_sample(DOT_CHANNEL);
		uint16_t dash = touch_sample(DASH_CHANNEL);

		if (!is_touched(dot, dot_baseline) &&
		    !is_touched(dash, dash_baseline))
			++released_samples;
		else
			released_samples = 0;

		Delay_Ms(POLL_TIME_MS);
	}
}

int main(void)
{
	SystemInit();
	touch_init();
	calibrate();

	while (1) {
		uint16_t dot = touch_sample(DOT_CHANNEL);
		uint16_t dash = touch_sample(DASH_CHANNEL);

		if (is_touched(dot, dot_baseline)) {
			pulse(DOT_TIME_MS);
			wait_for_release();
		} else if (is_touched(dash, dash_baseline)) {
			pulse(DASH_TIME_MS);
			wait_for_release();
		} else {
			dot_baseline = (uint16_t)(((uint32_t)dot_baseline * 63 + dot) / 64);
			dash_baseline = (uint16_t)(((uint32_t)dash_baseline * 63 + dash) / 64);
		}

		Delay_Ms(POLL_TIME_MS);
	}
}
