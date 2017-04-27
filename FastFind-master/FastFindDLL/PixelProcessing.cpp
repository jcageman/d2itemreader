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

#include "stdafx.h" // pre-compiled headers

// Exclusion area management
// =========================

// On utilise de bons vieux tableaux statiques globaux, plus simple et performant pour une dll
// Todo : g�rer des listes sp�cifiques pour chaque fen�tre, afin d'�viter les interd�pendances 
int GExcludedArea[NB_EXLUDED_AREA_MAX][4];
HWND GExcludedAreaWnd[NB_EXLUDED_AREA_MAX];
int GnbExludedArea = 0;

void WINAPI AddExcludedArea(int x1, int y1, int x2, int y2)
{
	if (GnbExludedArea >= NB_EXLUDED_AREA_MAX) return;
	GExcludedArea[GnbExludedArea][0] = x1;
	GExcludedArea[GnbExludedArea][1] = y1;
	GExcludedArea[GnbExludedArea][2] = x2;
	GExcludedArea[GnbExludedArea][3] = y2;
	GExcludedAreaWnd[GnbExludedArea++] = GhWnd;
	if (Tracer.TextDebug())
		Tracer.Format(DEBUG_STREAM_SYSTEM, _T("AddExcludedArea (%d,%d,%d,%d) => %d rect known\n"), x1, y1, x2, y2, GnbExludedArea);
}

void WINAPI ResetExcludedAreas()
{
	if (Tracer.TextDebug())
		Tracer.Format(DEBUG_STREAM_SYSTEM, _T("The %d exclusion rectangles are removed\n"), GnbExludedArea);		
	GnbExludedArea = 0;
}

bool IsAnyExclusion() {return GnbExludedArea>0;}

bool WINAPI IsExcluded(int x, int y, HWND hWnd)
{
	for (int i=0; i<GnbExludedArea; i++)
		if (GExcludedAreaWnd[i] == hWnd)
			if ( (x >= GExcludedArea[i][0]) &&
				 (y >= GExcludedArea[i][1]) &&
				 (x <= GExcludedArea[i][2]) &&
				 (y <= GExcludedArea[i][3]) )
			{
#ifdef MYTRACE
				if (Tracer.TextDebug())
					Tracer.Format(DEBUG_STREAM_SYSTEM_DETAIL, _T("IsExcluded(%d,%d,%8X) : inside Area(%d,%d,%d,%d) => returns true\n"), x, y, hWnd, GExcludedArea[i][0], GExcludedArea[i][1], GExcludedArea[i][2], GExcludedArea[i][3]);		
#endif
				return true;
			}
#ifdef MYTRACE
	if (Tracer.TextDebug())
		Tracer.Format(DEBUG_STREAM_SYSTEM_DETAIL, _T("IsExcluded(%d,%d,%8X) : not inside any of the %d Exluded Areas => returns false\n"), x, y, hWnd, GnbExludedArea);		
#endif
	return false;
}

// Window Management
// =================
HWND GhWnd = GetDesktopWindow(); // Par d�faut, on travaille sur tout l'�cran
bool GbClientOnly = true;

// Fonction permettant de s�lectionner la fen�tre courante
void WINAPI SetHWnd(HWND NewWindowHandle, bool bClientOnly) {	
	if (NewWindowHandle==0) 
		{
			if (GhWnd == GetDesktopWindow()) return;
			GhWnd = GetDesktopWindow(); 
			GbClientOnly = true;
		}
	else 
		{
			GhWnd = NewWindowHandle;
			GbClientOnly = bClientOnly;
		}
	if (Tracer.GraphicDebug()) { 
		HDC dc = GetDC(0);
		HGDIOBJ hOldPen = ::SelectObject( dc, ::GetStockObject(DC_PEN));
		::SetDCPenColor(dc, RGB(0,255,0));
		HGDIOBJ hOldBrush = ::SelectObject(dc, ::GetStockObject(NULL_BRUSH));
		RECT rect; 
		if (GbClientOnly) {
			::GetClientRect(GhWnd, &rect);
			::ClientToScreen(GhWnd, (LPPOINT)(&rect.left));   // Convert left+top first
			::ClientToScreen(GhWnd, (LPPOINT)(&rect.right));  // Then convert right+bottom
		}
		else
			::GetWindowRect(GhWnd, &rect);
		::Rectangle(dc, rect.left, rect.top, rect.right, rect.bottom);
		::SelectObject( dc, hOldPen);
		::SelectObject(dc, hOldBrush);
		ReleaseDC(0, dc);
		}
#ifdef MYTRACE
	if (Tracer.TextDebug()) {
		RECT rParent;
		if (GbClientOnly) {
			::GetClientRect(GhWnd, &rParent);
			::ClientToScreen(GhWnd, (LPPOINT)(&rParent.left));   // Convert left+top first
			::ClientToScreen(GhWnd, (LPPOINT)(&rParent.right));  // Then convert right+bottom
		}
		else
			::GetWindowRect(GhWnd, &rParent);
	
		TCHAR sTitle[128];
		::GetWindowText(GhWnd, sTitle, 128);
		Tracer.Format(DEBUG_MB_SYSTEM, _T("SetHWnd(%8X) => Window position (Title:%s) = (%d, %d, %d, %d)\n"), GhWnd, sTitle, rParent.left, rParent.top, rParent.right, rParent.bottom);	
		}
#endif
	}

// Gestion des couleurs pour ColorsSearch 
// ======================================
#define MAX_NB_COLORS 1024
int NbColors = 0;
COLORREF TabColors[MAX_NB_COLORS]; // On autorise jusqu'� MAX_NB_COLORS couleurs simultan�es

// Ajoute une couleur � la liste
int WINAPI AddColor (int NewColor)
{
	if (NbColors>=MAX_NB_COLORS) return false;
	SnapShotData::AdjustColor(NewColor);	
	TabColors[NbColors++] = NewColor & 0x00FFFFFF;
#ifdef MYTRACE
	Tracer.Format(DEBUG_STREAM_SYSTEM, _T("AddColor(%08X) => %d Colors in the list\n"), NewColor, NbColors);
#endif
	return true;
}

// Supprime une couleur dans le tableau (si elle existe)
int WINAPI RemoveColor (int NewColor)
{
	SnapShotData::AdjustColor(NewColor);	
	for (int i=0; i<NbColors; i++)
		if (TabColors[i]==NewColor)	{
			if (i<(NbColors-1)) 
				TabColors[i] = TabColors[NbColors-1];
			NbColors--;
#ifdef MYTRACE
			Tracer.Format(DEBUG_STREAM_SYSTEM, _T("RemoveColor(%08X) - color found, %d left\n"), NewColor, NbColors);
#endif
			return true;
			}
	return false; // Can't find the color
}

// Supprime toutes les couleurs de la liste
void WINAPI ResetColors ()
{
	NbColors = 0;
#ifdef MYTRACE
	Tracer.Format(DEBUG_STREAM_SYSTEM, _T("ResetColors() => No Colors in the list\n"));
#endif
}

/*
  ShadeVariation - Gestion des "Shade Variation" (tol�rance sur les couleurs)
  ==============
 */
_inline bool IsInShadeVariation(int PixelColor, int ColorToFind, int ShadeVariation)  {
	       if (ShadeVariation <= 0) return PixelColor == ColorToFind;
		   return  (abs(((int)PixelColor & 0x00FF0000) - ((int)ColorToFind & 0x00FF0000)) >> 16 <=  ShadeVariation) && 
				   (abs(((int)PixelColor & 0x0000FF00) - ((int)ColorToFind & 0x0000FF00)) >> 8 <=  ShadeVariation) && 
			       (abs(((int)PixelColor & 0x000000FF) - ((int)ColorToFind & 0x000000FF)) <=  ShadeVariation);
}

int GlobalI; bool GlobalFound;
_inline bool FindIt(COLORREF PixelToCheck) {
	for (GlobalI=0; GlobalI<NbColors; GlobalI++) 
		if (TabColors[GlobalI]==PixelToCheck) return true; 
	return false;
}

// Helper pour estimer si un pixel est d'une couleur ad�quate, utilis� uniquement lorsque la performance n'est pas critique (pour simplifier le code)
_inline bool FindIt(COLORREF PixelToCheck, int ShadeVariation) {
	for (GlobalI=0; GlobalI<NbColors; GlobalI++) 
		if (IsInShadeVariation(TabColors[GlobalI],PixelToCheck, ShadeVariation)) return true; 
	return false;
}

