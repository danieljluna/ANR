import GlobalSettings
import sqlite3
import json


def BuildDatabase():
	# Use the jsons to build one SQLite database with all the info
	# Each request will be stored in a different table
	
	print("Building Database Tables:")

	try:
		connection = sqlite3.connect(GlobalSettings.OutputFolder + GlobalSettings.DatabaseName)
		cursor = connection.cursor()

		for RequestName in GlobalSettings.NDB_RequestList:
			print("     '" + RequestName + "' table...", end="")
			with open(GlobalSettings.ScrapeFolder + RequestName + GlobalSettings.ScrapeSuffix, "r", encoding="utf8") as File:
				RequestJSON = json.load(File)
				JSONData = RequestJSON[GlobalSettings.JSONDataKey]

				ParseJSONDictIntoTable(cursor, RequestName, JSONData)

				# Save changes
				connection.commit()
			print("Complete")

		connection.close()
			
	except Exception as e:
		print("Failed!")
		print(e)

def ParseJSONDictIntoTable(SQLCursor, DesiredTableName, JSONData):
	TableColumns = CreateDynamicTableForJSON(SQLCursor, DesiredTableName, JSONData)
	AddEntriesToDynamicTable(SQLCursor, DesiredTableName, TableColumns, JSONData)


# Parses a list of JSON entries into an SQL database
def AddEntriesToDynamicTable(SQLCursor, TableName, TableColumns, JSONList):
	AllEntries = []
	FormattedFillerForColumns = CreateNLongString(len(TableColumns), "?", ", ")

	TableColumnsString = ""
	for TableColumn in TableColumns:
		if (TableColumnsString != ""):
			TableColumnsString += ", "
		TableColumnsString += TableColumn

	for JSONEntry in JSONList:
		EntryAsList = []
		for Column in TableColumns:
			if Column in JSONEntry:
				EntryAsList.append(JSONEntry[Column])
			else:
				EntryAsList.append(None)

		EntryAsTuple = tuple(EntryAsList)
		AllEntries.append(EntryAsTuple)

	SQLCursor.executemany("REPLACE INTO " + TableName + " (" + TableColumnsString + ") VALUES (" + FormattedFillerForColumns + ")", AllEntries)


# Creates a table with all the necessary fields for each item in the list
def CreateDynamicTableForJSON(SQLCursor, TableName, JSONList):
	TableColumnsWithTypes = GetTableColumnsWithTypes(SQLCursor, TableName, JSONList)
	FormattedFillerForColumns = CreateNLongString(len(TableColumnsWithTypes), "?", ", ")

	if not DoesTableExist(SQLCursor, TableName, TableColumnsWithTypes):
		# Create table
		SQLCommand = "CREATE TABLE " + TableName + " ("
		for i in range(0, len(TableColumnsWithTypes)):
			if i != 0:
				SQLCommand += ", "
			SQLCommand += GlobalSettings.GetSQLColumnWithConstraints(TableColumnsWithTypes[i])
		SQLCommand += ")"
		SQLCursor.execute(SQLCommand)

		
	TableColumns = []

	# Trim type data from return value
	for TableColumnWithType in TableColumnsWithTypes:
		TableColumn = TableColumnWithType.split(' ', 1)[0]
		TableColumns.append(TableColumn)

	return TableColumns



# Parses through a list of JSON objects and returns every key that shows
#	up in any JSON object from the list. If it finds a dict, it will
#	build a table for it first
def GetTableColumnsWithTypes(SQLCursor, TableName, ListOfDicts):
	SetOfKeys = set()
	ListOfColumns = []

	if type(ListOfDicts) is not list:
		print("Warning: the table " + TableName + " is not a list (" + str(type(ListOfDicts)) + ") and could not be parsed!")
		return []

	for JSONObject in ListOfDicts:
		if type(JSONObject) is not dict:
			print("Warning: the table " + TableName + " contained a non-dictionary entry (" + str(type(JSONObject)) + ") which could not be parsed!")
			continue

		for key,value in JSONObject.items():
			if (key not in SetOfKeys) and (value != None):
				ValueType = type(value)
				if ValueType is int:
					ListOfColumns.append(key + " INTEGER")
					SetOfKeys.add(key)
				elif ValueType is bool:
					ListOfColumns.append(key + " INTEGER")
					SetOfKeys.add(key)
				elif ValueType is float:
					ListOfColumns.append(key + " REAL")
					SetOfKeys.add(key)
				elif ValueType is str:
					ListOfColumns.append(key + " TEXT")
					SetOfKeys.add(key)
				elif ValueType is bytes:
					ListOfColumns.append(key + " BLOB")
					SetOfKeys.add(key)
				# We have found a table inside of a table!
				# For the next two cases, we want to parse each inner table seperately, so we don't add the key to our key set
				elif (ValueType is dict) and ('id' in JSONObject):
					# We need to convert it to a list of objects if it is a dict
					for SubTableKey in value:
						value[SubTableKey]["code"] = SubTableKey
					JSONObject[key] = list(value.values())

					SubTableName = TableName + "_" + str(JSONObject['id']) + "_" + str(key)
					ParseJSONDictIntoTable(SQLCursor, SubTableName, JSONObject[key])
				elif ValueType is list and ('id' in JSONObject):
					# We have found a table inside of a table!
					SubTableName = TableName + "_" + str(JSONObject['id']) + "_" + str(key)
					ParseJSONDictIntoTable(SQLCursor, SubTableName, value)
				else:
					print("Warning: could not decipher json entry '" + key + "' of type " + str(ValueType) + " to SQLite type while parsing table " + TableName + ".")
					SetOfKeys.add(key)

	return ListOfColumns


