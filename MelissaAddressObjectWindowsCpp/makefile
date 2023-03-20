# Melissa Address Object Windows C++

CXX=cl
CXXFLAGS=-Zi -EHsc
LDLIBS=mdAddr.lib
RM=del /q /f

all: MelissaAddressObjectWindowsCpp.exe clean

MelissaAddressObjectWindowsCpp.exe:	Main.cpp mdAddr.h mdEnums.h
	$(CXX) $(CXXFLAGS) Main.cpp AddressObject.cpp $(LDLIBS) /Fe:.\Build\MelissaAddressObjectWindowsCpp.exe

clean:
	$(RM) *.obj *.ilk *.pdb *.h.hash *.lib.hash 

