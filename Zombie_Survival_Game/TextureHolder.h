#pragma once
#include <map>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;
class TextureHolder
{
private:
	// A map container from the STL,
	// that holds related pais of String and Texture
	std::map<std::string, Texture> m_Textures;

	// Apointer of the same type as the class itself
	// the one and only instance
	static TextureHolder* m_s_Instance;

public:
	TextureHolder();
	static Texture& GetTexture(std::string const& filename);
	
};

