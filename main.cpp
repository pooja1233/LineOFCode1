/* 
 * File:   main.cpp
 * Author: Pooja
 *
 * Created on February 12, 2015, 11:11 AM
 */
// Author: Nathan McCollister
// Written in c++
// Created: 05/25/2014
// Edited: 06/01/2014
// This program is designed to count the number of lines of code in a file
//   and display them onto the screen. If the file does not exist it will just
//   end the execution of the program without error.
// This is an edit from the original program that has just the basic
//   components and platform independent.
//


#include <iostream>
#include <fstream>
#include <string>


using namespace std;					// std:: is used in lieu of using namespace std

#define MAX_FUNCTIONS 100
#define MAX_NAME 30

// define struct of function data
typedef struct
{
	int funcLOC;
	char funcName[MAX_NAME];
}FUNCDATA;

int countFuncLOC(fstream &file, FUNCDATA *funcDataPtr);

 main(){
	string sFileName;
	string sDir;       // the directory of the file
	fstream fin;
	FUNCDATA funcDataTable[MAX_FUNCTIONS];
	
	// initialize the funcDataTable
	for (int i = 0; i < MAX_FUNCTIONS; i++)
	{
		funcDataTable[i].funcLOC = 0;
		for (int j = 0; j < MAX_NAME; j++)
		{
			funcDataTable[i].funcName[j] = ' ';
		}
	}
	cout << "Please enter the file name: ";
	cin >> sFileName;
	sDir = "C:\\Users\\Pooja\\Desktop" + sFileName;
	fin.open(sDir.c_str(), ios::in);
	if (fin.is_open())
	{
		cout << "\nFilename: " << sFileName << endl;
		cout << "The total number of LOC in the file is: " << countFuncLOC(fin, funcDataTable) << endl << endl;
		
		// print function names and the number of LOC in each function
		for (int i = 0; i < MAX_FUNCTIONS; i++)
		{
			if (funcDataTable[i].funcLOC != 0)
			{
				cout << "Function name: ";
				for (int j = 0; j < MAX_NAME; j++)
				{
					if (funcDataTable[i].funcName[j] != ' ')
					{
						cout << funcDataTable[i].funcName[j];
					}
				}
				cout << endl;
				cout << "The number of LOC in this funciton is: " << funcDataTable[i].funcLOC << endl << endl;
			}
		}
		fin.close();
	}
	else // file does not exist
	{
		cout << "filename: " << sFileName << " - File does not exist.\n\n";
	}

	cout << "Press \"Enter\" key to continue...\n";
	cin.ignore();
	getchar();

	return 0;
}

int countFuncLOC(fstream &file, FUNCDATA *funcDataPtr)
{
	bool bRunTest = true;			// used to determine if the loop should search the current line.
	bool bBlockComment = false;		// a flag to determine if the program is currently inside of a block comment
	bool bFunction = false;         // if in a function
	int iTotalLOC = 0;
	int iFuncLOC = 0;               // the number of LOC in each function
	int iParenthesisIndex = 0;      // the index of parenthesis
	int iNumBraceLt = 0;            // the number of left braces
	int iFuncNum = 0;               // the number of functions in the file
	string tmpString;
	string doString;                // find the function name in the line

	while (!file.eof())			    // will loop until end of file
	{
		getline(file, tmpString);
		//cout << tmpString << endl;
		bRunTest = true;			// reset to true before each run through
		for (size_t i = 0; i < tmpString.length(); i++){		// a loop to scan each line character by character
			switch (tmpString[i]){
			case '/':				// if it starts with an '/' then check if it is a line comment, block comment, or neither
				if (i < tmpString.length() - 1)
					if (tmpString[i + 1] == '/')
						bRunTest = false;
					else if (tmpString[i + 1] == '*'){
						bBlockComment = true;
						i++;
					}
					break;
			
                                        
			
                                        
			case '{':
				iNumBraceLt++;
				if (tmpString.find('}') == tmpString.npos)        // if there's no } in the same line, count the line as a LOC
					iTotalLOC++;
				//cout << "Found a left brace, the number of left brace is: " << iNumBraceLt << endl;
				break;
			case '}':
				iNumBraceLt--;
				//cout << "Found a right brace, the number of left brace is: " << iNumBraceLt << endl;
				if (iNumBraceLt == 1)
				{
					bFunction = false;
					iFuncNum++;
				}
			default:				// all other characters signify a LOC assuming not in a comment
				if (bBlockComment == true || bRunTest == false)
					break;
				iTotalLOC++;
				bRunTest = false;
			}
		}
		if (tmpString.length() != 0)
		{
			if (tmpString[tmpString.length() - 1] == ')' && iNumBraceLt == 0 && bBlockComment == false)
			{
				bFunction = true;
				iParenthesisIndex = tmpString.find('(');
				//cout << iParenthesisIndex << endl;
				if (tmpString[iParenthesisIndex - 1] == ' ')      // function name and the parathesis are saperated
				{
					doString = tmpString.substr(0, iParenthesisIndex - 1);
					int iSpaceIndex = doString.rfind(' ');        // the index of the last space in front of the function name
					string nameString = doString.substr(iSpaceIndex + 1, iParenthesisIndex - iSpaceIndex - 2);
					for (int i = 0; i < nameString.length(); i++)
					{
						funcDataPtr[iFuncNum].funcName[i] = nameString[i];
					}					
					//funcDataPtr[iFuncNum - 1].funcName = doString.substr(iSpaceIndex + 1, iParenthesisIndex - iSpaceIndex - 2);
				}
				else // function name and the parathesis are not saperated
				{
					doString = tmpString.substr(0, iParenthesisIndex);
					int iSpaceIndex = doString.rfind(' ');        // the index of the last space in front of the function name
					string nameString = doString.substr(iSpaceIndex + 1, iParenthesisIndex - iSpaceIndex - 1);
					for (int i = 0; i < nameString.length(); i++)
					{
						funcDataPtr[iFuncNum].funcName[i] = nameString[i];
					}
					//funcDataPtr[iFuncNum - 1].funcName = doString.substr(iSpaceIndex + 1, iParenthesisIndex - iSpaceIndex - 1);
				}
				//cout << doString << endl;	
			}
		}
		if (iNumBraceLt > 0 && !bBlockComment)
		{
			iFuncLOC++;
		}
		if (!bFunction && iFuncLOC > 0)
		{
			if (iFuncLOC > 1)         // assign the number of LOC in each function to funcDataTable
				funcDataPtr[iFuncNum - 1].funcLOC = iFuncLOC;
			iFuncLOC = 0;
		}
	}
	return iTotalLOC;
}


