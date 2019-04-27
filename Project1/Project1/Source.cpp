#include<iostream> 
#include<fstream> 
#include<vector>
#include<set>
#include<tuple>
#include<string>
#include<map>
#include <memory>



using namespace std;


class BaseGraph 
{
public:
	virtual void readGraph(string fileName) {} //()=0; чисто вистуальный метод, теперь класс абстрактный, но нельзя применить к этому методу тк он не переопределен в (AdjMatrix, AdjList...))
	virtual void writeGraph(string fileName) = 0;
	virtual void addEdge(int from, int to, int weight) {}
	virtual void removeEdge(int from, int to) {} 
	virtual int changeEdge(int from, int to, int newWeight) { return 1; };
	virtual void transformToAdjList() {}
	virtual void transformToAdjMatrix() {}
	virtual void transformToListOfEdges() {}
};

class AdjMatrix : public BaseGraph
{
public:
	vector <vector <int> > matrix;
	int N, D, W;

	AdjMatrix(ifstream &file)
	{
		file >> N >> D >> W;
		matrix.resize(N, vector<int>(N));

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				file >> matrix[i][j];
			}
		}

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				cout << matrix[i][j] << " ";
			}
			cout << endl;
		}
	}

	AdjMatrix(int n, int d, int w, vector <set <int> > l)
	{
		N = n; D = d; W = w;
		matrix.resize(N, vector<int>(N));

		int i = 0;
		int j = 0;
		for (set <int> values : l)
		{
			for (int value : values)
			{
				matrix[i][value - 1] = 1;
			}
			++i;
		}
	}

	AdjMatrix(int n, int d, int w, vector <map <int, int> > l)
	{
		N = n; D = d; W = w;
		matrix.resize(N, vector<int>(N));

		int i = 0;
		for (auto maps : l)
		{
			for (auto map : maps)
			{
				int j = map.first;
				int value = map.second;
				matrix[i][--j] = value;
			}
			i++;
		}
	}

	AdjMatrix(int n, int d, int w, set <pair<int, int>> listOfEdges)
	{
		N = n; D = d; W = w;
		matrix.resize(N, vector<int>(N));

		for (auto value : listOfEdges)
		{
			int i = value.first;
			int j = value.second;
			matrix[--i][--j] = 1;
		}
	}

	AdjMatrix(int n, int d, int w, set <tuple<int, int, int>> listOfEdgesWihtWay)
	{
		N = n; D = d; W = w;
		matrix.resize(N, vector<int>(N));

		for (auto value : listOfEdgesWihtWay)
		{
			int i = get<0>(value);
			int j = get<1>(value);
			int way = get<2>(value);
			matrix[--i][--j] = way;
		}
	}

	void writeGraph(string fileName)
	{
		ofstream file;
		file.open(fileName, ios::out);

		file << 'C' << ' ' << N << endl << D << ' ' << W << endl;

		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
				file << matrix[i][j] << ' ';
			file << endl;
		}
	}

	void addEdge(int from, int to, int weight)
	{
		// проверку на W не добавляю, тк если w==0 то weight уже равен 1
		if (D == 0)
		{
			matrix[--from][--to] = weight;
			matrix[to][from] = weight;
		}
		else if (D == 1)
		{
			matrix[--from][--to] = weight;
		}
	}

	void removeEdge(int from, int to)
	{
		if (D == 0)
		{
			matrix[--from][--to] = 0;
			matrix[to][from] = 0;
		}
		else if (D == 1)
		{
			matrix[--from][--to] = 0;
		}
	}

	int changeEdge(int from, int to, int newWeight) 
	{
		int oldValue;
		if (D == 0)
		{
			oldValue = matrix[--from][--to];
			matrix[from][to] = newWeight;
			matrix[to][from] = newWeight;
		}
		else if (D == 1)
		{
			oldValue = matrix[--from][--to];
			matrix[from][to] = newWeight;
		}
		return oldValue;
	}

};

class AdjLists : public BaseGraph
{
public:
	vector <set <int> > list;
	vector <map <int, int> > listWithWay;
	int N, D, W;