# Creates a string that repeats RepeatingString N times, separated by
#	the DelimiterString if necessary
def CreateNLongString(N, RepeatingString, DelimiterString):
	RetVal = ""
	for i in range(0, N):
		if (i == 0):
			RetVal = RepeatingString
		else:
			RetVal = RetVal + DelimiterString + RepeatingString

	return RetVal


# Returns whether or not the table already exists. If it does, aligns 
#	TableColumnsWithTypes to match the existing ordering of columns.
#	This is actually expensive, so minimize calls
def DoesTableExist(SQLCursor, TableName, TableColumnsWithTypes):
	DoesTableExist = False
	SQLCursor.execute("SELECT name FROM sqlite_master WHERE type='table' AND name=?", [TableName])

	ExistingTable = SQLCursor.fetchone()
	if (ExistingTable != None):
		# We have a column of the same name, but if it doesn't align column wise, we need to nuke it
		DoesTableExist = True
		ShouldDropExistingTable = True
		SQLCursor.execute("PRAGMA TABLE_INFO(" + TableName + ")")
		ExistingTableColumnData = SQLCursor.fetchall()
		DesiredColumnsWithTypes = list(TableColumnsWithTypes)

		ShouldDropExistingTable = not SortTableColumnsToMatchSQLTable(DesiredColumnsWithTypes, ExistingTableColumnData)

		if ShouldDropExistingTable:
			SQLCursor.execute("DROP TABLE " + TableName)
			DoesTableExist = False

	return DoesTableExist


# Sorts info about a table's columns to match a SQL table provided if possible.
#	Returns whether there was a 1-1 successful sort
def SortTableColumnsToMatchSQLTable(TableColumnsToSort, ExistingSQLTableData):
	WasSortSuccessful = False

	if (len(TableColumnsToSort) == len(ExistingSQLTableData)):
		for i in range(0, len(ExistingSQLTableData)):
			IndexOfMatchingColumn = 0
			for j in range(i, len(TableColumnsToSort)):
				ColumnAsPair = TableColumnsToSort[j].split(' ', 1)
				if ColumnAsPair[0] == ExistingSQLTableData[i][1] and ColumnAsPair[1] == ExistingSQLTableData[i][2]:
					IndexOfMatchingColumn = j
					break
			else:
				# We found no match, stop searching
				break
			if (i != j):
				# Re-Order TableColumnsWithType to match
				TableColumnsToSort[j], TableColumnsToSort[i] = TableColumnsToSort[i], TableColumnsToSort[j]
		else:
			WasSortSuccessful = True

	return WasSortSuccessful


# Checks to see if table columns match the passed in existing SQL table
def CheckIfTableColumnsAreAMatch(TableColumnsToSort, ExistingSQLTableData):
	IsAMatch = False

	if (len(TableColumnsToSort) == len(ExistingSQLTableData)):
		for i in range(0, len(ExistingSQLTableData)):
			for j in range(0, len(TableColumnsToSort)):
				ColumnAsPair = TableColumnsToSort[j].split(' ', 1)
				if ColumnAsPair[0] == ExistingSQLTableData[i][1] and ColumnAsPair[1] == ExistingSQLTableData[i][2]:
					break
			else:
				# We found no match, stop searching
				break
		else:
			IsAMatch = True

	return IsAMatch
