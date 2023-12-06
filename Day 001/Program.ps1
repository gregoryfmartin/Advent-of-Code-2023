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

Write-Host '🎄🎅Because art elves...🎅🎄'
Get-Content '.\DataSource.txt' | ForEach-Object {
    $CurrentParseLine++
    Write-Progress -Activity 'Calibrating' -Id 1 -Status "$($Randos | Get-Random)" -PercentComplete $(($CurrentParseLine / $TotalNumLines * 100))
    [String]$Res = $_ -REPLACE "[^0-9]", ''
    ($Res.Length -EQ 1) ? ($CalibrationSum += [Convert]::ToInt16("$($Res)$($Res)")) : ($CalibrationSum += [Convert]::ToInt16("$($Res[0])$($Res[$Res.Length - 1])")) | Out-Null
}
Write-Progress -Activity 'Calibrating' -Id 1 -Completed
Write-Host "Calibration Complete! Value is $($CalibrationSum)."