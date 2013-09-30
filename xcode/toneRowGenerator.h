//
//  toneRowGenerator.h
//  toneRow
//
//  Created by Aaron Sherwood on 9/28/13.
//
//


#define __toneRow__toneRowGenerator__

#include "cinder/Rand.h"

using namespace ci;
using namespace ci::app;
using namespace std;

//    recursive backtracking to create a tone row where:
//        - each step should advance no more then 5 semitones in either direction
//        - each subsequent interval is different then the interval directly preceding it
//        - no note should repeat (obviously)

class Generator {
public:
	void generate();
    int mToneRow[12];
    string getNotes();
    bool hasRow=false;
    
private:
    int makeNewNote();
    int mMemNotes[12];
    int mNoteSelections[10];
    int mCount;
    int mMemCount;
    int mCurMemSpot;
    int mPreMemSpot;
    int mCurrentStep;
    int mPreviousStep;
    int mTotalCount;
    int mDirection;
    bool checkNoteSafe(int note);
    bool checkIntervalSafe();
    bool mBackStep;
    void makeToneRow();
    string mNotes;
};


