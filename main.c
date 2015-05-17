//*****************************************************************************
//
// MSP432 main.c template
//
//****************************************************************************

/* DriverLib Includes */
#include "driverlib.h"

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

/* Statics */
static volatile uint16_t curADCResult;
static volatile float normalizedADCRes;

void main(void) {
	/* Stop Watchdog  */
	MAP_WDT_A_holdTimer();

	/* Initializing Variables */
	curADCResult = 0;

	/* Setting DCO to 48MHz  */
	MAP_PCM_setPowerState(PCM_AM_LDO_VCORE1);
	MAP_CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_48);

	/* Enabling the FPU for floating point operation */
	MAP_FPU_enableModule();
	MAP_FPU_enableLazyStacking();

	/* Initializing ADC (MCLK/1/4) */
	MAP_ADC14_enableModule();
	MAP_ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_4,
			0);

	/* Configuring GPIOs (6.1 A14) */
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6, GPIO_PIN1,
			GPIO_TERTIARY_MODULE_FUNCTION);

	/* Configuring ADC Memory */
	MAP_ADC14_configureSingleSampleMode(ADC_MEM0, true);
	MAP_ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_AVCC_VREFNEG_VSS,
		ADC_INPUT_A14, false);

	/* Configuring Sample Timer */
	MAP_ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);

	/* Enabling/Toggling Conversion */
	MAP_ADC14_enableConversion();
	MAP_ADC14_toggleConversionTrigger();

	/* Enabling interrupts */
	MAP_ADC14_enableInterrupt(ADC_INT0);
	MAP_Interrupt_enableInterrupt(INT_ADC14);
	MAP_Interrupt_enableMaster();

	while (1)
	{
		MAP_PCM_gotoLPM0();
	}

}

/* ADC Interrupt Handler. This handler is called whenever there is a conversion
 * that is finished for ADC_MEM0.
 */
void adc_isr(void)
{
    uint64_t status = MAP_ADC14_getEnabledInterruptStatus();
    MAP_ADC14_clearInterruptFlag(status);

    if (ADC_INT0 & status)
    {
        curADCResult = MAP_ADC14_getResult(ADC_MEM0);
        normalizedADCRes = (curADCResult * 3.3) / 16384;

        MAP_ADC14_toggleConversionTrigger();
    }
}
