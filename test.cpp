
#include <JuceHeader.h>
#include <fstream>
#include "msgpack11.hpp"

using namespace msgpack11;
using namespace std;

File findMatchedSongFile( String name, const Array<File>& all_song )
{
    for ( auto e : all_song )
    {
        String fuck = e.getFileNameWithoutExtension();
        if ( fuck.contains( name ) && fuck.contains( "banzou" ) )
        {
            DBG( fuck );
            return e;
        }
    }
    return File();
}

double FuckToTime( const String& xx ) noexcept
{
    String k    = xx.substring( 0, 1 );
    String left = xx.substring( 1 );

    int a = k.getIntValue();
    int b = left.getIntValue();

    return a * 60 + b;
}

int msgPcakFile()
{
    File songDir = "C:/Users/Donkey/Desktop/aliyun_up_1024";

    if ( !songDir.exists() )
    {
        cout << "songDir func!" << endl;
        return 0;
    }

    Array<File> all_song_files = songDir.findChildFiles( File::findFiles, true, "*.mp3" );

    File textFile = "C:/Users/Donkey/Desktop/msgpack.txt";
    if ( !textFile.existsAsFile() )
    {
        cout << "fuck!" << endl;
        return 0;
    }

    String      content = textFile.loadFileAsString();
    StringArray all_songs;
    all_songs = StringArray::fromLines( content );

    File msg_song_file = "C:/Users/Donkey/Desktop/1/a";

	MsgPack my_msgpack;
    int     i = 0;
    for ( auto e : all_songs )
    {
        String songName      = e.upToFirstOccurrenceOf( " ", false, false );
        String startTimeText = e.fromFirstOccurrenceOf( " ", false, false ).upToFirstOccurrenceOf( " ", false, false );
        String endTimeText   = e.fromFirstOccurrenceOf( " ", false, false ).fromFirstOccurrenceOf( " ", false, false );
        songName             = songName.trim();
        startTimeText        = startTimeText.trim();
        endTimeText          = endTimeText.trim();

        File currSongFile = findMatchedSongFile( songName, all_song_files );

        double startTime = FuckToTime( startTimeText );
        double endTime   = FuckToTime( endTimeText );

        MemoryBlock blk;
        if ( !currSongFile.loadFileAsData( blk ) )
        {
            cout << "load failed" << endl;
            return 0;
        }

		MsgPack::binary bbb;
        bbb.resize( blk.getSize() );
        memcpy( bbb.data(), blk.getData(), blk.getSize() );

		// "mPartStartMS" : startTime : 开始播放的位置
        // "mPartEndMS" : endTime : 结束播放的位置
        // "mRecordingStartMS" : startTime : 开始录音的位置
        // "mRecordingEndMS" : endTime : 结束录音的位置

        my_msgpack = MsgPack::object{
            { "mPartStartMS", startTime },
            { "mPartEndMS", endTime },
            { "mRecordingStartMS", startTime },
            { "mRecordingEndMS", endTime },
            { "data", bbb }
        };
        string msgpack_bytes = my_msgpack.dump();
        msg_song_file.replaceWithData( msgpack_bytes.data(), msgpack_bytes.size() );
        i++;
		if (i == 10)
		{
            return 0;
		}
        //------------------------------
        MsgPack     test_pack;
        MemoryBlock testBlk;
        msg_song_file.loadFileAsData( testBlk );
#if 0
        std::string str( (const char*) testBlk.getData(), testBlk.getSize() );
        MsgPack     tmp;
        std::string err;
        tmp = MsgPack::parse( str, err );
        auto tmp1 = tmp.object_items();
        jassert( tmp1.count( "beginXX" ) > 0 );
        jassert( tmp1.count( "endXX" ) > 0 );
        jassert( tmp1["beginXX"].float64_value() == startTime );
        jassert( tmp1["endXX"].float64_value() == endTime );
#endif
    }
}

int main( int argc, char** argv )
{
    /*  MsgPack my_msgpack = MsgPack::object{
        { "key1", "value1" },
        { "key2","1"},
        { "key3", MsgPack::array{ 1, 2, 3 } },
    };*/

    msgPcakFile();

    //access to elements
    //std::cout << my_msgpack["key1"].string_value();

#if 0
    //serialize
    std::string msgpack_bytes = my_msgpack.dump();

	std::ofstream fileName1( "C:\\Users\\Donkey\\Desktop\\newFile\\3.txt", std::ios::binary );
    //fileName1.write( msgpack_bytes.data(), msgpack_bytes.size() );
	fileName1 << msgpack_bytes;
    fileName1.close();
	
	
	//deserialize
    std::ifstream fileName( "C:\\Users\\Donkey\\Desktop\\newFile\\3.txt", std::ios::binary );
   
    std::string err;
    MsgPack des_msgpack = MsgPack::parse( fileName , err );
   
    std::cout << des_msgpack["key3"].array_items();
	
    fileName.close();
#endif
    return 0;
}