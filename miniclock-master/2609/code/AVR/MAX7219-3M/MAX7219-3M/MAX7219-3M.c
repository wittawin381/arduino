//Development environment :AVR Studio 4.18
//Microcontroller:Atmega64-16AU
//Funtion:display "Welcome to our store " in circularly.We use 2x6pcs 8x8 dot matrix and 32 bytes define a character,so it only display 3 characters each time.
#include <avr/io.h>
#include <avr\pgmspace.h>
#define uchar unsigned char
#define uint  unsigned int
#define NOP()   asm("nop")

#define BIT0 1     //PB0 Pin,connect to CLK of MAX7219.
#define BIT1 2     //PB1 Pin,connect to CS of MAX7219.
#define BIT2 4	   //PB2 Pin,connect to DIN of MAX7219.
#define LED_N 12   //define the quanlity of 8x8 dot matrix MAX7219 module.It is 12 in here. 

#define Max7219_pinCLK0  PORTB &= ~BIT0  //~(1 << PB0)
#define Max7219_pinCLK1  PORTB |=  BIT0   // (1 << PB0)
#define Max7219_pinCS0   PORTB &= ~BIT1  //~(1 << PB1)
#define Max7219_pinCS1   PORTB |=  BIT1   // (1 << PB1)
#define Max7219_pinDIN0  PORTB &= ~BIT2 //~(1 << PB2)
#define Max7219_pinDIN1  PORTB |=  BIT2  // (1 << PB2)

//Original character.
//We define 21 characters in here.There are consist of  16x16 bits (32 bytes) for each character.
//So,you can imagine cut an character to 4 parts: top-right-0,top-left-1,bottom-right-2,bottom-left-3.
//The character is "Welcome to our store ".You can change character.
uchar font[21][32] ={                                                                    
{0x00,0x00,0x00,0x00,0x00,0x00,0x7B,0xCF,0x31,0xC6,0x31,0xC4,0x39,0xCC,0x3B,0xEC,//W
0x1B,0xF8,0x1F,0x78,0x1E,0x78,0x0E,0x70,0x0C,0x30,0x0C,0x20,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xE0,0x0F,0x78,//e
0x18,0x0C,0x3F,0xFE,0x3F,0xFC,0x38,0x00,0x1C,0x0C,0x07,0xF0,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x80,0x0F,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,//l
0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x01,0xC0,0x1F,0xFC,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xE0,0x0F,0x38,//c
0x18,0x1C,0x38,0x00,0x38,0x00,0x38,0x04,0x1C,0x0C,0x0F,0xF8,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xE0,0x0F,0x78,//o
0x38,0x0C,0x30,0x0E,0x70,0x0E,0x38,0x0E,0x18,0x1C,0x0F,0xF0,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x77,0x1C,0x7F,0xFE,//m
0x31,0xC6,0x31,0xC6,0x31,0xC6,0x31,0xC6,0x31,0xC6,0xFB,0xFF,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xE0,0x0F,0x78,//e
0x18,0x0C,0x3F,0xFE,0x3F,0xFC,0x38,0x00,0x1C,0x0C,0x07,0xF0,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,// �ո�
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x03,0x00,0x3F,0xF8,0x3F,0xF8,//t
0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x04,0x03,0x0C,0x01,0xF8,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xE0,0x0F,0x78,//o
0x38,0x0C,0x30,0x0E,0x70,0x0E,0x38,0x0E,0x18,0x1C,0x0F,0xF0,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,// �ո�
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xE0,0x0F,0x78,//o
0x38,0x0C,0x30,0x0E,0x70,0x0E,0x38,0x0E,0x18,0x1C,0x0F,0xF0,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x78,0x3C,0x78,0x3C,//u
0x18,0x1C,0x18,0x1C,0x18,0x1C,0x18,0x1C,0x18,0x3C,0x0F,0xFE,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x3C,0x7E,0xFE,//r
0x0F,0x04,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x7F,0xE0,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,// �ո�
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xE8,0x0E,0x7C,//s
0x18,0x0C,0x0F,0x80,0x03,0xF8,0x10,0x1C,0x18,0x0C,0x1F,0xF8,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x03,0x00,0x3F,0xF8,0x3F,0xF8,//t
0x03,0x00,0x03,0x00,0x03,0x00,0x03,0x04,0x03,0x0C,0x01,0xF8,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xE0,0x0F,0x78,//o
0x38,0x0C,0x30,0x0E,0x70,0x0E,0x38,0x0E,0x18,0x1C,0x0F,0xF0,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x7E,0x3C,0x7E,0xFE,//r
0x0F,0x04,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x0E,0x00,0x7F,0xE0,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0xE0,0x0F,0x78,//e
0x18,0x0C,0x3F,0xFE,0x3F,0xFC,0x38,0x00,0x1C,0x0C,0x07,0xF0,0x00,0x00,0x00,0x00},
{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,// �ո�
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},
};

uchar font_loop[84][8];    //Change font of original character in order to match dot matrix.
						   //It need 4 8x8 dot matrixs for each character, so we must ready	84 space to store data.
