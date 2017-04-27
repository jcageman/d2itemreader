Opt ( "CaretCoordMode", 2 )
Opt("MouseCoordMode", 2) ;1=absolute, 0=relative, 2=client
Opt("PixelCoordMode", 2) ;1=absolute, 0=relative, 2=client
Opt("WinTitleMatchMode", 2)
Opt("TrayIconDebug", 1)

; FastFind
#include "FastFind.au3"

#include <Array.au3>

#RequireAdmin

Opt("MustDeclareVars", 1)

Dim $arCharactersTemp[63][2]
$arCharactersTemp[1][0]="A"
$arCharactersTemp[1][1]="0,-1;-1,0;-1,-1;-7,0;-7,-1;"

$arCharactersTemp[2][0]="B"
$arCharactersTemp[2][1]="0,-1;0,-2;0,-3;0,-4;0,-5;0,-6;0,-7;0,-8;-1,0;-1,-1;-1,-2;-1,-3;-1,-4;-1,-5;-1,-6;-1,-7;-1,-8;-2,0;-2,-3;-2,-7;-3,0;-3,-2;-3,-3;-3,-7;-4,0;-4,-1;-4,-2;-4,-3;-4,-4;-4,-5;-4,-6;-4,-7;"

$arCharactersTemp[3][0]="C"
$arCharactersTemp[3][1]="0,-1;0,-2;-1,-1;-1,-2;-1,-3;-2,-3;-2,-4;-3,-4;-3,-5;-4,-4;-4,-5;-5,-4;-5,-5;-6,-4;"

$arCharactersTemp[4][0]="D"
$arCharactersTemp[4][1]="0,-1;0,-2;0,-5;0,-6;0,-7;0,-8;-1,0;-1,-1;-1,-2;-1,-3;-1,-4;-1,-5;-1,-6;-1,-7;-1,-8;"

$arCharactersTemp[5][0]="E"
$arCharactersTemp[5][1]="0,-1;0,-2;0,-3;0,-4;0,-5;0,-6;-1,0;-1,-1;-1,-2;-1,-3;-1,-4;-1,-5;-1,-6;-2,-1;-2,-2;-2,-5;-2,-6;-3,-1;-3,-2;-3,-5;-3,-6;-4,-1;-4,-2;-4,-5;-4,-6;"

$arCharactersTemp[6][0]="F"
$arCharactersTemp[6][1]="0,-2;0,-3;0,-4;0,-5;-1,0;-1,-1;-1,-2;-1,-3;-1,-4;-1,-5;-1,-6;-1,-7;-2,0;-2,-3;-2,-4;-3,0;-4,0;-4,-3;-4,-4;-5,-3;"

$arCharactersTemp[7][0]="G"
$arCharactersTemp[7][1]="0,-1;0,-2;0,-3;-1,-4;-2,-5;-3,-5;-3,-6;-4,-5;-4,-6;-5,-1;-5,-2;-5,-5;-6,-1;-6,-2;-6,-3;-6,-4;-6,-5;"

$arCharactersTemp[8][0]="H"
$arCharactersTemp[8][1]="0,-3;0,-4;0,-5;0,-6;0,-7;0,-8;-1,0;-1,-1;-1,-2;-1,-3;-1,-4;-1,-5;-1,-6;-1,-7;-1,-8;-2,-3;-2,-4;-3,-3;-3,-4;-4,-3;-4,-4;-5,0;-5,-1;-5,-2;-5,-3;-5,-4;-5,-5;-5,-6;-5,-7;-5,-8;-6,-4;-6,-6;-6,-7;"

$arCharactersTemp[9][0]="I"
$arCharactersTemp[9][1]="0,-1;0,-4;0,-5;0,-6;0,-7;-1,0;-1,-1;-1,-2;-1,-3;-1,-4;-1,-5;-1,-6;-1,-7;"

