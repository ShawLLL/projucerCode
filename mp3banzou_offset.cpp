#include <JuceHeader.h>
#include <iostream>
#include <vector>

using namespace std;
class Mp3BanzouOffset
{

public:
    Mp3BanzouOffset() = default;

    ~Mp3BanzouOffset() = default;

    void move_mp3( const File& song_dir, File& banzou_dir )
    {
        Array<File> all_song_files   = song_dir.findChildFiles( File::findFiles, false, "*.mp3" );
        Array<File> all_banzou_files = banzou_dir.findChildFiles( File::findFiles, false, "*.mp3" );

        for ( auto song_file : all_song_files )
        {

            String song_name = song_file.getFileNameWithoutExtension();
            song_name          = song_name + "_banzou";
			for ( auto banzou_file : all_banzou_files )
            {
                String banzou_name = banzou_file.getFileNameWithoutExtension();
                
                if ( banzou_name == song_name )
                {
                    update_banzou( song_file, banzou_file );
                }
                
            }
			song_name = "";
        }
    }

private:
    
	void write_banzou( File& banzou_path, int& best_offset, AudioSampleBuffer& buffers, int& lengthInsample )
	{
		
        File           wav_song = banzou_path.withFileExtension("wav"); 
		WavAudioFormat format;
        OutputStream*  output = wav_song.createOutputStream();
        if ( output == nullptr )
        {
            cout << "Mp3 OutputStream is Wrong! " << endl;
            exit( EXIT_FAILURE );
		}
        move_offset( buffers, best_offset , lengthInsample );
		
		//buffersÐ´Èë°é×à
        StringPairArray tmp = WavAudioFormat::createBWAVMetadata( "nothing", "TBT", "TBT", Time::getCurrentTime(), buffers.getNumSamples(), "" );	

		ScopedPointer<AudioFormatWriter> g = format.createWriterFor( output, sampleRates, buffers.getNumChannels(), 32, tmp, 0 );

        AudioSampleBuffer buffer = buffers;

		if ( !g->writeFromAudioSampleBuffer( buffer, 0, buffer.getNumSamples() ) ) throw 3;

	}

	void update_banzou( const File& song_path, File& banzou_path )
    {
        vector<float>     song_demo( 10000 );
        vector<float>     banzou_demo( 10000 );
        AudioSampleBuffer buffers;
        int               length_banzou;
        int               length_song;
        readmp3( song_path, song_demo, buffers ,length_song);
        readmp3( banzou_path, banzou_demo ,buffers,length_banzou);

        int offset = get_best_offset( song_demo, banzou_demo );
        write_banzou( banzou_path, offset, buffers, length_banzou );
    }



    void readmp3( const File& song_path, vector<float>& songdemo, AudioSampleBuffer& buffers ,int& lengthInsample)
    {
        MP3AudioFormat mp3format;
        InputStream*   mp3input = song_path.createInputStream();
        if ( mp3input == nullptr )
        {
            cout << "mp3input_stream is wrong" << endl;
            exit( EXIT_FAILURE );
        }

        ScopedPointer<AudioFormatReader> p = mp3format.createReaderFor( mp3input, true );
			
		sampleRates = p->sampleRate;  

        buffers.setSize( p->numChannels, (int) p->lengthInSamples );
        p->read( &buffers, 0, (int) p->lengthInSamples, 0, true, true );

		cout << (int) p->lengthInSamples << endl;
        lengthInsample = (int) p->lengthInSamples;

        for ( int i = 3000000; i < 3010000; ++i )
        {
            songdemo[ i - 3000000 ] = (float) buffers.getReadPointer( 0 )[i];
        }
        

    }

    int get_best_offset( const vector<float> min_song, vector<float> min_banzou )
    {
        int   banzou_offset = 0;
        int   size          = 10000;
        float maxValue      = 0.0f;
        float sumValue      = 0.0f;
        for ( int offset = ( -size + 1 ); offset < size; offset++ )
        {
            sumValue = 0;

            for ( int song_index = 0; song_index < size; song_index++ )
            {
                int b_index = song_index - offset;
                if ( b_index < 0 )
                    continue;
                if ( b_index >= size )
                    break;
                sumValue += min_song[song_index] * min_banzou[b_index];
            }

            if ( sumValue > maxValue )
            {
                banzou_offset = offset;
                maxValue      = sumValue;
            }
        }
        cout << banzou_offset << endl;
        return banzou_offset;
    }


    void move_offset( AudioBuffer<float>& buffers, int banzou_offset, int& lengthInsample )
    {
        vector<float> banzou_demo1( (int)lengthInsample, 0.0f );
        vector<float> banzou_demo2( (int)lengthInsample, 0.0f );

        for (int i = 0 ; i < lengthInsample ; ++i)
		{
			banzou_demo1[i] = buffers.getReadPointer( 0 )[i];
			banzou_demo2[i] = buffers.getReadPointer( 1 )[i];
        }

		/*for ( int j = 150000; j < 163000; ++j )
        {
            cout << "j" << buffers.getWritePointer( 0 )[j] << endl;
        }*/

		if ( banzou_offset < 0 )
        {
            for ( int i = 0; i < ( 0 - banzou_offset ); i++ )
            {
                banzou_demo1.erase( banzou_demo1.begin() );
                banzou_demo1.push_back( 0.0f );
                banzou_demo2.erase( banzou_demo2.begin() );
                banzou_demo2.push_back( 0.0f );
            }
        }

        else if ( banzou_offset > 0 )
        {
            for ( int i = 0; i < banzou_offset; i++ )
            {
                banzou_demo1.pop_back();
                banzou_demo1.insert( banzou_demo1.begin(), 0.0f );
                banzou_demo2.pop_back();
                banzou_demo2.insert( banzou_demo2.begin(), 0.0f );
            }
        }
        
        for ( int i = 0; i < lengthInsample; ++i )
        {
            buffers.getWritePointer( 0 )[i] = banzou_demo1[i] ;
            buffers.getWritePointer( 1 )[i] = banzou_demo2[i] ;
        }
       
		/* 
		for (int j = 150000; j < 163000; ++j )
		{
            cout << "j" <<buffers.getWritePointer( 0 )[j] << endl;
            
		}*/
    }


private:
    float sampleRates;
};

int main()
{

    Mp3BanzouOffset a;
    File            song_flie   = "D:/song_file/song_files/";
    File            banzou_file = "D:/song_file/banzou_files/";
    a.move_mp3( song_flie, banzou_file );

    return 0;
}
