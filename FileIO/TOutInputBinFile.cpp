//---------------------------------------------------------------------------

#pragma hdrstop

#include "TOutInputBinFile.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------

/**************
* 导出bin文件
* rSysPara 待导出系统参数
* FilePath 导出文件路径
* 返回值 true 导出成功 false 导出失败
**************/
bool __fastcall TOutInputBinFile::BinFileOutput(const TSysPara &rSysPara,const String &FilePath)
{
	int iFileHandle{-1};
	try{
		TBinData nBinData;
		memcpy(&nBinData.SysPara,&rSysPara,sizeof(rSysPara));    //数据拷贝

		if(!FileExists(FilePath)) iFileHandle = FileCreate(FilePath);
		else iFileHandle = FileOpen(FilePath, fmOpenWrite);
		if (iFileHandle == -1) throw Exception(L"文件打开/创建失败");

		FileWrite(iFileHandle,&nBinData,sizeof(nBinData));
		FileClose(iFileHandle);
		return true;
	}
	catch(Exception &err){
		FileClose(iFileHandle);
		ShowMessage(L"导出遇到问题:" + err.Message);
        return false;
	}
}
//---------------------------------------------------------------------------
/**************
* 导入bin文件
* rSysPara 导入系统参数数据接口 同样也是文件匹配判断依据
* FilePath 导入文件路径
* 返回值 true 成功 false 失败
* 进行匹配识别
**************/
bool __fastcall TOutInputBinFile::BinFileInput(TSysPara &rSysPara,const String &FilePath)
{
	int iFileHandle{-1};
	try{
		iFileHandle = FileOpen(FilePath, fmOpenRead);
		if(iFileHandle == -1) throw Exception(L"文件打开失败");

		TBinData nBinData;
		FileSeek(iFileHandle,0,0);
		int res = FileRead(iFileHandle,&nBinData,sizeof(nBinData));

		if(res == -1)
			{throw Exception(L"文件读取失败");}
		if(strcmp("PowBackConfigBin",nBinData.FileID) != 0)
			{throw Exception(L"文件类型不匹配");}

		if((strcmp(rSysPara.EquType,nBinData.SysPara.EquType) != 0))
			{throw Exception(L"设备类型不匹配");}
		if((strcmp(rSysPara.FWVer,nBinData.SysPara.FWVer) != 0))
			{throw Exception(L"设备版本不匹配");}

		//通过了重重考验,覆盖rSysPara的项目特殊部分
		memcpy(reinterpret_cast<u8*>(&rSysPara) + offsetof(TSysPara,DeviceType),reinterpret_cast<u8*>(&nBinData) + offsetof(TBinData,SysPara) + offsetof(TSysPara,DeviceType),
				 sizeof(rSysPara) - offsetof(TSysPara,DeviceType));
		FileClose(iFileHandle);

		return true;
	}
	catch(Exception &err){
		FileClose(iFileHandle);
		ShowMessage(L"导入异常:" + err.Message);
		return false;
	}
}
//---------------------------------------------------------------------------

