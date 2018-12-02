from PIL import Image
from math import floor
from io import BytesIO
import json
import GlobalSettings
import requests
import os.path



# Used to splice files into one sprite sheet for use later
class SpriteSheetManager():

	# param MaxSpriteSize: Max size of a sprite in the sheet
	# param MaxSheetSize: Max dimensions of the sheet before wrapping
	def __init__(self, MaxSpriteSize, MaxSheetSize):
		self._MaxSpriteSize = MaxSpriteSize
		self._MaxSheetSize = (MaxSheetSize[0] - (MaxSheetSize[0] % MaxSpriteSize[0]), MaxSheetSize[1] - (MaxSheetSize[1] % MaxSpriteSize[1]))
		self._SpriteSheet = Image.new('RGBA', self._MaxSpriteSize)
		self._CurrentSpriteCount = 0
		self._SpritesPerRow = self._MaxSheetSize[0] // self._MaxSpriteSize[0]

	# Inserts gap into sprite sheet
	def AddEmptySprite(self):
		self._CurrentSpriteCount = self._CurrentSpriteCount + 1

	# Adds the image in the given file to the spritesheet as the
	# next sprite. Sprites are added to the sheet in the same order
	# a book is read, and the Sheet will expand as necessary to fit.
	# param File: path to the new sprite
	# return: whether the sprite was successfully added
	def AddSprite(self, File, LeaveSpaceForFails=True):
		try:
			NewSprite = Image.open(File)

			# Handle sprites that are too big
			(NewSpriteWidth, NewSpriteHeight) = NewSprite.size
			if (NewSpriteWidth > self._MaxSpriteSize[0] or NewSpriteHeight > self._MaxSpriteSize[1]):
				NewSprite = NewSprite.resize((self._MaxSpriteSize), Image.LANCZOS)
				(NewSpriteWidth, NewSpriteHeight) = NewSprite.size

			PastePositionOnSheet = self.GetSpritePosition(self._CurrentSpriteCount)
			ImageSizeOnSheet = (min(self._MaxSpriteSize[0], NewSpriteWidth), min(self._MaxSpriteSize[1], NewSpriteHeight))
			PasteBoxOnSheet = (PastePositionOnSheet[0], PastePositionOnSheet[1], PastePositionOnSheet[0] + ImageSizeOnSheet[0], PastePositionOnSheet[1] + ImageSizeOnSheet[1])

			# Handle expansion of sheet
			MinSheetSize = (max(PasteBoxOnSheet[2], self._SpriteSheet.size[0]), max(PasteBoxOnSheet[3], self._SpriteSheet.size[1]))
			if (MinSheetSize[0] > self._SpriteSheet.size[0] or MinSheetSize[1] > self._SpriteSheet.size[1]):
				if (MinSheetSize[0] > self._MaxSheetSize[0] or MinSheetSize[1] > self._MaxSheetSize[1]):
					# Our sheet is at max size. We need a better way to report this.
					return False
				FinalSheet = Image.new('RGBA', MinSheetSize)
				FinalSheet.paste(im=self._SpriteSheet, box = (0, 0))
				self._SpriteSheet = FinalSheet

			self._SpriteSheet.paste(im=NewSprite, box = PasteBoxOnSheet)
			self._CurrentSpriteCount += 1
			return True

		except Exception as e:
			if LeaveSpaceForFails:
				self._CurrentSpriteCount += 1

			print("Unable to add sprite " + File)
			print(e)
			return False		

	# Returns the position in pixel space of the index-th sprite
	def GetSpritePosition(self, index):
		x = (index % self._SpritesPerRow) * self._MaxSpriteSize[0]
		y = (floor(index / self._SpritesPerRow)) * self._MaxSpriteSize[1]
		return (x, y)

	# Getter for the Sprite Sheet
	@property
	def SpriteSheet(self):
		return self._SpriteSheet

# END SPRITE SHEET CLASS ------------------------------------------------------------------------------


