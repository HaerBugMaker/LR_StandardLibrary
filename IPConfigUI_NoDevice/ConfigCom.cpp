//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ConfigCom.h"
#include <algorithm>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tconfigform *configform;
//---------------------------------------------------------------------------
//构造函数用于带参调用
/*************
* Owner 用于初始化父类
* f_mainip 和调用方的IP接口
*************/
__fastcall Tconfigform::Tconfigform(TComponent* Owner,TMyIp &f_mainip)
	: TForm(Owner),m_MainIP(f_mainip)
{
}
//---------------------------------------------------------------------------
void __fastcall Tconfigform::FormShow(TObject *Sender)
{
	try{
		if(GetMyIP(this->m_VC_MyIpPool) == 0) throw Exception(L"无可用IP");
		DataToUI();
	}
	catch(const Exception &err){
		ShowMessage(err.Message);
	}
}
//---------------------------------------------------------------------------
void __fastcall Tconfigform::DataToUI(void)
{
	LocalIPSelRgp->Items->Clear();
	for(const auto &myip:this->m_VC_MyIpPool){
		LocalIPSelRgp->Items->Add(myip.IpAddr);
	}
	const auto ipit = std::find_if(this->m_VC_MyIpPool.begin(),this->m_VC_MyIpPool.end(),
								   [&](const TMyIp &myip){if(myip.IpAddr == this->m_MainIP.IpAddr)
															{return true;}
														  else
															{return false;}});
	LocalIPSelRgp->ItemIndex = ipit == this->m_VC_MyIpPool.end()?-1:ipit - this->m_VC_MyIpPool.begin();
}
//---------------------------------------------------------------------------
void __fastcall Tconfigform::LocalIPSelRgpClick(TObject *Sender)
{
	MaskLE->Text = this->m_VC_MyIpPool.at(LocalIPSelRgp->ItemIndex).Mask;
	BcastLE->Text = this->m_VC_MyIpPool.at(LocalIPSelRgp->ItemIndex).Bcast;
	MacLE->Text = MACCharToStr(this->m_VC_MyIpPool.at(LocalIPSelRgp->ItemIndex).MAC);
}
//---------------------------------------------------------------------------
void __fastcall Tconfigform::BitBtn1Click(TObject *Sender)
{
    if(this->m_VC_MyIpPool.empty()) return;
	this->m_MainIP = this->m_VC_MyIpPool.at(LocalIPSelRgp->ItemIndex);
    this->ModalResult = mrOk;
}
//---------------------------------------------------------------------------
