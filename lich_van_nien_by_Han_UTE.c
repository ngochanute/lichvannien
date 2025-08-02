#INCLUDE <16F887.H>      
#FUSES HS,PUT
#use delay(clock=20M)  
                               

#define LCD_ENABLE_PIN  PIN_D2                                   
#define LCD_RS_PIN      PIN_D0                                   
#define LCD_RW_PIN      PIN_D1                                   
#define LCD_DATA4       PIN_D4                                   
#define LCD_DATA5       PIN_D5                                    
#define LCD_DATA6       PIN_D6                                   
#define LCD_DATA7       PIN_D7                            
#include <tv_lcd.c>  
#include <dht11.h>                                      
 
//LCD MA 7 DOAN               
UNSIGNED INT8 DOAN_F[8] ={0x07,0x0F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F};  //DOAN F - 0
UNSIGNED INT8 DOAN_A[8] ={0x1F,0x1F,0x1F,0X00,0X00,0X00,0X00,0X00};  //DOAN A - 1
UNSIGNED INT8 DOAN_B[8] ={0x1C,0x1E,0x1F,0x1F,0x1F,0x1F,0x1F,0x1F};  //DOAN B - 2
UNSIGNED INT8 DOAN_D[8] ={0X00,0X00,0X00,0X00,0X00,0x1F,0x1F,0x1F};  //DOAN D - 3
UNSIGNED INT8 DOAN_C[8] ={0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x1E,0x1C};  //DOAN C - 4
UNSIGNED INT8 DOAN_E[8] ={0x1F,0x1F,0x1F,0x1F,0x1F,0x1F,0x0F,0x07};  //DOAN E - 5
UNSIGNED INT8 DOAN_G[8] ={0x1F,0x1F,0x1F,0X00,0X00,0X00,0x1F,0x1F};                       
// DOAN I SU DUNG MA CUA SAN TRONG BANG MA ASCII LA 255
                                          
//GHEP CAC DOAN THANH MA SO LON
CONST UNSIGNED CHAR LCD_SO_Z[11][6] ={
                  0,1,2,5,3,4,               // SO 0
                  1,2,32,3,255,3,            // SO 1
                  6,6,2,5,3,3,               // SO 2
                  6,6,2,3,3,4,               // SO 3
                  255,3,255,32,32,255,       // SO 4
                  255,6,6,3,3,4,             // SO 5
                  0,6,6,5,3,4,               // SO 6
                  1,1,255,32,32,255,         // SO 7
                  0,6,2,5,3,4,               // SO 8          
                  0,6,2,3,3,4,
                  32,32,32,32,32,32};              // SO 9



VOID KHOITAO_CGRAM()               
{       
    //LCD_SET_CGRAM_CHAR(W, *P)
    LCD_SET_CGRAM_CHAR(0, DOAN_F);
    LCD_SET_CGRAM_CHAR(1, DOAN_A);  
    LCD_SET_CGRAM_CHAR(2, DOAN_B);
    LCD_SET_CGRAM_CHAR(3, DOAN_D);
    LCD_SET_CGRAM_CHAR(4, DOAN_C);           
    LCD_SET_CGRAM_CHAR(5, DOAN_E);
    LCD_SET_CGRAM_CHAR(6, DOAN_G);
}
VOID SO_LON_Z_TOADO_XY(UNSIGNED INT8 SO_Z,X1,Y1)
{                  
    UNSIGNED INT8 J;
    FOR (J=0;J<6;J++)
    {
      IF(J<3) LCD_GOTOXY(X1+J,Y1); 
      ELSE    LCD_GOTOXY((X1+J)-3,Y1+1);
                       
      LCD_PUTC(LCD_SO_Z[SO_Z][J]);
      DELAY_MS(1);
    }    
}
#USE i2c(master,slow,SDA=PIN_C4,SCL=PIN_C3)
                                  
#define mod PIN_B1     
#define inc PIN_B2
                                   
char calendar[] ="         /  /20  ";
unsigned int8 giay0,phut0,gio0,giay1,phut1,gio1,giay,phut,gio,ngay,thang,nam,thu ;
                                 
