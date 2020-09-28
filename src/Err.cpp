#include "Err.hpp"
#include <string>
#include <vector>
#include <csignal>

// Simple error (signal) management
// by Jakub Grzana

#define ERR_FILE "File: "
#define ERR_LINE "Line: "
#define ERR_FUNC "Function: "
#define ERR_COMM "Comment: "

#define ERR_UNRECOGNISED "Unrecognised \"behaviour\". Exitting"
#define ERR_RECEIVED "Received signal id = "
#define ERR_LOCATION "Location number = "

using namespace std;

namespace Err
{
	ostream& o_stream = cout;
	istream& i_stream = cin;
	
	struct ErrMess {
		string file;
		string line;
		string function;
		string comment;
		
		string GetErrorMess(void)
		{
			string output = "";
			output = output + (string)ERR_FILE + file + (string)ERR_BREAK;
			output = output + (string)ERR_LINE + line + (string)ERR_BREAK;
			output = output + (string)ERR_FUNC + function;
			if(comment != "") { output = output + (string)ERR_BREAK + (string)ERR_COMM + comment; }
			return output;
		}
	};
	
	vector<ErrMess> ErrorStack;
	
	Marker::Marker(const string& file, const int& line, const string& function, const string& comment)
	{
		ErrMess err_mess;
		err_mess.file = file;
		err_mess.line = to_string(line);
		err_mess.function = function;
		err_mess.comment = comment;
		ErrorStack.push_back(err_mess);
	}
	Marker::~Marker()
	{
		if(ErrorStack.size()) ErrorStack.pop_back();
	}
	
	vector<string> GetLast(int num)
	{
		vector<string> locations;
		for(int i = 0; i < num ; ++i)
		{
			const int j = ErrorStack.size() - 1 - i;
			if(j < 0) break;
			locations.push_back(ErrorStack[j].GetErrorMess());
		}
		return locations;
	}
	
	string GetLastLocations(int num)
	{
		const vector<string> locations = GetLast(num);
		string output = "";
		for(unsigned long int i = 0; i < locations.size(); ++i)
		{
			output = output + (string)ERR_LOCATION + to_string(i) + (string)"\n" + locations[i] + (string)"\n\n";
		}
		return output;
	}
	
	void Handle(int signal_id)
	{
		o_stream << endl << ERR_RECEIVED << signal_id << endl;
		o_stream << endl << GetLastLocations(DEFAULT_NUMBER_OF_LOCATIONS);
		switch(BEHAVIOUR)
		{
			case 0: {
				return;
			} break;
			
			case 1: {
				exit(signal_id);
			} break;
			
			case 2: {
				char tmp;
				while(i_stream.good())
				{
					o_stream << endl;
					o_stream << "What to do?" << endl;
					o_stream << "e - exit" << endl;
					o_stream << "c - continue normal execution" << endl;
					i_stream >> tmp;
					if(tmp == 'e') { exit(signal_id); }
					else if(tmp == 'c') { return; }
				}
			} break;
			
			default: {
				o_stream << ERR_UNRECOGNISED << endl;
				exit(-10);
			} break;
		}
	}
	
	void AddSignalHandler(int signal_id)
	{
		signal(signal_id,Handle);
	}
}

