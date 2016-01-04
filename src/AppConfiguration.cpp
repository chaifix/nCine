#include "AppConfiguration.h"
#include "IFile.h"

namespace ncine {

///////////////////////////////////////////////////////////
// CONSTRUCTORS and DESTRUCTOR
///////////////////////////////////////////////////////////

/// Empty constructor setting the defaults
AppConfiguration::AppConfiguration()
	: profileTextUpdateTime_(0.2f)
	, logFile_(128)
	, consoleLogLevel_(ILogger::LOG_VERBOSE)
	, fileLogLevel_(ILogger::LOG_OFF)
	, frameTimerLogInterval_(5.0f)
	, xResolution_(960), yResolution_(640)
	, inFullscreen_(false)
	, windowTitle_(128)
	, fontTexFilename_(128)
	, fontFntFilename_(128)
	, withProfilerGraphs_(true)
	, withProfilerText_(true)
	, withAudio_(true)
	, withThreads_(true)
	, withScenegraph_(true)
{
	logFile_ = "ncine_log.txt";
#ifdef __ANDROID__
	IFile::dataPath_ = "/sdcard/ncine/";
	fileLogLevel_ = ILogger::LOG_VERBOSE;

//	fontTexFilename_ = "asset::trebuchet16_128_L8.pvr.mp3";
//	fontFntFilename_ = "asset::trebuchet16_128.fnt";
	fontTexFilename_ = "trebuchet32_256_L8.pvr";
	fontFntFilename_ = "trebuchet32_256.fnt";
//	fontTexFilename_ = "trebuchet16_128_L8.pvr";
//	fontFntFilename_ = "trebuchet16_128.fnt";
#else
	windowTitle_ = "nCine";
	fontTexFilename_ = "fonts/trebuchet32_256_gray.png";
	fontFntFilename_ = "fonts/trebuchet32_256.fnt";
#endif

#ifdef NCINE_DEFAULT_DATA_DIR
	IFile::dataPath_ = NCINE_DEFAULT_DATA_DIR;
#endif
}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

/// Sets the path for the application to load data from
/*! Directly sets the value of the static field 'IFile::dataPath_' */
void AppConfiguration::setDataPath(const String& dataPath)
{
	IFile::dataPath_ = dataPath;
}

/// Sets the name of the log file
/*! The data path will be prefixed automatically before loading */
void AppConfiguration::setLogFile(const String& logFile)
{
	logFile_ = logFile;
}

/// Sets the logging level for messages printed on the console
void AppConfiguration::setConsoleLogLevel(ILogger::LogLevel logLevel)
{
	consoleLogLevel_ = logLevel;
}

/// Sets the logging level for messages written in the log file
void AppConfiguration::setFileLogLevel(ILogger::LogLevel logLevel)
{
	fileLogLevel_ = logLevel;
}

/// Sets the interval for frame timer accumulation average and log
/*! A value of 0 disables frame timer logging */
void AppConfiguration::setFrameTimerLogInterval(float logInterval)
{
	frameTimerLogInterval_ = logInterval;
}

/// Sets the screen resolution
void AppConfiguration::setResolution(unsigned int x, unsigned int y)
{
	xResolution_ = x;
	yResolution_ = y;
}

/// Sets the fullscreen mode
void AppConfiguration::setFullScreen(bool inFullscreen)
{
	inFullscreen_ = inFullscreen;
}

/// Sets the window title
void AppConfiguration::setWindowTitle(const String& windowTitle)
{
	windowTitle_ = windowTitle;
}

/// Sets the font texture filename for profiler information text
/*! The data path will be prefixed automatically before loading */
void AppConfiguration::setFontTexFilename(const String& fontTexFilename)
{
	fontTexFilename_ = fontTexFilename;
}

/// Sets the font FNT filename for profiler information text
/*! The data path will be prefixed automatically before loading */
void AppConfiguration::setFontFntFilename_(const String& fontFntFilename)
{
	fontFntFilename_ = fontFntFilename;
}

/// Enables the profiler graphs
void AppConfiguration::enableProfilerGraphs(bool shouldEnable)
{
	withProfilerGraphs_ = shouldEnable;
}

/// Enables the profiler information text
void AppConfiguration::enableProfilerText(bool shouldEnable)
{
	withProfilerText_ = shouldEnable;
}

/// Enables the audio subsystem
void AppConfiguration::enableAudio(bool shouldEnable)
{
	withAudio_ = shouldEnable;
}

/// Enables the threading subsystem
void AppConfiguration::enableThreads(bool shouldEnable)
{
	withThreads_ = shouldEnable;
}

/// Enables the scenegraph based rendering
void AppConfiguration::enableScenegraph(bool shouldEnable)
{
	withScenegraph_ = shouldEnable;
}

}