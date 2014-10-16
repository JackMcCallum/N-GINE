#include "Precompiled.h"
#include "Font.h"
#include <iostream>

Font::Font(const std::string& name) :
mName(name),
mIsLoaded(false),
mTexture(0),
mNumChars(0),
mTexWidth(0),
mTexHeight(0)
{
	mNameHash = NGine::Utilities::hash(mName);
}

Font::~Font()
{
	glDeleteTextures(1, &mTexture);
}

bool Font::load(std::string fname)
{
	TiXmlDocument doc(fname.c_str());
	bool loadOkay = doc.LoadFile();
	if (!loadOkay) 
	{
		std::cout << "Unable to load font. Document failed to load: " << fname << std::endl;
		return false;
	}
	
	TiXmlElement* element = doc.FirstChildElement("font");
	if (!element)
	{
		std::cout << "Unable to find font tag: " << fname << std::endl;
		return false;
	}

	TiXmlElement* texture = element->FirstChildElement("texture");
	if (!texture)
	{
		std::cout << "Unable to find texture tag: " << fname << std::endl;
		return false;
	}

	TiXmlElement* chars = element->FirstChildElement("chars");
	if (!texture)
	{
		std::cout << "Unable to find chars tag: " << fname << std::endl;
		return false;
	}

	mTexWidth = atoi(texture->Attribute("width"));
	mTexHeight = atoi(texture->Attribute("height"));
	std::string texFile = texture->Attribute("file");

	// Get all the file info
	std::string sDir, sFname, sExt;
	NGine::Utilities::splitPath(fname, &sDir, &sFname, &sExt);

	std::string texfname = (sDir + texFile);
	const char* texfnamech = texfname.c_str();
	mTexture = SOIL_load_OGL_texture(texfnamech, SOIL_LOAD_AUTO, mTexture, 0);
	if (!mTexture)
	{
		std::cout << "Failed to load the associated texture " << texfnamech << std::endl;
		return false;
	}

	TiXmlElement* ch = chars->FirstChildElement("char");
	while (ch)
	{
		FontData data;
		data.id = ch->Attribute("id")[0];
		if (data.id == ' ')
		{
			ch = ch->NextSiblingElement("char");
			continue;
		}
		
		// Position Data
		data.offsetx = atoi(ch->Attribute("offset_x"));
		data.offsety = atoi(ch->Attribute("offset_y"));
		data.width   = atoi(ch->Attribute("rect_w"));
		data.height  = atoi(ch->Attribute("rect_h"));

		// UV data
		data.minu = atoi(ch->Attribute("rect_x"))/float(mTexWidth);
		data.minv = atoi(ch->Attribute("rect_y"))/float(mTexHeight);
		data.maxu = data.width/float(mTexWidth) + data.minu;
		data.maxv = data.height/float(mTexHeight) + data.minv;
		
		if (data.id > 0)
			mFontData[data.id] = data;

		ch = ch->NextSiblingElement("char");
	}
	
	mIsLoaded = true;
	return true;
}

bool Font::getData(char ch, FontData& data)
{
	if (ch > 0 && mIsLoaded)
	{
		data = mFontData[ch];
		if (data.id != 0)
			return true;
	}
	return false;
}

uint32 Font::getTexture()
{
	return mTexture;
}

const std::string& Font::getName() const
{
	return mName;
}

uint32 Font::getNameHash() const
{
	return mNameHash;
}
