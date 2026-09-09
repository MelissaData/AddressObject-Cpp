/**
 * @file Main.cpp
 * @brief Address Object corrects, verifies and enhances U.S. and Canadian addresses. Use Address
 * Object to remove bad or incomplete information before it invades your database and creates
 * a negative impact on your data-driven initiatives. You'll reduce undeliverables, increase
 * communication efforts, and save money on all your marketing campaigns.
 *
 * High-level flow of this sample:
 *   1. SETUP     - create an mdAddr instance, hand it the license string and the
 *                  paths to the data files, then InitializeDataFiles() (one time).
 *   2. INPUT     - feed an address in with SetAddress/SetCity/SetState/SetZip.
 *   3. PROCESS   - VerifyAddress() validates, standardizes, and corrects the address.
 *   4. READ      - pull the corrected fields back out with the Get* getters
 *                  (GetAddress, GetCity, GetState, GetZip, GetMelissaAddressKey, ...).
 *   5. INTERPRET - GetResults() returns comma-separated result codes describing
 *                  what the object did/found; each code has a human description.
 *
 * The pieces of this sample map onto that flow:
 *   - main / ParseArguments / RunAsConsole : console harness (argument parsing + the interactive loop).
 *   - AddressObject   : thin wrapper around mdAddr that owns setup + the call sequence.
 *   - DataContainer   : plain holder for one record's input and output.
 *
 * Where mdAddr comes from:
 *   There is no generated wrapper source for C++. mdAddr.h and
 *   mdEnums.h declare the API, mdAddr.lib is the import library the linker resolves
 *   against, and mdAddr.dll carries the implementation. The accompanying
 *   MelissaAddressObjectWindowsCpp.ps1 script downloads all four on every run.
 *
 * Reference:
 *   Quickstart    : https://docs.melissa.com/on-premise-api/address-object/address-object-quickstart.html
 *   Release notes : https://releasenotes.melissa.com/on-premise-api/address-object/
 *   Result codes  : https://docs.melissa.com/on-premise-api/address-object/result-codes.html
 */

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

/**
 * Entry point. Reads the optional command-line arguments, then hands control to
 * RunAsConsole, which performs the actual Address Object setup and processing.
 *
 * @param argc The count of command-line arguments.
 * @param argv The raw command-line arguments.
 */
