#include "TreeNode.h"
#include <vector>
#include <iostream>
#include <exception>  

//初始化头结点
void initHead(Node head) {
	head->weight = 0;
	head->xy = *new XY(-1, -1);
	head->nexts = {};
}

//打印树
void printTreeNode(Node head, std::string info) {
	if (head->nexts.size() == 0) {
		std::cout << info << std::endl;
		return;
	}
	else {
		for (int i = 0; i < head->nexts.size(); i++) {
			std::string info2 = info;
			info2 += "(";
			info2 += head->nexts[i]->xy.toString();
			info2 += ",";
			info2 += std::to_string(head->nexts[i]->weight);
			info2 += ") ";
			printTreeNode(head->nexts[i], info2);
		}
	}
}

//判断has
int find(std::vector<struct node*>& nexts, XY xy) {
	std::cout << xy.toString() << std::endl;
	for (int i = 0; i < nexts.size(); i++) {
		if (nexts[i]->xy.equals(xy)) {
			return i;
		}
	}
	return -1;
}

//添加到树
void AddToTree(Node head, std::vector <XY>& data) {
	int i = 0, index = 0;
	for (; i < data.size(); i++) {
		if ((index = find(head->nexts, data[i])) != -1) {
			head = head->nexts[index];
		}
		else {
			break;
		}
	}
	for (; i < data.size(); i++) {
		try{
		//创建新结点
		Node newNode = new node;
			newNode->weight = 0;
			newNode->xy = *new XY(data[i].x, data[i].y);
			newNode->nexts = {};
			head->nexts.push_back(newNode);
			head = head->nexts[head->nexts.size()-1];
		}
		catch(std::exception& e) {
			std::cout << e.what() << std::endl;
		}
	}
}

//设置权重(最多步数*10+变化数*5)
int setWeight(Node head, int depth) {
	int max = depth;
	if (head->nexts.size() == 0) {
		head->weight = 0;
	}
	else {
		for (int i = 0; i < head->nexts.size(); i++) {
			max = (int)fmax(max, setWeight(head->nexts[i], depth + 1));
		}
		head->weight = (static_cast<unsigned long long>(max) - depth) * 10 + head->nexts.size() * 5;
	}
	return max;
}