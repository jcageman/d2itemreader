

; itemreader
#include "itemreader.au3"

#include <Math.au3>

HotKeySet("{HOME}", "begin")
HotKeySet("{END}", "end")
HotKeySet("{PAUSE}", "TogglePause")

Global $Paused = False
Global $InitialMouseLoc[2]

While 1
Sleep(10)
WEnd

Func GetLeftStat($string, $tofind)
   Local $position = StringInStr($string,$tofind)
   if $position Then
	  Return Number(StringStripWS(StringTrimRight($string,$position - 1),1))
   EndIf
   Return 0
EndFunc

Func GetRightStat($string, $tofind)
   Local $position = StringInStr($string,$tofind)
   if $position Then
	  Return Number(StringStripWS(StringTrimLeft($string,$position + StringLen($tofind) - 1),1))
   EndIf
   Return 0
EndFunc

Func GetFireResist($string)
   Const $LookupStr = "Fire Resist"
   return GetRightStat($string, $LookupStr)
EndFunc

Func GetLightningResist($string)
   Const $LookupStr = "Lightning Resist"
   return GetRightStat($string, $LookupStr)
EndFunc

Func GetColdResist($string)
   Const $LookupStr = "Cold Resist"
   return GetRightStat($string, $LookupStr)
EndFunc

Func GetPoisonResist($string)
   Const $LookupStr = "Poison Resist"
   return GetRightStat($string, $LookupStr)
EndFunc

Func GetAllResist($string)
   Const $LookupStr = "All Resistances"
   return GetRightStat($string, $LookupStr)
EndFunc

Func GetLife($string)
   Const $LookupStr = "to Life"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetReplenishLife($string)
   Const $LookupStr = "Replenish Life"
   return GetRightStat($string, $LookupStr)
EndFunc

Func GetToStrength($string)
   Const $LookupStr = "to Strength"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetToDexterity($string)
   Const $LookupStr = "to Dexterity"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetToEnergy($string)
   Const $LookupStr = "to Energy"
   if not StringInStr($string,"Sorceress Only") Then
	  return GetLeftStat($string, $LookupStr)
   EndIf
EndFunc

Func GetToMana($string)
   Const $LookupStr = "to Mana"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetFasterCastRate($string)
   Const $LookupStr = "Faster Cast Rate"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetFasterRunWalk($string)
   Const $LookupStr = "Faster RunWalk"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetFasterHitRecovery($string)
   Const $LookupStr = "Faster Hit Recovery"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetFasterBlockRate($string)
   Const $LookupStr = "Faster Block Rate"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetIncreasedAttackSpeed($string)
   Const $LookupStr = "Increased Attack Speed"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetToSorceressSkillLevels($string)
   Const $LookupStr = "to Sorceress Skill Levels"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetToBarbarianSkillLevels($string)
   Const $LookupStr = "to Barbarian Skill Levels"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetToPaladinSkillLevels($string)
   Const $LookupStr = "to Paladin Skill Levels"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetToAmazonSkillLevels($string)
   Const $LookupStr = "to Amazon Skill Levels"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetToNecroMancerSkillLevels($string)
   Const $LookupStr = "to Necromancer Skill Levels"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetEnhancedDefense($string)
   Const $LookupStr = "Enhanced Defense"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetEnhancedDamage($string)
   Const $LookupStr = "Enhanced Damage"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetToMinimumDamage($string)
   Const $LookupStr = "to Minimum Damage"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetToMaximumDamage($string)
   Const $LookupStr = "to Maximum Damage"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetAddedDamage($string)
   Const $LookupStr = "Adds"
   Local $position = StringInStr($string,$LookupStr)
   if $position and not StringInStr($string,"fire") and not StringInStr($string,"cold") and not StringInStr($string,"poison") and not StringInStr($string,"lightning") Then
	  Local $Right = StringStripWS(StringTrimLeft($string,$position + StringLen($LookupStr) - 1),1)
	  Local $damageposition = StringInStr($Right,"damage")
	  ConsoleWrite($Right & @CRLF)
	  if $damageposition Then
		 Local $addedDamage = StringStripWS(StringTrimRight($Right,$damageposition),8)
		 if StringLen($addedDamage) == 4 Then
			Return Number(StringLeft($addeddamage,2)) + Number(StringRight($addeddamage,2))
		 ElseIf StringLen($addedDamage) == 3 Then
			Return Number(StringLeft($addeddamage,1)) + Number(StringRight($addeddamage,2))
		 Else
			Return Number(StringLeft($addeddamage,1)) + Number(StringRight($addeddamage,1))
		 EndIf
	  EndIf
   EndIf

   Return 0