	AdjLists(ifstream &file)
	{
		file >> N >> D >> W;

		if (W == 0) //невзвешенный
		{
			list.resize(N);
			string str = "";
			getline(file, str);//первая стр считывается пустой почему-то
			int k = 0;
			while (getline(file, str))
			{
				cout << str << endl;
				set <int>insideList;

				for (unsigned int i = 0; i < str.length(); i++)
				{
					if (str[i] >= '0' && str[i] <= '9')
						insideList.insert(str[i] - '0');
				}
				list[k] = insideList;
				k++;
			}
		}
		else //взвешенный
		{
			listWithWay.resize(N);
			string str = "";
			getline(file, str);//первая стр считывается пустой почему-то
			int k = 0;
			while (getline(file, str))
			{
				cout << str << endl;

				for (unsigned int i = 0; i < str.length(); i++)
				{
					if (str[i] >= '0' && str[i] <= '9')
					{
						string num = "";
						int top = str[i] - '0';
						for (int j = i + 2; str[j] != ' ' && str[j] != '\0'; j++, i = j)
						{
							num += str[j];
						}
						//map<int, int> value;
						//value[top] = atoi(num.c_str());
						listWithWay[k].insert(pair<int, int>(top, atoi(num.c_str())));
					}
				}
				k++;
			}
		}
	}

	AdjLists(int n, int d, int w, vector <vector <int> > matrix)
	{
		N = n;
		D = d;
		W = w;

		if (W == 0)
		{
			list.resize(N);
			for (int i = 0; i < N; i++)
			{
				set <int>insideList;
				for (int j = 0; j < N; j++)
				{
					if (matrix[i][j] == 1)
						insideList.insert(j + 1);
				}
				list[i] = insideList;
			}
		}
		else if (W == 1)
		{
			listWithWay.resize(N);
			for (int i = 0; i < N; i++)
			{
				for (int j = 0; j < N; j++)
				{
					if (matrix[i][j] != 0)
					{
						int param1 = j + 1;
						int param2 = matrix[i][j];
						listWithWay[i].insert(pair<int, int>(param1, param2));
					}
				}
			}
		}
	}

	AdjLists(int n, int d, int w, set <pair<int, int>> listOfEdges)
	{
		N = n;
		D = d;
		W = w;

		list.resize(N);

		for (auto value : listOfEdges)
		{
			int i = value.first;
			int j = value.second;
			list[--i].insert(j);
		}
	}

	AdjLists(int n, int d, int w, set <tuple<int, int, int>> listOfEdgesWihtWay)
	{
		N = n;
		D = d;
		W = w;

		listWithWay.resize(N);

		for (auto value : listOfEdgesWihtWay)
		{
			int i = get<0>(value);
			int j = get<1>(value);
			int way = get<2>(value);

			listWithWay[--i].insert(pair<int, int>(j, way));
		}
	}

	void writeGraph(string fileName)
	{
		ofstream file;
		file.open(fileName, ios::out);

		file << 'L' << ' ' << N << endl << D << ' ' << W << endl;

		if (W == 0)
		{
			for (int i = 0; i < N; i++)
			{
				for (int value : list[i])
				{
					file << value << ' ';
				}
				file << endl;
			}
		}
		else if (W == 1)
		{
			for (int i = 0; i < N; i++)
			{
				for (auto it: listWithWay[i])
				{
					file << (&it)->first << " " << (&it)->second << ' ';
				}
				file << endl;
			}
		}
	}

	void addEdge(int from, int to, int weight)
	{
		if (D == 1)
		{
			if (W == 0)
			{
				(list[--from]).insert(to);
			}
			else if (W == 1)
			{
				//map<int, int>value;
				//value[to] = weight;
				(listWithWay[--from]).insert(pair<int, int>(to, weight));
			}
		}
		else if (D == 0)
		{
			if (W == 0)
			{
				(list[--from]).insert(to);
				(list[--to]).insert(from);
			}
			else if (W == 1)
			{
				(listWithWay[--from]).insert(pair<int, int>(to, weight));
				(listWithWay[--to]).insert(pair<int, int>(from, weight));
			}
		}
	}

