#include "OtherUtils.h"
#include <iostream>

OtherUtils& OtherUtils::operator=(const OtherUtils&)
{
	return *this;
}

void OtherUtils::printPatterns(std::vector<Pattern>& qipus)
{
	for (int i = 0; i < qipus.size(); i++) {
		std::cout << qipus[i].Pname << " " << qipus[i].Plocation << " " << std::ends;
		printXYs(qipus[i].xys);
	}
}

void OtherUtils::printLayouts(std::vector<Layout>& qipus)
{
	for (int i = 0; i < qipus.size(); i++) {
		std::cout << qipus[i].Lname << " " << qipus[i].Llocation << " " << std::ends;
		printXYs(qipus[i].xys);
	}
}

void OtherUtils::printXYs(std::vector<XY>& xys)
{
	for (int i = 0; i < xys.size(); i++) {
		std::cout << "(" + xys[i].toString() + ") " << std::ends;
	}
	std::cout << std::endl;
}


