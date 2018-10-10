//---------------------------------------------------------------------------

#ifndef aboutH
#define aboutH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include "ExeInfo.hpp"
#include "dxGDIPlusClasses.hpp"
//---------------------------------------------------------------------------
class Taboutform : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
	TLabel *FileDescLB;
	TLabel *FileVersionLB;
	TExeInfo *ExeInfo1;
	TLabel *Label1;
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall Taboutform(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE Taboutform *aboutform;
//---------------------------------------------------------------------------
#endif
