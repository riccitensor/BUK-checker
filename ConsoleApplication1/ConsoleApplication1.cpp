#include "stdafx.h"
#include "Metody.h"
#include <windows.h>
#include <stdlib.h>
#include <shellapi.h>
#include <iostream>
#include <string>
#include <fstream>

using namespace std;

//linki do REST API -- NIE ZMIENIAC
string michal = "https://api.gsmservice.pl/v5/send.php?login=riccitensor&pass=owlowl11&recipient=48728265442&message=HEHE&sender=GENIUS";
string slawek = "https://api.gsmservice.pl/v5/send.php?login=riccitensor&pass=owlowl11&recipient=48502557708&message=HEHE&sender=GENIUS";

//Sciezka do otwieranego programu

wstring firefox (L"firefox.exe"); //plik wykonywalny uruchamianego programu
wstring path_firefox = (L"C:\\Program Files(x86)\\Mozilla Firefox"); //plik wykonywalny uruchamianego programu


int main(int argc, char* argv[])
{
	//wczytujemy url do postów z pliku ../Date/forum.txt (po zape³nieniu lub zamknieciu w¹tku nale¿y wpisaæ url do nowego w¹tku w forum
	load_url();

	SYSTEMTIME ct;
	GetLocalTime(&ct);
	int h_start = ct.wHour;
	int odstep = 0;

	cout << "----------------------- 1 logowanie -----------------------" << endl;
	srand(time(NULL)); //randomizacja odstepów czasu pomiêdzy logowaniami
	for (int i = 0; i>=0; i++) 
	{
		SYSTEMTIME ct;
		GetLocalTime(&ct);
		int h_start = ct.wHour;
		int m_start = ct.wMinute;

		if (h_start >= 12 && h_start<21)  // program bedzie uruchamiany miedzy godzina 12 a 21
		{
			SYSTEMTIME st;
			GetLocalTime(&st);

				std::ofstream logs;
				logs.open("logs.txt", ios::app);
				logs << "[I]  Log start." << endl;


			try {
					open(firefox, path_firefox); //otwieramy program, firefox
					Sleep(6000); //czekamy az siê odpali
					singleButton(VK_RETURN);  //wciskamy enter, czyli logowanie z wypelnionymi juz danymi na stronie startowej
					Sleep(6000);  //czekamy az sie strona zaladuje
					singleButton(VK_F6);  //podswietlamy pasek adresu
					Sleep(1000);
				}

			catch (int e) 
				{
					logs << "[-] STAGE 0 OKAY" << endl;
				}

			try {
					pressCButton('A');
					pressCButton('X'); //po ctrl + X musi byæ SLEEP !!!!!!!!!!!
					//skopiowany adres z wwwXX.hatr.... musi zostac przerobiony na wlasciwy link
					Sleep(3000);
					link(extractURLData(clipboard())); //laczymy dwa stringi, glowny adres wwwXX.hatt plus adres do wlasciwego miejsca w forum
					Sleep(1000);
					pressCButton('V');
					Sleep(2000);

					singleButton(VK_RETURN);//przechodzimy do tego adresu
					Sleep(5000);
					//jestesmy na stronie forum
				}


			catch (int e) {
				logs << "[-] STAGE 1 FAIL" << endl;
			}

			try {
				//tu co bylo jest potrzebne tylko do srodowiska testowego, tutaj jest zbede, byloby zle przekierowanie
			}
			catch (int e) {
				logs << "[-] STAGE 2 FAIL" << endl;
			}

			try {
				pressCButton('U');
				Sleep(4500);
				pressCButton('A');
				pressCButton('C');//po ctrl + C musi byæ SLEEP !!!!!!!!!!!
				//kopiujemy zrodlo strony by znalezc date ostatniego postu
				//UWAGA, trzeba napisac pewn¹ funkcje, jakis licznik
				//np jestli program zczyta datê i iteracja nastapi za 8 minut, pozniej np za 7 minut to dostaniemy az 3 sms'y dotyczacego
				//tego samego postu, by sms'a dostajemy jesli program natrafi na post nie starszy niz 15 minut od aktualnej daty
				Sleep(2000);
				close_window();//zamykamy okno ze zrodlem strony
			}
			catch (int e) {
				logs << "[-] STAGE 3 FAIL" << endl;
			}

			int res = SzukanieDaty(clipboard());//szuka ostatniej daty na forum
			//jak bedzie wiecej jak 40 postow bedzie trzeba sprawdzic prawidlowosc dzialania programu !!!!
			logs << "[-] NO MESSAGE" << endl;

			if (res == 1) {

				toClipboard(slawek);
				singleButton(VK_F6);

				pressCButton('A');
				pressCButton('V');
				singleButton(VK_RETURN);

				logs << "[SMS]  ---------------------MESSAGE SENT ------------------------" << endl;

				Sleep(1000);
				close_window(); //zamykamy firefox
				//Sleep(300000);

			}


			close_window(); //zamykamy firefox
			logs << "[I]  Close window.\n" << endl;


			odstep = ((rand() % 240) + 240) * 1000;
			cout << "------- " << i + 2 << " logowanie (po " << odstep / 1000 / 60 << " minutach i " << (odstep / 1000) % 60 << " sekundach) --------" << endl;
			Sleep(odstep);
			logs.close(); //czy logi beda nadpisywane czy nowe dolaczane do starych??

			// pozniej zczytamy date systemow¹, jesli bedzie powyzej 22.00 ale ponizej 10.00 nie wykonujemy zawartosci, tylko wykonujemy petle co jakis czas
			// jesli data systemowa bedzie powyzej 10.00 ale ponizej 22.00 to wznawiamy petle
		}
		else
		{
			cout << "Jest godzina " << h_start << ":";
			cout << "skopiowany z data : "; link(extractURLData(clipboard()));
			Sleep(5000);
			pressCButton('V');
			if (m_start < 10) cout << "0";
			cout << m_start << ", wstrzymuje autologowanie do godz 11.00" << endl;
			Sleep(1800000); //co pó³ godziny bêdzie sprawdza³ godzinê systemow¹
		}//koniec if
	}
	
}





