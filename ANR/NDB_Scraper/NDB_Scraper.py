import ImageScraper
import RequestScraper
import GlobalSettings
import DatabaseBuilder

GlobalSettings.BuildFolderPaths()
RequestScraper.CheckForJSONScrape()
if ImageScraper.ScrapeImages():
    ImageScraper.BuildSheets(8192)
DatabaseBuilder.BuildDatabase()