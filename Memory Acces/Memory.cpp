#include "pch.h"
#include "includes.h"
#include "Memory.h"

// Var for getProcess
PROCESSENTRY32 entryProc;
HANDLE hProcSnap;
char procSearched[] = "hl2.exe";

// Var for getModule
MODULEENTRY32 entryMod32;
HANDLE hModSnap;
char modSearched[] = "client.dll";

bool CManager::getProcess() { // Fonction permettant d'ouvrir un processus demandé
	hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL); // Récupération de tous les processus en cours d'exec
	entryProc.dwSize = sizeof(PROCESSENTRY32); // Taille de la structure PROCESSENTRY32 nécessaire pour l'execution de Process32Next
	if (hProcSnap == INVALID_HANDLE_VALUE) { // Si CreateToolhelp32Snapshot renvoie l'erreur
		std::cout << "Echec de l'obtention des programmes en cours d'execution !" << std::endl;
		std::cout << "Code d'erreur : " << GetLastError() << std::endl << std::endl; // Récupération du code erreur

		std::cout << std::endl << "Appuyez sur ENTER pour fermer le programme" << std::endl;
		std::cin.ignore();
		CloseHandle(hProcSnap);
		return false;
	}
	while (Process32Next(hProcSnap, &entryProc)) { // Tant que il y a toujours un processus dans hProcSnap, on le stocke dans entryProc
		std::cout << "Programme en cours de verification : " << entryProc.szExeFile << std::endl; 
		if (!strcmp(entryProc.szExeFile, procSearched)) { // Comparaison du processus en cours de vérif avec celui demandé par user
			system("cls");
			std::cout << "Processus " << procSearched << " trouve !" << std::endl;
			std::cout << "Acces au processus en cours." << std::endl;
			pId = entryProc.th32ProcessID;
			hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, entryProc.th32ProcessID);
			if (hProcess == NULL) { // En cas d'une erreur retourné dans hProcess par OpenProcess
				std::cout << "Une erreur a ete rencontre lors de l'acces au programme !" << std::endl;
				std::cout << "Code erreur : " << GetLastError() << std::endl << std::endl; // Récupération du code erreur

				std::cout << "Appuyez sur ENTER pour fermez le programme" << std::endl;
				std::cin.ignore();
				return false;
			}
			CloseHandle(hProcSnap); // Ferme l'handle des snapshots
			system("cls");
			return true;
		}
	}
	system("cls");
	std::cout << "Aucun programme correspondant a : " << procSearched << " n'a ete trouvee !" << std::endl << std::endl;
	std::cout << "Appuyez sur ENTER pour fermez le programme" << std::endl;
	std::cin.ignore();
	return false;
}

bool CManager::getProcessTest() {
	hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	entryProc.dwSize = sizeof(PROCESSENTRY32);
	if (hProcSnap == INVALID_HANDLE_VALUE) { // Si CreateToolhelp32Snapshot renvoie l'erreur
		std::cout << "Echec de l'obtention des programmes en cours d'execution !" << std::endl;
		std::cout << "Code d'erreur : " << GetLastError() << std::endl << std::endl; // Récupération du code erreur

		std::cout << std::endl << "Appuyez sur ENTER pour fermer le programme" << std::endl;
		std::cin.ignore();
		CloseHandle(hProcSnap);
		return false;
	}
	while (Process32Next(hProcSnap, &entryProc)) { // Tant que il y a toujours un processus dans hProcSnap, on le stocke dans entryProc
		std::cout << "Programme en cours de verification : " << entryProc.szExeFile << std::endl;
		if (!strcmp(entryProc.szExeFile, procSearched)) { // Comparaison du processus en cours de vérif avec celui demandé par user
			system("cls");
			std::cout << "Processus " << procSearched << " trouve !" << std::endl;
			CloseHandle(hProcSnap); // Ferme l'handle des snapshots
			system("cls");
			return true;
		}
	}
	system("cls");
	std::cout << "Aucun programme correspondant a : " << procSearched << " n'a ete trouvee !" << std::endl << std::endl;
	std::cout << "Appuyez sur ENTER pour fermez le programme" << std::endl;
	std::cin.ignore();
	return false;
}

bool CManager::getModule() {
	hModSnap = INVALID_HANDLE_VALUE;
	hModSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, pId);
	entryMod32.dwSize = sizeof(MODULEENTRY32);
	if (hModSnap == INVALID_HANDLE_VALUE) {
		std::cout << "Erreur lors de la recuperation des modules lies a : " << procSearched << " !" << std::endl;
		system("COLOR 0C"); // Couleur en rouge
		std::cout << "Code erreur : " << GetLastError() << std::endl << std::endl;

		std::cout << std::endl << "Appuyez sur ENTER pour fermer le programme" << std::endl;
		std::cin.ignore();
		CloseHandle(hModSnap);
		return false;
	}
	if (Module32First(hModSnap, &entryMod32)) {
		if (!strcmp(entryMod32.szModule, modSearched)) {
			std::cout << "Module : " << modSearched << " trouve !" << std::endl;
			CloseHandle(hModSnap);
			bA = (DWORD)entryMod32.modBaseAddr;
			return true;
		}
	}
	while (Module32Next(hModSnap, &entryMod32)) {
		if (!strcmp(entryMod32.szModule, modSearched)) {
			std::cout << "Module : " << modSearched << " trouve !" << std::endl;
			CloseHandle(hModSnap);
			bA = (DWORD)entryMod32.modBaseAddr;
			return true;
		}
	}
	std::cout << "Module demandé inexistant !" << std::endl << std::endl;
	std::cout << std::endl << "Appuyez sur ENTER pour fermer le programme" << std::endl;
	std::cin.ignore();
	return false;
}

void CManager::set_lP() {
	lP = rpm<DWORD>(bA + dwLocalPlayer);
	if (GetLastError() == 0) {
		std::cout << "Une erreur a été rencontré !" << std::endl;
		std::cout << "Code erreur : " << GetLastError() << std::endl;
	}
	else {
		std::cout << "LocalPlayer trouvé !" << std::endl;
	}
}

int CManager::get_flags() {
	return rpm<int>(lP + m_fFlags);
}

void CManager::jumpForce() {
	wpm<int>(1, bA + dwForceJump);
	Sleep(1);
	wpm<int>(0, bA + dwForceJump);
}
