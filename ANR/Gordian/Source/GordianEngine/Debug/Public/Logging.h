// Gordian by Daniel Luna (2019)

#pragma once

#include "LogMacros.h"
#include "LogCategory.h"
#include "LogVerbosity.h"

// Used for temporary logs
DECLARE_LOG_CATEGORY_EXTERN(LogTemp, Log, Verbose);
// Used by file i/o
DECLARE_LOG_CATEGORY_EXTERN(LogFileIO, Log, Verbose);
// Used by core engine loop
DECLARE_LOG_CATEGORY_EXTERN(LogCore, Log, Verbose);
