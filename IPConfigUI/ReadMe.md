通讯配置界面
调用方调用界面，调用方的TMyIp和TDeviceInfo按照引用方式被传递，修改
调用代码如下
Tconfigform* pconfigform = new Tconfigform(this,this->m_ActiveIP,this->m_DeviceInfo);
	int modalresult = pconfigform->ShowModal();
	if(modalresult == mrOk){
		SetBindings();
		DataToUI(TUIType::stab);
		ShowMessage(L"配置成功");
	}
	delete pconfigform;
