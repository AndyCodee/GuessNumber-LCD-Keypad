#include <REGX52.H>

// Pin definitions
sbit LCD_RS=P2^6;
sbit LCD_RW=P2^5;
sbit LCD_EN=P2^7;
#define LCD_DataPort P0


// ---------------------------- Functions ----------------------------


/**
  * @brief Generates a delay (~1ms at 12MHz clock)
  * @param None
  * @retval None
  */
void LCD_Delay()
{
    unsigned char i, j;

    i = 2;
    j = 239;
    do
    {
        while (--j);
    } while (--i);
}


/**
  * @brief Sends a command byte to the LCD1602
  * @param Command: The command byte to send
  * @retval None
  */
void LCD_WriteCommand(unsigned char Command)
{
    LCD_RS=0;
    LCD_RW=0;
    LCD_DataPort=Command;
    LCD_EN=1;
    LCD_Delay();
    LCD_EN=0;
    LCD_Delay();
}


/**
  * @brief Sends a data byte to the LCD1602
  * @param Data: The character or data byte to display
  * @retval None
  */
void LCD_WriteData(unsigned char Data)
{
    LCD_RS=1;
    LCD_RW=0;
    LCD_DataPort=Data;
    LCD_EN=1;
    LCD_Delay();
    LCD_EN=0;
    LCD_Delay();
}


/**
  * @brief Sets the cursor position on the LCD
  * @param Line: Line number (1~2)
  * @param Column: Column number (1~16)
  * @retval None
  */
void LCD_SetCursor(unsigned char Line,unsigned char Column)
{
    if(Line==1)
    {
        LCD_WriteCommand(0x80|(Column-1));
    }
    else if(Line==2)
    {
        LCD_WriteCommand(0x80|(Column-1+0x40));
    }
}


/**
  * @brief Initializes the LCD1602 display
  * @param None
  * @retval None
  */
void LCD_Init()
{
    LCD_WriteCommand(0x38); // 8-bit mode, 2 lines, 5x7 font
    LCD_WriteCommand(0x0c); // Display ON, cursor OFF, blink OFF
    LCD_WriteCommand(0x06); // Auto-increment cursor, no shift
    LCD_WriteCommand(0x01); // Clear display
}


/**
  * @brief Displays a single character at the specified position
  * @param Line: Line number (1~2)
  * @param Column: Column number (1~16)
  * @param Char: The character to display
  * @retval None
  */
void LCD_ShowChar(unsigned char Line,unsigned char Column,char Char)
{
    LCD_SetCursor(Line,Column);
    LCD_WriteData(Char);
}


/**
  * @brief Displays a string starting from the specified position
  * @param Line: Starting line (1~2)
  * @param Column: Starting column (1~16)
  * @param String: Null-terminated string to display
  * @retval None
  */
void LCD_ShowString(unsigned char Line,unsigned char Column,char *String)
{
    unsigned char i;
    LCD_SetCursor(Line,Column);
    for(i=0;String[i]!='\0';i++)
    {
        LCD_WriteData(String[i]);
    }
}


/**
  * @brief Computes X raised to the power of Y
  * @param X: Base value
  * @param Y: Exponent value
  * @retval Result of X^Y
  */
int LCD_Pow(int X,int Y)
{
    unsigned char i;
    int Result=1;
    for(i=0;i<Y;i++)
    {
        Result*=X;
    }
    return Result;
}


/**
  * @brief Displays an unsigned integer at the specified position
  * @param Line: Starting line (1~2)
  * @param Column: Starting column (1~16)
  * @param Number: Number to display (0~65535)
  * @param Length: Number of digits to display (1~5)
  * @retval None
  */
void LCD_ShowNum(unsigned char Line,unsigned char Column,unsigned int Number,unsigned char Length)
{
    unsigned char i;
    LCD_SetCursor(Line,Column);
    for(i=Length;i>0;i--)
    {
        LCD_WriteData(Number/LCD_Pow(10,i-1)%10+'0');
    }
}



