
/*
 ============================================================================
 Name        : LCD_16x2
 Author      : Srini
 Created on	 : Jan 7, 2014
 Version     :
 Copyright   :
 Description :
 ============================================================================
*/

/****************************************** Inclusions ****************************************/



/****************************************** Static Variables ****************************************/


#define         RS_CMD          0
#define         RS_DATA         1
#define         DispON_CursorOn_CursorBlink             0x0F
#define         EntryMode_IncrCursorPos_NoDispShft      0x06

#define         LCD_DELAY_MS    2

void print_hex(uint8 cmd_data_fld, char* str) ;
void Send_LCD_Block(uint8 cmd_data_fld, char* str) ;

uint16_t LCD_pin_arr[8] = {
                          LCD_D0_PIN,
                          LCD_D1_PIN,
                          LCD_D2_PIN,
                          LCD_D3_PIN,
                          LCD_D4_PIN,
                          LCD_D5_PIN,
                          LCD_D6_PIN,
                          LCD_D7_PIN
                        } ;

/**
*********************************************************************************************************
* Initialization process
*********************************************************************************************************
*/


void LCD_16x2_Init(void)
{
  char cmd_str[10] = { '\0' } ;

  cmd_str[0] = 0x38 ;           // Function set, 8 bit, 2 lines, 5×7
//  cmd_str[1] = 0x0F ;           // Display ON, Cursor On, Cursor Blinking
  cmd_str[1] = 0x0C ;           // Display on/off Control (Entry Display,Cursor off,Cursor not Blink)
  cmd_str[2] = 0x06 ;           // Entry Mode, Increment cursor position, No display shift
  cmd_str[3] = 0x01 ;           // Clear Display  (Clear Display,Set DD RAM Address=0)
  cmd_str[4] = '\0' ;
  Send_LCD_Block(RS_CMD, cmd_str) ;
    
}

/*----------------------------------------------------*/

void LCD_EN_Ctrl(uint8 EN_pin_value)
{
  Delay(LCD_DELAY_MS); 
  // Set LCD Enable pin to HIGH / LOW
  GPIO_Set(LCD_PORT, LCD_EN_PIN, EN_pin_value );
  Delay(LCD_DELAY_MS); 
}

/*----------------------------------------------------*/

void LCD_RS_Ctrl(uint8 RS_pin_value)
{
  Delay(LCD_DELAY_MS); 
  // Set LCD Register Select to HIGH/LOW
  GPIO_Set(LCD_PORT, LCD_RS_PIN, RS_pin_value );
  Delay(LCD_DELAY_MS); 
}


/*----------------------------------------------------*/

void print_on_LCD(char *str)
{
    Send_LCD_Block(RS_DATA, str) ;
}

/*--------------------------------------------------------------------------*/

void Send_LCD_Block(uint8 cmd_data_fld, char* str)
{
  uint8 bit_idx, char_idx ;
    
  char_idx = 0;
  while(str[char_idx])
  {
    LCD_EN_Ctrl(HIGH);

    GPIO_Set(LCD_PORT, LCD_RS_PIN, cmd_data_fld );
    
    for(bit_idx=0; bit_idx<8; bit_idx++)
      GPIO_Set(LCD_PORT, LCD_pin_arr[bit_idx], ( (str[char_idx]>>(bit_idx) ) & 0x01 ) ); 

    char_idx++;

    LCD_EN_Ctrl(LOW); 
  }

}

/****************************/
/* Write Instruction to LCD */
/****************************/
void lcd_write_control(unsigned char val)
{ 
  uint8 cmd_val[2] ;
                                                                                        // RS = 0 = Instruction Select  
  cmd_val[0] = val ;
  cmd_val[1] = '\0' ;
  Send_LCD_Block(RS_CMD, cmd_val) ;  
}


/***************************/
/* Set LCD Position Cursor */
/***************************/
void goto_cursor(unsigned char i)
{
  i |= 0x80;                                                                                    // Set DD-RAM Address Command
  lcd_write_control(i);  
}

/***************************/
/* Clear LCD  */
/***************************/
void Clear_LCD(void)
{
  char buff[2];
  buff[0] = 0x01 ;           // Clear Display  (Clear Display,Set DD RAM Address=0)
  buff[1] = '\0' ;
  Send_LCD_Block(0, buff) ;
}

/***************************/
/* Scroll LCD Left Once  */
/***************************/
void Scroll_LCD_Left(void)
{
  char buff[2];
  buff[0] = 0x18 ;           // Scroll Display
  buff[1] = '\0' ;
  Send_LCD_Block(0, buff) ;  
}

/***************************/
/* Scroll LCD Right Once  */
/***************************/
void Scroll_LCD_Right(void)
{
  char buff[2];
  buff[0] = 0x1C ;           // Scroll Display
  buff[1] = '\0' ;
  Send_LCD_Block(0, buff) ;  
}

/***************************/
/* Cursor to Home   */
/***************************/
void Cursor_to_Home(void)
{
  char buff[2];
  buff[0] = 0x02 ;           // Scroll Display
  buff[1] = '\0' ;
  Send_LCD_Block(0, buff) ;  
}


/***************************/
/* Cursor shift left    */
/***************************/
void Cursor_shift_Left (void)
{
  char buff[2];
  buff[0] = 0x10 ;           // Scroll Display
  buff[1] = '\0' ;
  Send_LCD_Block(0, buff) ;  
}

/***************************/
/* Cursor shift Right    */
/***************************/
void Cursor_shift_Right (void)
{
  char buff[2];
  buff[0] = 0x14 ;           // Scroll Display
  buff[1] = '\0' ;
  Send_LCD_Block(0, buff) ;  
}
