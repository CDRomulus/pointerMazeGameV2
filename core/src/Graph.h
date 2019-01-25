#pragma once
#include <vector>
#include <iostream>
#include <limits.h>
#include <fstream>
#include <string>
#include <stdexcept>
#include <algorithm>


enum class Bearing
{
	NORTH = 0, EAST, SOUTH, WEST
};
class Node
{
private:
	std::string m_name;
	bool m_treasure = FALSE;
	std::vector<Bearing> m_bearing;//0=north,1=east,2=south,3=west
	std::vector<Node*> m_nodes;	//list of pointers to nodes

	void AssingNodes(Bearing p, const std::string x, std::vector<Node*> &m_map)
	{
		for (unsigned int i = 0; i < m_map.size(); i++)
		{
			if ((*m_map[i]).GetName() == x)
			{
				m_bearing.push_back(p);
				m_nodes.push_back(m_map[i]);
				break;
			}
		}
	}

public:

	Node(std::string name)
	{
		this->m_name = name;
	}
	~Node()
	{
		std::cout << "Node Destructor Called: " << m_name << "\n";
		for (unsigned int x = 0; x < m_nodes.size(); x++)
		{
			m_nodes[x] = nullptr;
		}

	}

	Node(const Node&) = delete;

	const std::vector<Bearing>& GetAvailableBearings()
	{
		return m_bearing;
	}
	Node* GetExit(Bearing x)
	{
		for (unsigned int i = 0; i < m_bearing.size(); i++)
		{
			if (m_bearing[i] == x)
			{
				return m_nodes[i];
			}
		}
		return nullptr;
	}

	std::string GetName()
	{
		return m_name;
	}
	void SetBearings(std::string North_node, std::string East_node, std::string South_node, const std::string West_node, std::vector<Node*> &m_map)
	{

		if (North_node != "*" || North_node != "")
		{
			AssingNodes(Bearing::NORTH, North_node, m_map);
		}
		if (East_node != "*" || East_node != "")
		{
			AssingNodes(Bearing::EAST, East_node, m_map);
		}
		if (South_node != "*" || South_node != "")
		{
			AssingNodes(Bearing::SOUTH, South_node, m_map);
		}
		if (West_node != "*" || West_node != "")
		{
			AssingNodes(Bearing::WEST, West_node, m_map);
		}
	}
	void setTreasure()
	{
		m_treasure = TRUE;
	}
	bool getTreasureBool()
	{
		return m_treasure;
	}
};

class Graph
{
public:
	Graph()
	{
		if (GraphGeneration() != 0)
		{
			throw std::runtime_error("Exit");
		}
		endgameNode = GetNode((CustomSystem::rngIntGen(GetTotalNodes() - 4, GetTotalNodes())) - 1);
	}

	~Graph()
	{
		printf("Graph Destructor Called.\n");
		for (auto nodes : m_map)
		{
			delete nodes;
			nodes = nullptr;
		}
	}
	Graph(const Graph&) = delete;

	int GetTotalNodes()
	{
		return static_cast<int>(m_map.size());
	}
	Node* GetNode(int x)
	{
		return m_map[x];
	}
	Node* GetNode(std::string x)
	{
		for (unsigned int i = 0; i < m_map.size(); i++)
		{
			if ((*m_map[i]).GetName() == x)
			{
				return m_map[i];
			}
		}
		return nullptr;
	}
	int getGenType()
	{
		return genType;
	}
	std::string getFileName()
	{
		if (getGenType() == 1)
		{
			return "RNG";
		}
		else
			return fileName;
	}
	Node* getEndGameNode()
	{
		return endgameNode;
	}
	void setGraphTreasure()
	{
		g_treasure = TRUE;
	}
	bool getTreasureBool()
	{
		return g_treasure;
	}
private:
	bool g_treasure = FALSE;
	std::vector<Node*> m_map;
	Node* endgameNode;

	int genType;
	std::string fileName;

