#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_set>
#include "newmain.h"

using namespace std;

struct chetverka {
	string label;
	string instruction;
	string op1, op2, res;

	chetverka(const string &label, const string &instruction, const string &op1, const string &op2, const string res)
		: label(label)
		, instruction(instruction)
		, op1(op1)
		, op2(op2)
		, res(res)
	{
		count++;
	}

	chetverka(const string &instruction, const string &op1 = "", const string &op2 = "", const string res = "")
		: instruction(instruction)
		, op1(op1)
		, op2(op2)
		, res(res)
	{
		stringstream ss;
		ss << count;
		label = ss.str();
		count++;

	}

	static size_t count;
};

size_t chetverka::count = 0;

ostream& operator<<(ostream &os, const chetverka &ch) {
	return os
		<< setw(8) << left << ch.label
		<< setw(8) << right << ch.instruction << ","
		<< setw(16) << right << ch.op1 << ","
		<< setw(16) << right << ch.op2 << ","
		<< setw(8) << right << ch.res;
}

vector<chetverka> instructions;

size_t tmp_var_count = 0;

string get_new_tmp() {
	stringstream ss;
	ss << "T" << ++tmp_var_count;
	return ss.str();
}

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
void errorHandler(string name, int line, vector<info> &rec);

void showRecords(vector<info> information);
void showName(vector<info> information, Type type);
void showSet(const unordered_set<string> &keywords);

bool isSpace(char c);
bool isAlpha(char c);
bool isSetOf(const string &value, const unordered_set<string> &s);
bool isSetOf(char c, const unordered_set<string> &s);

using info_iter = vector<info>::iterator;

pair<info_iter, string> parseExpression(info_iter it, info_iter end, vector<info> &rec);
info_iter parseParamList(info_iter it, info_iter end, vector<info> &rec);
info_iter parseStatement(info_iter it, info_iter end, vector<info> &rec);
info_iter parseIfStatement(info_iter it, info_iter end, vector<info> &rec);
info_iter parse(info_iter it, info_iter end, vector<info> &rec);

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

	vector<info> totals;
	vector<info> ifExpr;
	int keyPos = 0;
	int idPos = 0;
	int delPos = 0;
	int line = 1;
	bool hasError = false;
	char c;
	char filename[256] = "input.txt";

	//cout << "Enter the name of an existing text file: ";
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
		//Skip spaces
		while (isSpace(source.peek())) {
			char c = source.get();
			if (c == '\n')
				line++;
		}

		if (source.eof())
			break;

		//check keyword or identificator
		if (isAlpha(source.peek()))
		{
			string word;
			do 
			{
				word += source.get();
			} while (isAlpha(source.peek()));

			if (isSetOf(word, keywords))
			{
				//Keyword
				writeRecord(word, Type::Keyword, keyPos, totals);
			}
			else 
			{
				//Identificator
				writeRecord(word, Type::Identificator, idPos, totals);
			}
		}
		//else if (isDigit(source.peek())) { }
		else 
		{
			//Check delimeter
			if (isSetOf(source.peek(), delims))
			{
				string delimiter(1, char(source.get()));
				if (isSetOf(delimiter + char(source.peek()), delims))
				{
					delimiter += source.get();
				}
				writeRecord(delimiter, Type::Delimiter, delPos, totals);
			}
			else 
			{
				// Handle unknown symbol
				errorHandler(string(1, source.peek()), line, totals);
				hasError = true;
				source.get();
			}
		}
	}
	source.close();
	
	//parse logical blocks
	parse(totals.begin(), totals.end(), ifExpr);

	//Show info
	cout << "\nList of KEYWORDS\n";
	showSet(keywords);
	cout << "\nList of DELIMETERS\n";
	showSet(delims);
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

	if (ifExpr.size() != 0)
		showRecords(ifExpr);
	else cout << "\nNo errors\n";

	cout
		<< setw(8) << left << "LABEL"
		<< setw(8) << right << "INSTR" << ","
		<< setw(16) << right << "OP1" << ","
		<< setw(16) << right << "OP2" << ","
		<< setw(8) << right << "RES\n";
	for (auto &ch : instructions) {
		cout << ch << '\n';
	}
	
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

void showSet(const unordered_set<string> &keywords)
{
	for (auto &keyword : keywords) {
		cout << keyword << endl;
	}
}

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

