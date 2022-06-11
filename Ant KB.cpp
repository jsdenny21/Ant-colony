#include <iostream>
#include <string>
#include <time.h>
#include <math.h>
using namespace std;


struct address
{
	string name;
	double x;
	double y;
};

class addresses
{
private:
	address* alamat;
	int nAddress;
	int ant;
public:
	addresses()
	{
		nAddress = 0;
		ant = 0;
	}

	void addAddress(string x, double px, double py)
	{
		nAddress++;
		if (nAddress == 1)
		{
			alamat = new address[nAddress];
			alamat[0].name = x;
			alamat[0].x = px;
			alamat[0].y = py;
		}
		else
		{
			address* temp = new address[nAddress];
			for (int i = 0; i < nAddress - 1; i++)
			{
				temp[i] = alamat[i];
			}

			temp[nAddress - 1].name = x;
			temp[nAddress - 1].x = px;
			temp[nAddress - 1].y = py;

			alamat = new address[nAddress];

			alamat = temp;

		}

	}

	void printAddressList()
	{
		if (nAddress == 0)
			cout << "List Kosong!" << endl;
		else
		{
			for (int i = 0; i < nAddress; i++)
			{
				cout << "Urutan: " << i << " \n" << alamat[i].name << " px: " << alamat[i].x << " py: " << alamat[i].y << endl << endl;;
			}
		}
	}

	void deleteAddress(int pos)
	{
		if (nAddress > 0)
		{
			address* temp = new address[nAddress - 1];
			int ctr = 0;
			for (int i = 0; i < nAddress; i++)
			{
				if (i != pos)
				{
					temp[ctr++] = alamat[i];
				}
			}

			alamat = new address[nAddress - 1];

			alamat = temp;
			nAddress--;
		}
		else
		{
			cout << "Data Kosong\n";
		}
	}

	int getNAdress()
	{
		return nAddress;
	}

	double getX(int idx)
	{
		return alamat[idx].x;
	}

	double getY(int idx)
	{
		return alamat[idx].y;
	}

	string getName(int idx)
	{
		return alamat[idx].name;
	}

};

class ant
{
private:
	bool* visitedCity;
	double cost;
	int defCity;
	int* pathTaken;
	int pathLen;
public:
	ant()
	{
		cost = 0;
		pathLen = 0;
	}

	void setEachAnt(int size) //Set semut awal e dek mana
	{
		//size adalah banyak kota
		visitedCity = new bool[size];
		pathTaken = new int[size];
		for (int i = 0; i < size; i++)
		{
			visitedCity[i] = 0;
		}

		//random default city for each ant
		int rng = rand() % size;
		defCity = rng;

		visitedCity[rng] = 1;

		//menambahkan default city ke path yang diambil semut
		addPath(defCity);
	}

	void addPath(int idx)
	{
		pathTaken[pathLen++] = idx;
	}

	void resetAnt(int size)
	{
		for (int i = 0; i < size; i++)
		{
			visitedCity[i] = 0;
		}

		visitedCity[defCity] = 1;
		pathLen = 0;
		addPath(defCity);
		cost = 0;
	}

	void chooseNextCity(int antId, int size, double** pheromone, double** attractiveness, double** Costprm)
	{
		double* probability = new double[size - pathLen];
		double* valueOfPheXAttr = new double[size - pathLen];
		double* tempProbability = new double[size - pathLen];
		double sumOfPheXAttr = 0;

		//looking for the unvisited city
		int* unvisitedCity = new int[size - pathLen];
		int ctr = 0;
		for (int i = 0; i < size; i++)
		{
			if (visitedCity[i] == 0)
			{
				unvisitedCity[ctr++] = i;
			}
		}


			//calculate the value of attr * pheromone
		for (int j = 0; j < size - pathLen; j++)
		{
			valueOfPheXAttr[j] = pow(attractiveness[pathTaken[pathLen - 1]][unvisitedCity[j]],5) * pow(pheromone[pathTaken[pathLen - 1]][unvisitedCity[j]],1);
			sumOfPheXAttr += valueOfPheXAttr[j];
		}

		double cummulativeDistance = 0;

		//calculate the probability of ant city to any next unvisited city
		for (int j = 0; j < size - pathLen; j++)
		{
			probability[j] = valueOfPheXAttr[j] / sumOfPheXAttr * 1000;
			tempProbability[j] = (int)(probability[j]);
			cummulativeDistance += tempProbability[j];
		}

		//roulette
		double rng = rand() % (int)cummulativeDistance;
		cummulativeDistance = 0;


		//random nextcity
		for (int j = 0; j < size - pathLen; j++)
		{
			cummulativeDistance += tempProbability[j];
			if (rng <= cummulativeDistance)
			{
				cost += Costprm[pathTaken[pathLen - 1]][unvisitedCity[j]];
				addPath(unvisitedCity[j]);
				visitedCity[unvisitedCity[j]] = 1;
				return;
			}
		}

	}

