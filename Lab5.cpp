#include<iostream>
#include<vector>
#include<string>
#include <fstream>
#include<sstream>
#include <iterator>
#include<algorithm>
#include <omp.h>
#include<mutex>
#include<signal.h>
#include<unistd.h>
#include<stdio.h>


using namespace std;
std::mutex loc;
int cou = 0;


void funhandle(int p)
{
cout << cou << "  in 2 sec" << endl;
alarm(2);
cou = 0;
}


int main()
{

    signal(SIGALRM, funhandle);
    alarm(2);


	vector<string> a;
	vector<int> b;
	ifstream file("text.txt");
	string s;
	while (file >> s)
		a.push_back(s);

	for (int i = 2; i < a.size(); i++)
	{
		stringstream temp(a[i]);
		int temp2 = 0;
		temp >> temp2;
		b.push_back(temp2);
	}
	int value = b[0];
	int clause = b[1];

	
	vector<vector<int>> table;
	vector<int> row;
	for (int i = 2; i < b.size(); i++)
	{
		if (b[i] == 0)
		{
			table.push_back(row);
			row.clear();
		}
		else
		{
			row.push_back(b[i]);
		}
	}
	

	bool satify = false; //false
	vector<int> solution;
	bool con = true;
	for (int m = 0; m < value; m++)
	{
		int yes = 0;
		int no = 0;
		int unknown = 0;

		if (con == true)
		{
			solution.push_back(m + 1);
		}
		else if (con == false)
		{
			solution.push_back(-(m + 1));
			con = true;
		}

#pragma omp parallel for
		for (int i = 0; i < table.size(); i++) 
		{
			
			int min = 0;
			int max = 0;
			int larger = 0;
			vector<int>::const_iterator it;
			it = min_element(table[i].begin(), table[i].end());
			min = *it;
			if (min < 0)
				min = -min;
			it = max_element(table[i].begin(), table[i].end());
			max = *it;
			if (max < 0)
				max = -max;
			
			if (max > min)
				larger = max;
			else
				larger = min;

			
			bool clause_con = false;
			for (int j = 0; j < table[i].size(); j++)
			{
				
				for (int k = 0; k < solution.size(); k++)
				{
					if (table[i][j] == solution[k])
					{
						clause_con = true;
						break;
					}
				}
				if (clause_con == true)
				{
					break;
				}
			}

			if (clause_con == true)
			{
                loc.lock();
				yes++;
                loc.unlock();
			}
			else if (clause_con == false)
			{
				if (larger == (m + 1))
				{
                    loc.lock();
					no++;
                    loc.unlock();
					
				}
			}
		}

		int neg_sol_count = 0;
		bool no_sol = false;
		if (no > 0)
		{
			if (solution[m] > 0)
			{
				con = false;
				solution.erase(solution.begin() + m);
				m--; 
                cou++;
			}
			else if (solution[m] < 0)
			{
				int pre_m = m;
				for (int n = m - 1; n >= 0; n--)
				{
					if (solution[n] > 0)
					{
						con = false;
						solution.erase(solution.begin() + n, solution.end());
						m = n - 1;
                        cou++;
						break;
					}
					else
						neg_sol_count++;
				}
				if(neg_sol_count == pre_m)
				{
					satify = false;
					cout << "not satify" << endl;
					break;
				}
			}
		}

		else if (yes == clause)
		{
			satify = true;
			cout << "satify" << endl;
			for (int i = 0; i < solution.size(); i++)
				cout << solution[i] << " ";
			cout << endl;
			
			break;
		}
	}

	
	return 0;
}