// Helper pour estimer si un pixel est d'une couleur ad�quat, utilis� uniquement lorsque la performance n'est pas critique (pour simplifier le code)
// Prend en compte la Liste de couleurs et le Shade variation
_inline bool GenericColorChecker(COLORREF PixelToCheck, int ColorToFind, int ShadeVariation) {
	if (ColorToFind==-1) 
		if (ShadeVariation==0)
			return FindIt(PixelToCheck);
		else
			return FindIt(PixelToCheck, ShadeVariation);
	else
		//if (ShadeVariation==0)
		//	return PixelToCheck==ColorToFind;
		//else
			return IsInShadeVariation(PixelToCheck, ColorToFind, ShadeVariation);
}

// Routine de test et d'identification de la DLL
int * WINAPI FFTest()
{
	int *tab = new int[5];
	tab[0] =     4;
	tab[1] =   545;
	tab[2] =   777;
	tab[3] =   888;
	tab[4] = 12000;
	return tab;
}

// D�tection rapide d'un changement entre deux �crans (sans pr�ciser l'ampleur et la localisation des changements)
int WINAPI HasChanged(int NoSnapShot, int NoSnapShot2, int ShadeVariation)
	// Author: HasChanged By FastFrench
{	
	if (!SnapShotData::IsSnapShotValid(NoSnapShot, _T("HasChanged (1)"))) return 0;	
	if (!SnapShotData::IsSnapShotValid(NoSnapShot2, _T("HasChanged (2)"))) return 0;	
	SnapShotData snapShot = GtSnapShotData[NoSnapShot];
	SnapShotData snapShot2 = GtSnapShotData[NoSnapShot2];
	if ( snapShot.lScreenWidth == 0 ) return 0;

	if ( snapShot.x1 != snapShot2.x1 ||
		 snapShot.x2 != snapShot2.x2 ||
		 snapShot.y1 != snapShot2.y1 ||
		 snapShot.y2 != snapShot2.y2 )
	{
#ifdef MYTRACE
		Tracer.Format(DEBUG_SYSTEM_ERROR, _T("HasChanged : Position or size of the two SnapShots is not the same\n"));
#endif
		return 0;
	}

#ifdef MYTRACE
	GChrono.Restart();
#endif
	bool bFound = false;
	LPCOLORREF	screen_pixel1 = snapShot.SnapShotPixels,
				screen_pixel2 = snapShot2.SnapShotPixels;
	LONG screen_pixel_count = snapShot.GetPixelCount();
#ifdef MYTRACE
	Tracer.Format(DEBUG_STREAM_SYSTEM, _T("HasChanged(%d, %d) %ld pixels � traiter\n"), NoSnapShot, NoSnapShot2, screen_pixel_count);
#endif		
	bool bIsAnyExclusion = IsAnyExclusion();
	
	for (int i=0; i<screen_pixel_count; i++)
		if (!IsInShadeVariation(screen_pixel1[i], screen_pixel2[i], ShadeVariation))  { // Difference detected
			int x = i % snapShot.lScreenWidth;
			int y = i / snapShot.lScreenWidth;
			if (bIsAnyExclusion && snapShot._IsExcluded(x, y)) continue; // Dans une zone d'exclusion => on ignore ce changement
			
			if (Tracer.GraphicDebug()) { // Si debuggage visuel, on affiche � l'�cran la zone trouv�e, le point renvoy� et le point de r�f�rence fourni
				HDC hdc = snapShot.GetDC();				
				if (hdc) {
					HGDIOBJ hOldPen = ::SelectObject( hdc, ::GetStockObject(DC_PEN));
					::SetDCPenColor(hdc, RGB(255,0,0));
					HGDIOBJ hOldBrush = ::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));
					::Ellipse( hdc, // Avec un petit rond rouge sur le point renvoy�
								x -6,
								y -6,
								x +6,
								y +6);
					::SelectObject( hdc, hOldPen);
					::SelectObject(hdc, hOldBrush);
					snapShot.ReleaseDC(hdc);
					}
				}
#ifdef MYTRACE
			Tracer.Format(DEBUG_MB_SYSTEM, _T("HasChanged : Some changes detected\nFirst difference in %d,%d\n\tIts color changed from %6x to %6x\n\nProcessing time: %s\n"), x, y, screen_pixel1[i], screen_pixel2[i], GChrono.GetTime());
#endif
			return true;
			}
#ifdef MYTRACE
	Tracer.Format(DEBUG_MB_SYSTEM, _T("HasChanged : NO Change detected\n\nProcessing time: %s\n"), GChrono.GetTime());
#endif			
	return false;
	}

// Cette fonction d�tecte si il y eu des changements entre deux SnapShots, 
// et pr�cise la zone o� se sont produit ces changements ainsi que le nombre de pixels affect�s.
int WINAPI LocalizeChanges(int NoSnapShot, int NoSnapShot2, int &xMin, int &yMin, int &xMax, int &yMax, int &nbFound, int ShadeVariation)
	// Author: LocalizeChanges By FastFrench
{
	
	if (!SnapShotData::IsSnapShotValid(NoSnapShot, _T("DetectChanges (1)"))) return 0;	
	if (!SnapShotData::IsSnapShotValid(NoSnapShot2, _T("DetectChanges (2)"))) return 0;	
	SnapShotData snapShot = GtSnapShotData[NoSnapShot];
	SnapShotData snapShot2 = GtSnapShotData[NoSnapShot2];
	
	if (GtSnapShotData[NoSnapShot].x1   != GtSnapShotData[NoSnapShot2].x1 ||
		GtSnapShotData[NoSnapShot].x2   != GtSnapShotData[NoSnapShot2].x2 ||
		GtSnapShotData[NoSnapShot].y1   != GtSnapShotData[NoSnapShot2].y1 ||
		GtSnapShotData[NoSnapShot].y2   != GtSnapShotData[NoSnapShot2].y2 || 
		GtSnapShotData[NoSnapShot].hWnd != GtSnapShotData[NoSnapShot2].hWnd )
	{
#ifdef MYTRACE
		Tracer.Format(DEBUG_SYSTEM_ERROR, _T("LocalizeChanges : The two SnapShots are not on the same area or Window\n"));
#endif
		return 0;
	}
#ifdef MYTRACE
	GChrono.Restart();
#endif
	xMin = xMax = yMin = yMax = 0;
	nbFound = 0; 
	LPCOLORREF	screen_pixel1 = GtSnapShotData[NoSnapShot].SnapShotPixels,
				screen_pixel2 = GtSnapShotData[NoSnapShot2].SnapShotPixels;
	LONG	BufferWidth = GtSnapShotData[NoSnapShot].lScreenWidth;
	LONG screen_pixel_count = GtSnapShotData[NoSnapShot].GetPixelCount();
	if (BufferWidth == 0) return 0;
#ifdef MYTRACE
	Tracer.Format(DEBUG_STREAM_SYSTEM, _T("LocalizeChanges(%d, %d) %ld pixels to process\n"), NoSnapShot, NoSnapShot2, screen_pixel_count);
#endif	
	int  x, y;
	bool bFound = false; 
	HDC hdc=0;
	
	for (int i=0; i<screen_pixel_count; i++)
		if ( !IsInShadeVariation(screen_pixel1[i], screen_pixel2[i], ShadeVariation) )
			{ // Difference detected
			x = i % BufferWidth;
			y = i / BufferWidth;
			if (GtSnapShotData[NoSnapShot]._IsExcluded(x, y)) continue;
			if (!bFound) {
				xMin = xMax = x;
				yMin = yMax = y;
				bFound = true;
				}
			else {
				if (x>xMax) xMax = x;
				else 
					if (x<xMin) xMin = x;
				if (y>yMax) yMax = y;
				else 
					if (y<yMin) yMin = y;
			}
			
			nbFound++;
		} 

	if (bFound) { // On recadre le r�sultat, dans les coordonn�es de l'�cran
		xMin += GtSnapShotData[NoSnapShot].x1;
		yMin += GtSnapShotData[NoSnapShot].y1;
		xMax += GtSnapShotData[NoSnapShot].x1;
		yMax += GtSnapShotData[NoSnapShot].y1;
		}
	if (Tracer.GraphicDebug() && bFound) { // Si debuggage visuel, on affiche � l'�cran la zone trouv�e, le point renvoy� et le point de r�f�rence fourni
		hdc = GtSnapShotData[NoSnapShot].GetDC();
		if (hdc) {
			HGDIOBJ hOldPen = ::SelectObject( hdc, ::GetStockObject(DC_PEN));
			::SetDCPenColor(hdc, RGB(0,0,255));
			HGDIOBJ hOldBrush = ::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));
			::Rectangle(hdc, xMin, yMin, xMax, yMax); // Affichage du rectangle
			::SelectObject( hdc, hOldPen);
			::SelectObject( hdc, hOldBrush);
			GtSnapShotData[NoSnapShot].ReleaseDC(hdc);	
		}
