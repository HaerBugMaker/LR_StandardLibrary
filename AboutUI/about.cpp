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
  	FileDescLB->Caption = ExeInfo1->FileDescription;           //�ļ�����
	FileVersionLB->Caption = L"Version:" + ExeInfo1->FileVersion; //�ļ��汾
}
//---------------------------------------------------------------------------
