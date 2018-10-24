import requests
import io
import sys
from datetime import date
import json
import GlobalSettings



# Given an array of requests to make of the Netruner Data Base, 
#   this function will call them and save them to a resource 
#   file with the same name as the command
def ScrapeNDB(DesiredRequests):
	try:
		for Request in DesiredRequests:
			for _ in range(0, 3):
				try:
					print("Scraping " + Request + "...", end="")

					RequestPacket = requests.get(GlobalSettings.NDB_Prefix + Request, timeout=10.0)
					RequestPacket.raise_for_status()

					FileName = GlobalSettings.ScrapeFolder + Request + GlobalSettings.ScrapeSuffix
					with open(FileName, "w", encoding="utf-8") as File:
						File.write(RequestPacket.text)

					print('Complete!')
					break
				except requests.ConnectTimeout as e:
					print()
					print(e)
			else:
				raise requests.exceptions.RetryError("Netrunner Database Scrape failed 3 times in a row due to a connection timeout!")
	
	except requests.RequestException as e:
		print()
		print(e)
		return 1

	print("Full Scrape Complete!")		
	return 0


# Returns the number of days since a successful scrape
def GetDaysSinceSuccessfulJSONScrape():
    LastSuccessfulScrape = date(2000, 1, 1)
    try:
        with open(GlobalSettings.ScrapeFolder + GlobalSettings.ScrapeDataFileName, "r") as File:
            FileDataAsArray = json.load(File)
            LastSuccessfulScrape = date(FileDataAsArray[2], FileDataAsArray[1], FileDataAsArray[0])
    except FileNotFoundError:
        pass
    finally:
        CurrentDate = date.today()
        return (CurrentDate - LastSuccessfulScrape).days



# Scrapes NDB for json data
def CheckForJSONScrape():
	DaysSinceSuccessfulJSONScrape = GetDaysSinceSuccessfulJSONScrape()
	bShouldScrapeJSON = DaysSinceSuccessfulJSONScrape > GlobalSettings.ScrapeFrequency

	if '-f' in sys.argv:
		bShouldScrapeJSON = True

	if bShouldScrapeJSON:
		# Pull all json files
		ErrorCode = ScrapeNDB(GlobalSettings.NDB_RequestList)

		if (ErrorCode == 0):
			# Be sure to note the date for later scrapes
			CurrentDate = date.today()
			with open(GlobalSettings.ScrapeFolder + GlobalSettings.ScrapeDataFileName, "w") as File:
				json.dump([CurrentDate.day, CurrentDate.month, CurrentDate.year], File)
	else:
		print("Last successful JSON scrap was within " + str(DaysSinceSuccessfulJSONScrape) + " days, skipping...")

