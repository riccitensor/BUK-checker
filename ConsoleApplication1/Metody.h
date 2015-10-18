#include "stdafx.h"

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <shellapi.h>
#include <iostream>
#include <string>
//#include <atlstr.h>
#include <sstream>
#include <ctime>


//#include "WinHTTPClient.h"

using namespace std;

string SubUrl_test = "Forum/?actionType=refresh";
string SubUrl_prev = "Forum/Read.aspx?n=";
string SubUrl_next = "&t=16839741&v=4";
string linkToStomil = "Forum/Read.aspx?t=16839741&n=1&v=4&u=96182800"; //zajrzyj to funkcji link, nie chce zczytac ostatniego zera do pamieci


string adres_domenowy;
int liczbapostow;


boolean open(wstring program, wstring path_program)
{
	try{
		ShellExecute(GetDesktopWindow(), L"open", program.c_str(), 0, path_program.c_str(), SW_SHOWDEFAULT);
		
	}
	catch (int e){
		cout << "ERROR: " << e << '\n';
		return -1;
	}


	return 0;
}
boolean singleButton(WORD key)
{
	INPUT ip, iip;

	// Set up a generic keyboard event.
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0; // hardware scan code for key
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	iip.type = INPUT_KEYBOARD;
	iip.ki.wScan = 0; // hardware scan code for key
	iip.ki.time = 0;
	iip.ki.dwExtraInfo = 0;

	// Press the "single" key (wejscie do paska adresu)
	iip.ki.wVk = key; // virtual-key code for the "a" key
	iip.ki.dwFlags = 0; // 0 for key press
	SendInput(1, &iip, sizeof(INPUT));


	// Release the "single" key
	iip.ki.dwFlags = KEYEVENTF_KEYUP; // KEYEVENTF_KEYUP for key release
	SendInput(1, &iip, sizeof(INPUT));
	return 0;
}
boolean pressCButton(WORD key) //CTRL+A, CTRL+C, CTRL+V, CTRL+X,
{
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	while (1)
	{

		// Press the "Ctrl" key
		ip.ki.wVk = VK_CONTROL;
		ip.ki.dwFlags = 0; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));

		// Press the key
		ip.ki.wVk = key;
		ip.ki.dwFlags = 0; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));

		// Release the key
		ip.ki.wVk = key;
		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(INPUT));

		// Release the "Ctrl" key
		ip.ki.wVk = VK_CONTROL;
		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(INPUT));

		//Sleep(2000);
		return 0;
	}
}
boolean close_window()
{
	INPUT ip;
	ip.type = INPUT_KEYBOARD;
	ip.ki.wScan = 0;
	ip.ki.time = 0;
	ip.ki.dwExtraInfo = 0;

	while (1)
	{

		// Press the "Alt" key
		ip.ki.wVk = VK_LMENU;
		ip.ki.dwFlags = 0; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));

		// Press the "F4" key
		ip.ki.wVk = 0x73;
		ip.ki.dwFlags = 0; // 0 for key press
		SendInput(1, &ip, sizeof(INPUT));

		// Release the "F4" key
		ip.ki.wVk = 0x73;
		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(INPUT));

		// Release the "Alt" key
		ip.ki.wVk = VK_MENU;
		ip.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &ip, sizeof(INPUT));

		Sleep(50);
		return 0;
	}
}



void toClipboard(const string &s){
	OpenClipboard(0);
	EmptyClipboard();
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, s.size());
	if (!hg){
		CloseClipboard();
		return;
	}
	memcpy(GlobalLock(hg), s.c_str(), s.size());
	GlobalUnlock(hg);
	SetClipboardData(CF_TEXT, hg);
	CloseClipboard();
	GlobalFree(hg);
}

string clipboard()
{
	char *schowek;
	OpenClipboard(NULL);
	schowek = (char*)GetClipboardData(CF_TEXT);
	CloseClipboard();

	return schowek;
}
string extractURLData(string abc)
{
	
	char mainUrl[1024];
	memset(mainUrl, 0, sizeof(mainUrl));
	int rozmiar = abc.length();
	for (int i = 0; i<rozmiar; i++)
	{
		if ((abc[i] != 'M') && (abc[i] != '?'))
		{
			mainUrl[i] = abc[i];
		}
		else
		{
			break;
		}
	}
	adres_domenowy = mainUrl;

	return adres_domenowy;
}




