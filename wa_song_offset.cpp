#include <JuceHeader.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
class Mp3BanzouOffset
{

public:
    Mp3BanzouOffset() = default;

    ~Mp3BanzouOffset() = default;

    void move_wav( const File& banzou_dir, File& song_dir )
    {
        Array<File> all_song_files   = song_dir.findChildFiles( File::findFiles, false, "*.wav" );
        Array<File> all_banzou_files = banzou_dir.findChildFiles( File::findFiles, false, "*.wav" );

        int i = 0;
		for ( auto banzou_file : all_banzou_files )
        {
            i++;
            cout << i << endl;
            String banzou_name = banzou_file.getFileNameWithoutExtension();

            for ( auto song_file : all_song_files )
            {
                String song_name = song_file.getFileNameWithoutExtension();
                song_name        = song_name + "_banzou";

                if ( banzou_name == song_name )
                {
                    update_banzou( song_file, banzou_file );
                }
                song_name = "";
            }
            if ( i == 10 )
                break;
        }
    }

private:
    void write_banzou( File& song_path, int& best_offset, AudioSampleBuffer& buffers, int& lengthInsample )
    {
        File wav_path = "C:/Users/Donkey/Desktop/aa/" + song_path.getFileName();
        File wav_song = wav_path.withFileExtension( "wav" );

        // std::clog << "failed to open input file " << wav_song.getFileName().toRawUTF8() << std::endl;

        WavAudioFormat format;

        if ( wav_song.existsAsFile() )
        {
            wav_song.deleteFile();
        }
        if ( !wav_song.create() )
        {
            throw 3;
        }

        OutputStream* output = wav_song.createOutputStream();
        if ( output == nullptr )
        {
            cout << "Mp3 OutputStream is Wrong! " << endl;
            exit( EXIT_FAILURE );
        }
        move_offset( song_path, buffers, best_offset, lengthInsample );

        //buffers写入伴奏
        StringPairArray tmp = WavAudioFormat::createBWAVMetadata( "nothing", "TBT", "TBT", Time::getCurrentTime(), buffers.getNumSamples(), "" );

        ScopedPointer<AudioFormatWriter> g = format.createWriterFor( output, sampleRates, buffers.getNumChannels(), 32, tmp, 0 );

        AudioSampleBuffer buffer = buffers;

        if ( !g->writeFromAudioSampleBuffer( buffer, 0, buffer.getNumSamples() ) ) throw 3;
    }

    void update_banzou( File& song_path, const File& banzou_path )
    {
        vector<float>     song_demo( 1000000, 0.0f );
        vector<float>     banzou_demo( 1000000, 0.0f );
        AudioSampleBuffer banzou_buffers;
        AudioSampleBuffer song_buffers;
        int               length_banzou;
        int               length_song;

        readwav( banzou_path, banzou_demo, banzou_buffers, length_banzou );
        readwav( song_path, song_demo, song_buffers, length_song );

        int offset = get_best_offset( song_demo, banzou_demo );
        write_banzou( song_path, offset, song_buffers, length_song );
    }

    void readwav( const File& song_path, vector<float>& songdemo, AudioSampleBuffer& buffers, int& lengthInsample )
    {
        WavAudioFormat wavformat;
        InputStream*   wavinput = song_path.createInputStream();
        if ( wavinput == nullptr )
        {
            cout << "mp3input_stream is wrong" << endl;
            exit( EXIT_FAILURE );
        }

        ScopedPointer<AudioFormatReader> p = wavformat.createReaderFor( wavinput, true );

        sampleRates = p->sampleRate;

        buffers.setSize( p->numChannels, (int) p->lengthInSamples );
        p->read( &buffers, 0, (int) p->lengthInSamples, 0, true, true );

        string song_name = song_path.getFileNameWithoutExtension().toStdString();

        cout << (int) p->lengthInSamples << endl;

        lengthInsample = (int) p->lengthInSamples;

        for ( int i = 0; i < 1000000; ++i )
        {
            songdemo[i] = (float) buffers.getReadPointer( 0 )[i];
        }
    }

