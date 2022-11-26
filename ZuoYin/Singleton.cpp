#include "Singleton.h"
#include <iostream>
#include "JsonUtils.h"
#include "StringUtils.h"
#include <algorithm>

Singleton& Singleton::operator=(const Singleton&)
{
	return *this;
}

void Singleton::buildPatternTree()
{
	//初始化
	initHead(&patternhead);
	//添加
	for (int i = 0; i < patterns.size(); i++) {
		AddToTree(&patternhead, patterns[i].xys);
	}
	setWeight(&patternhead, 0);
}

void Singleton::updateTree(std::vector<XY>& xys)
{
	AddToTree(&patternhead, xys);
}

std::string Singleton::disposeLocation(int location, Qipu& qipu)
{
	switch (location)
	{
	case 0:
		return "topleft-right";
	case 1:
		qipu.turnOverFront();
		return "topleft-left";
	case 2:
		qipu.rotate();
		return "topright-right";
	case 3:
		qipu.rotate();
		qipu.turnOverBack();
		return "topright-left";
	case 4:
		qipu.rotate();
		qipu.rotate();
		return "bottomright-right";
	case 5:
		qipu.rotate();
		qipu.rotate();
		qipu.turnOverFront();
		return "bottomright-left";
	case 6:
		qipu.rotate();
		qipu.rotate();
		qipu.rotate();
		return "bottomleft-right";
	case 7:
		qipu.rotate();
		qipu.rotate();
		qipu.rotate();
		qipu.turnOverBack();
		return "bottomleft-left";
	default:
		return "topleft-right";
	}
}

int Singleton::getLocation(std::string& location)
{
	if (location == "topleft-right") {
		return 0;
	}
	else if (location == "topleft-left") {
		return 1;
	}
	else if (location == "toplright-right") {
		return 2;
	}
	else if (location == "topright-left") {
		return 3;
	}
	else if (location == "bottomright-right") {
		return 4;
	}
	else if (location == "bottomright-left") {
		return 5;
	}
	else if (location == "bottomleft-right") {
		return 6;
	}
	else if (location == "bottomleft-left") {
		return 7;
	}
	else {
		return 0;
	}
}

std::string Singleton::getName(XY xy)
{
	if (xy.x == 3 && xy.y == 2) {
		//小目
		return "xiaomu";
	}
	else if (xy.x == 3 && xy.y == 3) {
		//星位
		return "xingwei";
	}
	else if (xy.x == 2 && xy.y == 2) {
		//三三
		return "sansan";
	}
	else if (xy.x == 4 && xy.y == 3) {
		//高目
		return "gaomu";
	}
	else if (xy.x == 4 && xy.y == 2) {
		//目外
		return "muwai";
	}
	else {
		return "error";
	}
}

int Singleton::getDegree(std::string& degree)
{
	if (degree == "easy") {
		return 1;
	}
	else if (degree == "medium") {
		return 2;
	}
	else if (degree == "hard") {
		return 3;
	}
	else {
		return 1;
	}
}

bool compare(XYW a, XYW b) {
	return a.weight < b.weight;
}

XY Singleton::getXY(std::vector<XYW>& xyws, int degree)
{
	srand(time(NULL));
	std::sort(xyws.begin(),xyws.end(),compare);
	if (degree == 2) {
		return xyws[rand() % xyws.size()].xy;
	}
	else if (degree == 3) {
		return xyws[xyws.size() - 1].xy;
	}
	else {
		return xyws[0].xy;
	}
}

int findPatternType(Node head, std::string name) {
	if (name == "xingwei") {
		return find(head->nexts, *new XY(3, 3));
	}
	else if (name == "xiaomu") {
		return find(head->nexts, *new XY(3, 2));
	}
	else if (name == "sansan") {
		return find(head->nexts, *new XY(2, 2));
	}
	else if (name == "gaomu") {
		return find(head->nexts, *new XY(4, 3));
	}
	else if (name == "muwai") {
		return find(head->nexts, *new XY(4, 2));
	}
}