$arCharactersTemp[10][0]="J"
$arCharactersTemp[10][1]="0,-1;0,-2;0,-3;0,-4;0,-5;0,-6;0,-7;0,-8;0,-9;-1,0;-1,-1;-1,-2;-1,-3;-1,-4;-1,-5;-1,-6;-1,-7;-1,-8;"

$arCharactersTemp[11][0]="K"
$arCharactersTemp[11][1]="-1,0;-6,0;"

$arCharactersTemp[12][0]="L"
$arCharactersTemp[12][1]="0,-1;0,-2;0,-3;0,-4;0,-5;0,-6;0,-7;0,-8;-1,-7;-2,-7;-2,-8;-3,-7;-3,-8;-4,-8;"

$arCharactersTemp[13][0]="M"
$arCharactersTemp[13][1]="0,-1;0,-2;0,-3;0,-4;0,-5;0,-6;0,-7;0,-8;-1,0;-2,0;-3,0;-3,-1;-3,-2;-3,-3;-3,-4;-3,-5;-3,-6;-3,-7;-3,-8;-4,0;-4,-1;-4,-2;-4,-3;-4,-4;-4,-5;-4,-6;-4,-7;-4,-8;-5,0;-6,0;-7,0;-7,-1;-7,-2;-7,-3;-7,-4;-7,-5;-7,-6;-7,-7;-7,-8;-8,0;-8,-1;-8,-2;-8,-3;-8,-4;-8,-5;-8,-6;-8,-7;-8,-8;"

$arCharactersTemp[14][0]="N"
$arCharactersTemp[14][1]="0,-3;0,-4;0,-5;0,-6;0,-7;0,-8;-1,0;-1,-1;-1,-2;-1,-3;-1,-4;-1,-5;-1,-6;-1,-7;-1,-8;-2,-2;-3,-2;-3,-3;-4,-4;-5,-5;-6,0;-6,-1;-6,-2;-6,-3;-6,-4;-6,-5;-6,-6;-6,-8;-7,0;-7,-7;-7,-8;"

$arCharactersTemp[15][0]="O"
$arCharactersTemp[15][1]="0,-1;0,-2;0,-3;-1,0;-1,-1;-1,-2;-1,-3;-1,-4;-2,-1;-2,-2;-2,-5;-3,-1;-3,-2;-3,-5;-3,-6;-4,0;-4,-1;-4,-2;-4,-3;-4,-4;-4,-5;-4,-6;-5,-1;-5,-2;-5,-5;-6,-1;-6,-2;-6,-4;-6,-5;-7,0;-7,-1;-7,-2;-7,-3;-7,-4;-8,0;-8,-1;-8,-2;-8,-3;"

$arCharactersTemp[16][0]="P"
$arCharactersTemp[16][1]="0,-1;0,-2;0,-5;0,-6;0,-7;-1,0;-1,-1;-1,-2;-1,-3;-1,-4;-1,-5;-1,-6;-1,-7;-2,0;-2,-4;-3,0;-3,-4;-4,0;-4,-1;-4,-2;-4,-3;-5,-2;"

$arCharactersTemp[17][0]="Q"
$arCharactersTemp[17][1]="0,-1;0,-2;0,-3;0,-4;-1,0;-1,-4;-1,-5;-2,-5;-3,-5;-3,-6;-4,-3;-4,-4;-4,-5;-4,-6;-4,-7;-5,-5;-5,-6;-6,-5;-7,0;-7,-1;-7,-3;-7,-4;-8,0;-8,-1;-8,-2;-8,-3;"

$arCharactersTemp[18][0]="R"
$arCharactersTemp[18][1]="0,-1;0,-2;0,-3;0,-7;0,-8;-1,0;-1,-4;-1,-6;-1,-7;-2,0;-2,-4;-3,0;-3,-3;-3,-4;-3,-5;-4,-1;-4,-2;-4,-3;-4,-6;-5,-7;-6,-7;-6,-8;"

