#include <iostream>
#include <msgpack.hpp>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
#if 0
using namespace msgpack11;

int main( int argc, char **argv )
{
    MsgPack my_msgpack = MsgPack::object{
        { "key1", "value1" },
        { "key2", false },
        { "key3", MsgPack::array{ 1, 2, 3 } },
    };

    //access to elements
    std::cout << my_msgpack["key1"].string_value();

    //serialize
    std::string msgpack_bytes = my_msgpack.dump();

    //deserialize
    std::string err;
    MsgPack     des_msgpack = MsgPack::parse( msgpack_bytes, err );

    std::ofstream fileName( "2", std::ios::binary );
    fileName.write( err.data(), err.size() );
    fileName.close();

    return 0;
}
#endif 


#if 1
int main()
{
    msgpack::sbuffer buffer_;
    msgpack::packer<msgpack::sbuffer> pack_(&buffer_);

    vector<int>arr;
    arr.push_back(1);
    arr.push_back(3);
    arr.push_back(13);
    pack_.pack(arr);
//    pack_.pack(3);
//    pack_.pack(2.2);
//    pack_.pack(string("Kris.Wu"));

    //msgpack::type::tuple<int, bool, std::string> pack_(1, true, "example");
    ofstream fileName("2",ios::binary);
    fileName.write(buffer_.data(),buffer_.size());
    fileName.close();

    return 0;
}

#endif


