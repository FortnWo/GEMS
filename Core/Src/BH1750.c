#include "BH1750.h"
 
static uint8_t s_MTReg;     /* 灵敏度倍率 */
static uint8_t s_Mode;      /* 测量模式 */
 

BH1750_Dev_t BH1750_Dev={0};


static uint8_t bh1750_WeReg(  uint8_t *pData, uint16_t Size )
{
    HAL_StatusTypeDef status;
    
    status = HAL_I2C_Master_Transmit( &hi2c1, BH1750_SLAVE_ADDRESS, pData, Size, 1000);
    if( status == HAL_OK)
        return BH1750_OK;
    else
        return BH1750_ERROR; 
}
 
static uint8_t bh1750_RdReg( uint8_t *pData, uint16_t Size )
{
    HAL_StatusTypeDef status;
    
    status = HAL_I2C_Master_Receive( &hi2c1, BH1750_SLAVE_ADDRESS, pData, Size, 1000);
    if( status == HAL_OK)
        return BH1750_OK;
    else
        return BH1750_ERROR; 
}
 
void BH1750_ChageMode(uint8_t _ucMode)
{
    uint8_t cmd;
    
    if (_ucMode == 1)          /* 连续高分测量模式1 */
    {
        cmd = BHOP_CON_H_RES;
        bh1750_WeReg(&cmd, 1);    
        s_Mode = 1;             /* 测量模式1，分辨率 1 lux*/
    }
    else if (_ucMode == 2)       /* 连续高分测量模式2 */
    {
        cmd = BHOP_CON_H_RES2;
        bh1750_WeReg(&cmd, 1);  
        s_Mode = 2;              /* 测量模式2, 分辨率 0.5 lux */
    }
    else if (_ucMode == 3)       /* 连续低分测量模式 */
    {
        cmd = BHOP_CON_L_RES;
        bh1750_WeReg(&cmd, 1);
        s_Mode = 3;              /* 测量模式3，低分辨率 4 lux*/
    }
}
 
void BH1750_AdjustSensitivity(uint8_t _ucMTReg)
{
    uint8_t cmd;
    
    if (_ucMTReg <= 31)
    {
        _ucMTReg = 31;
    }
    else if (_ucMTReg >= 254)
    {
        _ucMTReg = 254;
    }
    
    s_MTReg = _ucMTReg;
    
    cmd = 0x40 + (s_MTReg >> 5);
    bh1750_WeReg(&cmd,1);             /* 更改高3bit */
    
    cmd = 0x60 + (s_MTReg & 0x1F);
    bh1750_WeReg(&cmd,1);             /* 更改低5bit */
    
    /*　更改量程范围后，需要重新发送命令设置测量模式　*/
    BH1750_ChageMode(s_Mode);
}
 
uint16_t BH1750_ReadData(void)
{
    uint8_t buff[2];
 
    bh1750_RdReg( buff, 2);
    return (buff[0] << 8) + buff[1];
}
 
void BH1750_GetLux(void)
{
    uint16_t usLight;
    float lux;
    
    usLight = BH1750_ReadData();
    
    /* 
     计算光强度 = 16位寄存器值 / 1.2  * (69 / X)
    */
    lux = (float)(usLight * 5 * 69) / (6 * s_MTReg);
    
    if (s_Mode == 2)     /* 高分辨率测量模式2 */
    {
        lux = lux / 2;
    }
    
    BH1750_Dev.light = (uint16_t)lux;
}
 
void BH1750_Init(void)
{
	uint8_t cmd;
	cmd = BHOP_POWER_ON;             /* 芯片上电 */
	bh1750_WeReg(&cmd, 1);

	BH1750_ChageMode(2);             /* 高分辨率连续测量 */

	BH1750_AdjustSensitivity(69);    /* 芯片缺省灵敏度倍率 = 69 */
}



void BH1750_test( void )
{
	BH1750_GetLux();
    printf("Ambient Light = %d \r\n", BH1750_Dev.light);
}
 
 
 
/* End of this file */







