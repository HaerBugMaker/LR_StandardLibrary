//---------------------------------------------------------------------------
/****************************************
* 导入导出bin文件类（通用类）
****************************************/
#ifndef TOutInputBinFileH
#define TOutInputBinFileH
#include "UDPStructs.h"
#include <System.SysUtils.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TOutInputBinFile{
private:
	struct TBinData{
		char FileID[20]{"PowBackConfigBin"};   //文件标识符(第一重识别)
		TSysPara SysPara;  //系统参数
	};
public:
	__fastcall TOutInputBinFile(){};
	virtual __fastcall ~TOutInputBinFile(){};
	//导出
	static bool __fastcall BinFileOutput(const TSysPara &rSysPara,const String &FilePath);
	//导入
	static bool __fastcall BinFileInput(TSysPara &rSysPara,const String &FilePath);
};

#endif
