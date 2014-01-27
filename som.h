#include "Headers.h"

#pragma once
class cSound
{

public:

	struct smp3
	{

	}mp3;

	struct swav
	{
		void playsound(LPCSTR sound);
		void playsoundmem(LPCSTR sound);
	}wav;

};
extern cSound Sound;