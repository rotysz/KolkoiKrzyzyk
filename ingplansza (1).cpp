#include <iostream>
#include <windows.h>
#include <stdio.h>

using namespace std;
#define ROZMIARX 80
#define ROZMIARY 24
#define MAXPLANSZA 10

char ekran[ROZMIARX][ROZMIARY]; //wirtualna reprezentacja znaków na ekranie
WORD ekolor[ROZMIARX][ROZMIARY]; //wirturalna reprezentacja kolorów znaków na ekranie
int wagipozycji[ROZMIARX][ROZMIARY];

char gracz_1[30]; //nazwa gracza 1
char gracz_2[30]; //nazwa gracza 2
char *nazwa_gracza_aktywnego; // wskaŸnik na aktywnego gracza

int gracz_aktywny; //numer gracza aktywnego {0,1}
int rozmiar_planszy;
int tryb;
int numer_ruchu;
int x,y;
int ilosc_do_wygrania; //iloœæ znaków w lini koniecznych do wygrania
char znak_aktywny;     //Znak aktywnego gracza {'X','O'}

char plansza[MAXPLANSZA][MAXPLANSZA];//reprezentacja planszy

//Zapisanie ci¹gu znaków w wirtualnej reprezentacji ekranu
void Wyswietl_ciag(char *ciag,int x, int y)
{
    for(int i=0; i<strlen(ciag); i++)
    {
        ekran[x+i][y]=*(ciag+i);
    }

}

//Zapisanie ci¹gu znaków w wirtualnej reprezentacji ekranu z mo¿liwoœci¹ zmiany koloru
void Wyswietl_ciag_kolor(char *ciag,WORD kolor,int x, int y)
{
    for(int i=0; i<strlen(ciag); i++)
    {
        ekran[x+i][y]=*(ciag+i);
        ekolor[x+i][y] = kolor;
    }

}

//inicjalizacja reprezentacji planszy
void Plansza_poczatkowa(int rozmiar_planszy)
{

    for(int i=0; i<rozmiar_planszy; i++)
    {
        for(int j=0; j<rozmiar_planszy; j++)
        {
            plansza[i][j]='#';
            //zmienna++;

        }
    }
}

//wyczyszczenie wirturalnej reprezentacji ekranu
void Wyczysc_ekran()
{
    for(int x =0; x<ROZMIARX; x++)
    {
        for(int y=0; y<ROZMIARY; y++)
        {
            ekran[x][y]=' ';
            ekolor[x][y]= 0;
        }
    }
}

//Przeniesienie wirturalnej reprezentacji ekranu na ekran rzeczywisty
void Odswiezenie()
{
    //Pobieramy uchwyt do systemowej konsoli potrzebny do wywo³ania funkcji zmiany koloru
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    //czyœcimy ekran
    system("cls");
    //dla ka¿dego elementu tablicy ekran bê¹cego wirtualn¹ reprezentacj¹ ekranu do wyœwietlania
    for(int y =0; y<ROZMIARY; y++)
    {
        for(int x=0; x<ROZMIARX; x++)
        {
            if (ekolor[x][y] != 0 ) //je¿eli znak o wspó³rzêdnych x,y ma mieæ kolor inny ni¿ domyœlny
                SetConsoleTextAttribute(hConsole,ekolor[x][y]); //ustawiamy zadany kolor
            else
                //w przeciwnym wypadku ustawiamy kolor bia³y jako z³o¿enie trzech barw podstawowych niebieskiej,zielonej, i czerwonej
                SetConsoleTextAttribute(hConsole,FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);

            // wyœwietlamy znak z wittualnej reprezentacji ekranu na ekran
            cout<<ekran[x][y];
        }
        //przy zmianie y musimy zacz¹æ wyœwietlanie od nowej lini
        cout<<"\n";

    }
}

//Zapisanie jednego znaku w wirturalnej reprezentacji ekranu
void Wyswietl_znak(char znak, int x, int y)
{
    ekran[x][y]=znak;

}

//Zapisanie jednego znaku w wirturalnej reprezentacji ekranu z  ozliwoœci¹ zmiany koloru
void Wyswietl_znak_kolor(char znak, WORD kolor,int x, int y)
{
    ekran[x][y]=znak;
    ekolor[x][y]= kolor;
}

//Zapisanie liczby w wirturalnej reprezentacji ekranu
void Wyswietl_liczbe(int liczba, int x, int y)
{
    char tablica_znakow[80];
    sprintf(tablica_znakow,"%d",liczba);
    Wyswietl_ciag(tablica_znakow,x,y);
}

