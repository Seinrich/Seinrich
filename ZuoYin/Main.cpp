#include "Server.h"
#include "JsonUtils.h"
#include "OtherUtils.h"
#include <iostream>
#include "Class.h"
#include "TreeNode.h"
#include "Singleton.h"
#include "StringUtils.h"

int main() {
	
	/*
	//测试数据结构
	srand(time(NULL));
	std::vector<Pattern> patterns = {};
	std::vector<Layout> layouts = {};
	for (int i = 0; i < 10; i++) {
		std::vector<XY> xys = {};
		xys.push_back(*new XY(rand() % 3 + 2, rand() % 2 + 2));
		for (int j = 0; j < 10; j++) {
			xys.push_back(*new XY(rand() % 19, rand() % 19));
		}
		if (StringUtils::getInstance().getName(xys[0]) != "error") {
			patterns.push_back(*new Pattern(xys, StringUtils::getInstance().getName(xys[0]), "topleft-right"));
			layouts.push_back(*new Layout(xys, StringUtils::getInstance().getName(xys[0]), "topleft-right"));
		}
	}

	//写入功能
	JsonUtils::getInstance().writeJsonFile(
		"pattern.json", JsonUtils::getInstance().PatternsToJson(patterns));
	JsonUtils::getInstance().writeJsonFile(
		"layout.json", JsonUtils::getInstance().LayoutsToJson(layouts));
	*/
	
	//读取文件
	Singleton::getInstance().patterns = JsonUtils::getInstance().PatternsFromJsonValue(
		JsonUtils::getInstance().readJsonFile("pattern.json"));
	Singleton::getInstance().layouts = JsonUtils::getInstance().LayoutsFromJsonValue(
		JsonUtils::getInstance().readJsonFile("layout.json"));
	Singleton::getInstance().buildPatternTree();

	//打印
	std::cout << "patternTree 定式树" << std::endl;
	printTreeNode(&Singleton::getInstance().patternhead, "");
	std::cout << "\npatterns 定式数组" << std::endl;
	OtherUtils::getInstance().printPatterns(Singleton::getInstance().patterns);
	std::cout << "\nlayouts 布局数组" << std::endl;
	OtherUtils::getInstance().printLayouts(Singleton::getInstance().layouts);

	//创建服务器
	Server* server = new Server("localhost", 8088);
	//启动服务器
	server->startListen();
	return 0;
}