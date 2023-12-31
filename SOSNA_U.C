/*_____________________________________________________________ */
/* sosna_u.c - Основной управляющий файл для ПР 7084.32.01.400  */
/* 03.01.2003 __________________________________________________*/
/* Разработал: Воложин А.Г. ОАО "Пеленг"________________________*/
/*______________________________________________________________*/


#include <intrins.h>
#include <bios.h>

extern bit fl_start;
extern   xdata uchar _RG_INTF;
extern   xdata uchar _RG_MASK;
extern   xdata uchar _RG_DAC_L;
extern   xdata uchar _RG_DAC_H;
extern   xdata uchar _RG_BV_CTRL;
extern   xdata uchar _RG_ERR;
extern   xdata uchar _RG_PU_GN_H;
extern   xdata uchar _RG_PU_GN_L;
extern   xdata uchar _RG_PU_VN_H;
extern   xdata uchar _RG_PU_VN_L;
extern   xdata uchar _RG_CTRL;
extern   xdata uchar _RG_RAM[15];
extern   xdata uchar _RG_TX_BV[15];
extern   xdata uchar _RG_RX_BV[15];
extern   xdata uchar _RG_0_VN_L;
extern   xdata uchar _RG_0_VN_H;
extern   xdata uchar _RG_0_GN_L;
extern   xdata uchar _RG_0_GN_H;
extern   xdata uchar _RG_UVKV_VN_L;
extern   xdata uchar _RG_UVKV_VN_H;
extern   xdata uchar _RG_UVKV_GN_L;
extern   xdata uchar _RG_S_EL_L;
extern   xdata uchar _RG_S_EL_H;
extern   xdata uchar _RG_A_AZ_L;
extern   xdata uchar _RG_A_AZ_H;
extern   xdata uchar _RG_UV_VN_L;
extern   xdata uchar _RG_UV_VN_H;
extern   xdata uchar _RG_UV_GN_L;
extern   xdata uchar _RG_UV_GN_H;

//
//
/************************************************/
/************* Начальная инициализация **********/
/************************************************/
  /*_____________________ Начальная инициализация __________________________*/
/* 1 - OKEY ,0 - ERROR */
void Begin_init(void)
{
 EA=0;                        /* запрет всех прерываний */
// Delay_1ms(200);              /* задержка на всякий случай !!! */

 /*  запись нулевых регулировок для настройки */
 /* ПС */
 _RG_DAC_L=0x00;
 _RG_DAC_H=0x80;
 Delay_1ms(1);
/* Рег. 0 Гн */
 _RG_DAC_L=0x01;
 _RG_DAC_H=0x80;
 Delay_1ms(1);
 /* Поправка ГН */
 _RG_DAC_L=0x02;
 _RG_DAC_H=0x80;
 Delay_1ms(1);
 /* Увод по ГН */
 _RG_DAC_L=0x03;
 _RG_DAC_H=0x80;
 Delay_1ms(1);

 /* Масштаб */
 _RG_DAC_L=0xFC;
 _RG_DAC_H=0x7F;
 Delay_1ms(1);
/* Рег. 0 Вн */
 _RG_DAC_L=0x0D;
 _RG_DAC_H=0x80;
 Delay_1ms(1);
 /* Поправка ВН */
 _RG_DAC_L=0x0E;
 _RG_DAC_H=0x80;
 Delay_1ms(1);
 /* Увод по ВН */
 _RG_DAC_L=0x0F;
 _RG_DAC_H=0x80;
 Delay_1ms(1);
 //
 /* Настройка таймеров  и внешних прерываний */
 /* TMR0 - 16 bit -timer на 1мс ,interrupt - enable */
 TMOD=0x11; // задает режим 16 bit
 TH0=_C_1000MKS >>8;
 TL0=(char)(_C_1000MKS);
 /* TMR1 - 16 bit -timer на 20мс ,interrupt - enable */
 TL1=(char)_C_20MS;
 TH1=_C_20MS >>8;
 //
 TF0=0; // флаг переполнения таймера.
 TF1=0;
 //
 TR0=1; // бит управления таймером 1-пуск 0-стоп
 TR1=1;
 //
 IE0=0; // флаг  внешнего прерывания
 IE1=0;
 //
 IT0=0; // бит управления типом прерывания срез/низкий ур.
 IT1=1;
 //
 /* Настройка UART */
 RCAP2L=_C_SPEED_UARTL;
 RCAP2H=_C_SPEED_UARTH;
 T2CON=0x34;
 T2MOD=0x00;
 SCON=0x50;
 TI=0;
 RI=0;

 IP=0x10;                       /* max priopitet UART-interrupt */
 IE=0x9F;
}
//
//
/*________________________ Передача блока в BOM ____________________________*/
bit Write_bom()
{unsigned char sum=0,cnt;
 if (fl_busy_bom==0)
  {fl_busy_bom=1;
   ptr_tx_bom=0;
   cnt_tx_bom=_C_SIZE_TX_BOM-1;
   buf_tx_bom[0]=_C_START_CU_BOM;
   for(cnt=0;cnt<(_C_SIZE_TX_BOM-1);cnt++) {sum=sum^buf_tx_bom[cnt];}
   buf_tx_bom[_C_SIZE_TX_BOM-1]=sum;
   SBUF=buf_tx_bom[0];
   return(1);
  }
 else
  {return(0);}
}