//Wyczyszczenie jednej lini w wirtralnej reprezentacji ekranu
void Wyczysc_linie(int y)
{
    for(int x =0; x<ROZMIARX; x++)
    {
        Wyswietl_znak_kolor(' ',0,x,y);
    }
}

//Zapisanie danych lini statusowej w wirturalnej reprezentacji ekranu
void Wyswietl_status(int numer_ruchu, char *gracz)
{
    Wyczysc_linie(0);
    Wyswietl_ciag("Ruch: ", 0,0);
    Wyswietl_liczbe(numer_ruchu,7,0);
    Wyswietl_ciag_kolor(gracz,FOREGROUND_GREEN,20,0);
    Wyswietl_ciag(gracz_1,40,0);
    Wyswietl_ciag(gracz_2,60,0);

}

//Zapisanie planszy w wirturalnej reprezentacji ekranu
void Wyswietl_plansze(int x, int y)
{

    //Linia statusowa
    Wyswietl_status(numer_ruchu,nazwa_gracza_aktywnego);
    //Kreski poziome
    int zmienna2=0; //powieksza linie
    for(int j=0; j<=rozmiar_planszy; j++) //rysuje kreski poziome
    {
        int zmienna1=0; //powieksza linie
        for(int i=0; i<=6*rozmiar_planszy; i++)
        {
            Wyswietl_ciag("-",x+zmienna1,(y-1)+zmienna2);
            zmienna1=zmienna1+1;

        }
        zmienna2=zmienna2+2;
    }

    for(int i=0; i<=rozmiar_planszy*2-1; i++) //rysuje kreski pionowe
    {
        int zmiennax=0;
        for(int j=0; j<=rozmiar_planszy; j++)
        {
            Wyswietl_znak('|',x+zmiennax,y+i);
            zmiennax=zmiennax+6;
        }
        i++;
    }
    //Odwzorowanie aktualnego stanu gry

    for(int x=0; x<rozmiar_planszy; x++)
    {
        for(int y=0; y<rozmiar_planszy; y++)
        {
            if (plansza[x][y] == 'X')
                //Krzy¿ki na zielono
                Wyswietl_znak_kolor(plansza[x][y],FOREGROUND_GREEN|FOREGROUND_INTENSITY,16+x*6,4+y*2);
            else if (plansza[x][y] == 'O')
                //Kolka na czerwono
                Wyswietl_znak_kolor(plansza[x][y],FOREGROUND_RED|FOREGROUND_INTENSITY,16+x*6,4+y*2);
            else
                //Puste pola na niebiesko
                Wyswietl_znak_kolor(plansza[x][y],FOREGROUND_BLUE,16+x*6,4+y*2);
        }
    }
    //Wyœwietlenie indeksów planszy
    for(int i=0; i<rozmiar_planszy; i++)
    {
        Wyswietl_liczbe(i,16+i*6,2); //Kolumny numerycznie
        Wyswietl_znak('a'+i,11,4+i*2); //Wiersze literowo
    }

}


int Sprawdz_wygrana_poziomo(int x_pocz, int y_pocz,int ilosc_do_wygrania)
{
    int koniec_funkcji=1;
    int index=1;
    if(plansza[x_pocz][y_pocz]=='#')
    {
        return 0;
    }
    else if(x_pocz>rozmiar_planszy-ilosc_do_wygrania)
        return 0;
    for(int i=1; i<ilosc_do_wygrania; i++)
    {
        if(plansza[x_pocz+i][y_pocz]!=plansza[x_pocz][y_pocz])
        {
            return 0;
        }

    }
    return 1;

}

int Sprawdz_wygrana_pionowo(int x_pocz, int y_pocz,int ilosc_do_wygrania)
{
    int koniec_funkcji=1;
    if(plansza[x_pocz][y_pocz]=='#')
    {
        return 0;
    }
    else if(y_pocz>rozmiar_planszy-ilosc_do_wygrania)
        return 0;
    for(int i=1; i<ilosc_do_wygrania; i++)
    {
        if(plansza[x_pocz][y_pocz+i]!=plansza[x_pocz][y_pocz])
        {
            return 0;
        }

    }
    return 1;

}



int Sprawdz_wygrana_ukos1(int x_pocz,int y_pocz,int ilosc_do_wygrania)
{
    int koniec_funkcji=1;
    if(plansza[x_pocz][y_pocz]=='#')
    {
        return 0;
    }
    else if(y_pocz>rozmiar_planszy-ilosc_do_wygrania)
        return 0;
    else if(x_pocz>rozmiar_planszy-ilosc_do_wygrania)
    {
        return 0;
    }
    for(int i=1; i<ilosc_do_wygrania; i++)
    {
        if(plansza[x_pocz+i][y_pocz+i]!=plansza[x_pocz][y_pocz])
        {
            return 0;
        }

    }
    return 1;

}