uchar  Change_bit( uchar num)//Change sequence of byte,make the bit0 to bit7 and so on.
{
	uchar i;
	uchar temp = 0;
	for( i = 0; i < 8 ; ++i)
	{
		temp <<= 1;
		if(num&0x01) temp |= 0x01;
		num >>= 1;
	}
	return temp;
}
void font_charge( uchar *num)//change font and make original character to font_loop[][].
{                            //num[]:the origrinal character need to change.
	uchar i,j;
	uchar *temp;
	for(j=0;j<21;j++)//it need 4 times to change each one characher.We must change all 21 character. 
	{
		temp = num + j*32; //keep address of top-right-0 for each character
		for(i=0;i<8;i++)
		{
			font_loop[j*2][i]= *temp;
			temp +=2;
		}

		temp = num + j*32;//keep address of top-left-1 for each character
		temp += 1;
		for(i = 0; i < 8 ; ++i)
		{
			font_loop[j*2+1][i] =  *temp;
			temp += 2;   
		}

		temp = num + j*32;//keep address of bottom-right-2 for each character
		temp += 30;
		for(i = 0; i < 8 ; ++i)
		{
			font_loop[83-j*2][i] =Change_bit(*temp) ;
			temp -= 2;                      
		}

		temp = num + j*32;//keep address of bottom-left-3 for each character
		temp += 31;
		for(i = 0; i < 8 ; ++i)
		{
			font_loop[83-j*2-1][i] =Change_bit(*temp) ;
			temp -= 2 ;                       
		}

	}
}
void Delay_xms(uint x)//delay 0.1ms
{
	uint i,j;
	for(i=0;i<x;i++)
		for(j=0;j<100;j++);
}

void Write_Max7219_byte(uchar temp) //write a byte to MAX7219        
{
	uchar i;    
	for(i=8;i >= 1;--i)
	{		  
		Max7219_pinCLK0;       
		if (temp&0x80) Max7219_pinDIN1;
		else  Max7219_pinDIN0;
		temp <<= 1;
		Max7219_pinCLK1;
	}                                 
}
void Write_Max7219(uchar address,uchar dat, uchar sel)//write data to MAX7219.
{
	uchar good;
	Max7219_pinCS0;
	good = LED_N;
	for( ; good > 0; --good )                  //Determine which one equipment operation
	{
		Write_Max7219_byte(0x00);               //Write the address, that is, digital tube number
		Write_Max7219_byte(0x00);               //Write data, that is, digital tube display digital	  
	}
	Write_Max7219_byte(address);           //Write the address, that is, digital tube number
	Write_Max7219_byte(dat);               //Write data, that is, digital tube display digital
	good = sel ;
	for( ; good > 0; --good )                  //Determine which one equipment operation
	{
		Write_Max7219_byte(0x00);               //Write the address, that is, digital tube number
		Write_Max7219_byte(0x00);               //Write data, that is, digital tube display dig
	}
	Max7219_pinCS1;  
}
void Initial_comm( uchar address,uchar dat)
{
	uchar i,good;
	Max7219_pinCS0;
	//good = LED_N;
/*	for( ; good > 0; --good )                  //Determine which one equipment operation
	{
		Write_Max7219_byte(0x00);               //Write the address, that is, digital tube number
		Write_Max7219_byte(0x00);               //Write data, that is, digital tube display digital	  
	}*/

	for( i = 0; i < LED_N ; ++i)
	{
		Write_Max7219_byte(address);
		Write_Max7219_byte(dat);
	}
	Max7219_pinCS1;
}
void Init_MAX7219(void)
{
	DDRB |= ( BIT0+BIT1+BIT2);
	Max7219_pinCLK0;
	Max7219_pinCS0;
	Initial_comm( 0x09,0x00); //Decoding: BCD decoding
	Initial_comm( 0x0a, 0x03);//light intensity
	Initial_comm( 0x0b, 0x07);//Scan line; Eight digital tube display
	Initial_comm( 0x0c, 0x01);//Power down mode: 0, normal mode: 1
	Initial_comm( 0x0f, 0x00);//Display test: 1; End of the test, normal display: 0

}
void write_word( void)//write 3 character to dot matrix.
{
	uchar i ,j;
	for(j = 0; j <= 11 ; ++j)
	{
		if(j<6)//write data to dot matrix in top.
		{
			for(i = 1;i <= 8;++i)
				Write_Max7219(i,font_loop[j][i-1],j);		
		}
		else//write data to matrix in bottom.
		{
			for(i = 1;i <= 8;++i)
				Write_Max7219(i,font_loop[78+j%6][i-1],j);			
		}

	}
}
void Mov_bity(void)//move each character in order to see character move in dot matrix.
{
	uchar temp1,temp2,i,j;
	uchar ktemp1[8],ktemp2[8];
	for(i=0;i<8;i++)
	{
		ktemp1[i]=font_loop[0][i];
		ktemp2[i]=font_loop[83][i];
	}
	for(i=0;i<42;i++)
	{
		for(j=0;j<8;j++)
		{	
			temp1=font_loop[i+1][j];
			temp2=font_loop[82-i][j];
			if(i<41)
			{
				font_loop[i][j] = (font_loop[i][j]<<1) | ((temp1&0x80)>>7);      //move top dot matrix to right
				font_loop[83-i][j] = (font_loop[83-i][j]>>1) | ((temp2&0x01)<<7);//move bottom dot matrix to left
			}
			else
			{
				font_loop[41][j]=(font_loop[41][j]<<1)|((ktemp1[j]&0x80)>>7);//change bit8 of font_loop[0] to bit0 of font_loop[41]
				font_loop[42][j]=(font_loop[42][j]>>1)|((ktemp2[j]&0x01)<<7);//change bit0 of font_loop[83] to bit8 of font_loop[43]
			}
		}
	}
}
int main()
{
	Delay_xms(50);
	Init_MAX7219(); 
	while(1)
	{
		font_charge(*font);//change character 
		write_word();
		while(1)
		{
			Mov_bity();
			write_word();
 			Delay_xms(10);
		}	
		Delay_xms(1);
	}
}