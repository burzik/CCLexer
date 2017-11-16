#include <iostream>
#include <conio.h>
#include <stdio.h>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>

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


void key(const vector<string> &keywords)
{
	for (int i = 0; i < keywords.size(); i++) {
		cout << keywords[i] << endl;
	}

	for (auto &keyword : keywords) {
		cout << keyword << endl;
	}
}

int mainold()
{
	vector<string> keywords = {
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
	vector<string> delims = {
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
	string sCurrentChar, sCurrentWord;
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
	while (source.get(c)) 
	{
		
		sCurrentChar = c;

		if (sCurrentWord == "<>")
			int a = 1;

		if (checkSymbol(c))
		{
			if (sCurrentChar == " " || sCurrentChar == "\n")
			{
				for (int j = 0; j < keywords.size(); j++)
				{
					if (keywords[j] == sCurrentWord)
					{
						writeRecord(sCurrentWord, "KEYWORD", keyPos, totals);
						bKeyChecker = true;
					}

					if (sCurrentWord == "if")
					{
						bIfBlock = true;
						bExpr = true;
						bState = false;
					}

					if (sCurrentWord == "then")
					{
						//block.exp += " ";
						bIfBlock = true;
						bExpr = false;
						bState = true;
					}

				}

				if (!bKeyChecker)
				{
					if (sCurrentWord != "<>")
					{
						if (sCurrentChar != " " && sCurrentChar != "\n" && sCurrentChar != "\t")
						{
							writeRecord(sCurrentWord, "ID", idPos, totals);
							bKeyChecker = false;
						}
						else if (sCurrentWord != " " && sCurrentChar != "" && sCurrentWord != " " && sCurrentWord != "")
						{
							if(sCurrentWord != "<")
								writeRecord(sCurrentWord, "ID", idPos, totals);
							else writeRecord(sCurrentWord, "DELIM", delPos, totals);
						}
					}
					else
					{
						if (sCurrentWord == "<>")
						{
							writeRecord(sCurrentWord, "DELIM", delPos, totals);
						}
						else {

							//NO ONE THERE

							writeRecord(sCurrentWord, "ID", idPos, totals);
						}
					}
				}
				bKeyChecker = false;
				sCurrentWord = "";
			}

			for (int j = 0; j < (sizeof(delims) / 4); j++)
			{
				if (delims[j] == sCurrentChar)
				{
					for (int k = 0; k < sizeof(keywords) / 4; k++)
					{
						if (keywords[k] == sCurrentWord)
						{
							writeRecord(sCurrentWord, "KEYWORD", keyPos, totals);
							//check delim
							writeRecord(sCurrentChar, "DELIM", delPos, totals);
							bDelCheker = true;
						}
					}
				
					//new
					if (sCurrentChar == ";" && bIfBlock)
					{
						bIfBlock = false;
						bExpr = false;
						bState = false;

						//split all words 

						string fullblock = block.exp + block.state;
						string element = "";
						string other = "";
						fullblock = fullblock.substr(1);

						//find first term
						if (fullblock.find_first_of(" ") < fullblock.find_first_of("<"))
						{
							element = fullblock.substr(0, fullblock.find_first_of(" "));
							other = fullblock.substr(fullblock.find_first_of(" ")+1);
						}
						else 
						{
							element = fullblock.substr(0, fullblock.find_first_of("<")); 
							other = fullblock.substr(fullblock.find_first_of("<"));
						}
						// check element
						bool err = true;
						for (size_t i = 0; i < sizeof(keywords) / 4; i++)
							if (keywords[i] == element)
								err = false;
						//find operator
						//find second term
						//find then
						//find func name
						//find .
						//find function
						//find list of arguments
						//find argumet


						/*
						if (fullblock.find(" then ") == -1)
							writeRecord("Can't find logical then", "SYNTAX", line, vIfError);
						else {
							int a = fullblock.find_last_of(" then ");
							string expr = fullblock.substr(0, fullblock.find_first_of(" then "));
							string state = fullblock.substr(0, fullblock.find_last_of(" then "));



							string thens = block.exp.substr(block.exp.find_last_of(" ") + 1);
							block.exp = block.exp.substr(block.exp.find_first_of(" ")+1, block.exp.size());
							int z = 0;
							if (thens == "then")
								z = 1;
							//block.exp = block.exp.substr(1, block.exp.find_first_of("then"));
							
							
							block.exp = block.exp.substr(0, block.exp.size() - 5);
							block.state = block.state.substr(1, block.state.size());

							//check expr
							//TODO Global error and type
							//First term
							string sFirstTerm = block.exp.substr(0, block.exp.find_first_of("<"));
							sFirstTerm = sFirstTerm.substr(0, sFirstTerm.find_first_of(" "));
							bIfError = true;
							for (size_t i = 0; i < totals.size(); i++)
							{
								if (totals[i].name == sFirstTerm)
									bIfError = false;
							}

							if (bIfError)
								writeRecord("Error in expression near <>", "SYNTAX", line, vIfError);

							//Second term
							string sLastTerm = block.exp.substr(block.exp.find_last_of(">") + 1);
							sLastTerm = sLastTerm.substr(sLastTerm.find_last_of(" ") + 1);

							bIfError = true;
							for (size_t i = 0; i < totals.size(); i++)
							{
								if (totals[i].name == sLastTerm)
									bIfError = false;
							}

							if (bIfError)
								writeRecord("Error in expression near <>", "SYNTAX", line, vIfError);

							//NotEqual
							bIfError = false;
							string sNotEqual = "<>"; //check this symbol
							if (block.exp.find("<>") == -1)
								writeRecord("Error in expression cant find <> near " + sFirstTerm, "SYNTAX", line, vIfError);



							//check state
							if (block.state.find(".") == -1)
								writeRecord("Error in state cant find function name <>", "SYNTAX", line, vIfError);
							string sFuncName = block.state.substr(0, block.state.find_first_of("."));
							string sFunc = block.state.substr(block.state.find_last_of(".") + 1);

							//check function name
							bIfError = true;
							for (size_t i = 0; i < totals.size(); i++)
							{
								if (totals[i].name == sFuncName)
									bIfError = false;
							}
							if (bIfError) writeRecord("Error in IF state", "SYNTAX", line, totals);

							//check function
							string sFuncName2 = sFunc.substr(0, sFunc.find_first_of("("));

							bIfError = true;
							for (size_t i = 0; i < totals.size(); i++)
							{
								if (totals[i].name == sFuncName2)
									bIfError = false;
							}
							if (bIfError) writeRecord("Error in IF state", "SYNTAX", line, totals);


							string sFuncParams = sFunc.substr(sFunc.find_first_of("(") + 1);
							sFuncParams = sFuncParams.substr(0, sFuncParams.find_last_of(")"));

							string sFuncParam;
							string sOther = sFuncParams;
							while (sOther != "")
							{

								sFuncParam = sOther.substr(0, sOther.find_first_of(","));

								if (sOther.find(",") != -1)
								{
									sOther = sOther.substr(sOther.find_first_of(","));
									sOther = sOther.substr(sOther.find_first_of(",") + 1);
									if (sOther.substr(0, 1) == " ")
										sOther = sOther.substr(1);
								}
								else sOther = "";

								bIfError = true;
								for (size_t i = 0; i < totals.size(); i++)
								{
									if (totals[i].name == sFuncParam)
										bIfError = false;
								}
								if (bIfError) writeRecord("Error in IF state", "SYNTAX", line, totals);

							}
							//sLastTerm = sLastTerm.substr(sFirstTerm.find_last_of(" "));
							string tst2 = block.exp.substr(0, block.exp.find_last_of(" ") + 1);
							//cout << tst << endl;
						}

						*/


						block = { "", "" };
					}
						
					//


					if (sCurrentWord == "<>")
					{
						writeRecord(sCurrentWord, "DELIM", delPos, totals);
						sCurrentChar = "<";
						sCurrentWord = "";
					}
					else if (sCurrentChar == "<")
					{
						//nothing?
						if (sCurrentWord != "")
						{
							writeRecord(sCurrentWord, "DELIM", delPos, totals);
							sCurrentChar = "<";
							sCurrentWord = "";
							bDelCheker = false;
						}

					}
					else if (!bDelCheker)
					{
						if (sCurrentWord != "")
						{
							writeRecord(sCurrentWord, "ID", idPos, totals);
							bDelCheker = false;
						}
						writeRecord(sCurrentChar, "DELIM", delPos, totals);
					}
					if (sCurrentChar != "<") 
					{
						if (bIfBlock && bExpr)
							block.exp += sCurrentChar;
						else if (bIfBlock && bState)
							block.state += sCurrentChar;
						sCurrentChar = "";
						sCurrentWord = "";
					}
					bDelCheker = false;
				}
				
				if (sCurrentChar == ">")
				{
					sCurrentWord += sCurrentChar;
					writeRecord(sCurrentWord, "DELIM", delPos, totals);
					if (bIfBlock && bExpr)
						block.exp += sCurrentChar;
					else if (bIfBlock && bState)
						block.state += sCurrentChar;
					sCurrentChar = "";
					sCurrentWord = "";
					
				}
			};

			if (sCurrentChar == "\n") line++;
			if (sCurrentChar != " " && sCurrentChar != "\n" && sCurrentChar != "\t" && sCurrentChar != "")
				sCurrentWord += sCurrentChar;
			
			if (bIfBlock && bExpr)
				block.exp += sCurrentChar;
			else if (bIfBlock && bState)
				block.state += sCurrentChar;
		}
		else
		{ 
			errorHandler(sCurrentChar, line, totals); 
			hasError = true;
		}
	}
	source.close();
	
	// TODO
	cout << "\nList of KEYWORDS\n";
	//showChars(keywords);
	
	for (int j = 0; j < sizeof(keywords) / 4; j++)
		cout << "   " << keywords[j] << endl;

	cout << "\nList of DELIMETERS\n";
	for (int j = 0; j < sizeof(delims) / 4; j++)
		cout << "   " << delims[j] << endl;
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

bool checkSymbol(char symbol)
{
	if ((int(symbol) >= 65 && int(symbol) <= 90) || (int(symbol) >= 97 && int(symbol) <= 122) || int(symbol) == 40 || int(symbol) == 41 || int(symbol) == 44 ||
			int(symbol) == 46 || (int(symbol) >= 58 && int(symbol) <= 60) || int(symbol) == 62 || int(symbol) == 32 || int(symbol) == 10 || int(symbol) == 9)
		return true;
	else return false;
}

void showName(vector<info> information, string type)
{
	cout << "\nPosition|\tName\t\n--------------------------------------------\n";
	for (size_t i = 0; i < information.size(); i++)
		if (type == information[i].type)
			cout << information[i].position <<"\t|\t" << information[i].name << endl;
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