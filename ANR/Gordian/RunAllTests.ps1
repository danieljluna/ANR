[string[]] $TestFiles = "GordianTests.exe"
[string[]] $Configurations = "Debug","Test","Release"

foreach ($Config in $Configurations) 
{
	foreach ($Test in $TestFiles)
	{
		$ParentFolder = (Get-Item $PSScriptRoot).parent
		$TestFullPath = Join-Path $ParentFolder $Config $Test
		
		if (Test-Path $TestFullPath)
		{
			$OutputText = "Testing " + $Config + " Config..."
			Write-Output $OutputText
			& $TestFullPath --reporter compact --warn NoAssertions --invisibles --rng-seed 356 --order rand $args
		}
		else
		{
			$WarningText = "No compiled tests exist for configuration " + $Config
			Write-Error $WarningText
		}
	}
}