$arCharactersTemp[19][0]="S"
$arCharactersTemp[19][1]="0,-1;0,-5;0,-6;-1,0;-1,-1;-1,-2;-1,-5;-1,-6;-2,-2;-2,-6;-3,-3;-3,-6;-4,0;-4,-3;-4,-4;-4,-5;-4,-6;"

$arCharactersTemp[20][0]="T"
$arCharactersTemp[20][1]="-1,0;-2,0;-3,0;-4,0;-4,-2;-4,-4;-4,-5;-4,-7;-4,-8;-5,0;-5,-1;-5,-2;-5,-3;-5,-4;-5,-5;-5,-6;-5,-7;-5,-8;-6,0;-7,0;-8,0;"

$arCharactersTemp[21][0]="U"
$arCharactersTemp[21][1]="0,-1;0,-2;0,-3;0,-4;-1,0;-1,-1;-1,-2;-1,-3;-1,-4;-1,-5;-1,-6;-2,-5;-2,-7;-3,-7;-4,-7;-4,-8;-5,-7;-5,-8;-6,-7;-7,-7;-8,0;-8,-1;-8,-2;-8,-3;-8,-4;-8,-5;-8,-6;-9,0;-9,-1;-9,-2;-9,-3;-9,-4;-9,-5;"

$arCharactersTemp[22][0]="V"
$arCharactersTemp[22][1]="-1,0;-1,-1;-1,-2;-2,-2;-2,-3;-3,-4;-3,-5;-4,-6;-4,-7;-5,-5;-5,-6;-6,-3;-6,-4;-6,-5;-7,-1;-7,-2;-7,-3;-8,0;-8,-1;"

$arCharactersTemp[23][0]="W"
$arCharactersTemp[23][1]="-1,0;-1,-1;-1,-2;-2,-2;-2,-3;-2,-4;-3,-4;-3,-5;-3,-6;-4,0;-4,-6;-4,-7;-4,-8;-5,0;-5,-1;-5,-2;-5,-5;-5,-6;-5,-7;-6,-2;-6,-3;-6,-4;-6,-5;-7,-1;-7,-2;-7,-3;-7,-5;-7,-6;-8,0;-8,-1;-8,-6;-8,-7;-9,-5;-9,-6;-10,-3;-10,-4;-11,0;-11,-1;-11,-2;-12,0;"

$arCharactersTemp[24][0]="X"
$arCharactersTemp[24][1]="0,-7;0,-8;-1,-1;-1,-6;-2,-2;-2,-5;-3,-3;-3,-4;-4,-3;-4,-4;-5,-2;-5,-5;-6,-1;-6,-6;-7,0;-7,-1;-7,-7;"

$arCharactersTemp[25][0]="Y"
$arCharactersTemp[25][1]="-1,0;-1,-1;-2,-1;-2,-2;-2,-3;-3,-2;-3,-3;-3,-8;-4,-3;-4,-4;-4,-5;-4,-6;-4,-7;-4,-8;-5,-2;-5,-3;-6,-1;-6,-2;-7,0;-7,-1;-8,0;"

$arCharactersTemp[26][0]="Z"
$arCharactersTemp[26][1]="-1,0;-1,-6;-1,-7;-1,-8;-2,0;-2,-5;-2,-6;-2,-7;-2,-8;-3,0;-3,-3;-3,-4;-3,-7;-3,-8;-4,0;-4,-1;-4,-2;-4,-3;-4,-7;-5,0;-5,-1;-5,-7;-5,-8;"

$arCharactersTemp[27][0]="a"
$arCharactersTemp[27][1]="0,-1;-1,0;-2,0;-5,0;"

$arCharactersTemp[28][0]="b"
$arCharactersTemp[28][1]="0,-1;0,-2;0,-3;0,-4;0,-5;-1,0;-1,-2;-2,0;-2,-2;-2,-3;-2,-5;-3,-3;-3,-4;"