EndFunc

Func GetColdDamage($string)
   Local $position = StringInStr($string,"cold damage")
   Return $position
EndFunc

Func GetToEnergyShield($string)
   Const $LookupStr = "to Energy Shield Sorceress Only"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetToChillingArmor($string)
   Const $LookupStr = "to Chilling Armor Sorceress Only"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetToShiverArmor($string)
   Const $LookupStr = "to Shiver Armor Sorceress Only"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetToBlizzard($string)
   Const $LookupStr = "to Blizzard Sorceress Only"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetLifeStolenPerHit($string)
   Const $LookupStr = "Life stolen per hit"
   return GetLeftStat($string, $LookupStr)
EndFunc

Func GetMinRequirements($string)
   Const $LookupStr = "Requirements"
   return GetRightStat($string, $LookupStr)
EndFunc

Func ShouldKeep()

Local $iColdResist, $iLightningResist, $iFireResist, $iPoisonResist, $iAllResist
Local $iToLife,$iToStrength, $iToDexterity, $iToEnergy, $iToMana, $iToReplenishLife
Local $iFasterCast, $iFasterRun, $iFasterHitRecovery, $iFasterBlockRate, $iIncreasedAttackSpeed
Local $iBarbarianSkills, $iSorcSkills, $iPaladinSkills, $iNecromancerSkills, $iAmazonSkills
Local $iEnhancedDefense,$iEnhancedDamage,$iToMinDamage, $iToMaxDamage, $iAddedDamage, $iColdDamage
Local $iMinRequirements, $iLifeStolenPerHit
Local $iToEnergyShield, $iToChillingArmor, $iToShiverArmor, $iToBlizzard

Local $aArray = GetItemStats()

ConsoleWrite('found new item: ' & @CRLF)

Local Const $arrayLength = UBound($aArray)
For $i = 0 to $arrayLength - 1
Local $Stringvalue = $aArray[$i]
ConsoleWrite('stat: ' & $Stringvalue & @CRLF)

$iColdResist += GetColdResist($Stringvalue)
$iLightningResist += GetLightningResist($Stringvalue)
$iFireResist += GetFireResist($Stringvalue)
$iPoisonResist += GetPoisonResist($Stringvalue)
$iAllResist += GetAllResist($Stringvalue)

$iSorcSkills += GetToSorceressSkillLevels($Stringvalue)
$iBarbarianSkills += GetToBarbarianSkillLevels($Stringvalue)
$iPaladinSkills += GetToPaladinSkillLevels($Stringvalue)
$iAmazonSkills += GetToAmazonSkillLevels($Stringvalue)
$iNecromancerSkills += GetToNecroMancerSkillLevels($Stringvalue)

$iToLife += GetLife($Stringvalue)
$iToStrength += GetToStrength($Stringvalue)
$iToDexterity += GetToDexterity($Stringvalue)
$iToEnergy += GetToEnergy($Stringvalue)
$iToMana += GetToMana($Stringvalue)
$iToReplenishLife += GetReplenishLife($Stringvalue)

$iFasterCast += GetFasterCastRate($Stringvalue)
$iFasterRun += GetFasterRunWalk($Stringvalue)
$iFasterHitRecovery += GetFasterHitRecovery($Stringvalue)
$iFasterBlockRate += GetFasterBlockRate($Stringvalue)
$iIncreasedAttackSpeed += GetIncreasedAttackSpeed($Stringvalue)

$iEnhancedDefense += GetEnhancedDefense($Stringvalue)
$iEnhancedDamage += GetEnhancedDamage($Stringvalue)
$iToMinDamage += GetToMinimumDamage($Stringvalue)
$iToMaxDamage += GetToMaximumDamage($Stringvalue)
$iAddedDamage += GetAddedDamage($Stringvalue)
$iLifeStolenPerHit += GetLifeStolenPerHit($Stringvalue)
$iColdDamage += GetColdDamage($Stringvalue)

$iMinRequirements += GetMinRequirements($Stringvalue)

$iToEnergyShield += GetToEnergyShield($Stringvalue)
$iToChillingArmor += GetToChillingArmor($Stringvalue)
$iToShiverArmor += GetToShiverArmor($Stringvalue)
$iToBlizzard +=GetToBlizzard($Stringvalue)

Next

