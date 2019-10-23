#include <msgpack.hpp>
#include <vector>
#include <string>
#include <iostream>
#include "fstream"

int main()
{
	/*文件二进制读取*/
	std::ifstream is("3",std::ios::binary);
	is.seekg (0, is.end);
	int n = is.tellg();
	is.seekg (0, is.beg);
	msgpack::unpacker unpack_;
	unpack_.reserve_buffer(n);
	is.read(unpack_.buffer(), n);
	unpack_.buffer_consumed(n);
	msgpack::unpacked result_;
	while (unpack_.next(result_))
	{
		std::cout << result_.get() << std::endl;
	}
	return 0;
}
