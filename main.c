/*bild.c*/

/*
Projekt: bild
*/

/*
History:
	FS 27.12.2015 erstellt
*/


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <fcntl.h>
#include "bild.h"


/*
Parametertyplisten
*/
int Einlesen(daten *ptDat /*Daten des Bildes*/); //Parametertypliste
/*Rueckgabewert: Ergebnis*/
/*Die Funktion liesst die Daten des Bildes ein*/
/*
History:
    FS 19.08.2015 erstellt
*/

unsigned char Gradient(int iX /*Variable fuer X-Koordinaten*/, int iY /*Variable fuer Y*/, int iBreit /*Breite*/, unsigned char **ppucBDat /*Bilddaten*/); //Parametertypliste
/*Rueckgabewert: Ergebnis*/
/*Die Funktion berechnet die Gradienten*/
/*
History:
    FS 19.08.2015 erstellt
*/

void Ausgabe(char cName[35] /*Name der Datei*/, daten tDaten /*Struktur der Bilddaten*/, unsigned char *ppucG /*Gradienten*/); //Parametertypliste
/*Rueckgabewert: void*/
/*Die Funktion erstellt die Ergebnissbilder*/
/*
History:
    FS 27.12.2015 erstellt
*/


int main(void);
int main()
{
    /*
    Dieses Modul identifiziert den Rand eines Graustufenbildes,
    welches ein beliebiges Objekt darstellt.
    Anschlieﬂend werden die Gradienten des Bildes als Falsch-
    farbenbild und Randbild dargestellt.
    */

    /*
    Variablen deklarationen
    */
    int iZ;                 //Zaehlvariable
    int iZ2;                //Zaehlvariable
    int iDatei;             //Handle auf Datenkanal
    daten tDaten;           //Daten des Bildes vom Typ "daten"
    unsigned char *pucG;    //Gradienten




    /*
    Main
    */
        /*Einlesen*/
        iDatei = Einlesen(&tDaten);      //Funktionsaufruf zum Einlesen des Bildes

        if(iDatei == -1)                 //Sicherrung
        {
            printf("Fehler beim Einlesen - Datei wohlmoeglich nicht vorhanden!\n");
            return 0;
        }

            /*Testausgabe
            printf("Die Kennung: %c\n", tDaten.ucKennung);
            printf("Die Laenge: %d\n", tDaten.iLaenge);
            printf("Der Offset: %d\n", tDaten.iOffset);
            printf("Die Breite: %d\n", tDaten.iBreit);
            printf("Die Hoehe: %d\n", tDaten.iHoch);
            for(iZ = 0; iZ < (tDaten.iLaenge - tDaten.iOffset); iZ = iZ +1)
            {
                printf("%d ", (int) *(tDaten.pucBDaten + iZ)); //Bilddaten
            }*/





        /*Gradienten berechen*/
        pucG = malloc(tDaten.iBreit * tDaten.iHoch * (sizeof(unsigned char)));      //Groesse der Gradienten bestimmen

        for(iZ2 = 0; iZ2 < (tDaten.iHoch - 1); iZ2 = iZ2 +1)
        {
            for(iZ = 0; iZ < (tDaten.iBreit - 1); iZ = iZ +1)
            {
                *(pucG + (iZ2 * tDaten.iBreit + iZ)) = Gradient(iZ, iZ2, tDaten.iBreit, &tDaten.pucBDaten); //Funktionsaufruf zum Gradienten berechenen
            }
        }




        /*Ausgabe als Falschfarbenbild*/
        Ausgabe("Falschfarbenbild.bmp", tDaten, pucG);




        /*Rauschen und Satellitenpunkte entfernen*/
        for(iZ2 = 0; iZ2 < (tDaten.iHoch - 1); iZ2 = iZ2 +1)
        {
            for(iZ = 0; iZ < (tDaten.iBreit - 1); iZ = iZ +1)
            {
                if(*(pucG + (iZ2 * tDaten.iBreit + iZ)) > 40)       //Wenn der Gradienten Wert groesser als 40 ist
                {
                    *(pucG + (iZ2 * tDaten.iBreit + iZ)) = 255;     //Sonst weiss
                }
                else
                {
                    *(pucG + (iZ2 * tDaten.iBreit + iZ)) = 0;       //Sonst schwarz
                }
            }
        }




        /*Ausgabe als Randbild*/
        Ausgabe("Randbild.bmp", tDaten, pucG);




    return 0;
}