/********************************/
/* Программная задержка на 1 ms */
/********************************/
void Delay_1ms(unsigned char Par)
 { idata unsigned char i,j;
      #define c_count 11
    for (j=Par; j!=0; j--)
     for (i=0; i < c_count; i++)
     {_nop_(); _nop_();}
 }
/********************************/
/* Чтение из EEPROM *************/
/********************************/
void Read_eep(unsigned char Par)
 {
  WMCON=WMCON | EEMEN_;
  switch (Par)
   {case 0:{_reg_value._char[0]=_RG_0_VN_H;
            _reg_value._char[1]=_RG_0_VN_L;
            break;}
    case 1:{_reg_value._char[0]=_RG_0_GN_H;
            _reg_value._char[1]=_RG_0_GN_L;
            break;}
    case 2:{_reg_value._char[0]=_RG_UVKV_VN_H;
            _reg_value._char[1]=_RG_UVKV_VN_L;
            break;}
    case 3:{_reg_value._char[0]=_RG_UVKV_GN_H;
            _reg_value._char[1]=_RG_UVKV_GN_L;
            break;}
    case 4:{_reg_value._char[0]=_RG_S_EL_H;
            _reg_value._char[1]=_RG_S_EL_L;
            break;}
    case 5:{_reg_value._char[0]=_RG_A_AZ_H;
            _reg_value._char[1]=_RG_A_AZ_L;
            break;}
    case 6:{_reg_value._char[0]=_RG_UV_VN_H;
            _reg_value._char[1]=_RG_UV_VN_L;
            break;}
    case 7:{_reg_value._char[0]=_RG_UV_GN_H;
            _reg_value._char[1]=_RG_UV_GN_L;
            break;}
     }// switch
  buf_tx_bom[1]=_reg_value._char[1];
  buf_tx_bom[2]=_reg_value._char[0];
  WMCON=WMCON&(~EEMEN_);
 }
/********************************/
/* Запись в ЕЕPROM  *************/
/********************************/
void Write_eep(unsigned char Par)
 {
  WMCON=WMCON | EEMEN_ | EEMWE_;
   switch (Par)
   {case 0:{_RG_0_VN_H=_reg_value._char[0];}
        case 1:{_RG_0_GN_H=_reg_value._char[0];}
        case 2:{_RG_UVKV_VN_H=_reg_value._char[0];}
        case 3:{_RG_UVKV_GN_H=_reg_value._char[0];}
        case 4:{_RG_S_EL_H=_reg_value._char[0];}
        case 5:{_RG_A_AZ_H=_reg_value._char[0];}
        case 6:{_RG_UV_VN_H=_reg_value._char[0];}
        case 7:{_RG_UV_GN_H=_reg_value._char[0];}
    }// switch
    while ((WMCON&EERDY_)==0);
     switch (Par)
   {case 0:{_RG_0_VN_L=_reg_value._char[1];}
    case 1:{_RG_0_GN_L=_reg_value._char[1];}
    case 2:{_RG_UVKV_VN_L=_reg_value._char[1];}
    case 3:{_RG_UVKV_GN_L=_reg_value._char[1];}
    case 4:{_RG_S_EL_L=_reg_value._char[1];}
    case 5:{_RG_A_AZ_L=_reg_value._char[1];}
    case 6:{_RG_UV_VN_L=_reg_value._char[1];}
    case 7:{_RG_UV_GN_L=_reg_value._char[1];}
   }// switch
  while ((WMCON&EERDY_)==0);
  WMCON&=~(EEMEN_ | EEMWE_);
 }
/********************************/
/* Запись в ЦАП  *************/
/********************************/
void Write_dac(unsigned char Par)
 {
  _RG_DAC_L=Par | (_reg_value._char[1]<<4);
  _RG_DAC_H=(_reg_value._char[0]<<4)|(_reg_value._char[1]>>4);
 }
/******************************************/
/******************************************/
/********* СИСТЕМА ПРЕРЫВАНИЙ *************/
/******************************************/
/*____ ПРЕРЫВАНИЕ TMR0______*/
/* Быстрый таймер - 20ms                                                         */
void T0_int(void) interrupt 1
{ TH0=(_C_20MS>>8);
  TL0=(char)(_C_20MS);
  fl_start=1; // флаг начала цикла в 1
}
/*____ ПРЕРЫВАНИЕ TMR1______*/
/* Медленный таймер: 1ms */
void T1_int(void) interrupt 3
{ TH1=(_C_1000MKS >>8);
  TL1=(char)_C_1000MKS;
 }