#ifdef MYTRACE
		else
			Tracer.Format(DEBUG_STREAM_SYSTEM, _T("LocalizeChanges(%d, %d) : GetDC(%8X) failed\n"), NoSnapShot, NoSnapShot2, GtSnapShotData[NoSnapShot].hWnd);	
#endif
	}

	if (bFound) {
#ifdef MYTRACE
		Tracer.Format(DEBUG_MB_SYSTEM, _T("LocalizeChanges : %d pixels have changed in the rectangle [%d, %d, %d, %d]\n\nProcessing time : %s\n"), nbFound, xMin, yMin, xMax, yMax, GChrono.GetTime());
#endif
		return 1;
	}
#ifdef MYTRACE
	Tracer.Format(DEBUG_MB_SYSTEM, _T("LocalizeChanges : No changes detected\n"));
#endif	
	return 0; // 0 = aucun changement d�tect�
}

// Cette fonction change le contenu du 1er SnapShot en fonction des changements d�tect�s vs le 2nd
// Chaque pixel diff�rent prend la couleur du SnapShot N�2, les pixels identiques deviennent noir. 
int WINAPI KeepChanges(int NoSnapShot, int NoSnapShot2, int ShadeVariation)
	// Author: LocalizeChanges By FastFrench
{	
	if (!SnapShotData::IsSnapShotValid(NoSnapShot, _T("KeepChanges (1)"))) return 0;	
	if (!SnapShotData::IsSnapShotValid(NoSnapShot2, _T("KeepChanges (2)"))) return 0;	
	if (GtSnapShotData[NoSnapShot].x1   != GtSnapShotData[NoSnapShot2].x1 ||
		GtSnapShotData[NoSnapShot].x2   != GtSnapShotData[NoSnapShot2].x2 ||
		GtSnapShotData[NoSnapShot].y1   != GtSnapShotData[NoSnapShot2].y1 ||
		GtSnapShotData[NoSnapShot].y2   != GtSnapShotData[NoSnapShot2].y2 || 
		GtSnapShotData[NoSnapShot].hWnd != GtSnapShotData[NoSnapShot2].hWnd )
	{
#ifdef MYTRACE
		Tracer.Format(DEBUG_SYSTEM_ERROR, _T("LocalizeChanges : The two SnapShots are not on the same area or Window\n"));
#endif
		return 0;
	}
#ifdef MYTRACE
	GChrono.Restart();
#endif
	int nbFound = 0; 
	LPCOLORREF	screen_pixel1 = GtSnapShotData[NoSnapShot].SnapShotPixels,
				screen_pixel2 = GtSnapShotData[NoSnapShot2].SnapShotPixels;
	LONG screen_pixel_count = GtSnapShotData[NoSnapShot].GetPixelCount();
#ifdef MYTRACE
	Tracer.Format(DEBUG_STREAM_SYSTEM, _T("KeepChanges(%d, %d) %ld pixels to process\n"), NoSnapShot, NoSnapShot2, screen_pixel_count);
#endif	
	bool bFound = false; 
	HDC hdc=0;
	
	for (int i=0; i<screen_pixel_count; i++)
		if (IsInShadeVariation(screen_pixel1[i], screen_pixel2[i], ShadeVariation)) 
			screen_pixel1[i] = 0;
		else
		{
			screen_pixel1[i] = screen_pixel2[i];
			nbFound++;
		}

	return nbFound; // 0 = aucun changement d�tect�
}


// Cette fonction analyse un SnapShot et ne conserve que les couleurs valides (couleur unique ou liste + 
// ShadeVariation. Ignore les rectangles d'exclusion)
// Retourne le nombre de couleurs trouv�es
int WINAPI KeepColor(int NoSnapShot, int ColorToFind, int ShadeVariation)
	// Author: LocalizeChanges By FastFrench
{	
	if (!SnapShotData::IsSnapShotValid(NoSnapShot, _T("KeepColor"))) return 0;	
	
	int nbFound = 0; 
	LPCOLORREF	screen_pixel1 = GtSnapShotData[NoSnapShot].SnapShotPixels;

	LONG	BufferWidth = GtSnapShotData[NoSnapShot].lScreenWidth;
	LONG screen_pixel_count = GtSnapShotData[NoSnapShot].GetPixelCount();
	bool bFound = false; 
	HDC hdc=0;
	
	for (int i=0; i<screen_pixel_count; i++)
		if (GenericColorChecker(screen_pixel1[i], ColorToFind, ShadeVariation))
			nbFound++;
		else
			screen_pixel1[i] = 0; 

	return nbFound; // 0 = aucun changement d�tect�
}



int WINAPI IsInSnapShot(int X, int Y, int NoSnapShot) 
{
	if (SnapShotData::IsSnapShotValid(NoSnapShot, _T("IsInSnapShot")))
		return GtSnapShotData[NoSnapShot].IsInSnapShot(X,Y);
	return false;
}

// Renvoie la couleur d'un pixel dans un SnapShot. Equivalent � PixelGetColor, en beaucoup plus rapide.
// Particuli�rement int�ressant lorsque vous avez beaucoup d'appels � PixelGetColor pour un seul SnapShot()
int WINAPI FFGetPixel(int X, int Y, int NoSnapShot) {
	//char msg[256];
	//sprintf(msg,"Entr�e dans GetPixel(%d,%d, %d)\n", X, Y, NoSnapShot);
	//Tracer.Format(DEBUG_STREAM_SYSTEM, msg);		
	//MessageBox(NULL, msg, L"GetPixel", MB_OK);
	
	if (!SnapShotData::IsSnapShotValid(NoSnapShot, _T("FFGetPixel")) || !GtSnapShotData[NoSnapShot].IsInSnapShot(X,Y)) 
		{
			//if (Tracer.TextDebug()) {
				if (!SnapShotData::IsSnapShotValid(NoSnapShot, _T("FFGetPixel")))  // => SnapShot incorrect
					Tracer.Format(DEBUG_STREAM_SYSTEM, _T("FFGetPixel(x:%d, y:%d, Snap:%d): SnapShot N�%d is not valid => returns -1\n"), X, Y, NoSnapShot, NoSnapShot);		
				else
					Tracer.Format(DEBUG_STREAM_SYSTEM, _T("FFGetPixel(x:%d, y:%d, Snap:%d): this point is outside the SnapShot boundaries (%d,%d,%d,%d) => returns -1\n"), X, Y, NoSnapShot, GtSnapShotData[NoSnapShot].x1, GtSnapShotData[NoSnapShot].y1, GtSnapShotData[NoSnapShot].x2, GtSnapShotData[NoSnapShot].y2);		
			//}	
		return -1;	
		}
	return GtSnapShotData[NoSnapShot].SSGetPixel(X, Y);
}

// Renvoie la couleur d'un pixel dans un SnapShot. Equivalent � PixelGetColor, en beaucoup plus rapide.
// Particuli�rement int�ressant lorsque vous avez beaucoup d'appels � PixelGetColor pour un seul SnapShot()
bool WINAPI FFSetPixel(int X, int Y, int Color, int NoSnapShot) {
	//char msg[256];
	//sprintf(msg,"Entr�e dans GetPixel(%d,%d, %d)\n", X, Y, NoSnapShot);
	//Tracer.Format(DEBUG_STREAM_SYSTEM, msg);		
	//MessageBox(NULL, msg, L"GetPixel", MB_OK);
	
	if (!SnapShotData::IsSnapShotValid(NoSnapShot, _T("FFSetPixel")) || !GtSnapShotData[NoSnapShot].IsInSnapShot(X,Y)) 
		{
			//if (Tracer.TextDebug()) {
				if (!SnapShotData::IsSnapShotValid(NoSnapShot, _T("FFSetPixel")))  // => SnapShot incorrect
					Tracer.Format(DEBUG_STREAM_SYSTEM, _T("FFSetPixel(x:%d, y:%d, Color:0x%06X, Snap:%d): SnapShot N�%d is not valid => returns -1\n"), X, Y, Color, NoSnapShot, NoSnapShot);		
				else
					Tracer.Format(DEBUG_STREAM_SYSTEM, _T("FFSetPixel(x:%d, y:%d, Color:0x%06X, Snap:%d): this point is outside the SnapShot boundaries (%d,%d,%d,%d) => returns -1\n"), X, Y, Color, NoSnapShot, GtSnapShotData[NoSnapShot].x1, GtSnapShotData[NoSnapShot].y1, GtSnapShotData[NoSnapShot].x2, GtSnapShotData[NoSnapShot].y2);		
			//}	
		return false;	
		}
	GtSnapShotData[NoSnapShot].SSSetPixel(X, Y, Color);
	return true;
}