	void removeEdge(int from, int to)
	{
		if (D == 1)
		{
			--from;
			if (W == 0)
			{
				auto i = list[from].find(to);
				if(i!= list[from].end())
					(list[from]).erase(to);
			}
			else if (W == 1)
			{
				auto forDel = listWithWay[from].find(to);//!!! Поиск map по ключу
				auto i = listWithWay[from].find(to);
				if (i != listWithWay[from].end())
					listWithWay[from].erase(forDel);
			}
		}
		else if (D == 0)
		{
			if (W == 0)
			{
				auto i = list[--from].find(to);
				if (i != list[from].end())
				{
					(list[from]).erase(to);

					//auto i = list[--to].find(++from);
					//if (i != list[to].end())
					(list[--to]).erase(++from);
				}
			}
			else if (W == 1)
			{
				auto forDel = listWithWay[--from].find(to);//!!! Поиск map по ключу
				auto i = listWithWay[from].find(to);
				if (i != listWithWay[from].end())
				{
					listWithWay[from].erase(forDel);
					auto forDel2 = listWithWay[--to].find(++from);
					listWithWay[from].erase(forDel);
					listWithWay[to].erase(forDel2);
				}
			}
		}
	}

	int changeEdge(int from, int to, int newWeight) // Мы же не можем его вызвать при невзвешенном графе!!!???
	{
		if (W == 0)
			throw ("Граф невзвешенный!");
		else if (W == 1)
		{
			if (D == 0)
			{
				map<int, int>::iterator forChange1 = listWithWay[--from].find(to);
				if (forChange1 != listWithWay[from].end())
				{
					auto it = forChange1->second;
					forChange1->second = newWeight;
					map<int, int>::iterator forChange2 = listWithWay[--to].find(++from);
					forChange2->second = newWeight;
					//listWithWay[from].erase(forDel); 
					//auto it = forDel->second; Здесь работать не будет ибо erase() стерает данные из it
					return it;
				}
			}
			else if (D == 1)
			{
				map<int, int>::iterator forChange1 = listWithWay[--from].find(to);
				if (forChange1 != listWithWay[from].end())
				{
					auto it = forChange1->second;
					forChange1->second = newWeight;
					return it;
				}
			}
			return -1;
		}
	}
};

class ListOfEdges : public BaseGraph
{
public:

	set <pair<int, int>> listOfEdges;
	set <tuple<int, int, int>> listOfEdgesWihtWay;
	int N, M, D, W;

	ListOfEdges(ifstream &file)
	{
		file >> N >> M >> D >> W;

		if (W == 0)
		{
			while (!file.eof())
			{
				int top1, top2;
				file >> top1 >> top2;
				listOfEdges.insert(make_pair(top1, top2));
			}
		}
		else
		{
			while (!file.eof())
			{
				int top1, top2, way;
				file >> top1 >> top2 >> way;
				listOfEdgesWihtWay.insert(make_tuple(top1, top2, way));
			}
		}
	}

	ListOfEdges(int n, int d, int w, vector <vector <int> > matrix)
	{
		N = n;
		D = d;
		W = w;

		if (W == 0)
		{
			for (int i = 0; i < N; i++)
			{
				for (int j = 0; j < N; j++)
				{
					if (matrix[i][j] == 1)
					{
						listOfEdges.insert(pair<int, int>(i + 1, j + 1));
					}
				}
			}
			if (D == 0)
				M = (listOfEdges.size()) / 2;
			else if (D == 1)
				M = (listOfEdges.size());
		}

		else if (W == 1)
		{
			for (int i = 0; i < N; i++)
			{
				for (int j = 0; j < N; j++)
				{
					if (matrix[i][j] != 0)
					{
						listOfEdgesWihtWay.insert(tuple<int, int, int>(i + 1, j + 1, matrix[i][j]));
					}
				}
			}
			if (D == 0)
				M = (listOfEdgesWihtWay.size()) / 2;
			else if (D == 1)
				M = (listOfEdgesWihtWay.size());
		}
	}

