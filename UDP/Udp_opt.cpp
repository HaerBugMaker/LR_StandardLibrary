//---------------------------------------------------------------------------

#pragma hdrstop

#include "Udp_opt.h"
#include <vector>
#include <iphlpapi.h>
#pragma link "iphlpapi.lib"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//---------------------------------------------------------------------------
//将32位的IP转换为点分十进制的IP字符串
String IntToIpstr(u32 addr)
{
  String ctmp = "";

  for (int ii=0;ii<4;ii++) {
	ctmp += IntToStr(int(addr & 0xff));
	if (ii != 3) ctmp += ".";
	addr >>= 8;
  }
  return ctmp;
}
//---------------------------------------------------------------------------
//将4个字节的IP地址转换为点分十进制的IP字符串
/***************
* IP[4] 源IP数据
* Endian 字节序 false 小端 true 大端
***************/
String IPCharToStr(const u8 IP[4],bool Endian)
{
	String IPtmp;
	if(Endian == false) IPtmp.sprintf(L"%d.%d.%d.%d",IP[0],IP[1],IP[2],IP[3]);
	else                IPtmp.sprintf(L"%d.%d.%d.%d",IP[3],IP[2],IP[1],IP[0]);
	return IPtmp;
}
//---------------------------------------------------------------------------
//将4个字节的IP地址转换为点分十进制的IP字符串
/***************
* IP[4] 源IP数据
***************/
const String MACCharToStr(const u8 MAC[6])
{
	String MACtmp;
	for(int i=0;i<6;i++){

		MACtmp += IntToHex(MAC[i],2);
		if(i!=5)
		MACtmp += L"-";
	}
	return MACtmp;
}
//---------------------------------------------------------------------------
//将点分十进制的IP字符串(const char*)转换为ip[4]    此功能同inet_pton 但是可以支持XP    字节序按照 192.168.0.1 C0 A8 00 01
/*******************
* ipstr 点分十进制IP字符串
* ip[4]    返回的ip[4]
*******************/
void __fastcall StrToIPChar(const char* ipstr,u8 ip[4])
{
	unsigned long longaddr;
	longaddr = ::inet_addr(ipstr);
	if(longaddr == INADDR_NONE) throw String(L"IP地址不符合规范");
	memcpy(ip,&longaddr,4);
}
//---------------------------------------------------------------------------
//将以‘-’分隔的MAC地址字符串转换为6个字节的MAC地址
/***************
* strtmp 源MAC地址字符串
* dp1    目标数据区指针
***************/
int StrToMac6(const char* strtmp,u8 * dp1)
{
	u8 ci;
	for (int ii=0;ii<6;ii++) {
		ci = 0;
		if ((strtmp[ii*2]>='0')&&(strtmp[ii*2]<='9')) ci = strtmp[ii*2]-'0';
		else if ((strtmp[ii*2]>='A')&&(strtmp[ii*2]<='F')) ci = strtmp[ii*2]-'A' + 10;
			 else if ((strtmp[ii*2]>='a')&&(strtmp[ii*2]<='f')) ci = strtmp[ii*2]-'a' + 10;
				  else return(0);
		ci *= 16;
		if ((strtmp[ii*2+1]>='0')&&(strtmp[ii*2+1]<='9')) ci += strtmp[ii*2+1]-'0';
		else if ((strtmp[ii*2+1]>='A')&&(strtmp[ii*2+1]<='F')) ci += strtmp[ii*2+1]-'A' + 10;
			 else if ((strtmp[ii*2+1]>='a')&&(strtmp[ii*2+1]<='f')) ci += strtmp[ii*2+1]-'a' + 10;
				  else return(0);
		*dp1 ++ = ci;
	}
	return (1);

}
//---------------------------------------------------------------------------
//将以‘-’分隔的MAC地址字符串转换为6个字节的MAC地址
/***************
* macstr 源MAC地址字符串
* mac6   目标数据区指针
***************/
void StrToMac6(const String &macstr,char mac[6])
{
	std::unique_ptr<TStringList>sl(new TStringList);
	sl->Delimiter = L'-';
	sl->DelimitedText = macstr;
	if((sl->Count!=6)||(macstr == L"")) throw String(L"不符合规范的MAC地址");
	String strtmp;
	for(int i=0;i<sl->Count;i++){
		strtmp = L"0x" + sl->Strings[i];
		mac[i] = StrToInt(strtmp);
	}
}
//---------------------------------------------------------------------------
//判断是不是合法的点分十进制
/*************
* IP IP字符串
* 返回值 true 合法  false 不合法
*************/
bool WhetherValid_IP(const String &IP)
{
	try{
		std::unique_ptr<TStringList>psl(new TStringList);
		psl->Delimiter = L'.';
		psl->DelimitedText = IP;
		if((psl->Count!=4)||(IP == L"0.0.0.0")) return false;
		int IPCell;
		for(int i=0;i<4;i++){
			IPCell = psl->Strings[i].ToInt();
			if((IPCell > 255)||(IPCell < 0)) return false;
		}
	}
	catch(...){
		return false;
	}
    return true;	
}
//---------------------------------------------------------------------------
//获取网卡信息
/***************
*　VC_MyIp IP池
***************/
u16 GetMyIP(std::vector<TMyIp> &VC_MyIp)
{
	TMyIp MyIp;
	VC_MyIp.clear();
	ULONG Maclen{6};
	//获取IP的一个结构体
	PMIB_IPADDRTABLE pIPTable = NULL;
	//大小
	DWORD dwSize = 0;
	//获取一次IP地址填写进去
	::GetIpAddrTable(pIPTable, &dwSize, TRUE);
	//给他分配空间
	pIPTable = (PMIB_IPADDRTABLE)::GlobalAlloc(GPTR, dwSize);
	//这句话就是获取IP地址然后填入pIPTable里面去
	::GetIpAddrTable(pIPTable, &dwSize, TRUE);
	//dwNumEntries大概是一个数量 IP数量？
	if (pIPTable->dwNumEntries > 1) {
		//这里还要进行网卡信息填入VECTOR的操作
		for (u8 i=0; i < pIPTable->dwNumEntries; i++) {
			if(IntToIpstr(pIPTable->table[i].dwAddr) == L"127.0.0.1") continue;
			MyIp.IpAddr = IntToIpstr(pIPTable->table[i].dwAddr);                   //IP
			MyIp.Mask = IntToIpstr(pIPTable->table[i].dwMask);                     //掩码
			MyIp.Bcast =  IntToIpstr(((pIPTable->table[i].dwAddr & pIPTable->table[i].dwMask) | ~pIPTable->table[i].dwMask));    //广播地址
			::SendARP(::inet_addr(AnsiString(MyIp.IpAddr).c_str()),0,(PULONG)MyIp.MAC,&Maclen);    //获取MAC地址是不是会卡
			VC_MyIp.push_back(MyIp);
			memset(&MyIp,0,size_t(sizeof(TMyIp)));
		}
	}//end of more then 1 ip
	::GlobalFree(pIPTable);
	return VC_MyIp.size();
}
//---------------------------------------------------------------------------
//要数据   问题在于多包的时候 parastaadd是+1还是+1024
/********
* startpos 起始位置
* paranum  需要的数据数量
* VC_TBytes   数据接口
* PkgIncrease  数据包间隔
*返回值 数据包个数
********/
int __fastcall RdPara(int startpos, int paranum,std::vector<TBytes> &VC_TBytes,TPkgIncrease PkgIncrease)
{
	TUDPSysPara * UDPSysPara;
	TBytes AByte;
	VC_TBytes.clear();

	u8 NetSBuf[1500] = {0x00};
	int ii,reallen,jj;
	int pkgnum,lastpkgnum;
	if(paranum == 0){
		pkgnum = 1;
		lastpkgnum = paranum;
	}
	else{
		pkgnum = paranum / 1024 + ((paranum % 1024) ? 1 : 0);
		lastpkgnum = paranum % 1024;
		if(lastpkgnum == 0) lastpkgnum = 1024;
	}
	//不考虑1024字节以上的包
	for (jj=0; jj<pkgnum; jj++)
	{
		UDPSysPara = (TUDPSysPara *)NetSBuf;
		strcpy(UDPSysPara->Cmd,"SetPara");

		strcpy(UDPSysPara->IdentifyingCode,EquIdStr);

		UDPSysPara->StartByte = 0xca;
		UDPSysPara->CmdType = 0x3;
		switch (PkgIncrease) {
			//数据+1
			case TPkgIncrease::increase_1:{
				UDPSysPara->ParaStaAdd = startpos + jj;
				break;
			}
			//数据+1024
			case TPkgIncrease::increase_1024:{
				UDPSysPara->ParaStaAdd = startpos + jj*1024;
				break;
			}
			default:break;
		}
		//最后一包就不是1024个了 是余数
		if (jj == pkgnum-1) UDPSysPara->ParaNum = lastpkgnum;
		else UDPSysPara->ParaNum = 1024;

		AByte.set_length(sizeof(TUDPSetParaConfirm) + 2);
		for (int i=AByte.Low; i<=AByte.High; i++){
			AByte[i] = NetSBuf[i];
		}
		VC_TBytes.push_back(AByte);
	}
	return VC_TBytes.size();
}
//---------------------------------------------------------------------------
//读数据_重载 equid是外部参数
/********
* startpos 起始位置
* paranum  需要的数据数量
* VC_TBytes   数据接口
* EquIdStr   设备识别字
* PkgIncrease  数据包间隔
*返回值 数据包个数
********/
int __fastcall RdPara(int startpos, int paranum,std::vector<TBytes> &VC_TBytes,const char* EquIdStr,TPkgIncrease PkgIncrease)
{
	TUDPSysPara * UDPSysPara;
	TBytes AByte;
	VC_TBytes.clear();

	u8 NetSBuf[1500] = {0x00};
	int ii,reallen,jj;
	int pkgnum,lastpkgnum;
	if(paranum == 0){
		pkgnum = 1;
		lastpkgnum = paranum;
	}
	else{
		pkgnum = paranum / 1024 + ((paranum % 1024) ? 1 : 0);
		lastpkgnum = paranum % 1024?paranum % 1024:1024;
	}
	//不考虑1024字节以上的包
	for (jj=0; jj<pkgnum; jj++)
	{
		UDPSysPara = (TUDPSysPara *)NetSBuf;
		strcpy(UDPSysPara->Cmd,"SetPara");

		strcpy(UDPSysPara->IdentifyingCode,EquIdStr);

		UDPSysPara->StartByte = 0xca;
		UDPSysPara->CmdType = 0x3;
		switch (PkgIncrease) {
			//数据+1
			case TPkgIncrease::increase_1:{
				UDPSysPara->ParaStaAdd = startpos + jj;
				break;
			}
			//数据+1024
			case TPkgIncrease::increase_1024:{
				UDPSysPara->ParaStaAdd = startpos + jj*1024;
				break;
			}
			default:break;
		}
		//最后一包就不是1024个了 是余数
		if (jj == pkgnum-1) UDPSysPara->ParaNum = lastpkgnum;
		else UDPSysPara->ParaNum = 1024;

		AByte.set_length(sizeof(TUDPSetParaConfirm) + 2);
		for (int i=AByte.Low; i<=AByte.High; i++){
			AByte[i] = NetSBuf[i];
		}
		VC_TBytes.push_back(AByte);
	}
	return VC_TBytes.size();
}
//---------------------------------------------------------------------------
//写数据
/********
* startpos  起始位置
* paranum   需要的数据数量
* dp        源数据指针
* VC_TBytes 数据接口
********/
void __fastcall WrPara(int startpos, int paranum,const void* dp,std::vector<TBytes> &VC_TBytes)
{
	TUDPSysPara * UDPSysPara;
	TBytes AByte;
	VC_TBytes.clear();


	u8 NetSBuf[1500] = {0x00};
	int ii,reallen,jj;
	int pkgnum,lastpkgnum;
	if(paranum == 0){
		pkgnum = 1;
		lastpkgnum = paranum;
	}
	else{
		pkgnum = paranum / 1024 + ((paranum % 1024) ? 1 : 0);
		lastpkgnum = paranum % 1024?paranum % 1024:1024;
	}
	//不考虑1024字节以上的包
	for (jj=0; jj<pkgnum; jj++)
	{
		UDPSysPara = (TUDPSysPara *)NetSBuf;
		strcpy(UDPSysPara->Cmd,"SetPara");

		strcpy(UDPSysPara->IdentifyingCode,EquIdStr);
		UDPSysPara->StartByte = 0xca;
		UDPSysPara->CmdType = 0x10;
		UDPSysPara->ParaStaAdd = startpos + jj*1024;
		//最后一包就不是1024个了 是余数
		if (jj == pkgnum-1) UDPSysPara->ParaNum = lastpkgnum;
		else UDPSysPara->ParaNum = 1024;
		memcpy(UDPSysPara->Para,reinterpret_cast<const char*>(dp)+jj*1024,UDPSysPara->ParaNum);

		AByte.set_length(sizeof(TUDPSetParaConfirm) + UDPSysPara->ParaNum);
		for (int i=AByte.Low; i<=AByte.High; i++){
			AByte[i] = NetSBuf[i];
		}
		VC_TBytes.push_back(AByte);
	}
}
//---------------------------------------------------------------------------
//写数据其中equid是外部参数
/********
* startpos 起始位置
* paranum  需要的数据数量
* dp       源数据指针
* EquIdStr 设备外部参数
* VC_TBytes 数据接口
********/
int __fastcall WrPara(int startpos, int paranum,const void* dp,const char* EquIdStr,std::vector<TBytes> &VC_TBytes)
{
	TUDPSysPara * UDPSysPara;
	TBytes AByte;
	VC_TBytes.clear();


	u8 NetSBuf[1500] = {0x00};
	int ii,reallen,jj;
	int pkgnum,lastpkgnum;
	if(paranum == 0){
		pkgnum = 1;
		lastpkgnum = paranum;
	}
	else{
		pkgnum = paranum / 1024 + ((paranum % 1024) ? 1 : 0);
		lastpkgnum = paranum % 1024?paranum % 1024:1024;
	}
	//不考虑1024字节以上的包
	for (jj=0; jj<pkgnum; jj++)
	{
		UDPSysPara = (TUDPSysPara *)NetSBuf;
		strcpy(UDPSysPara->Cmd,"SetPara");

		strcpy(UDPSysPara->IdentifyingCode,EquIdStr);
		UDPSysPara->StartByte = 0xca;
		UDPSysPara->CmdType = 0x10;
		UDPSysPara->ParaStaAdd = startpos + jj*1024;
		//最后一包就不是1024个了 是余数
		if (jj == pkgnum-1) UDPSysPara->ParaNum = lastpkgnum;
		else UDPSysPara->ParaNum = 1024;
		memcpy(UDPSysPara->Para,reinterpret_cast<const char*>(dp)+jj*1024,UDPSysPara->ParaNum);

		AByte.set_length(sizeof(TUDPSetParaConfirm) + UDPSysPara->ParaNum);
		for (int i=AByte.Low; i<=AByte.High; i++){
			AByte[i] = NetSBuf[i];
		}
		VC_TBytes.push_back(AByte);
	}
	return VC_TBytes.size();
}
//---------------------------------------------------------------------------
//发送广播包（无需识别字）
/************
* &AByte    数据接口
************/
void __fastcall SearchEqu(TBytes &AByte)
{
	u8 *dp;
	int i, SendLen;
	//一定要初始化 不然里面非常乱
	u8 sbuf[1000] = {0};
	TUDPAddressPara * pUDPAddressPara = (TUDPAddressPara *)&sbuf;
	WinExec("cmd /c arp -d",SW_HIDE);
	strcpy(pUDPAddressPara->Cmd,"GetNetPara");

	dp = (u8*)sbuf;
	pUDPAddressPara->ChkSum = 0;
	//计算校验因此不要包括最后两位校验位
	for (int i = 0; i<sizeof(TUDPAddressPara)-2; i++) {
		pUDPAddressPara->ChkSum += *dp++;
	}

	AByte.set_length(sizeof(TUDPAddressPara));
	u8 *p = sbuf;
	for (int i=AByte.Low; i<=AByte.High; i++)
		AByte[i] = *p++;
}
//---------------------------------------------------------------------------
//下发控制指令
/********
* startpos 起始位置
* paranum  需要的数据数量
* dp       源数据指针
* VC_TBytes   数据接口
********/
void __fastcall CtlPara(int startpos, int paranum,const void* dp,std::vector<TBytes> &VC_TBytes)
{
	TUDPSysPara * UDPSysPara;
	TBytes AByte;
	VC_TBytes.clear();


	u8 NetSBuf[1500] = {0x00};
	int ii,reallen,jj;
	int pkgnum,lastpkgnum;
	if(paranum == 0){
		pkgnum = 1;
		lastpkgnum = paranum;
	}
	else{
		pkgnum = paranum / 1024 + ((paranum % 1024) ? 1 : 0);
		lastpkgnum = paranum % 1024?paranum % 1024:1024;
	}
	VC_TBytes.resize(pkgnum);
	//不考虑1024字节以上的包
	for (jj=0; jj<pkgnum; jj++)
	{
		UDPSysPara = (TUDPSysPara *)NetSBuf;
		strcpy(UDPSysPara->Cmd,"SetPara");

		strcpy(UDPSysPara->IdentifyingCode,EquIdStr);
		UDPSysPara->StartByte = 0xca;
		UDPSysPara->CmdType = 0x06;
		UDPSysPara->ParaStaAdd = startpos + jj*1024;
		//最后一包就不是1024个了 是余数
		if (jj == pkgnum-1) UDPSysPara->ParaNum = lastpkgnum;
		else UDPSysPara->ParaNum = 1024;
		memcpy(UDPSysPara->Para,reinterpret_cast<const char*>(dp)+jj*1024,UDPSysPara->ParaNum);

		AByte.set_length(sizeof(TUDPSetParaConfirm) + UDPSysPara->ParaNum);
		for (int i=AByte.Low; i<=AByte.High; i++){
			AByte[i] = NetSBuf[i];
		}
		VC_TBytes[jj] = AByte;
	}
}
//---------------------------------------------------------------------------
//下发控制指令 设备标识作为外部参数
/********
* startpos 起始位置
* paranum  需要的数据数量
* dp       源数据指针
* VC_TBytes   数据接口
********/
int __fastcall CtlPara(int startpos, int paranum,const char* dp,const char* EquIdStr,std::vector<TBytes> &VC_TBytes)
{
	TUDPSysPara * UDPSysPara;
	TBytes AByte;
	VC_TBytes.clear();


	u8 NetSBuf[1500] = {0x00};
	int ii,reallen,jj;
	int pkgnum,lastpkgnum;
	if(paranum == 0){
		pkgnum = 1;
		lastpkgnum = paranum;
	}
	else{
		pkgnum = paranum / 1024 + ((paranum % 1024) ? 1 : 0);
		lastpkgnum = paranum % 1024?paranum % 1024:1024;
	}
	VC_TBytes.resize(pkgnum);
	//不考虑1024字节以上的包
	for (jj=0; jj<pkgnum; jj++)
	{
		UDPSysPara = (TUDPSysPara *)NetSBuf;
		strcpy(UDPSysPara->Cmd,"SetPara");

		strcpy(UDPSysPara->IdentifyingCode,EquIdStr);
		UDPSysPara->StartByte = 0xca;
		UDPSysPara->CmdType = 0x06;
		UDPSysPara->ParaStaAdd = startpos + jj*1024;
		//最后一包就不是1024个了 是余数
		if (jj == pkgnum-1) UDPSysPara->ParaNum = lastpkgnum;
		else UDPSysPara->ParaNum = 1024;
		memcpy(UDPSysPara->Para,reinterpret_cast<const char*>(dp)+jj*1024,UDPSysPara->ParaNum);

		AByte.set_length(sizeof(TUDPSetParaConfirm) + UDPSysPara->ParaNum);
		for (int i=AByte.Low; i<=AByte.High; i++){
			AByte[i] = NetSBuf[i];
		}
		VC_TBytes[jj] = AByte;
	}
	return VC_TBytes.size();
}
//---------------------------------------------------------------------------
//校时 port 15288 广播方式校时(无需识别字)
/************
* AByte 数据接口
************/
void __fastcall CalibTime(TBytes &AByte)
{
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	AByte.set_length(sizeof(TUDPCalibTime));
	TUDPCalibTime* pUDPCalibTime = reinterpret_cast<TUDPCalibTime*>(&AByte[AByte.Low]);

	strcpy(pUDPCalibTime->Cmd,"CalibDateTime");
	pUDPCalibTime->DT.year = sys.wYear-2000;
	pUDPCalibTime->DT.month = sys.wMonth;
	pUDPCalibTime->DT.date = sys.wDay;
	pUDPCalibTime->DT.hour = sys.wHour;
	pUDPCalibTime->DT.min = sys.wMinute;
	pUDPCalibTime->DT.sec = sys.wSecond;
	pUDPCalibTime->DT.msCnt = sys.wMilliseconds;
	pUDPCalibTime->DT.weekday = sys.wDayOfWeek;

}
//---------------------------------------------------------------------------
//非公司标准UDP组包(UDP透传)
/****************
* data  源数据
* size  数据长度(字节)
* AByte 数据接口
****************/
void __fastcall PackPara_nonstruct(const void *data,size_t size,TBytes &AByte)
{
	AByte.set_length(0);
	if(data == nullptr) return;
	const char *pdat = reinterpret_cast<const char*>(data);
	AByte.set_length(size);
	for(int i=AByte.Low;i<=AByte.High;i++){
		AByte[i] = *pdat++;
	}
}
//---------------------------------------------------------------------------
//打包CAN报文（不分包）
/**************
* address   装置地址
* len       数据区报文长度
* cmd       指令
* dp        数据区报文数据
* AByte     数据接口
**************/
void __fastcall PackCan(int address,int dp_len,int cmd,const void* dp,TBytes &AByte)
{
	u8 NetSBuf[1500] = {0x00};
	TCanTRDat* pCanTRDat = reinterpret_cast<TCanTRDat*>(NetSBuf);
	int packlen = dp_len + offsetof(TCanTRDat, CanDat);                 //总报文长度

	pCanTRDat->Address = address;         //装置地址
	pCanTRDat->len = dp_len;              //数据区报文长度
	strcpy(pCanTRDat->Cmd,"CanTDat");
	pCanTRDat->Exid[0] = 0;
	pCanTRDat->Exid[1] = cmd;
	memcpy(pCanTRDat->CanDat,dp,dp_len);

	AByte.set_length(packlen);
	for (int i=AByte.Low; i<=AByte.High; i++){
		AByte[i] = NetSBuf[i];
	}
}
//---------------------------------------------------------------------------
/********************
* FLASH协议要数据(528为单位)
* Address   :   串口通讯时用的，是设备地址，网络时不关心
* StaAdd    :   要擦除或编程的 FLASH 的绝对地址  操作45DB161时，以528为单位，操作CPU内部FLASH时，以1024为单位
* DatLen    :   就是要操作的数据  操作45db16的时候不可以超过 528
* VC_TBytes :   数据接口
********************/
void __fastcall RdPara_FLASH(u16 Address,u32 StaAdd,u32 DatLen,std::vector<TBytes> &VC_TBytes)
{
	VC_TBytes.clear();
	u8 NetSBuf[200]{0x00};

	int ii,reallen,jj;
	int pkgnum,lastpkgnum;
	if(DatLen == 0){
		pkgnum = 1;
		lastpkgnum = DatLen;
	}
	else{
		pkgnum = DatLen / 528 + ((DatLen % 528) ? 1 : 0);
		lastpkgnum = DatLen % 528;
		if(lastpkgnum == 0) lastpkgnum = 528;
	}

	for (jj=0; jj<pkgnum; jj++)
	{
		TFlashAddCmd* pFlashAddCmd = reinterpret_cast<TFlashAddCmd*>(NetSBuf);
		strcpy(pFlashAddCmd->Cmd,"FlashReadPage");   //命令
		pFlashAddCmd->StaAdd = StaAdd + jj*528;      //起始地址


		//最后一包就不是1024个了 是余数
		if (jj == pkgnum-1) pFlashAddCmd->DatLen = lastpkgnum;
		else pFlashAddCmd->DatLen = 528;

		//计算校验和
		u16 nchksum{0};
		for(int i=0;i<sizeof(TFlashAddCmd) - 2;i++){
			nchksum += NetSBuf[i];
		}
		pFlashAddCmd->chksum = nchksum;

		//拷贝数据
		TBytes nabyte;
		nabyte.set_length(sizeof(TFlashAddCmd));
		for(int i=0;i<sizeof(TFlashAddCmd);i++){
			nabyte[i] = NetSBuf[i];
		}
		VC_TBytes.push_back(nabyte);
	}
}
//---------------------------------------------------------------------------
/********************
* FLASH协议写数据(528为单位)
* Address   :   串口通讯时用的，是设备地址，网络时不关心
* StaAdd    :   要擦除或编程的 FLASH 的绝对地址  操作45DB161时，以528为单位，操作CPU内部FLASH时，以1024为单位
* DatLen    :   就是要操作的数据  操作45db16的时候不可以超过 528
* srcdata   :   需要写入的数据
* VC_TBytes :   数据接口
********************/
void __fastcall WrPara_FLASH(u16 Address,u32 StaAdd,u32 DatLen,const void* srcdata,std::vector<TBytes> &VC_TBytes)
{
	VC_TBytes.clear();
	u8 NetSBuf[1500]{0x00};

	int ii,reallen,jj;
	int pkgnum,lastpkgnum;
	if(DatLen == 0){
		pkgnum = 1;
		lastpkgnum = DatLen;
	}
	else{
		pkgnum = DatLen / 528 + ((DatLen % 528) ? 1 : 0);
		lastpkgnum = DatLen % 528;
		if(lastpkgnum == 0) lastpkgnum = 528;
	}

	for (jj=0; jj<pkgnum; jj++)
	{
		TFlashProg* pFlashAddProg = reinterpret_cast<TFlashProg*>(NetSBuf);
		strcpy(pFlashAddProg->Cmd,"FlashWritePage");  //命令
		pFlashAddProg->StaAdd = StaAdd + jj*528;      //起始地址


		//最后一包就不是1024个了 是余数
		if (jj == pkgnum-1) pFlashAddProg->DatLen = lastpkgnum;
		else pFlashAddProg->DatLen = 528;
		memcpy(pFlashAddProg->databuf,srcdata,pFlashAddProg->DatLen);


		//计算校验和
		u16 nchksum{0};
		for(int i=0;i<sizeof(TFlashAddCmd) + pFlashAddProg->DatLen - 2;i++){
			nchksum += NetSBuf[i];
		}
		pFlashAddProg->chksum = nchksum;

		//拷贝数据
		TBytes nabyte;
		nabyte.set_length(sizeof(TFlashAddCmd) + pFlashAddProg->DatLen);
		for(int i=0;i<nabyte.Length;i++){
			nabyte[i] = NetSBuf[i];
		}
		VC_TBytes.push_back(nabyte);
	}
}
//---------------------------------------------------------------------------
/*****************
* IAP命令组包
* naddress   :   地址
* srccmd     :   源数据
* nabyte     :   数据接口
*****************/
void __fastcall Pack_TIAPSigCmd(int naddress,const char* srccmd,TBytes &nabyte)
{
    nabyte.set_length(0);
	if(srccmd == nullptr) return;
	char SBuf[100]{0x00};
	TIAPSigCmd* pIAPSigCmd = reinterpret_cast<TIAPSigCmd*>(SBuf);
	pIAPSigCmd->Address = naddress;       //地址
	memcpy(pIAPSigCmd->Cmd,srccmd,16);    //指令
	for(int i=0;i<sizeof(TIAPSigCmd)-2;i++){
		pIAPSigCmd->chksum += SBuf[i];
	}
	nabyte.set_length(sizeof(TIAPSigCmd));
	for(int i=0;i<sizeof(TIAPSigCmd);i++){
		nabyte[i] = SBuf[i];
	}
}
//---------------------------------------------------------------------------
/******************************
* 标准时间结构转换为u32格式
* SDT      :     标准时间结构
* 返回值   :     u32格式的时间
******************************/
u32 RtcRealToBin(TSysTime * SDT)
{
if(SDT == nullptr) return 0x00000000;
u32 RtcCnt;
 u32 days;
 u8 i,j;
//get days
	days = (SDT->year >> 2) * 1461;
	i = SDT->year % 4;
	for (j=0;j<i;j++) days += YearDayTab[j];

	if (i)
		for (j=0;j<SDT->month - 1;j++) days += MonthDayTab[j];
	else
		for (j=0;j<SDT->month - 1;j++) days += LeapMonthDayTab[j];
	days += SDT->date - 1;
//get
    RtcCnt = SDT->hour*3600 + SDT->min*60 + SDT->sec;
	RtcCnt += (days * 86400);   //24*60*60
	return(RtcCnt);
}

//---------------------------------------------------------------------------
/******************************
* u32格式的时间转换为标准时间格式
* RtcCnt   :     u32格式的时间
* 返回值   :     转换结果
******************************/
TSysTime RtcBinToReal(u32 RtcCnt)
{
 u32 days,time;
 u8 i,j;
 TSysTime nsdt;
//get time
	time = RtcCnt % 86400;
	nsdt.hour = time / 3600;
	time %= 3600;
	nsdt.min = time / 60;
	nsdt.sec = time % 60;
//get date
	days = RtcCnt / 86400;
	time = days % 1461;
	for (i=0;i<4;i++) {
		if (time<YearDayTab[i]) break;
		else time -= YearDayTab[i];
	}
	nsdt.year = (days / 1461) * 4 + i;

	for (j=0;j<12;j++) {
		if (i) {
			if (time < MonthDayTab[j]) break;
			else time -= MonthDayTab[j];
		}
		else {
			if (time < LeapMonthDayTab[j]) break;
			else time -= LeapMonthDayTab[j];
		}
	}
	nsdt.month = j+1;
	nsdt.date = time+1;
	nsdt.weekday = (days + 6) % 7;
	return nsdt;
}
//---------------------------------------------------------------------------






