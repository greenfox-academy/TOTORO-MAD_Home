/* Includes ------------------------------------------------------------------*/
#include "iic.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables --------------------------------------------------------*/
GPIO_InitTypeDef GPIO_InitStruct;
I2C_HandleTypeDef i2c_handle;
uint8_t data = 0;
uint8_t temperature;

/* TCP client variables */
#define I2C_TIMING		   0xD0101671 // 40kHz, rise 100ns, fall 10ns
#define I2C_ADDRESS        0b10010000
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
void i2c_init() {
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_I2C1_CLK_ENABLE();

	GPIO_InitStruct.Pin = GPIO_PIN_8 | GPIO_PIN_9;
	GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	i2c_handle.Instance             = I2C1;
	i2c_handle.Init.Timing          = I2C_TIMING;
	i2c_handle.Init.AddressingMode  = I2C_ADDRESSINGMODE_7BIT;
	i2c_handle.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
	i2c_handle.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
	i2c_handle.Init.NoStretchMode   = I2C_NOSTRETCH_DISABLE;

	if (HAL_I2C_Init(&i2c_handle) == HAL_OK) {
	  printf("i2c init ok\n");
	}

}

void get_temp() {

//	if (HAL_I2C_IsDeviceReady(&i2c_handle, (uint16_t)I2C_ADDRESS, 10, 10000) != HAL_OK)
//		printf("device is not rdy\n");
//	if (HAL_I2C_Master_Receive(&i2c_handle, (uint16_t)I2C_ADDRESS, &temperature, 1, 10000) != HAL_OK)
//		printf("receive doesn't work\n");

	i2c_handle.Instance->CR2 |= I2C_CR2_START;

//	printf("%d\n", HAL_I2C_Mem_Read(&i2c_handle, (uint16_t)I2C_ADDRESS, 0x00, 1, &temperature, 1, 1000));

	printf("temperature: %d\n", temperature);

}

