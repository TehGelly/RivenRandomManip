//rand.cpp
//a lot of this code was intentionally ripped from ScummVm
//to make sure that it was the least tampered as possible
//and that seemed to make sure it worked

//for TASing through libTAS, the system time starts out at 1000
//and ScummVm can only start Riven 5 frames after initialization
//and due to some weirdness, 6 frames is impossible
//but other than that, any value of frames can be waited for to manip
//the system time right

#include <iostream>

using namespace std;

//From Common::RandomSource, ScummVm

uint _randSeed = 0;

void setSeed(uint seed) {
	_randSeed = seed;
}

uint getRandomNumber(uint max) {
	_randSeed = 0xDEADBF03 * (_randSeed + 1);
	_randSeed = (_randSeed >> 13) | (_randSeed << 19);
	return _randSeed % (max + 1);
}

uint getRandomBit() {
	_randSeed = 0xDEADBF03 * (_randSeed + 1);
	_randSeed = (_randSeed >> 13) | (_randSeed << 19);
	return _randSeed & 1;
}

uint getRandomNumberRng(uint min, uint max) {
	return getRandomNumber(max - min) + min;
}



int main()
{
    //the commented vars below are intended to pinpoint your system time
    //not useful, atm
    /*uint TELE = 15422;
    uint PRISON = 11321;
    uint DOME [5] = {1,16,18,21,22};
    uint G = 25;
    uint DOMEBITS = (1<<(G-DOME[0]))|(1<<(G-DOME[1]))|(1<<(G-DOME[2]))|(1<<(G-DOME[3]))|(1<<(G-DOME[4]));*/

    
    uint metric = 135;
    
    //fm is 60*frames, to avoid decimal milliseconds
    for(uint fm = 5000; fm < 72000000; fm += 1000){
	      setSeed((fm/60)+1000);
        uint teleCombo = 0;
	      for (char i = 0; i < 5; i++) {
	    	  teleCombo *= 10;
	      	teleCombo += getRandomNumberRng(1, 5); // 5 buttons
	      }

    	// Randomize the prison combination
    	uint prisonCombo = 0;
    	for (char i = 0; i < 5; i++) {
	    	prisonCombo *= 10;
	    	prisonCombo += getRandomNumberRng(1, 3); // 3 buttons/sounds
	    }

	    // Randomize the dome combination -- each bit represents a slider position,
	    // the highest bit (1 << 24) represents 1, (1 << 23) represents 2, etc.
	    uint domeCombo = 0;
	    uint sum = 0;
	    uint largest = 0;
	    for (char bitsSet = 0; bitsSet < 5;) {
		    uint rand = getRandomNumber(24);
		    uint randomBit = 1 << (24 - rand);

    		// Don't overwrite a bit we already set, and throw out the bottom five bits being set
	    	if (domeCombo & randomBit || (domeCombo | randomBit) == 31)
		    	continue;

		    sum += rand;
		    if(rand > largest){
			    largest = rand;
		    }
    		domeCombo |= randomBit;
	    	bitsSet++;
	    }
    
    //the metric here is directly related to how many frames you have to wait for
    //for inputting the code, each digit requires one frame per move
    //for verification, only the largest digit takes one frame per move, as they all move simultaneously
		if(sum + largest <metric){
			metric = sum + largest;
			cout<<"tele = "<<teleCombo<<", prison = "<<prisonCombo<<", fm = "<<fm<<"\n";
			cout<<"this gives a metric of "<<metric<<" and a code of "<<domeCombo<<"\n";
		}

    }

    cout<<"done.\n";
    return 0;
}

//results:

//best frame within 20 minutes of waiting: 42866
//gives the dome combo of 1 2 3 4 6, with a metric of 16

//worst frame within 20 minutes of waiting: 199
//gives the dome combo of 20 22 23 24 25, with a metric of 133