#CS ConsoleWrite('enhanced defense: ' & $iEnhancedDefense & @CRLF)

 ConsoleWrite('FCR: ' & $iFasterCast & @CRLF)
 ConsoleWrite('FRW: ' & $iFasterRun & @CRLF)
 ConsoleWrite('FHR: ' & $iFasterHitRecovery & @CRLF)
 ConsoleWrite('FBR: ' & $iFasterBlockRate & @CRLF)

 ConsoleWrite('cold resist: ' & $iColdResist & @CRLF)
 ConsoleWrite('light resist: ' & $iLightningResist & @CRLF)
 ConsoleWrite('fire resist: ' & $iFireResist & @CRLF)
 ConsoleWrite('poison resist: ' & $iPoisonResist & @CRLF)
 ConsoleWrite('all resist: ' & $iAllResist & @CRLF)
 ConsoleWrite('to life: ' & $iToLife & @CRLF)

 ConsoleWrite('life: ' & $iToLife & @CRLF)
 ConsoleWrite('strength: ' & $iToStrength & @CRLF)
 ConsoleWrite('dexterity: ' & $iToDexterity & @CRLF)
 ConsoleWrite('energy: ' & $iToEnergy & @CRLF)
 ConsoleWrite('mana: ' & $iToMana & @CRLF)

 ConsoleWrite('dexterity: ' & $iToDexterity & @CRLF)
 ConsoleWrite('energy: ' & $iToEnergy & @CRLF)
 ConsoleWrite('mana: ' & $iToMana & @CRLF)
  ConsoleWrite('added damage: ' & $iAddedDamage & @CRLF)
   ConsoleWrite('energy shield: ' & $iToEnergyShield & @CRLF)
 #CE



#CS
;amulets
if $iSorcSkills == 2 and $iFasterCast == 20 and $iColdResist + $iLightningResist + $iFireResist > 70 and $iToLife  + $iToStrength * 2 + $iToDexterity * 2 + $iToEnergy + $iToMana * 0.5 >= 30 Then
    Return True
 EndIf

 if $iSorcSkills == 2 and $iFasterCast == 20 and $iColdResist + $iLightningResist + $iFireResist > 90 and $iToLife  + $iToStrength * 2 + $iToDexterity * 2 + $iToEnergy + $iToMana * 0.5 >= 40 Then
    Return True
 EndIf

 if $iSorcSkills ==2 and $iFasterCast >= 10 And $iColdResist + $iLightningResist + $iFireResist > 70 and $iToLife  + $iToStrength * 2 + $iToDexterity * 2 + $iToEnergy + $iToMana * 0.5 >= 80 Then
    Return True
 EndIf

 if $iNecromancerSkills == 2  and $iFasterCast >= 10 And $iColdResist + $iLightningResist + $iFireResist > 70 and $iToLife  + $iToStrength * 2 >= 70 Then
    Return True
 EndIf

 if $iNecromancerSkills == 2  and $iFasterCast >= 10 And $iToLife  + $iToStrength * 2 >= 90 Then
    Return True
 EndIf

 if $iPaladinSkills == 2 and $iFasterCast >= 10 And $iColdResist + $iLightningResist + $iFireResist > 70 and $iToLife  + $iToStrength * 3 >= 70 Then
    Return True
 EndIf

 if $iPaladinSkills == 2 and $iFasterCast >= 10 And $iToLife  + $iToStrength * 3 >= 110 Then
    Return True
 EndIf

 If $iBarbarianSkills == 2 And $iToLife  + $iToStrength * 4 + $iToDexterity * 4 + $iToMinDamage * 4 + $iToMaxDamage * 4 >= 110 and $iColdResist + $iLightningResist + $iFireResist > 70 Then
    Return True
 EndIf

 If $iBarbarianSkills == 2 And $iToLife  + $iToStrength * 4 + $iToDexterity * 4 + $iToMinDamage * 4 + $iToMaxDamage * 4 >= 150 Then
    Return True
 EndIf

 if $iAmazonSkills == 2 and $iToLife  + $iToStrength * 3 + $iToDexterity * 3 + $iToMinDamage * 4 + $iToMaxDamage * 4 >= 90 and $iColdResist + $iLightningResist + $iFireResist > 70 Then
    Return True
 EndIf

 if $iAmazonSkills == 2 and $iToLife  + $iToStrength * 3 + $iToDexterity * 3 + $iToMinDamage * 4 + $iToMaxDamage * 4 >= 120 Then
    Return True
 EndIf
 #CE