pair<info_iter, string> parseExpression(info_iter it, info_iter end, vector<info> &rec)
{
	if (it == end)
		return make_pair(end, "");
	//check 1 elem
	if (it->type != Type::Keyword && it->type != Type::Identificator) {
		//error
		errorHandler("If | Error near '" + it->name + "'", 0, rec);
	}
	auto op1 = it++;
	//check <>
	if (it->type != Type::Delimiter || it->name != "<>") {
		//error
		errorHandler("If | Error near '<>'", 0, rec);
	}
	auto instr = it++;
	//check 2 elem
	if (it->type != Type::Keyword && it->type != Type::Identificator) {
		//error
		errorHandler("If | Error near '" + it->name + "'", 0, rec);
	}
	auto op2 = it++;

	auto tmp_var = get_new_tmp();
	instructions.push_back(chetverka("NE", op1->name, op2->name, tmp_var));

	return make_pair(it, tmp_var);
}

info_iter parseParamList(info_iter it, info_iter end, vector<info> &rec)
{
	if (it == end)
		return end;

	if (it->type != Type::Delimiter || it->name != "(") {
		//error
		errorHandler("if | Error near '('", 0, rec);
	}
	++it;

	bool hasSeparator = true;
	while (it->name != ")" && it->name != ";")
	{
		if (it->type != Type::Keyword && it->type != Type::Identificator)
		{
			
			if (it->type != Type::Delimiter && it->name != ",")
				errorHandler("If | Error near '" + it->name + "'", 0, rec);
			else if (it->name == "(")
			{
				errorHandler("If | Error near '" + it->name + "'", 0, rec);
				while (it->name == "(")
					it++;
			}
				else hasSeparator = true;

				
		}
		else if (!hasSeparator)
		{
			errorHandler("If | Error near '" + it->name + "'", 0, rec);
		}
		else
		{
			hasSeparator = false;
			auto label = it->name;
			instructions.push_back(chetverka("push", label, "T2", ""));
		}
		
		++it;
		if (it == end)
		{
			errorHandler("If | Can't find ')'", 0, rec);
			return end;
		}
	}

	if (it->type != Type::Delimiter || it->name != ")") {
		//error
		errorHandler("If | Can't find ')'", 0, rec);
		return it;
	}
	++it;
	return it;
}

info_iter parseStatement(info_iter it, info_iter end, vector<info> &rec)
{
	if (it == end)
		return end;
	//check 1 elem
	if (it->type != Type::Identificator) {
		//error
		errorHandler("If | Error near '" + it->name + "'", 0, rec);
	}

	auto label = it->name;
	++it;
	

	//check dot
	if (it->type != Type::Delimiter || it->name != ".") {
		//error
		errorHandler("If | Error '.' near '" + it->name + ".'", 0, rec);
		return end;
	}
	++it;
	//check func
	if (it->type != Type::Identificator) {
		//error
		errorHandler("If | Error near '" + it->name + "'", 0, rec);
	}

	auto label2 = it->name;
	//instructions.push_back(chetverka("func", label, label2, "T2"));

	++it;
	//check variables

	//instructions.push_back(chetverka("push", label));
	it = parseParamList(it, end, rec);
	if (it == end)
	{
		errorHandler("If | Can't find ';'", 0, rec);
		return end;
	}

	if (it->type != Type::Delimiter || it->name != ";") {
		//error
		errorHandler("If | Can't find ';'", 0, rec);
	}

	//auto label = it->name;
	instructions.push_back(chetverka("call", label, label2, "T2"));

	++it;
	return it;
}

info_iter parseIfStatement(info_iter it, info_iter end, vector<info> &rec)
{
	if (it == end)
		return end;

	auto label_end = "5";

	if (it->type != Type::Keyword || it->name != "if") {
		//error
		errorHandler("If |Can't find if", 0, rec);
		return end;
	}
	//instructions.push_back(chetverka("blcif"));
	++it;

	auto [it, tmp_var] = parseExpression(it, end, rec);

	instructions.push_back(chetverka("brz", tmp_var, label_end));

	if (it->type != Type::Keyword || it->name != "then") {
		//error
		errorHandler("If | Can't find 'then'", 0, rec);
		return end;
	}
	++it;

	it = parseStatement(it, end, rec);

	instructions.push_back(chetverka(label_end, "nop", "", "", ""));

	return it;
}

info_iter parse(info_iter it, info_iter end, vector<info> &rec)
{
	if (it == end)
		return end;
	//while (it->name != "if") ++it;
	return parseIfStatement(it, end, rec);
}