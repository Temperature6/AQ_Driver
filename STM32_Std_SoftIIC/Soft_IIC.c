#include "Soft_IIC.h"
#include "stdio.h"

void SoftIIC_Init()
{
	if 		(SOFT_PORT == GPIOA) 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if (SOFT_PORT == GPIOB) 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else if (SOFT_PORT == GPIOC) 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	else if (SOFT_PORT == GPIOD) 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	else if (SOFT_PORT == GPIOE) 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
	else if (SOFT_PORT == GPIOF) 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);
	else if (SOFT_PORT == GPIOG) 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);
	
	
	GPIO_InitTypeDef io_init;
	io_init.GPIO_Mode = GPIO_Mode_Out_OD;
	io_init.GPIO_Pin = SDA_PIN | SCL_PIN;
	io_init.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(SOFT_PORT, &io_init);
	
	SDA_Write(1);
	SCL_Write(1);
}

void SoftIIC_Test()
{
	Start();
	Send_Byte(0xA0);
	
	printf("%d", Recv_ACK());
	Stop();
}

uint8_t isDeviceReady(uint8_t addr)
{
	Start();
	Send_Byte(addr);
	uint8_t recv_bit = Recv_ACK();
	Stop();
	
	return recv_bit == SoftIIC_OK;
}

uint8_t SoftIIC_WriteByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t RegAddrSize, uint8_t Data)
{
	uint8_t recv_bit = 0x00;
	
	Start();
	Send_Byte(DevAddr); 				//发送从机地址
	recv_bit |= Recv_ACK();				//发送应答位
	
	if (RegAddrSize == SoftIIC_AddrSize_8Bit)
	{
		Send_Byte(RegAddr);				//发送寄存器地址，设置设备的当前地址指针
		recv_bit |= Recv_ACK();	
	}
	else
	{
		Send_Byte(RegAddr / 256);		//发送寄存器地址高8位，设置设备的当前地址指针
		recv_bit |= Recv_ACK();	
		Send_Byte(RegAddr % 256);		//发送寄存器地址低8位，设置设备的当前地址指针
		recv_bit |= Recv_ACK();	
	}
	
	Send_Byte(Data);	//发送数据
	recv_bit |= Recv_ACK();
	
	Stop();
	
	return recv_bit != 0;
}

uint8_t SoftIIC_ReadByte(uint8_t DevAddr, uint8_t RegAddr, uint8_t RegAddrSize, uint8_t* pData)
{
	uint8_t recv_bit = 0x00;
	
	Start();
	Send_Byte(DevAddr); 		//发送从机地址
	recv_bit |= Recv_ACK();		//发送应答位
	
	if (RegAddrSize == SoftIIC_AddrSize_8Bit)
	{
		Send_Byte(RegAddr);		//发送寄存器地址，设置设备的当前地址指针
		recv_bit |= Recv_ACK();	
	}
	else
	{
		Send_Byte(RegAddr / 256);		//发送寄存器地址高8位，设置设备的当前地址指针
		recv_bit |= Recv_ACK();	
		Send_Byte(RegAddr % 256);		//发送寄存器地址低8位，设置设备的当前地址指针
		recv_bit |= Recv_ACK();	
	}
	
	
	Start(); 					//重复起始条件
	
	Send_Byte(DevAddr | 0x01);	//指定读写位
	recv_bit |= Recv_ACK();
	
	*pData = Recv_Byte();	//读一个字节
	Send_ACK(1);
	
	Stop();
	
	return recv_bit != 0;
}

uint8_t SoftIIC_WriteMem(uint8_t DevAddr, uint8_t RegAddr, uint8_t RegAddrSize, uint8_t* pData, uint16_t Len)
{
	uint8_t recv_bit = 0x00;
	
	Start();
	Send_Byte(DevAddr); 				//发送从机地址
	recv_bit |= Recv_ACK();				//发送应答位
	
	if (RegAddrSize == SoftIIC_AddrSize_8Bit)
	{
		Send_Byte(RegAddr);				//发送寄存器地址，设置设备的当前地址指针
		recv_bit |= Recv_ACK();	
	}
	else
	{
		Send_Byte(RegAddr / 256);		//发送寄存器地址高8位，设置设备的当前地址指针
		recv_bit |= Recv_ACK();	
		Send_Byte(RegAddr % 256);		//发送寄存器地址低8位，设置设备的当前地址指针
		recv_bit |= Recv_ACK();	
	}
	
	//连续写不需要重复起始条件
	//Start(); 							//重复起始条件
	//Send_Byte(DevAddr);
	//recv_bit |= Recv_ACK();
	
	for (uint16_t i = 0; i < Len; i++)
	{
		Send_Byte(pData[i]);			//发一个字节
		recv_bit |= Recv_ACK();
		
		if (recv_bit != SoftIIC_OK)
			break;
	}
	Stop();
	
	return recv_bit != 0;
}

