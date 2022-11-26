#include "StringUtils.h"

StringUtils& StringUtils::operator=(const StringUtils&)
{
	return *this;
}

//Ìæ»»×Ó´®
std::string& StringUtils::replace_all(std::string& src, const std::string& old_value, const std::string& new_value) {
	for (std::string::size_type pos(0); pos != std::string::npos; pos += new_value.length()) {
		if ((pos = src.find(old_value, pos)) != std::string::npos) {
			src.replace(pos, old_value.length(), new_value);
		}
		else break;
	}
	return src;
}

//½ØÈ¡×Ó´®
std::string StringUtils::midstr(std::string& oldstr, std::string startstr, std::string endstr) {
	std::string res = "";
	int i = oldstr.find(startstr) + 2;
	int j = oldstr.find(endstr) - 7;
	return oldstr.substr(i, j);
}