/*
	FastFind 
	    Copyright (c) 2010 - 2013 FastFrench (antispam@laposte.net)

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
*/


#ifndef util_h
#define util_h

#include "stdafx.h" // pre-compiled headers

extern HWND GhWnd;
extern bool GbClientOnly;

extern TCHAR GsErrorMsg[1024];
extern TCHAR GsMsg[1024];
extern int LastFileNameNb;

// Excluded areas
#define NB_EXLUDED_AREA_MAX 1024
extern int GExcludedArea[NB_EXLUDED_AREA_MAX][4];
extern HWND GExcludedAreaWnd[NB_EXLUDED_AREA_MAX];
extern int GnbExludedArea;
bool WINAPI IsExcluded(int x, int y, HWND hWnd);
void WINAPI ResetExcludedAreas();
void WINAPI AddExcludedArea(int x1, int y1, int x2, int y2);
bool IsAnyExclusion();

// Color mngt


// Snapshots
#define NB_SNAP_SHOT_MAX 1024
LPCOLORREF getbits(LPCOLORREF &image_pixel, HBITMAP ahImage, HDC hdc, LONG &aWidth, LONG &aHeight, bool &aIs16Bit, int aMinColorDepth = 8);
class SnapShotData {
public : 
	SnapShotData(); // On ne fait pas dans le d�tail => tout � z�ro � la construction !
	~SnapShotData();
	LPCOLORREF	SnapShotPixels; // Buffer contenant les informations de couleur des pixels
	static bool	bIsScreen16Bits;// Capture d'un �cran ayant 16 bits / pixel
	long        lScreenWidth,   // Nombre de COLORREF par ligne dans le buffer
				lScreenHeight;  // Nombre de lignes dans le buffer
	int			x1, y1, x2, y2; // Limites des zones captur�es - relatives � la zone client ou la fen�tre, selon bClientArea
	bool		bClientArea;
	HWND        hWnd;			// Handle de la fen�tre utilis�e
	int			GetAreaWidth();	
	int			GetAreaHeight();
	long		GetPixelCount();
	LPCOLORREF  GetPixels();    
	void		ReleasePixels(); // Fonction non utilis�e, pr�vue si un m�canisme de verrouillage de la m�moire est utilis� (GlobalAlloc)
	static void AdjustColor(COLORREF &Color);
	static void AdjustColor(int &Color);
	COLORREF SSGetPixel(int x, int y); // retourne la couleur du pixel en coordonn�es relatives � la fen�tre (ou client)
	COLORREF SSGetPixelRelative(int x, int y); // retourne la couleur du pixel en coordonn�es relatives au snapshot
	bool	 IsInSnapShotRelative(int x, int y);

	void SSSetPixel(int x, int y, COLORREF color);
	bool IsInSnapShot(int x, int y);
	POINT TopLeftCapture;
	int GetPixelFromScreen(int x, int y);
	static bool IsSnapShotValid(int NoSnapShot, LPCTSTR sFunctionName=NULL);
	HDC         GetDC();
	void        ReleaseDC(HDC hdc);
	// x et y sont ici relatifs � la zone de capture (et non � l'�cran ou la fen�tre associ�e � la capture)
	bool _IsExcluded(int x, int y);	
};
extern SnapShotData GtSnapShotData[NB_SNAP_SHOT_MAX];

// Configuration
void WINAPI SetDebugMode(int NewMode);
void WINAPI SetHWnd(HWND NewWindowHandle, bool bClientArea);
LPCTSTR WINAPI GetLastErrorMsg();

// Fonctions simples
int WINAPI IsInSnapShot(int X, int Y, int NoSnapShot);

int WINAPI FFGetPixel(int X, int Y, int NoSnapShot);
int WINAPI ColorPixelSearch(int &XRef, int &YRef, int ColorToFind, int NoSnapShot);
int WINAPI ColorPixelSearchRectangle(int &XRef, int &YRef, int StartX, int StartY, int EndX, int EndY, int ColorToFind, bool bVerticalFirst, int NoSnapShot);