unsigned int8 dh_nhiet_do, dh_nhiet_do2, dh_do_am, dh_do_am2;
unsigned int8 str[20];       
// hien thi thoi gian len LCD 16x2|20x4   
void ds1307_display()         
{                    
   ///chuyen doi BCD sang nhi phan
   ///x = (x >> 4) * 10 + (x & 0x0F) 
   giay = (giay >> 4) * 10 + (giay & 0x0F);
   phut = (phut >> 4) * 10 + (phut & 0x0F);
   gio = (gio >> 4) * 10 + (gio & 0x0F); 
   thu = (thu >> 4) * 10 + (thu & 0x0F);
   ngay = (ngay >> 4) * 10 + (ngay & 0x0F);
   thang = (thang >> 4) * 10 + (thang & 0x0F);
   nam = (nam >> 4) * 10 + (nam & 0x0F);
   //ket thuc chuyen doi
   giay0     = giay % 10;// + 48;
   giay1     = giay / 10;// + 48; 
   phut0      = phut % 10;// + 48;
   phut1      = phut / 10;// + 48;
   gio0      = gio % 10 ;//+ 48;
   gio1      = gio / 10 ;//+ 48;
   calendar[16] = nam % 10 + 48;
   calendar[15] = nam / 10 + 48;             
   calendar[11] = thang % 10 + 48;
   calendar[10] = thang / 10 + 48;
   calendar[8] = ngay % 10 + 48;
   calendar[7] = ngay / 10 + 48; 
   lcd_gotoxy(1,1);                
   switch(thu)      
   {
     case 2: lcd_putc("MON"); break;     
     case 3: lcd_putc("TUE"); break;                
     case 4: lcd_putc("WED"); break;
     case 5: lcd_putc("THU"); break;
     case 6: lcd_putc("FRI"); break;
     case 7: lcd_putc("SAT"); break; 
     case 8: lcd_putc("SUN"); break;
   }                              
   lcd_gotoxy(4,1);       
   printf(lcd_putc, calendar);
}

// chuong trinh ghi vao ds1307
void ds1307_write(unsigned int8 address, data_)
{                
   i2c_start();
   i2c_write(0xD0);    //dia chi cua ds1307
   i2c_write(address); //ghi vao dia chi thanh ghi 
   i2c_write(data_);   // ghi data 
   i2c_stop();                                                 
}              
                         
//chuong trinh dieu chinh thoi gian
void dieu_chinh_time_va_calender()
{
   if(!input(mod))           
   {              
        while(true)         
        {       
          if(!input(inc)) phut++;      
          if(phut>59)     phut=0;  
          phut0 = phut % 10;        
          phut1 = phut / 10;       
          SO_LON_Z_TOADO_XY(phut1,8,2);        
          SO_LON_Z_TOADO_XY(phut0,11,2);        
          delay_ms(100);          
          SO_LON_Z_TOADO_XY(10,8,2);                   
          SO_LON_Z_TOADO_XY(10,11,2);
          delay_ms(100);
          if(!input(mod)) break;               
          delay_ms(20);
        } 
        SO_LON_Z_TOADO_XY(phut1,8,2);
        SO_LON_Z_TOADO_XY(phut0,11,2);
        while(!input(mod));
        // chinh gio //////////////////////////////
        while(true)
        {
          if(!input(inc)) gio++;      
          if(gio>23)      gio=0;     
          gio0 = gio % 10 ;  
          gio1 = gio / 10 ;                
          SO_LON_Z_TOADO_XY(gio1,1,2);    
          SO_LON_Z_TOADO_XY(gio0,4,2);
          delay_ms(100);
          SO_LON_Z_TOADO_XY(10,1,2);
          SO_LON_Z_TOADO_XY(10,4,2);
          delay_ms(100); 
          if(!input(mod)) break; 
          delay_ms(20);
        }
        SO_LON_Z_TOADO_XY(gio1,1,2);    
        SO_LON_Z_TOADO_XY(gio0,4,2);
        while(!input(mod));  
        ////chinh thu /////////////////////////////
        while(true)
        {                                 
          if(!input(inc))  thu++;      
          if(thu>8)      thu=2;
          lcd_gotoxy(1,1);   
          lcd_putc("   ");
          delay_ms(100);
          lcd_gotoxy(1,1);
          switch(thu)                                               
          {
             case 2: lcd_putc("MON"); break;
             case 3: lcd_putc("TUE"); break;                
             case 4: lcd_putc("WED"); break;   
             case 5: lcd_putc("THU"); break;
             case 6: lcd_putc("FRI"); break;
             case 7: lcd_putc("SAT"); break; 
             case 8: lcd_putc("SUN"); break;
          }
          delay_ms(100);             
          if(!input(mod)) break; 
          delay_ms(20);
        }  
        while(!input(mod));  
        //chinh ngay  ////////////////////////////////                               
        while(true)                                   
        {                                 
          if(!input(inc))  ngay++;      
          if(ngay>31)      ngay=0;  
          lcd_gotoxy(11,1);   
          printf(lcd_putc, "%02u",ngay);
          delay_ms(100);        
          lcd_gotoxy(11,1);   
          lcd_putc("  ");
          delay_ms(100);
          if(!input(mod)) break; 
          delay_ms(20);
        } 
        lcd_gotoxy(11,1);
        printf(lcd_putc, "%02u",ngay);   
        while(!input(mod));
        // chinh thang ///////////////////////////////////       
        while(true)
        {                                 
          if(!input(inc))   thang++;      
          if(thang>12)      thang=1;  
          lcd_gotoxy(14,1); 
          printf(lcd_putc, "%02u",thang); 
          delay_ms(100);        
          lcd_gotoxy(14,1);   
          lcd_putc("  ");
          delay_ms(100);
          if(!input(mod)) break;  
          delay_ms(20);
        }  
        lcd_gotoxy(14,1);
        printf(lcd_putc, "%02u",thang);
        while(!input(mod));   
        // chinh nam /////////////////////////////////////           
        while(true)   
        {                                 
          if(!input(inc))   nam++;      
          if(nam>99)        nam=0;  
          lcd_gotoxy(19,1);  
          printf(lcd_putc, "%02u",nam);   
          delay_ms(100);        
          lcd_gotoxy(19,1);   
          lcd_putc("  ");     
          delay_ms(100);       
          if(!input(mod)) break;  
          delay_ms(20); 
        }   
        lcd_gotoxy(19,1); 
        printf(lcd_putc, "%02u",nam);
        while(!input(mod)); 
        while(true)
        {   
          lcd_gotoxy(1,1);                          
          lcd_putc("SVTH:NGUYEN NGOC HAN"); 
          lcd_gotoxy(1,2);   
          lcd_putc("MSSV:       21161397");
          lcd_gotoxy(1,3);   
          lcd_putc("GVHD:NGUYEN VAN HIEP");
          lcd_gotoxy(1,4);   
          lcd_putc("TRUONG DH SPKT TPHCM");
          if(!input(mod)) 
          {      
            // chuyen nhi phan sang BCD de luu vao ds1307
            // x = ((x / 10 << 4) + (x % 10))
            phut  = ((phut / 10 << 4) + (phut % 10));
            gio   = ((gio / 10 << 4) + (gio % 10)); 
            ngay  = ((ngay / 10 << 4) + (ngay % 10));
            thang = ((thang / 10 << 4) + (thang % 10));
            nam   = ((nam / 10 << 4) + (nam % 10));
            delay_ms(10);
            //ket thuc chuyen doi       
            
            // chuyen xong roi ta luu vao ds1307
            ds1307_write(1, phut);  
            ds1307_write(1, gio);  
            ds1307_write(1, thu);
            ds1307_write(1, ngay);
            ds1307_write(1, thang);
            ds1307_write(1, nam);
            ds1307_write(0, 0);       
            
            i2c_stop(); // ket thuc truyen du lieu  
            delay_ms(50);
            lcd_putc('\f');  
            while(!input(mod));
            break; 
          }  
          
        }                          
     }       
}
  
