//---------------------------------------------------------------------------

#pragma hdrstop

#include "Udp_opt.h"
#include <vector>
#include <iphlpapi.h>
#pragma link "iphlpapi.lib"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//---------------------------------------------------------------------------
//��32λ��IPת��Ϊ���ʮ���Ƶ�IP�ַ���
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
//��4���ֽڵ�IP��ַת��Ϊ���ʮ���Ƶ�IP�ַ���
/***************
* IP[4] ԴIP����
* Endian �ֽ��� false С�� true ���
***************/
String IPCharToStr(const u8 IP[4],bool Endian)
{
	String IPtmp;
	if(Endian == false) IPtmp.sprintf(L"%d.%d.%d.%d",IP[0],IP[1],IP[2],IP[3]);
	else                IPtmp.sprintf(L"%d.%d.%d.%d",IP[3],IP[2],IP[1],IP[0]);
	return IPtmp;
}
//---------------------------------------------------------------------------
//��4���ֽڵ�IP��ַת��Ϊ���ʮ���Ƶ�IP�ַ���
/***************
* IP[4] ԴIP����
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
//�����ʮ���Ƶ�IP�ַ���(const char*)ת��Ϊip[4]    �˹���ͬinet_pton ���ǿ���֧��XP    �ֽ����� 192.168.0.1 C0 A8 00 01
/*******************
* ipstr ���ʮ����IP�ַ���
* ip[4]    ���ص�ip[4]
*******************/
void __fastcall StrToIPChar(const char* ipstr,u8 ip[4])
{
	unsigned long longaddr;
	longaddr = ::inet_addr(ipstr);
	if(longaddr == INADDR_NONE) throw String(L"IP��ַ�����Ϲ淶");
	memcpy(ip,&longaddr,4);
}
//---------------------------------------------------------------------------
//���ԡ�-���ָ���MAC��ַ�ַ���ת��Ϊ6���ֽڵ�MAC��ַ
/***************
* strtmp ԴMAC��ַ�ַ���
* dp1    Ŀ��������ָ��
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
//���ԡ�-���ָ���MAC��ַ�ַ���ת��Ϊ6���ֽڵ�MAC��ַ
/***************
* macstr ԴMAC��ַ�ַ���
* mac6   Ŀ��������ָ��
***************/
void StrToMac6(const String &macstr,char mac[6])
{
	std::unique_ptr<TStringList>sl(new TStringList);
	sl->Delimiter = L'-';
	sl->DelimitedText = macstr;
	if((sl->Count!=6)||(macstr == L"")) throw String(L"�����Ϲ淶��MAC��ַ");
	String strtmp;
	for(int i=0;i<sl->Count;i++){
		strtmp = L"0x" + sl->Strings[i];
		mac[i] = StrToInt(strtmp);
	}
}
//---------------------------------------------------------------------------
//�ж��ǲ��ǺϷ��ĵ��ʮ����
/*************
* IP IP�ַ���
* ����ֵ true �Ϸ�  false ���Ϸ�
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
//��ȡ������Ϣ
/***************
*��VC_MyIp IP��
***************/
u16 GetMyIP(std::vector<TMyIp> &VC_MyIp)
{
	TMyIp MyIp;
	VC_MyIp.clear();
	ULONG Maclen{6};
	//��ȡIP��һ���ṹ��
	PMIB_IPADDRTABLE pIPTable = NULL;
	//��С
	DWORD dwSize = 0;
	//��ȡһ��IP��ַ��д��ȥ
	::GetIpAddrTable(pIPTable, &dwSize, TRUE);
	//��������ռ�
	pIPTable = (PMIB_IPADDRTABLE)::GlobalAlloc(GPTR, dwSize);
	//��仰���ǻ�ȡIP��ַȻ������pIPTable����ȥ
	::GetIpAddrTable(pIPTable, &dwSize, TRUE);
	//dwNumEntries�����һ������ IP������
	if (pIPTable->dwNumEntries > 1) {
		//���ﻹҪ����������Ϣ����VECTOR�Ĳ���
		for (u8 i=0; i < pIPTable->dwNumEntries; i++) {
			if(IntToIpstr(pIPTable->table[i].dwAddr) == L"127.0.0.1") continue;
			MyIp.IpAddr = IntToIpstr(pIPTable->table[i].dwAddr);                   //IP
			MyIp.Mask = IntToIpstr(pIPTable->table[i].dwMask);                     //����
			MyIp.Bcast =  IntToIpstr(((pIPTable->table[i].dwAddr & pIPTable->table[i].dwMask) | ~pIPTable->table[i].dwMask));    //�㲥��ַ
			::SendARP(::inet_addr(AnsiString(MyIp.IpAddr).c_str()),0,(PULONG)MyIp.MAC,&Maclen);    //��ȡMAC��ַ�ǲ��ǻῨ
			VC_MyIp.push_back(MyIp);
			memset(&MyIp,0,size_t(sizeof(TMyIp)));
		}
	}//end of more then 1 ip
	::GlobalFree(pIPTable);
	return VC_MyIp.size();
}
//---------------------------------------------------------------------------
//Ҫ����   �������ڶ����ʱ�� parastaadd��+1����+1024
/********
* startpos ��ʼλ��
* paranum  ��Ҫ����������
* VC_TBytes   ���ݽӿ�
* PkgIncrease  ���ݰ����
*����ֵ ���ݰ�����
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
	//������1024�ֽ����ϵİ�
	for (jj=0; jj<pkgnum; jj++)
	{
		UDPSysPara = (TUDPSysPara *)NetSBuf;
		strcpy(UDPSysPara->Cmd,"SetPara");

		strcpy(UDPSysPara->IdentifyingCode,EquIdStr);

		UDPSysPara->StartByte = 0xca;
		UDPSysPara->CmdType = 0x3;
		switch (PkgIncrease) {
			//����+1
			case TPkgIncrease::increase_1:{
				UDPSysPara->ParaStaAdd = startpos + jj;
				break;
			}
			//����+1024
			case TPkgIncrease::increase_1024:{
				UDPSysPara->ParaStaAdd = startpos + jj*1024;
				break;
			}
			default:break;
		}
		//���һ���Ͳ���1024���� ������
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
//������_���� equid���ⲿ����
/********
* startpos ��ʼλ��
* paranum  ��Ҫ����������
* VC_TBytes   ���ݽӿ�
* EquIdStr   �豸ʶ����
* PkgIncrease  ���ݰ����
*����ֵ ���ݰ�����
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
		lastpkgnum = paranum % 1024;
		if(lastpkgnum == 0) lastpkgnum = 1024;
	}
	//������1024�ֽ����ϵİ�
	for (jj=0; jj<pkgnum; jj++)
	{
		UDPSysPara = (TUDPSysPara *)NetSBuf;
		strcpy(UDPSysPara->Cmd,"SetPara");

		strcpy(UDPSysPara->IdentifyingCode,EquIdStr);

		UDPSysPara->StartByte = 0xca;
		UDPSysPara->CmdType = 0x3;
		switch (PkgIncrease) {
			//����+1
			case TPkgIncrease::increase_1:{
				UDPSysPara->ParaStaAdd = startpos + jj;
				break;
			}
			//����+1024
			case TPkgIncrease::increase_1024:{
				UDPSysPara->ParaStaAdd = startpos + jj*1024;
				break;
			}
			default:break;
		}
		//���һ���Ͳ���1024���� ������
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
//д����
/********
* startpos  ��ʼλ��
* paranum   ��Ҫ����������
* dp        Դ����ָ��
* VC_TBytes ���ݽӿ�
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
		lastpkgnum = paranum % 1024;
	}
	//������1024�ֽ����ϵİ�
	for (jj=0; jj<pkgnum; jj++)
	{
		UDPSysPara = (TUDPSysPara *)NetSBuf;
		strcpy(UDPSysPara->Cmd,"SetPara");

		strcpy(UDPSysPara->IdentifyingCode,EquIdStr);
		UDPSysPara->StartByte = 0xca;
		UDPSysPara->CmdType = 0x10;
		UDPSysPara->ParaStaAdd = startpos + jj;//*1024;
		//���һ���Ͳ���1024���� ������
		if (jj == pkgnum-1) UDPSysPara->ParaNum = lastpkgnum;
		else UDPSysPara->ParaNum = 1024;
		memcpy(UDPSysPara->Para,dp,paranum);

		AByte.set_length(sizeof(TUDPSetParaConfirm) + UDPSysPara->ParaNum);
		for (int i=AByte.Low; i<=AByte.High; i++){
			AByte[i] = NetSBuf[i];
		}
		VC_TBytes.push_back(AByte);
	}
}
//---------------------------------------------------------------------------
//д��������equid���ⲿ����
/********
* startpos ��ʼλ��
* paranum  ��Ҫ����������
* dp       Դ����ָ��
* EquIdStr �豸�ⲿ����
* VC_TBytes ���ݽӿ�
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
		lastpkgnum = paranum % 1024;
	}
	//������1024�ֽ����ϵİ�
	for (jj=0; jj<pkgnum; jj++)
	{
		UDPSysPara = (TUDPSysPara *)NetSBuf;
		strcpy(UDPSysPara->Cmd,"SetPara");

		strcpy(UDPSysPara->IdentifyingCode,EquIdStr);
		UDPSysPara->StartByte = 0xca;
		UDPSysPara->CmdType = 0x10;
		UDPSysPara->ParaStaAdd = startpos + jj;//*1024;
		//���һ���Ͳ���1024���� ������
		if (jj == pkgnum-1) UDPSysPara->ParaNum = lastpkgnum;
		else UDPSysPara->ParaNum = 1024;
		memcpy(UDPSysPara->Para,dp,paranum);

		AByte.set_length(sizeof(TUDPSetParaConfirm) + UDPSysPara->ParaNum);
		for (int i=AByte.Low; i<=AByte.High; i++){
			AByte[i] = NetSBuf[i];
		}
		VC_TBytes.push_back(AByte);
	}
	return VC_TBytes.size();
}
//---------------------------------------------------------------------------
//���͹㲥��������ʶ���֣�
/************
* &AByte    ���ݽӿ�
************/
void __fastcall SearchEqu(TBytes &AByte)
{
	u8 *dp;
	int i, SendLen;
	//һ��Ҫ��ʼ�� ��Ȼ����ǳ���
	u8 sbuf[1000] = {0};
	TUDPAddressPara * pUDPAddressPara = (TUDPAddressPara *)&sbuf;
	WinExec("cmd /c arp -d",SW_HIDE);
	strcpy(pUDPAddressPara->Cmd,"GetNetPara");

	dp = (u8*)sbuf;
	pUDPAddressPara->ChkSum = 0;
	//����У����˲�Ҫ���������λУ��λ
	for (int i = 0; i<sizeof(TUDPAddressPara)-2; i++) {
		pUDPAddressPara->ChkSum += *dp++;
	}

	AByte.set_length(sizeof(TUDPAddressPara));
	u8 *p = sbuf;
	for (int i=AByte.Low; i<=AByte.High; i++)
		AByte[i] = *p++;
}
//---------------------------------------------------------------------------
//�·�����ָ��
/********
* startpos ��ʼλ��
* paranum  ��Ҫ����������
* dp       Դ����ָ��
* VC_TBytes   ���ݽӿ�
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
		lastpkgnum = paranum % 1024;
	}
	VC_TBytes.resize(pkgnum);
	//������1024�ֽ����ϵİ�
	for (jj=0; jj<pkgnum; jj++)
	{
		UDPSysPara = (TUDPSysPara *)NetSBuf;
		strcpy(UDPSysPara->Cmd,"SetPara");

		strcpy(UDPSysPara->IdentifyingCode,EquIdStr);
		UDPSysPara->StartByte = 0xca;
		UDPSysPara->CmdType = 0x06;
		UDPSysPara->ParaStaAdd = startpos + jj;//*1024;
		//���һ���Ͳ���1024���� ������
		if (jj == pkgnum-1) UDPSysPara->ParaNum = lastpkgnum;
		else UDPSysPara->ParaNum = 1024;
		memcpy(UDPSysPara->Para,dp,paranum);

		AByte.set_length(sizeof(TUDPSetParaConfirm) + UDPSysPara->ParaNum);
		for (int i=AByte.Low; i<=AByte.High; i++){
			AByte[i] = NetSBuf[i];
		}
		VC_TBytes[jj] = AByte;
	}
}
//---------------------------------------------------------------------------
//�·�����ָ�� �豸��ʶ��Ϊ�ⲿ����
/********
* startpos ��ʼλ��
* paranum  ��Ҫ����������
* dp       Դ����ָ��
* VC_TBytes   ���ݽӿ�
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
		lastpkgnum = paranum % 1024;
	}
	VC_TBytes.resize(pkgnum);
	//������1024�ֽ����ϵİ�
	for (jj=0; jj<pkgnum; jj++)
	{
		UDPSysPara = (TUDPSysPara *)NetSBuf;
		strcpy(UDPSysPara->Cmd,"SetPara");

		strcpy(UDPSysPara->IdentifyingCode,EquIdStr);
		UDPSysPara->StartByte = 0xca;
		UDPSysPara->CmdType = 0x06;
		UDPSysPara->ParaStaAdd = startpos + jj;//*1024;
		//���һ���Ͳ���1024���� ������
		if (jj == pkgnum-1) UDPSysPara->ParaNum = lastpkgnum;
		else UDPSysPara->ParaNum = 1024;
		memcpy(UDPSysPara->Para,dp,paranum);

		AByte.set_length(sizeof(TUDPSetParaConfirm) + UDPSysPara->ParaNum);
		for (int i=AByte.Low; i<=AByte.High; i++){
			AByte[i] = NetSBuf[i];
		}
		VC_TBytes[jj] = AByte;
	}
	return VC_TBytes.size();
}
//---------------------------------------------------------------------------
//Уʱ port 15288 �㲥��ʽУʱ(����ʶ����)
/************
* AByte ���ݽӿ�
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
//�ǹ�˾��׼UDP���(UDP͸��)
/****************
* data  Դ����
* size  ���ݳ���(�ֽ�)
* AByte ���ݽӿ�
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
//���CAN���ģ����ְ���
/**************
* address   װ�õ�ַ
* len       ���������ĳ���
* cmd       ָ��
* dp        ��������������
* AByte     ���ݽӿ�
**************/
void __fastcall PackCan(int address,int dp_len,int cmd,const void* dp,TBytes &AByte)
{
	u8 NetSBuf[1500] = {0x00};
	TCanTRDat* pCanTRDat = reinterpret_cast<TCanTRDat*>(NetSBuf);
	int packlen = dp_len + offsetof(TCanTRDat, CanDat);                 //�ܱ��ĳ���

	pCanTRDat->Address = address;         //װ�õ�ַ
	pCanTRDat->len = dp_len;              //���������ĳ���
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







