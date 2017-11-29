// RootMaster.cpp : by Michal Rzepka

#include "stdafx.h"
#include <iostream>
#include <cmath>
#include <vector>
#include <fstream>
#include<string>
#include <windows.h>
using namespace std;

enum InputCase { DefaultIO = 1, DefaultInput = 2, DefaultOutput = 3, IOGiven = 4, HelpParam = 5, WrongParams = 6};//lista wyliczeniowa zawierająca etykiety nazw dla poszczególnych przypadków danych wpisanych przez użytkownika jako przełączniki

int CheckCase(double re, double im) {
	int occurence;

	if (re != 0 || im != 0) {
		if (re > 0 && im > 0) {
			occurence = 1;
		}
		if (re < 0 && im>0) {
			occurence = 2;
		}
		if (re > 0 && im < 0) {
			occurence = 3;
		}
		if (re == 0) {
			if (im > 0)
				occurence = 4;
			else
				occurence = 5;
		}
		if (im == 0) {
			if (re > 0)
				occurence = 6;
			else
				occurence = 7;
		}

	}
	else {
		occurence = 0;
	}
	return occurence;
}//funkcja sprawdza na podstawie wartości re oraz im, do jakiej ćwiartki należy arg główny bądź na której półprostej układu współrzędnych się on znajduje 
double FindAlpha(int occurence, double re, double im, double &module) {
	double alpha;

	switch (occurence) {
	case 0: alpha = acos(-1) + 1; break;
	case 1: alpha = asin(im / module); break;
	case 2: alpha = acos(-1) - asin(im / module); break;
	case 3: alpha = -acos(re / module); break;
	case 4: alpha = 0; break;
	case 5: alpha = acos(-1); break;
	case 6: alpha = acos(-1) / 2; break;
	case 7: alpha = -acos(-1) / 2; break;
	}
	return alpha;
}//na podstawie danych przekazanych przez funkcję CheckCase, funkcja FindAlpha wyznacza argument główny przy wykorzystaniu własności funkcji cyklometrycznych
vector<double> CalculateRoot(double alpha, double &module, int root) {
	vector<double> solutions;
	module = pow(module, (1.0 / root));

	if (alpha == acos(-1) + 1) {
		solutions.push_back(0);
		solutions.push_back(0);
	}
	else {
		alpha = alpha / root;
		
		for (int i = 0; i < root; i++) {
			solutions.push_back(module*cos(alpha + (2 * acos(-1)*i / root)));
			solutions.push_back(module*sin(alpha + (2 * acos(-1)*i / root)));
		}
	}
	return solutions;
}//Funkcja FindAlpha przekazuje do funkcji CalculateRoot argument główny. Na tej podstawie, funkcja wyznacza pierwiastki liczby zespolonej. (Na podstawie własności liczby zespolonej w postaci trygonometrycznej oraz twierdzenia De Moivre'a)
void WriteSolutions(vector<double> solutions, int root) {
	if (solutions[0] == 0 && solutions[1] == 0) {
		cout << "Z= 0";
	}
	else {
		for (int i = 0; i < 2 * root - 1; i += 2) {
			cout << "Z= " << solutions[i] << " + " << solutions[i + 1] << "i";
			cout << endl;
		}
	}
}//Funkcja wypisuje na wyjście standardowe wyniki przekazane jako argument przez funkcję CalculateRoot
void HandleWrongInput() {
	cout << "Podane parametry pierwiastka sa nieprawidlowe." << endl;
	cout << "Nie mozna obliczyc pierwiastka." << endl;
	cout << "Prosze wprowadzic poprawne dane." << endl;
}
int InteprateParams(int number, vector<string> &arg) {//funkcja odczytująca przełączniki podane przez użytkownika i określająca przypadek
	int occurence;
	ifstream file;

	if (number == 1 || (number == 3 && ((arg[1] == "-i"&&arg[2] == "-o") || (arg[2] == "-i"&&arg[1] == "-o")))) {//brak parametrów lub "-i -o" lub "-o -i"
		occurence = DefaultIO;
	}
	else if (number == 5 && ((arg[1] == "-o" && arg[3] == "-i") || (arg[3] == "-o" && arg[1] == "-i")) && arg[2] != arg[4]) {//oczekiwane parametry "-i sciezka1 -o sciezka2" lub "-o sciezka1 -i sciezka2"
		file.open(arg[2]);
		if (file.is_open()) {
			file.close();
			file.open(arg[4]);
			if (file.is_open()) {
				file.close();
				occurence = IOGiven;
			}else {
				occurence = WrongParams;
			}
		}else {
			occurence = WrongParams;
		}
	}
	else if ((number == 4 && ((arg[1] == "-i"&&arg[2] == "-o")||(arg[1]=="-o"&&arg[3]=="-i")))||(number==3&&arg[1]=="-o")) {//parametrwy w postaci "-i -o sciezka" lub "-o sciezka -i"
		file.open(arg[2]);
		if (file.is_open()) {
			occurence = DefaultInput;
			file.close();
		}else {
			file.close();
			file.open(arg[3]);
			if (file.is_open()) {
				occurence = DefaultInput;
			}
			else {
				occurence = WrongParams;
			}
		}
	}
	else if ((number == 4 && ((arg[1] == "-i"&&arg[3] == "-o") || (arg[1] == "-o"&&arg[2] == "-i")))||(number==3&&arg[1]=="-i")) {//parametry w postaci "-o -i sciezka" lub "-i sciezka -o" lub "-i sciezka"
		file.open(arg[2]);
		if (file.is_open()) {
			occurence = DefaultOutput;
			file.close();
		}
		else {
			file.close();
			file.open(arg[3]);
			if (file.is_open()) {
				occurence = DefaultOutput;
			}
			else {
				occurence = WrongParams;
			}
		}
	}
	else if (number == 2 && arg[1] == "-h") {
		occurence = HelpParam;
	}
	else {
		occurence = WrongParams;
	}
	return occurence;
}
void HandleIfDefaultIO() {//funkcja wykonywana, gdy użytkownik nie poda przełączników określających ściężkę I/O
	cout << "Nie wskazano plikow I/O" << endl;
	Sleep(1000);
	cout << "Program zapyta o dane wejsciowe." << endl;
	long double input_root;
	int root;
	double re, im,module;
	Sleep(2000);
	cout << "Prosze podac stopien pierwiastka:" << endl;
	cin >> input_root;
	if (floor(input_root) == input_root&&input_root>0) {
		root = input_root;
		cout << "Prosze podac czesc rzeczywista" << endl;
		cin >> re;
		cout << "Prosze podac czesc urojona:" << endl;
		cin >> im;
		module = sqrt(im*im + re*re);
		WriteSolutions(CalculateRoot(FindAlpha(CheckCase(re, im), re, im, module), module, root), root);
	}else {
		HandleWrongInput();
	}
}
void HandleIfDefaultInput(vector<string> &arg) {//funkcja wykonywana, gdy użytkownik nie poda przełącznika określającego ścieżkę Input
	cout << "Nie wskazano pliku, z ktorego beda czytane dane." << endl;
	Sleep(1000);
	cout << "Program zapyta o dane wejsciowe." << endl;
	long double input_root;
	int root;
	double re, im, module;
	Sleep(2000);
	cout << "Prosze podac stopien pierwiastka:" << endl;
	cin >> input_root;
	if (floor(input_root) == input_root&&input_root>0) {
		root = input_root;
		cout << "Prosze podac czesc rzeczywista:" << endl;
		cin >> re;
		cout << "Prosze podac czesc urojona:" << endl;
		cin >> im;
		module = sqrt(im*im + re*re);

		ofstream file;

		if (arg[2] == "-o") {
			file.open(arg[3]);
		}
		else {
			file.open(arg[2]);
		}
		vector<double> solutions = CalculateRoot(FindAlpha(CheckCase(re, im), re, im, module), module, root);
		if (solutions[0] == 0 && solutions[1] == 0) {
			file << "Z= 0";
		}
		else {
			for (int i = 0; i < 2 * root - 1; i += 2) {
				file << "Z= " << solutions[i] << " + " << solutions[i + 1] << "i";
				file << endl;
				file << endl;
			}
		}
		file.close();
		cout << "Wyniki zapisano w podanym pliku";
	}
	else {
		HandleWrongInput();
	}
	
}
void HandleIfDefaultOutput(vector<string> &arg) {//funkcja wykonywana, gdy użytnkownik nie poda przełącznika określającego ścieżkę Output
	cout << "Nie podano pliku wyjsciowego." << endl;
	Sleep(1000);
	cout << "Wyniki zostana wyswietlone w konsoli." << endl;
	Sleep(2000);
	ifstream file;
	if (arg[2] == "-i") {
		file.open(arg[3]);
	}
	else {
		file.open(arg[2]);
	}
	long double input_root;
	int root;
	double re, im, module;
	file >> input_root;
	if (floor(input_root) == input_root&&input_root>0) {
		root = input_root;
		file.ignore();
		file >> re;
		file.ignore();
		file >> im;
		file.close();

		module = sqrt(im*im + re*re);
		WriteSolutions(CalculateRoot(FindAlpha(CheckCase(re, im), re, im, module), module, root), root);
	}
	else {
		HandleWrongInput();
	}
	

}//! narazie wszystkie dane wejsciowe traktowane sa jako pojedyncze znaki!
void HandleIfGiven(vector<string> &arg) {//funkcja wykonywana, gdy użytkownik poprawnie poda wszystkie parametry określające ścieżki I/O
	ifstream iFile;
	ofstream oFile;

	if (arg[1] == "-i") {
		iFile.open(arg[2]);
		oFile.open(arg[4]);
	}
	else {
		iFile.open(arg[4]);
		oFile.open(arg[2]);
	}
	long double input_root;
	int root;
	double re, im, module;

	iFile >> input_root;

	if (floor(input_root) == input_root&&input_root>0) {
		root = input_root;
		iFile >> re;

		iFile >> im;

		module = sqrt(im*im + re*re);
		vector<double> solutions = CalculateRoot(FindAlpha(CheckCase(re, im), re, im, module), module, root);

		if (solutions[0] == 0 && solutions[1] == 0) {
			oFile << "Z= 0";
		}
		else {
			for (int i = 0; i < 2 * root - 1; i += 2) {
				oFile << "Z= " << solutions[i] << " + " << solutions[i + 1] << "i";
				oFile << endl;
				oFile << endl;
			}
		}

		iFile.close();
		oFile.close();
	}
	else {
		HandleWrongInput();
	}

}
void HandleFileStream(int value,vector <string> &arg) {//na podstawie wartości przekazanej z funkcji InteprateParams, funkcja HandleFileStream wywołuje odpowiednie funkcje
	switch (value) {
	case DefaultIO: HandleIfDefaultIO();
		break;
	case DefaultInput: HandleIfDefaultInput(arg);
		break;
	case DefaultOutput: HandleIfDefaultOutput(arg);
		break;
	case IOGiven: HandleIfGiven(arg);
		break;
	case HelpParam: cout << "Help"; 
		break;
	case WrongParams: cout << "Blad. Prosze wprowadzic poprawne dane!"; 
		break;
	default:cout << "Blad. Cos poszlo nie tak."; 
	}
}
vector <string> GetParams(int argc, char *argv[]) {
	vector<string> args;
	for (int i = 0; i < argc; i++) {//wypełnienie wektora odpowiednimi wartościami
		args.push_back(argv[i]);
	}
	return args;
}

int main(int argc, char *argv[]){
	vector<string> args = GetParams(argc, argv);
	HandleFileStream(InteprateParams(argc,args),args);//wywołanie funkcji opisanych powyżej
    return 0;
}






