//---------------------------------------------------------------------------
/****************************************************************************
* UDP�����
* UDP����ѭ�����ط�����
* ������ִ���κ�һ����������ȫ���ж�
* ����ص���Ҫ��;��FLASH�շ�����Ҫ���˳���͵Ļ���
* 2018.8.14 IsWantedData�����ж�InProcess
****************************************************************************/

#ifndef TUDPTaskPoolH
#define TUDPTaskPoolH
#include <deque>
#include <vector>
#include <System.Classes.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPServer.hpp>
#include <Vcl.Dialogs.hpp>

//---------------------------------------------------------------------------
//������     �� д
enum TTaskDire{Rd,Wr};
//���Դ���
const int RETRYTIMES = 20;

//UDP����Ԫ
struct TUDPTaskCell{
	int RetryTimes{0};                  //���Դ��� ��ʼ��Ϊ0��
	String DestIP{L"255.255.255.255"};  //Ŀ��IP
	int DestPort{2305};                 //Ŀ��˿�
	TBytes ActivePkg;                   //��ǰ��
	std::vector<TBytes> vc_TBytes;      //��������(�洢���)
	int WantedParaStaAdd{0};            //������ʼ��ַ(����������1)
	int PkgIncStep{1024};               //����ʼ��ַ��������
	TTaskDire TaskDire{TTaskDire::Rd};  //���ݷ���(����������2)
	TDateTime TimeStamp;                //����ʱ���
	String TaskName;                    //��ǰ��������
	bool InProcess{false};              //�Ƿ������(�����ڱ�ʶ�����Ƿ�ʧ�� ���InProcess && Execeute����-1 ������ʧ��)

	/***************
	* ִ�б�����
	* pUDPSvr   :   UDP���Ϳؼ�
    * ����ֵ    :   ʣ�����Դ���
	***************/
	int __fastcall Execute(TIdUDPServer *pUDPSvr){
		if(RetryTimes <= 0) return -1;                       //���Դ����Ѿ�����
		if(pUDPSvr == nullptr) return -1;                    //��ֹ���ݿ�ָ��
		if(ActivePkg.Length == 0) return -1;                 //��ֹ������
			pUDPSvr->Bindings->Items[0]->SendTo(DestIP,DestPort,ActivePkg);
		RetryTimes--;                                        //���Դ���-1
		TimeStamp = Now();                                   //ʱ�������
		return RetryTimes;
	}

	/****************
	* ��������
	* TaskName      :  ��������
	* WantedStaAdd  :  ����������ʼ��ַ(������ɱ�ʶ1)
	* PkgIncStep    :  ����ʼ��ַ��������
	* TaskDire      :  ������(������ɱ�ʶ2)
	* vc_TBytes     :  UDP���ݰ�����
	* DestIP        :  Ŀ��IP��ַ
	* DestPort      :  Ŀ��˿� ��ȱʡ 2305
	* ����ֵ        :  ����������
	****************/
	static TUDPTaskCell __fastcall Task_Create(const String &TaskName,int WantedParaStaAdd,int PkgIncStep,TTaskDire TaskDire,std::vector<TBytes> &vc_TBytes,const String &DestIP,
												int DestPort = 2305){
		TUDPTaskCell nudptask;
		nudptask.TaskName = TaskName;                   //��������
		nudptask.RetryTimes = RETRYTIMES;               //���Դ���
		nudptask.DestPort = DestPort;                   //Ŀ��˿�
		nudptask.DestIP = DestIP;                       //Ŀ��IP��ַ
		nudptask.vc_TBytes = vc_TBytes;                 //UDP���ݰ�����
		nudptask.WantedParaStaAdd = WantedParaStaAdd;   //������ʼ��ַ(����������1)
		nudptask.PkgIncStep = PkgIncStep;               //����ʼ��ַ��������
		nudptask.TaskDire = TaskDire;                   //���ݷ���(����������2)
		if(!nudptask.vc_TBytes.empty()){                //�ǿ���ָ�ɵ�һ��Ϊ��ǰ���
			nudptask.ActivePkg = vc_TBytes.front();
			nudptask.vc_TBytes.erase(nudptask.vc_TBytes.begin());  //�Ƴ��װ�
		}
		nudptask.InProcess = true;                      //���������
		return nudptask;
	}

	/**********************
	* ����յ��������Ƿ�ǰ��������������
	* fParastaadd  :  ������ʼ��ַ
	* fTaskDire    :  ���ݷ���
	**********************/
	bool __fastcall IsWantedData(int fParastaadd,TTaskDire fTaskDire){
		if((WantedParaStaAdd == fParastaadd)&&(TaskDire == fTaskDire)&&(InProcess == true))
			{return true;}
		else
			{return false;}
	}

	/**********************
	* �������
	**********************/
	void __fastcall Finish(void){
		RetryTimes = 0;             //���Դ�����0 Ҳ�����������
		InProcess = false;          //�����ڽ�����
	}

	/**********************
	* ����Ƿ��ж��  �����Զ�����ActivePkg
	* ����ֵ   :    true ��   false û��
	**********************/
	bool __fastcall CheckHasFuture(void){
		//�޸����
		if(vc_TBytes.empty())
			{return false;}
		//���и����
		else{
			ActivePkg = vc_TBytes.front();       //�������
			vc_TBytes.erase(vc_TBytes.begin());  //�Ƴ��װ�
			RetryTimes = RETRYTIMES;             //���Դ�����ԭ
			WantedParaStaAdd += PkgIncStep;      //������ʼ��ַ
            return true;
		}
	}

};
//---------------------------------------------------------------------------
#endif