	int* getPathTaken()
	{
		return pathTaken;
	}

	double getcost()
	{
		return cost;
	}

	void addcost(double** Costprm)
	{
		cost += Costprm[pathTaken[0]][pathTaken[pathLen - 1]];
	}

	void printPath(addresses city)
	{
		cout << "Path Taken: ";
		for (int i = 0; i < pathLen; i++)
		{
			cout << city.getName(pathTaken[i]) << " -> ";
		}

		cout << city.getName(defCity) << endl;
		cout << "Cost: " << cost << endl;
	}
};


class graph
{
private:
	int size;
	double** pheromone;
	double** Attractiveness;
	double** cost; // jarak kota
	addresses city;
	ant* ants;
public:
	graph()
	{
		size = 0;
	}

	graph(int size, addresses city)
	{
		this->size = size;
		ants = new ant[size];


		//setup pheromone
		pheromone = new double* [size];
		for (int i = 0; i < size; i++)
		{
			pheromone[i] = new double[size];
		}
		this->city = city;

		//make all path pheromone is 1
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				if (i != j)
					pheromone[i][j] = 1;
				else
					pheromone[i][j] = 0;
			}
		}

		//setup attractiveness
		Attractiveness = new double* [size];
		for (int i = 0; i < size; i++)
		{
			Attractiveness[i] = new double[size];
		}

		//calculate the attractiveness value for each path

		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				Attractiveness[i][j] = 1.0f / ecludianDistance(i, j);
			}
		}

		//setup cost
		cost = new double* [size];
		for (int i = 0; i < size; i++)
		{
			cost[i] = new double[size];
		}

		//calculate cost from city i to city j
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{

				cost[i][j] = ecludianDistance(i, j);
			}
		}
	}

	double ecludianDistance(int i, int j)
	{
		double x1, y1, x2, y2;
		x1 = city.getX(i);
		x2 = city.getX(j);
		y1 = city.getY(i);
		y2 = city.getY(j);
		return (sqrt(pow((x1 - x2), 2) + pow((y1 - y2), 2)));
	}

	void TSP()
	{
		//place each ant in randomly chosen city
		for (int i = 0; i < size; i++)
		{
			ants[i].setEachAnt(size);
		}


		//start ant
		int limit = 0;
		do {

			int x = 0;
			do
			{
				for (int i = 0; i < size; i++)//30 semut
				{
					ants[i].chooseNextCity(i, size, pheromone, Attractiveness, cost);
				}

				x++;
			} while (x < size - 1);//kota nambah 1 sampe kota sudah terterlusuri semua



			for (int i = 0; i < size; i++)//nambah cost kota awal
			{
				ants[i].addcost(cost);
			}

			//update pheromone
			for (int i = 0; i < size; i++)
			{
				for (int j = 0; j < size; j++)//kota ke i j
				{
					if (i != j)
					{
						double sum = 0;
						int* path = new int[size];
						for (int k = 0; k < size; k++)//semut ke k
						{
							path = ants[k].getPathTaken();

							for (int l = 0; l < size - 1; l++)//cek visited city
							{
								if (path[l] == i && path[l + 1] == j)
								{
									sum += 1.0f / ants[k].getcost();
								}
							}
						}
						pheromone[i][j] = pheromone[i][j] * 0.8 + sum;
					}
				}
			}


			if (limit < 99)
			{
				//making the ants dead
				for (int i = 0; i < size; i++)
				{
					ants[i].resetAnt(size);
				}
			}
			else
			{
				double* costt = new double[size];
				ant* dummb = new ant[size];
				dummb = ants;
				for (int i = 0; i < size; i++) {
					costt[i] = ants[i].getcost();
				}
				for (int i = 0; i < size - 1; i++)
				{

					int dumb = i;
					for (int j = i + 1; j < size; j++)
					{
						if (costt[dumb] > costt[j])
						{
							dumb = j;
						}
					}
					double k = costt[i];
					costt[i] = costt[dumb];
					costt[dumb] = k;
					ant tem = dummb[i];
					dummb[i] = dummb[dumb];
					dummb[dumb] = tem;
				}
				ants = dummb;
				for (int i = 0; i < size; i++)
				{
					ants[i].printPath(city);
					cout << costt[i] << endl;
				}
			}

			limit++;
		} while (limit < 100);
	}
};