	ListOfEdges(int n, int d, int w, vector <set <int> > list)
	{
		N = n;
		D = d;
		W = w;

		for (int i = 0; i < N; i++)
		{
			for (int value : list[i])
			{
				listOfEdges.insert(make_pair(i + 1, value));
			}
		}

		if (D == 0)
			M = (listOfEdges.size()) / 2;
		else if (D == 1)
			M = (listOfEdges.size());
	}

	ListOfEdges(int n, int d, int w, vector <map <int, int> > listWithWay)
	{
		N = n;
		D = d;
		W = w;

		for (int i = 0; i < N; i++)
		{
			for (auto value : listWithWay[i])
			{
				int param2 = value.first;
				int param3 = value.second;
				listOfEdgesWihtWay.insert(tuple<int, int, int>(i + 1, param2, param3));
			}
		}
		
		if (D == 0)
			M = (listOfEdgesWihtWay.size()) / 2;
		else if (D == 1)
			M = (listOfEdgesWihtWay.size());
	}

	void writeGraph(string fileName)
	{
		ofstream file;
		file.open(fileName, ios::out);

		file << 'E' << ' ' << N << ' ' << M << endl << D << ' ' << W << endl;

		if (W == 0)
		{
			for (pair<int, int> value : listOfEdges)
				file << value.first << ' ' << value.second << endl;
		}
		else if (W == 1)
		{
			for (tuple<int, int, int> value : listOfEdgesWihtWay)
				file << get<0>(value) << ' ' << get<1>(value) << ' ' << get<2>(value) << endl; //обращение к элементам кортежа
		}
	}

	void addEdge(int from, int to, int weight)
	{
		if (D == 0)
		{
			if (W == 0)
			{
				listOfEdges.insert(make_pair(from, to));
			}
			else if (W == 1)
			{
				listOfEdgesWihtWay.insert(make_tuple(from, to, weight));
			}
		}
		else if (D == 1)
		{
			if (W == 0)
			{
				listOfEdges.insert(make_pair(from, to));
				listOfEdges.insert(make_pair(to, from));
			}
			else if (W == 1)
			{
				listOfEdgesWihtWay.insert(make_tuple(from, to, weight));
				listOfEdgesWihtWay.insert(make_tuple(to, from, weight));
			}
		}
	}

	void removeEdge(int from, int to)
	{
		if (D == 0)
		{
			if (W == 0)
			{
				pair<int, int>forDelete = make_pair(from, to);

				pair<int, int> forDelete1;
				pair<int, int> forDelete2;
				for (pair<int, int> value : listOfEdges)
				{
					if (value.first == from && value.second == to)
					{
						forDelete1 = value;
						continue;
					}
					if (value.first == to && value.second == from)
					{
						forDelete2 = value;
						continue;
					}
				}
				listOfEdges.erase(forDelete1);
				listOfEdges.erase(forDelete2);
			}
			else if (W == 1)
			{
				tuple<int, int, int> forDelete1;
				tuple<int, int, int> forDelete2;
				for (tuple<int, int, int> value : listOfEdgesWihtWay)
				{
					if (get<0>(value) == from && get<1>(value) == to)
					{
						forDelete1 = value;
						continue;
					}
					if (get<0>(value) == to && get<1>(value) == from)
					{
						forDelete2 = value;
						continue;
					}
				}
				listOfEdgesWihtWay.erase(forDelete1);
				listOfEdgesWihtWay.erase(forDelete2);
			}
		}
		else if (D == 1)
		{
			if (W == 0)
			{
				pair<int, int> forDelete;
				for (pair<int, int> value : listOfEdges)
				{
					if (value.first == from && value.second == to)
					{
						forDelete = value;
						break;
					}
				}
				listOfEdges.erase(forDelete);
			}
			else if (W == 1)
			{
				tuple<int, int, int> forDelete;
				for (tuple<int, int, int> value : listOfEdgesWihtWay)
				{
					if (get<0>(value) == from && get<1>(value) == to)
					{
						forDelete = value;
						break;
					}
				}
				listOfEdgesWihtWay.erase(forDelete);
			}
		}
	}

