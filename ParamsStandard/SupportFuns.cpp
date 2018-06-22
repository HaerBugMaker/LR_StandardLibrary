//---------------------------------------------------------------------------

#pragma hdrstop

#include "SupportFuns.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//---------------------------------------------------------------------------
//StringGrid重复列判断
/*******************
* tstr 接收列数据
* 抛出异常字符串
******************/
void __fastcall GridRepeatJudge(TStrings* tstr)
{
	String errstr;
	int errrow;
	for(int i=1;i<tstr->Count;i++){
		if((errrow =tstr->IndexOf(tstr->Strings[i])) != i){
			errstr = L"第" + IntToStr(errrow) + L"和" + IntToStr(i) + L"行出现重复";
			throw Exception(errstr);
		}
	}
}
//---------------------------------------------------------------------------
//表格取消选中
/*************
* StringGridtmp 目标表格
*************/
void __fastcall CancelGridSel(TStringGrid* StringGridtmp)
{
	TGridRect myRect;
	myRect.Left = StringGridtmp->ColCount+5;
	myRect.Top = StringGridtmp->RowCount+5;
	myRect.Right = StringGridtmp->ColCount+5;
	myRect.Bottom = StringGridtmp->RowCount+5;
	StringGridtmp->Selection = myRect;
}
//---------------------------------------------------------------------------
//清空表格
/*************
* StringGridtmp 目标表格
*************/
void __fastcall ClearGrid(TStringGrid* StringGridtmp)
{
	for(int row=0;row<StringGridtmp->RowCount;row++){
		for(int col=0;col<StringGridtmp->ColCount;col++){
			StringGridtmp->Cells[col][row] = L"";
		}
	}
}
//---------------------------------------------------------------------------
//表格查找_行
/******************
* StringGridtmp 目标单元格
* col           目标列
* val           目标值
******************/
int __fastcall LocateGrid_Row(TStringGrid* StringGridtmp,int col,const String &val)
{
	TStrings *tstr = StringGridtmp->Cols[col];
  /*	for(int row=0;row<StringGridtmp->RowCount;row++){
		if(StringGridtmp->Cells[col][row] == val) return row;
	}        */
	return -1;
}
//---------------------------------------------------------------------------
//TCustomEdit系列控件对齐样式设定
/*******************
* Edit 编辑框
* style 编辑框对齐样式
*******************/
void __fastcall TEditStyleSet(TCustomEdit *Edit,DWORD style)
{
	DWORD dwStyle = ::GetWindowLong(Edit->Handle,GWL_STYLE);
	// ES_LEFT: 左对齐, ES_CENTER: 居中, ES_RIGHT: 右对齐
	dwStyle = dwStyle | style;
	::SetWindowLong(Edit->Handle, GWL_STYLE, dwStyle);
	Edit->Invalidate();
}
//---------------------------------------------------------------------------
//stringgrid的特殊样式
/*****************
* StringGrid 表格控件
* ACol 目标列
* ARow 目标行
* str  目标文本
* Style 样式  0左对齐 1 居中 2右对齐 默认0可缺省
*****************/
void __fastcall TStringGridStyleInput(TStringGrid* StringGrid,int ACol,int ARow,const String &str,int Style)
{
	TRect rect = StringGrid->CellRect(ACol,ARow);
	int width = StringGrid->Canvas->TextWidth(str);
	int height = StringGrid->Canvas->TextHeight(str);
	switch (Style) {
		//左对齐
		case 0:{
			StringGrid->Canvas->TextRect(rect,rect.Left,(rect.Top+rect.Bottom-height)/2,str);
			break;
		}
		//居中
		case 1:{
			StringGrid->Canvas->TextRect(rect,(rect.Left+rect.Right-width)/2,(rect.Top+rect.Bottom-height)/2,str);
			break;
		}
		//右对齐
		case 2:{
			StringGrid->Canvas->TextRect(rect,rect.Right-width,(rect.Top+rect.Bottom-height)/2,str);
			break;
		}
	}

}
//---------------------------------------------------------------------------