//处理数据
BackData* Singleton::disposePlayerData(ReceiveDataFromPlayer& data)
{
	srand(time(NULL));
	if (data.command.type == "flush") {
		int index = rand() % layouts.size();
		int location = rand() % 8;
		Layout layout = *new Layout(layouts[index].xys, layouts[index].Lname, layouts[index].Llocation);
		//变换棋谱
		layout.Llocation = disposeLocation(location, layout);

		//随机先后手
		int choice = rand() % 2;
		//玩家挂角
		if (choice == 0) {
			return new FlushBackData(
				"flush", layout.xys, *new LayoutAttribute(
					layout.Lname, layout.Llocation, index));
		}
		//玩家守角
		else {
			std::vector<int> indexs = {};
			for (int i = 0; i < patterns.size(); i++) {
				if (patterns[i].Pname == layout.Lname) {
					indexs.push_back(i);
				}
			}
			int random = rand() % indexs.size();
			layout.xys.push_back(patterns[indexs[random]].xys[1].disposeLocation(getLocation(data.layout_attribute.location)));
			return new FlushBackData(
				"flush", layout.xys, *new LayoutAttribute(
					layout.Lname, layout.Llocation, index));
		}
	}
	else if (data.command.type == "tip") {
		//获得布局信息
		int index = data.layout_attribute.index;
		Layout layout = *new Layout(layouts[index].xys, layouts[index].Lname, layouts[index].Llocation);

		//变换棋谱
		layout.Llocation = disposeLocation(getLocation(data.layout_attribute.location), layout);

		//根据棋谱走树
		Node head = &patternhead;
		//定式类型
		int type = findPatternType(head, layout.Lname);
		if (type == -1) {
			return new BackData("fail");
		}

		head = head->nexts[type];
		for (int i = layout.xys.size(); i < data.qizis.size(); i++) {
			int num = find(head->nexts,data.qizis[i].antiDisposeLocation(getLocation(data.layout_attribute.location)));
			if (num == -1) {
				return new BackData("fail");
			}
			head = head->nexts[num];
		}

		//返回提示坐标
		std::vector<XY> xys = {};
		for (int i = 0; i < head->nexts.size(); i++) {
			xys.push_back(head->nexts[i]->xy.disposeLocation(getLocation(data.layout_attribute.location)));
		}
		return new TipBackData("tip", xys);

	}
	else if (data.command.type == "luozi") {
		//获得布局信息
		int index = data.layout_attribute.index;
		Layout layout = *new Layout(layouts[index].xys, layouts[index].Lname, layouts[index].Llocation);

		//变换棋谱
		layout.Llocation = disposeLocation(getLocation(data.layout_attribute.location), layout);

		//根据棋谱走树
		Node head = &patternhead;
		//定式类型
		int type = findPatternType(head, layout.Lname);
		if (type == -1) {
			std::cout << 1;
			return new BackData("fail");
		}
		head = head->nexts[type];
		for (int i = layout.xys.size(); i < data.qizis.size(); i++) {
			int num = find(head->nexts, data.qizis[i].antiDisposeLocation(getLocation(data.layout_attribute.location)));
			if (num == -1) {
				std::cout << 2;
				return new BackData("fail");
			}
			head = head->nexts[num];
		}

		XY luozi = *new XY(data.command.x, data.command.y);

		int num = find(head->nexts, luozi.antiDisposeLocation(getLocation(data.layout_attribute.location)));
		if (num == -1) {
			std::cout << 3;
			return new BackData("fail");
		}
		head = head->nexts[num];

		//返回落子坐标
		if (head->nexts.size() == 0) {
			return new BackData("finish");
		}
		else {
			bool hasNext = false;
			std::vector<XYW> xyws = {};
			for (int i = 0; i < head->nexts.size(); i++) {
				if (head->nexts[i]->nexts.size() != 0) {
					hasNext = true;
				}
				xyws.push_back(*new XYW(head->nexts[i]->xy.disposeLocation(getLocation(data.layout_attribute.location)), head->nexts[i]->weight));
			}
			XY xy = getXY(xyws, getDegree(data.command.degree));
			if (hasNext) {
				return new LuoziBackData("luozi", xy);
			}
			else {
				return new LuoziBackData("finish", xy);
			}
		}
	}
	else {
		return new BackData("fail");
	}
}

//处理数据
bool Singleton::disposeSetterData(ReceiveDataFromSetter& data)
{
	if (data.type == "pattern") {
		//棋谱为空和棋谱只有一手均不能添加
		if (data.qizis.size() < 1) {
			std::cout << "棋谱有误，添加失败" << std::endl;
			return false;
		}
		std::string name = Singleton::getInstance().getName(data.qizis[0]);
		if (name == "error") {
			std::cout << "定式有误，添加失败" << std::endl;
			return false;
		}
		std::string location = "topleft-right";

		Singleton::getInstance().patterns.push_back(
			*new Pattern(data.qizis, name, location));

		//将更新写入文件
		JsonUtils::getInstance().writeJsonFile(
			"pattern.json", JsonUtils::getInstance().PatternsToJson(Singleton::getInstance().patterns));

		//更新数据
		Singleton::getInstance().updateTree(data.qizis);

		return true;
	}
	else if(data.type == "layout") {
		if (data.qizis.size() == 0) {
			std::cout << "棋谱有误，添加失败" << std::endl;
			return false;
		}
		std::string name = Singleton::getInstance().getName(data.qizis[0]);
		if (name == "error") {
			std::cout << "定式有误，添加失败" << std::endl;
			return false;
		}
		std::string location = "topleft-right";

		Singleton::getInstance().layouts.push_back(
			*new Layout(data.qizis, name, location));

		//将更新写入文件
		JsonUtils::getInstance().writeJsonFile(
			"layout.json", JsonUtils::getInstance().LayoutsToJson(Singleton::getInstance().layouts));

		return true;
	}
	else {
		std::cout << "格式有误，添加失败" << std::endl;
		return false;
	}
}
