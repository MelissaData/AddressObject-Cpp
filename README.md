# Melissa - Address Object Windows C++

## Purpose
This code showcases the Melissa Address Object using C++.

Please feel free to copy or embed this code to your own project. Happy coding!

For the latest Melissa Address Object release notes, please visit: https://releasenotes.melissa.com/on-premise-api/address-object/

For further details, please visit: https://docs.melissa.com/on-premise-api/address-object/address-object-quickstart.html

The console will ask the user for:

- Address
- City
- State
- Zip

And return 

- Melissa Address Key (MAK)
- Address Line 1
- Address Line 2
- City
- State
- Zip
- Result Codes

## Tested Environments
- Windows 10 64-bit Microsoft Visual C++ 14.34, Powershell 5.1
- Melissa data files for 2025-08
- Nmake 14.34
- Visual Studio 2022 Developer Command Prompt v17.4.2 64-bit

## Required File(s) and Programs

#### mdAddr.dll
This is the c++ code of the Melissa Object

#### Data File(s)
- Addr.dbf
- Congress.csv
- dph256.dte
- dph256.hsa
- dph256.hsc
- dph256.hsf
- dph256.hsn
- dph256.hsv
- dph256.hsx
- ews.txt
- lcd256
- mdAddr.dat
- mdAddr.lic
- mdAddr.nat
- mdAddr.str
- mdAddrKey.db
- mdAddrKeyCA.db
- mdCanada3.db
- mdCanadaPOC.db
- mdLACS256.dat
- mdRBDI.dat
- mdSteLink256.dat
- mdSuiteFinder.db
- month256.dat

#### Dependencies
- mdEnums.h
- mdAddr.h
- mdAddr.lib

## Getting Started
These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

#### Visual Studio Developer Command Prompt
It is important to note that you must be able to initialize the Visual Studio Developer Command Prompt environment for `x86_x64` in order to test the Melissa Address Object. The Visual Studio Developer Command Prompt should already be downloaded if you have Microsoft Visual Studio installed. 

To check if you are able to intialize the Visual Studio Developer Command Prompt for `x86_x64`, you can open the start menu and search for `x86_x64 Cross Tools Command Prompt for VS 2022`. If this program exists, then you may continue to the next steps.

Alternatively, you can check to see if the following filepath exists: `C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat` (with Visual Studio 2022 installed). If the filepath exists, then you may continue to the next steps.

#### Set up Powershell settings
If running Powershell for the first time, you will need to run this command in the Powershell console: `Set-ExecutionPolicy RemoteSigned`.
The console will then prompt you with the following warning shown in the image below. 
 - Enter `'A'`. 
 	- If successful, the console will not output any messages. (You may need to run Powershell as administrator to enforce this setting).
	
 ![alt text](/screenshots/powershell_executionpolicy.png)

#### Download this project
```
git clone https://github.com/MelissaData/AddressObject-Cpp
cd AddressObject-Cpp
```

#### Set up Melissa Updater
Melissa Updater is a CLI application allowing the user to update their Melissa applications/data.
- Download Melissa Updater here: <https://releases.melissadata.net/Download/Library/WINDOWS/NET/ANY/latest/MelissaUpdater.exe>
- Create a folder within the cloned repo called `MelissaUpdater`.
- Put `MelissaUpdater.exe` in the `MelissaUpdater` folder you've just created.

#### Different ways to get data file(s)
1. Using Melissa Updater
    - It will handle all of the data download/path and dll(s) for you.
2. If you already have the latest DQS release zip, you can find the data file(s) in there
    - To pass in your own data file path directory, you may either use the '-dataPath' parameter or enter the data file path directly in interactive mode.
    - Comment out this line "DownloadDataFiles -license $License" in the powershell script.
    - This will prevent you from having to redownload all the files.


## Run Powershell Script
Parameters:
- -address: a test street address (house number & street name)
- -city (optional): a test city
- -state (optional): a test state
- -zip (optional): a test zip code

    These are convenient when you want to get results for a specific address in one run instead of testing multiple addresses in interactive mode.

- -dataPath (optional): a data file path directory to test the Address Object
- -license (optional): a license string to test the Address Object 
- -quiet (optional): add to command if you do not want to get any console output from the Melissa Updater

When you have modified the script to match your data location, let's run the script.
There are two modes:
- Interactive
    
    The script will prompt the user for an address, city, state, and zip, then use the provided inputs to test Address Object. For example:
    ```
    .\MelissaAddressObjectWindowsCpp.ps1
    ```
    For quiet mode:
    ```
    .\MelissaAddressObjectWindowsCpp.ps1 -quiet
    ```

- Command Line

    You can pass an address, city, state, zip, and a license string into the `-address`, `-city`, `-state`, `-zip`, and `-license` parameters respectively to test Address Object. For example:
    With all parameters:
    ```
    .\MelissaAddressObjectWindowsCpp.ps1 -address "22382 Avenida Empresa" -city "Rancho Santa Margarita" -state "CA" -zip "92688"
    .\MelissaAddressObjectWindowsCpp.ps1 -address "22382 Avenida Empresa" -city "Rancho Santa Margarita" -state "CA" -zip "92688" -license "<your_license_string>"
    ```

    With any known (optional) parameters:
    ```
    .\MelissaAddressObjectWindowsCpp.ps1 -address "22382 Avenida Empresa" -state "CA" 
    .\MelissaAddressObjectWindowsCpp.ps1 -address "22382 Avenida Empresa" -state "CA" -license "<your_license_string>"
    ```

    For quiet mode:
    ```
    .\MelissaAddressObjectWindowsCpp.ps1 -address "22382 Avenida Empresa" -city "Rancho Santa Margarita" -state "CA" -zip "92688" -quiet
    .\MelissaAddressObjectWindowsCpp.ps1 -address "22382 Avenida Empresa" -city "Rancho Santa Margarita" -state "CA" -zip "92688" -license "<your_license_string>" -quiet

This is the expected outcome of a successful setup for interactive mode:

![alt text](/screenshots/output.png)

## Troubleshooting
Troubleshooting for errors found while running your program.

### C# Errors:
| Error      | Description |
| ----------- | ----------- |
| ErrorRequiredFileNotFound      | Program is missing a required file. Please check your Data folder and refer to the list of required files above. If you are unable to obtain all required files through the Melissa Updater, please contact technical support below. |
| ErrorLicenseExpired   | Expired license string. Please contact technical support below. |

## Contact Us
For free technical support, please call us at 800-MELISSA ext. 4 (800-635-4772 ext. 4) or email us at tech@melissa.com.

To purchase this product, contact the Melissa sales department at 800-MELISSA ext. 3 (800-635-4772 ext. 3).
