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
* f_DeviceInfo 和调用方的设备数据接口
*************/
__fastcall Tconfigform::Tconfigform(TComponent* Owner,TMyIp &f_mainip,TDeviceInfo &f_DeviceInfo)
	: TForm(Owner),m_MainIP(f_mainip),m_DeviceInfo(f_DeviceInfo)
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
	DeviceIPLE->Text = this->m_DeviceInfo.IP;
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
	try{
		if(this->m_VC_MyIpPool.empty()) return;
		this->m_MainIP = this->m_VC_MyIpPool.at(LocalIPSelRgp->ItemIndex);
		if(WhetherValid_IP(DeviceIPLE->Text))
			{this->m_DeviceInfo.IP = DeviceIPLE->Text;}
		else
			{throw Exception(L"设备IP不合法");}
		this->ModalResult = mrOk;
	}
	catch(Exception &err){
		ShowMessage(L"保存出现异常:" + err.Message);
	}
}
//---------------------------------------------------------------------------
