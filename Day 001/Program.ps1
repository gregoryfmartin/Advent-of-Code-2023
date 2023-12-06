[Int]$CalibrationSum   = 0
[Int]$TotalNumLines    = 1000
[Int]$CurrentParseLine = 0
[String[]]$Randos      = @(
    'Baking cookies...',
    'Decorating trees...',
    'Feeding the reindeer...',
    'Making macaroni pictures...',
    'Stealing presents from your neighbor...',
    'Blowing snow...',
    'CANDY CANES!!!'
)

# I REALLY DON'T LIKE THIS SOLUTION, BUT I'LL DO IT ANYWAY
# OTHERWISE I MAY HAVE TO PING-PONG A REGEX, BUT MULTIPLE TARGETED REPLACEMENT VALUES AREN'T WORKING FOR ME
# FUCK THE OVERLAP, BTW
[Hashtable]$LitMaps = @{
    'one'   = 'one1one'
    'two'   = 'two2two'
    'three' = 'three3three'
    'four'  = 'four4four'
    'five'  = 'five5five'
    'six'   = 'six6six'
    'seven' = 'seven7seven'
    'eight' = 'eight8eight'
    'nine'  = 'nine9nine'
}

Write-Host '🎄🎅Because art elves...🎅🎄'
Write-Progress -Activity 'Calibrating' -Id 1 -Status "$($Randos | Get-Random)" -PercentComplete 0
Get-Content '.\DataSource.txt' | ForEach-Object {
    $CurrentParseLine++
    Write-Progress -Activity 'Calibrating' -Id 1 -Status "$($Randos | Get-Random)" -PercentComplete $(($CurrentParseLine / $TotalNumLines * 100))

    [String]$Res = $_
    Foreach($LitMapEntry in $LitMaps.GetEnumerator()) {
        $Res = $Res -REPLACE "($($LitMapEntry.Key))", "$($LitMapEntry.Value)"
    }

    $Res = $Res -REPLACE "[^0-9]", ''

    ($Res.Length -EQ 1) ? ($CalibrationSum += [Convert]::ToInt16("$($Res)$($Res)")) : ($CalibrationSum += [Convert]::ToInt16("$($Res[0])$($Res[$Res.Length - 1])")) | Out-Null
}
# Write-Progress -Activity 'Calibrating' -Id 1 -Completed
Write-Host "Calibration Complete! Value is $($CalibrationSum)."