// ColorPixelSearch est une am�lioration de PixelSearch : 
//   au lieu de renvoyer le premier trouv� correspondant � une couleur donn�e, 
//   il recherche le pixel le plus proche d'un point pass� en param�tre. 
// La couleur � trouver peut �tre relativis�e � une distance max de ShadeVariation sur chaque composant R, G et B (fonctionnalit� � venir)
int WINAPI ColorPixelSearch(int &XRef, int &YRef, int ColorToFind, int NoSnapShot) {
	if (!SnapShotData::IsSnapShotValid(NoSnapShot, _T("ColorPixelSearch"))) return 0;	
	int NbMatchMin = 1;
	return GenericColorSearch(1, NbMatchMin, XRef, YRef, ColorToFind, 0, NoSnapShot);
}

int WINAPI ColorPixelSearchRectangle(int &XRef, int &YRef, int StartX, int StartY, int EndX, int EndY, int ColorToFind, bool bVerticalFirst, int NoSnapShot)
{
	if (!SnapShotData::IsSnapShotValid(NoSnapShot, _T("ColorPixelSearchRectangle"))) return 0;
	Tracer.Format(DEBUG_STREAM_SYSTEM, _T("ColorPixelSearchRectangle(Startx:%d Starty:%d Endx:%d EndY:%d\n"), StartX, StartY, EndX, EndY, ColorToFind);
	GtSnapShotData[NoSnapShot].AdjustColor(ColorToFind);
	if (bVerticalFirst)
	{
		if (StartX < EndX)
		{
			for (int xpos = StartX; xpos <= EndX; xpos++)
			{
				for (int ypos = StartY; ypos <= EndY; ypos++)
				{
					if (GtSnapShotData[NoSnapShot].SSGetPixelRelative(xpos, ypos) == ColorToFind)
					{
						XRef = xpos;
						YRef = ypos;
						return 1;
					}
				}
			}
		}
		else
		{
			for (int xpos = StartX; xpos >= EndX; xpos--)
			{
				for (int ypos = StartY; ypos <= EndY; ypos++)
				{
					if (GtSnapShotData[NoSnapShot].SSGetPixelRelative(xpos, ypos) == ColorToFind)
					{
						XRef = xpos;
						YRef = ypos;
						return 1;
					}
				}
			}
		}
	}
	else
	{
		for (int ypos = StartY; ypos <= EndY; ypos++)
		{
			for (int xpos = StartX; xpos <= EndX; xpos++)
			{
				Tracer.Format(DEBUG_STREAM_SYSTEM, _T("ColorPixelSearchRectangle(Startx:%d Starty:%d found color:%d search color:%d\n"), xpos, ypos, GtSnapShotData[NoSnapShot].SSGetPixelRelative(xpos, ypos), ColorToFind);
				if (GtSnapShotData[NoSnapShot].SSGetPixelRelative(xpos, ypos) == ColorToFind)
				{
					XRef = xpos;
					YRef = ypos;
					return 1;
				}
			}
		}
	}

	return 0;
}

bool GbDofusDistance = false;

// Calcul une valeur rendant compte de la distance (fonction croissante en fonction de la distance)
// Cas 1 : carr� de la distance 
//#define Distance(dx, dy) ((dx) * (dx) + (dy) * (dy))
// Cas 2 : distance Dofus. Dx +1 correspond � +37 pour dx, et -18.5 pour dy. Dy +1 correspond � -37 pour dx, et -18.5 pour dy.
_inline int Distance(int dx, int dy)
{
	if (GbDofusDistance)
		return abs(dy *2 - dx) + abs(dy *2 + dx); // Distance Dofus (Manhattan + rep�re iso)
	else
		return ((dx) * (dx) + (dy) * (dy)); // => distance usuelle (Euclidienne)
}



