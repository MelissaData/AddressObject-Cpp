<#
.SYNOPSIS
    Downloads the required components and then builds and runs MelissaAddressObjectWindowsCpp

.DESCRIPTION
    This script uses the Melissa Updater to fetch the data file(s), the DLL, the C++ headers,
    and the import library, verifies the DLL, headers, and import library arrived, then builds
    inside the MSVC build environment and runs it against the supplied address.

    Overall flow:
      1. Read parameters / prompt for the license and data path.
      2. Download the data file(s), DLL, headers, and import library via the Melissa Updater.
      3. Confirm the DLL, headers, and import library are present (data files are not checked).
      4. Build with nmake (via BuildProgram.ps1) and run (single test address or interactive).

.PARAMETER address
    Street address to verify.

.PARAMETER city
    City for the address to verify.

.PARAMETER state
    State/province for the address to verify.

.PARAMETER zip
    ZIP/postal code for the address to verify.

.PARAMETER dataPath
    Path to an existing data files directory. If omitted, the script prompts for
    a path; pressing Enter at that prompt skips it and downloads the data files
    into the project's Data folder via the Melissa Updater. A path that does not
    exist aborts the script.

.PARAMETER license
    License string. Resolved in this order:
      1. This parameter.
      2. An interactive prompt, if the parameter was not supplied.
      3. The MD_LICENSE environment variable, if the prompt was left blank.
    Note that the environment variable is the last resort, not the first: running
    without -license always prompts, even when MD_LICENSE is set.

.PARAMETER quiet
    Suppresses the Melissa Updater console output during the DLL, header, and import
    library downloads. The data file download is not affected.

.EXAMPLE
    .\MelissaAddressObjectWindowsCpp.ps1 -license "your-license"

.EXAMPLE
    .\MelissaAddressObjectWindowsCpp.ps1 -address "22382 Avenida Empresa" -city "Rancho Santa Margarita" -state "CA" -zip "92688" -license "your-license"
#>

######################### Parameters ##########################

param(
    $address = '""',
    $city = '""',
    $state = '""',
    $zip = '""',
    $dataPath = '',
    $license = '',
    [switch]$quiet = $false
    )

######################### Classes ##########################

# Describes a single file to request from the Melissa Updater
class DLLConfig {
  [string] $FileName;
  [string] $ReleaseVersion;
  [string] $OS;
  [string] $Compiler;
  [string] $Architecture;
  [string] $Type;
}

######################### Config ###########################

# Product release the updater pulls files for
$RELEASE_VERSION = '2026.09'
$ProductName = "DQ_ADDR_DATA"

# Uses the location of the .ps1 file 
$CurrentPath = $PSScriptRoot
Set-Location $CurrentPath
$ProjectPath = "$CurrentPath\MelissaAddressObjectWindowsCpp"

# Configure the path to vcvarsall.bat if needed
$CmdPath = "C:\Program Files\Microsoft Visual Studio\2022\Professional\VC\Auxiliary\Build\vcvarsall.bat"

$BuildPath = "$ProjectPath\Build"
if (!(Test-Path $BuildPath)) {
  New-Item -Path $ProjectPath -Name 'Build' -ItemType "directory"
}

if ([string]::IsNullOrEmpty($dataPath)) {
  $DataPath = "$ProjectPath\Data" 
}

if (!(Test-Path $DataPath) -and ($DataPath -eq "$ProjectPath\Data")) {
  New-Item -Path $ProjectPath -Name 'Data' -ItemType "directory"
}
elseif (!(Test-Path $DataPath) -and ($DataPath -ne "$ProjectPath\Data")) {
  Write-Host "`nData file path does not exist. Please check that your file path is correct."
  Write-Host "`nAborting program, see above.  Press any button to exit.`n"
  $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown") > $null
  exit
}

