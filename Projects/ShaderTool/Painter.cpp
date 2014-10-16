#include "Precompiled.h"
#include "Painter.h"


Painter::Painter() :
mWidth(0),
mHeight(0)
{
}

Painter::~Painter()
{
	for (auto iter : mFontList)
		delete iter;
}

void Painter::width(float width)
{
	glLineWidth(width);
}

void Painter::begin(uint32 mode)
{
	glBegin(mode);
}

void Painter::color(float r, float g, float b, float a)
{
	glColor4f(r, g, b, a);
	mCurrentColor = glm::vec4(r, g, b, a);
}

void Painter::color(const glm::vec4& col)
{
	color(col.r, col.g, col.b, col.a);
}

void Painter::vertex(float x, float y)
{
	glVertex2d((x / mWidth) * 2.0f - 1.0f, (y / mHeight) * -2.0f + 1.0f);
}

void Painter::vertex(const glm::vec2& pos)
{
	vertex(pos.x, pos.y);
}

void Painter::end()
{
	glEnd();
}

void Painter::texcoord(float x, float y)
{
	glTexCoord2f(x, y);
}

void Painter::texcoord(const glm::vec2& tex)
{
	texcoord(tex.x, tex.y);
}

void Painter::setResolution(uint32 width, uint32 height)
{
	mWidth = width;
	mHeight = height;
}

void Painter::cubic(const glm::vec2& pos1, const glm::vec2& pos2, const glm::vec2& handle1, const glm::vec2& handle2, uint32 steps)
{
	assert(steps > 0);
	color(1, 1, 1, 1);
	begin(GL_LINE_STRIP);
	for (uint32 i = 0; i <= steps; i++)
		vertex(glm::hermite(pos1, handle1, pos2, handle2, glm::smoothstep(0.0f, 1.0f, i / (float)steps)));
	end();
}

uint32 Painter::getResWidth() const
{
	return mWidth;
}

uint32 Painter::getResHeight() const
{
	return mHeight;
}

