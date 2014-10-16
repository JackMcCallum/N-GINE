#ifndef __FONT_FILE_H_
#define __FONT_FILE_H_

/*
@Author
Jack McCallum

@Description
Loads a texture and font data ready for use with a text generator
see "Text.h"
*/

#include <string>
#include <vector>


// Defines the data for each character in the font
struct FontData
{
	FontData() : id(0) {};

	char id;				// Id for this Char
	float minu, minv;		// TexCoords
	float maxu, maxv;		
	int width, height;		// Dimensions
	int offsetx, offsety;	// Offset in pixels
}; 

class Font
{
public:
	Font(const std::string& name);
	~Font();

	// Import
	bool load(std::string fname);
	
	// Data retrieval
	bool getData(char ch, FontData& data);
	uint32 getTexture();

	const std::string& getName() const;
	uint32 getNameHash() const;

protected:
	// Font data
	int mNumChars;
	int mTexWidth;
	int mTexHeight;
	FontData mFontData[256];
	uint32 mTexture;

	// Is loaded
	bool mIsLoaded;

	std::string mName;
	uint32 mNameHash;
};

#endif