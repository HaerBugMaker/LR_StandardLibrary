//---------------------------------------------------------------------------
/**********************
* 常用一些辅助函数
* 本库中存在异常抛出函数 使用请酌情处理异常
 Git  版本记录
* 2018.1.30 增加编辑框居右功能
* 2018.5.3 修改异常抛出类型为标准Exception类型
*********************/
#ifndef SupportFunsH
#define SupportFunsH
#include <System.Classes.hpp>
#include <Vcl.Grids.hpp>

//---------------------------------------------------------------------------
//自定义消息机制测试
#define MY_MSG WM_USER + 0x310
#define MYMSG_ASK_AVAILABLEDEVICE WM_USER + 0x310
//---------------------------------------------------------------------------
/*****************
* 设备剪贴的格式
* 2018.1.15版本
*****************/
struct TClipDevice{
	char ClipID[50];        //剪贴板识别字 是目标配置软件的EXE文件名 目标配置软件也这么验证
	char DeviceIP[20];      //设备IP
	char DeviceMAC[20];     //设备MAC
};
/*****************
* 带参打开设备参数结构
* 2018.1.15版本
*****************/
struct TParams{
	String AppIP;       //软件IP
	String DeviceIP;    //设备IP
	String ServerAppHwndName;  //窗体名字

};


//---------------------------------------------------------------------------
//StringGrid重复列判断
void __fastcall GridRepeatJudge(TStrings* tstr);
//表格取消选中
/*************
* StringGridtmp 目标表格
*************/
void __fastcall CancelGridSel(TStringGrid* StringGridtmp);
//清空表格
/*************
* StringGridtmp 目标表格
*************/
void __fastcall ClearGrid(TStringGrid* StringGridtmp);
//表格查找_行
/******************
* StringGridtmp 目标单元格
* col           目标列
* val           目标值
******************/
int __fastcall LocateGrid_Row(TStringGrid* StringGridtmp,int col,const String &val);
//TEdit文字风格设定
/*******************
* Edit 编辑框
* style 编辑框对齐样式
*******************/
void __fastcall TEditStyleSet(TCustomEdit *Edit,DWORD style);
//stringgrid的特殊样式
/*****************
* StringGrid 表格控件
* ACol 目标列
* ARow 目标行
* str  目标文本
* Style 样式  0左对齐 1 居中 2右对齐 默认0可缺省
*****************/
void __fastcall TStringGridStyleInput(TStringGrid* StringGrid,int ACol,int ARow,const String &str,int Style = 0);
#endif
