#ifndef CLASS_NCTEXTNODE
#define CLASS_NCTEXTNODE

#include "ncDrawableNode.h"
#include "ncRenderCommand.h"
#include "ncFont.h"

class ncFontGlyph;

/// A scene node to draw a text label
class ncTextNode : public ncDrawableNode
{
public:
	static const unsigned int MAX_LENGHT = 256;

	enum eAlignment {
		ALIGN_LEFT,
		ALIGN_CENTER,
		ALIGN_RIGHT
	};

private:
	/// The string to be rendered
	char m_vString[MAX_LENGHT];
	/// Dirty flag for vertices and texture coordinates
	bool m_bDirtyDraw;
	/// Dirty flag for boundary rectangle
	mutable bool m_bDirtyBoundaries;
	/// Kerning flag for rendering
	bool m_bWithKerning;
	/// The font class used to render text
	ncFont *m_pFont;
	/// Scale factor for glyph size
	float m_fScaleFactor;
	/// The array of vertices for every glyph in the batch
	ncArray<float> m_vVertices;
	/// The array of texture coordinates for every glyph in the batch
	ncArray<float> m_vTexCoords;

	/// Advance on the X-axis for the next processed glyph
	mutable float m_fXAdvance;
	/// Total advance on the X-axis for the longest line (horizontal boundary)
	mutable float m_fXAdvanceSum;
	/// Advance on the Y-axis for the next processed glyph
	mutable float m_fYAdvance;
	/// Total advance on the Y-axis for the entire string (vertical boundary)
	mutable float m_fYAdvanceSum;
	/// Text width for each line of text
	mutable ncArray<float> m_vLineLengths;
	/// Horizontal text alignment
	eAlignment m_alignment;

	// Calculates rectangle boundaries for the rendered text
	void CalculateBoundaries() const;
	// Calculates align offset for a particular line
	float CalculateAlignment(unsigned int uLineIndex) const;
	// Fills the batch draw command with data from a glyph
	void ProcessGlyph(const ncFontGlyph* pGlyph);

	virtual void UpdateRenderCommand();
public:
	ncTextNode(ncSceneNode* pParent, ncFont *pFont);
	virtual ~ncTextNode() { }

	// Returns rendered text width
	float Width() const;
	// Returns rendered text height
	float Height() const;
	/// Is kerning enabled for this node rendering?
	inline bool withKerning() const { return m_bWithKerning; }
	// Sets the kerning flag for this node rendering
	void EnableKerning(bool bWithKerning);
	/// Gets the horizontal text alignment
	inline eAlignment Alignment() const { return m_alignment; }
	// Sets the horizontal text alignment
	void SetAlignment(eAlignment alignment);

	/// Gets the glyph scale factor
	inline float Scale() const { return m_fScaleFactor; }
	/// Scales the glyph size
	inline void SetScale(float fScaleFactor)
	{
		m_fScaleFactor = fScaleFactor;
		m_bDirtyBoundaries = true;
		m_bDirtyDraw = true;
	}
	/// Gets the font base scaled by the scale factor
	inline float FontBase() const { return m_pFont->Base() * m_fScaleFactor; }
	/// Gets the font line height scaled by the scale factor
	inline float FontLineHeight() const { return m_pFont->LineHeight() * m_fScaleFactor; }
	// Sets the string to render
	void SetString(const char *pString);

	virtual void Visit(ncRenderQueue& rRenderQueue);
	virtual void Draw(ncRenderQueue& rRenderQueue);

	inline static eObjectType sType() { return TEXT_TYPE; }
	static ncTextNode* FromId(unsigned int uId);
};

#endif
