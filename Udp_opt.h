/*********************************************************************
* �汾��¼
* 18.1.31 ���� StrToIPChar���� ֧��XPϵͳ
* 18.3.20 ���� WhetherValid_IP �����ж�IP�Ƿ�Ϸ�
* 18.3.23 ����CAN�������
* 18.4.23 UDP͸�������������PackPara_nonstruct
* 18.4.28 IP��ȡ����֧�ֻ�ȡMAC��ַ(SendArp)
* 18.5.8  IP�Ϸ����жϺ���BUG�޸�
* 18.5.15 �޸�IP��ȡ������127.0.0.1 BUG
*********************************************************************/
//---------------------------------------------------------------------------

#ifndef Udp_optH
#define Udp_optH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include "UDPStructs.h"
#include <vector>
#include <memory>

//---------------------------------------------------------------------------


//------------------------------------------------------------------------------

//�豸��ʶ
char const EquIdStr[] = {"PractCommConfigData"};

//�豸�����
enum TPkgIncrease{increase_1,increase_1024};


//------------------------------------------------------------------------------
//����ԭ��
String IntToIpstr(u32 addr);
u16 GetMyIP(std::vector<TMyIp> &VC_MyIp);
String IPCharToStr(const u8 IP[4],bool Endian = false);
void __fastcall StrToIPChar(const char* ipstr,u8 ip[4]);
const String MACCharToStr(const u8 MAC[6]);
int StrToMac6(const char* strtmp,u8 * dp1);
void StrToMac6(const String &macstr,char mac[6]);
bool WhetherValid_IP(const String &IP);
//-------------��׼UDPЭ��----------------
//Ҫ����
int __fastcall RdPara(int startpos, int paranum,std::vector<TBytes> &VC_TBytes,TPkgIncrease PkgIncrease);
//Ҫ����_�ⲿʶ����
int __fastcall RdPara(int startpos, int paranum,std::vector<TBytes> &VC_TBytes,const char* EquIdStr,TPkgIncrease PkgIncrease);
//��������
void __fastcall WrPara(int startpos, int paranum,const void* dp,std::vector<TBytes> &VC_TBytes);
//��������_�ⲿʶ����
int __fastcall WrPara(int startpos, int paranum,const void* dp,const char* EquIdStr,std::vector<TBytes> &VC_TBytes);
//�豸����ָ��
void __fastcall CtlPara(int startpos, int paranum,const void* dp,std::vector<TBytes> &VC_TBytes);
//�豸����ָ��_�ⲿʶ����
int __fastcall CtlPara(int startpos, int paranum,const char* dp,const char* EquIdStr,std::vector<TBytes> &VC_TBytes);
//�㲥��
void __fastcall SearchEqu(TBytes &AByte);
//Уʱָ��
void __fastcall CalibTime(TBytes &AByte);
//�Ǳ�׼UDP�ṹ�����
void __fastcall PackPara_nonstruct(const void *data,size_t size,TBytes &AByte);
//----------CAN����Э��--------------------
//���CAN����
void __fastcall PackCan(int address,int dp_len,int cmd,const void* dp,TBytes &AByte);
//------------------------------------------------------------------------------

#endif
