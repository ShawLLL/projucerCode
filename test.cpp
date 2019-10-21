#include<iostream>
#include<assert.h>
using namespace std;

bool isnotZero(float a) 
{
	//if ((int)a != 0 || (int)a != a)
	if(a!=0.0f)
	{
		return true;
	}
	else {
		return false;
	}
}

std::pair<int, int> getXX(const float* ptr, int len)
{
	assert(len);
	assert(ptr);
	int left = 0;
	int right = 0;

	for (int i = 0; i < len; i++)
	{
		if (isnotZero(ptr[i]))
		{
			left = i;
			break;
		}
	}
	for (int i = len - 1; i >= left; i--)
	{
		if (isnotZero(ptr[i]))
		{
			right = i;
			if (left == right)
			{
				return std::make_pair(ptr[left], 1);
			}
			break;
		}
	}
	if (!left && !right) 
	{
		return std::make_pair(NULL,0);
	}
	return std::make_pair(ptr[left], (right - left + 1));
}

int main() {
	float a[] = { 0,0,0,1,2,0,0,0,0 };
	getXX(a, 9);
	system("pause");
	return 0;
}