	int changeEdge(int from, int to, int newWeight)
	{
		if (W == 0)
			throw ("Граф невзвешенный!");
		else if (W == 1)
		{
			if (D == 0)
			{
				int oldWeigth;
				for (tuple<int, int, int> value : listOfEdgesWihtWay)
				{
					if (get<0>(value) == from && get<1>(value) == to)
					{
						oldWeigth = get<2>(value);
						get<2>(value) = newWeight;
						continue;
					}
					if (get<0>(value) == to && get<1>(value) == from)
					{
						get<2>(value) = newWeight;
						continue;
					}
				}
				return oldWeigth;
			}
			else if (D == 1)
			{
				int oldWeigth;
				for (tuple<int, int, int> value : listOfEdgesWihtWay)
				{
					if (get<0>(value) == from && get<1>(value) == to)
					{
						oldWeigth = get<2>(value);
						/*get<2>(value) = newWeight;*/
						auto newValue = make_tuple(get<0>(value), get<1>(value), newWeight);
						listOfEdgesWihtWay.erase(value);
						listOfEdgesWihtWay.insert(newValue);
						//????????????? ну это же несовсем верно
						break;
					}
				}
				return oldWeigth;
			}
		}
	}
};

class Graph : public BaseGraph
{
	//auto ptr = std::make_shared<BaseGraph>();
	//auto_ptr<BaseGraph>ptr(new BaseGraph); 
	//не работает, тк используем new
	//shared_ptr<BaseGraph> gptr;
protected:
	BaseGraph *gptr;
	char type;
public:
	void readGraph(string fileName)
	{
		ifstream file;
		file.open(fileName, ios::in);

		file >> type;
		cout << type << endl;
		if (type == 'C')
		{
			AdjMatrix *m = new AdjMatrix(file);
			//auto_ptr<AdjMatrix> m(new AdjMatrix(file));
			//gptr = (AdjMatrix*)&m;
			gptr = m;
		}
		else if (type == 'L')
		{
			AdjLists *l = new AdjLists(file);
			gptr = l;
		}
		else if (type == 'E')
		{
			ListOfEdges *l2 = new ListOfEdges(file);
			gptr = l2;
		}
	}
	
	void writeGraph(string fileName)
	{
		gptr->writeGraph(fileName);
	}

	void addEdge(int from, int to, int weight=1)
	{
		(*gptr).addEdge(from, to, weight);
	}

	void removeEdge(int from, int to)
	{
		gptr->removeEdge(from, to);
	}

	int changeEdge(int from, int to, int newWeigth=1)
	{
		return gptr->changeEdge(from, to, newWeigth);
	}

	void transformToAdjMatrix() 
	{
		if(type=='L')
			if(dynamic_cast<AdjLists*>(gptr)->W==0)
				gptr = new AdjMatrix(dynamic_cast<AdjLists*>(gptr)->N, dynamic_cast<AdjLists*>(gptr)->D, 
					dynamic_cast<AdjLists*>(gptr)->W, dynamic_cast<AdjLists*>(gptr)->list);
			else
				gptr = new AdjMatrix(dynamic_cast<AdjLists*>(gptr)->N, dynamic_cast<AdjLists*>(gptr)->D,
					dynamic_cast<AdjLists*>(gptr)->W, dynamic_cast<AdjLists*>(gptr)->listWithWay);
		else if (type == 'E')
		{
			if(dynamic_cast<ListOfEdges*>(gptr)->W ==0)
				gptr = new AdjMatrix(dynamic_cast<ListOfEdges*>(gptr)->N, dynamic_cast<ListOfEdges*>(gptr)->D,
					dynamic_cast<ListOfEdges*>(gptr)->W, dynamic_cast<ListOfEdges*>(gptr)->listOfEdges);
			else
				gptr = new AdjMatrix(dynamic_cast<ListOfEdges*>(gptr)->N, dynamic_cast<ListOfEdges*>(gptr)->D,
					dynamic_cast<ListOfEdges*>(gptr)->W, dynamic_cast<ListOfEdges*>(gptr)->listOfEdgesWihtWay);
		}
		else
		{
			gptr = gptr;
		}	
	}

