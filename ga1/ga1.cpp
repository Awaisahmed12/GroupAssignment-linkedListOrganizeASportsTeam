#include <iostream>
#include <fstream>
#include <string>
#include "ArgumentManager.h"

using namespace std;
//to disclose, this was a group project between me: Awais Ahmed,
// Mathew Manalang and Frederico Nocioni

struct Player {
	Player *next = nullptr;
	Player *back = nullptr;
	string name = "";
	string position = "";
	int jersey = 0;
};

void readPlayers(ifstream &, ifstream &, ofstream &, int, Player *);
void printPlayers(int, Player *);
void sortPlayers(int, Player *);
bool isBigger(string, string);
void findIndex(ifstream &, ofstream &, int, Player *);
int searchName(int, Player *, string);

int main(int argc, char* argv[]) {

	int numOfPlayers = 0;
	Player *head = new Player;
	ArgumentManager am(argc, argv);
	const string in = am.get("input"); //input name
	const string command = am.get("command");
	const string out = am.get("output"); //output name

	ifstream ifs;
	ifstream cmd;
	ofstream output(out);						//out11.txt		(for testing)
	ifs.open(in);								//input11.txt	(for testing)
	cmd.open(command);							//command11.txt	(for testing)

	//Get Number Of Players In The Input File
	while (!ifs.eof()) {
		string s;
		getline(ifs, s);
		if (s != "") {
			numOfPlayers++;
		}
	}
	ifs.clear();
	ifs.seekg(0, ios::beg); //Reset Ifstream Position To Beginning Of File

	readPlayers(ifs, cmd, output, numOfPlayers, head);	//Store All Players In A Linked List
	system("pause");
}

std::string removeLeadingSpace(const std::string& str, const string& ws = " \n")
{
	const auto strBegin = str.find_first_not_of(ws);
	if (strBegin == std::string::npos) {
		return "";
	}
	const auto strEnd = str.find_last_not_of(ws);
	const auto strRange = strEnd - strBegin + 1;

	return str.substr(strBegin, strRange);
}

void readPlayers(ifstream &ifs, ifstream &cmd, ofstream &output, int numOfPlayers, Player *head) {
	Player *cu = head;
	for (int i = 0; i < numOfPlayers; i++) {
		Player *temp = new Player;
		string tempName = "";
		getline(ifs, tempName, ',');
		tempName = removeLeadingSpace(tempName);
		int tempJersey;
		ifs >> tempJersey;
		string tempPosition;
		ifs >> tempPosition; //Done twice to get rid of the comma in the input files.
		ifs >> tempPosition;
		temp->name = tempName;
		temp->jersey = tempJersey;
		temp->position = tempPosition;
		getline(ifs, tempName, '\n'); //Used to fix whitespace issues
		if (i == 0) {
			temp->back = 0;
			head = temp;
			cu = temp;
		}
		else if (i == numOfPlayers - 1) {
			temp->back = cu;
			temp->next = 0;
			cu->next = temp;
		}
		else {
			temp->back = cu;
			cu->next = temp;
			cu = cu->next;
		}
	}
	sortPlayers(numOfPlayers, head);
	findIndex(cmd, output, numOfPlayers, head);
}

void printPlayers(int numOfPlayers, Player *head) {
	Player *cu = head;
	for (int i = 0; i < numOfPlayers; i++) {
		cout << "Player " << i+1 << ": " << cu->name << ", " << cu->jersey << ", " << cu->position << ".\n";
		cu = cu->next;
	}
	
}

void sortPlayers(int numOfPlayers, Player *head) {
	Player *cu = head;
	Player *temp = cu->next;
	for (int a = 0; a < numOfPlayers - 1; a++) {
		cu = head;
		temp = cu->next;
		for (int i = 1; i < numOfPlayers; i++) {
			if (isBigger(cu->name, temp->name)) {
				string n = temp->name;
				string p = temp->position;
				int j = temp->jersey;
				temp->name = cu->name;
				temp->position = cu->position;
				temp->jersey = cu->jersey;
				cu->name = n;
				cu->position = p;
				cu->jersey = j;
			}
			cu = cu->next;
			temp = temp->next;
		}
	}
	printPlayers(numOfPlayers, head);
}

bool isBigger(string a, string b) {
	for (unsigned int i = 0; i < a.length(); i++) {
		if (a[i] > b[i]) {
			return true;
		}
		else {
			if (a[i] < b[i]) {
				return false;
			}
		}
	}
	return true;
}

void findIndex(ifstream &cmd, ofstream &output, int numOfPlayers, Player *head) {
	int numOfSearches = 0;
	while (!cmd.eof()) {
		string s;
		getline(cmd, s);
		if (s != "") {
			numOfSearches++;
		}
	}
	cmd.clear();
	cmd.seekg(0, ios::beg);
	for (int i = 0; i < numOfSearches; i++) {
		string tempName;
		getline(cmd, tempName);
		while (tempName == "") {
			getline(cmd, tempName);
		}
		output << tempName << ": " << searchName(numOfPlayers, head, tempName) << endl;
	}

}

int searchName(int numOfPlayers, Player *head, string tempName) {
	Player *cu = head;
	for (int v = 0; v < numOfPlayers; v++) {
		if (cu->name == tempName) {
			return v;
		}
		else {
			cu = cu->next;
		}
	}
	return -1;
}