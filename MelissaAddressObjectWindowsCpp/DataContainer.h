#include<iostream>

using namespace std;

#ifndef DATACONTAINER_H
#define DATACONTAINER_H

class DataContainer
{
public:
	char Address[50] = "";
	char City[50] = "";
	char State[50] = "";
	char Zip[50] = "";
	string ResultCodes;

	DataContainer() {};
};

#endif