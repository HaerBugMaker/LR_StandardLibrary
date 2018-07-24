//---------------------------------------------------------------------------
/****************************************
* ���뵼��bin�ļ��ࣨͨ���ࣩ
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
		char FileID[20]{"PowBackConfigBin"};   //�ļ���ʶ��(��һ��ʶ��)
		TSysPara SysPara;  //ϵͳ����
	};
public:
	__fastcall TOutInputBinFile(){};
	virtual __fastcall ~TOutInputBinFile(){};
	//����
	static bool __fastcall BinFileOutput(const TSysPara &rSysPara,const String &FilePath);
	//����
	static bool __fastcall BinFileInput(TSysPara &rSysPara,const String &FilePath);
};

#endif
