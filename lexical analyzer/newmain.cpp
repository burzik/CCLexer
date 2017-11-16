#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_set>
#include "newmain.h"

using namespace std;

struct info
{
	string name;
	string type;
	int position;
};

struct ifBlock
{
	string exp;
	string state;
};

void writeRecord(string name, string type, int &position, vector<info> &rec);
void showRecords(vector<info> information);
void showName(vector<info> information, string type);

bool checkSymbol(char symbol);
void errorHandler(string name, int line, vector<info> &rec);

void showChars(char *txt[])
{
	for (int i = 0; i < sizeof(txt) / 4; i++)
		cout << txt[i] << endl;
}


void key(const unordered_set<string> &keywords)
{
	for (auto &keyword : keywords) {
		cout << keyword << endl;
	}
}

bool isSpace(char c)
{
return c == ' ' || c == '\n' || c == '\r' || c == '\t';
}

bool isAlpha(char c)
{
	return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

bool isSetOf(const string &value, const unordered_set<string> &s)
{
	return s.find(value) != s.end();
}

bool isSetOf(char c, const unordered_set<string> &s)
{
	return s.find(string(1, c)) != s.end();
}

int main()
{
	unordered_set<string> keywords = {
		"procedure",
		"inherited",
		"Word",
		"Boolean",
		"Begin",
		"End",
		"case",
		"of",
		"if",
		"then",
		"nil" };
	unordered_set<string> delims = {
		"(",
		")",
		".",
		",",
		":",
		";",
		"<",
		"<>" };

	key(keywords);

	vector<info> totals;
	int keyPos = 0;
	int idPos = 0;
	int delPos = 0;
	bool bDelCheker = false;
	bool bKeyChecker = false;
	string sCurrentWord;
	char c;
	int line = 1;
	bool hasError = false;

	//fff
	ifBlock block = { "", "" };
	bool bIfBlock = false;
	bool bExpr = false;
	bool bState = false;
	bool bIfError = false;
	info ifError = {};
	vector<info> vIfError;
	//cout << "Enter the name of an existing text file: ";
	char filename[256] = "input.txt";
	//cin.get(filename, 256);  
	ifstream source(filename);
	if (!source)
	{
		cout << "File open error!" << endl;
		_getch();
		return 0;
	}

	while (!source.eof())
	{
		while (isSpace(source.peek())) {
			char c = source.get();
			if (c == '\n')
				line++;
		}

		if (source.eof())
			break;
		
		if (isAlpha(source.peek()))
		{
			string word;
			do {
				word += source.get();
			} while (isAlpha(source.peek()));

			if (isSetOf(word, keywords)) {
				// Keyword
				writeRecord(word, "KEYWORD", keyPos, totals);
			}
			else {
				// Identificator
				writeRecord(word, "ID", idPos, totals);
			}
		}
		//else if (isDigit(source.peek())) {
		//}
		else {
			// Probably delimiter
			if (isSetOf(source.peek(), delims)) {
				string delimiter(1, char(source.get()));
				if (isSetOf(delimiter + char(source.peek()), delims)) {
					delimiter += source.get();
				}
				// delimiter!
				writeRecord(delimiter, "DELIM", delPos, totals);
			}
			else {
				// WTF
				errorHandler(string(1, source.peek()), line, totals);
				source.get();
			}
		}
	}
	source.close();

	// TODO
	cout << "\nList of KEYWORDS\n";
	//showChars(keywords);

	for (auto &keyword : keywords)
		cout << "   " << keyword << endl;

	cout << "\nList of DELIMETERS\n";
	for (auto &delim : delims)
		cout << "   " << delim << endl;
	//----

	cout << "\n\n\tKey words:\n";
	showName(totals, "KEYWORD");
	cout << "\n\tDelimeters:\n";
	showName(totals, "DELIM");
	cout << "\n\tIdentificators:\n";
	showName(totals, "ID");
	cout << "\n\t\tTABLE:\n";
	showRecords(totals);

	if (hasError)
	{
		cout << "\nUnknown symbols:\n";
		showName(totals, "ERROR");
	}

	//new
	//cout << "\nexpr\n" << block.exp << "\nstate\n" << block.state << endl;
	//if (bIfError)
	showRecords(vIfError);
	//

	_getch();
	return 0;
}

void showName(vector<info> information, string type)
{
	cout << "\nPosition|\tName\t\n--------------------------------------------\n";
	for (size_t i = 0; i < information.size(); i++)
		if (type == information[i].type)
			cout << information[i].position << "\t|\t" << information[i].name << endl;
	cout << "--------------------------------------------\n";
}

void showRecords(vector<info> information)
{
	cout << "\nType\t|    Position\t|\tName\n--------------------------------------------\n";
	for (size_t j = 0; j < information.size(); j++)
		cout << information[j].type << "\t|\t" << information[j].position << "\t|\t" << information[j].name << endl;
	cout << "--------------------------------------------\n";
}

/*void showChars(char *txt[])
{
for (int i = 0; i < sizeof(txt) / 4; i++)
cout << txt[i] << endl;
}*/

void errorHandler(string name, int line, vector<info> &rec)
{
	info error;
	error.name = name;
	error.type = "ERROR";
	error.position = line;
	rec.push_back(error);
}

void writeRecord(string name, string type, int &position, vector<info> &rec)
{
	info information;
	information.name = name;
	information.type = type;
	information.position = position;
	rec.push_back(information);
	position++;
}