$arCharactersTemp[29][0]="c"
$arCharactersTemp[29][1]="-1,0;-1,-1;-2,-2;-3,-3;-5,-3;"

$arCharactersTemp[30][0]="d"
$arCharactersTemp[30][1]="0,-1;0,-2;0,-3;0,-4;0,-5;-1,0;-1,-1;-1,-2;-1,-3;-1,-4;-1,-5;-1,-6;-2,0;-2,-6;-3,-6;-4,0;-4,-5;-5,-1;-5,-2;-5,-3;-5,-4;"

$arCharactersTemp[31][0]="e"
$arCharactersTemp[31][1]="0,-1;0,-2;0,-3;0,-5;-1,0;-1,-2;-1,-3;-2,0;-2,-2;-2,-3;-3,0;-3,-2;-3,-3;-3,-6;"

$arCharactersTemp[32][0]="f"
$arCharactersTemp[32][1]="0,-1;0,-2;0,-3;0,-4;-1,0;-1,-1;-1,-2;-1,-3;-1,-4;-1,-5;-2,0;-2,-2;-2,-3;-3,0;-3,-2;-3,-3;"

$arCharactersTemp[33][0]="g"
$arCharactersTemp[33][1]="0,-1;-1,-2;-1,-3;-2,-4;-3,-1;-3,-4;-4,-1;-4,-4;-5,-1;-5,-2;-5,-3;"

$arCharactersTemp[34][0]="h"
$arCharactersTemp[34][1]="0,-1;0,-2;0,-3;0,-4;0,-5;-1,-1;-1,-2;-1,-4;-2,-1;-2,-2;-3,-1;-4,0;-4,-1;-4,-2;-4,-3;-4,-4;-4,-5;"

$arCharactersTemp[35][0]="i"
$arCharactersTemp[35][1]="0,-1;0,-2;0,-3;0,-4;0,-5;0,-6;"

$arCharactersTemp[36][0]="j"
$arCharactersTemp[36][1]="0,-1;0,-2;0,-3;0,-4;0,-5;0,-6;"

$arCharactersTemp[37][0]="k"
$arCharactersTemp[37][1]="0,-1;0,-2;0,-3;0,-4;0,-5;0,-6;-1,-1;-1,-3;-2,0;-2,-4;-3,0;-3,-5;"

$arCharactersTemp[38][0]="l"
$arCharactersTemp[38][1]="0,-1;0,-2;0,-3;0,-4;0,-5;0,-6;-1,-6;-2,-6;"

$arCharactersTemp[39][0]="m"
$arCharactersTemp[39][1]="0,-1;0,-2;0,-3;0,-4;0,-5;0,-6;-1,0;-2,0;-3,0;-3,-1;-3,-2;-3,-3;-3,-4;-3,-5;-3,-6;-4,0;-5,0;-6,0;-6,-1;-6,-2;-6,-3;-6,-4;-6,-5;-6,-6;"

$arCharactersTemp[40][0]="n"
$arCharactersTemp[40][1]="0,-1;0,-2;0,-3;0,-4;0,-5;0,-6;-1,-1;-2,-2;-2,-3;-3,-3;-3,-4;-4,0;-4,-1;-4,-2;-4,-3;-4,-4;-4,-5;-5,0;-5,-1;-5,-2;-5,-3;-5,-4;-5,-5;-5,-6;"

$arCharactersTemp[41][0]="o"
$arCharactersTemp[41][1]="0,-1;0,-2;0,-3;-1,-2;-1,-3;-1,-4;-2,-2;-2,-4;-2,-5;-3,0;-3,-1;-3,-2;-3,-3;-3,-4;-3,-5;-4,-2;-4,-4;-4,-5;-5,-2;-5,-4;-6,0;-6,-1;-6,-2;-6,-3;"

$arCharactersTemp[42][0]="p"
$arCharactersTemp[42][1]="0,-3;0,-5;-1,0;-1,-3;-2,0;-2,-3;-3,0;-3,-1;-3,-2;"

