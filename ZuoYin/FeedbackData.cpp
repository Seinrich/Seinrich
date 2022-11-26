#include "Class.h"
#include "FeedbackData.h"

//反馈的数据（类型）
BackData::BackData(std::string type)
{
	this->type = type;
}

//反馈的数据
BackData::BackData()
{
	this->type = "null";
}

std::string BackData::classID()
{
	return "BackData";
}

//刷新反馈（类型和棋谱）
FlushBackData::FlushBackData(std::string type, std::vector<XY>& xys, LayoutAttribute& layout_attribute)
{
	this->type = type;
	this->xys = xys;
	this->layout_attribute = layout_attribute;
}

std::string FlushBackData::classID()
{
	return "FlushBackData";
}

//提示反馈（类型和提示）
TipBackData::TipBackData(std::string type, std::vector<XY>& xys)
{
	this->type = type;
	this->xys = xys;
}

std::string TipBackData::classID()
{
	return "TipBackData";
}

//落子反馈（类型和坐标）
LuoziBackData::LuoziBackData(std::string type, XY xy)
{
	this->type = type;
	this->xy = xy;
}

std::string LuoziBackData::classID()
{
	return "LuoziBackData";
}