	void transformToAdjList() 
	{
		if (type == 'C')
		{
			gptr = new AdjLists(dynamic_cast<AdjMatrix*>(gptr)->N, dynamic_cast<AdjMatrix*>(gptr)->D,
				dynamic_cast<AdjMatrix*>(gptr)->W, dynamic_cast<AdjMatrix*>(gptr)->matrix);
		}
		else if (type == 'E')
		{
			if (dynamic_cast<ListOfEdges*>(gptr)->W == 0)
				gptr = new AdjLists(dynamic_cast<ListOfEdges*>(gptr)->N, dynamic_cast<ListOfEdges*>(gptr)->D,
					dynamic_cast<ListOfEdges*>(gptr)->W, dynamic_cast<ListOfEdges*>(gptr)->listOfEdges);
			else
				gptr = new AdjLists(dynamic_cast<ListOfEdges*>(gptr)->N, dynamic_cast<ListOfEdges*>(gptr)->D,
					dynamic_cast<ListOfEdges*>(gptr)->W, dynamic_cast<ListOfEdges*>(gptr)->listOfEdgesWihtWay);
		}
		else
		{
			gptr = gptr;
		}
	}

	void transformToListOfEdges()
	{
		if (type == 'C')
		{
			gptr = new ListOfEdges(dynamic_cast<AdjMatrix*>(gptr)->N, dynamic_cast<AdjMatrix*>(gptr)->D,
				dynamic_cast<AdjMatrix*>(gptr)->W, dynamic_cast<AdjMatrix*>(gptr)->matrix);
		}
		else if (type == 'L')
		{
			if(dynamic_cast<AdjLists*>(gptr)->W ==0)
				gptr = new ListOfEdges(dynamic_cast<AdjLists*>(gptr)->N, dynamic_cast<AdjLists*>(gptr)->D,
					dynamic_cast<AdjLists*>(gptr)->W, dynamic_cast<AdjLists*>(gptr)->list);
			else
			{
				gptr = new ListOfEdges(dynamic_cast<AdjLists*>(gptr)->N, dynamic_cast<AdjLists*>(gptr)->D,
					dynamic_cast<AdjLists*>(gptr)->W, dynamic_cast<AdjLists*>(gptr)->listWithWay);
			}
		}
		else
		{
			gptr = gptr;
		}
	}

	class adjIterator {
		int n;
		Graph* gr;
		int current;
		char type;
		bool isTop=false;
	public:
		adjIterator(Graph * g, int num)
		{
			n = num;
			current = -1;
			gr = g;
			type = (g)->type;
		}