int SzukanieDaty(string schowek)
{
	SYSTEMTIME st;
	GetLocalTime(&st);

	int pozycja = 0;
	bool czyjestdata = 0;

	//data systemowa
	int godzina = st.wHour;
	int minuta = st.wMinute;
	int dzien = st.wDay;
	int miesiac = st.wMonth;
	int rok = st.wYear;

	//data ostatniego posta
	int godzina_post = 0;
	int minuta_post = 0;
	int dzien_post = 0;
	int miesiac_post = 0;
	int rok_post = 0;

	string SzukanaFraza = "class=\"backToTop\" /></a> ";
	size_t znalezionaPozycja = schowek.find(SzukanaFraza);

	if (znalezionaPozycja == std::string::npos)
	{
		cout << "Nie znaleziono frazy w tekscie" << endl;
		return 0;
	}

	do
	{
		pozycja = znalezionaPozycja + 25;
		znalezionaPozycja = schowek.find(SzukanaFraza, znalezionaPozycja + SzukanaFraza.size());

	} while (znalezionaPozycja != std::string::npos);



	char obrobka[17];
	int checkpoint = 0;
	for (int i = 0; i < 17; i++)
	{

		obrobka[i] = schowek[pozycja + i];
		if (schowek[pozycja + i] == ':') checkpoint = pozycja + i + 2;

		if (checkpoint == pozycja + i)
		{
			obrobka[i + 1] = '\0';
			if ((i + 1) == 5) czyjestdata = 1; //sama godzina bez daty, czyli aktualny dzien
			break;
		}
	}
	if (czyjestdata == 1)
	{
		if ((obrobka[0] == 0) && (obrobka[1] == 0))
		{
			godzina_post = 0;
		}
		else
		{
			dzien_post = st.wDay;
			miesiac_post = st.wMonth;
			rok_post = st.wYear;
			godzina_post = int(obrobka[0]) * 10 - 480 + int(obrobka[1]) - 48;
			minuta_post = int(obrobka[3]) * 10 - 480 + int(obrobka[4]) - 48;
		}
	}
	else
	{
		dzien_post = int(obrobka[0]) * 10 - 480 + int(obrobka[1]) - 48;
		miesiac_post = int(obrobka[3]) * 10 - 480 + int(obrobka[4]) - 48;
		rok_post = int(obrobka[6]) * 1000 - 48000 + int(obrobka[7]) * 100 - 4800 + int(obrobka[8]) * 10 - 480 + int(obrobka[9]) - 48;
		godzina_post = int(obrobka[11]) * 10 - 480 + int(obrobka[12]) - 48;
		minuta_post = int(obrobka[14]) * 10 - 480 + int(obrobka[15]) - 48;
	}

	cout << "godzina systemowa: ";
	cout << godzina << ":";
	if (minuta < 10) cout << "0";
	cout<< minuta << endl;
	cout << "ostatni post: ";
	cout << godzina_post << ":";
	if (minuta_post < 10) cout << "0";
	cout<< minuta_post << "\t" << dzien_post << "-" << miesiac_post << "-" << rok_post << endl;

	struct tm czasforum = { 0, minuta_post, godzina_post, dzien_post, miesiac_post - 1, rok_post - 1900 }; /* June 24, 2004 */
	struct tm systemowa = { 0, minuta, godzina, dzien, miesiac - 1, rok - 1900 }; /* July 5, 2004 */
	time_t x = mktime(&czasforum);
	time_t y = mktime(&systemowa);
	if (x != (std::time_t)(-1) && y != (std::time_t)(-1))
	{
		double difference = std::difftime(y, x) / (60);
		int roznica = difference;
		cout << "Od ostatniego posta minelo - " << roznica / 1440 << " dni " << (roznica % 1440) / 60;
		cout<< " godzin i " << roznica % 1440 % 60 << " minut." << endl << endl;
		if (difference < 16) return TRUE; else return FALSE;
	}
}


void link(string s)
{
	string new_adres;

	new_adres = s + linkToStomil; //JEST OK, wszystko kopiuje siê prawidlowo, rowniez 0 na koncu stringa
	toClipboard(new_adres); //nie chce sie skopiowac 0 ktore jest w zdefiniowanym stringu na koncu w linkToStomil

	
}
