/*
Student Name : Rey Baltar
Student ID : hrb217
Compiler used : Eclipse using MinGW
Program description:
	This program allows the user to interact with a command line interface so that the user
	can make modifications to an adjacency list and input or output text files
*/



#include <iostream>
#include "AdjacencyList.h"
#include <fstream>
#include <sstream>
using namespace std;

string breakString(string word, string cmd){
//"breaks" the second string by the length of the first string and returns new broken string

	//initialize variables
	string hanger;
	int value1 = word.length()+1; //incremented by 1 to include the space in the second word
	int value2 = cmd.length();

	//begins adding characters after the space after the first word.
	for(int i = value1; i < value2; i++){
		hanger += cmd[i];
	}

	return hanger;
}


string buildString(string word, string cmd){
//simply adds the second string to the first string and returns the new string

	// adds a space between words and increments
	word += " ";
	word += cmd;

	return word;
}

void parse(string& New, string Old, string& LS, string FS){
	//rebuilds the string to a stored variable and breaks the full state_ment so that 'new'
	//gets the remaining text starting with the desired word
	LS = buildString(LS, Old);
	New = breakString(LS, FS);
}

int main() {
// main
	//instances adjacency list based on user choice of type
	AdjacencyList<string> graph;


	//initialize variables
	bool run = true;
	string command, full_statement, object1, prep1, object2, prep2, object3, leftSide, type, dummy;
	double weight;
	ifstream itext;
	ofstream otext;
	type = "string";

	//introduce program
	cout <<"This program allows the user to interact with a command line " << endl;
	cout <<"interface so that the user can make modifications to an AdjacencyList" <<endl;
	cout<< "list without typing any code." << endl;
	cout <<"NOTE: for the 'load' and 'output' command, include the '.txt' after the file name" << endl<< endl;

	//enters parse loop
	while(run || !itext.eof()){

		/*
		 * checks whether or not a file is open so that the program knows
		 * whether to prompt the user for a command line or automatically
		 * parse the next line of the file
		 */
		if(!itext.is_open()){
			cout << "Command Line <" + type + ">: ";
			getline(cin, command);
		}else{
			getline(itext, command);
		}

		//stores the full command statement
		full_statement = command;
		stringstream ss(command);

		// redefines command as the first word in full statement
		getline(ss, command, ' ');

		// stores words that are "broken" from strings
		leftSide = command;

		// assigns object1 to a modified full_statement with 'command' removed
		object1 = breakString(command, full_statement);

		//and define 'object1' as the second word of the full statement and
		getline(ss, object1, ' ');

		//for the following operations the remaining statement from the user will be parsed
		parse(prep1, object1, leftSide, full_statement);
		getline(ss, prep1, ' ');

		parse(object2, prep1, leftSide, full_statement);
		getline(ss, object2, ' ');

		parse(prep2, object2, leftSide, full_statement);
		getline(ss, prep2, ' ');

		parse(object3, prep2, leftSide, full_statement);
		getline(ss, object3);

		//*** NOW WE ENTER THE IF STATEMENTS *** //

		if(command == "exit"){
			//exits the program
			break;


		}else if(command == "display"){
			//displays wither the matrix, vertices, or the full list
			if(object1 == "matrix"){
				graph.matrix(cout);
			}
			else if(object1 == "vertices"){
				graph.rows(cout);
			}else{
				cout << graph;
			}
		}else if(command== "load"){
			//opens a .txt file and obtains its text
			itext.open( object1 );

		}else if(command == "output"){
			//saves the matrix and vertices of the adjacency list into a file.
			otext.open( object1 );
			graph.rows(otext);
			graph.matrix(otext);
			otext.close();

		}else if(command == "remove"){
			//removes either a vertex or edge
			//initial two booleans
			bool edge = false;
			bool pass = true;

			//label 'edge' based on preposition
			if(prep1 == "to"){ edge = true;}
			string initial = object1, final = object2;

			//check if the user wants to remove an edge or a vertex
			if(!edge){
				pass = graph.remove_vertex(initial);// this also removes any edges related to this vertex

			}else if(edge){
				pass = graph.remove_link(initial, final);
			}

			//check if removal passes through and informs the user of pass or fail
			if(!pass){
				cout << " - deletion fail" << endl;
			}else{
				if(!edge){
					cout << object1 + " vertex deleted from list" << endl;
					cout << "all edges removed relating to " + object1<< endl;
				}else{
					cout << "link from " + object1 + " to " + object2 + " removed." << endl;
				}
			}


		}else if(command == "find"){
			//finds the weight from one vertex to another
			bool pass = true;
			string initial = object1, final = object2;

			//check if the vertices exist in the list of vertices
			if(!graph.search(object1) or !graph.search(object2)){
				pass = false;
			}

			//check if find passes and return the weight otherwise inform on failure
			if(!pass){
				cout << " - finding edge fail" << endl;
			}else{
				cout << "Weight found: " << graph.find(initial, final)<< endl;
			}

		}else if(command == "add"){
			//adds a vertex or edge
			//initialize booleans
			bool edge = false;
			bool pass = true;

			//check if there is a preposition to signify an edge insertion
			if(prep1 == "to"){ edge = true;}
			string initial = object1, final = object2;

			//for an edge addition, tests the conversion of weight to a double
			//also defaults the weight to 1 if the prepostion2 is not stated.
			if(edge){
				try{

					if(prep2 == "weight"){weight = stod(object3);}
					else{weight = 1;}
				}catch(...){
					cout << "invalid weight. make sure it is a double (number)";
					pass = false;
				}
			}

			//check if the user wants to add a vertex or if the conversion passes add an edge
			if(!edge and pass){
				pass = graph.insert_vertex(initial);

			}else if(edge and pass){
				pass = graph.insert_link(initial, final, weight);
			}

			//inform the user on pass or fail of the insertion
			if(!pass){
				cout << " - insertion fail" << endl;
			}else{
				if(!edge){cout << object1 + " vertex added to list" << endl;}
				else{cout << object3 + " edge added from " + object1 + " to " + object2 << endl;}
			}

		}else{
			//inform user that the command is not within the program (or awkward space in command line)
			cout << "No command of that nature found (or awkward spaces)" << endl;
		}

		// *** EXIT IF STATEMENTS *** //

		//checks if the test file is being processed and if it has reached its end
		if(itext.is_open() and itext.eof()){
			//closes text file and returns to the user interface command line
			itext.close();
		}

		// ** END OF WHILE lOOP ** //
	}
	cout << "TERMINATED" << endl;

	return 0;
}