void hienthinhietdo()
{                
                                      
   if (DHT_GetTemHumi(dh_nhiet_do, dh_nhiet_do2, dh_do_am, dh_do_am2))
   {                       
      sprintf(str, "TEMP:%2u", dh_nhiet_do);
      lcd_gotoxy(1,4); 
      printf(LCD_putc, str);  
      lcd_gotoxy(8,4); 
      lcd_putc(0xdf);    
      lcd_gotoxy(9,4); 
      lcd_putc("C"); 
                       
      sprintf(str, "HUM:%2u", dh_do_am);
      lcd_gotoxy(14,4);            
      printf(LCD_putc, str); 
      lcd_gotoxy(20,4);     
      lcd_putc(0x25);
   }
   delay_ms(20);
}                          
                                                   
void main()
{               
   set_tris_d(0X00);      
   set_tris_b(0Xff);
   lcd_init();  
   KHOITAO_CGRAM(); 
   delay_ms(20); 
   lcd_putc('\f');
   hienthinhietdo();
   
   while(true)             
   {   
      //read data from ds1307 and decode, display to lcd
      i2c_start();     //khoi dong i2c                  
      i2c_write(0xD0); //dia chi cua ds1307  
      i2c_write(0);    //ghi vao dia chi thanh ghi 
      i2c_stop();                        
      i2c_start();     //khoi dong lai i2c      
      i2c_write(0xD1); //khoi tao    
      giay  = i2c_read(1);  
      phut  = i2c_read(1);  
      gio   = i2c_read(1); 
      thu   = i2c_read(1);  
      ngay  = i2c_read(1);         
      thang = i2c_read(1);                                                 
      nam   = i2c_read(0); 
      i2c_stop();  
      //ket thuc doc data va hien thi   
      ds1307_display();             
      //hien thi TIME so lon        
      SO_LON_Z_TOADO_XY(gio1,1,2);
      delay_ms(1);
      SO_LON_Z_TOADO_XY(gio0,4,2);  
      delay_ms(1);
      SO_LON_Z_TOADO_XY(phut1,8,2); 
      delay_ms(1); 
      SO_LON_Z_TOADO_XY(phut0,11,2);   
      delay_ms(1);           
      SO_LON_Z_TOADO_XY(giay1,15,2);   
      delay_ms(1); 
      SO_LON_Z_TOADO_XY(giay0,18,2);   
      delay_ms(1); 
      //hien thi dau 2 cham//////          
      LCD_GOTOXY(7,2);                      
      LCD_PUTC(0xA5); 
      LCD_GOTOXY(7,3);                      
      LCD_PUTC(0xA5);                      
      LCD_GOTOXY(14,2);                      
      LCD_PUTC(0xA5); 
      LCD_GOTOXY(14,3);                      
      LCD_PUTC(0xA5);        
      ///////////////////////////
      dieu_chinh_time_va_calender();  
      delay_ms(10);
      hienthinhietdo();  
      delay_ms(10);
   }                            
}                                   
