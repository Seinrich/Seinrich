#include <string>
#include <json/json.h>
#include "Class.h"
#include "ReceiveData.h"

class JsonUtils
{
private:
	JsonUtils() { };
	~JsonUtils() { };
	JsonUtils(const JsonUtils&);
	JsonUtils& operator=(const JsonUtils&);
public:
	static JsonUtils& getInstance()
	{
		static JsonUtils instance;
		return instance;
	}

	//文件写入
	void writeFileFromString(const std::string& filename, const std::string& body);

	//文件读取
	std::string readFileIntoString(const char* filename);

	//FileToJson
	Json::Value readJsonFile(const std::string& filename);

	//StringToJson
	Json::Value readJsonFromString(const std::string& mystr);

	//fromJson
	void writeJsonFile(const std::string& filename, const Json::Value& root);

	//PatternsToJson
	Json::Value PatternsToJson(std::vector<Pattern>& patterns);

	//PatternsFromJson::value
	std::vector<Pattern> PatternsFromJsonValue(const Json::Value& root);

	//LayoutsToJson
	Json::Value LayoutsToJson(std::vector<Layout>& layouts);

	//LayoutsFromJson::value
	std::vector<Layout> LayoutsFromJsonValue(const Json::Value& root);

	//ReceiveDataFromJson::value
	ReceiveDataFromPlayer ReceiveDataFromPlayerFromJsonValue(const Json::Value& root);

	//ReceiveDataFromJson::value
	ReceiveDataFromSetter ReceiveDataFromSetterFromJsonValue(const Json::Value& root);

	//BackDataToString
	std::string BackDataToString(BackData* data);
};