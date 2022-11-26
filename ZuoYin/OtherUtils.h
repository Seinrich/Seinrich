#pragma once
#include <string>
#include <vector>
#include "Class.h"

class OtherUtils
{
private:
	OtherUtils() { };
	~OtherUtils() { };
	OtherUtils(const OtherUtils&);
	OtherUtils& operator=(const OtherUtils&);
public:
	static OtherUtils& getInstance()
	{
		static OtherUtils instance;
		return instance;
	}

	//打印定式
	void printPatterns(std::vector<Pattern>& qipus);

	//打印布局
	void printLayouts(std::vector<Layout>& qipus);

	//打印棋谱
	void printXYs(std::vector<XY>& xys);
};