#CS
;bows
If $iIncreasedAttackSpeed < 20 Then
 	Return False
  EndIf

  If $iAmazonSkills == 2 and $iEnhancedDamage >= 180 and $iMinRequirements >=30 Then
 	Return True
  EndIf

   If $iAmazonSkills == 2 and $iEnhancedDamage >= 195 Then
 	Return True
 EndIf

 #CE

 #CS
; weapons
  If $iEnhancedDamage >= 190 and $iBarbarianSkills == 2 AND $iToMinDamage + $iToMaxDamage + $iAddedDamage + $iToStrength * 0.5 >= 10 Then
 	Return True
  EndIf

  If $iEnhancedDamage >= 190 AND $iToMinDamage + $iToMaxDamage + $iAddedDamage + $iToStrength * 0.5 >= 20 Then
 	Return True
  EndIf

  #CE

#CS
; rings
 If $iToMinDamage + $iToMaxDamage + $iAddedDamage >= 4 AND $iToStrength * 3 + $iToDexterity * 3 + $iToLife >= 60 And $iColdResist + $iLightningResist + $iFireResist >= 55 Then
    Return True
 EndIf

 If $iToStrength + $iToDexterity >= 29 And $iColdResist + $iLightningResist + $iFireResist >= 55 Then
    Return True
 EndIf

 If $iToMinDamage + $iToMaxDamage + $iAddedDamage >= 4 and $iToStrength And $iColdResist + $iLightningResist + $iFireResist >= 70 Then
    Return True
 EndIf

 If $iFasterCast >= 10 AND $iToStrength * 3 + $iToDexterity * 3 + $iToLife >= 60 And $iColdResist + $iLightningResist + $iFireResist >= 70 Then
    Return True
 EndIf

 If $iToStrength * 4 + $iToDexterity * 4 + $iToLife >= 100 And $iColdResist + $iLightningResist + $iFireResist >= 70 Then
    Return True
 EndIf
 #CE

#CS
; boots
 If $iFasterRun >= 30 and $iToDexterity * 2 + $iToStrength * 2 + $iToLife >= 40 and $iColdResist + $iLightningResist + $iFireResist >= 70 Then
	Return True
 EndIf

  If $iFasterHitRecovery >= 20 and $iToDexterity * 2 + $iToStrength * 2 + $iToLife + $iToMana >= 40 and $iColdResist + $iLightningResist + $iFireResist >= 70 Then
	Return True
 EndIf
 #CE

#CS
; helms
If $iToMinDamage >= 2 AND $iToDexterity >= 10 AND $iToLife >= 45 And  ( $iColdResist + $iFireResist >= 50 OR $iColdResist + $iLightningResist >= 50 ) Then
   Return True
EndIf

If $iToLife + $iToDexterity >= 60 And $iColdResist + $iLightningResist + $iFireResist >= 75 Then
   Return True
EndIf

If $iFasterHitRecovery >= 20 AND $iToDexterity * 2 + $iToLife + $iToMana + $iToReplenishLife * 2 >= 100 And $iColdResist >= 28 Then
   Return True
EndIf

If $iFasterHitRecovery >= 20 AND $iToDexterity * 2 + $iToLife + $iToMana + $iToReplenishLife * 2 >= 55 And ( $iColdResist + $iFireResist >= 55 OR $iColdResist + $iLightningResist >= 55 ) Then
   Return True
EndIf
#CE

#CS
If $iSorcSkills ==2 AND $iToEnergyShield >= 3 AND $iToChillingArmor >= 3 Then
   Return True
EndIf
#CE
#CS
; belts
If $iFasterHitRecovery >= 10 AND $iToDexterity * 1.5 + $iToStrength * 2 + $iToLife >= 65 AND ( $iColdResist + $iFireResist >= 55 OR $iColdResist + $iLightningResist >= 55) Then
   Return True
EndIf

If $iColdDamage > 0 and $iToDexterity * 1.5 + $iToStrength * 4 + $iToLife >= 110 AND ( $iColdResist + $iFireResist >= 55 OR $iColdResist + $iLightningResist >= 55 )Then
   Return True
EndIf

#CE

#cs
;bvb armors
If $iEnhancedDefense >=95 AND $iMinRequirements == 40 AND $iToStrength * 4 + $iToLife >= 80 Then
   Return True
EndIf

If $iEnhancedDefense >=98 AND $iToDexterity * 2 + $iToStrength * 4 + $iToLife >= 120 Then
   Return True
EndIf

#CE

;shields
If $iFasterHitRecovery >= 10  AND ( $iColdResist >= 30 and $iFireResist >= 15 and $iLightningResist >= 15 ) AND $iToLife  + $iToStrength * 2 + $iToDexterity * 2 + $iToEnergy + $iToMana + $iToReplenishLife * 4 >= 80 Then
   Return True