$arCharactersTemp[43][0]="q"
$arCharactersTemp[43][1]="0,-1;0,-2;0,-3;-1,-4;-2,-5;-3,-3;-3,-4;-3,-5;-3,-6;-4,-4;-5,0;-5,-4;-6,0;-6,-1;-6,-2;-6,-3;"

$arCharactersTemp[44][0]="r"
$arCharactersTemp[44][1]="0,-2;0,-3;-1,-2;-1,-3;-2,0;-2,-3;-3,0;-3,-1;-3,-2;-3,-4;-4,-5;"

$arCharactersTemp[45][0]="s"
$arCharactersTemp[45][1]="0,-4;-1,0;-1,-1;-1,-4;-1,-5;-2,-1;-2,-5;-3,-2;-3,-5;-4,-3;"

$arCharactersTemp[46][0]="t"
$arCharactersTemp[46][1]="0,-1;-1,-1;-2,-1;-3,-1;-3,-2;-3,-3;-3,-4;-3,-5;-3,-6;-4,-1;-5,-1;-6,-1;"

$arCharactersTemp[47][0]="u"
$arCharactersTemp[47][1]="0,-1;0,-2;0,-3;0,-4;-1,-5;"

$arCharactersTemp[48][0]="v"
$arCharactersTemp[48][1]="0,-1;-1,-1;-1,-2;-2,-3;-2,-4;-3,-5;-3,-6;-4,-3;-4,-4;-5,-1;-5,-2;-6,0;"

$arCharactersTemp[49][0]="w"
$arCharactersTemp[49][1]="0,-1;0,-2;-1,-3;-1,-4;-2,-5;-2,-6;-3,-2;-3,-3;-3,-5;-3,-6;-4,-3;-4,-4;-4,-5;-5,-1;-5,-2;-5,-3;-5,-5;-5,-6;-6,-1;-6,-6;-7,-3;-7,-4;-8,-1;-8,-2;-8,-3;-9,-1;"

$arCharactersTemp[50][0]="x"
$arCharactersTemp[50][1]="0,-5;-1,-1;-1,-4;-2,-2;-2,-3;-3,-2;-3,-3;-4,-1;-4,-4;-5,0;-5,-5;-6,0;"

$arCharactersTemp[51][0]="y"
$arCharactersTemp[51][1]="-1,0;-2,-1;-3,-2;-3,-4;-3,-5;-4,-2;-5,-1;-6,0;-7,0;"

$arCharactersTemp[52][0]="z"
$arCharactersTemp[52][1]="-1,0;-1,-5;-1,-6;-2,0;-2,-3;-2,-4;-2,-6;-3,-2;-3,-6;-4,0;-4,-1;-4,-6;-5,0;-5,-6;"

$arCharactersTemp[53][0]="1"
$arCharactersTemp[53][1]="0,-1;0,-2;0,-3;0,-4;0,-5;0,-6;0,-7;"

$arCharactersTemp[54][0]="2"
$arCharactersTemp[54][1]="0,-1;0,-6;-1,0;-1,-1;-1,-5;-1,-6;-2,-4;-2,-5;-2,-6;-3,-3;-3,-4;-3,-6;-4,0;-4,-1;-4,-2;-4,-3;-4,-6;-5,0;-5,-1;-5,-6;"

$arCharactersTemp[55][0]="3"
$arCharactersTemp[55][1]="0,-1;-1,0;-1,-5;-1,-6;-1,-7;-2,0;-2,-3;-2,-7;-3,0;-3,-2;-3,-3;-4,0;-4,-1;-4,-3;-4,-4;-4,-7;-5,0;-5,-4;-5,-5;-5,-6;"

$arCharactersTemp[56][0]="4"
$arCharactersTemp[56][1]="-1,0;-2,0;-3,0;-4,0;-4,-1;-4,-2;-5,0;-6,0;"

