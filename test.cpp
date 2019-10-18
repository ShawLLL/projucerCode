#include <iostream>
#include <assert.h>
using namespace std;

class ProBank
{
public:
	ProBank() {

	}

	~ProBank() = default;

	void push(float* out, const float* in, int len) 
	{
		float inout[1024] = { 0 };
		addArr(out,in, len);//1.合并数组并且传给数组
		sendProcess(inout,in,length(len));
		a = in[len - 1];
	}

protected:
	int length(int len) 
	{
		len = len -(((len + 1) % 2) - 1);
		return len;
	}

	void process(float* inout, int len) 
	{
		for (int i = 0; i < len; i++)
		{
			cout << inout[i] << "," << endl;
		}
	}
	//virtual void process(float* inout, int len) = 0;

private:
	void sendProcess(float* inout,const float* in, int len)
	{
		for (int i = 1; i < len; i++) {
			inout[i] = in[i - 1];
		}
		inout[0] = a;
		process(inout, len);
	}

	void addArr(float* out, const float *in, int len)
	{
		out[0] = a;
		for (int i = 0; i < len - 1; i++) {
			out[i + 1] = in[i];
		}
		for (int i = 0; i < len; i++) {
			cout << "out:"<<out[i] << "!!!!!" << endl;
		}
	}
	
	float a = 0.0000;
};

int main() {
	ProBank obj;

	float a1[5] = {2.2,2,1.10,12,45};
	float a2[5];
	obj.push(a2, a1, 5);
	system("pause");
	return 0;
}

#if 0
class ringObj {
public:
	ringObj() = default;

	~ringObj() = default;

	
	float * pushArray(float ptr[],int len)
	{
		swap(ptr[len - 1], a);
		finalArr(ptr,len);
		return ptr;
	}

private:
	
	void finalArr(float ptr[], int len)
	{
		float x = ptr[len - 1];
		for (int i = len - 1; i > 0; i--)
		{
			ptr[i] = ptr[i - 1];
		}
		ptr[0] = x;
	}
	float a = 0.00;
};

int main(){
	ringObj obj;
	float arr[] = { 1,1.2,12 };
	float *p;
	int length = sizeof(arr) / sizeof(arr[0]);
	p = obj.pushArray(arr,length);
	for (int i = 0; i < length; i++) {
		cout << p[i] << endl;
	}
#if 0	
	float arr[] = {1,1.2,12};
	obj.pushArray(arr);
	int length = sizeof(arr) / sizeof(arr[0]);
	for (int i = 0; i < length; i++)
	{
		cout << arr[i] << endl;
	}
	cout << endl << endl;
	
	float arr1[] = { 7,7,7,7 };

	obj.pushArray(arr1);
	int length1 = sizeof(arr1) / sizeof(arr1[0]);
	for (int i = 0; i < length1; i++)
	{
		cout << arr1[i] << endl;
	}
#endif

	system("pause");
	return 0;
}

#endif