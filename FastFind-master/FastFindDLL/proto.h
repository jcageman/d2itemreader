/*
 * Fonctions export�es de FastFind.dll
 * WINAPI est d�fini comme suit : #define WINAPI _stdcall
 * COLORREF est d�fini comme suit : typedef DWORD COLORREF
 */

// Exclusion areas
bool WINAPI IsExcluded(int x, int y, HWND hWnd);
void WINAPI ResetExcludedAreas();

// Configuration
void WINAPI SetDebugMode(int NewMode);
void WINAPI SetHWnd(HWND NewWindowHandle, bool bClientArea);
LPCTSTR WINAPI GetLastErrorMsg();

// Fonctions simples
int WINAPI GetPixel(int X, int Y, int NoSnapShot);
int WINAPI ColorPixelSearch(int &XRef, int &YRef, int ColorToFind, int NoSnapShot);
int WINAPI ColorPixelSearchRectangle(int &XRef, int &YRef, int StartX, int StartY, int EndX, int EndY, int ColorToFind, bool bVerticalFirst, int NoSnapShot);

// Snapshots
int WINAPI SnapShot(int aLeft, int aTop, int aRight, int aBottom, int NoSnapShot);

// Gestion du tableau des couleurs
int WINAPI AddColor (COLORREF NewColor);
int WINAPI RemoveColor (COLORREF NewColor);
void WINAPI ResetColors ();

// Fonctions de recherche explicitement multi-couleurs
int WINAPI ColorsPixelSearch(int &XRef, int &YRef, int NoSnapShot);

// ColorsSearch est similaire � ColorSearch � un d�tail important pr�s : il recherche non plus UNE mais plusieurs couleurs simultan�ment. 
int WINAPI ColorsSearch(int SizeSearch, int &NbMatchMin, int &XRef, int &YRef, int NoSnapShot);

// Fonctions de recherche la plus g�n�rique (celle qui est appel�e dans la plupart des cas in fine)
int WINAPI GenericColorSearch(int SizeSearch, int &NbMatchMin, int &XRef, int &YRef, int ColorToFind, int ShadeVariation, int NoSnapShot);

// Fonction encore plus archi-puissante (nouveaut� 1.4)
int WINAPI ProgressiveSearch(int SizeSearch, int &NbMatchMin, int NbMatchMax, int &XRef, int &YRef, int ColorToFind/*-1 if several colors*/, int ShadeVariation, int NoSnapShot);

// Fonction de comptage des pixels d'une couleur donn�e
int WINAPI ColorCount(int ColorToFind, int NoSnapShot, int ShadeVariation);

// D�tection des changements
int WINAPI HasChanged(int NoSnapShot, int NoSnapShot2);
int WINAPI LocalizeChanges(int NoSnapShot, int NoSnapShot2, int &xMin, int &yMin, int &xMax, int &yMax, int &nbFound);
