#include<iostream>
#include<assert.h>
using namespace std;

int isZero(float a) {
	//²»Îª0
	if ((int)a !=0||(int)a!=a)
	{
		return 1;
	}
	else {
		return 0;
	}
}

std::pair<int, int> getXX(const float* ptr, int len)
{
	assert(len);
	assert(ptr);
	int left = 0;
	int right = 0;

	for (int i = 0; i < len;i++)
	{
		if (isZero(ptr[i]))
		{
			left = i;
			break;
		}
	}
	for (int i =len-1;i>=left;i--)
	{
		if (isZero(ptr[i]))
		{
			right = i;
			if (left == right)
			{
				return std::make_pair(ptr[left], 1);
			}
			break;
		}
	}

	return std::make_pair(ptr[left], (right - left + 1));
}


int main()
{
	float a[] = { 0,0,0,0,0 };
	
	getXX(a, 5);

	system("pause");
	return 0;
}