// ColorSearch est l'extension de ColorPixelSearch � toute une zone (spot) : 
//   il s'agit de trouver parmi toutes les zones de SizeSearch x SizeSearch pixels contenant au moins NbMatchMin pixel proches de la couleur ColorToFind 
//   celle qui est la plus proche de ColorSearch. 
// La couleur � trouver peut �tre relativis�e � une distance max de ShadeVariation sur chaque composant R, G et B (fonctionnalit� � venir)
int WINAPI GenericColorSearch(int SizeSearch, int &NbMatchMin, int &XRef, int &YRef, int ColorToFind/*-1 if several colors*/, int ShadeVariation, int NoSnapShot)
	// Author: GenericColorSearch By FastFrench
{
	// Param�trage de la recherche
	bool bFull = (NbMatchMin >= (SizeSearch * SizeSearch)); // Designed for possible nice optimization in this specific case - if I care to do that one day - 
	if (bFull)  NbMatchMin = SizeSearch * SizeSearch; // We can't have more
	bool bAllColors = (ColorToFind==-1);
	bool bShadeVariation = ShadeVariation>0;

	#ifdef MYTRACE
	GChrono.Restart();
	#endif

	if (!SnapShotData::IsSnapShotValid(NoSnapShot, _T("GenericColorSearch"))) return 0;
	
	
	int *pBufOpt  = NULL; // Buffer permettant d'optimiser la recherche locale des pixels
	
	XRef -= GtSnapShotData[NoSnapShot].x1; // On exprime XRef/YRef en relatif par rapport � la fen�tre utile
	YRef -= GtSnapShotData[NoSnapShot].y1;
	bool bFound = false;
	//if (!SnapShot(aLeft, aTop, aRight, aBottom, NoSnapShot)) return 0;
	LPCOLORREF screen_pixel = GtSnapShotData[NoSnapShot].SnapShotPixels;
	// Create an empty bitmap to hold all the pixels currently visible on the screen that lie within the search area:
	int search_width  = GtSnapShotData[NoSnapShot].GetAreaWidth();
	int search_height = GtSnapShotData[NoSnapShot].GetAreaHeight();
	LONG screen_pixel_count = GtSnapShotData[NoSnapShot].GetPixelCount();
	
	LONG BufferWidth     = GtSnapShotData[NoSnapShot].lScreenWidth, 
		 BufferLineCount = GtSnapShotData[NoSnapShot].lScreenHeight;

	if (XRef < 0 || YRef < 0 || XRef >= BufferWidth || YRef >= BufferLineCount)
	{
		#ifdef MYTRACE
		Tracer.Format(DEBUG_SYSTEM_ERROR, _T("Coordinates (%d, %d) are outside the SnapShot(%d, %d, %d, %d)\n"), XRef + GtSnapShotData[NoSnapShot].x1, YRef + GtSnapShotData[NoSnapShot].y1, GtSnapShotData[NoSnapShot].x1, GtSnapShotData[NoSnapShot].y1, GtSnapShotData[NoSnapShot].x2, GtSnapShotData[NoSnapShot].y2);
		#endif
		return 0;
	}

#ifdef MYTRACE
	if (ColorToFind==-1)
		Tracer.Format(DEBUG_STREAM_SYSTEM, _T("GenericColorSearch(Size:%d, NbPixel:%d, X:%d, Y:%d, Color:-1 (%d colors), ShadeVariation:%d, N�SS:%d) %ld pixels - %d Excluded areas\n"), SizeSearch, NbMatchMin, XRef, YRef, NbColors, ShadeVariation, NoSnapShot, screen_pixel_count, GnbExludedArea);
	else
		Tracer.Format(DEBUG_STREAM_SYSTEM, _T("GenericColorSearch(Size:%d, NbPixel:%d, X:%d, Y:%d, Color:%08X, ShadeVariation:%d, N�SS:%d) %ld pixels - %d Excluded areas\n"), SizeSearch, NbMatchMin, XRef, YRef, ColorToFind, ShadeVariation, NoSnapShot, screen_pixel_count, GnbExludedArea);
#endif

	int	 x, y; 
	
	// Best candidate so far
	int BestX = -1, BestY = -1,  // Position du meilleur "Spot"
		BestCount = 0, // Nombre de "hits" dans le meilleur Spot
		BestDistance = 0x7FFFFFFF,  // Carr� de la distance entre position de r�f�rence et centre de gravit� du meilleur spot
		BestX0=-1, BestY0=-1;
	int NbCurCountTot, CurDistance; 
	if (SizeSearch < 1) SizeSearch = 1;
	bool bIsAnyExclusion = IsAnyExclusion();
	int ResearchCase = (bAllColors ? 1 : 0) | (bShadeVariation ? 2 : 0) | (bIsAnyExclusion?4:0);

	int LargeurFenetre = SizeSearch * 2 - 1;

	LPCOLORREF rowPixelUp, rowPixelDown;
	
	// If either is 16-bit, convert *both* to the 16-bit-compatible 32-bit format:
	//GtSnapShotData[NoSnapShot].AdjustColor(ColorToFind); => would corrupt -1 d�tection :(
	
	int NbTotalCount = 0;
	int yDown = 0;
	if (SizeSearch>1) {
		pBufOpt  = new int[search_width+1]; // Buffer permettant d'optimiser la recherche locale des pixels
		ZeroMemory(pBufOpt, search_width * sizeof(int));
	
		rowPixelUp = screen_pixel ;
		for (y = 0; (y < SizeSearch) && (y<BufferLineCount); y++, rowPixelUp += BufferWidth) 
				switch(ResearchCase) { // On pr�voit explicitement tous les cas possibles, en faisant la logique de tri au niveau de la ligne pour optimiser le traitement des pixels 
					case 0: // Une seule couleur, Pas de ShadeVariation, Pas d'exclusions
						for (x = 0; x < search_width; x++) // 1�re passe, on met � jour les cumuls par abscisse sur le pixel courant et les SizeSearch pixel vers le bas (d'indice 0 � SizeSearch-1)
							if (rowPixelUp[x] == ColorToFind)	
									pBufOpt[x]++;	
						break;
					case 1: // Plusieurs couleurs, Pas de ShadeVariation, Pas d'exclusions
						for (x = 0; x < search_width; x++) // 1�re passe, on met � jour les cumuls par abscisse sur le pixel courant et les SizeSearch pixel vers le bas (d'indice 0 � SizeSearch-1)
							if (::FindIt(rowPixelUp[x]))
									pBufOpt[x]++;	
						break;
					case 2: // Une seule couleur, ShadeVariation, Pas d'exclusions
						for (x = 0; x < search_width; x++) // 1�re passe, on met � jour les cumuls par abscisse sur le pixel courant et les SizeSearch pixel vers le bas (d'indice 0 � SizeSearch-1)
							if (IsInShadeVariation(rowPixelUp[x], ColorToFind, ShadeVariation))
									pBufOpt[x]++;	
						break;
					case 3: // Plusieurs couleurs, ShadeVariation, Pas d'exclusions
						for (x = 0; x < search_width; x++) // 1�re passe, on met � jour les cumuls par abscisse sur le pixel courant et les SizeSearch pixel vers le bas (d'indice 0 � SizeSearch-1)
							if (::FindIt(rowPixelUp[x], ShadeVariation))
									pBufOpt[x]++;	
						break;
					case 4: // Une seule couleur, Pas de ShadeVariation, avec zones exclues
						for (x = 0; x < search_width; x++) // 1�re passe, on met � jour les cumuls par abscisse sur le pixel courant et les SizeSearch pixel vers le bas (d'indice 0 � SizeSearch-1)
							if (!GtSnapShotData[NoSnapShot]._IsExcluded(x, y) && rowPixelUp[x] == ColorToFind)	
									pBufOpt[x]++;	
						break;
					case 5: // Plusieurs couleurs, Pas de ShadeVariation, avec zones exclues
						for (x = 0; x < search_width; x++) // 1�re passe, on met � jour les cumuls par abscisse sur le pixel courant et les SizeSearch pixel vers le bas (d'indice 0 � SizeSearch-1)
							if (!GtSnapShotData[NoSnapShot]._IsExcluded(x, y) && ::FindIt(rowPixelUp[x]))
									pBufOpt[x]++;	
						break;
					case 6: // Une seule couleur, ShadeVariation, avec zones exclues
						for (x = 0; x < search_width; x++) // 1�re passe, on met � jour les cumuls par abscisse sur le pixel courant et les SizeSearch pixel vers le bas (d'indice 0 � SizeSearch-1)
							if (!GtSnapShotData[NoSnapShot]._IsExcluded(x, y) && IsInShadeVariation(rowPixelUp[x], ColorToFind, ShadeVariation))
									pBufOpt[x]++;	
						break;
					case 7: // Plusieurs couleurs, ShadeVariation, avec zones exclues
						for (x = 0; x < search_width; x++) // 1�re passe, on met � jour les cumuls par abscisse sur le pixel courant et les SizeSearch pixel vers le bas (d'indice 0 � SizeSearch-1)
							if (!GtSnapShotData[NoSnapShot]._IsExcluded(x, y) && ::FindIt(rowPixelUp[x], ShadeVariation))
									pBufOpt[x]++;	
						break;
				}
		rowPixelDown = screen_pixel + BufferWidth * (SizeSearch-1);
		yDown = SizeSearch-1;
		}
	
	rowPixelUp = screen_pixel ; // On se repositionne sur la premi�re ligne
	
	// Astuce pour acc�lerer les traitements : on d�cale XRef et YRef pour prendre en compte la diff�rence 
	// entre le centre de la fen�tre de d�tection 
	// et son bord sup�rieur droit identifi� point� par (x,y)
	XRef += SizeSearch/2;
	YRef -= SizeSearch/2;

	for (y = 0; y < search_height-SizeSearch; y++, rowPixelUp += BufferWidth)
		{
			
			if (SizeSearch==1) { // Cas d'une recherche sur un seul pixel 
				switch(ResearchCase) { // On pr�voit explicitement tous les cas possibles, en faisant la logique de tri au niveau de la ligne pour optimiser le traitement des pixels 
					case 0: // Une seule couleur, Pas de ShadeVariation, Pas d'exclusions
						for (x = 0; x <= search_width; x++)
							if (rowPixelUp[x] == ColorToFind)	{
								CurDistance = Distance(XRef - x, YRef - y);
								if (CurDistance < BestDistance) {
									bFound = true;
									BestDistance = CurDistance;
									BestX = x;
									BestY = y;
									BestCount = 1;
								}
							}
						break;
					case 1: // Plusieurs couleurs, Pas de ShadeVariation, Pas d'exclusions
						for (x = 0; x <= search_width; x++)
							if (::FindIt(rowPixelUp[x]))	{
								CurDistance = Distance(XRef - x, YRef - y);
								if (CurDistance < BestDistance) {
									bFound = true;
									BestDistance = CurDistance;
									BestX = x;
									BestY = y;
									BestCount = 1;
								}
							}
						break;
					case 2: // Une seule couleur, ShadeVariation, Pas d'exclusions
						for (x = 0; x <= search_width; x++)
							if (IsInShadeVariation(rowPixelUp[x], ColorToFind, ShadeVariation))	{
								CurDistance = Distance(XRef - x, YRef - y);
								if (CurDistance < BestDistance) {
									bFound = true;
									BestDistance = CurDistance;
									BestX = x;
									BestY = y;
									BestCount = 1;
								}
							}
						break;
					case 3: // Plusieurs couleurs, ShadeVariation, Pas d'exclusions
						for (x = 0; x <= search_width; x++)
							if (::FindIt(rowPixelUp[x], ShadeVariation))	{
								CurDistance = Distance(XRef - x, YRef - y);
								if (CurDistance < BestDistance) {
									bFound = true;
									BestDistance = CurDistance;
									BestX = x;
									BestY = y;
									BestCount = 1;
								}
							}
						break;
					case 4: // Une seule couleur, Pas de ShadeVariation, avec zones exclues
						for (x = 0; x <= search_width; x++)
							if (!GtSnapShotData[NoSnapShot]._IsExcluded(x, y) && rowPixelUp[x] == ColorToFind)	{
								CurDistance = Distance(XRef - x, YRef - y);
								if (CurDistance < BestDistance) {
									bFound = true;
									BestDistance = CurDistance;
									BestX = x;
									BestY = y;
									BestCount = 1;
								}
							}
						break;
					case 5: // Plusieurs couleurs, Pas de ShadeVariation, avec zones exclues
						for (x = 0; x <= search_width; x++)
							if (!GtSnapShotData[NoSnapShot]._IsExcluded(x, y) && ::FindIt(rowPixelUp[x]))	{
								CurDistance = Distance(XRef - x, YRef - y);
								if (CurDistance < BestDistance) {
									bFound = true;
									BestDistance = CurDistance;
									BestX = x;
									BestY = y;
									BestCount = 1;
								}
							}
						break;
					case 6: // Une seule couleur, ShadeVariation, avec zones exclues
						for (x = 0; x <= search_width; x++)
							if (!GtSnapShotData[NoSnapShot]._IsExcluded(x, y) && IsInShadeVariation(rowPixelUp[x], ColorToFind, ShadeVariation))	{
								CurDistance = Distance(XRef - x, YRef - y);
								if (CurDistance < BestDistance) {
									bFound = true;
									BestDistance = CurDistance;
									BestX = x;
									BestY = y;
									BestCount = 1;
								}
							}
						break;
					case 7: // Plusieurs couleurs, ShadeVariation, avec zones exclues
						for (x = 0; x <= search_width; x++)
							if (!GtSnapShotData[NoSnapShot]._IsExcluded(x, y) && ::FindIt(rowPixelUp[x], ShadeVariation))	{
								CurDistance = Distance(XRef - x, YRef - y);
								if (CurDistance < BestDistance) {
									bFound = true;
									BestDistance = CurDistance;
									BestX = x;
									BestY = y;
									BestCount = 1;
								}
							}
						break;
				} // switch
			}
			else // SizeSearch > 1 => Cas de recherche sur des zones (au moins NbMatchMin pixel sur une zone SizeSearch x SizeSearch)
			{
				NbCurCountTot = 0;
				for (x = 0; x < (SizeSearch-1); x++) // 1�re passe, on initialise les premiers rangs sur la largeur de la fen�tre de recherche
						NbCurCountTot += pBufOpt[x];

				for (; x < search_width; x++) // 1�re passe, on cherche si on trouve un meilleur spot sur la ligne
				{
					NbCurCountTot += pBufOpt[x]; // Ajoute les stats pour abscisse courant

					if (NbCurCountTot > BestCount) // Am�lioration 1.4 : le meilleur spot, � d�faut d'atteindre NbMatchMin
					{
						if (BestCount < NbMatchMin) 
							{
							BestCount = NbCurCountTot; // On conserve le meilleur compte, pour le retourner en cas d'�chec
							BestX = x; // Il faudra les d�caler
							BestY = y;
							}
					}

					if (NbCurCountTot >= NbMatchMin) // Cette zone r�pond au crit�re de la fonction
					{ 

						CurDistance = Distance(XRef - x, YRef - y); // x et y sont un peu en d�calage
						if (CurDistance < BestDistance)
						{ // Nouveau meilleur spot
							BestDistance = CurDistance;
							BestX = x; // Il faudra les d�caler
							BestY = y;
							BestCount = NbCurCountTot;
#ifdef MYTRACE
/*							if (Tracer.TextDebug()) {
								Tracer.Format(DEBUG_STREAM_SYSTEM_DETAIL, _T("Inside GenericColorSearch Spot Found : bFound:%d X:%d, Y:%d, Count:%d (L"), bFound, x, y, NbCurCountTot);
								for (int xxx=x+1-SizeSearch; xxx<=x; xxx++)
									Tracer.Format(DEBUG_STREAM_SYSTEM_DETAIL|DEBUG_SAME_LINE, _T("%d%c"),  pBufOpt[xxx], xxx<x?',':')');
								Tracer.Format(DEBUG_STREAM_SYSTEM_DETAIL|DEBUG_SAME_LINE, _T(", MinCount:%d\n"), NbMatchMin);
							}*/
#endif
							bFound = true;
							
						}						
					}
					NbCurCountTot -= pBufOpt[x+1-SizeSearch]; // Retire les stats pour dernier abscisse entr� 
				} // for
				rowPixelDown += BufferWidth;
				yDown++;

				switch(ResearchCase) { // On pr�voit explicitement tous les cas possibles, en faisant la logique de tri au niveau de la ligne pour optimiser le traitement des pixels 
					case 0: // Une seule couleur, Pas de ShadeVariation, Pas d'exclusions
						for (x = 0; x < search_width; x++) { // 1�re passe, on met � jour les cumuls par abscisse sur le pixel courant et les SizeSearch pixel vers le bas (d'indice 0 � SizeSearch-1)
							if (rowPixelUp[x] == ColorToFind)	
								pBufOpt[x]--;	
							if (rowPixelDown[x] == ColorToFind)	
								pBufOpt[x]++;
						}
						break;
					case 1: // Plusieurs couleurs, Pas de ShadeVariation, Pas d'exclusions
						for (x = 0; x < search_width; x++) { // 1�re passe, on met � jour les cumuls par abscisse sur le pixel courant et les SizeSearch pixel vers le bas (d'indice 0 � SizeSearch-1)
							if (::FindIt(rowPixelUp[x]))
								pBufOpt[x]--;	
							if (::FindIt(rowPixelDown[x]))
								pBufOpt[x]++;	
						}
						break;
					case 2: // Une seule couleur, ShadeVariation, Pas d'exclusions
						for (x = 0; x < search_width; x++) {// 1�re passe, on met � jour les cumuls par abscisse sur le pixel courant et les SizeSearch pixel vers le bas (d'indice 0 � SizeSearch-1)
							if (IsInShadeVariation(rowPixelUp[x], ColorToFind, ShadeVariation))
								pBufOpt[x]--;	
							if (IsInShadeVariation(rowPixelDown[x], ColorToFind, ShadeVariation))
								pBufOpt[x]++;	
						}
						break;
					case 3: // Plusieurs couleurs, ShadeVariation, Pas d'exclusions
						for (x = 0; x < search_width; x++) {// 1�re passe, on met � jour les cumuls par abscisse sur le pixel courant et les SizeSearch pixel vers le bas (d'indice 0 � SizeSearch-1)
							if (::FindIt(rowPixelUp[x], ShadeVariation))
								pBufOpt[x]--;	
							if (::FindIt(rowPixelDown[x], ShadeVariation))
								pBufOpt[x]++;	
						}
						break;
					case 4: // Une seule couleur, Pas de ShadeVariation, avec zones exclues
						for (x = 0; x < search_width; x++) {// 1�re passe, on met � jour les cumuls par abscisse sur le pixel courant et les SizeSearch pixel vers le bas (d'indice 0 � SizeSearch-1)
							if (!GtSnapShotData[NoSnapShot]._IsExcluded(x, y) && rowPixelUp[x] == ColorToFind)	
								pBufOpt[x]--;	
							if (!GtSnapShotData[NoSnapShot]._IsExcluded(x, yDown) && rowPixelDown[x] == ColorToFind)	
								pBufOpt[x]++;	
						}
						break;
					case 5: // Plusieurs couleurs, Pas de ShadeVariation, avec zones exclues
						for (x = 0; x < search_width; x++) {// 1�re passe, on met � jour les cumuls par abscisse sur le pixel courant et les SizeSearch pixel vers le bas (d'indice 0 � SizeSearch-1)
							if (!GtSnapShotData[NoSnapShot]._IsExcluded(x, y) && ::FindIt(rowPixelUp[x]))
								pBufOpt[x]--;	
							if (!GtSnapShotData[NoSnapShot]._IsExcluded(x, yDown) && ::FindIt(rowPixelDown[x]))
								pBufOpt[x]++;	
						}
						break;
					case 6: // Une seule couleur, ShadeVariation, avec zones exclues
						for (x = 0; x < search_width; x++) {// 1�re passe, on met � jour les cumuls par abscisse sur le pixel courant et les SizeSearch pixel vers le bas (d'indice 0 � SizeSearch-1)
							if (!GtSnapShotData[NoSnapShot]._IsExcluded(x, y) && IsInShadeVariation(rowPixelUp[x], ColorToFind, ShadeVariation))
								pBufOpt[x]--;	
							if (!GtSnapShotData[NoSnapShot]._IsExcluded(x, yDown) && IsInShadeVariation(rowPixelDown[x], ColorToFind, ShadeVariation))
								pBufOpt[x]++;	
						}
						break;
					case 7: // Plusieurs couleurs, ShadeVariation, avec zones exclues (la totale !)
						for (x = 0; x < search_width; x++) {// 1�re passe, on met � jour les cumuls par abscisse sur le pixel courant et les SizeSearch pixel vers le bas (d'indice 0 � SizeSearch-1)
							if (!GtSnapShotData[NoSnapShot]._IsExcluded(x, y) && ::FindIt(rowPixelUp[x], ShadeVariation))
								pBufOpt[x]--;	
							if (!GtSnapShotData[NoSnapShot]._IsExcluded(x, yDown) && ::FindIt(rowPixelDown[x], ShadeVariation))
								pBufOpt[x]++;	
						}
						break;
				} // switch

			}		// Recherche zone
	} // y

#ifdef MYTRACE
	Tracer.Format(DEBUG_STREAM_SYSTEM_DETAIL, _T("Inside GenericColorSearch (before PostProcessing) bFound:%d XRef:%d, YRef:%d, hWnd:%8X\n"), bFound, XRef, YRef, GtSnapShotData[NoSnapShot].hWnd);
#endif
	// PostProcessing 
	// ==============
	// Maintenant que l'on sait o� se trouve la localisation approximative du "Best Spot", on �largit un peu cette zone (+2 pixels)
	// et on cherche pr�cis�ment le "centre de gravit�" des points ayant la couleur voulue. 
	BestX -= SizeSearch/2;
	BestY += SizeSearch/2;
	if (BestCount>0 && SizeSearch>1) { // Cas d'une recherche sur plusieurs pixels 
		int x1 = max(0, BestX - SizeSearch/2 -2);
		int x2 = min(search_width-1, BestX + SizeSearch/2 +2);
		int y1 = max(0,  BestY - SizeSearch/2 -2);
		int y2 = min(search_height-1, BestY + SizeSearch/2 +2);
		unsigned int  CumulX=0, CumulY = 0;
		int NbHit = 0;
#ifdef MYTRACE
		Tracer.Format(DEBUG_STREAM_SYSTEM_DETAIL, _T("Detail %d pixels found inside GenericColorSearch :"), BestCount);
#endif
		for (x=x1; x<x2; x++)
			for (y=y1; y<y2; y++)
				if (!bIsAnyExclusion ||  !GtSnapShotData[NoSnapShot]._IsExcluded(x, y))
					if (GenericColorChecker(screen_pixel[x+y*BufferWidth], ColorToFind, ShadeVariation))
						{
						CumulX += x;
						CumulY += y;
						NbHit++;
#ifdef MYTRACE
		Tracer.Format(DEBUG_STREAM_SYSTEM_DETAIL|DEBUG_SAME_LINE, _T("(%d,%d) "), x+GtSnapShotData[NoSnapShot].x1, y+GtSnapShotData[NoSnapShot].y1);
#endif
						}
		//if ((NbHit < BestCount || NbHit < NbMatchMin) && bFound)
		//	MessageBox(0, L"Zut, y'a une couille dans le potage !", L"GenericColorSearch", MB_OK); // Ce message ne devrait jamais appara�tre :p
		//BestCount = NbHit; // NbHit peut d�passer SizeSearch * SizeSearch => on garde plut�t l'ancienne valeur
		if (NbHit > 0)
		{
			BestX = CumulX / NbHit;
			BestY = CumulY / NbHit;
			if (NbHit > BestCount)
				BestCount = NbHit;
		}
#ifdef MYTRACE
		Tracer.Format(DEBUG_STREAM_SYSTEM_DETAIL, _T(" => %d pixels, center (%d,%d)\n\n"), NbHit, BestX+GtSnapShotData[NoSnapShot].x1, BestY+GtSnapShotData[NoSnapShot].y1);
#endif
		}

	BestX += GtSnapShotData[NoSnapShot].x1; // on recadre la r�ponse en coordonn�es absolues dans la fen�tre
	BestY += GtSnapShotData[NoSnapShot].y1;
	if (bFound) {		
		if (Tracer.GraphicDebug())  { // Si debuggage visuel, on affiche � l'�cran la zone trouv�e, le point renvoy� et le point de r�f�rence fourni
			HDC hdc = GtSnapShotData[NoSnapShot].GetDC();
			if (hdc != 0) {
				HGDIOBJ hOldPen = ::SelectObject( hdc, ::GetStockObject(DC_PEN));
				::SetDCPenColor(hdc, RGB(255,0,0));
				HGDIOBJ hOldBrush = ::SelectObject(hdc, ::GetStockObject(NULL_BRUSH));
				
				::Rectangle( // On entoure la zone par un rectangle rouge
							hdc,
							BestX-SizeSearch/2,
							BestY-SizeSearch/2,
							BestX+SizeSearch/2,
							BestY+SizeSearch/2);
				::Ellipse( hdc, // Avec un petit rond rouge sur le point renvoy�
							BestX -3,
							BestY -3,
							BestX +3,
							BestY +3);

				::SetDCPenColor(hdc, RGB(0,0,255));

					// On repositionne le point de r�f�rence
				XRef -= SizeSearch/2;
				YRef += SizeSearch/2;
				XRef += GtSnapShotData[NoSnapShot].x1;
				YRef += GtSnapShotData[NoSnapShot].y1;
#ifdef MYTRACE
				Tracer.Format(DEBUG_STREAM_SYSTEM_DETAIL, _T("Inside GenericColorSearch XRef:%d, YRef:%d, hWnd:%8X\n"), XRef, YRef, GtSnapShotData[NoSnapShot].hWnd);
#endif
				::Ellipse( hdc, // Et un petit rond bleu sur le point renvoy�
							XRef -3,
							YRef -3,
							XRef +3,
							YRef +3);
				::SelectObject( hdc, hOldPen);
				::SelectObject(hdc, hOldBrush);
				GtSnapShotData[NoSnapShot].ReleaseDC(hdc);	
				}
			}
		}
	// If found==false when execution reaches here, ErrorLevel is already set to the right value, so just 
	// clean up then return.
	
	if (pBufOpt!=NULL) delete pBufOpt; 

#ifdef MYTRACE
	if (bFound)
		Tracer.Format(DEBUG_MB_SYSTEM, _T("GenericColorSearch : Spot found\n\t\tBestX=%d, BestY=%d, BestCount=%d ScreenWidth=%d SearchWidth=%d\n\nTotal processing time: %s\n"), BestX, BestY, BestCount, BufferWidth, search_width, GChrono.GetTime());
	else
		Tracer.Format(DEBUG_MB_SYSTEM, _T("GenericColorSearch : No Spot found (best count = %d)\n"), BestCount);
#endif	

	NbMatchMin = BestCount;
	XRef = BestX;
	YRef = BestY;
	
	if (!bFound) // Let ErrorLevel, which is either "1" or "2" as set earlier, tell the story.		
		return 0; // 0 = Pas trouv� - Dans ce cas NbMatchMin renvoie le meilleur compte
	
	return 1; // 1 = trouv�
}

	// ProgressiveSearch : Cherche la zone ayant le maximum de pixels d'une couleur donn�e (avec un nombre minimum et un nombre optimum de pixels)
    //		  - La recherche est d�j� men�e sans ShadeVariation. Si elle �choue, elle est refaite avec ShadeVariation (Si ShadeVariation demand�). 
