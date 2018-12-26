#include <iostream>
#include <windows.h>
#include <string>
#include <ctime>

#define LOG(x) std::cout << x << std::endl;

//Get PID
HWND hWindow = NULL;
LPCSTR LWindow = "FTL: Faster Than Light";
DWORD dwProcID = 0;
bool IsGameAvail = false;

//Menu
bool UpdateOnNextRun;
std::string sScrap = "OFF";
std::string sFuel = "OFF";
std::string sMissiles = "OFF";
std::string sDrones = "OFF";
std::string sHull = "OFF";

//Toggles
bool bScrap;
bool bFuel;
bool bMissiles;
bool bDrones;
bool bHull;

//Functions
void WriteToMemory(HANDLE hProcHandle);

//Memory Addresses
DWORD aScrap = { 0x015C448C };
DWORD aScrapOffset[] = { 0x4D4 };

DWORD aFuel = { 0x015C448C };
DWORD aFuelOffset[] = { 0x494 };

DWORD aMissiles = { 0x015C448C };
DWORD aMissilesOffset[] = { 0x48, 0x1E8 };

DWORD aDrones = { 0x015C448C };
DWORD aDronesOffset[] = { 0x800 };

DWORD aHull = { 0x015C448C };
DWORD aHullOffset[] = { 0xCC };

int main()
{
		HWND hGameWindow = NULL;

		int timeSinceLastUpdate = clock(); //Uses ctime to time updates. Optimizes performance.
		int GameAvailTMR = clock();
		int onePressTMR = clock();

		DWORD dwProcID = NULL; //Used in getting the Process ID.
		HANDLE hProcHandle = NULL;
		UpdateOnNextRun = true;

		while (!GetAsyncKeyState(VK_INSERT))
		{
				

					if (IsGameAvail)
					{
						if (UpdateOnNextRun)
						{
							system("CLS");
							system("color a");
							std::cout << "/////////////////////////////////////////////////////" << std::endl;
							std::cout << "//                     FTL Trainer                 //" << std::endl;
							std::cout << "//                     by Breadguy                 //" << std::endl;
							std::cout << "/////////////////////////////////////////////////////" << std::endl;
							std::cout << "[F1] Infinite Scrap: " << sScrap << std::endl;
							std::cout << "[F2] Infinite Fuel: " << sFuel << std::endl;
							std::cout << "[F3] Infinite Missiles: " << sMissiles << std::endl;
							std::cout << "[F4] Infinite Drone Parts: " << sDrones << std::endl;
							std::cout << "[F5] Infinite Hull: " << sHull << std::endl << std::endl;

							std::cout << "[INSERT] Exit" << std::endl;
							UpdateOnNextRun = false;
						}
					}
					else if (!IsGameAvail)
					{
						if (UpdateOnNextRun)
						{
							system("color C");
							std::cout << "/////////////////////////////////////////////////////" << std::endl;
							std::cout << "//                     FTL Trainer                 //" << std::endl;
							std::cout << "//                     by Breadguy                 //" << std::endl;
							std::cout << "/////////////////////////////////////////////////////" << std::endl;
							std::cout << "FTL: Faster Than Light not found!" << std::endl;
							std::cout << "Please open the game." << std::endl;

							UpdateOnNextRun = false;
						}

						hWindow = FindWindowA(NULL, LWindow);

						if (hWindow)
						{
							GetWindowThreadProcessId(hWindow, &dwProcID);
							if (dwProcID != 0)
							{
								hProcHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcID);
								if (hProcHandle == INVALID_HANDLE_VALUE || hProcHandle == NULL)
								{
									system("CLS");
									LOG("Failed to open process for valid handle");
								}
								else
								{
									IsGameAvail = true;
									UpdateOnNextRun = true;
								}
							}
						}
					}

					if (clock() - GameAvailTMR > 150)
					{
						if (GetAsyncKeyState(VK_F1))
						{
							bScrap = !bScrap;
							if (bScrap)
								sScrap = "ON";
							else
								sScrap = "OFF";
							UpdateOnNextRun = true;
						}

						if (GetAsyncKeyState(VK_F2))
						{
							bFuel = !bFuel;
							if (bFuel)
								sFuel = "ON";
							else
								sFuel = "OFF";

							UpdateOnNextRun = true;
						}

						if (GetAsyncKeyState(VK_F3))
						{
							bMissiles = !bMissiles;
							if (bMissiles)
								sMissiles = "ON";
							else
								sMissiles = "OFF";

							UpdateOnNextRun = true;
						}

						if (GetAsyncKeyState(VK_F4))
						{
							bDrones = !bDrones;
							if (bDrones)
								sDrones = "ON";
							else
								sDrones = "OFF";

							UpdateOnNextRun = true;
						}

						if (GetAsyncKeyState(VK_F5))
						{
							bHull = !bHull;
							if (bHull)
								sHull = "ON";
							else
								sHull = "OFF";

							UpdateOnNextRun = true;
						}

						WriteToMemory(hProcHandle);
						GameAvailTMR = clock();
					}
		}
		CloseHandle(hProcHandle);
		DestroyWindow(hWindow);
}

