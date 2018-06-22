//---------------------------------------------------------------------------
/**************************
* 通讯配置
* 2018.4.28 界面微调 显示MAC
* 2018.5.21 保存设备IP不对则抛出异常
**************************/

#ifndef ConfigComH
#define ConfigComH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include "Udp_opt.h"
#include <vector>
//---------------------------------------------------------------------------
class Tconfigform : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
	TRadioGroup *LocalIPSelRgp;
	TLabeledEdit *MaskLE;
	TLabeledEdit *BcastLE;
	TBitBtn *BitBtn1;
	TLabeledEdit *MacLE;
	TLabeledEdit *DeviceIPLE;
	TImage *Image2;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall LocalIPSelRgpClick(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
	TMyIp &m_MainIP;
	TDeviceInfo &m_DeviceInfo;
	std::vector<TMyIp> m_VC_MyIpPool;
public:		// User declarations
	__fastcall Tconfigform(TComponent* Owner,TMyIp &f_mainip,TDeviceInfo &f_DeviceInfo);
	void __fastcall DataToUI(void);
};
//---------------------------------------------------------------------------
extern PACKAGE Tconfigform *configform;
//---------------------------------------------------------------------------
#endif