int WINAPI ProgressiveSearch(int SizeSearch, int &NbMatchMin, int NbMatchMax, int &XRef, int &YRef, int ColorToFind/*-1 if several colors*/, int ShadeVariation, int NoSnapShot)
{
	int NbMatchMin0 = NbMatchMin, XRef0 = XRef, YRef0 = YRef;
	int NbPixel = NbMatchMax;
#ifdef MYTRACE
	Tracer.Format(DEBUG_STREAM_SYSTEM, _T("ProgressiveSearch Start (SpotSize:%d, MinPixels:%d, OptPixels:%d, XRef:%d, YRef:%d, Color:%6X (%d in list), ShadeVariation:%d, NoSnapShot:%d)\n"), SizeSearch, NbMatchMin, NbMatchMax, XRef0, YRef0, ColorToFind, NbColors, ShadeVariation, NoSnapShot);

	GChrono.Restart();
#endif
	if (ShadeVariation>0)
	{
		GenericColorSearch(SizeSearch, NbPixel, XRef, YRef, ColorToFind, 0, NoSnapShot); // On recherche tout d'abord sans ShadeVariation
		if (NbPixel>NbMatchMin)
		{
#ifdef MYTRACE
			Tracer.Format(DEBUG_MB_SYSTEM, _T("ProgressiveSearch (SpotSize:%d, MinPixels:%d, OptPixels:%d, XRef:%d, YRef:%d, Color:%6X (%d in list), ShadeVariation:%d, NoSnapShot:%d): Spot found with no need of ShadeVariation \n\t\t(X,Y)=(%d,%d), NbPixels=%d\n\nTotal processing time: %s\n"), SizeSearch, NbMatchMin, NbMatchMax, XRef0, YRef0, ColorToFind, NbColors, ShadeVariation, NoSnapShot, XRef, YRef, NbPixel, GChrono.GetTime());
#endif	
			NbMatchMin = NbPixel;

			return 1;
		}
		Tracer.Format(DEBUG_STREAM_SYSTEM, _T("ProgressiveSearch Step 1 Failed (%d/[%d,%d] pixels found)\n"), NbPixel, NbMatchMin, NbMatchMax);

		// Echec => On remet les param�tres initiaux pour les recherches suivantes
		NbPixel = NbMatchMax;
		XRef = XRef0;
		YRef = YRef0;
	}
	GenericColorSearch(SizeSearch, NbPixel, XRef, YRef, ColorToFind, ShadeVariation, NoSnapShot); // On recherche tout d'abord sans ShadeVariation
	if (NbPixel>NbMatchMin)
	{
		//MessageBox(0,"OK: found", L"After Generic", MB_OK);

#ifdef MYTRACE
		Tracer.Format(DEBUG_MB_SYSTEM, _T("ProgressiveSearch (SpotSize:%d, MinPixels:%d, OptPixels:%d, XRef:%d, YRef:%d, Color:%6X (%d in list), ShadeVariation:%d, NoSnapShot:%d): Spot found \n\t\t(X,Y)=(%d,%d), NbPixels=%d\n\nTotal processing time: %s\n"), SizeSearch, NbMatchMin, NbMatchMax, XRef, YRef, ColorToFind, NbColors, ShadeVariation, NoSnapShot, XRef, YRef, NbPixel, GChrono.GetTime());
#endif	
		NbMatchMin = NbPixel;
		return 1;
	}
	// Echec
	//MessageBox(0,"OK: Not Found", L"After Generic", MB_OK);
#ifdef MYTRACE
	Tracer.Format(DEBUG_MB_SYSTEM, _T("ProgressiveSearch (SpotSize:%d, MinPixels:%d, OptPixels:%d, XRef:%d, YRef:%d, Color:%6X (%d in list), ShadeVariation:%d, NoSnapShot:%d): Spot NOT found \n\t\tBest spot : (X,Y)=(%d,%d), NbPixels=%d\n\nTotal processing time: %s\n"), SizeSearch, NbMatchMin, NbMatchMax, XRef, YRef, ColorToFind, NbColors, ShadeVariation, NoSnapShot, XRef, YRef, NbPixel, GChrono.GetTime());
#endif	
	NbMatchMin = NbPixel;
	return 0;
}


