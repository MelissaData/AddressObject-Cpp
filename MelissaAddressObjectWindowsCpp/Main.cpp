#include <iostream>
#include <string>
#include <cstdlib>
#include <list>

#include "mdAddr.h"
#include "AddressObject.h"
#include "DataContainer.h"

using namespace std;

// function declarations
void ParseArguments(string& license, string& testAddress, string& testCity, string& testState, string& testZip, string& dataPath, int argc, char* argv[]);
void RunAsConsole(string license, string testAddress, string testCity, string testState, string testZip, string dataPath);
list<string> SplitResultCodes(string s, string delimiter);

int main(int argc, char* argv[])
{
	// Variables
	string license = "";
	string testAddress = "";
	string testCity = "";
	string testState = "";
	string testZip = "";
	string dataPath = "";

	ParseArguments(license, testAddress, testCity, testState, testZip, dataPath, argc, argv);
	RunAsConsole(license, testAddress, testCity, testState, testZip, dataPath);

	return 0;
}

void ParseArguments(string& license, string& testAddress, string& testCity, string& testState, string& testZip, string& dataPath, int argc, char* argv[])
{
	for (int i = 1; i < argc; i++)
	{
		if (string(argv[i]) == "--license" || string(argv[i]) == "-l")
		{
			if (argv[i + 1] != NULL)
			{
				license = argv[i + 1];
			}
		}
		if (string(argv[i]) == "--address" || string(argv[i]) == "-a")
		{
			if (argv[i + 1] != NULL)
			{
				testAddress = argv[i + 1];
			}
		}
		if (string(argv[i]) == "--city" || string(argv[i]) == "-c")
		{
			if (argv[i + 1] != NULL)
			{
				testCity = argv[i + 1];
			}
		}
		if (string(argv[i]) == "--state" || string(argv[i]) == "-s")
		{
			if (argv[i + 1] != NULL)
			{
				testState = argv[i + 1];
			}
		}
		if (string(argv[i]) == "--zip" || string(argv[i]) == "-z")
		{
			if (argv[i + 1] != NULL)
			{
				testZip = argv[i + 1];
			}
		}
		if (string(argv[i]) == "--dataPath" || string(argv[i]) == "-d")
		{
			if (argv[i + 1] != NULL)
			{
				dataPath = argv[i + 1];
			}
		}
	}
}

void RunAsConsole(string license, string testAddress, string testCity, string testState, string testZip, string dataPath)
{
	cout << "\n\n========== WELCOME TO MELISSA ADDRESS OBJECT WINDOWS C++ ===========\n" << endl;
	
	AddressObject* addressObject = new AddressObject(license, dataPath);

	bool shouldContinueRunning = true;

	while (shouldContinueRunning)
	{
		DataContainer dataContainer = DataContainer();

		if (testAddress.empty() && testCity.empty() && testState.empty() && testZip.empty())
		{
			string inputAddress;
			string inputCity;
			string inputState;
			string inputZip;

			cout << "\nFill in each value to see the Address Object results" << endl;

			cout << "Address: ";
			getline(cin, inputAddress);
			strcpy_s(dataContainer.Address, inputAddress.c_str());

			cout << "City: ";
			getline(cin, inputCity);
			strcpy_s(dataContainer.City, inputCity.c_str());

			cout << "State: ";
			getline(cin, inputState);
			strcpy_s(dataContainer.State, inputState.c_str());

			cout << "Zip: ";
			getline(cin, inputZip);
			strcpy_s(dataContainer.Zip, inputZip.c_str());
		}
		else
		{
			strcpy_s(dataContainer.Address, testAddress.c_str());
			strcpy_s(dataContainer.City, testCity.c_str());
			strcpy_s(dataContainer.State, testState.c_str());
			strcpy_s(dataContainer.Zip, testZip.c_str());
		}

		// Print user input
		cout << "\n============================== INPUTS ==============================\n" << endl;
		cout << "                  Address Line 1: " + string(dataContainer.Address) << endl;
		cout << "                            City: " + string(dataContainer.City) << endl;
		cout << "                           State: " + string(dataContainer.State) << endl;
		cout << "                             Zip: " + string(dataContainer.Zip) << endl;

		// Execute Address Object
		addressObject->ExecuteObjectAndResultCodes(dataContainer);

		// Print output
		cout << "\n============================== OUTPUT ==============================\n" << endl;
		cout << "\n\tAddress Object Information:" << endl;
		cout << "\t                     MAK: " + string(addressObject->mdAddressObj->GetMelissaAddressKey())		<< endl;
		cout << "\t          Address Line 1: " + string(addressObject->mdAddressObj->GetAddress())							<< endl;
		cout << "\t          Address Line 2: " + string(addressObject->mdAddressObj->GetAddress2())							<< endl;
		cout << "\t                    City: " + string(addressObject->mdAddressObj->GetCity())									<< endl;
		cout << "\t                   State: " + string(addressObject->mdAddressObj->GetState())								<< endl;
		cout << "\t                     Zip: " + string(addressObject->mdAddressObj->GetZip())									<< endl;
		cout << "\t            Result Codes: " + dataContainer.ResultCodes																			<< endl;

		list<string> rs = SplitResultCodes(dataContainer.ResultCodes, ",");
		list<string>::iterator it;

		for (it = rs.begin(); it != rs.end(); it++)
		{
			printf("        %s: %s", it->c_str(), addressObject->mdAddressObj->GetResultCodeDescription(it->c_str(), addressObject->mdAddressObj->ResultCodeDescriptionLong));
			cout << endl;
		}

		bool isValid = false;
		if (!(testAddress + testCity + testState + testZip).empty())
		{
			isValid = true;
			shouldContinueRunning = false;
		}
		while (!isValid)
		{
			string testAnotherResponse;

			cout << "\nTest another address? (Y/N)" << endl;
			getline(cin, testAnotherResponse);

			if (!testAnotherResponse.empty())
			{
				if (testAnotherResponse == "y" || testAnotherResponse == "Y")
				{
					isValid = true;
				}
				else if (testAnotherResponse == "n" || testAnotherResponse == "N")
				{
					isValid = true;
					shouldContinueRunning = false;
				}
				else
				{
					cout << "Invalid Response, please respond 'Y' or 'N'" << endl;
				}
			}
		}
	}
	cout << "\n============== THANK YOU FOR USING MELISSA C++ OBJECT ==============\n" << endl;
}

/// <summary>
/// This function splits the resultCodes based on a delimiter
/// </summary>
/// <param name="s">the resultCode string</param>
/// <param name="delimiter"the delimiter string></param>
/// <returns></returns>
list<string> SplitResultCodes(string s, string delimiter) {
	list<string> resultCodes;

	size_t pos = 0;
	string token;

	while ((pos = s.find(delimiter)) != string::npos) {
		token = s.substr(0, pos);
		resultCodes.push_back(token);
		s.erase(0, pos + delimiter.length());
	}

	// push back the last resultCode
	resultCodes.push_back(s);

	return resultCodes;
}