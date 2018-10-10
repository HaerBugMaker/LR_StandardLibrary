//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "about.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ExeInfo"
#pragma link "dxGDIPlusClasses"
#pragma resource "*.dfm"
Taboutform *aboutform;
//---------------------------------------------------------------------------
__fastcall Taboutform::Taboutform(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Taboutform::FormShow(TObject *Sender)
{
  	FileDescLB->Caption = ExeInfo1->FileDescription;           //文件描述
	FileVersionLB->Caption = L"Version:" + ExeInfo1->FileVersion; //文件版本
}
//---------------------------------------------------------------------------