$arCharactersTemp[57][0]="5"
$arCharactersTemp[57][1]="0,-2;0,-3;-1,0;-1,-1;-1,-2;-1,-3;-1,-4;-2,0;-2,-4;-3,0;-3,-4;-4,0;-4,-1;-4,-3;-4,-4;-5,-1;-5,-2;-5,-3;"

$arCharactersTemp[58][0]="6"
$arCharactersTemp[58][1]="-1,0;-1,-1;-1,-2;-2,-2;-3,-2;-3,-3;-4,-2;-5,0;-5,-1;"

$arCharactersTemp[59][0]="7"
$arCharactersTemp[59][1]="-1,0;-2,0;-2,-6;-2,-7;-3,-4;-3,-5;-4,0;-4,-1;-4,-2;-4,-3;-5,0;"

$arCharactersTemp[60][0]="8"
$arCharactersTemp[60][1]="0,-3;0,-4;0,-5;-1,0;-1,-1;-1,-2;-1,-3;-1,-5;-1,-6;-2,-2;-2,-6;-2,-7;-3,-1;-3,-2;-3,-6;-4,0;-4,-1;-4,-3;-4,-4;-4,-5;-4,-6;-5,-4;"

$arCharactersTemp[61][0]="9"
$arCharactersTemp[61][1]="0,-1;-1,0;-1,-1;-1,-2;-2,-3;-2,-6;-3,-3;-3,-4;-3,-5;-4,-2;-4,-3;-5,0;-5,-1;-5,-2;"

$arCharactersTemp[62][0]="0"
$arCharactersTemp[62][1]="0,-1;0,-2;0,-3;-1,0;-1,-4;-2,-3;-2,-4;-2,-5;-3,-2;-3,-5;-4,-1;-4,-5;-4,-6;-5,0;-6,0;-6,-5;-7,0;-7,-3;-7,-4;-8,0;-8,-1;-8,-2;-8,-3;"

Const $arCharacters = $arCharactersTemp
;HotKeySet("{HOME}", "startReader")
;HotKeySet("{END}", "EndReader")
;HotKeySet("{PAUSE}", "TogglePauseReader")

Const $Pixel = 0x5050AC ; 5263532 = blue
Global $hWnd = WinActive("[CLASS:Diablo II]")

;global const $DEBUG_DEFAULT = 3
global const $DEBUG_GRAPHIC = 0;$DEBUG_DEFAULT + 4

Func TimeSpan($dif)
	Local $res = $dif
	if ($res >= 10) Then
		return " = "&Round($res)&" miliS "&@lf
	Else
		return " = "&Round($res*1000)&" nanoS "&@lf
	EndIf
 EndFunc

Func startReader()
ConsoleWrite('starting' & @CRLF)

Local $aArray = GetItemStats()
Local Const $arrayLength = UBound($aArray)
For $i = 0 to $arrayLength - 1
Local $Stringvalue = $aArray[$i]
ConsoleWrite('stats: ' & $Stringvalue & @CRLF)
Next
Exit
EndFunc

Func GetItemStats()
FFSetDebugMode($DEBUG_GRAPHIC)
FFAddColor($Pixel)
$hWnd = WinActive("[CLASS:Diablo II]")
FFSetWnd($hWnd)
FFSetDefaultSnapShot(0)
FFSnapShot(0, 0, 0, 0, $FFLastSnap, $hWnd)

Local $Line = GetLines( 0, 0, 400, 600 , $Pixel )

;ConsoleWrite("found " & $Line[0][0] & " lines" & @CRLF)

Local $aResult = []

