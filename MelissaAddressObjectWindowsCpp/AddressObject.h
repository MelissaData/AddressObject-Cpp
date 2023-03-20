#include<iostream>
#include <string>

#include "mdAddr.h"
#include "DataContainer.h"

using namespace std;

#ifndef ADDRESSOBJECT_H
#define ADDRESSOBJECT_H

class AddressObject
{
public:
	// Path to Address Object data files (.dat, etc)
	string dataFilePath;

	// Create instance of Melissa Address Object
	mdAddr* mdAddressObj = new mdAddr;

	AddressObject(string license, string dataPath);

	// This will call the Lookup function to process the input address, city, state, and zip as well as generate the result codes
	void ExecuteObjectAndResultCodes(DataContainer& data);
};

#endif
