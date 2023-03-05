#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;
int Find(string source, string toFind);
string Find(fstream &sourceStream, string find);
string Find(fstream& sourceStream, string find, string options);
//Simple exception class for all of my purposes. Contains only getter for description.
class MyException { private: string description; public: MyException(string _description) { description = _description; }; string GetDescription() { return description; } };
int main(int argc, char* argv[])
{
	string source, find;
	if (argc == 1)
	{
		cout << "Give a string from which to search for: ";
		getline(cin, source);
		cout << "Give search string: ";
		getline(cin, find);
		int res = Find(source, find);
		if (res == -1)
		{
			cout << "\"" << find << "\" NOT found in \"" << source << "\"";
			return 0;
		}
		cout << "\"" << find << "\" found in \"" << source << "\" at position: " << res;
	}
	else try
	{
		string options;
		switch (argc)
		{
		case 3:
			find = *(argv + 1);
			source = *(argv + 2);
			break;
		case 4:
			options= *(argv + 1);
			find = *(argv + 2);
			source = *(argv + 3);
			break;
		default:
			throw MyException("Incorrect arguments");
		}
		fstream stream = fstream();
		stream.open(source, ios::in);
		if (!stream)
		{
			throw MyException("File not found or cannot be read");
		}
		cout << Find(stream, find, options);
	}
	catch (MyException e)
	{
		cout << e.GetDescription();
	}

	return 0;
}
/// <summary>
/// returns position of "toFind" string in source string or -1 if not found
/// </summary>
/// <param name="source">Where to find</param>
/// <param name="toFind">What to find</param>
/// <returns>Position of first symbol in source string</returns>
int Find(string source, string toFind)
{
	int found = 0;
	//found = source.find(toFind); too simple
	for (found; found <= ((int)source.length()-(int)toFind.length()); found++)
	{
		if (source[found] == toFind[0])
		{   
			bool res = true;
			for (int i = 0; i < toFind.length(); i++)
			{
				if (toFind[i] != source[found + i])
				{
					res = false;
					break;
				}
			}
			if (res) return found;
		}
	}
	return -1;
}
/// <summary>
/// returns position of "toFind" string in source string or -1 if not found ignoring case
/// </summary>
/// <param name="source">Where to find</param>
/// <param name="toFind">What to find</param>
/// <returns>Position of first symbol in source string</returns>
int FindIgnoringCase(string source, string toFind)
{
	int found = 0;
	//found = source.find(toFind); too simple
	for (found; found <= ((int)source.length() - (int)toFind.length()); found++)
	{
		//to the same case
		if (toupper(source[found]) == toupper(toFind[0]))
		{
			bool res = true;
			for (int i = 0; i < toFind.length(); i++)
			{
				if (toupper(toFind[i]) != toupper(source[found + i]))
				{
					res = false;
					break;
				}
			}
			if (res) return found;
		}
	}
	return -1;
}
/// <summary>
/// returns line from source file where desired string is located
/// </summary>
/// <param name="sourceStream">- Stream with opened file</param>
/// <param name="find">- What to find</param>
/// <returns>- Full line with desired string</returns>
string Find(fstream &sourceStream, string find)
{
	string output;
	string line;
	char lastSymb;
	while (!( sourceStream).eof())
	{
		(sourceStream).get(lastSymb);
		if (lastSymb == '\n')
		{
			if (Find(line, find) != -1)
			{
				output += line+'\n';
			}
			line = "";
		}
		else
		{
			line+=lastSymb;
		}
	}
	(sourceStream).close();
	if (output != "")
	{
		return output;
	}
	throw MyException("Not Found");
}
/// <summary>
/// Returns line from source file where desired string is located. Options is string that starts with "-o"(options) and contains letters:
/// o: for an occurences counter at the bottom of an output,
/// l: for printing the number of a line before it,
/// r: for reverse search,
/// i: for ignoring case differences.
/// </summary>
/// <param name="sourceStream">- Stream with opened file</param>
/// <param name="find">- What to find</param>
/// <param name="options">- Options string ("-olori")</param>
/// <returns>- Full line with desired string</returns>
string Find(fstream& sourceStream, string find, string options)
{
	bool printNumber = false;
	bool printTotal = false;
	bool reverseSearch = false;

	//pointer to a find function
	int (*findPtr)(string, string) = &Find;
	if (options[0] == '-' && options[1] == 'o')
	{
		for (char* ptr = &options[2]; ptr <= &options.back(); ptr++)
		{
			if (*ptr == 'l')
			{
				printNumber = true;
			}		
			if (*ptr == 'o')
			{
				printTotal = true;
			}
			if (*ptr == 'i')
			{
				findPtr = &FindIgnoringCase;
			}
			if (*ptr == 'r')
			{
				reverseSearch = true;
			}
		}
	}
	string output;
	string line;
	char lastSymb;
	int lineCounter = 1;
	int occurCounter = 0;
	while (!(sourceStream).eof())
	{
		(sourceStream).get(lastSymb);
		if (lastSymb == '\n')
		{
			if (!(findPtr(line, find) != -1)==reverseSearch)
			{
				if (printNumber)output += to_string(lineCounter) + ":	";
				output += line + '\n';
				occurCounter++;
			}
			lineCounter++;
			line = "";
		}
		else
		{
			line += lastSymb;
		}
	}
	//there is no symbol for the end of document, so have to check last line manually
	//Moreover,  fstream.eof() become true only after trying to read value outside the file, so the last symbol is kinda broken and we need to clear it
	line.pop_back();
	if (!(findPtr(line, find) != -1) == reverseSearch)
	{
		if (printNumber)output += to_string(lineCounter) + ":	";
		output += line + '\n';
		occurCounter++;
	}
	lineCounter++;

	(sourceStream).close();
	string no = "";
	if (reverseSearch)no = "NOT";
	if (output != "")
	{
		if (printTotal)output += "\nOccurrences of lines "+no +" containing \"" + find + "\": " + to_string(occurCounter);
		return output;
	}
	throw MyException("Text not found");
}