For $LineNum = 1 to $Line[0][0]
   Local $String = ''
   ;ConsoleWrite("reading line" & $LineNum & "with coords: " & $Line[$LineNum][0] & ", " & $Line[$LineNum][1] & ", " & $Line[$LineNum][2] & ", " & $Line[$LineNum][3] & @CRLF)
   ;Local $begin = TimerInit()
   Local $Find = GetCharacters ( $Line[$LineNum][0], $Line[$LineNum][1], $Line[$LineNum][2], $Line[$LineNum][3], $Pixel )
   ;Local $dif = TimerDiff($begin)
   ;ConsoleWrite("determine characters " & TimeSpan($dif) & @CRLF )
   Local $DistanceString[$Find[0][0]+1]

   For $i = 1 to $Find[0][0]
	  ;$begin = TimerInit()
	  ;ConsoleWrite("reading pixels for" & $LineNum & "with coords: " & $Find[$i][0] & ", " & $Find[$i][1] & ", " & $Find[$i][2] & ", " & $Find[$i][3] & @CRLF)
	  Local $var = _GetAllPixels($Find[$i][0], $Find[$i][1], $Find[$i][2], $Find[$i][3], $Pixel )
	  ;$dif = TimerDiff($begin)
	  ;ConsoleWrite("determine all pixels " & TimeSpan($dif)& @CRLF )
	  For $e = 1 to $Var[0][0]
		 Local $distx = $Find[$i][0] - $Var[$e][0]
		 Local $disty = $Find[$i][1] - $Var[$e][1]
		 $DistanceString[$i] = $DistanceString[$i] & $distx & "," & $disty & ";"
	  Next

	  $DistanceString[$i] = StringTrimLeft ( $DistanceString[$i], 4 )
	  ;ConsoleWrite($DistanceString[$i] & @CRLF)
	  For $x = 1 to UBound($arCharacters) - 1
		 If $arCharacters[$x][1] == $DistanceString[$i] Then
			$String = $String & $arCharacters[$x][0]
			;ConsoleWrite($arCharacters[$x][0] & @CRLF)
			ExitLoop
		 EndIf
	  Next
	  If UBound($Find) > $i + 1 Then
		 If Abs($Find[$i][2] - $Find[$i+1][0]) > 4 Then $String = $String & " "
		 EndIf
   Next
   _ArrayAdd($aResult, $String)
   ;ConsoleWrite("get actual characters " & TimeSpan($dif) & @CRLF )
Next

Return $aResult

EndFunc

Func _GetAllPixels($StartX, $StartY, $EndX, $EndY, $Color)
Local $aCoord, $Length, $Width, $Area, $i, $x, $y, $CurrentElement = 1
$Length = Abs($StartX - $EndX)
$Width = Abs($StartY - $EndY)
$Area = $Length * $Width
Dim $aReturn[$Area + 1][2]
For $x = $StartX to $EndX
   For $y = $StartY to $EndY
	  If FFGetPixel( $x, $y) = $Color Then
		 $aReturn[$CurrentElement][0] = $x
		 $aReturn[$CurrentElement][1] = $y
		 $CurrentElement = $CurrentElement + 1
	  EndIf
   Next
Next
ReDim $aReturn[$CurrentElement][2]
$aReturn[0][0] = $CurrentElement - 1
Return $aReturn
EndFunc

Func ExcludeAround($StartX,$StartY,$EndX,$EndY)
   FFResetExcludedAreas()
   FFAddExcludedArea(0,0,800,$StartY-1)
   FFAddExcludedArea(0,$EndY+1,800,600)
   FFAddExcludedArea(0,0,$StartX-1,600)
   FFAddExcludedArea($EndX+1,0,800,600)
EndFunc

Func SearchPixel($StartX,$StartY, $EndX, $EndY,$Color, $VerticalFirst)
   ;ExcludeAround($StartX,$StartY, $EndX, $StartY)
   ;$arLineCheck = FFNearestPixel($StartX, $StartY, $Color, False)
   Local $result = FFPixelInRectangle($StartX, $StartY,$EndX, $EndY, $Color, $VerticalFirst)
   SetError(@error)
   Return $result
EndFunc

Func GetLines($StartX, $StartY, $EndX, $EndY, $Color)
Local $arLineCheck = 0, $intElement = 1, $FirstFound = 0