int Sprawdz_wygrana_ukos2(int x_pocz,int y_pocz,int ilosc_do_wygrania)
{
    if(plansza[x_pocz][y_pocz]=='#')
    {
        return 0;
    }
    else if(y_pocz>rozmiar_planszy-ilosc_do_wygrania)
        return 0;
    else if(x_pocz-ilosc_do_wygrania+1<0)
    {
        return 0;
    }
    for(int i=1; i<ilosc_do_wygrania; i++)
    {
        if(plansza[x_pocz-i][y_pocz+i]!=plansza[x_pocz][y_pocz])
        {
            return 0;
        }

    }
    return 1;

}

int Sprawdz_wygrana(int ilosc_znakow_wygr)
{
    int wygrana = 0;
    for(int x=0; x<rozmiar_planszy; x++)
    {
        for(int y=0; y<rozmiar_planszy; y++)
        {
            if(Sprawdz_wygrana_pionowo(x,y,ilosc_znakow_wygr)==1)
                wygrana++;
            if(Sprawdz_wygrana_poziomo(x,y,ilosc_znakow_wygr)==1)
                wygrana++;
            if(Sprawdz_wygrana_ukos1(x,y,ilosc_znakow_wygr)==1)
                wygrana++;
            if(Sprawdz_wygrana_ukos2(x,y,ilosc_znakow_wygr)==1)
                wygrana++;

        }
    }
    return wygrana;
}


int Skalkuluj_wagi()
{
    char tmp[20];
    //zerujemuy tablice zawierajaca kalkulacje siły ruchu w konkratenekj pozycji
    for(int x=0; x<rozmiar_planszy; x++)
    {
        for(int y=0; y<rozmiar_planszy; y++)
        {
            if (plansza[x][y] == '#') //czy komórka jest pusta
                wagipozycji[x][y] = 0;  // jeżeli tak to zaczynamy od 0
            else
                wagipozycji[x][y] = -100; //jeżeli jest zajęta to siła jet mninimalna - nie możemy zrobić tam ruchu
        }
    }

    //robimy symulację potencjalnych efektów ruchu w danej komórce dla różnych dłufości wygranej (od 3 do maksymalnej wygranej)
    for (int dlwyg = min(ilosc_do_wygrania-1,3); dlwyg <= ilosc_do_wygrania; dlwyg++)
    {
        //Liczymy czy jeżeli zrobiliśmy my lub przeciwnik ruch w danej koórce to moglibyśmy liczyć na wygraną
        for(int x=0; x<rozmiar_planszy; x++)
        {
            for(int y=0; y<rozmiar_planszy; y++)
            {

                char znak = plansza[x][y]; //zabezpieczamy stan symulowanej komórki
                if (znak == '#') //jeżeli komórka jest pusta
                {
                    plansza[x][y]= 'X';
                    // sprawdzamy czy wygramy jeżeli wstawimy X
                    //if (Sprawdz_wygrana(dlwyg)==1)
                    //    wagipozycji[x][y] +=dlwyg*dlwyg; // siła ruchu jest tym większa im dłuższa linia
                    wagipozycji[x][y] +=Sprawdz_wygrana(dlwyg)*dlwyg*dlwyg;

                    plansza[x][y]= 'O'; //// sprawdzamy czy wygramy jeżeli wstawimy O
                    //if (Sprawdz_wygrana(dlwyg)==1)
                    //    wagipozycji[x][y] +=dlwyg*dlwyg;
                    wagipozycji[x][y] +=Sprawdz_wygrana(dlwyg)*dlwyg*dlwyg;
                    plansza[x][y]= znak; //Przywracamy stan początkowy komórki
                }

            }
        }
    }

    int xn;
    int yn;
    int maxwag = -2;

    //znajdujemy komórkę o największej sile ruchu
    for(int y=0; y<rozmiar_planszy; y++)
    {
        for(int x=0; x<rozmiar_planszy; x++)
        {
            if (wagipozycji[x][y]>=maxwag)
            {
                xn= x;
                yn=y;
                maxwag = wagipozycji[x][y];
            }
            cout<<wagipozycji[x][y]<<" ";
        }
        cout<<"\n";
    }
    plansza[xn][yn]=znak_aktywny; //wykonujemy ruch do komórki o największej sile
    cin>>tmp;

}