		int begin()
		{
			if (type == 'C')
			{
				AdjMatrix *bg = dynamic_cast<AdjMatrix*>(gr->gptr);
				for (int j = 0; j < bg->N; j++)
				{
					if (bg->matrix[n - 1][j] != 0)
					{
						current = ++j;
						return current;
					}
				}
			}
			else if (type == 'L')
			{
				AdjLists *bg = dynamic_cast<AdjLists*>(gr->gptr);
				if (bg->W == 0)
					for (auto value : bg->list[n - 1])
					{
						current = value;
						return current;
					}
				else if (bg->W == 1)
					for (auto value : bg->listWithWay[n - 1])
					{
						current = value.first;
						return current;
					}
			}
			else if (type == 'E')
			{
				ListOfEdges *bg = dynamic_cast<ListOfEdges*>(gr->gptr);
				if (bg->W == 0)
				{
					if (bg->D == 1)
					{
						for (int i = 1; i < bg->N + 1; i++)
						{
							auto it = bg->listOfEdges.find(pair<int, int>(n, i));
							if (it != bg->listOfEdges.end())
							{
								return current = (*it).second;
							}
						}
					}
					else if (bg->D == 0)
					{
						if (current == bg->N && !isTop)
						{
							isTop = true;
							current = 0;
						}

						if (!isTop)
						{
							for (int i = current + 1; i < n; ++i)
							{
								auto it = bg->listOfEdges.find(pair<int, int>(i, n));
								if (it != bg->listOfEdges.end())
								{
									return current = (*it).first;
								}
							}

							for (int i = n + 1; i < bg->N + 1; i++)
							{
								auto it = bg->listOfEdges.find(pair<int, int>(i, n));
								if (it != bg->listOfEdges.end())
								{
									return current = (*it).first;
								}
							}
							isTop = true;
							current = 0;
						}


						if (isTop)
						{
							for (int i = current; i < bg->N + 1; i++)
							{
								auto it = bg->listOfEdges.find(pair<int, int>(n, i));
								auto it2 = bg->listOfEdges.find(pair<int, int>(i, n));
								if (it != bg->listOfEdges.end() && it2 == bg->listOfEdges.end())
								{
									return current = (*it).second;
								}
							}
						}
						return current = -1;

						//for (int i = 1; i < bg->N + 1; i++)
						//{
						//	auto it = bg->listOfEdges.find(pair<int, int>(n, i));
						//	if (it != bg->listOfEdges.end())
						//	{
						//		return current = (*it).second;
						//	}
						//}
						//for (int i = 1; i < bg->N + 1; i++)
						//{

						//	auto it = bg->listOfEdges.find(pair<int, int>(i, n));
						//	auto it2= bg->listOfEdges.find(pair<int, int>(n, i));
						//	if (it != bg->listOfEdges.end() && it2 == bg->listOfEdges.end())
						//	{
						//		return current = (*it).first;
						//	}
						//}

						/*for (int i = 1; i < n; i++)
						{
							auto it = bg->listOfEdges.find(pair<int, int>(i, n));
							if (it != bg->listOfEdges.end())
							{
								return current = (*it).first;
							}
						}

						for (int i = n+1; i < bg->N + 1; i++)
						{
							auto it = bg->listOfEdges.find(pair<int, int>(i, n));
							if (it != bg->listOfEdges.end())
							{
								return current = (*it).first;
							}
						}
						
						for (int i = 1; i < bg->N + 1; i++)
						{
							auto it = bg->listOfEdges.find(pair<int, int>(n, i));
							auto it2 = bg->listOfEdges.find(pair<int, int>(n, i));
							if (it != bg->listOfEdges.end() && it2 == bg->listOfEdges.end())
							{
								return current = (*it).second;
							}
						}*/
					}
				}

				else if (bg->W == 1)
				{
					if (bg->D == 1)
					{
							//auto it = bg->listOfEdgesWihtWay.find(pair<int, int>(n, i));
							for(auto value: bg->listOfEdgesWihtWay)
							{ 
								if (get<0>(value) == n && get<1>(value) > current)
								{
									return current = get<1>(value);
								}
							}
						//ok
					}
					else if (bg->D == 0)
					{
						if(!isTop)
							for (auto value : bg->listOfEdgesWihtWay)
							{
								if (get<0>(value) == n && get<1>(value) > current)
								{
									return current = get<1>(value);
								}

							}
						isTop = true;
						current == -1;
						int i=0;
						if (isTop)
						{
							for (auto value : bg->listOfEdgesWihtWay)
							{
								if (get<0>(value) > current && get<1>(value) == n)
								{
									i = get<0>(value);
								}
							}
							for (auto value : bg->listOfEdgesWihtWay)
							{
								if (get<0>(value) = n && get<1>(value) == i)
								{
									continue;
								}
							}

							return (i == 0) ? current = -1 : current = i;
						}
					}
				}
			}

			return current;
		}

		bool end()
		{
			return current == -1;
		}

