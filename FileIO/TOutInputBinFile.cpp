//---------------------------------------------------------------------------

#pragma hdrstop

#include "TOutInputBinFile.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------

/**************
* ����bin�ļ�
* rSysPara ������ϵͳ����
* FilePath �����ļ�·��
* ����ֵ true �����ɹ� false ����ʧ��
**************/
bool __fastcall TOutInputBinFile::BinFileOutput(const TSysPara &rSysPara,const String &FilePath)
{
	int iFileHandle{-1};
	try{
		TBinData nBinData;
		memcpy(&nBinData.SysPara,&rSysPara,sizeof(rSysPara));    //���ݿ���

		if(!FileExists(FilePath)) iFileHandle = FileCreate(FilePath);
		else iFileHandle = FileOpen(FilePath, fmOpenWrite);
		if (iFileHandle == -1) throw Exception(L"�ļ���/����ʧ��");

		FileWrite(iFileHandle,&nBinData,sizeof(nBinData));
		FileClose(iFileHandle);
		return true;
	}
	catch(Exception &err){
		FileClose(iFileHandle);
		ShowMessage(L"������������:" + err.Message);
        return false;
	}
}
//---------------------------------------------------------------------------
/**************
* ����bin�ļ�
* rSysPara ����ϵͳ�������ݽӿ� ͬ��Ҳ���ļ�ƥ���ж�����
* FilePath �����ļ�·��
* ����ֵ true �ɹ� false ʧ��
* ����ƥ��ʶ��
**************/
bool __fastcall TOutInputBinFile::BinFileInput(TSysPara &rSysPara,const String &FilePath)
{
	int iFileHandle{-1};
	try{
		iFileHandle = FileOpen(FilePath, fmOpenRead);
		if(iFileHandle == -1) throw Exception(L"�ļ���ʧ��");

		TBinData nBinData;
		FileSeek(iFileHandle,0,0);
		int res = FileRead(iFileHandle,&nBinData,sizeof(nBinData));

		if(res == -1)
			{throw Exception(L"�ļ���ȡʧ��");}
		if(strcmp("PowBackConfigBin",nBinData.FileID) != 0)
			{throw Exception(L"�ļ����Ͳ�ƥ��");}

		if((strcmp(rSysPara.EquType,nBinData.SysPara.EquType) != 0))
			{throw Exception(L"�豸���Ͳ�ƥ��");}
		if((strcmp(rSysPara.FWVer,nBinData.SysPara.FWVer) != 0))
			{throw Exception(L"�豸�汾��ƥ��");}

		//ͨ�������ؿ���,����rSysPara����Ŀ���ⲿ��
		memcpy(reinterpret_cast<u8*>(&rSysPara) + offsetof(TSysPara,DeviceType),reinterpret_cast<u8*>(&nBinData) + offsetof(TBinData,SysPara) + offsetof(TSysPara,DeviceType),
				 sizeof(rSysPara) - offsetof(TSysPara,DeviceType));
		FileClose(iFileHandle);

		return true;
	}
	catch(Exception &err){
		FileClose(iFileHandle);
		ShowMessage(L"�����쳣:" + err.Message);
		return false;
	}
}
//---------------------------------------------------------------------------

