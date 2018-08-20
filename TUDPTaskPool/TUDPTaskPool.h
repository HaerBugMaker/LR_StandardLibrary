//---------------------------------------------------------------------------
/****************************************************************************
* UDP任务池
* UDP任务循环，重发机制
* 任务串行执行任何一环出错都任务全部中断
* 任务池的主要用途是FLASH收发中需要多包顺序发送的环节
* 2018.8.14 IsWantedData函数判断InProcess
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
//任务方向     读 写
enum TTaskDire{Rd,Wr};
//重试次数
const int RETRYTIMES = 20;

//UDP任务单元
struct TUDPTaskCell{
	int RetryTimes{0};                  //重试次数 初始化为0次
	String DestIP{L"255.255.255.255"};  //目标IP
	int DestPort{2305};                 //目标端口
	TBytes ActivePkg;                   //当前包
	std::vector<TBytes> vc_TBytes;      //数据内容(存储多包)
	int WantedParaStaAdd{0};            //期望起始地址(任务特征标1)
	int PkgIncStep{1024};               //包起始地址递增步长
	TTaskDire TaskDire{TTaskDire::Rd};  //数据方向(任务特征标2)
	TDateTime TimeStamp;                //任务时间戳
	String TaskName;                    //当前任务名称
	bool InProcess{false};              //是否进行中(仅用于标识任务是否失败 如果InProcess && Execeute返回-1 则任务失败)

	/***************
	* 执行本任务
	* pUDPSvr   :   UDP发送控件
    * 返回值    :   剩余重试次数
	***************/
	int __fastcall Execute(TIdUDPServer *pUDPSvr){
		if(RetryTimes <= 0) return -1;                       //重试次数已经用完
		if(pUDPSvr == nullptr) return -1;                    //防止传递空指针
		if(ActivePkg.Length == 0) return -1;                 //防止空数据
			pUDPSvr->Bindings->Items[0]->SendTo(DestIP,DestPort,ActivePkg);
		RetryTimes--;                                        //重试次数-1
		TimeStamp = Now();                                   //时间戳更新
		return RetryTimes;
	}

	/****************
	* 创建任务
	* TaskName      :  任务名称
	* WantedStaAdd  :  期望数据起始地址(任务完成标识1)
	* PkgIncStep    :  包起始地址递增步长
	* TaskDire      :  任务方向(任务完成标识2)
	* vc_TBytes     :  UDP数据包内容
	* DestIP        :  目标IP地址
	* DestPort      :  目标端口 可缺省 2305
	* 返回值        :  创建的任务
	****************/
	static TUDPTaskCell __fastcall Task_Create(const String &TaskName,int WantedParaStaAdd,int PkgIncStep,TTaskDire TaskDire,std::vector<TBytes> &vc_TBytes,const String &DestIP,
												int DestPort = 2305){
		TUDPTaskCell nudptask;
		nudptask.TaskName = TaskName;                   //任务名称
		nudptask.RetryTimes = RETRYTIMES;               //重试次数
		nudptask.DestPort = DestPort;                   //目标端口
		nudptask.DestIP = DestIP;                       //目标IP地址
		nudptask.vc_TBytes = vc_TBytes;                 //UDP数据包内容
		nudptask.WantedParaStaAdd = WantedParaStaAdd;   //期望起始地址(任务特征标1)
		nudptask.PkgIncStep = PkgIncStep;               //包起始地址递增步长
		nudptask.TaskDire = TaskDire;                   //数据方向(任务特征标2)
		if(!nudptask.vc_TBytes.empty()){                //非空则指派第一包为当前活动包
			nudptask.ActivePkg = vc_TBytes.front();
			nudptask.vc_TBytes.erase(nudptask.vc_TBytes.begin());  //移除首包
		}
		nudptask.InProcess = true;                      //任务进行中
		return nudptask;
	}

	/**********************
	* 检测收到的数据是否当前任务期望的数据
	* fParastaadd  :  数据起始地址
	* fTaskDire    :  数据方向
	**********************/
	bool __fastcall IsWantedData(int fParastaadd,TTaskDire fTaskDire){
		if((WantedParaStaAdd == fParastaadd)&&(TaskDire == fTaskDire)&&(InProcess == true))
			{return true;}
		else
			{return false;}
	}

	/**********************
	* 完成任务
	**********************/
	void __fastcall Finish(void){
		RetryTimes = 0;             //重试次数置0 也就是任务完成
		InProcess = false;          //任务不在进行中
	}

	/**********************
	* 检测是否有多包  有则自动更换ActivePkg
	* 返回值   :    true 有   false 没有
	**********************/
	bool __fastcall CheckHasFuture(void){
		//无更多包
		if(vc_TBytes.empty())
			{return false;}
		//还有更多包
		else{
			ActivePkg = vc_TBytes.front();       //更换活动包
			vc_TBytes.erase(vc_TBytes.begin());  //移除首包
			RetryTimes = RETRYTIMES;             //重试次数还原
			WantedParaStaAdd += PkgIncStep;      //更新起始地址
            return true;
		}
	}

};
//---------------------------------------------------------------------------
#endif
