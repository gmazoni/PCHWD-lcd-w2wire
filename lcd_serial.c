///////////////////////////////////////////////////////////////////////////
////                          lcd_serial.C                             ////
////                 Biblioteca para conexão serial                    ////
////                     com displays LCD 2x16.                        ////
////                Otimizada para altas frequências.                  ////
////                                                                   ////
////              Uso do CI 74LS174 como shift register.               ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////  lcd_init()   Deve ser chamada antes de qualquer função           ////
////                       desta biblioteca.                           ////
////                                                                   ////
////  lcd_putc(c)  Exibe o valor de c na próxima posição do display.   ////
////                     Os seguintes caracteres são especiais:        ////
////                      \f  Limpa o display.                         ////
////                      \n  Vai para o início da segunda linha       ////
////                      \b  Volta uma posiposiçãoo                   ////
////                                                                   ////
////  lcd_gotoxy(x,y) Vai para a posição x e y do display.             ////
////                         (canto superior esquerdo 1,1)             ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////
////                                                                   ////
////                     2011 - Guilherme Mazoni                       ////
//// PINAGEM PADRÃO:                                                   ////
//// #define LCDCLK PIN_B0  define o pino B0 como o clock.             ////
//// #define LCDDAT PIN_B1  define o pino B1 como data.                ////
////                                                                   ////
//// Caso seja necessário alterar os pinos:                            ////
//// (Faça o #define antes do include desta biblioteca)                ////
////                                                                   ////
//// #define LCDCLK PIN_XX  define o pino como o clock.                ////
//// #define LCDDAT PIN_XX  define o pino como data.                   ////
////                                                                   ////
///////////////////////////////////////////////////////////////////////////

#ifndef LCDCLK
#define LCDCLK PIN_B0
#endif

#ifndef LCDDAT
#define LCDDAT PIN_B1
#endif

void lcd_es()
{
output_high(LCDDAT);
delay_us(5); //delay_us(100);   
output_low(LCDDAT);
}

void lcd_Clk()
{
output_high(LCDCLK);
delay_us(5); //delay_cycles(1);
output_low(LCDCLK);

}

void lcd_send_nibble(char nibble, int1 rs)
{
      int8 i;

      output_low(LCDDAT);
      for(i=0; i<6; i++)
         lcd_Clk();
     
      output_high(LCDDAT);
      lcd_Clk();

      if(rs==1)
      output_high(LCDDAT);
      else
      output_low(LCDDAT);
      
      lcd_Clk();

      for (i = 0; i<4; i++)
         {
         if(nibble & 0x08)
            output_high(LCDDAT);
         else
            output_low(LCDDAT);
         lcd_Clk();
         nibble<<=1;
         }
      lcd_es(); 
}


void lcd_send_byte(int n , int1 rs)
{
      lcd_send_nibble(n >> 4 , rs);
      lcd_send_nibble(n & 0xf, rs);
}


void lcd_init(void)
{
    lcd_send_nibble(0x03,0);
    delay_ms(5);
    lcd_es();
    delay_us(160);
    lcd_es();
    delay_us(160);
    lcd_send_nibble(0x02,0);
    delay_us(160);
    lcd_send_byte(0x28,0);
    lcd_send_byte(0x08,0);
    lcd_send_byte(0x01,0);
    delay_ms(5);
    lcd_send_byte(0x06,0);
    lcd_send_byte(0x0C,0);
}


void lcd_gotoxy(int x, int y)
{
   BYTE address;
   if(y!=1) 
      address=0x40;
   else         
      address=0;
      
   address+=x-1;
   lcd_send_byte(0x80|address,0);
}


void lcd_putc(char c)
{
   switch (c)
   {
     case '\f': 
     lcd_send_byte(1,0); 
     delay_ms(5);
     break;
     
     case '\n': 
     lcd_gotoxy(1,2);
     break;
     
     case '\b': 
     lcd_send_byte(0x10,0);  
     break;
     
     default: 
     lcd_send_byte(c,1);
     break;
   }
}
