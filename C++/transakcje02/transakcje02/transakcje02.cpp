// transakcje02.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <iomanip>
using namespace std;

struct Account {
	string owner;
	double balance;
};

string getName(string &line) {
	string name;
	for (int i = 0; i < line.length(); i++) {
		if ((line.at(i) >= 65 && line.at(i) <= 90) || line.at(i) >= 97 && line.at(i) <= 127) {
			if (line.at(i + 1) == ' ') {
				name.push_back(line.at(i));
				break;
			}
			else {
				name.push_back(line.at(i));
			}
		}
	}
	return name;

}

double getBalance(string &line) {
	string balance;
	bool sign = true;
	for (int i = 0; i < line.length(); i++) {
		if (line.at(i) == '-') {
			sign = false;
		}
		else if ((line.at(i) >= 48 && line.at(i) <= 57) || line.at(i) == '.') {
			balance.push_back(line.at(i));
		}
	}
	double result;
	stringstream text(balance);
	text >> result;
	if (sign)
		return result;
	else
		return -result;
}

double getBills(string &line) {
	bool sign = true;
	string amount;
	stringstream stream;
	double sum = 0;
	double tmp;
	for (int i = 0; i < line.size()-1;i++) {
		if ((line.at(i) == ' ' || line.at(i) == '-') && (line.at(i+1) >= 48 && line.at(i+1) <= 57)) {
			if (line.at(i) = '-') {
				sign = false;
			}
			else {
				while (line.at(i) != ' ') {
					amount.push_back(line.at(i));
					i++;
				}
				stream.str(amount);
				amount.clear();
				if (sign) {
					stream >> tmp;
					sum += tmp;
					stream.clear();
				}
				else {
					stream >> tmp;
					sum -= tmp;
					stream.clear();
					sign = true;

				}

			}
		}
	}
	return sum;
}

void WriteAccounts(vector<Account> &accounts) {
	for (int i = 0; i < accounts.size(); i++) {
		cout << accounts[i].owner <<setw(40) << accounts[i].balance << endl;
	}
}

int main(){

	vector<Account> accounts;
	vector<Account> bills;
	ifstream iAccounts("konta.txt");
	ifstream iBills("transakcje.txt");


	//while (!iAccounts.eof()) {
		//string line;
		//getline(iAccounts, line);
		//accounts.push_back({getName(line),getBalance(line)});
	//}
	//accounts.erase(accounts.begin()+accounts.size()-1);

	

	while (!iBills.eof()) {
		string line;
	 getline(iBills, line);
	 cout << line<<endl;
		//bills.push_back({ getName(line),getBills(line) });
	}

	//WriteAccounts(accounts);

	WriteAccounts(bills);




	iAccounts.close();
	
	

    return 0;
}

