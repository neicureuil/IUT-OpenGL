#include "demoGLEngine/offLoader.hpp"

OFFObject* OFFLoader::loadFile(const char* path) {
	FILE* file;

	if ((file = fopen(path, "r")) == NULL)	{
		perror("Erreur a l'ouverture du fichier OFF");
		exit(1);
	}

	char magic[10];
	fscanf(file, "%s\n", magic);

	if (strcmp(magic, "OFF") != 0) {
		perror("Erreur dans l'en tête du fichier OFF");
		exit(0);
	}

	unsigned int nbsommets;
	unsigned int nbfaces;
	unsigned int i;

	fscanf(file, "%u %u %u\n", &nbsommets, &nbfaces, &i);

	OFFObject* obj = new OFFObject();

	// Lecture des vertices
	for (i = 0;i < nbsommets;i++) {
		double x, y, z, u, v;
		fscanf(file, "%lf %lf %lf %lf %lf\n", &x, &y, &z, &u, &v);
		obj->vertices.push_back((float)x);
		obj->vertices.push_back((float)y);
		obj->vertices.push_back((float)z);
		obj->uvs.push_back((float)v);
		obj->uvs.push_back((float)u);
	}

	// Lecture des faces
	for (i = 0;i < nbfaces;i++)	{
		unsigned int n, n1, n2, n3;
		fscanf(file, "%u %u %u %u\n", &n, &n1, &n2, &n3);
		if (n != 3) {
			perror("Erreur au chargement des faces, non triangulaires");
			exit(1);
		}
		obj->triangles.push_back(n1);
		obj->triangles.push_back(n2);
		obj->triangles.push_back(n3);
	}

	fclose(file);

	std::reverse(obj->uvs.begin(), obj->uvs.end());

	return obj;
}