    int get_best_offset( vector<float>& min_song, vector<float>& min_banzou )
    {
        int   banzou_offset = 0;
        int       size          = 1000000;
        float maxValue      = 0;
        float sumValue		= 0;
        int   public_length = 0;
        int   begin_index   = 0;
        int   end_index     = 0;

        for ( int offset = ( -size + 1 ); offset < size - 1; offset++ )
        {
            sumValue = 0;
            if ( offset < size - 6 )
            {
                offset += 5;
            }

            if ( offset <= 0 )
            {
                public_length = size + offset - 1;
                end_index     = size - 1 + offset;
				begin_index   = 0;
            }
            else if ( offset > 0 )
            {
                public_length = size - offset - 1;
                end_index     = size - 1;
                begin_index   = offset;
            }

            for ( int banzou_index = begin_index; banzou_index <end_index ; banzou_index++ )
            {

                int b_index = banzou_index - offset;
                if ( b_index < 0 )
                    continue;
                if ( b_index >= size )
                    break;
                sumValue += min_banzou[banzou_index] * min_song[b_index];
            }

            sumValue = sumValue*10;
            sumValue = sumValue / public_length + (int) sumValue % public_length;

            if ( sumValue > maxValue )
            {
                banzou_offset = offset;
                maxValue      = sumValue;
            }
        }

        cout << "最大互相关数：" << maxValue << "偏移量：" << banzou_offset << endl;

        return banzou_offset;
    }

    void move_offset( File& song_path, AudioBuffer<float>& buffers, int banzou_offset, int& lengthInsample )
    {
        vector<float> banzou_demo1( (int) lengthInsample, 0.0f );
        vector<float> banzou_demo2( (int) lengthInsample, 0.0f );

        for ( int i = 0; i < lengthInsample; ++i )
        {
            banzou_demo1[i] = buffers.getReadPointer( 0 )[i];
            banzou_demo2[i] = buffers.getReadPointer( 1 )[i];
        }

        if ( ( banzou_offset < -300 ) && ( banzou_offset > -45000 ) )
        {
            for ( int i = 0; i < ( 0 - banzou_offset ); i++ )
            {
                banzou_demo1.erase( banzou_demo1.begin() );
                banzou_demo1.push_back( 0.0f );
                banzou_demo2.erase( banzou_demo2.begin() );
                banzou_demo2.push_back( 0.0f );
            }
        }

        else if ( ( banzou_offset > 300 ) && ( banzou_offset < 45000 ) )
        {
            for ( int i = 0; i < banzou_offset; i++ )
            {
                banzou_demo1.pop_back();
                banzou_demo1.insert( banzou_demo1.begin(), 0.0f );
                banzou_demo2.pop_back();
                banzou_demo2.insert( banzou_demo2.begin(), 0.0f );
            }
        }

        else if ( ( banzou_offset >= 45000 ) || ( banzou_offset <= -45000 ) )
        {
            String song_name = song_path.getFileNameWithoutExtension();
            cout << song_name;
        }

        else
        {
        }

        for ( int i = 0; i < lengthInsample; ++i )
        {
            buffers.getWritePointer( 0 )[i] = banzou_demo1[i];
            buffers.getWritePointer( 1 )[i] = banzou_demo2[i];
        }
    }

private:
    float sampleRates;
};

int main()
{

    Mp3BanzouOffset a;

    File song_flie   = "C:/Users/Donkey/Desktop/gg/song/";
    File banzou_file = "C:/Users/Donkey/Desktop/gg/banzou/";
    /*if ( argc != 3 )
    {
        cout << "Usage: " << argv[0] << " DIR TEXT_FILE MSG_FILE" << endl;
        exit( EXIT_FAILURE );
    }*/

    /*File song_flie = File::createFileWithoutCheckingPath( argv[1] );
    File banzou_file = File::createFileWithoutCheckingPath( argv[2] );*/

    a.move_wav( banzou_file, song_flie );

    return 0;
}