Dim $arNewLine[255][4]
While $StartY < $EndY
   ;ConsoleWrite("getting line" & $StartX & ", " & $StartY & ", " & $EndX & ", " & $StartY & @CRLF)
   $arLineCheck = SearchPixel($StartX, $StartY, $EndX, $StartY, $Color, False)
   If @error = 0 AND $FirstFound = 0 Then
	  ;ConsoleWrite("found line" & $arLineCheck[0] & ", " & $arLineCheck[1] & @CRLF)
	  $arNewLine[$intElement][0] = $StartX ; correct a bit since NearestPixel might find a location to far to the right
	  $arNewLine[$intElement][1] = $arLineCheck[1]
	  $FirstFound = 1
   EndIf
   If @error = 1 AND $FirstFound > 0 Then
	  $arNewLine[$intElement][2] = $EndX
	  $arNewLine[$intElement][3] = $StartY
	  $intElement = $intElement + 1;
	  $FirstFound = 0
   EndIf
   $StartY = $StartY + 1;
WEnd
$arNewLine[0][0] = $intElement - 1
;ConsoleWrite("number of lines" & $intElement & @CRLF)
ReDim $arNewLine[$intElement][4]
Return $arNewLine
EndFunc

Func GetCharacters($StartX, $StartY, $EndX, $EndY, $Color)
Local $vCharacter = 1, $vLineCheck[2], $FirstFound = 0
Dim   $vCharCheck, $aNewCharacter[255][4], $currentX = 0

$vLineCheck = SearchPixel($StartX, $StartY,$EndX, $EndY, $Color, True)

If @error = 1 Then
   ConsoleWrite("Error in GetCharacters")
   SetError(1)
   Return
EndIf

Local $vEndOfLineCheck = SearchPixel($EndX, $StartY,$StartX, $EndY, $Color, True)

;ConsoleWrite("GetCharacters start " & $vLineCheck[0] & ", " & $vLineCheck[1] & @CRLF )
;ConsoleWrite("GetCharacters end " & $vEndOfLineCheck[0] & ", " & $vEndOfLineCheck[1] & @CRLF )

$currentX = $vLineCheck[0] - 1
While $currentX <= $vEndOfLineCheck[0]
   $currentX = $currentX + 1
   ;Local $begin = TimerInit()
   ;Local $begin2 = TimerInit()
   $vLineCheck = SearchPixel($currentX, $StartY,$currentX, $EndY, $Color, True)
   Local $Error = @error
   ;Local $dif = TimerDiff($begin)
   ;ConsoleWrite("NearestPixel took " & TimeSpan($dif) & @CRLF )
   If $Error = 0 AND $FirstFound = 0 Then
	  ;ConsoleWrite("FirstChar start " & $vLineCheck[0] & ", " & $vLineCheck[1] & @CRLF )
	  $currentX = $vLineCheck[0]
	  $aNewCharacter[$vCharacter][0] = $vLineCheck[0]
	  $aNewCharacter[$vCharacter][1] = $vLineCheck[1]
	  $vCharacter = $vCharacter + 1
	  $FirstFound = 1
   EndIf

   If $Error = 1 AND $FirstFound = 1 Then
	  $aNewCharacter[$vCharacter-1][2] = $currentX
	  $aNewCharacter[$vCharacter-1][3] = $EndY
	  $FirstFound = 0
   EndIf

   ;Local $dif2 = TimerDiff($begin2)
   ;ConsoleWrite("NearestPixel full took " & TimeSpan($dif2) & @CRLF )
WEnd

;ConsoleWrite("GetCharacters took " & $Iterations & @CRLF )
$aNewCharacter[0][0] = $vCharacter - 1
ReDim $aNewCharacter[$vCharacter][4]
Return $aNewCharacter
EndFunc


Func EndReader()
Exit
EndFunc
Func TogglePauseReader()
Global $Paused = NOT $Paused
While $Paused
sleep(100)
WEnd
EndFunc


