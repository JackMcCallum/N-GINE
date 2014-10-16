#ifndef __PAINTER_H_
#define __PAINTER_H_

#include <glm/glm.hpp>
#include "Font.h"

class Painter
{
public:
	Painter();
	~Painter();

	Font* createFont(const std::string& name, const std::string& fname);
	Font* getFont(const std::string& name);

	// Enable or disable smoothing
	void width(float width);
	void smoothing(bool enabled);
	void alpha(bool enabled);

	void begin(uint32 mode);
	void color(float r, float g, float b, float a);
	void color(const glm::vec4& col);
	void vertex(float x, float y);
	void vertex(const glm::vec2& pos);
	void texcoord(float x, float y);
	void texcoord(const glm::vec2& tex);
	void end();
	
	void text(const glm::vec2& pos, const std::string& phrase, const std::string& font, const glm::vec4& col);
	void rect(const glm::vec2& pos, const glm::vec2& scale, const glm::vec4& col, bool outline);
	void cubic(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec2& handle1, const glm::vec2& handle2, uint32 steps);
	
	
	
	//void circle(glm::vec2 pos, float radius, uint32 sections, bool outline = false, float min = 0, float max = 360);

	void setResolution(uint32 width, uint32 height);
	uint32 getResWidth() const;
	uint32 getResHeight() const;



private:
	uint32 mWidth;
	uint32 mHeight;
	
	std::vector<Font*> mFontList;
	glm::vec4 mCurrentColor;
};

#endif // !__PAINTER_H_
