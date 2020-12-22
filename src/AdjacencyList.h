/*
Student Name : Rey Baltar
Student ID : hrb217
Compiler used : Eclipse using MinGW
Program description:
	This program allows the user to interact with a command line interface so that the user
	can make modifications to an adjacency list and input or output text files
*/

#ifndef ADJACENCYLIST_H_
#define ADJACENCYLIST_H_
#include <vector>
using std::vector;
using std::ostream;
using std::endl;
using std::cout;

//these are the edges that a vertex uses to point to another vertex and contain a weight value
template <typename T>
class Link {
public:
	double weight;
	T fake_pointer;
	Link():weight(0), fake_pointer(nullptr){}
	Link(int weight, T host): weight(weight), fake_pointer(host){}
};

//these are the nodes in the adjacency list that contain a list of Links and a name
template <typename T>
class Vertex{
public:
	T name;
	vector <Link<T>> edges;
	Vertex(): edges({}){}
	Vertex(T name): name(name), edges({}){}

};

//this class is a vector of vertices that also contain vectors. (vector of vectors)
template <typename T>
class AdjacencyList {
public:
	vector<Vertex<T>> vertices;

//methods//
	AdjacencyList():vertices({}){}

	//additional methods
	bool search(T point);

	//required methods
	bool insert_vertex(T point);
	bool insert_link(T name, T target, double w);
	bool remove_vertex(T point);
	bool remove_link(T name, T target);
	double find(T init, T final);
	void rows(ostream &os);
	void matrix(ostream &os);

	//inside class methods (I decided I wanted to be extra and template this class)
	friend ostream & operator<<(ostream& os, AdjacencyList&rhs){
		//outputs the contents of the vector of vectors
		os << "Adjacency List:" << endl;
		for(Vertex<T>& node: rhs.vertices){
			os << "Vertex->" << node.name << ": ";
			for(Link<T>& edge: node.edges){
				os<< edge.fake_pointer << ":" << edge.weight << " ";
			}
			os <<endl;
		}
		return os;
	}

};


//class functions

//For my benefit, this determines whether a vertex exists in the vector of vertices
template <typename T>
bool AdjacencyList<T>::search(T point)
{
		//initialize iterator
		typename vector<Vertex<T>>::iterator it = vertices.begin();

		//check if the vertex is in the vector.
		for(; it != vertices.end(); it++){
			if( (*it).name == point){
				return true;
			}
		}
		return false;
	}

//inserts a vertex
template <typename T>
bool AdjacencyList<T>::insert_vertex(T point){
	//this avoids duplicate vertices
	if(AdjacencyList::search(point)){
		return false;
	}

	//adds to the vector
	vertices.push_back(Vertex<T>(point));
	return true;
}

//inserts an edge (or Link) from one vertex to another
template <typename T>
bool AdjacencyList<T>::insert_link(T point, T target, double w){
	//this avoids a vertex to point at itself
	if(point == target){return false;}

	//inserts the link to the initial vertex but only if the final vertex exists
	for(Vertex<T>& node: vertices){
		if(node.name == point and AdjacencyList::search(target)){
			node.edges.push_back(Link<T>(w,target));
			return true;
		}
	}
	return false; //false if not found
}

//removes a vertex AND any edges that are related to it
template <typename T>
bool AdjacencyList<T>::remove_vertex(T point){
	//initialize iterator
	typename vector<Vertex<T>>::iterator it;

	//Traverses the vector of vertices to see if the vertex exist
	for(it = vertices.begin(); it != vertices.end(); ++it){
		if( (*it).name == point){

			//finds all edges related to the vertex and removes them
			for(Vertex<T>& node: vertices){
				AdjacencyList::remove_link(node.name, point);
			}
			vertices.erase(it); //deletes the vertex

			return true;
		}
	}
	return false;
}

//deletes an edge from one vertex to another
template <typename T>
bool AdjacencyList<T>::remove_link(T point, T target){
	//traverses for the initial vertex
	for(Vertex<T>& node: vertices){
		if(node.name == point){

			//initialize iterator and vector
			vector<Link<T>>& row = node.edges;
			typename vector<Link<T>>::iterator it;

			//deletes the edge connecting from point to target
			for(it = row.begin(); it != row.end(); ++it){
				if( (*it).fake_pointer == target){
					row.erase(it);
					return true;
				}
			}
		}
	}
	return false;
}

//finds an edge and returns its weight
template <typename T>
double AdjacencyList<T>::find(T init, T final){
	//traverses through the list of vertices to see if init exist
	for(Vertex<T>& node: vertices)
	{
		if(node.name == init)
		{
			//Traverse through the list of edges and return its weight if final exists
			vector<Link<T>> row = node.edges;
			for(Link<T>& edge: row)
			{
				if(edge.fake_pointer == final){return edge.weight;}
			}
		}
	}
	return 0;
}

//displays all of the vertices in the adjacency list
template <typename T>
void AdjacencyList<T>::rows(ostream & os){
	os << "Vertices: ";
	for(Vertex<T>& node: vertices){
		os << node.name << " ";
	}
	os << endl;
}

//displays the matrix of the adjacency list as an nxn matrix
template <typename T>
void AdjacencyList<T>::matrix(ostream & os){
	//for each vertex (y), there is a set of values
	for(Vertex<T> row: vertices){
		os<< "| ";

		//for each vertex (x) now in the x direction, check if the y vertex is linked to the x vertex
		for(Vertex<T>& node: vertices){

			//to check if y is linked to x, check if any of the links of y 'points' to the name of x
			typename vector<Link<T>>::iterator it;
			for(it = row.edges.begin(); it != row.edges.end(); ++it){//(Link<T>& edge: node.edges){

				//if the link and vertex x match then break the loop
				if( (*it).fake_pointer == node.name){
					os<< (*it).weight << " ";
					break;
				}
			}
			//if there are no links in vertex x that point to y then output a weight of 0
			if(it == row.edges.end()){
				os<< 0 << " ";
			}
		}
		os << "|" << endl;//end of vertex y index now repeat
	}
}

#endif /* ADJACENCYLIST_H_ */
