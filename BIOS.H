/******************************************************************************/
/****************** ����������,���������,�����������,����������, function i/o */
/******************************************************************************/
#ifndef         __BIOS_H
#define __BIOS_H

#include <AT89S8253.h>
//
#define true    1
#define false   0
// ����  ����������� ��������� Xilinx
// _RG_CTRL
//
#define _B_MODE_ADJ    1    // 1 - ����� ����������� , 0 - ��� ������
#define _B_H0_S1_WDI   2    // 1 - ����� �� WDI soft , 0 - xilinx
#define _B_MODE_UVKV   4    // 1 - ����� ���� ���.   , 0 - ����.
#define _B_MODE_TAS    8    //
#define _B_CLK_WDI     16   // ��� _B_H0_S1_WDI=1,�������� ������� �� WDT
#define _B_DC_BV       32   // ������ DC �� �� 1- DC 0- ��� DC
#define _B_MODE_DUBL_TKN    64   // 1 - ����� ����� ��� ���.   , 0 - ����.
#define _B_LED         128  // ���������
//
// �������  ����� ����������(����������) (R/W)
// _RG_MASK_INT
//#define _B_M_BV_TX     1               // �������� BV
#define _B_M_BV_RX     2               // ����� BV
#define _B_M_TAS_TX    4               // �������� TAS
#define _B_M_TAS_RX    8               // �����  TAS
#define _B_M_ADJ       16              // ������������ ������ ADJ
#define _B_M_UVKV      32              // ������������ ������ UVKV
#define _B_M_TAS       64              // ������������ ������ TAS
#define _B_M_DUBL_TKN  128             //             //
//
//
// ������� ������ ���������� (R)
//_RG_FLAG_INT
//
//#define _B_F_BV_TX     1               // �������� BV
#define _B_F_BV_RX     2               // ����� BV
#define _B_F_TAS_TX    4               // �������� TAS
#define _B_F_TAS_RX    8               // �����  TAS
#define _B_F_ADJ       16              // ������������ ������ ADJ
#define _B_F_UVKV      32              // ������������ ������ UVKV
#define _B_F_TAS       64              //
#define _B_F_DUBL_TKN  128             //

#define _B_F_MODE      32             //
// ������� ���������� �������� � TAS (R/W)
// ������� �������: ���������� ������������ ����
// _RG_CTRL_TAS_TX
#define _B_START_TAS_TX 1              // ����� ��������
#define _B_RSTI_TAS_TX  2              // ����� ����������
// ������� ���������� ��������� �� TAS�  (R/W)
// ������� �������: ���������� ����������� ���� ��� START � CRC
// _RG_CTRL_TAS_RX
#define _B_RSTI_TAS_RX  1              // ����� ����������
//
// ������� ���������� ������� � CU  (R/W)
// _RG_CTRL_BV
#define _B_START_BV_TX  1              // ����� ��������
#define _B_RSTI_BV_TX   2              // ����� ���������� �� ��������
#define _B_RSTI_BV_RX   4              // C���� ���������� �� �����
#define _B_BUSY_BV_TX   128            // 1 - ����� �������� ������,0 - �������� ���������
//
//
typedef unsigned char uchar;
typedef unsigned int  uint;
// ��������� �������
typedef
enum  {sys0,sys1,sys2,sys3,sys4,sys5,sys6} enum_sost_syst;
// ��������� ������ � �����
typedef
enum  {s_waitbom,s_receiptbom,s_endbom,s_faultbom} enum_sost_rx_bom;

// sys0 - �� ��������� ������ �����
// sys1 - ����� ������� ->  ������� � �� �����
// sys2 - �������� ������ ��
// sys3 - ���� �� �� �������
// sys4 - ����������� ������ ������ � ��
// sys5 - ����� �����������
// sys6 - ����� ����
//
//
//
//
#define _C_SPEED_UARTL  0xFA // 115200
#define _C_SPEED_UARTH  0xFF

/*____����� ��������� ___________________*/
 #define _C_OKEY         0
//
//
/*___������� ������������ ______________*/
//
#define _C_CNT_FAULT    10      /* ���������� �������� �� �������� ������� �� ������ */
/*____������� �������� __________________*/
#define _C_1000MKS              -1844
#define _C_20MS                 -36864
//
#define _C_BV_RX_S              0x00    // ������� ������ ������� �� ��
#define _C_BV_RX_F              0xFF   // ������� ������ ������� �� ��
// ___________________ ����� � TAS
#define _C_SIZE_TX_TAS            12  // ���������� ���� � ������� ��->TAS -1
#define _C_SIZE_RX_TAS            4   // ���������� ���� � ������� TAS->��
#define _C_START_TX_TAS           0xAA // ��������� ���� � ������� ��->TAS
#define _C_START_RX_TAS           0x55 // ��������� ���� � ������� TAS->��
/* ___________________ ����� � BOM  ����������� ����������� ________________________*/
//
#define _C_SIZE_RX_BOM           2  // ���������� ���� � ������� ���->��
#define _C_SIZE_TX_BOM           4  // ���������� ���� � ������� ��->���
#define _C_START_BOM           0xA5 // ��������� ���� � ������� �� ����
#define _C_START_CU_BOM        0x5A // ��������� ���� � ������� �� ��
//
#define _C_SAVE_AN_L           0xFA // ����� ��� ������ �� ������� ������
#define _C_SAVE_AN_H           0xAF // ��� � �������� ����������
//
#define _C_COM_READ             16
#define _C_COM_WRITE            32
#define _C_COM_PLUS             128
#define _C_COM_MINUS            64
//
#define _C_LIMIT_MIN            -2048
#define _C_LIMIT_MAX            2047
//
// ------------------ ������� ��� ���� � ����� ------------
//
//���������� ��������� ������ �������� !!!!!!!!!!!!!!!!!!!!!
//
#define _C_UVKV_VN_PLUS         100 //
#define _C_UVKV_GN_PLUS         100 //
//
/*����� ��� ������� �������� ������*/
//
//������� �� ������� ���� �� ������ 14.12.04
#define _C_UVKV                 0x40
#define _C_ADJ                  0x20
//������� �� ������� ������ �� ������ 2.12.05
#define _C_TAS                  0x80
// ������� � ������ 5.06.09
#define _C_D_TKN                0x10
//
#define _B_ERR1       16         // ��� ���
#define _B_ERR2       1          // ��� ���� ���
#define _B_ERR3       8          // ���������� ��
#define _B_ERR4       4          // �������� +27�
#define _B_ERR5       2          // ����� ��������
#define _B_ERR6       32         // ��� ������������ �������
#define _B_OK         64         // MSS OK
//
// #define _B_F_NOP2      128             //
// ---------------------------------------------------------
void Rst_System(void);
void Begin_init(void);             /* ��������� ������������� ������� */
bit  Write_bom(void);              /* �������� ����� � BOM */
void Delay_1ms(uchar Par); /* �������� 1 ms */
int Read_eep(uchar n_r);               /* ������ EEPROM */
bit Write_eep(uchar n_r, int r_v);     /* ������ EEPROM */
 /* ������ ���� */
void Write_dac(uchar num, int value);
char Analiz_bom(void);             /* ������ ��������� ���������� �� BOM */
void Change_num(uchar Par, uchar *num_r,uchar *num_d);  /* ������� ������ ���� � ����������� */
int Tran_pr_dop(uchar vol1,vol2,maska,maska_sg);/* ������� � �������������� ��� */
// ����� ���������� �����. ����� � ������� �����
//_____ ��������� � bios_.h
#endif // __BIOS_H




