#ifndef XYZ_ERROR_MESSAGE
#define XYZ_ERROR_MESSAGE

// Simple error (signal) management
// by Jakub Grzana

/*******************************************************************/
/*************************** INTERNAL ******************************/
#include <string.h>
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#include <iostream>
#include <string>
#include <vector>
namespace Err 
{
	struct Marker {
		Marker(const std::string& file, const int& line, const std::string& function, const std::string& comment);
		~Marker();
	};
	std::vector<std::string> GetLast(int num);
	std::string GetLastLocations(int num);
	void AddSignalHandler(int signal_id);
}
#define ERR_BREAK "\n"
/*********************** END OF INTERNAL ***************************/
/*******************************************************************/



/*******************************************************************/
/************************ USER INTERFACE ***************************/
// STARTNA(comm) - This macro "marks" location in error handler. "comm" must be string. Use one of those ONCE per block {}
#define START Err::Marker err_mark(__FILENAME__,__LINE__,__func__,"");
#define STARTNA(comm) Err::Marker err_mark(__FILENAME__,__LINE__,__func__,comm);
// RETURN() and RETURNV are used to return values (or V-void) from functions that are started with STARTNA
#define RETURN(x) return x; 
#define RETURNV return; 
// GET_LAST_LOCATIONS returns std::string with describtion of last visited locations - "num" specifies number of locations
#define GET_LAST_LOCATIONS(num) Err::GetLast(num)
// Use tool specified in Err.cpp to handle signals
// signal_id - ID of signal to be handlede)
#define HANDLE_SIGNAL(signal_id) { Err::AddSignalHandler(signal_id); }
// Behaviour for signal handler (0 - write output and continue, 1 - exit, 2 - ask)
#define BEHAVIOUR 2
#define DEFAULT_NUMBER_OF_LOCATIONS 10
/******************** END OF USER INTERFACE ************************/
/*******************************************************************/

#endif
