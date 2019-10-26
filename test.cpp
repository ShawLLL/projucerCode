
#include <JuceHeader.h>
#include <fstream>
#include "LameEncoder.h"
#include "msgpack11.hpp"

using namespace msgpack11;
using namespace std;

class msgPackFile
{

public:
    msgPackFile() = default;

    ~msgPackFile() = default;

    int msgPcakFile( const File& songDir, const File& textFile, const File& msgFile )
    {

        if ( !songDir.isDirectory() )
        {
            cout << "song_dir "<<songDir.getFullPathName().toStdString()<<" not exist!" << endl;
            exit( EXIT_FAILURE );
        }
        Array<File> all_song_files = songDir.findChildFiles( File::findFiles, true, "*.mp3" );

        if ( !textFile.existsAsFile() )
        {
            cout << "text_file "<<textFile.getFullPathName()<<" not exist" << endl;
            exit( EXIT_FAILURE );
        }


        String      content = textFile.loadFileAsString();
        StringArray all_songs;
        all_songs = StringArray::fromLines( content );


        for ( auto e : all_songs )
        {
            String temp1         = e.upToLastOccurrenceOf( " ", false, false );
            String endTimeText   = e.fromLastOccurrenceOf( " ", false, false );   //结束时间
            String startTimeText = temp1.fromLastOccurrenceOf( " ", false, false ); //开始时间
            String songName      = temp1.upToLastOccurrenceOf( " ", false, false ); //歌名
            songName             = songName.trim();
            startTimeText        = startTimeText.trim();
            endTimeText          = endTimeText.trim();

            File   currSongFile  = findMatchedSongFile( songName, all_song_files );
            File   msg_song_file = msgFile.getChildFile(currSongFile.getFileNameWithoutExtension());
            double startTime     = FuckToTime( startTimeText );
            double endTime       = FuckToTime( endTimeText );

            InputStream* currSongFh = currSongFile.createInputStream();
            if ( currSongFh == nullptr )
			{
                std::clog << "failed to open input file " << currSongFile.getFullPathName().toRawUTF8() << std::endl;
                exit( EXIT_FAILURE );
			}

            MP3AudioFormat                   mp3Format;
            ScopedPointer<AudioFormatReader> b      = mp3Format.createReaderFor( currSongFh, true );
            double                           rates  = b->sampleRate;
            int64                            length = b->lengthInSamples;

            int64                            curr_pos = currSongFh->getPosition();
            if ( curr_pos > 0 )
            {
                currSongFh->setPosition( 0 );  //指针回到0位置
            }
            LameDelay                        delays    = get_encode_delay( currSongFh );
            int                              delay_num = delays.headdelay + delays.taildelay;
           
			/*
			if ( delays.headdelay == -1 || delays.headdelay == -1 )
			{
                jassertfalse;
			}
			*/
            length -= delay_num;
            
            
            
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
                { "partStartMS", startTime },
                { "partEndMS", endTime },
                { "recordingStartMS", startTime },
                { "recordingEndMS", endTime },
                { "mp3Data", bbb },
                { "reverbIndex", 0 },
                { "samplerate", rates },//采样率
                { "lengthSP",  length}, //采样点
            };

            string msgpack_bytes = my_msgpack.dump();
            msg_song_file.replaceWithData( msgpack_bytes.data(), msgpack_bytes.size() );
        }
        return 0;
    }

private:
    File findMatchedSongFile( String name, const Array<File>& all_song )
    {
		String fuck_name = name + "_banzou";
        for ( auto e : all_song )
        {
            String fuck = e.getFileNameWithoutExtension();
            if( fuck.trim() == fuck_name )
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
        int    a    = k.getIntValue();
        int    b    = left.getIntValue();
        return a * 60 + b;
    }
};

int main( int argc, char** argv )
{
	if (argc != 4)
	{
        cout << "Usage: "<<argv[0]<<" DIR TEXT_FILE MSG_FILE" << endl;
        exit( EXIT_FAILURE );
	}

    //_CrtSetBreakAlloc( 1868 );
    msgPackFile a;
    File songDir  = File::createFileWithoutCheckingPath(argv[1]);
    File textFile = File::createFileWithoutCheckingPath(argv[2]);
    File msgFile  = File::createFileWithoutCheckingPath(argv[3]);
    a.msgPcakFile( songDir, textFile, msgFile );
    return 0;
}