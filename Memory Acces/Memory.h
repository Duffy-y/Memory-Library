#pragma once
#include "pch.h"
#include "string"
#include "iostream"
#include "windows.h"
#include "tlhelp32.h"

class CManager {
	public:

		// Fonction pour ouvrir le processus en question
		bool getProcess(); // Cr�ation de l'handle hProcess
		bool getProcessTest(); // V�rifie si un process existe (AKA AC)
		bool getModule(); // R�cup�ration de l'adresse de base du client

		// Fonction pour simplifier l'interaction avec le jeu
		void set_lP(); // Recupere l'adresse du Local
		int get_flags(); // Recupere la valeur des flags
		void jumpForce();

		// Fonction d'�criture/lecture
		template <class dataType>
		void wpm(dataType dataToWrite, DWORD addressToW) {
			WriteProcessMemory(hProcess, (LPVOID)addressToW, &dataToWrite, sizeof(dataType), NULL);
		}

		template <class dataType>
		dataType rpm(DWORD addressToR) {
			dataType dataBuffer;
			ReadProcessMemory(hProcess, (LPCVOID)addressToR, &dataBuffer, sizeof(dataType), NULL);
			return dataBuffer;
		}

	protected:
		HANDLE hProcess;
		DWORD pId;
		DWORD bA;
		DWORD lP;

	private:
		// Netvar
		DWORD m_fFlags = 0x100;

		// Offsets
		DWORD dwForceJump = 0x050E444C;
		DWORD dwLocalPlayer = 0x00C648AC;
};