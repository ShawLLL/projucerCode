
#include <JuceHeader.h>
#include <fstream>
#include "msgpack11.hpp"

using namespace msgpack11;

int main( int argc, char **argv )
{
    MsgPack my_msgpack = MsgPack::object{
        { "key1", "value1" },
        { "key2","1"},
        { "key3", MsgPack::array{ 1, 2, 3 } },
    };

    //access to elements
    //std::cout << my_msgpack["key1"].string_value();

#if 0
    //serialize
    std::string msgpack_bytes = my_msgpack.dump();

	std::ofstream fileName1( "C:\\Users\\Donkey\\Desktop\\newFile\\3", std::ios::binary );
    //fileName1.write( msgpack_bytes.data(), msgpack_bytes.size() );
	fileName1 << msgpack_bytes;
    fileName1.close();
#endif	
	
	//deserialize
    std::ifstream fileName( "C:\\Users\\Donkey\\Desktop\\newFile\\3", std::ios::binary );
   
    std::string err;
    MsgPack des_msgpack = MsgPack::parse( fileName , err );
   
    std::cout << des_msgpack["key3"].array_items();
	
    fileName.close();
	
    return 0;
}