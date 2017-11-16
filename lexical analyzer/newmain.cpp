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

enum class Type {
	Error,
	Keyword,
	Identificator,
	Delimiter,
};

ostream& operator<<(ostream &os, Type type)
{
	switch (type) {
	default:
	case Type::Error: os << "ERROR"; break;
	case Type::Keyword: os << "KEYWORD"; break;
	case Type::Identificator: os << "ID"; break;
	case Type::Delimiter: os << "DELIM"; break;
	}
	return os;
}

struct info
{
	string name;
	Type type;
	int position;
};

struct ifBlock
{
	string exp;
	string state;
};

void writeRecord(string name, Type type, int &position, vector<info> &rec);
void showRecords(vector<info> information);
void showName(vector<info> information, Type type);

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

using info_iter = vector<info>::iterator;

info_iter parseExpression(info_iter it, info_iter end)
{
	if (it == end)
		return end;
	return it;
}

info_iter parseStatement(info_iter it, info_iter end)
{
	return it;
}

info_iter parseIfStatement(info_iter it, info_iter end)
{
	if (it == end)
		return end;
	
	if (it->type != Type::Keyword || it->name != "if") {
		//error
		return end;
	}
	++it;

	it = parseExpression(it, end);

	if (it->type != Type::Keyword || it->name != "then") {
		//error
		return end;
	}
	++it;

	it = parseStatement(it, end);
	return it;
}

info_iter parse(info_iter it, info_iter end)
{
	if (it == end)
		return end;

	return parseIfStatement(it, end);
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
				writeRecord(word, Type::Keyword, keyPos, totals);
			}
			else {
				// Identificator
				writeRecord(word, Type::Identificator, idPos, totals);
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
				writeRecord(delimiter, Type::Delimiter, delPos, totals);
			}
			else {
				// WTF
				errorHandler(string(1, source.peek()), line, totals);
				source.get();
			}
		}
	}
	source.close();

	parse(totals.begin(), totals.end());

	//for (auto &i : totals) {

	//}

	//for (vector<info>::reverse_iterator i = totals.rbegin(); i != totals.rend(); ++i) {

	//}

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
	showName(totals, Type::Keyword);
	cout << "\n\tDelimeters:\n";
	showName(totals, Type::Delimiter);
	cout << "\n\tIdentificators:\n";
	showName(totals, Type::Identificator);
	cout << "\n\t\tTABLE:\n";
	showRecords(totals);

	if (hasError)
	{
		cout << "\nUnknown symbols:\n";
		showName(totals, Type::Error);
	}

	//new
	//cout << "\nexpr\n" << block.exp << "\nstate\n" << block.state << endl;
	//if (bIfError)
	showRecords(vIfError);
	//

	_getch();
	return 0;
}

void showName(vector<info> information, Type type)
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
	error.type = Type::Error;
	error.position = line;
	rec.push_back(error);
}

void writeRecord(string name, Type type, int &position, vector<info> &rec)
{
	info information;
	information.name = name;
	information.type = type;
	information.position = position;
	rec.push_back(information);
	position++;
}