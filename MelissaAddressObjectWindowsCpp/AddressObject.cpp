#include "AddressObject.h"

AddressObject::AddressObject(string license, string dataPath)
{
	// Set license string and set path to data files
	mdAddressObj->SetLicenseString(license.c_str());
	dataFilePath = dataPath;

	// Address Object draws on several USPS data sets; point each one at the data folder.
	mdAddressObj->SetPathToUSFiles(dataFilePath.c_str());
	mdAddressObj->SetPathToAddrKeyDataFiles(dataFilePath.c_str());
	mdAddressObj->SetPathToDPVDataFiles(dataFilePath.c_str());
	mdAddressObj->SetPathToLACSLinkDataFiles(dataFilePath.c_str());
	mdAddressObj->SetPathToRBDIFiles(dataFilePath.c_str());
	mdAddressObj->SetPathToSuiteFinderDataFiles(dataFilePath.c_str());
	mdAddressObj->SetPathToSuiteLinkDataFiles(dataFilePath.c_str());

	// Load the data files. The returned ProgramStatus reports whether initialization succeeded.
	// If you see a different date than expected, check your license string and either download the new data files
	// or use the Melissa Updater program to update your data files.
	mdAddr::ProgramStatus pStatus = mdAddressObj->InitializeDataFiles();

	// If an issue occurred, please investigate the common causes.
	// Common causes: an invalid/expired license, or missing/wrong-path data files.
	if (pStatus != mdAddr::ProgramStatus::ErrorNone)
	{
		cout << "Failed to Initialize Object." << endl;
		cout << pStatus << endl;
		return;
	}

	// Diagnostic information, handy for confirming the object loaded the data you expect:

	// Build date of the data files
	cout << "                   DataBase Date: " + string(mdAddressObj->GetDatabaseDate()) << endl;

	// When the license stops working
	cout << "                 Expiration Date: " + string(mdAddressObj->GetLicenseExpirationDate()) << endl;

	/**
	 * This number should match with the file properties of the Melissa Object binary file.
	 * If TEST appears with the build number, there may be a license key issue.
	 */
	cout << "                  Object Version: " + string(mdAddressObj->GetBuildNumber()) << endl;
}

// This will call the functions to process the input address as well as generate the result codes
void AddressObject::ExecuteObjectAndResultCodes(DataContainer& data)
{
	// Reset any state left over from a previous address. Important when reusing the same
	// object across multiple records so fields from a prior address don't bleed into this one.
	mdAddressObj->ClearProperties();

	// Supply the raw input fields to process
	mdAddressObj->SetAddress(data.Address);
	mdAddressObj->SetCity(data.City);
	mdAddressObj->SetState(data.State);
	mdAddressObj->SetZip(data.Zip);

	// Validate, standardize, and correct the address
	mdAddressObj->VerifyAddress();

	// Collect the result codes for this run
	// ResultsCodes explain any issues Address Object has with the object.
	// List of result codes for Address Object
	// https://docs.melissa.com/on-premise-api/address-object/result-codes.html
	data.ResultCodes = mdAddressObj->GetResults();
}