# Everything the example needs from the updater: the DLL (into the Build folder),
# plus the headers and import library the compiler and linker need (into the project folder)
$DLLs = @(
  [DLLConfig]@{
    FileName       = "mdAddr.dll";
    ReleaseVersion = "$RELEASE_VERSION";
    OS             = "WINDOWS";
    Compiler       = "DLL";
    Architecture   = "64BIT";
    Type           = "BINARY";
  },
  [DLLConfig]@{
    FileName       = "mdEnums.h";
    ReleaseVersion = $RELEASE_VERSION;
    OS             = "ANY";
    Compiler       = "C";
    Architecture   = "ANY";
    Type           = "INTERFACE";  
  },
  [DLLConfig]@{
    FileName       = "mdAddr.h";
    ReleaseVersion = $RELEASE_VERSION;
    OS             = "ANY";
    Compiler       = "C";
    Architecture   = "ANY";
    Type           = "INTERFACE";
  },
  [DLLConfig]@{
    FileName       = "mdAddr.lib";
    ReleaseVersion = $RELEASE_VERSION;
    OS             = "WINDOWS";
    Compiler       = "C";
    Architecture   = "64BIT";
    Type           = "INTERFACE";
  }

)

######################## Functions #########################

# Download the product data file(s) into $DataPath via the Melissa Updater.
function DownloadDataFiles([string] $license) {
  $DataProg = 0
  Write-Host "========================== MELISSA UPDATER ========================="
  Write-Host "MELISSA UPDATER IS DOWNLOADING DATA FILE(S)..."
  
  .\MelissaUpdater\MelissaUpdater.exe manifest -p $ProductName -r $RELEASE_VERSION -l $license -t $DataPath
  
  if ($? -eq $false) {
    Write-Host "`nCannot run Melissa Updater. Please check your license string!"
    exit
  }

  Write-Host "Melissa Updater finished downloading data file(s)!"
}

# Download each entry in $DLLs, routing the DLL to the Build folder and the headers
# and import library to the project folder (with a progress bar).
function DownloadDLLs() {
  Write-Host "MELISSA UPDATER IS DOWNLOADING DLL(S)..."
  $DLLProg = 0
  foreach ($DLL in $DLLs) {
    Write-Progress -Activity "Downloading DLL(S)" -Status "$([math]::round($DLLProg / $DLLs.Count * 100, 2))% Complete:"  -PercentComplete ($DLLProg / $DLLs.Count * 100)

    # Check for quiet mode
    if ($quiet) {
      if ($DLL.FileName -eq "mdAddr.dll")
      {
        .\MelissaUpdater\MelissaUpdater.exe file --filename $DLL.FileName --release_version $DLL.ReleaseVersion --license $LICENSE --os $DLL.OS --compiler $DLL.Compiler --architecture $DLL.Architecture --type $DLL.Type --target_directory $BuildPath > $null
      }
      else
      {
        .\MelissaUpdater\MelissaUpdater.exe file --filename $DLL.FileName --release_version $DLL.ReleaseVersion --license $LICENSE --os $DLL.OS --compiler $DLL.Compiler --architecture $DLL.Architecture --type $DLL.Type --target_directory $ProjectPath > $null
      }
      
      if(($?) -eq $False) {
          Write-Host "`nCannot run Melissa Updater. Please check your license string!"
          Exit
      }
    }
    else {
      if ($DLL.FileName -eq "mdAddr.dll")
      {
        .\MelissaUpdater\MelissaUpdater.exe file --filename $DLL.FileName --release_version $DLL.ReleaseVersion --license $LICENSE --os $DLL.OS --compiler $DLL.Compiler --architecture $DLL.Architecture --type $DLL.Type --target_directory $BuildPath 
      }
      else
      {
        .\MelissaUpdater\MelissaUpdater.exe file --filename $DLL.FileName --release_version $DLL.ReleaseVersion --license $LICENSE --os $DLL.OS --compiler $DLL.Compiler --architecture $DLL.Architecture --type $DLL.Type --target_directory $ProjectPath
      }
      
      if(($?) -eq $False) {
          Write-Host "`nCannot run Melissa Updater. Please check your license string!"
          Exit
      }
    }
    
    Write-Host "Melissa Updater finished downloading " $DLL.FileName "!"
    $DLLProg++
  }
}

