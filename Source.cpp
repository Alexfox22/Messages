#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <chrono>
#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include <random>
#include <Windows.h>
#include <thread>
#include <mutex>

using namespace std;
std::mutex meow;
class link
{
private:
	string _number;
	string _login;
public:

	string number()
	{
		srand(time(0));
		string num = "+7";
		int min = 100;
		int max = 999;
		for (int i = 0; i < 2; i++)
		{
			num += "-";
			int a = min + rand() % (max - min);
			num += to_string(a);
		}
		min = 10;
		max = 99;
		for (int i = 0; i < 2; i++)
		{
			num += "-";
			int a = min + rand() % (max - min);
			num += to_string(a);
		}
		return num;
	}

	string login()
	{
		srand(time(0));
		string login;
		int min = 1;
		int max = 9;
		for (int i = 0; i < 1; i++)
		{
			login += to_string(min + rand() % (max - min));
		}
		return login;
	}

	void generator()
	{
		srand(time(0));
		int id = 1 + rand() % 3;
		if (id == 1)
		{
			_number = number();
			_login = login();
		}
		if (id == 2)
		{
			_number = "";
			_login = login();
		}
		if (id == 3)
		{
			_number = number();
			_login = "";
		}
	}

	void _print()
	{
		cout << _number << ' ' << _login << endl;
	}

	string Get_number()
	{
		return _number;
	}

	string Get_login()
	{
		return _login;
	}
};

void threadFunction1(vector<link> &shared_container, bool &object_created, int start)
{
	do
	{
		link x;
		x.generator();
		meow.lock();
		shared_container.push_back(x);
		meow.unlock();
		cout << "NEW ";
		x._print();
		object_created = TRUE;
		this_thread::sleep_for(chrono::seconds(3));
	} while (std::time(0) - start != 300);
}

void threadFunction2(int &timer, vector<link> &shared_container, vector<link> &local_container, bool &object_created, int start)
{
	do
	{
		if (std::time(0) - timer < 60)
		{
			bool pair_finded = FALSE;
			if ((object_created == TRUE) && (shared_container.size() > 0))
			{
				meow.lock();
				link buf = shared_container.back();
				meow.unlock();
				if (local_container.size() > 0)
				{
					for (int i = 0; i < local_container.size(); i++)
					{
						if (((buf.Get_number() == local_container[i].Get_number()) && (buf.Get_login() == local_container[i].Get_login())) ||
							((buf.Get_number() != "") && (buf.Get_number() == local_container[i].Get_number())) ||
							((buf.Get_login() != "") && (buf.Get_login() == local_container[i].Get_login())))
						{

							cout << "\t\t\tPAIR!!!!   " << endl << "\t\t\tNUM: " << buf.Get_number() << " LOGIN: " << buf.Get_login() << endl << "\t\t\tNUM: " << local_container[i].Get_number() << " LOGIN: " << local_container[i].Get_login() << endl << endl;
							shared_container.pop_back();
							pair_finded = TRUE;
						}
					}
					if (pair_finded == FALSE)
					{
						local_container.push_back(buf);
					}
				}
				else
				{
					local_container.push_back(buf);
				}
			}
		}
		else
		{
			local_container.clear();
			cout << "---CLEANED---" << endl;
			timer = std::time(0);
		}
		object_created = FALSE;
	} while (std::time(0) - start != 300);
}

int main()
{
	srand(time(0));
	std::time_t start = std::time(0);
	int timer = start;

	vector<link> shared_container;
	vector<link> local_container;
	bool object_created = FALSE;
	thread thr1(threadFunction1, ref(shared_container), ref(object_created), start); 
	thread thr2(threadFunction2, ref(timer), ref(shared_container), ref(local_container), ref(object_created), start);
	thr1.join();
	thr2.join();

	system("pause");
	return 0;
}