int WINAPI ColorsPixelSearch(int &XRef, int &YRef, int NoSnapShot) {
	if (!SnapShotData::IsSnapShotValid(NoSnapShot, _T("ColorPixelSearch"))) return 0;	
	int NbMatchMin = 1;
	return ColorsSearch(1, NbMatchMin, XRef, YRef, NoSnapShot);
}


// ColorsSearch est similaire � ColorSearch � un d�tail important pr�s : il recherche non plus UNE mais plusieurs couleurs simultan�ment. 
int WINAPI ColorsSearch(int SizeSearch, int &NbMatchMin, int &XRef, int &YRef, int NoSnapShot)
	// Author: ColorSearch By FastFrench
{
	return GenericColorSearch(SizeSearch, NbMatchMin, XRef, YRef,  -1, 0, NoSnapShot);
}

/*
 * Appel en AutoIt : 
 *    $Result = DllCall(L"FastFind.dll", L"int", L"ColorCount", L"int", $Left, L"int", $Top, L"int", $Right, L"int", $Bottom, L"int", $ColorToCount, L"int", $ShadeVariation) 
 *    If (Not IsArray($Result)) Return $Result;
 *    Return $Result[0];
 */

// ColorCount vous indique combien de pixel ont la couleur (exacte ou approch�e) demand�e dans le SnapShot
int WINAPI ColorCount(int ColorToFind, int NoSnapShot, int ShadeVariation)
	// Author: ColorSearch By FastFrench
{

	if (!SnapShotData::IsSnapShotValid(NoSnapShot, _T("ColorCount"))) return 0;	

	LPCOLORREF screen_pixel = GtSnapShotData[NoSnapShot].GetPixels();
	bool found = false;
	LONG screen_pixel_count = GtSnapShotData[NoSnapShot].GetPixelCount();
	int i, NbFound = 0;
#ifdef MYTRACE
	GChrono.Restart();
#endif
	GtSnapShotData[NoSnapShot].AdjustColor(ColorToFind);
	
	if (ShadeVariation<1) {
		for (i = 0; i < screen_pixel_count; ++i)
			if (screen_pixel[i] == ColorToFind) // A screen pixel has been found that matches the exact color we're looking for.
					NbFound++;		
		}		
	else {
		int RedRef = ColorToFind & 0x00FF0000;
		int GreenRef = ColorToFind & 0x0000FF00;
		int BlueRef = ColorToFind & 0x000000FF;
		int RedDelta = ShadeVariation << 16;
		int GreenDelta = ShadeVariation << 8;
		int BlueDelta = ShadeVariation ;
		for (i = 0; i < screen_pixel_count; ++i) {
			if (   ((abs(((int) screen_pixel[i] & 0x00FF0000) - RedRef)) <=  RedDelta) &&
				   (abs((((int) screen_pixel[i] & 0x0000FF00) - GreenRef)) <=  GreenDelta) &&
					(abs((((int) screen_pixel[i] & 0x000000FF) - BlueRef)) <=  BlueDelta)       )					
						NbFound++;
			}
		}
#ifdef MYTRACE
	Tracer.Format(DEBUG_MB_SYSTEM, _T("ColorCount(color:%06X, SnapShot:%d, ShadeVariation:%d) : %d pixels Found in %s."), ColorToFind, NoSnapShot, ShadeVariation, NbFound, GChrono.GetTime());		
#endif
	return NbFound;
}

