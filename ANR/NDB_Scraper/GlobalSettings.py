# GlobalSetting.py
import os

# Global settings
NDB_Prefix = "https://netrunnerdb.com/api/2.0/public/"
NDB_CardsRequest = 1
NDB_RequestList = ['types', 'cards', 'packs', 'cycles', 'mwl']

ScrapeFolder = "./Intermediate/Scrapes/"
ScrapeSuffix = ".ndb"
ScrapeDataFileName = "ScrapeData.dat"
ScrapeFrequency = 3 # Measured in days

ImageFolder = "./Intermediate/Images/"
ImageUrlTemplateKey = "imageUrlTemplate"
ImageUrlOverrideKey = "image_url"
ScrapeImageIdealSize = (300, 419)
JSONDataKey = "data"

OutputFolder = "./Output/"
DatabaseName = "NetrunnerDB.db"
SQLColumnSuffixes = { "code" : "PRIMARY KEY" }

def BuildFolderPaths():
	if not os.path.exists(ScrapeFolder):
		os.makedirs(ScrapeFolder)
	if not os.path.exists(ImageFolder):
		os.makedirs(ImageFolder)
	if not os.path.exists(OutputFolder):
		os.makedirs(DatabaseFolder)

		
def GetSQLColumnWithConstraints(ColumnNameAndType):
	RetVal = ColumnNameAndType
	SearchValue = ColumnNameAndType.split(' ', 1)[0]

	if SearchValue in SQLColumnSuffixes:
		RetVal += " " + SQLColumnSuffixes[SearchValue]

	return RetVal

