// regressionMultivariableAloTrading.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "pch.h"
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#pragma warning(disable : 4996)

using namespace std;

void getDataFromFile(string filPath, vector<vector<float>> &features,vector<float> &targets)
{

	FILE* file = NULL;
	file = fopen(filPath.c_str(), "r");
	if (!file)
		cout << "**** Impossible d'ouvrir le fichier "<< filPath << " ****" << endl;
	else
	{
		int currentCaracter = 0;
		int ligne = 0;
		vector<float> vectorLigne;

		do
		{
			float tempData;

			if (currentCaracter == 10)//if current caracter  == \n
			{
				features.push_back(vector<float>(1, 1));

				for (size_t i = 0; i < vectorLigne.size(); i++)
				{
					if (i == vectorLigne.size()-1)
						targets.push_back(vectorLigne.at(i));
					else
						features.at(ligne).push_back(vectorLigne.at(i));
				}

				ligne++;
				vectorLigne.clear();
			}

			fscanf(file, "%f", &tempData);

			vectorLigne.push_back(tempData);
			currentCaracter = fgetc(file);

			if (currentCaracter == EOF)
				vectorLigne.push_back(tempData);
		} while (currentCaracter != EOF);

		//persist last ligne data
		features.push_back(vector<float>(1, 1));
		vectorLigne.pop_back();

		for (size_t i = 0; i < vectorLigne.size(); i++)
		{
			if (i == vectorLigne.size() - 1)
				targets.push_back(vectorLigne.at(i));
			else
				features.at(ligne).push_back(vectorLigne.at(i));
		}

		fclose(file);
	}
}

void showFeatures(vector<vector<float>> features)
{
	for (size_t i = 0; i < features.size(); i++)
	{
		for (size_t y = 0; y < features.at(i).size(); y++)
			printf("[%f]", features.at(i).at(y));
		cout << endl;
	}
}

void showTargets(vector<float> targets)
{
	for (size_t i = 0; i < targets.size(); i++)
		printf("[%f] \n", targets.at(i));
}

int main()
{
	
	int caracterActuel = 0;
	float RowDataSet[10] = { 0 };

	float alpha = 0.000001;

	vector<vector<float>> features(0,vector<float> (0));
	vector<float> targets;
	vector<float> thetas;
	vector<float> thetasUpdate;
	vector<float> prediction;

	getDataFromFile("data-test.txt",features,targets);

}

// Exécuter le programme : Ctrl+F5 ou menu Déboguer > Exécuter sans débogage
// Déboguer le programme : F5 ou menu Déboguer > Démarrer le débogage

// Conseils pour bien démarrer : 
//   1. Utilisez la fenêtre Explorateur de solutions pour ajouter des fichiers et les gérer.
//   2. Utilisez la fenêtre Team Explorer pour vous connecter au contrôle de code source.
//   3. Utilisez la fenêtre Sortie pour voir la sortie de la génération et d'autres messages.
//   4. Utilisez la fenêtre Liste d'erreurs pour voir les erreurs.
//   5. Accédez à Projet > Ajouter un nouvel élément pour créer des fichiers de code, ou à Projet > Ajouter un élément existant pour ajouter des fichiers de code existants au projet.
//   6. Pour rouvrir ce projet plus tard, accédez à Fichier > Ouvrir > Projet et sélectionnez le fichier .sln.
