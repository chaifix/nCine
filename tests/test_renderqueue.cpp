#ifndef NO_GLEW
	#include <GL/glew.h>
#else
	#include <GL/gl.h>
	#include <GL/glext.h>
#endif
#include <SDL/SDL.h>

#include <cmath>

#include "ncRect.h"
#include "ncFrameTimer.h"
#include "ncServiceLocator.h"
#include "ncFileLogger.h"
#include "ncSDLGfxDevice.h"
#include "ncTexture.h"
#include "ncRenderQueue.h"
#include "ncSceneNode.h"
#include "ncSprite.h"
#include "ncSpriteBatchNode.h"

//#define WITH_BATCH

int main(int argc, char **argv)
{
	SDL_Event event;
	int iWidth = 960;
	int iHeight = 640;
	bool bQuit = false;

	const int NUM_TEXTURES = 4;
	ncTexture *pTextures[NUM_TEXTURES];
	ncRect texRects[NUM_TEXTURES];
	const int NUM_SPRITES = 500;
	ncSprite *pSprites[NUM_SPRITES];
	int vRadius[NUM_SPRITES];
	int vX[NUM_SPRITES];
	int vY[NUM_SPRITES];

// ----- Init ----------------------
	float fAngle = 0.0f;
	float fAngle2 = 0.0f;
	ncFrameTimer t(5, 0);
	ncServiceLocator::RegisterLogger(new ncFileLogger("log.txt", ncILogger::LOG_VERBOSE, ncILogger::LOG_OFF));
	ncSDLGfxDevice gfxDevice(iWidth, iHeight);
	SDL_WM_SetCaption("Test", NULL);

	ncRenderQueue renderQueue;
	glEnable(GL_BLEND); // HACK: for alpha blending
	ncSceneNode rootNode;

#ifdef WITH_BATCH
	ncTexture *pMegaTexture = new ncTexture("megatexture.png");
	texRects[0] = ncRect(0, 0, 145, 121);
	texRects[1] = ncRect(256-100, 0, 100, 100);
	texRects[2] = ncRect(0, 256-96, 96, 96);
	texRects[3] = ncRect(256-96, 256-96, 96, 96);
	ncSpriteBatchNode spriteBatch(&rootNode, pMegaTexture);
#else
	pTextures[0] = new ncTexture("texture1.png");
	pTextures[1] = new ncTexture("texture2.png");
	pTextures[2] = new ncTexture("texture3.png");
	pTextures[3] = new ncTexture("texture4.png");
#endif

	for (int i = 0; i < NUM_SPRITES; i++)
	{
		vRadius[i] = 20 + rand() % 50;
		vX[i] = rand() % iWidth;
		vY[i] = rand() % iHeight;
		float scale = 0.35f + 0.1f * (rand() % 3);
#ifdef WITH_BATCH
		pSprites[i] = new ncSprite(&spriteBatch, pMegaTexture, vX[i], vY[i]);
		pSprites[i]->SetTexRect(texRects[rand() % NUM_TEXTURES]);
#else
		ncTexture *pTexture = pTextures[rand() % NUM_TEXTURES];
		pSprites[i] = new ncSprite(pTexture, vX[i], vY[i]);
		rootNode.AddChildNode(pSprites[i]);
#endif
		pSprites[i]->SetScale(scale);
	}

	t.Reset();


// ----- Event cycle --------------------
	while (!bQuit) {
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				bQuit = true;
				break;
			case SDL_KEYDOWN:
				switch( event.key.keysym.sym ){
				case SDLK_ESCAPE:
				case SDLK_q:
					bQuit = true;
					break;
				case SDLK_F1:
					gfxDevice.ToggleFullScreen();
					break;
				default:
					break;
				}
			}
		}


// ----- Blitting on the screen --------
		t.AddFrame();
		fAngle += 0.1f * t.Interval();
		fAngle2 += 0.25f * t.Interval();

		gfxDevice.Clear();

		float fSinus = sinf(fAngle * 0.01f);
		float fCosine = cosf(fAngle * 0.01f);
		float fSinus2 = sinf(fAngle2 * 0.01f);
		float fCosine2 = cosf(fAngle2 * 0.01f);

		for (int i = 0; i < NUM_SPRITES; i++)
		{
			pSprites[i]->x = vX[i] + fSinus2*vRadius[i];
			pSprites[i]->y = vY[i] + fCosine2*vRadius[i];
		}

		rootNode.Update(t.Interval());
		rootNode.Visit(renderQueue);
		renderQueue.Draw();

		gfxDevice.Update();
	}


// ----- Quitting ----------------------
//	SDL_Quit(); // GfxDevice should call this

	for (int i = 0; i < NUM_SPRITES; i++)
		delete pSprites[i];

#ifdef WITH_BATCH
	delete pMegaTexture;
#else
	for (int i = 0; i < NUM_TEXTURES; i++)
		delete pTextures[i];
#endif

	return 0;	
}