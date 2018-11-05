#pragma once

#include "pch.h"
#include <iostream>
#include <mmdeviceapi.h>
#include <Audioclient.h>
#include <conio.h>

#include "cap.h"

class MyAudioSink
{
public:
	FILE *pfile;
	DWORD filelength = 0;
	waveheader mwaveheader;
	MyAudioSink()
	{
		fopen("*/1.wav", "wb+");
	}
	~MyAudioSink()
	{
		//log length&data length
		//offset 4  & 44
		fseek(pfile, 4, 0);
		fwrite((&filelength) - 8, 4, 1, pfile);
		fseek(pfile, 36 + 4, 0);
		fwrite((&filelength) - 8 - 36, 4, 1, pfile);

		fclose(pfile);
	}
	HRESULT SetFormat(WAVEFORMATEX *wfat)
	{
		mwaveheader.channel = wfat->nChannels;
		mwaveheader.samplerate = wfat->nSamplesPerSec;
		mwaveheader.bitsdepth = wfat->wBitsPerSample;
		mwaveheader.samplesize = wfat->nBlockAlign;
		mwaveheader.bytespersec = wfat->nAvgBytesPerSec;
		mwaveheader.pcm = wfat->wFormatTag;

		WORD exinfolength = wfat->cbSize;
		if (exinfolength != 0)return -1;
		//if extra info exists ,cannot handle
		//can only handle when exinfo=0
		filelength = 36 + 8;
		fwrite(&mwaveheader, filelength, 1, pfile);
		//write waveheader
		return 1;
	}
	HRESULT CopyData(byte *pData, UINT32 numFramesAvailable, BOOL *bDone)
	{
		filelength += numFramesAvailable * mwaveheader.samplesize;
		if (pData == NULL)
		{
			for (int i = 0;i < numFramesAvailable;++i)
			{
				BYTE zero[32] = { 0 };
				fwrite(&zero, mwaveheader.samplesize, 1, pfile);
				//write silence
			}

		}
		else memcpy(pfile + filelength, pData, numFramesAvailable);

		//move buffer data into file(memory)
		while (kbhit())
		{
			*bDone = TRUE;
		}
		*bDone = FALSE;
		return 1;
	}
};