/*____ ПРЕРЫВАНИЕ ПО ВНЕШНИМ СОБЫТИЯМ ____*/
void EX0_int(void) interrupt 0 using 1
{unsigned char self,help;
  self=_RG_INTF;


}
//
/*__________________ ПРЕРЫВАНИЕ UART __________________________________________*/
void UART_int(void) interrupt 4 using 2
{data unsigned char _SBUF;
 if (RI)
  {/*____________ Прием ______________*/
  RI=0;
   _SBUF=SBUF;
   switch (sost_rx_bom)  // анализ состяния обмена с БОМом
   {case s_endbom:
    {break;}
    case s_faultbom:
    {break;}
    case s_waitbom:
    {if (_SBUF==_C_START_BOM) // проверка старта приёма
      {ptr_rx_bom=0;
       cnt_rx_bom=_C_SIZE_RX_BOM;
       sum_rx_bom=_SBUF;
       sost_rx_bom=s_receiptbom;
      }
     break;
    }
    case s_receiptbom:
    {sum_rx_bom=sum_rx_bom ^ _SBUF;
     buf_rx_bom[ptr_rx_bom]=_SBUF;
     --cnt_rx_bom;
     ++ptr_rx_bom;
     if (cnt_rx_bom==0)
      {if (sum_rx_bom==0)
        {sost_rx_bom=s_endbom;}
       else
        {sost_rx_bom=s_faultbom;}
      }
     break;
    }
   }
  }
 else
  {/*_______________ Передача___________ */
    TI=0;
    if (cnt_tx_bom==0)
     {fl_busy_bom=0;
     }
    else
     {SBUF=buf_tx_bom[++ptr_tx_bom];
      cnt_tx_bom--;
     }
  }
}

/*_________________________ Анализ получения блока от BOM ______________________*/
/* если return(x): 0 - получены данные без ошибок */
/*                 1 - нет данных */
/*                 2 - начало приема блока */
/*                 3 - ошибка SUM */
/*                 4 - количество ошибок превысело допуск */
char Analiz_bom()
{ switch (sost_rx_bom)
  {case s_waitbom:       /* ожидание блока */
   {return(1);}

   case s_endbom:        /* блок принят правильно */
   {sost_rx_bom=s_waitbom;


    return(0);
   }

   case s_receiptbom:    /* начало приема блока */
   {return(2);}

   case s_faultbom:      /* принято с ошибкой */
   {if ((++cnt_err_rx_bom)>_C_CNT_FAULT)
     {_B_ERR_BOM=1;
      _B_GLOBAL_ERR_HARD=1;
      return(4);
     }
    else
     {sost_rx_bom=s_waitbom;
      return(3);
     }
    }

  } /* switch */
}
/************************************************/
/************* Управляющий монитор **************/
/************************************************/
void RTOS(void)
{
   switch (sost_sys)
 { case sys0:// ожидание начала цикла
   {  while (_TRUE_) {
     //if (fl_start)
     //   {
          _RG_TX_BV[0]=0x40;
          _RG_TX_BV[1]=_RG_PU_VN_L;
          _RG_TX_BV[2]=_RG_PU_VN_H;
          _RG_TX_BV[3]=_RG_PU_GN_L;
          _RG_TX_BV[4]=_RG_PU_GN_H;
          _RG_TX_BV[5]=_RG_RAM[3];
          _RG_TX_BV[6]=_RG_RAM[2];
          _RG_TX_BV[7]=_RG_RAM[1];
          _RG_TX_BV[8]=_RG_RAM[0];
          _RG_BV_CTRL=0x01;
           Delay_1ms(10);
           fl_start=0;};
         break;
   }
  // старт получен, начинаем основной цикл
  case sys1:
   { // запись поправок в ЦАПы
       fl_start=0; // флаг начала цикла в 0
       sost_sys=sys0;
    break;
   }
  // ожидание ответа от БВ
  case sys2:
   { if (fl_mode_adj)
        {sost_sys=sys5;};
     if (fl_mode_uvkv)
        {sost_sys=sys6;};

    break;
   }

  // блок от БВ получен, необходимо обработать
  case sys3:
   {

    break;
   }
  // критическая ошибка обмена с БВ
  case sys4:
   {

    break;
   }
  // режим регулировок
  case sys5:
   { switch (Analiz_bom())
    {case 1: break; // ещё нет данных
     case 3: break; // ошибка контрольной суммы
     case 4:
      {sost_sys=sys4; // критическая ошибка
       break;
      }
     case 0: // данные от БОМА получены -> анализ
      {  if (buf_rx_bom[0]==_C_START_BOM)
       {
         switch (buf_rx_bom[1]&0xF0)
          { case 32: // команда "чтение"
            {
            Read_eep(buf_rx_bom[1]&0x0F);
            };
           case 64: // команда "запись"
            {
            Write_eep(buf_rx_bom[1]&0x0F);
            };
           case 96: // команда "-"
            { --_reg_value._int;

            break;
            };
           case 128: // команда "+"
            { _reg_value._int++;

            break;
            };
          };
        };
       break;
      }
    }// switch Analiz_bom

    break;
   }
   // режим УВКВ
  case sys6:
   {

    break;
   }
 } // switch sost_sys

 }

/************************************************/
/************* Основная программа ***************/
/************************************************/

main()
{Begin_init();
while (_TRUE_) RTOS(); /* бесконечный цикл */
}