// Snapshots
int WINAPI SnapShot(int aLeft, int aTop, int aRight, int aBottom, int NoSnapShot);

// Gestion du tableau des couleurs
int WINAPI AddColor (int NewColor);
int WINAPI RemoveColor (int NewColor);
void WINAPI ResetColors ();

int WINAPI GetPixelFromScreen(int x, int y, int NoSnapShot);
	
// Fonctions de recherche explicitement multi-couleurs
int WINAPI ColorsPixelSearch(int &XRef, int &YRef, int NoSnapShot);
// ColorsSearch est similaire � ColorSearch � un d�tail important pr�s : il recherche non plus UNE mais plusieurs couleurs simultan�ment. 
int WINAPI ColorsSearch(int SizeSearch, int &NbMatchMin, int &XRef, int &YRef, int NoSnapShot);

// Fonctions de recherche la plus g�n�rique (celle qui est appel�e dans la plupart des cas in fine)
int WINAPI GenericColorSearch(int SizeSearch, int &NbMatchMin, int &XRef, int &YRef, int ColorToFind, int ShadeVariation, int NoSnapShot);

// Fonction encore plus archi-puissante
int WINAPI ProgressiveSearch(int SizeSearch, int &NbMatchMin, int NbMatchMax, int &XRef, int &YRef, int ColorToFind/*-1 if several colors*/, int ShadeVariation, int NoSnapShot);

// Fonction de comptage des pixels d'une couleur donn�e
int WINAPI ColorCount(int ColorToFind, int NoSnapShot, int ShadeVariation);

// D�tection des changements
int WINAPI HasChanged(int NoSnapShot, int NoSnapShot2, int ShadeVariation);
int WINAPI LocalizeChanges(int NoSnapShot, int NoSnapShot2, int &xMin, int &yMin, int &xMax, int &yMax, int &nbFound, int ShadeVariation);

LPCWSTR WINAPI GetLastErrorMsg(void);
LPCWSTR WINAPI FFVersion(void);

bool WINAPI SaveBMP(int NoSnapShot, LPCWSTR szFileName /* With no extension (xxx.bmp added)*/);
void WINAPI StartGDIplus();
void WINAPI StopGDIplus();
bool WINAPI SaveJPG(int NoSnapShot, LPCWSTR szFileName /* With no extension*/, ULONG uQuality) ;
int WINAPI GetLastFileSuffix();
bool WINAPI LoadFromFile(int NoSnapShot, LPCWSTR szFileName /* With extension*/);
LPCWSTR WINAPI GetLastFileName();

int WINAPI KeepChanges(int NoSnapShot, int NoSnapShot2, int ShadeVariation);
int WINAPI KeepColor(int NoSnapShot, int ColorToFind, int ShadeVariation);

bool WINAPI DrawSnapShot(int NoSnapShot);
bool WINAPI DrawSnapShotXY(int NoSnapShot, int X, int Y);
bool WINAPI FFSetPixel(int x, int y, int Color, int NoSnapShot);

bool WINAPI DuplicateSnapShot(int Src, int Dst);
int * WINAPI GetRawData(int NoSnapShot, int &NbBytes);

bool WINAPI FindRedCircle(int &x0, int &y0, int NoSnapShot, bool bUnchangedAreaInBlack);
bool WINAPI FindBlueCircle(int &x0, int &y0, int NoSnapShot, bool bUnchangedAreaInBlack);
void WINAPI setDofusDistanceMode(bool bDofusDistance);
int Distance(int dx, int dy);

int WINAPI ComputeMeanValues(int NoSnapShot, int &MeanRed, int &MeanGreen, int &MeanBlue);

int WINAPI ApplyFilterOnSnapShot(int NoSnapShot, int Red, int Green, int Blue);

#endif