# Verify the DLL, headers, and import library landed where the build expects them
function CheckDLLs() {
  Write-Host "`nDouble checking dll(s) were downloaded."
  $FileMissing = $false 
  if (!(Test-Path ("$BuildPath\mdAddr.dll"))) {
    Write-Host "mdAddr.dll not found." 
    $FileMissing = $true
  }
  if (!(Test-Path ("$ProjectPath\mdEnums.h"))) {
    Write-Host "mdEnums.h not found." 
    $FileMissing = $true
  }
  if (!(Test-Path ("$ProjectPath\mdAddr.h"))) {
    Write-Host "mdAddr.h not found." 
    $FileMissing = $true
  }
  if (!(Test-Path ("$ProjectPath\mdAddr.lib"))) {
    Write-Host "mdAddr.lib not found." 
    $FileMissing = $true
  }

  if ($FileMissing) {
    Write-Host "`nMissing the above data file(s).  Please check that your license string and directory are correct."
    return $false
  }
  else {
    return $true
  }
}

########################## Main ############################

Write-Host "`n====================== Melissa Address Object =====================`n                     [ C++ | Windows | 64BIT ]`n"

# Get license (either from parameters or user input)
if ([string]::IsNullOrEmpty($license) ) {
  $License = Read-Host "Please enter your license string"
}

# Check for License from Environment Variables 
if ([string]::IsNullOrEmpty($License) ) {
  $License = $env:MD_LICENSE 
}

if ([string]::IsNullOrEmpty($License)) {
  Write-Host "`nLicense String is invalid!"
  Exit
}

# Get data file path (either from parameters or user input)
if ($DataPath -eq "$ProjectPath\Data") {
  $dataPathInput = Read-Host "Please enter your data files path directory if you have already downloaded the release zip.`nOtherwise, the data files will be downloaded using the Melissa Updater (Enter to skip)"

  if (![string]::IsNullOrEmpty($dataPathInput)) {
    if (!(Test-Path $dataPathInput)) {
      Write-Host "`nData file path does not exist. Please check that your file path is correct."
      Write-Host "`nAborting program, see above.  Press any button to exit.`n"
      $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown") > $null
      exit
    }
    else {
      $DataPath = $dataPathInput
    }
  }
}

# Use Melissa Updater to download data file(s) 
# Download data file(s) 
DownloadDataFiles -license $License # Comment out this line if using own DQS release

# Download dll(s)
DownloadDlls -license $License

# Check if all dll(s) have been downloaded. Exit script if missing
$DllsAreDownloaded = CheckDLLs

if (!$DLLsAreDownloaded) {
	Write-Host "`nAborting program, see above.  Press any button to exit."
  $Host.UI.RawUI.ReadKey("NoEcho,IncludeKeyDown")
  exit
}

Write-Host "All file(s) have been downloaded/updated!"

# Start program
# Build project
# Initializes the MSVC build environment, then hands off to BuildProgram.ps1,
# which runs nmake against the project's makefile.
Write-Host "`n=========================== BUILD PROJECT =========================="

cmd.exe /C """$CmdPath"" x86_x64 && Powershell -File BuildProgram.ps1" > $null

# Run Project
# No address supplied -> run interactively; otherwise pass the address in.
# The executable is produced into the Build folder by the build step above.
if ([string]::IsNullOrEmpty($address) -and [string]::IsNullOrEmpty($city) -and [string]::IsNullOrEmpty($state) -and [string]::IsNullOrEmpty($zip)){
	& $BuildPath\MelissaAddressObjectWindowsCpp.exe --license $License --dataPath $DataPath
}
else {
  & $BuildPath\MelissaAddressObjectWindowsCpp.exe --license $License --dataPath $DataPath --address $address --city $city --state $state --zip $zip
}
