#ifndef unter_h
#define unter_h

/*
Strukturen
*/
typedef struct{
    unsigned char ucKennung;    //Kennung "BM"
    int iLaenge;                //Laenge der Datei in Bytes
    int iOffset;                //Offset vom Dateianfang bis zu den Bilddaten
    int iBreit;                 //Bildbreite in Pixel
    int iHoch;                  //Bildhoehe
    unsigned char *pucBDaten;   //Bilddaten
    unsigned char *pucBHead;    //Bildheader
}daten;     //Struktur mit allen Datei Informationen



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


#endif // BILD_H_INCLUDED
