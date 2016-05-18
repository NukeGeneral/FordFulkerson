#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <istream>
#include <sstream>
#include <queue>

class Graph
{
private:
public:
	int robots;
	int objects;
	int vertexCount;
	int **adjMatrix;
	Graph();
	void Adjustment();
};

Graph::Graph() {}

void Graph::Adjustment() //EXPAND GRAPH TO DESIRED SIZE
{
	int i = 0;
	int j = 0;
	vertexCount = robots + objects + 2;
	adjMatrix = new int*[vertexCount];
	for (i = 0; i < vertexCount; i++)
	{
		adjMatrix[i] = new int[vertexCount];
		for (j = 0; j < vertexCount; j++)
		{
			adjMatrix[i][j] = 0;
		}
	}

	for (i = 1; i <= objects; i++) //FROM START NODE S TO OBJECTS
	{
		adjMatrix[0][i] = 1;
	}

	for (i = objects + 1; i <= robots + objects; i++) //FROM ROBOTS TO END NODE T
	{
		adjMatrix[i][vertexCount - 1] = 1;
	}
}

class MaxFlow : public Graph
{
private:
public:
	int BFS(int s, int t, int **resGraph,int previous[]);
	void MaxBiPartiteMatch(Graph &maxBPTGraph,int start,int target);
};

//BREADTH FIRST SEARCH FOR PATHFINDING
int MaxFlow::BFS(int s,int t,int **resGraph,int previous[]) 
{
	std::queue<int> queue;
	std::list<int> path;
	int *visited = new int[t+1];
	for (int i = 0; i <= t; i++)
	{
		visited[i] = 0;
		previous[i] = -1;
	}
	queue.push(s);
	visited[s] = 1;
	while (!queue.empty())
	{
		int current = queue.front();
		queue.pop();
		for (int i = 0; i <= t; i++)
		{
			if (resGraph[current][i] >0  && visited[i] == 0)
			{
				queue.push(i);
				previous[i] = current;
				visited[i] = 1;
			}
		}
	}
	//PATH PRINTING
	/*int temp = t;
	path.push_front(t);
	while (previous[temp] != -1)
	{
		path.push_front(previous[temp]);
		temp = path.front();
	}

	for (std::list<int>::iterator it = path.begin(); it != path.end(); ++it)
	{
		std::cout << " " << *it;
	}*/
	return (visited[t] == 1);
}

//FORD-FULKERSON ALGORITHM TO FIND MAX FLOW AND ASSIGNMENTS
void MaxFlow::MaxBiPartiteMatch(Graph &maxBPTGraph,int start,int target)
{
	int i, j;
	int path_cost = 1; //SINCE WE KNOW THAT COST BETWEEN ROBOT AND OBJECT WILL BE 1 
	int **resGraph = new int*[target+1];
	int *previous = new int[target+1];
	for (i = 0; i <= target; i++)
	{
		resGraph[i] = new int[target+1];
		for (j = 0; j <= target; j++)
		{
			resGraph[i][j] = maxBPTGraph.adjMatrix[i][j];
		}
	}
	while (BFS(start, target, resGraph,previous))
	{
		for (j = target; j != start; j = previous[j])
		{
			i = previous[j];
			resGraph[i][j] -= path_cost;
			resGraph[j][i] += path_cost;
		}
	}
	//MATRIX PRINT
	/*for (int x = 0; x < 16; x++)
	{
		std::cout << "\n";
		for (int y = 0; y < 16; y++)
		{
			std::cout << resGraph[x][y];
		}
	}
	std::cout << "\n";
	for (int x = 0; x < 16; x++)
	{
		std::cout << "\n";
		for (int y = 0; y < 16; y++)
		{
			std::cout << maxBPTGraph.adjMatrix[x][y];
		}
	}*/
	//ASSIGNMENTS
	std::list<int> assignments;
	for (int x = maxBPTGraph.objects+1; x <= target; x++)
	{
		for (int y = 1; y <= maxBPTGraph.objects; y++)
		{
			if (resGraph[x][y] == 1)
			{
				assignments.push_front(x-10);
				assignments.push_front(y);
			}
		}
	}
	int counter = 0;
	for (std::list<int>::iterator it = assignments.begin(); it != assignments.end(); ++it)
	{
		if (counter == 2)
		{
			std::cout << "\n";
			counter = 0;
		}
		std::cout << " " << *it;
		counter++;
	}
	std::cout << "\n";
}

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Need only one parameter to run" << std::endl;
		return 0;
	}

	std::ifstream inputData(argv[1]);
	if (!inputData.good())
	{
		std::cout << "Error while opening input data" << std::endl;
	}
	std::string lastline;
	int counter = 0;
	Graph maxBPTGraph;
	MaxFlow maxBPTCalculation;
	int i = 0;
	while (std::getline(inputData, lastline))
		if (counter == 0)
		{
			std::stringstream ss(lastline);
			int temp[2] = { 0,0 }; //Since we know that we'll get input in first line as objects and robots
			while (ss)
			{
				ss >> temp[i];
				if (!ss)
				{
					break;
				}
				i++;
			}
			maxBPTGraph.objects = temp[0];
			maxBPTGraph.robots = temp[1];
			maxBPTGraph.Adjustment();
			counter++;
		}
		else if (counter > 0 && counter <= maxBPTGraph.objects)
		{
			std::stringstream ss(lastline);
			for (i = 0; i < maxBPTGraph.robots; i++)
			{
				ss >> maxBPTGraph.adjMatrix[counter][maxBPTGraph.objects + i + 1];
			}
			counter++;
		}

		else
		{
			std::stringstream ss(lastline);
			for (i = 1; i <= maxBPTGraph.robots; i++)
			{
				ss >> maxBPTGraph.adjMatrix[maxBPTGraph.objects + i][maxBPTGraph.objects + maxBPTGraph.robots + 1];
			}
			counter++;
		}
		maxBPTGraph.vertexCount = maxBPTGraph.objects + maxBPTGraph.robots + 2;
		std::cout << "Assignments\n";
		maxBPTCalculation.MaxBiPartiteMatch(maxBPTGraph,0,maxBPTGraph.vertexCount-1);
		return 0;
}