#pragma once
#include "TreeNode.h"
#include "FeedbackData.h"
#include "ReceiveData.h"
//单例模式树
class Singleton
{
private:
	Singleton() { };
	~Singleton() { };
	Singleton(const Singleton&);
	Singleton& operator=(const Singleton&);
public:

	//树头结点
	struct node patternhead;
	//布局数组
	std::vector<Layout> layouts;
	//定式数组
	std::vector<Pattern> patterns;

	static Singleton& getInstance()
	{
		static Singleton instance;
		return instance;
	}

	//构建定式树
	void buildPatternTree();

	//更新定式树数据
	void updateTree(std::vector<XY>& xys);

	//翻转/旋转棋谱
	std::string disposeLocation(int location, Qipu& qipu);

	//返回位置
	int getLocation(std::string& location);

	//返回name
	std::string getName(XY xy);

	//返回难度指数
	int getDegree(std::string& degree);

	//返回落子坐标
	XY getXY(std::vector<XYW>& xyws, int degree);

	//处理数据
	BackData* disposePlayerData(ReceiveDataFromPlayer& data);

	//处理数据
	bool disposeSetterData(ReceiveDataFromSetter& data);
};