# Builds sprite sheets of the card images that exist in the resource folder
def BuildSheets(MaxSheetSize):
    
    SheetManager = SpriteSheetManager(GlobalSettings.ScrapeImageIdealSize, (MaxSheetSize, MaxSheetSize))
    SheetEndCode = 0
    LastCompletedSheetCode = 0

    try:
        with open(GlobalSettings.ScrapeFolder + GlobalSettings.NDB_RequestList[GlobalSettings.NDB_CardsRequest] + GlobalSettings.ScrapeSuffix, "r", encoding="utf8") as File:
            CardInfo = json.load(File)
            GenericUrlTemplate = CardInfo[GlobalSettings.ImageUrlTemplateKey]

            CardData = CardInfo[GlobalSettings.JSONDataKey]
            for Card in CardData:
                CardImageUrl = ParseUrl(Card, GenericUrlTemplate)
                if CardImageUrl == "":
                    continue
                IndexOfFileTypeSeparator = CardImageUrl.rfind(".")
                ImagePath = GlobalSettings.ImageFolder + Card["code"] + CardImageUrl[IndexOfFileTypeSeparator:]
                    

                if not SheetManager.AddSprite(ImagePath):
                    SheetManager.SpriteSheet.save(GlobalSettings.OutputFolder + SheetEndCode + ".png")
                    LastCompletedSheetCode = SheetEndCode
                        
                    print("Sheet {} Complete".format(SheetEndCode))
                    SheetManager = SpriteSheetManager(GlobalSettings.ScrapeImageIdealSize, (MaxSheetSize, MaxSheetSize))
                    if not SheetManager.AddSprite(ImagePath):
                        break

                SheetEndCode = Card["code"]

        if SheetEndCode != LastCompletedSheetCode:
            SheetManager.SpriteSheet.save(GlobalSettings.OutputFolder + SheetEndCode + ".png")
            print("Final Sheet {} Complete!".format(SheetEndCode))
        return True

    except Exception as e:
        print(e)
        return -1


# Scrapes all of the card images
# Returns whether 0 or 1 if no or any new cards were scraped respectively, -1 if we aborted
def ScrapeImages():
	# We need to load the json and iterate through it using either the imageURLTemplate or the image_url as appropriate to find the image
	# Then save it to the Sprite Sheet in a way that makes it easier to look up card images by code

	# Load json with card data
	DidScrapeAnyImages = 0

	try:
		with open(GlobalSettings.ScrapeFolder + GlobalSettings.NDB_RequestList[GlobalSettings.NDB_CardsRequest] + GlobalSettings.ScrapeSuffix, "r", encoding="utf8") as File:
			CardInfo = json.load(File)
			GenericUrlTemplate = CardInfo[GlobalSettings.ImageUrlTemplateKey]

			CardData = CardInfo[GlobalSettings.JSONDataKey]
			for Card in CardData:
				for _ in range(0, 3):
					try:
						CardImageUrl = ParseUrl(Card, GenericUrlTemplate)
						if CardImageUrl == "":
							continue
						IndexOfFileTypeSeparator = CardImageUrl.rfind(".")
						FinalPath = GlobalSettings.ImageFolder + Card["code"] + CardImageUrl[IndexOfFileTypeSeparator:]

						# Only bother attempting to get a file if it doesn't exist
						if not os.path.isfile(FinalPath):
							RequestPacket = requests.get(CardImageUrl, timeout=10.0)
							RequestPacket.raise_for_status()

							CardImage = Image.open(BytesIO(RequestPacket.content))
					
							CardImage.save(FinalPath)
							DidScrapeAnyImages = 1
							print("Saved image for card " + Card["code"] + "...")
					except requests.HTTPError as e:
						print("Image for card " + Card["code"] + " does not exist!")
						break
					except requests.ConnectTimeout as e:
						print(e)
					else:
						break
				else:
					raise requests.exceptions.RetryError("Image Scrape failed 3 times in a row due to connection timeouts!")

		print("Image Scrape Complete!")
		return DidScrapeAnyImages

	except requests.RequestException as e:
		print(e)
		print("Aborting Image Scrape...")
		return -1


# Returns the url of this card's image.
# Card is the dict of the data for that card
# GenericTemplate is the fall back if the override is not found
# Returns empty string upon failure.
def ParseUrl(Card, GenericTemplate):

	Result = GenericTemplate

	if Card.get(GlobalSettings.ImageUrlOverrideKey) != None:
		Result = Card.get(GlobalSettings.ImageUrlOverrideKey)
	else:
		ShouldContinue = True
		while (ShouldContinue):
			OpenBracketIndex = Result.find("{", 0)
			ClosedBracketIndex = Result.find("}", 0)
			ShouldContinue = OpenBracketIndex != -1 and ClosedBracketIndex != -1

			if (ShouldContinue):
				DesiredDictionaryLookupString = GenericTemplate[OpenBracketIndex + 1 : ClosedBracketIndex]
				FinalReplacement = Card.get(DesiredDictionaryLookupString)
				if (FinalReplacement == None):
					FinalReplacement = ""

				Result = Result[:OpenBracketIndex] + FinalReplacement + Result[ClosedBracketIndex + 1:]

	return Result

