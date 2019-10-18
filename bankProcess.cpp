#include <iostream>
#include <assert.h>
using namespace std;
class ProBank
{
public:
	ProBank() = default;
	~ProBank() = default;

	void push(float* out, const float* in, int len)
	{
		float inout[1024] = { 0 };
		addArr(out, in, len);//1.合并数组并且传给数组
		sendProcess(inout, in, length(len));
		a = in[len - 1];
	}

protected:
	virtual void process(float* inout, int len) = 0;

private:
	int length(int len)
	{
		len = len - (((len + 1) % 2) - 1);
		return len;
	}

	void sendProcess(float* inout, const float* in, int len)
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
	}

	float a = 0.0000;
};
