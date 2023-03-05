#include <iostream>
#include <sstream>
#include <fstream>
using namespace std;
int Find(string source, string toFind);
string Find(fstream &sourceStream, string find);
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
		if (argc!=3) throw MyException("Incorrect arguments");
		find = *(argv + 1);
		source = *(argv+2);
		fstream stream = fstream();
		stream.open(source, ios::in);
		if (!stream)
		{
			throw MyException("File not found");
		}
		cout << Find(stream, find);
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
	found = source.find(toFind);
	return found;
}
/// <summary>
/// returns line from source file where desired string is located
/// </summary>
/// <param name="sourceStream">Stream with opened file</param>
/// <param name="find">What to find</param>
/// <returns>Full line with desired string</returns>
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