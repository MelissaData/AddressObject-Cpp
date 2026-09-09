#include<iostream>
#include <string>

#include "mdAddr.h"
#include "DataContainer.h"

using namespace std;

#ifndef ADDRESSOBJECT_H
#define ADDRESSOBJECT_H

/**
 * Wrapper that owns a single Melissa Address Object instance and encapsulates the two
 * things every Melissa object needs: one-time setup (license + data files) and the
 * per-record processing sequence. Reuse one instance across many addresses; do NOT
 * re-initialize per address.
 */
class AddressObject
{
public:
	// Path to the Address Object data files.
	string dataFilePath;

	// The underlying Melissa Address Object instance.
	mdAddr* mdAddressObj = new mdAddr;

	/**
	 * Performs the mandatory one-time setup, in this required order:
	 *   1. SetLicenseString     - authorize the object.
	 *   2. SetPathTo*DataFiles  - tell it where each set of data files lives.
	 *   3. InitializeDataFiles  - load the data into memory.
	 *
	 * @param license  The Melissa license string used to authorize the object.
	 * @param dataPath Path to the folder containing the Address Object data files.
	 */
	AddressObject(string license, string dataPath);

	/**
	 * Runs the full Address Object processing sequence for one address and captures its
	 * result codes. This is the canonical per-record call pattern to copy into your
	 * own application:
	 *   ClearProperties -> SetAddress/SetCity/SetState/SetZip -> VerifyAddress -> GetResults
	 *
	 * @param data The record to process. Its Address/City/State/Zip are read as input, and
	 *             ResultCodes is populated with this run's result codes.
	 */
	void ExecuteObjectAndResultCodes(DataContainer& data);
};

#endif