//Zmiana aktywnego gracza
void Zmiana_gracza()
{
    if(gracz_aktywny==0)
    {
        gracz_aktywny=1;
        nazwa_gracza_aktywnego=gracz_2;

    }
    else
    {
        gracz_aktywny=0;
        nazwa_gracza_aktywnego=gracz_1;
    }
    if(znak_aktywny=='X')
    {
        znak_aktywny='O';
    }
    else
        znak_aktywny='X';

}
//Je¿eli wszystkie pola zape³nione to remis
int Sprawdz_czy_remis()
{
    for(int x=0; x<rozmiar_planszy; x++)
    {
        for(int y=0; y<rozmiar_planszy; y++)
        {
            if(plansza[x][y]=='#')
            {
                return 0; //nie ma remisu jeszcze jest puste pole
            }
        }
    }
    return 1; //Wszystkie pola zape³nione - remis
}

//Realizacja ruchu jednego gracza
void Ruch(int numer_gracza, char znak_gracza,char *nazwa_gracza)
{
    char tablica[20];//bufor na dane ruchu
    int poprawny_ruch=0;
    int x,y; //wspó³rzêne ruchu

    while(poprawny_ruch==0)
    {
        cout<<nazwa_gracza<<" -> Wprowadz ruch:("<<znak_gracza<<") x0 - ruch komputera";
        cin>>tablica;
        if(strlen(tablica)!=2)
        {
            cout<<"Bledny ruch. Wprowadz dwa znaki";
            continue;
        }
        else if(tablica[0]== 'x' && tablica[1] == '0')
        {
            Skalkuluj_wagi();
            return;
        }
        else if(tablica[0]<'a'||tablica[0]>'a'+rozmiar_planszy)
        {

            cout<<"bledny ruch. pierwsza musi byc litera w zakresie\n";
            continue;//nie wykonuje petli do konca tylko wraca na poczatek
        }
        else if(tablica[1]<'0'||tablica[1]>'0'+rozmiar_planszy)
        {
            cout<<"bledny ruch. Druga musi byc liczba\n";
            continue;
        }
        x=tablica[1]-'0';
        y=tablica[0]-'a'; //jezeli odejmujemy numeryczna reprezentacje wprowadzonego znaku od numerycznej reprezentacji a
        if(plansza[x][y]!='#')
        {
            cout<<"bledny ruch. Pole zajete\n";
        }
        else
        {
            poprawny_ruch=1;
            plansza[x][y]=znak_gracza;
        }

    }
}

int main()
{
    cout<<"Wybierz tryb gry: 1-jednoosobowy lub 2-dwuosobowy: ";
    cin>>tryb;
    while(tryb!=2&&tryb!=1)
    {
        cout<<"wybierz 1 lub 2: ";
        cin>>tryb;

    }
    system("cls");
    if(tryb==2)
    {
        cout<<"Wpisz nazwe gracza nr1: ";
        cin>>gracz_1;
        system("cls");
        cout<<"Wpisz nazwe gracza nr2: ";
        cin>>gracz_2;
        system("cls");
    }
    else if(tryb==1)
    {
        cout<<"Wpisz nazwe gracza: ";
        cin>>gracz_1;
        system("cls");
        strcpy(gracz_2, "Komputer");
    }
    cout<<"Podaj rozmiar planszy: ";
    cin>>rozmiar_planszy;
    system("cls");
    ilosc_do_wygrania=min(rozmiar_planszy,5);

    gracz_aktywny=0;
    znak_aktywny='O';
    nazwa_gracza_aktywnego=gracz_1;
    int koniec_gry=0;
    numer_ruchu=1;
    Wyczysc_ekran();
    Plansza_poczatkowa(rozmiar_planszy);
    //Test_sprawdzania();


    while(koniec_gry==0)
    {
        Wyswietl_plansze(13,4);
        Odswiezenie();
        //     Skalkuluj_wagi();
        Ruch(gracz_aktywny,znak_aktywny,nazwa_gracza_aktywnego);
        numer_ruchu++;
        Wyswietl_plansze(13,4);
        Odswiezenie();

        if (Sprawdz_wygrana(ilosc_do_wygrania) > 0 )
        {
            cout<<"wygral "<<nazwa_gracza_aktywnego<<" !!!!";
            koniec_gry=1;
        }
        if(koniec_gry!=1&&Sprawdz_czy_remis()==1)
        {
            cout<<"Remisssss";
            koniec_gry=1;
        }
        Zmiana_gracza();
    }
    return 0;
}
