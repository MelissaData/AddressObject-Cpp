#include<iostream>

using namespace std;

#ifndef DATACONTAINER_H
#define DATACONTAINER_H

/**
 * Data holder for a single record: carries the input address in and the result codes out.
 */
class DataContainer
{
public:
	// Input: the street address to process.
	char Address[50] = "";

	// Input: the city to process.
	char City[50] = "";

	// Input: the state to process.
	char State[50] = "";

	// Input: the ZIP code to process.
	char Zip[50] = "";

	// Output: comma-separated result codes from GetResults().
	string ResultCodes;

	DataContainer() {};
};

#endif