EndIf

If $iFasterBlockRate == 30 AND $iFasterHitRecovery >= 10 AND ( $iColdResist >= 30 and $iFireResist >= 15 and $iLightningResist >= 15 ) AND $iToLife  + $iToStrength * 2 + $iToDexterity * 2 + $iToEnergy + $iToMana + $iToReplenishLife * 4 >= 30 Then
   Return True
EndIf

Return False
EndFunc

Func GetCurrentInventoryLoc($column,$row)
   local $CoordResult[2] = [432 + $column * 30,332 + $row * 30]
   return $CoordResult
EndFunc

Func MoveItemToInventory($column,$row)
Sleep(1000)
MouseClick("left",$InitialMouseLoc[0],$InitialMouseLoc[1])
Sleep(1000)
Local $MoveToLoc = GetCurrentInventoryLoc($column,$row)
MouseClick("left",$MoveToLoc[0], $MoveToLoc[1] )
Sleep(1000)
EndFunc

Func MoveItemFromInventory($column,$row)
Local $MoveToLoc = GetCurrentInventoryLoc($column,$row)
Sleep(1000)
MouseClick("left",$MoveToLoc[0], $MoveToLoc[1] )
Sleep(1000)
MouseClick("left",$InitialMouseLoc[0],$InitialMouseLoc[1])
Sleep(1000)
EndFunc

Func GetRandomName()
Local $result = ''

For $i = 1 to 5
   $result = $result & Chr(Random(97, 122, 1)) ;a-z
Next
Return $result
EndFunc

Func WaitUntilPixelHasColor($x,$y,$color)
   Local $count = 0
   While 1
	  If $count > 100000 Then
		 ConsoleWrite("still wrong color: " & PixelGetColor($x,$y) & " after " & $count & " iterations, stopping script" & @CRLF )
		 Exit
	  Endif
	  Sleep(100)
	  If PixelGetColor($x,$y) == $color Then
		 Sleep(500)
		 ExitLoop
	  Else
		 ConsoleWrite("wrong color: " & PixelGetColor($x,$y) & @CRLF )
	  EndIf
	  $count = $count + 1
   WEnd

   Return
EndFunc

Func RestartGame()
   Send("{ESC}")
   Sleep(1000)
   Send("{ESC}")
   MouseMove(400,600)
   Sleep(500)
   Send("{UP}")
   Sleep(500)
   Send("{ENTER}")
   WaitUntilPixelHasColor(756,407, 1052688)
   MouseClick("left",593,460)
   Sleep(500)
   Send(GetRandomName())
   Sleep(100)
   Send("{TAB}")
   Sleep(100)
   Send("cx")
   Sleep(100)
   Send("{ENTER}")

   WaitUntilPixelHasColor(9,571, 1052688)
   Sleep(500)
   Send("i")
   Sleep(500)
   MouseClick("right",681,350)
EndFunc

Func begin()
Local $column = 0, $row = 0
Const $itemwidth = 2
Const $itemheight = 3
$InitialMouseLoc = MouseGetPos()

ConsoleWrite('starting rolling ' & @CRLF)

Local $gametimer = TimerInit()

While 1
   If Not WinActive("[Class:Diablo II]") Then
	  ConsoleWrite("killed script due to inactive diablo window" & @CRLF)
	  Exit
   EndIf
   Local $gametime = TimerDiff($gametimer)
   If Number($gametime,3) / (1000*60*60) > 1 Then
	  RestartGame()
	  $gametimer = TimerInit()
   EndIf

   Local $shouldkeep = ShouldKeep()
   If $shouldkeep Then
	  MoveItemToInventory($column,$row)
	  $column = $column + $itemwidth
	  if $column >= 8 Then
	   $column = 0
	   $row = $row + $itemheight
	  EndIf
	  If $row > 3 + $itemheight - 1 Then
		 Exit
	  EndIf
	  If $row >= 3 + $itemheight - 1 and $column >= 7 + $itemwidth - 1 Then
		 Exit
	  EndIf
	  Sleep(500)
	  MoveItemFromInventory($column,$row)
   Else
	  MouseClick("left",239,338)
	  MouseMove($InitialMouseLoc[0],$InitialMouseLoc[1])
	  Sleep(50)
   Endif

WEnd
EndFunc

Func End()
Exit
EndFunc
Func TogglePause()
$Paused = NOT $Paused
While $Paused
sleep(100)
WEnd
EndFunc