	int GraphGeneration()
	{
		CustomSystem::ClearScreen();

		std::vector<std::string>* txtMap = Parsing();
		if (txtMap != nullptr)
		{
			int rng = 0;
			for (unsigned int i = 0; i < (*txtMap).size(); i = i + 5)
			{
				m_map.push_back(new Node((*txtMap)[i]));
				int rng = CustomSystem::rngIntGen(0, 10);
				if (i >= 6 * 5 && rng >= 7 && getTreasureBool() == FALSE)
				{
					(*m_map[i / 5]).setTreasure();
					setGraphTreasure();
				}
			}

			for (size_t i = 0, x = 0; i < (*txtMap).size(); i = i + 5, x++)
			{
				(*m_map[x]).SetBearings((*txtMap)[i + 1], (*txtMap)[i + 2], (*txtMap)[i + 3], (*txtMap)[i + 4], m_map);
			}

			delete txtMap;
			txtMap = nullptr;
			return 0;
		}
		else
		{
			return 1;
		}
	}

	std::vector<std::string>* Parsing()
	{

		std::string input;
		bool correctInput = false;
		while (!correctInput)
		{
			printf("Please select generation type.\n[R] for RNG.\n[S] for STATIC.\n[Q] to Exit.\n");
			std::cin >> input;
			if (input.size() != 1)
			{
				input = "z";
			}
			if (input == "s" || input == "S")
			{
				genType = 0;
				correctInput = 1;
			}
			else if (input == "r" || input == "R")
			{
				genType = 1;
				correctInput = 1;
			}
			else if (input == "q" || input == "Q")
			{
				throw std::runtime_error("Exit");
			}
			else
			{
				CustomSystem::ClearScreen();
				printf("Incorrect Input.\n");
			}

			CustomSystem::ClearScreen();
			if (genType == 1 && correctInput == 1)
			{
				return randomGeneration();
			}
			else if (genType == 0 && correctInput == 1)
			{

				return staticGeneration();
			}
		}
		return nullptr;
	}
	std::vector<std::string>* staticGeneration()
	{

		bool success = false;


		std::ifstream fileObject;
		std::string input;

		std::vector<std::string>* txtMap = new std::vector<std::string>();
		while (!success)
		{
			printf("Please enter the name of .txt file of map.\n");
			printf("Type 'default' for DEFAULT map.\n");
			printf("Type 'exit' to EXIT.\n");

			std::cin >> input;

			if (input == "exit")
			{
				CustomSystem::ClearScreen();
				throw std::runtime_error("Exit");
			}
			fileObject = std::ifstream(CustomSystem::GetDirectory() + "/" + input + ".txt");

			if (fileObject.is_open())
			{
				std::vector<std::string> results;

				std::string nodeName, n, e, s, w;

				while (fileObject >> nodeName >> n >> e >> s >> w)
				{
					(*txtMap).push_back(nodeName);
					(*txtMap).push_back(n);
					(*txtMap).push_back(e);
					(*txtMap).push_back(s);
					(*txtMap).push_back(w);
				}
				fileObject.close();
				fileName = input;
				success = true;
			}
			else
			{
				CustomSystem::ClearScreen();
				std::cout << "Cannot find file: " << CustomSystem::GetDirectory() + "/" + input + ".txt" << "\n";
			}
		}
		return txtMap;
	}
	std::vector<std::string>* randomGeneration()
	{
		genType = 1;
		int nodeNumber = CustomSystem::rngIntGen(13, 17);
		std::vector<std::string>* txtMap = new std::vector<std::string>(nodeNumber * 5);
		(*txtMap)[0] = "A";
		(*txtMap)[5] = "B";
		(*txtMap)[10] = "C";
		(*txtMap)[15] = "D";
		(*txtMap)[20] = "E";
		(*txtMap)[25] = "F";
		(*txtMap)[30] = "G";
		(*txtMap)[35] = "H";
		(*txtMap)[40] = "I";
		(*txtMap)[45] = "J";
		(*txtMap)[50] = "K";
		(*txtMap)[55] = "L";
		(*txtMap)[60] = "N";

		switch (nodeNumber)
		{
		case 14:
		{
			(*txtMap)[65] = "M";
			break;
		}
		case 15:
		{
			(*txtMap)[65] = "M";
			(*txtMap)[70] = "O";
			break;
		}
		case 16:
		{
			(*txtMap)[65] = "M";
			(*txtMap)[70] = "O";
			(*txtMap)[75] = "P";
			break;
		}
		case 17:
		{
			(*txtMap)[65] = "M";
			(*txtMap)[70] = "O";
			(*txtMap)[75] = "P";
			(*txtMap)[80] = "Q";
			break;
		}
		}
		int rng;
		int currentNode = 0;
		for (unsigned int i = 5; i < (*txtMap).size(); i += 5)
		{
			rng = CustomSystem::rngIntGen(1, 10);
			if (rng >= 5)
			{
				bool check = FALSE;
				for (unsigned int j = 1; j <= 4; j++)
				{
					rng = CustomSystem::rngIntGen(0, 10);
					if (rng >= 7)
					{
						switch (j)
						{
						case 1:
							if ((*txtMap)[i + j] == "" && (*txtMap)[currentNode + 3] == "")
							{
								(*txtMap)[i + j] = (*txtMap)[currentNode];
								(*txtMap)[currentNode + 3] = (*txtMap)[i];
								check = TRUE;
							}
							break;
						case 2:
							if ((*txtMap)[i + j] == "" && (*txtMap)[currentNode + 4] == "")
							{
								(*txtMap)[i + j] = (*txtMap)[currentNode];
								(*txtMap)[currentNode + 4] = (*txtMap)[i];
								check = TRUE;
							}
							break;
						case 3:
							if ((*txtMap)[i + j] == "" && (*txtMap)[currentNode + 1] == "")
							{
								(*txtMap)[i + j] = (*txtMap)[currentNode];
								(*txtMap)[currentNode + 1] = (*txtMap)[i];
								check = TRUE;
							}
							break;
						case 4:
							if ((*txtMap)[i + j] == "" && (*txtMap)[currentNode + 2] == "")
							{
								(*txtMap)[i + j] = (*txtMap)[currentNode];
								(*txtMap)[currentNode + 2] = (*txtMap)[i];
								check = TRUE;
							}
							break;
						}

					}
					if (check == TRUE)
					{
						break;
					}

					if (j == 4 && check == FALSE)
					{
						j = 1;
					}
				}
				currentNode = i;
			}
		}
		for (unsigned int i = 5; i < (*txtMap).size(); i += 5)
		{
			while ((*txtMap)[i + 1] == "" && (*txtMap)[i + 2] == "" && (*txtMap)[i + 3] == "" && (*txtMap)[i + 4] == "")
			{
				int x = CustomSystem::rngIntGen(1, 4);
				//check
				switch (x)
				{
				case 1:

					if ((*txtMap)[i - 5 + 3] == "")
					{
						(*txtMap)[i + x] = (*txtMap)[i - 5];
						(*txtMap)[i - 5 + 3] = (*txtMap)[i];

					}
					break;

				case 2:
					if ((*txtMap)[i - 5 + 4] == "")
					{
						(*txtMap)[i + x] = (*txtMap)[i - 5];
						(*txtMap)[i - 5 + 4] = (*txtMap)[i];
					}
					break;
				case 3:
					if ((*txtMap)[i - 5 + 1] == "")
					{
						(*txtMap)[i + x] = (*txtMap)[i - 5];
						(*txtMap)[i - 5 + 1] = (*txtMap)[i];
					}
					break;
				case 4:
					if ((*txtMap)[i - 5 + 2] == "")
					{
						(*txtMap)[i + x] = (*txtMap)[i - 5];
						(*txtMap)[i - 5 + 2] = (*txtMap)[i];
					}
					break;

				}
			}
		}
		return txtMap;
	}
};
