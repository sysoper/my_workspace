#include <iostream>
#include <cmath>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include "stdafx.h"

using namespace std;

enum InputCase { IODefault, OutputDefault, InputDefault, HelpNeeded, IOGiven, WrongParams };

struct ComplexNum {
	int root;
	double re;
	double im;
	double module = sqrt(re*re + im*im);
};

int CheckCase(ComplexNum &number) {
	double re = number.re;
	double im = number.im;
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
double FindAlpha(int occurence, ComplexNum &number) {
	double re = number.re;
	double im = number.im;
	double module = number.module;
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
vector<double>CalculateRoot(double alpha, ComplexNum &number) {
	vector<double> solutions;
	double module = number.module;
	int root = number.root;
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
void WriteSolutions(ostream& stream, vector<double> &solutions) {
	if (solutions.size() == 1) {
		stream << solutions[0] << endl;
	}
	else {
		for (int i = 0; i<solutions.size(); i += 2) {
			if (i == solutions.size() - 2) {
				stream << solutions[i];
				if (solutions[i + 1]>0) {
					stream << " + ";
					stream << solutions[i + 1] << "i" << endl;
				}
				else if (solutions[i + 1]<0) {
					stream << " - ";
					stream << abs(solutions[i + 1]) << "i" << endl;
				}
				else {
					stream << endl;
				}
			}
			else {
				stream << solutions[i];
				if (solutions[i + 1]>0) {
					stream << " + ";
					stream << solutions[i + 1] << "i" << " LUB ";
				}
				else if (solutions[i + 1]<0) {
					stream << " - ";
					stream << abs(solutions[i + 1]) << "i" << " LUB ";
				}
				else {
					stream << endl;
				}
			}
		}
	}
}

void SortParams(vector<string> &params, InputCase occurence) {
	vector<string> tmp;
	for (int i = 1; i<params.size(); i++) {
		if (occurence == OutputDefault&&params[i - 1] == "-i") {
			tmp.push_back(params[i]); break;
		}
		else if (occurence == InputDefault&&params[i - 1] == "-o") {
			tmp.push_back(params[i]); break;
		}
		else if (occurence == IOGiven&&params[i - 1] == "-i"&&tmp.size() == 0) {
			tmp.push_back(params[i]);
			i = 0;
		}
		else if (occurence == IOGiven&&params[i - 1] == "-o"&&tmp.size()>0) {
			tmp.push_back(params[i]); break;
		}
	}
	params.clear();
	params = tmp;
}
bool IsValidPath(string &path) {
	fstream file;
	file.open(path);
	if (file.is_open()) {
		file.close();
		return true;
	}
	else {
		file.close();
	}	return false;
}
bool IsValidParam(string &param) {
	if (param == "-o" || param == "-i")
		return true;
	else
		return false;
}
InputCase CheckParams(vector<string> &params) {
	bool CheckA, CheckB;
	if (params.size() == 0 || ((params.size() == 1 && IsValidParam(params[0])) || (params.size() == 2 && IsValidParam(params[0]) && IsValidParam(params[1])))) {
		return IODefault;
	}
	else if (params.size() == 1 && params[0] == "-h") {
		return HelpNeeded;
	}
	else {
		for (int i = 0; i<params.size() - 1; i++) {
			if (params[i] == "-i"&&IsValidPath(params[i + 1])) {
				CheckA = true;
			}
			if (params[i] == "-o"&&IsValidPath(params[i + 1])) {
				CheckB = true;
			}
		}
		if (CheckA&&CheckB) {
			return IOGiven;
		}
		else if (CheckA && !CheckB) {
			return OutputDefault;
		}
		else if (!CheckA&&CheckB) {
			return InputDefault;
		}
		else {
			return WrongParams;
		}
	}
}
vector<string> ReadParams(int number, char *args[]) {
	vector<string> tmp;
	for (int i = 1; i<number; i++) {
		tmp.push_back(args[i]);
	}
	return tmp;
}
bool AreNoForbiddenSigns(string &line) {
	bool areNoForbiddenSigns;
	for (int i = 0; i<line.length(); i++) {
		if (line.at(i) == '+' || line.at(i) == '-' || line.at(i) == 'i' || line.at(i) == '.' || (line.at(i) >= 48 && line.at(i) <= 57) || iswspace(line.at(i))) {
			areNoForbiddenSigns = true;
		}
		else {
			areNoForbiddenSigns = false;
			break;
		}
	}
	return areNoForbiddenSigns;
}
bool IsNatural(string &line) {
	bool isNatural;
	bool isNotZero = false;
	for (int i = 0; i<line.length(); i++) {
		if (line.at(i) >= 48 && line.at(i) <= 57) {
			if (line.at(i) != '0') {
				isNotZero = true;
			}
			isNatural = true;
		}
		else {
			isNatural = false;
			break;
		}
	}
	return isNatural&&isNotZero;
}
bool IsComplexNumber(string &line) {
	bool isComplex;
	bool imGiven = false;
	bool reGiven = false;
	bool operatorExpected = false;
	for (int i = 0; i<line.length(); i++) {
		if (i<line.length() - 1 && operatorExpected && (line.at(i) == '-' || line.at(i) == '+') && line.at(i + 1) != '-'&&line.at(i + 1) != '+') {
			operatorExpected = false;
			continue;
		}
		else if (operatorExpected && (line.at(i) != '-' || line.at(i) != '+')) {
			return false;
		}
		else if ((line.at(i) == '-' || line.at(i) == '+')) {
			continue;
		}
		else if (line.at(i) == 'i' && !imGiven) {
			imGiven = true;
			operatorExpected = true;
			continue;
		}
		else if (isdigit(line[i])) {
			int j = i + 1;
			while ((j<line.length() && isdigit(line[j])) || (j<line.length() - 1 && line[j] == '.'&&isdigit(line[j + 1]))) {
				j++;
			}
			i = j - 1;

			if (!imGiven&&i<line.length() - 1 && (line.at(i + 1) == 'i' || line.at(i + 1) == '0')) {
				imGiven = true;
				operatorExpected = true;
				i++;
				continue;
			}
			else if (!reGiven) {
				operatorExpected = true;
				reGiven = true;
				continue;
			}
			else {
				return false;
			}
		}
		else {
			return false;
		}
	}

	if (imGiven || reGiven) {
		isComplex = true;
	}
	else {
		isComplex = false;
	}
	return isComplex;
}
bool CheckInput(string &line) {
	bool isValid;
	string root;
	string complexNumber;
	stringstream lineStream(line);
	lineStream >> root;

	if (IsNatural(root)) {
		string tmp;
		while (lineStream >> tmp) {
			complexNumber.append(tmp);
		}
		if (IsComplexNumber(complexNumber)) {
			isValid = true;
		}
		else {
			isValid = false;
		}
	}
	else {
		isValid = false;
	}
	return isValid;
}
ComplexNum GetValuesFromInput(string &line) {
	stringstream lineStream(line);
	string root;
	lineStream >> root;
	string num;
	string tmp;
	while (lineStream >> tmp) {
		num.append(tmp);
	}
	ComplexNum number;
	number.root = stoi(root);
	string re;
	string im;
	string value;
	bool reGiven = false;
	bool imGiven = false;
	string sign = "+";
	for (int i = 0; i<num.length(); i++) {
		if (num[i] == '-') {
			sign = "-";
			continue;
		}
		else if (num[i] == '+') {
			sign = "+";
			continue;
		}
		else if (isdigit(num[i])) {
			value.push_back(num[i]);
			int j = i + 1;
			while (isdigit(num[j]) || (j<num.length() - 1 && num[j] == '.'&&isdigit(num[j + 1]))) {
				value.push_back(num[j]);
				j++;
			}
			i = j - 1;
			if (!imGiven&&num[i + 1] == 'i') {
				imGiven = true;
				im = sign + value;
				value.clear();
				continue;
			}
			else if (!reGiven) {
				reGiven = true;
				re = sign + value;
				value.clear();
				continue;
			}
		}
		else if (num[i] == 'i' && !imGiven) {
			im = sign + "1";
			imGiven = true;
			continue;
		}
	}

	if (!imGiven) {
		number.im = 0;
	}
	else {
		number.im = stod(im);
	}
	if (!reGiven) {
		number.re = 0;
	}
	else {
		number.re = stod(re);
	}

	return number;
}
bool Calculate(istream &in, ostream &out) {
	string input;
	vector <double> solutions;
	ComplexNum number;
	while (getline(in, input)) {
		if (AreNoForbiddenSigns(input)) {
			if (CheckInput(input)) {
				number = GetValuesFromInput(input);
				solutions = CalculateRoot(FindAlpha(CheckCase(number), number), number);
				WriteSolutions(out, solutions);
				solutions.clear();
			}
			else {
				return false;
			}

		}
		else {
			return false;
		}
	}
	return true;
}
int main(int argc, char *argv[]) {

	vector<string> params = ReadParams(argc, argv);

	switch (CheckParams(params)) {
	case WrongParams:
		cout << "Blad. Niepoprawne dane wejsciowe";
		break;
	case HelpNeeded:
		cout << "HELP";//Zrobic funkcje "display help"
		break;
	case IOGiven:
		SortParams(params, IOGiven);//czytaj i pisz do pliku - stworzyc funkcje!
		ifstream iFile(params[0]);
		break;
	case InputDefault:
		cout << "Nie podano pliku wejsciowego." << endl << "Prosze wprowadzic dane ponize:" << endl;
		SortParams(params, InputDefault);

		break;
	case OutputDefault:
		cout << "Nie podano pliku wyjsciowego." << endl << "Wyniki zostana wyswietlone w konsoli" << endl;
		SortParams(params, OutputDefault);
		break;//stworzyc funkcje czytajaca z pliku i wpisujaca do konsoli
	case IODefault:
		cout << "Nie podano sciezek do plikow I/O." << endl << "Prosze podac dane wejsciowe ponizej:" << endl;
		Calculate(cin, cout);
	}



	return 0;
}

