#include <iostream>
#include <algorithm>
#include <iterator>
#include <memory>
#include <string>
#include <vector>
#include <stdio.h>
#include <sqlite3.h>
using namespace std;

/* These files are called in case the database needed creation in the first place */

void createConnection(const char* filepath) 
//also creates database if it doesn't exist
{
	sqlite3* DB;
	sqlite3_open(filepath, &DB);
	sqlite3_close(DB);
    //open and close database after creation
    
    //code is based off https://www.youtube.com/watch?v=L-hnA82JsEM&t=189s
}

int createTable(const char* s)
{
	sqlite3* DB;

    //Time is the primary key
	string sql = "CREATE TABLE IF NOT EXISTS classes("
		         "time TIME PRIMARY KEY, "
		         "monday VARCHAR(10),"
		         "tuesday, VARCHAR(10),"
		         "wednesday VARCHAR(10),"
		         "thursday VARCHAR(10)"
		         "friday VARCHAR(10));";
        //code is based on https://www.youtube.com/watch?v=L-hnA82JsEM&t=189s

	
	try
	{
		int exit = 0;
		exit = sqlite3_open(s, &DB);

		char* messageError;
		exit = sqlite3_exec(DB, sql.c_str(), NULL, 0, &messageError);

		if (exit != SQLITE_OK)
		{
			cerr << "Error Create Table" << endl;
			sqlite3_free(messageError);
		}

		else
		{
			cout << "Table Created Successfully" << endl;
			sqlite3_close(DB);
		}
		
	}
	catch (exception&e)
	{
		cerr << e.what() << endl ;
	}

	delete DB ;
	return 0 ;
        //code is based on https://www.youtube.com/watch?v=L-hnA82JsEM&t=189s

}







ifndef RETRIEVE_HPP
#define RETRIEVE_HPP
#pragma once

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <memory>
#include <string>
#include <vector>
#include <stdio.h>
#include <sqlite3.h>
using namespace std;

/* Functions in this file will be used to select, store and return information from the database */

int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	/* int argc: holds the number of results 
	 * (array) azColName: holds each column returned 
	 * (array) argv: holds each value */
	vector<string> a ;
	
    for(int i = 0; i < argc; i++) 
	{
        //name: value
        a.push_back(argv[i]);
    }
	
	ofstream myfile;
    
    //open file
	myfile.open ("ids.txt");
	
	for(auto it = begin(a) ; it != end(a) ; it = next(it))
	{
		myfile << (*it) << endl;
	}
	myfile << "#" << endl;
	
	myfile.close();
	
	return 0 ;
}

vector< vector<int> > collectData(const char* filename, vector<string> tables)
//this function should return a list of IDs
{
	char *zErrMsg = 0;
	sqlite3 *db;
	sqlite3_open(filename, &db);
	string sql ;
	
	for(auto it = begin(tables) ; it != end(tables) ; it = next(it))
	{
		sql = "SELECT ID FROM '" + *it + "'" ;
		sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
	}
	
	vector< vector<int> > IDS ;
	vector<int> meow ;
	string line;
	ifstream myfile("ids.txt");
	if(myfile.is_open())
	{
		while(myfile.good())
		{
			getline(myfile,line) ;
			if(line != "#")
			{
				meow.push_back(stoi(line)) ;
			}
			else {
				IDS.push_back(meow) ;
				meow.clear() ;
			}
		}
		
		myfile.close();
	}
	sqlite3_close(db);
	
	delete db ;
	return IDS ;
}

































#include "createDatabase.hpp" //used to establish connection (and create database and tables if it didn't exist)
#include "retrieveDatabase.hpp" //used to select information
#include <iostream>
#include <algorithm>
#include <iterator>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <stdio.h>
#include <sqlite3.h>
using namespace std;

/* This source file will be used to call all functions */

int main()
{
    const char* filepath = "./timetable.db" ;
	sqlite3* DB ;
	
	createConnection(filepath); //establishes/tests connection
// 	createTable(dir); // only needed to be used one time
	
	auto data = collectData(filepath, {"Buildings", "Classes", "Courses", "Faculties", "Modules", "Rooms"}) ;
	vector<int> BuildingIDs = data[0] ;
	vector<int> ClassIDs = data[1] ;
	vector<int> CourseIDs = data[2] ;
	vector<int> FacultyIDs = data[3] ;
	vector<int> ModuleIDs = data[4] ;
	vector<int> RoomIDs = data[5] ;
	
	typedef multimap<int, int> timeRange ; //start time first
	typedef multimap<int, timeRange> roomandtime ; //room ID first
	typedef multimap<int, roomandtime> Times ; //class ID first
	Times times ;
	
	//fill in data here
	for(auto it = begin(ClassIDs) ; it != end(ClassIDs) ; it = next(it))
	{
		for(auto fir = times.begin(); fir != times.end() ; ++fir) 
		{
			for(auto dir = (fir->second).begin(); dir != (fir->second).end() ; ++dir) 
			{
				for(auto meow = begin(RoomIDs) ; meow != end(RoomIDs) ; meow = next(meow))
				{
					//if room ID not in map
					if ((fir->second).find(*meow) == (fir->second).end())
					{
						multimap<int, int> stuff = {9,11} ;
						roomandtime otherStuff = {(*meow), stuff} ;
						times.insert(*it, otherStuff) ;
					}
				}
			}
		}
	}
		
		
	if(!times.empty())
	{
		cout << "Data displayed below: " << endl ;
		for(auto it = times.begin(); it != times.end() ; ++it) 
		{
			//class ID = it->first
			for(auto tir = (it->second).begin(); tir != (it->second).end() ; ++tir) 
			{
				//room ID = tir->first
				for(auto fir = (tir->second).begin(); fir != (tir->second).end() ; ++fir) 
				{
					//time start = fir->first
					//time end = fir->second
                    //output to user
					cout << "- Class #" << it->first << " has a class in Room #" << tir->first << endl ;
					cout << "This session will commence at " << fir->first << " and will end at " << fir->second << endl ;
				}
			}
		}
	}
	//
	delete DB ;
	return 0;
}