void Painter::smoothing(bool enabled)
{
	if (enabled)
	{
		glEnable(GL_LINE_SMOOTH);
		glEnable(GL_POLYGON_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
		glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	}
	else
	{
		glDisable(GL_LINE_SMOOTH);
		glDisable(GL_POLYGON_SMOOTH);
	}
}

void Painter::alpha(bool enabled)
{
	if (enabled)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else
	{
		glDisable(GL_BLEND);
	}
}

void Painter::rect(const glm::vec2& pos, const glm::vec2& scale, const glm::vec4& col, bool outline)
{
	if (outline)
	{
		width(1);
		begin(GL_LINE_STRIP);
		color(col);
		vertex(pos);
		vertex(pos + glm::vec2(scale.x, 0));
		vertex(pos + glm::vec2(scale.x, scale.y));
		vertex(pos + glm::vec2(0, scale.y));
		vertex(pos);
		end();
	}
	else
	{
		begin(GL_TRIANGLE_STRIP);
		color(col);
		vertex(pos);
		vertex(pos + glm::vec2(scale.x, 0));
		vertex(pos + glm::vec2(0, scale.y));
		vertex(pos + glm::vec2(scale.x, scale.y));
		end();
	}
}

void Painter::text(const glm::vec2& pos, const std::string& phrase, const std::string& font, const glm::vec4& col)
{
	// Use default if none specified
	Font* fontp = getFont(font);
	if (!fontp)
	{
		std::cout << "Failed to find font: " << font << std::endl;
		return;
	}

	float lineSpacing = 12;
	float wordSpacing = 8;
	float charSpacing = -2;

	float curHPos = 0;
	float curVPos = 0;

	glBindTexture(GL_TEXTURE0, fontp->getTexture());
	glEnable(GL_TEXTURE_2D);

	begin(GL_TRIANGLES);
	color(col);

	FontData data;
	uint32 s = phrase.size();
	for (int i = 0; i < s; i++)
	{
		char ch = phrase[i];

		if (ch == ' ')
		{
			curHPos += wordSpacing;
			continue;
		}

		if (!fontp->getData(ch, data))
			continue;

		float ww = (float)data.width;
		float hh = (float)data.height;
		float xx = curHPos;
		float yy = curVPos - data.offsety;
		
		texcoord(data.minu, data.minv);
		vertex(xx + pos.x, yy + pos.y);
		texcoord(data.maxu, data.minv);
		vertex(xx + ww + pos.x, yy + pos.y);
		texcoord(data.minu, data.maxv);
		vertex(xx + pos.x, yy + hh + pos.y);
		
		texcoord(data.maxu, data.minv);
		vertex(xx + ww + pos.x, yy + pos.y);
		texcoord(data.maxu, data.maxv);
		vertex(xx + ww + pos.x, yy + hh + pos.y);
		texcoord(data.minu, data.maxv);
		vertex(xx + pos.x, yy + hh + pos.y);
		
		// Get ready for next char
		curHPos += data.width + charSpacing;
	}

	end();
	glDisable(GL_TEXTURE_2D);
}

Font* Painter::createFont(const std::string& name, const std::string& fname)
{
	Font* font = new Font(name);
	if (font->load(fname))
	{
		mFontList.push_back(font);
		return font;
	}
	else
		delete font;
	return nullptr;
}

Font* Painter::getFont(const std::string& name)
{
	uint32 hash = NGine::Utilities::hash(name);
	for (auto iter : mFontList)
	{
		if (iter->getNameHash() == hash)
			return iter;
	}
	return nullptr;
}









/*
void Painter::circle(glm::vec2 pos, float radius, uint32 sections, bool outline, float min, float max)
{
	glm::vec4 oldCol = mCurrentColor;

	float radmin = min * glm::pi<float>() / 180;
	float radmax = max * glm::pi<float>() / 180;
	
	if (!outline)
	{
		float spread = 0.5f;
		begin(GL_TRIANGLE_FAN);
		vertex(pos);
		for (uint32 i = 0; i <= sections; i++)
		{
			vertex(
				pos.x + glm::sin((i / (float)sections) * radmax + radmin) * (radius - spread),
				pos.y + glm::cos((i / (float)sections) * radmax + radmin) * (radius - spread));
		}
		end();

		begin(GL_TRIANGLE_STRIP);
		vertex(pos);
		for (uint32 i = 0; i <= sections; i++)
		{
			float s = glm::sin((i / (float)sections) * radmax + radmin);
			float c = glm::cos((i / (float)sections) * radmax + radmin);
			color(oldCol);
			vertex(pos.x + s * (radius - spread), pos.y + c * (radius - spread));
			color(glm::vec4(oldCol.rgb, 0));
			vertex(pos.x + s * (radius + spread), pos.y + c * (radius + spread));
		}
		end();
		color(oldCol);
	}
	else
	{
		float spread = 1.0f;
		begin(GL_TRIANGLE_STRIP);
		for (uint32 i = 0; i <= sections; i++)
		{
			float s = glm::sin((i / (float)sections) * radmax + radmin);
			float c = glm::cos((i / (float)sections) * radmax + radmin);
			color(oldCol);
			vertex(pos.x + s * radius, pos.y + c * radius);
			color(glm::vec4(oldCol.rgb, 0));
			vertex(pos.x + s * (radius + spread), pos.y + c * (radius + spread));
		}

		end();
		begin(GL_TRIANGLE_STRIP);
		for (uint32 i = 0; i <= sections; i++)
		{
			float s = glm::sin((i / (float)sections) * radmax + radmin);
			float c = glm::cos((i / (float)sections) * radmax + radmin);
			color(oldCol);
			vertex(pos.x + s * radius, pos.y + c * radius);
			color(glm::vec4(oldCol.rgb, 0));
			vertex(pos.x + s * (radius - spread), pos.y + c * (radius - spread));
		}
		end();

		color(oldCol);
	}
	}
	*/