Compression:Compression.cpp
	g++ Compression.cpp -o Compression -I msgpack-c/include
unCompression:unCompression.cpp
	g++ unCompression.cpp -o unCompression -I msgpack-c/include

test1:test1.cpp
	g++ test1.cpp -o test -I msgpack-c/include
.PHONY:clean
	rm test test1 Compression