void main()
{
	int chc;
	addresses RUMAHDENNY;
	srand(time(NULL));

	//input statis dari excel
	{
		RUMAHDENNY.addAddress("a1", -4, 4);
		RUMAHDENNY.addAddress("a2", 16, -5);
		RUMAHDENNY.addAddress("a3", 22, 8);
		RUMAHDENNY.addAddress("a4", 0, -16);
		RUMAHDENNY.addAddress("a5", 3, -19);
		RUMAHDENNY.addAddress("a6", 24, -20);
		RUMAHDENNY.addAddress("a7", 24, -15);
		RUMAHDENNY.addAddress("a8", 16, -1);
		RUMAHDENNY.addAddress("a9", 10, -15);
		RUMAHDENNY.addAddress("a10", -7, -2);
		RUMAHDENNY.addAddress("a11", 13, 10);
		RUMAHDENNY.addAddress("a12", 17, -6);
		RUMAHDENNY.addAddress("a13", 19, 1);
		RUMAHDENNY.addAddress("a14", 19, -16);
		RUMAHDENNY.addAddress("a15", -8, -8);
		RUMAHDENNY.addAddress("a16", -4, -1);
		RUMAHDENNY.addAddress("a17", 20, -3);
		RUMAHDENNY.addAddress("a18", 17, -17);
		RUMAHDENNY.addAddress("a19", -2, 9);
		RUMAHDENNY.addAddress("a20", 23, -24);
		RUMAHDENNY.addAddress("a21", 32, -26);
		RUMAHDENNY.addAddress("a22", 37, -20);
		RUMAHDENNY.addAddress("a23", 30, -30);
		RUMAHDENNY.addAddress("a24", 26, -22);
		RUMAHDENNY.addAddress("a25", 37, -16);
		RUMAHDENNY.addAddress("a26", 25, -18);
		RUMAHDENNY.addAddress("a27", 17, -30);
		RUMAHDENNY.addAddress("a28", 26, -28);
		RUMAHDENNY.addAddress("a29", 27, -15);
		RUMAHDENNY.addAddress("a30", 18, -24);
	}
	do
	{
		system("CLS");
		cout << "1. Add Addresses\n";
		cout << "2. Delete Addresses\n";
		cout << "3. View Addresses\n";
		cout << "4. TSP\n";
		cout << "option:";
		cin >> chc;

		if (chc == 1)
		{
			cin.get();
			string nama;
			double px, py;
			cout << "Masukkan Nama: ";
			getline(cin, nama);
			cout << "Masukkan Posisi X: ";
			cin >> px;
			cout << "Masukkan Posisi Y: ";
			cin >> py;

			RUMAHDENNY.addAddress(nama, px, py);

		}
		else if (chc == 2)
		{
			cout << "Masukkan Posisi: ";
			int idx;
			cin >> idx;

			RUMAHDENNY.deleteAddress(idx);
		}
		else if (chc == 3)
		{
			RUMAHDENNY.printAddressList();
		}
		else if (chc == 4)
		{
			graph map(RUMAHDENNY.getNAdress(), RUMAHDENNY);
			map.TSP();
		}

		system("pause");
	} while (chc != 0);
}