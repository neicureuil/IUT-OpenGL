#pragma once

#include <stdlib.h>
#include <glm/ext/vector_float3.hpp>

/*
* Classe contenant des fonctions statiques utilitaires
*/
class Utils {

public:
	Utils() = delete;
	~Utils() = delete;

	/*
	* Fonction qui genere un nombre flotant aleatoire en 2 bornes entieres
	*/
	static float random(int min, int max);
	/*
	* Fonction qui genere un nombre flotant aleatoire en 0 et 1
	*/
	static float random();
	/*
	* Fonction qui realise le produit vectoriel entre 2 vecteurs
	*/
	static glm::vec3 vecProd(const glm::vec3& a, const glm::vec3& b);
	/*
	* Fonction qui realise le produit scalaire entre 2 vecteurs
	*/
	static float scalProd(const glm::vec3& a, const glm::vec3& b);
	/*
	* Fonction qui calcul la norme d'un vecteur
	*/
	static float norme(const glm::vec3& a);
};