#include <stdio.h>
#include <stdlib.h>
#include<iostream>
#include<string>
#include<cstring>
#include<WS2tcpip.h>
#include <WinSock2.h>
#include<Windows.h>
#pragma comment(lib, "ws2_32.lib")
#include "Server.h"
#include "StringUtils.h"
#include "JsonUtils.h"
#include "Singleton.h"

Server::Server(const char ip[10], int port) {
	std::cout << "\n-----------服务器-----------" << std::endl;
	initWSADATA();
	initSocket(ip, port);
	initListen();
}

//终止服务器
Server::~Server() {
	closesocket(serviceSocket);
	WSACleanup();
	std::cout << "服务器停止" << std::endl;
	std::cin.get();
}

//开始监听
void Server::startListen() {
	while (true) {
		receive();
		feedback();
		closesocket(recvClientSocket);
	}
}

//接受请求
void Server::receive() {
	sockaddr_in revClientAddr{};

	//初始化客户端socket
	recvClientSocket = INVALID_SOCKET;
	int _revSize = sizeof(sockaddr_in);
	recvClientSocket = accept(serviceSocket, (SOCKADDR*)&revClientAddr, &_revSize);

	//判断接收是否成功
	if (INVALID_SOCKET == recvClientSocket) {
		std::cout << "服务端接受请求失败!" << std::endl;
	}
	else {
		std::cout << "服务端接受请求成功!" << std::endl;
	}
}

//反馈数据
void Server::feedback() {
	//接收数据
	int reLen = recv(recvClientSocket, recvBuf, 1024, 0);

	//处理数据
	std::string receiveText = recvBuf;
	receiveText = StringUtils::getInstance().midstr(receiveText, "/?", "HTTP");
	StringUtils::getInstance().replace_all(receiveText, "%22", "\"");

	//打印数据
	std::cout << "服务器接收数据:\n" << receiveText << std::endl;

	//处理数据
	Json::Value root = JsonUtils::getInstance().readJsonFromString(receiveText);

	//返回数据
	BackData* backdata = nullptr;
	std::string content;

	if (!root.isMember("command")) {
		ReceiveDataFromSetter data = JsonUtils::getInstance().ReceiveDataFromSetterFromJsonValue(root);
		bool success = Singleton::getInstance().disposeSetterData(data);
		content = std::to_string(success);
		content = (content == "1" ? "true" : "false");
	}
	else {
		ReceiveDataFromPlayer data = JsonUtils::getInstance().ReceiveDataFromPlayerFromJsonValue(root);
		backdata = Singleton::getInstance().disposePlayerData(data);
		content = JsonUtils::getInstance().BackDataToString(backdata);
	}

	//生成发送数据
	std::string sendText = "";
	sendText += "http/1.1 200 ok\r\n";
	sendText += "Access-Control-Allow-Origin:*\r\n";
	sendText += "Access-Control-Allow-Headers:*\r\n";
	sendText += "Content-Type: text/html;charset=ANSI\r\n";
	sendText += "Content-Length:";
	sendText += std::to_string(content.size());
	sendText += "\r\n\r\n";
	sendText += content;

	//发送数据
	int sLen = send(recvClientSocket, sendText.c_str(), sendText.size(), 0);

	//判断发送是否成功
	if (SOCKET_ERROR == reLen) {
		std::cout << "服务端发送数据失败" << std::endl;
	}
	else {
		std::cout << "服务器发送数据:\n" << content << std::endl;
	}
	std::cout << std::endl;
}

void Server::initWSADATA() {
	//初始化
	WSADATA wsadata;
	//make word
	int err = WSAStartup(MAKEWORD(2, 2), &wsadata);
	if (err != 0) {
		printf("WSAStartup failed with error: %d\n", err);
		return;
	}
}

void Server::initSocket(const char ip[10], int port) {
	//socket(协议族,socket数据传输方式,某个协议)
	serviceSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (SOCKET_ERROR == serviceSocket) {
		std::cout << "套接字闯创建失败!" << std::endl;
	}
	else {
		std::cout << "套接字创建成功!" << std::endl;
	}

	//绑定套接字 指定绑定的IP地址和端口号
	//
	//一个绑定地址:有IP地址,有端口号,有协议族
	sockaddr_in socketAddr{};
	socketAddr.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &socketAddr.sin_addr.S_un.S_addr);
	socketAddr.sin_port = htons(port);

	//强制类型转换
	int bRes = bind(serviceSocket, (SOCKADDR*)&socketAddr, sizeof(SOCKADDR));

	if (SOCKET_ERROR == bRes) {
		std::cout << "绑定失败!" << std::endl;
	}
	else {
		std::cout << "绑定成功!" << std::endl;
	}
}

void Server::initListen() {
	//服务器监听
	//
	//SOMAXCONN 并发编程存放客户端请求的数量
	int lLen = listen(serviceSocket, SOMAXCONN);
	if (SOCKET_ERROR == lLen) {
		std::cout << "监听失败!" << std::endl;
	}
	else {
		std::cout << "监听成功!" << std::endl << std::endl;
	}
}