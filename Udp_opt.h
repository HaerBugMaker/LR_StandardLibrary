/*********************************************************************
* 版本记录
* 18.1.31 新增 StrToIPChar函数 支持XP系统
* 18.3.20 新增 WhetherValid_IP 用于判断IP是否合法
* 18.3.23 新增CAN打包报文
* 18.4.23 UDP透传打包函数更新PackPara_nonstruct
* 18.4.28 IP获取函数支持获取MAC地址(SendArp)
* 18.5.8  IP合法性判断函数BUG修复
* 18.5.15 修改IP获取方法的127.0.0.1 BUG
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

//设备标识
char const EquIdStr[] = {"PractCommConfigData"};

//设备包间隔
enum TPkgIncrease{increase_1,increase_1024};


//------------------------------------------------------------------------------
//函数原型
String IntToIpstr(u32 addr);
u16 GetMyIP(std::vector<TMyIp> &VC_MyIp);
String IPCharToStr(const u8 IP[4],bool Endian = false);
void __fastcall StrToIPChar(const char* ipstr,u8 ip[4]);
const String MACCharToStr(const u8 MAC[6]);
int StrToMac6(const char* strtmp,u8 * dp1);
void StrToMac6(const String &macstr,char mac[6]);
bool WhetherValid_IP(const String &IP);
//-------------标准UDP协议----------------
//要数据
int __fastcall RdPara(int startpos, int paranum,std::vector<TBytes> &VC_TBytes,TPkgIncrease PkgIncrease);
//要数据_外部识别字
int __fastcall RdPara(int startpos, int paranum,std::vector<TBytes> &VC_TBytes,const char* EquIdStr,TPkgIncrease PkgIncrease);
//下载数据
void __fastcall WrPara(int startpos, int paranum,const void* dp,std::vector<TBytes> &VC_TBytes);
//下载数据_外部识别字
int __fastcall WrPara(int startpos, int paranum,const void* dp,const char* EquIdStr,std::vector<TBytes> &VC_TBytes);
//设备控制指令
void __fastcall CtlPara(int startpos, int paranum,const void* dp,std::vector<TBytes> &VC_TBytes);
//设备控制指令_外部识别字
int __fastcall CtlPara(int startpos, int paranum,const char* dp,const char* EquIdStr,std::vector<TBytes> &VC_TBytes);
//广播包
void __fastcall SearchEqu(TBytes &AByte);
//校时指令
void __fastcall CalibTime(TBytes &AByte);
//非标准UDP结构体组包
void __fastcall PackPara_nonstruct(const void *data,size_t size,TBytes &AByte);
//----------CAN报文协议--------------------
//打包CAN报文
void __fastcall PackCan(int address,int dp_len,int cmd,const void* dp,TBytes &AByte);
//------------------------------------------------------------------------------

#endif
