
#include <JuceHeader.h>
#include <fstream>
#include "msgpack11.hpp"

using namespace msgpack11;
using namespace std;

class msgPackFile
{

public:
    msgPackFile() = default;

	~msgPackFile() = default;

    int msgPcakFile(const File& songDir,const File& textFile,String& msgFile )
	{
		
		if ( !songDir.exists() )
		{
			cout << "songDir func!" << endl;
			return 0;
		}
		Array<File> all_song_files = songDir.findChildFiles( File::findFiles, true, "*.mp3" );
		//File textFile = "C:/Users/Donkey/Desktop/msgpack.txt";
		
		if ( !textFile.existsAsFile() )
		{
			cout << "fuck!" << endl;
			return 0;
		}

	    String      content = textFile.loadFileAsString();
		StringArray all_songs;
		all_songs = StringArray::fromLines( content );
  
		for ( auto e : all_songs )
		{
        String songName      = e.upToFirstOccurrenceOf( " ", false, false );
        String startTimeText = e.fromFirstOccurrenceOf( " ", false, false ).upToFirstOccurrenceOf( " ", false, false );
        String endTimeText   = e.fromFirstOccurrenceOf( " ", false, false ).fromFirstOccurrenceOf( " ", false, false );
        songName             = songName.trim();
        startTimeText        = startTimeText.trim();
        endTimeText          = endTimeText.trim();

        File currSongFile = findMatchedSongFile( songName, all_song_files );
        File   msg_song_file = msgFile + currSongFile.getFileNameWithoutExtension();
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

        MsgPack my_msgpack = MsgPack::object{
            { "mPartStartMS", startTime },
            { "mPartEndMS", endTime },
            { "mRecordingStartMS", startTime },
            { "mRecordingEndMS", endTime },
            { "data",bbb },
        };
        
		string msgpack_bytes = my_msgpack.dump();
        msg_song_file.replaceWithData( msgpack_bytes.data(), msgpack_bytes.size() );
    }
}

private:

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
};


int main( int argc, char** argv )
{
    msgPackFile a;
    File        songDir = "C:/Users/Donkey/Desktop/aliyun_up_1024";
    File        textFile = "C:/Users/Donkey/Desktop/msgpack.txt";
    String        msgFile  = "C:/Users/Donkey/Desktop/3/";
    a.msgPcakFile( songDir ,textFile,msgFile);

    return 0;
}