/*
Funktionen
*/
int Einlesen(daten *ptDat /*Daten des Bildes*/) //Funktion
{
    int iDatei;       //Handle auf Dateikanal

    iDatei = open("bild.bmp", O_RDONLY);     //Rueckgabewert Handle

    read(iDatei, &(*ptDat).ucKennung, 2);  //Einlesen der Kennung

    lseek(iDatei, 2, SEEK_SET);            //Dateizeiger auf das Byte der Laengenangebe manipulieren
    read(iDatei, &(*ptDat).iLaenge, 4);    //Einlesen der Laenge

    lseek(iDatei, 10, SEEK_SET);           //Dateizeiger auf das Byte des Offsets manipulieren
    read(iDatei, &(*ptDat).iOffset, 4);    //Einlesen des Offsets

    lseek(iDatei, 0, SEEK_SET);            //Dateizeiger auf das Byte des Headers manipulieren
    (*ptDat).pucBHead = malloc((*ptDat).iOffset * (sizeof(unsigned char)));   //Groesse definieren
    read(iDatei, (*ptDat).pucBHead, (*ptDat).iOffset); //Einlesen des Headers

    lseek(iDatei, 18, SEEK_SET);           //Dateizeiger auf das Byte der Breite manipulieren
    read(iDatei, &(*ptDat).iBreit, 4);     //Einlesen der Breite

    lseek(iDatei, 22, SEEK_SET);           //Dateizeiger auf das Byte der Hoehe manipulieren
    read(iDatei, &(*ptDat).iHoch, 4);      //Einlesen der Hoehe

    lseek(iDatei, (*ptDat).iOffset, SEEK_SET);         //Dateizeiger auf das Byte der Bilddaten manipulieren
    (*ptDat).pucBDaten = malloc((*ptDat).iBreit * (*ptDat).iHoch * (sizeof(unsigned char)));    //Groesse definieren
    read(iDatei, (*ptDat).pucBDaten, ((*ptDat).iLaenge - (*ptDat).iOffset)); //Einlese der Bilddaten

    close(iDatei);      //Handle schliessen

    return(iDatei);
}


unsigned char Gradient(int iX /*Variable fuer X-Koordinaten*/, int iY /*Variable fuer Y*/, int iBreit /*Breite*/, unsigned char **ppucBDat /*Bilddaten*/) //Funktion
{
    unsigned char ucX;          //Hilfsvariable
    unsigned char ucY;          //Hilfsvariable
    unsigned char ucZ;          //Hilfsvariable
    unsigned char ucGrad;       //Ergebniss

    ucX = *(*ppucBDat + (iY * iBreit + iX + 1));
    ucY = *(*ppucBDat + ((iY + 1) * iBreit + iX));
    ucZ = *(*ppucBDat + (iY * iBreit + iX));

    ucGrad = (unsigned char) sqrt(((ucZ - ucX) * (ucZ - ucX) + ((ucZ - ucY) * (ucZ - ucY))));
    ucGrad = (unsigned char)lround((double)ucGrad / (double)sqrt(2));

    return(ucGrad);     //Rueckgabe
}


void Ausgabe(char cName[35] /*Name der Datei*/, daten tDaten /*Struktur der Bilddaten*/, unsigned char *ppucG /*Gradienten*/) //Funktion
{
    int iDatei;         //Handle auf Datenkanal


    iDatei = open(cName, O_CREAT);  //Bild erstellen
    close(iDatei);      //Handle schliessen

    SetFileAttributes(cName, FILE_ATTRIBUTE_NORMAL);
    iDatei = open(cName, O_WRONLY | O_BINARY);      //Bild oeffnen

    lseek(iDatei, 0, SEEK_SET);     //Auf Anfang manipulieren
    write(iDatei, tDaten.pucBHead, tDaten.iOffset); //Schreiben des Headers
    write(iDatei, ppucG, tDaten.iLaenge);           //Schreiben der Gradienten
    close(iDatei);      //Handle schliessen

    printf("%s erstellt\n", cName);
}
