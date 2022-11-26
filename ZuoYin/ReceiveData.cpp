#include "Class.h"
#include "ReceiveData.h"
#include "Singleton.h"

//接收的数据（命令，棋谱，布局，定式）
ReceiveDataFromPlayer::ReceiveDataFromPlayer(Command& command, std::vector<XY>& qizis, LayoutAttribute& layout_attribute)
{
	this->command = command;
	this->qizis = qizis;
	this->layout_attribute = layout_attribute;
}

//处理数据
BackData* ReceiveDataFromPlayer::disposeData()
{
	return Singleton::getInstance().disposePlayerData(*this);
}

//接收设置者数据（定式/布局，棋谱）
ReceiveDataFromSetter::ReceiveDataFromSetter(std::vector<XY>& qizis, std::string& type)
{
	this->qizis = qizis;
	this->type = type;
}

//处理数据
void ReceiveDataFromSetter::disposeData()
{
	Singleton::getInstance().disposeSetterData(*this);
}
