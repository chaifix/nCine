#ifndef NO_GLEW
	#include <GL/glew.h>
#else
	#include <GL/gl.h>
	#include <GL/glext.h>
	#include <GL/glu.h>
#endif

#include "Texture.h"
#include "../ServiceLocator.h"

///////////////////////////////////////////////////////////
// CONSTRUCTORS and DESTRUCTOR
///////////////////////////////////////////////////////////

Texture::Texture() : m_uId(0), m_iWidth(0), m_iHeight(0)
{
	glGenTextures(1, &m_uId);
}


Texture::Texture(const char *pFilename) : m_uId(0), m_iWidth(0), m_iHeight(0)
{
	glGenTextures(1, &m_uId);
	Bind();
	Load(pFilename);
}


Texture::Texture(SDL_Surface *pImage) : m_uId(0)
{
	glGenTextures(1, &m_uId);
	Bind();
	Load(pImage);
}


Texture::Texture(TextureFormat format, int iWidth, int iHeight, GLubyte *pPixels) : m_uId(0)
{
	glGenTextures(1, &m_uId);
	Bind();
	Load(format, iWidth, iHeight, pPixels);
}


Texture::~Texture()
{
	glDeleteTextures(1, &m_uId);
}

///////////////////////////////////////////////////////////
// PUBLIC FUNCTIONS
///////////////////////////////////////////////////////////

/// Set texture filtering for both magnification and minification
void Texture::SetFiltering(GLenum eFilter)
{
	Bind();
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, eFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, eFilter);
}

///////////////////////////////////////////////////////////
// PRIVATE FUNCTIONS
///////////////////////////////////////////////////////////

void Texture::Load(const char *pFilename)
{
	SDL_Surface *pImage;

	ServiceLocator::GetLogger().Write(1, (char *)"Texture::Load - Loading \"%s\"", pFilename);
	if (!(pImage = (SDL_Surface *)IMG_Load(pFilename))) {
		ServiceLocator::GetLogger().Write(2, (char *)"Texture::Load - Cannot load \"%s\"", pFilename);
		exit(-1);
	}

	Load(pImage);

	SDL_FreeSurface(pImage);
}


void Texture::Load(SDL_Surface *pSurface)
{
	Uint8 uBpp;

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	uBpp = pSurface->format->BitsPerPixel;
	TextureFormat *pFormat = 0;
	if (uBpp == 32)
		pFormat = new TextureFormat(GL_RGBA8);
	else if (uBpp  == 24)
		pFormat = new TextureFormat(GL_RGB8);
	else {
		ServiceLocator::GetLogger().Write(2, (char *)"Texture::Load - Not a true color image: %d", uBpp);
		exit(-1);
	}

	GLenum eFormat;
	if (pSurface->format->Rmask == 0x000000ff)
		eFormat = pFormat->Format();
	else
		eFormat = pFormat->BGRFormat();

	m_iWidth = pSurface->w;
	m_iHeight = pSurface->h;
	glTexImage2D(GL_TEXTURE_2D, 0, pFormat->Internal(), m_iWidth, m_iHeight, 0, pFormat->Format(), pFormat->Type(), pSurface->pixels);
}


void Texture::Load(TextureFormat format, int iWidth, int iHeight, GLubyte *pPixels)
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	m_iWidth = iWidth;
	m_iHeight = iHeight;
	glTexImage2D(GL_TEXTURE_2D, 0, format.Internal(), m_iWidth, m_iHeight, 0, format.Format(), format.Type(), pPixels);
}