		int operator++()
		{
			if (type == 'C')
			{
				AdjMatrix *bg = dynamic_cast<AdjMatrix*>(gr->gptr);
				for (int j = current; j < bg->N; j++)
					if (dynamic_cast<AdjMatrix*>(bg)->matrix[n - 1][j] != 0)
					{
						current = ++j;
						return current;
					}
			}
			else if (type == 'L')
			{
				AdjLists *bg = dynamic_cast<AdjLists*>(gr->gptr);
				if (bg->W == 0)
					for (auto value : bg->list[n - 1])
					{
						if (current != value && current < value)
						{
							current = value;
							return current;
						}
					}
				else if (bg->W == 1)
					for (auto value : bg->listWithWay[n - 1])
					{
						if (current != value.first && current < value.first)
						{
							current = value.first;
							return current;
						}
					}

			}
			else if (type == 'E')
			{
				ListOfEdges *bg = dynamic_cast<ListOfEdges*>(gr->gptr); //есть доступ тк итератор находится в этом классе
				if (bg->W == 0)
				{
					if (bg->D == 1)
					{
						for (int i = current + 1; i < bg->N + 1; i++)
						{
							auto it = bg->listOfEdges.find(pair<int, int>(n, i));
							if (it != bg->listOfEdges.end())
							{
								return current = (*it).second;
							}
						}
					}

					else if (bg->D == 0)
					{
						if (current == bg->N && !isTop)
						{
							isTop = true;
							current = 0;
						}

						if (!isTop)
						{
							for (int i = current + 1; i < n; ++i)
							{
								auto it = bg->listOfEdges.find(pair<int, int>(i, n));
								if (it != bg->listOfEdges.end())
								{
									return current = (*it).first;
								}
								current = n;
							}

							for (int i = current + 1; i < bg->N + 1; i++)
							{
								auto it = bg->listOfEdges.find(pair<int, int>(i, n));
								if (it != bg->listOfEdges.end())
								{
									return current = (*it).first;
								}
							}
							isTop = true;
							current = 0;
						}

						if (isTop)
						{
							for (int i = current+1; i < bg->N + 1; i++)
							{
								auto it = bg->listOfEdges.find(pair<int, int>(n, i));
								auto it2 = bg->listOfEdges.find(pair<int, int>(i, n));
								if (it != bg->listOfEdges.end() && it2 == bg->listOfEdges.end())
								{
									return current=(*it).second;
								}
							}
						}
					}
				}

				else if (bg->W == 1)
				{
					if (bg->D == 1)
					{
						//auto it = bg->listOfEdgesWihtWay.find(pair<int, int>(n, i));
						for (auto value : bg->listOfEdgesWihtWay)
						{
							if (get<0>(value) == n && get<1>(value) > current)
							{
								return current = get<1>(value);
							}
						}
						//ok
					}
					else if (bg->D == 0)
					{
						if (!isTop)
						{
							for (auto value : bg->listOfEdgesWihtWay)
							{
								if (get<0>(value) == n && get<1>(value) > current)
								{
									return current = get<1>(value);
								}
							}

							isTop = true;
							current = -1;
						}
						int i = 0;
						if (isTop)
						{
							for (auto value : bg->listOfEdgesWihtWay)
							{
								if (get<0>(value) > current && get<1>(value) == n)
								{
									i = get<0>(value);
									break;
								}
							}
							if (i != 0)
								for (auto value : bg->listOfEdgesWihtWay)
								{
									if (get<0>(value) == n && get<1>(value) == i)
									{
										i = 0; // найдена схожая
										break;
									}
								}

							return (i == 0) ? current = -1 : current = i;
						}
					}
				}
			}

			return current=-1;
		}

		bool operator!=(const int i)
		{
			return current != i;
		}

		int operator=(const int i)
		{
			current = i;
			return current;
		}

		int getTop()
		{
			return current;
		}
	};

	void outputNeighbors(int nTop)
	{
		auto iter = Graph::adjIterator(this, nTop);
		iter = iter.begin();
		cout << "Соседи вершины " << nTop << ": ";
		while (!iter.end())
		{
			cout << iter.getTop() << " ";
			++iter;
		} 
	}
};

int main()
{
	setlocale(LC_ALL, "rus");
	Graph gr;
	gr.readGraph("input_1e5_1e5.txt");
	//gr.addEdge(4, 1, 55555);
	//gr.addEdge(2, 1 ,5);
	//gr.removeEdge(2, 4);
	//gr.transformToAdjList();
	//cout<<gr.changeEdge(2, 1, 55)<<endl;

	int nTop = 2;
	gr.outputNeighbors(nTop);
	//auto iter = Graph::adjIterator(&(gr), nTop);
	//iter = iter.begin();
	//cout << "Соседи вершины " << nTop << ": ";
	//while (!iter.end())
	//{
	//	int nTop = 1;
	//	cout << iter.getTop() << " ";
	//	++iter;
	//}

	gr.writeGraph("output.txt");
	gr.transformToAdjMatrix();
	gr.writeGraph("output1.txt");

	return 0;
}