int WINAPI ComputeMeanValues(int NoSnapShot, int &MeanRed, int &MeanGreen, int &MeanBlue)
{
	unsigned long RedCumul = 0, BlueCumul = 0, GreenCumul = 0;
	if (!SnapShotData::IsSnapShotValid(NoSnapShot, _T("MeanValues"))) return 0;	

	LPCOLORREF screen_pixel = GtSnapShotData[NoSnapShot].GetPixels();
	LONG screen_pixel_count = GtSnapShotData[NoSnapShot].GetPixelCount();
	if (screen_pixel_count == 0) return 0;

	int i;
#ifdef MYTRACE
	GChrono.Restart();
#endif	
	for (i = 0; i < screen_pixel_count; ++i) {
		RedCumul += ((unsigned int) screen_pixel[i] & 0x00FF0000) >> 16;
		GreenCumul += ((unsigned int) screen_pixel[i] & 0x0000FF00) >> 8;
		BlueCumul += ((unsigned int) screen_pixel[i] & 0x000000FF);
		}
#ifdef MYTRACE
	Tracer.Format(DEBUG_MB_SYSTEM, _T("MeanValues(SnapShot:%d) : %d pixels processed in %s."), NoSnapShot, screen_pixel_count, GChrono.GetTime());		
#endif
	MeanRed = RedCumul / screen_pixel_count;
	MeanGreen = GreenCumul / screen_pixel_count;
	MeanBlue = BlueCumul / screen_pixel_count;
	return 1;
}

int WINAPI ApplyFilterOnSnapShot(int SnapShot, int Red, int Green, int Blue)
{
	COLORREF Filter = ((Red & 0x000000FF)<<16) + ((Green & 0x000000FF)<<8) + (Blue & 0x000000FF);
	if (!SnapShotData::IsSnapShotValid(SnapShot, _T("ApplyFilterOnSnapShot"))) return 0;	

	LPCOLORREF screen_pixel = GtSnapShotData[SnapShot].GetPixels();
	LONG screen_pixel_count = GtSnapShotData[SnapShot].GetPixelCount();
	int i;
#ifdef MYTRACE
	GChrono.Restart();
#endif	
	for (i = 0; i < screen_pixel_count; ++i) 
		screen_pixel[i] &= Filter;
#ifdef MYTRACE
	Tracer.Format(DEBUG_MB_SYSTEM, _T("MeanValues(ApplyFilterOnSnapShot:%d) : %d pixels processed in %s."), SnapShot, screen_pixel_count, GChrono.GetTime());		
#endif
	return 1;
}



