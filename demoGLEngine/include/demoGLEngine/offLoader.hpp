#pragma once

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <iostream>
#include <glm/ext/vector_float3.hpp>

/*
* Structure représentant un objet off chargé
*/
struct OFFObject
{
	std::vector<float> vertices;
	std::vector<unsigned int> triangles;
	std::vector<float> uvs;
	glm::vec3 min, max;
};

/*
* Classe contenant les fonctions statique qui permettent le chargement d'un fichier OFF-UV
*/
class OFFLoader {

private:

public:
	OFFLoader() = delete;
	~OFFLoader() = delete;

	static OFFObject* loadFile(const char*);


};