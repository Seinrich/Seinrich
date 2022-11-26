#pragma once
#include <vector>
#include <string>

//坐标
class XY {
public:
	int x;
	int y;
	XY(int x, int y);
	XY();
	bool equals(XY& xy);
	std::string toString();

	//返回变换后坐标
	XY disposeLocation(int location);
	//返回变换前坐标
	XY antiDisposeLocation(int location);
	//沿正对角线翻转
	void turnOverFront();
	//沿反对角线翻转
	void turnOverBack();
	//旋转
	void rotate();
	//逆正对角线翻转
	void antiTurnOverFront();
	//逆反对角线翻转
	void antiTurnOverBack();
	//逆旋转
	void antiRotate();
};

//坐标+权重
class XYW {
public:
	XY xy;
	int weight;
	XYW(XY xy, int weight);
	XYW();
};

//指令
class Command {
public:
	std::string type;
	int x;
	int y;
	std::string degree;
	Command(std::string& type, int x, int y, std::string& degree);
	Command();
};

//棋谱
class Qipu {
public:
	std::vector<XY> xys;
	Qipu(std::vector<XY>& xys);
	Qipu();
	//正对角线翻转
	void turnOverFront();
	//反对角线翻转
	void turnOverBack();
	//旋转
	void rotate();
};

//布局
class Layout : public Qipu {
public:
	std::string Lname;
	std::string Llocation;
	Layout(std::vector<XY>& xys, const std::string& name, const std::string& location);
	Layout();
};

//布局属性
class LayoutAttribute {
public:
	std::string name;
	std::string location;
	int index;
	LayoutAttribute(const std::string& name, const std::string& location, int index);
	LayoutAttribute();
};

//定式
class Pattern : public Qipu {
public:
	std::string Pname;
	std::string Plocation;
	Pattern(std::vector<XY>& xys, const std::string& name, const std::string& location);
	Pattern();
};

//棋盘
class Qipan {
public:
	std::vector<int> nums;
	int qipan[19][19];
	int opposite;
	Qipan(int(&qipan)[19][19]);
};