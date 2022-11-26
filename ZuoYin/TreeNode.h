#pragma once
#include "Class.h"

//多叉树节点
struct node {
	int weight = 0;
	XY xy;
	std::vector<struct node*> nexts;
};

typedef struct node* Node;

//初始化头结点
void initHead(Node head);

//打印
void printTreeNode(Node head, std::string info);

//查找
int find(std::vector<struct node*>& nexts, XY xy);

//添加
void AddToTree(Node head, std::vector<XY>& data);

//设置权重
int setWeight(Node head, int count);