#ifndef CLASS_NCSPRITE
#define CLASS_NCSPRITE

#include "ncSceneNode.h"
#include "ncTexture.h"
#include "ncRect.h"

/// A scene node representing a basic sprite
class ncSprite : public ncSceneNode
{
private:
	/// The sprite texture
	ncTexture *m_pTexture;
	/// Used as source rectangle by the sprite batch class
	ncRect m_texRect;
	/// Sprite width in pixel
	int m_iWidth;
	/// Sprite height in pixel
	int m_iHeight;
	/// Scale factor for sprite size
	float m_fScaleFactor;
public:
	ncSprite(ncSceneNode* pParent, ncTexture *pTexture, int iX, int iY);
	ncSprite(ncTexture *pTexture, int iX, int iY);
	virtual ~ncSprite() { }

	/// Return sprite width
	inline int Width() const { return m_iWidth * m_fScaleFactor; }
	/// Return sprite height
	inline int Height() const { return m_iHeight * m_fScaleFactor; }
	/// Return sprite size
	inline ncPoint Size() const
	{
		return ncPoint(m_iWidth*m_fScaleFactor, m_iHeight*m_fScaleFactor);
	}

	/// Get the sprite scale factor
	inline float Scale() const { return m_fScaleFactor; }
	/// Scale the sprite size
	inline void SetScale(float fScaleFactor) { m_fScaleFactor = fScaleFactor; }

	/// Get the texture source rectangle for blitting
	inline ncRect TexRect() const { return m_texRect; }
	/// Set the texture source rectangle for blitting
	inline void SetTexRect(const ncRect& rect) { m_texRect = rect; }

	virtual void Update(unsigned long int ulInterval);

	inline static eObjectType sType() { return SPRITE_TYPE; }
	static ncSprite* FromId(unsigned int uId);

	friend class ncRenderGraph;
};

#endif
