// regressionMultivariableAloTrading.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "pch.h"
#include <vector>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <windows.h>

#pragma warning(disable : 4996) 

using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);


void gotoxy(short a, short b) //Custom gotoxy() function
{
	COORD coordinates; //Data type of co-ordinates
	coordinates.X = a; //Assign value to X- Co-ordinate
	coordinates.Y = b; //Assign value to Y Co-ordinate

	SetConsoleCursorPosition(
		GetStdHandle(STD_OUTPUT_HANDLE), coordinates);

}

void testcolor()
{
	for (size_t i = 0; i < 255; i++)
	{
		SetConsoleTextAttribute(hConsole, (WORD)i);
		cout << "TEST : " << i << endl;
	}

	system("pause");
}

bool getDataFromFile(string filPath, vector<vector<float>> &features, vector<float> &targets)
{
	/*
	* this method which allows to recover the data from a file and the file must be of a format (X = feateurs Y = Targets)

	*   0.1 0.5 0.8 ... 0.5 0.8
	*   0.7 3.2 3.8 ... 1.6 0.1
	*                .
	*		 	     .
	*				 .
	*	0.3 0.7 1.4 ... 9.0 9.2
	*/

	FILE* file = NULL;
	file = fopen(filPath.c_str(), "r");
	if (!file)
	{
		cout << "**** Impossible d'ouvrir le fichier "<< filPath << " ****" << endl;
		return false;
	}
	else
	{
		int currentCaracter = 0;
		int ligne = 0;
		float rowFile = 0;
		int lastProgress = 0;
		vector<float> vectorLigne;

		cout << endl << endl <<"	Initialization ... " << endl;

		do//get number of row from file to calculate file refing progress
		{
			currentCaracter = fgetc(file);

			if (currentCaracter == '\n')
				rowFile++;

		} while (currentCaracter != EOF);

		rewind(file);//puts the cursor back to the beginning of the file

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

				if(ligne > 0)
				if (vectorLigne.size() != features.at(ligne - 1).size())
				{
					SetConsoleTextAttribute(hConsole, 207);
					string messageError = "    /!\\ The File " + filPath + " is corrupted, check your data file    ";
					cout << endl;

					for (size_t i = 0; i < messageError.length(); i++)
						cout << " ";

					cout << endl;
					cout << messageError<< endl;

					for (size_t i = 0; i < messageError.length(); i++)
						cout << " ";

					cout << endl;
					cout << endl;
					SetConsoleTextAttribute(hConsole, 15);

					return false; 
				}

				ligne++;
				float progresion = 100 + (ligne - rowFile) / rowFile * 100;

				if (ligne == 1)
				{
					gotoxy(0, 5);
					cout << "Loding File DATA ...";
				}

				if (lastProgress != (int)progresion)//for optimisation
				{
					gotoxy(21, 5);
					cout << " " <<(int)progresion << "%" << endl;
					lastProgress = (int)progresion;
				}
				
				vectorLigne.clear();
			}

			fscanf(file, "%f", &tempData);

			vectorLigne.push_back(tempData);
			currentCaracter = fgetc(file);

			if (currentCaracter == EOF)
				vectorLigne.push_back(tempData);

		} while (currentCaracter != EOF);

		fclose(file);

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

		string messageSuccsess = "    File "+filPath+" is loding    ";

		cout << endl;

		SetConsoleTextAttribute(hConsole, 47);
		for (size_t i = 0; i < messageSuccsess.length(); i++)
			cout << " ";
		cout << endl;

		cout << messageSuccsess << endl;

		for (size_t i = 0; i < messageSuccsess.length(); i++)
			cout << " ";
		cout << endl << endl;
		SetConsoleTextAttribute(hConsole, 15);


		return true;
	}
}

void showFeatures(vector<vector<float>> features)
{
	for (size_t i = 0; i < features.size(); i++)
	{
		for (size_t y = 0; y < features.at(i).size(); y++)
			cout << "[" << features.at(i).at(y) << "]";
		cout << endl;
	}
}

void showTargets(vector<float> targets)
{
	for (size_t i = 0; i < targets.size(); i++)
		cout << "[" << targets.at(i) << "]" << endl;
}

float linearModel(const vector<float> inputs,vector<float> &thetas)
{
	float Result = 0;

	if (thetas.empty())
		for (size_t i = 0; i < inputs.size(); i++)
			thetas.push_back(0.0);

	for (size_t i = 0; i < inputs.size(); i++)
		Result += (thetas.at(i) * inputs.at(i));

	return Result;
}

float ploynomialeMode(const vector<float> inpusts, vector<float> &thetas)
{
	return 0.0;
}

float costFunction(const vector<vector<float>> features, const vector<float> targets, vector<float> &thetas)
{
	float sumDeltaSquared = 0.0;
	float meanSquaredError = 0;

	for (size_t row = 0; row < features.size(); row++)
		sumDeltaSquared += pow(linearModel(features.at(row),thetas) - targets.at(row),2);

	meanSquaredError = (features.size() * 2) / 1 * sumDeltaSquared;
	return meanSquaredError;
}

vector<float> gradientDescent(const vector<vector<float>> features,const vector<float> targts,vector<float> &thetas)
{
	double alpha = 0.1;
	vector<float> derivatives;
	float result = 0.0;

	for (size_t i = 0; i < thetas.size(); i++)
	{
		for (size_t row = 0; row < features.size(); row++)
			result += (linearModel(features.at(row), thetas) - targts.at(row)) * features.at(row).at(i);

		derivatives.push_back(result);
	}

	return derivatives;
}

int main()
{
	vector<vector<float>> features(0,vector<float> (0));
	vector<float> targets;
	vector<float> thetas;
	vector<float> thetasUpdate;

	int lastProsessing = 0;
	FILE* file = NULL;

	//testcolor();

	if (getDataFromFile("data-test.txt", features, targets))
	{
		float iterations = 3500;

		for (size_t i = 0; i < (size_t)iterations; i++)
		{
			file = fopen("SaveCosts.txt", "a+");
			if (!file)
			{
				cout << "**** Impossible d'ouvrir le fichier SaveCosts.txt ****" << endl;
				return false;
			}

			thetasUpdate = gradientDescent(features, targets, thetas);

			for (size_t i = 0; i < thetas.size(); i++) 
				thetas.at(i) = thetas.at(i) - 0.0005 * thetasUpdate.at(i);

			float progresion = 100 + (i+1 - iterations) / iterations * 100;

			if (i == 0)
			{
				gotoxy(0, 11);
				cout << "Processing ... 0%";
			}

			if (lastProsessing != (int)progresion)
			{
				gotoxy(15, 11);
				cout << " " << (int)progresion << "%" << endl;
				lastProsessing = progresion;
			}

			costFunction(features, targets, thetas);

			if (file)
				fprintf(file, "%ld %f\n", i, costFunction(features, targets, thetas));
			
			fclose(file);
		}


		cout << "LAST Cost sunction : " << costFunction(features, targets, thetas);

		/*for (size_t i = 0; i < thetas.size(); i++)
			cout << "[" << thetas.at(i) << "] - ";*/
	}
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