int main(int argc, char* argv[])
{
	// Populated by ParseArguments below.
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

/**
 * Reads the supported command-line options into the reference parameters.
 *
 * Recognized flags (each followed by its value, e.g. "--address 22382 Avenida Empresa"):
 *   --license / -l   : the Melissa license string
 *   --dataPath / -d  : path to the Address Object data files
 *   --address / -a   : street address to test in one-shot mode
 *   --city / -c      : city to test in one-shot mode
 *   --state / -s     : state to test in one-shot mode
 *   --zip / -z       : ZIP code to test in one-shot mode
 *
 * @param license     Receives the Melissa license string.
 * @param testAddress Receives the street address to test in one-shot mode.
 * @param testCity    Receives the city to test in one-shot mode.
 * @param testState   Receives the state to test in one-shot mode.
 * @param testZip     Receives the ZIP code to test in one-shot mode.
 * @param dataPath    Receives the path to the Address Object data files.
 * @param argc        The count of command-line arguments to parse.
 * @param argv        The raw command-line arguments to parse.
 */
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

/**
 * Sets up the Address Object once, then drives the input -> process -> output cycle.
 *
 * In interactive mode (no address args) it loops, asking for a new address each pass
 * until the user answers "N". In one-shot mode (address args supplied) it runs a
 * single pass and exits.
 *
 * @param license     The Melissa license string used to initialize the object.
 * @param testAddress A street address to process in one-shot mode; if empty, the program prompts interactively.
 * @param testCity    A city to process in one-shot mode.
 * @param testState   A state to process in one-shot mode.
 * @param testZip     A ZIP code to process in one-shot mode.
 * @param dataPath    Path to the Address Object data files.
 */
void RunAsConsole(string license, string testAddress, string testCity, string testState, string testZip, string dataPath)
{
	cout << "\n\n========== WELCOME TO MELISSA ADDRESS OBJECT WINDOWS C++ ===========\n" << endl;
	
	// Construct the wrapper. This is where the object is licensed, pointed at the
	// data files, and initialized (see the AddressObject constructor below).
	AddressObject* addressObject = new AddressObject(license, dataPath);

	bool shouldContinueRunning = true;

	while (shouldContinueRunning)
	{
		// Holder for this pass's input and result codes.
		DataContainer dataContainer = DataContainer();

		if (testAddress.empty() && testCity.empty() && testState.empty() && testZip.empty())
		{
			// Interactive mode: prompt the user for each address component.
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
			// One-shot mode: use the address passed on the command line.
			strcpy_s(dataContainer.Address, testAddress.c_str());
			strcpy_s(dataContainer.City, testCity.c_str());
			strcpy_s(dataContainer.State, testState.c_str());
			strcpy_s(dataContainer.Zip, testZip.c_str());
		}

		// Print user input
		cout << "\n============================== INPUTS ==============================\n" << endl;
		cout << "                         Address: " + string(dataContainer.Address) << endl;
		cout << "                            City: " + string(dataContainer.City) << endl;
		cout << "                           State: " + string(dataContainer.State) << endl;
		cout << "                             Zip: " + string(dataContainer.Zip) << endl;

		// Execute Address Object
		// Runs the verify sequence and stores the result codes on dataContainer
		addressObject->ExecuteObjectAndResultCodes(dataContainer);

		// Print output
		// Each Get* getter below returns one component the object produced for the most
		// recently processed address. These read directly from the mdAddr instance, which
		// still holds the results from the Execute call above.
		cout << "\n============================== OUTPUT ==============================\n" << endl;
		cout << "\n\tAddress Object Information:" << endl;
		cout << "\t                     MAK: " + string(addressObject->mdAddressObj->GetMelissaAddressKey())		<< endl;
		cout << "\t          Address Line 1: " + string(addressObject->mdAddressObj->GetAddress())							<< endl;
		cout << "\t          Address Line 2: " + string(addressObject->mdAddressObj->GetAddress2())							<< endl;
		cout << "\t                    City: " + string(addressObject->mdAddressObj->GetCity())									<< endl;
		cout << "\t                   State: " + string(addressObject->mdAddressObj->GetState())								<< endl;
		cout << "\t                     Zip: " + string(addressObject->mdAddressObj->GetZip())									<< endl;
		cout << "\t            Result Codes: " + dataContainer.ResultCodes																			<< endl;

		// Result codes come back as a single comma-separated string (e.g. "AS01,AC01").
		// Split it and ask the object for a readable description of each code.
		// ResultCodeDescriptionLong requests the long-form text; a short form is also
		// available via ResultCodeDescriptionShort.
		list<string> rs = SplitResultCodes(dataContainer.ResultCodes, ",");
		list<string>::iterator it;

		for (it = rs.begin(); it != rs.end(); it++)
		{
			printf("        %s: %s", it->c_str(), addressObject->mdAddressObj->GetResultCodeDescription(it->c_str(), addressObject->mdAddressObj->ResultCodeDescriptionLong));
			cout << endl;
		}

		bool isValid = false;

		// In one-shot mode there is nothing more to do after a single pass: mark the
		// input handled and stop the outer loop.
		if (!(testAddress + testCity + testState + testZip).empty())
		{
			isValid = true;
			shouldContinueRunning = false;
		}

		// Interactive mode: ask whether to process another address. Keep prompting until
		// we get a valid Y/N. "N" ends the program; "Y" falls through to another pass.
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

/**
 * Splits the comma-separated result-code string into individual codes.
 *
 * @param s         The result-code string (e.g. "AS01,AC01").
 * @param delimiter The delimiter string to split on.
 * @return A list holding each individual result code.
 */
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