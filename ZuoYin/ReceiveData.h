#pragma once
#include "Class.h"
#include "FeedbackData.h"

//接受玩家数据
class ReceiveDataFromPlayer {
public:
	Command command;
	std::vector<XY> qizis;
	LayoutAttribute layout_attribute;
	ReceiveDataFromPlayer(Command& command, std::vector<XY>& qizis, LayoutAttribute& layout_attribute);
	//处理数据
	BackData* disposeData();
};

//接受setter数据
class ReceiveDataFromSetter {
public:
	std::vector<XY> qizis;
	std::string type;
	ReceiveDataFromSetter(std::vector<XY>& qizis, std::string& type);
	//处理数据
	void disposeData();
};