//Handles Dynamic memory allocation
//Receives how high the pointer level is e.g. 4 levels and from that calculates the initial address
//the offset values and the memory addresses for those offsets
DWORD FindDmaAddy(int PointerLevel, HANDLE hProcHandle, DWORD Offsets[], DWORD BaseAddress)
{
	//DECLARE BASE ADDRESS
	DWORD pointer = BaseAddress;             // Declare a pointer of DWORD
											 //USED TO output the contents in the pointer
	DWORD pTemp;

	DWORD pointerAddr;
	for (int i = 0; i < PointerLevel; i++)
	{
		if (i == 0)
		{
			ReadProcessMemory(hProcHandle, (LPCVOID)pointer, &pTemp, 4, NULL);
		}
		//add first offset to that address
		pointerAddr = pTemp + Offsets[i];   // Set p1 to content of p + offset

											//Read memory one more time and exit the loop
		ReadProcessMemory(hProcHandle, (LPCVOID)pointerAddr, &pTemp, 4, NULL);
	}
	return pointerAddr;
}

void WriteToMemory(HANDLE hProcHandle)
{
	if (bScrap)
	{
		int scrapval = 999;
		DWORD wScrapAddress = FindDmaAddy(1, hProcHandle, aScrapOffset, aScrap);
		WriteProcessMemory(hProcHandle, (BYTE*)wScrapAddress, &scrapval, sizeof(scrapval), NULL);
	}

	if (bFuel)
	{
		int fuelval = 999;
		DWORD wFuelAddress = FindDmaAddy(1, hProcHandle, aFuelOffset, aFuel);
		WriteProcessMemory(hProcHandle, (BYTE*)wFuelAddress, &fuelval, sizeof(fuelval), NULL);
	}

	if (bMissiles)
	{
		int missilevalue = 999;
		DWORD wMissileAddress = FindDmaAddy(2, hProcHandle, aMissilesOffset, aMissiles);
		WriteProcessMemory(hProcHandle, (BYTE*)wMissileAddress, &missilevalue, sizeof(missilevalue), NULL);
	}

	if (bDrones)
	{
		int dronesvalue = 999;
		DWORD wDroneAddress = FindDmaAddy(1, hProcHandle, aDronesOffset, aDrones);
		WriteProcessMemory(hProcHandle, (BYTE*)wDroneAddress, &dronesvalue, sizeof(dronesvalue), NULL);
	}

	if (bHull)
	{
		int hullvalue = 999;
		DWORD wHullAddress = FindDmaAddy(1, hProcHandle, aHullOffset, aHull);
		WriteProcessMemory(hProcHandle, (BYTE*)wHullAddress, &hullvalue, sizeof(hullvalue), NULL);
	}
}