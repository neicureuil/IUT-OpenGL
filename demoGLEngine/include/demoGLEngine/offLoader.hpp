#pragma once

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <vector>
#include <iostream>

struct OFFObject
{
	std::vector<float> vertices;
	std::vector<unsigned int> triangles;
	std::vector<float> uvs;
};

class OFFLoader {

private:

public:
	OFFLoader() = delete;
	~OFFLoader() = delete;

	static OFFObject* loadFile(const char*);


};