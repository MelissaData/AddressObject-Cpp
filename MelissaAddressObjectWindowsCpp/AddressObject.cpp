#include "AddressObject.h"

AddressObject::AddressObject(string license, string dataPath)
{
	// Set license string and set path to datafiles (.dat, etc)
	mdAddressObj->SetLicenseString(license.c_str());
	dataFilePath = dataPath;

	mdAddressObj->SetPathToUSFiles(dataFilePath.c_str());
	mdAddressObj->SetPathToAddrKeyDataFiles(dataFilePath.c_str());
	mdAddressObj->SetPathToDPVDataFiles(dataFilePath.c_str());
	mdAddressObj->SetPathToLACSLinkDataFiles(dataFilePath.c_str());
	mdAddressObj->SetPathToRBDIFiles(dataFilePath.c_str());
	mdAddressObj->SetPathToSuiteFinderDataFiles(dataFilePath.c_str());
	mdAddressObj->SetPathToSuiteLinkDataFiles(dataFilePath.c_str());

	// If you see a different date than expected, check your license string and either download the new data files or use the Melissa Updater program to update your data files.  
	mdAddr::ProgramStatus pStatus = mdAddressObj->InitializeDataFiles();

	if (pStatus != mdAddr::ProgramStatus::ErrorNone)
	{
		cout << "Failed to Initialize Object." << endl;
		cout << pStatus << endl;
		return;
	}

	cout << "                   DataBase Date: " + string(mdAddressObj->GetDatabaseDate()) << endl;
	cout << "                 Expiration Date: " + string(mdAddressObj->GetLicenseExpirationDate()) << endl;

	/**
	 * This number should match with the file properties of the Melissa Object binary file.
	 * If TEST appears with the build number, there may be a license key issue.
	 */
	cout << "                  Object Version: " + string(mdAddressObj->GetBuildNumber()) << endl;
}

// This will call the Lookup function to process the input address, city, state, and zip as well as generate the result codes
void AddressObject::ExecuteObjectAndResultCodes(DataContainer& data)
{
	mdAddressObj->ClearProperties();

	mdAddressObj->SetAddress(data.Address);
	mdAddressObj->SetCity(data.City);
	mdAddressObj->SetState(data.State);
	mdAddressObj->SetZip(data.Zip);

	mdAddressObj->VerifyAddress();
	data.ResultCodes = mdAddressObj->GetResults();

	// ResultsCodes explain any issues Address Object has with the object.
	// List of result codes for Address Object
	// https://wiki.melissadata.com/?title=Result_Code_Details#Address_Object
}