uint8_t SoftIIC_ReadMem(uint8_t DevAddr, uint8_t RegAddr, uint8_t RegAddrSize, uint8_t* pData, uint16_t Len)
{
	uint8_t recv_bit = 0x00;
	
	Start();
	Send_Byte(DevAddr); 		//发送从机地址
	recv_bit |= Recv_ACK();		//发送应答位
	
	if (RegAddrSize == SoftIIC_AddrSize_8Bit)
	{
		Send_Byte(RegAddr);		//发送寄存器地址，设置设备的当前地址指针
		recv_bit |= Recv_ACK();	
	}
	else
	{
		Send_Byte(RegAddr / 256);		//发送寄存器地址高8位，设置设备的当前地址指针
		recv_bit |= Recv_ACK();	
		Send_Byte(RegAddr % 256);		//发送寄存器地址低8位，设置设备的当前地址指针
		recv_bit |= Recv_ACK();	
	}
	
	
	Start(); 					//重复起始条件
	Send_Byte(DevAddr | 0x01);	//指定读写位
	recv_bit |= Recv_ACK();
	
	for (uint16_t i = 0; i < Len; i++)
	{
		if (i != Len - 1)
		{
			pData[i] = Recv_Byte();	//读一个字节
			Send_ACK(0);
		}
		else
		{
			pData[i] = Recv_Byte();	//读一个字节
			Send_ACK(1);
		}
		
	}
	Stop();
	
	return recv_bit != 0;
}

static void Start()
{	
	//SCL,SDA高
	SDA_Write(1);
	SCL_Write(1);
	
	SDA_Write(0);	//先拉低SDA
	SCL_Write(0);	//再拉低SCL
}

static void Stop()
{
	SDA_Write(0);
	
	SCL_Write(1);
	SDA_Write(1);
}

static void Send_Byte(uint8_t byte)
{
	uint8_t i;
	for (i = 0; i < 8; i++)
	{
		//任何操作（除了终止条件）都应以SCL低电平结尾，此时SCL已经是低电平
		
		//SCL低电平期间，主机把数据放到SDA上
		SDA_Write((byte << i) & 0x80);
		//SCL高电平，让从机读取数据
		SCL_Write(1);
		//SCL低电平
		SCL_Write(0);
	}
}

static uint8_t Recv_Byte()
{
	uint8_t byte = 0x00;
	//释放SDA，等待从机将数据放到SDA
	SDA_Write(1);
	//释放SCL，主机读取SDA
	for (int i = 7; i >= 0; i--)
	{
		SCL_Write(1);
		byte |= SDA_Read() << i;
		SCL_Write(0);
	}
	
	return byte;
}

static void Send_ACK(uint8_t Bit_ACK)
{
	SDA_Write(Bit_ACK);
	//SCL高电平，让从机读取应答
	SCL_Write(1);
	//SCL低电平
	SCL_Write(0);
}

static uint8_t Recv_ACK()
{
	uint8_t byte = 0x00;
	
	//释放SDA
	SDA_Write(1);
	//释放SCL，主机读取SDA
	SCL_Write(1);
	byte = SDA_Read();
	SCL_Write(0);
	
	return byte;
}

static void SCL_Write(uint8_t state)
{
	GPIO_WriteBit(SOFT_PORT, SCL_PIN, (BitAction)state);
	
#ifdef DELAY_ENABLE
	delay_us(DELAY_VALUE_US);
#endif //DELAY_ENABLE
	
}

static void SDA_Write(uint8_t state)
{
	GPIO_WriteBit(SOFT_PORT, SDA_PIN, (BitAction)state);
	
#ifdef DELAY_ENABLE
	delay_us(DELAY_VALUE_US);
#endif //DELAY_ENABLE
	
}

static uint8_t SDA_Read()
{
	uint8_t bit = (GPIO_ReadInputDataBit(SOFT_PORT, SDA_PIN) == Bit_SET);
	
#ifdef DELAY_ENABLE
	delay_us(DELAY_VALUE_US);
#endif //DELAY_ENABLE
	
	return bit;
}

//This function work well on STM32F103C8T6 @ 72MHz
static void delay_us(uint32_t xus)
{
	volatile unsigned int i;
	for (i = 0; i < xus; i++)
	{
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop(); 	//*9
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop(); 	//*9
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop(); 	//*9
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop(); 	//*9
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop(); 	//*9
		__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop();__nop(); 	//*9
		__nop();__nop();__nop();__nop();											//*4
	}
}
