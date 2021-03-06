#include "AudioLoaderOgg.h"

#ifdef __ANDROID__
	#include "AssetFile.h" // for `AssetFile::sType()`
#endif

namespace ncine {

#ifdef __ANDROID__
namespace {
	size_t asset_read(void *ptr, size_t size, size_t nmemb, void *datasource)
	{
		AssetFile *assetFile = static_cast<AssetFile *>(datasource);
		return assetFile->read(ptr, size * nmemb);
	}

	int asset_seek(void *datasource, ogg_int64_t offset, int whence)
	{
		AssetFile *assetFile = static_cast<AssetFile *>(datasource);
		return assetFile->seek(offset, whence);
	}

	int asset_close(void *datasource)
	{
		AssetFile *assetFile = static_cast<AssetFile *>(datasource);
		assetFile->close();
		return 0;
	}

	long asset_tell(void *datasource)
	{
		AssetFile *assetFile = static_cast<AssetFile *>(datasource);
		return assetFile->tell();
	}

	const ov_callbacks oggCallbacks = { asset_read, asset_seek, asset_close, asset_tell };
}
#endif

///////////////////////////////////////////////////////////
// CONSTRUCTORS and DESTRUCTOR
///////////////////////////////////////////////////////////

AudioLoaderOgg::AudioLoaderOgg(const char *filename)
    : AudioLoaderOgg(IFile::createFileHandle(filename))
{
}

AudioLoaderOgg::AudioLoaderOgg(nctl::UniquePtr<IFile> fileHandle)
    : IAudioLoader(nctl::move(fileHandle))
{
	LOGI_X("Loading \"%s\"", fileHandle_->filename());

	// File is closed by `ov_clear()`
	fileHandle_->setCloseOnDestruction(false);

#ifdef __ANDROID__
	if (fileHandle_->type() == AssetFile::sType())
	{
		fileHandle_->open(IFile::OpenMode::FD | IFile::OpenMode::READ);

		if (ov_open_callbacks(fileHandle_.get(), &oggFile_, nullptr, 0, oggCallbacks) != 0)
		{
			LOGF_X("Cannot open \"%s\" with ov_open_callbacks()", fileHandle_->filename());
			fileHandle_->close();
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		fileHandle_->open(IFile::OpenMode::READ | IFile::OpenMode::BINARY);

		if (ov_open(fileHandle_->ptr(), &oggFile_, nullptr, 0) != 0)
		{
			LOGF_X("Cannot open \"%s\" with ov_open()", fileHandle_->filename());
			fileHandle_->close();
			exit(EXIT_FAILURE);
		}
	}
#else
	const int err = ov_fopen(fileHandle_->filename(), &oggFile_);
	FATAL_ASSERT_MSG_X(err == 0, "Cannot open \"%s\" with ov_fopen()", fileHandle_->filename());
#endif

	// Get some information about the OGG file
	const vorbis_info *info = ov_info(&oggFile_, -1);

	bytesPerSample_ = 2; // Ogg is always 16 bits
	numChannels_ = info->channels;
	frequency_ = info->rate;

	numSamples_ = static_cast<unsigned long int>(ov_pcm_total(&oggFile_, -1));
	duration_ = float(ov_time_total(&oggFile_, -1));

	LOGI_X("duration: %.2fs, channels: %d, frequency: %dHz", duration_, numChannels_, frequency_);
}

AudioLoaderOgg::~AudioLoaderOgg()
{
	ov_clear(&oggFile_);
}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

unsigned long int AudioLoaderOgg::read(char *buffer, unsigned long int bufferSize) const
{
	ASSERT(buffer);
	ASSERT(bufferSize > 0);

	static int bitStream = 0;
	long bytes = 0;
	unsigned long int bufferSeek = 0;

	do
	{
		// Read up to a buffer's worth of decoded sound data
		// (0: little endian, 2: 16bit, 1: signed)
		bytes = ov_read(&oggFile_, buffer + bufferSeek, bufferSize - bufferSeek, 0, 2, 1, &bitStream);

		if (bytes < 0)
		{
			ov_clear(&oggFile_);
			FATAL_MSG_X("Error decoding at bitstream %d", bitStream);
		}

		// Reset the static variable at the end of a decoding process
		if (bytes <= 0)
			bitStream = 0;

		bufferSeek += bytes;
	} while (bytes > 0 && bufferSize - bufferSeek > 0);

	return bufferSeek;
}

void AudioLoaderOgg::rewind() const
{
	ov_raw_seek(&oggFile_, 0);
}

}
