//
//  toneRowGenerator.cpp
//  toneRow
//
//  Created by Aaron Sherwood on 9/28/13.
//
//

#include "toneRowGenerator.h"

void Generator::generate()
{
    mNotes="C C#D D#E F F#G G#A A#B ";
    mBackStep=false;
    mTotalCount=0;
    mMemCount=0;
    mCurMemSpot=0;
    mCurrentStep=0;
    mDirection=1;
    
    //initialize the arrays
    for (int i=0; i<12; i++) {
        mToneRow[i]=0;
        mMemNotes[i]=0;
    }
    
    //select a random starting point for the tone row
    mToneRow[mCurrentStep] = Rand::randInt(12)+60;
    
    //move to next step
    mPreviousStep=mCurrentStep;
    mCurrentStep++;
    
    makeToneRow();
}

void Generator::makeToneRow()
{
    while (mCurrentStep<12) {
        int note = makeNewNote();
        if ( checkNoteSafe(note) ) {
            if (checkIntervalSafe()) {
                mToneRow[mCurrentStep]=note;
                mCurrentStep++;
                mTotalCount++;
                //print out
                cout<<"ADVANCING step - current note is now: "<<mCurrentStep<<endl;
                cout<<"total steps is: "<<mTotalCount<<endl;
                int octave;
                for (int i=0; i<12; i++) {
                    if (mToneRow[i]!=0){
                        int start = mToneRow[i]%12 * 2;
                        octave=mToneRow[i]/12-1;
                        string note=mNotes.substr(start,2);
                        std::string::iterator end_pos = std::remove(note.begin(), note.end(), ' ');
                        note.erase(end_pos, note.end());
                        cout<<note<<octave<<" ";
                    }
                }
                cout<<endl<<endl;
                //yep, it's all printing out
            }
        }
    }
    hasRow=true;
    //this is actually fake, if i recurse instead of the while loop, on tone rows with tons of steps the program runs out of memory
    if (mCurrentStep<12) makeToneRow();
}

// fill array with new notes
int Generator::makeNewNote()
{
    //reset current step note tracker if new step
    if (mPreviousStep!=mCurrentStep) {
        mCount=0;
        for (int i=0; i<10; i++) {
            mNoteSelections[i]=0;
        }
    }
    
    //set previous step
    mPreviousStep=mCurrentStep;
    
    int mCurrentNote;
    
    //if exhausted all possibilties go back a step
    if (mNoteSelections[9]!=0) {
        mBackStep=true;
        
    } else {
        //try random notes until we find one we haven't tried
        while (mNoteSelections[mCount]==0) {
            
            //switch the direction of the interval 70% of the time
            int mDirProb = Rand::randInt(10)+1;
            if (mDirProb<7) {
                mDirection*=-1;
            }
            if (mDirection==1) {
                mCurrentNote = mToneRow[mCurrentStep-1] + (Rand::randInt(5)+1);
            } else {
                mCurrentNote = mToneRow[mCurrentStep-1] - (Rand::randInt(5)+1);
            }
            
            //check current note selections to see if we've exhausted all possibilties
            for (int i=0; i<10; i++) {
                //if note not used previously, set new note and break
                if (mNoteSelections[i]!=mCurrentNote) {
                    mNoteSelections[mCount]=mCurrentNote;
                    break;
                }
            }
        }
    }
    
    mCount++;
    return mCurrentNote;
}

//check to see if note has been used already in the tone row
bool Generator::checkNoteSafe(int note)
{
    bool safe = true;
    
    //move back a step if we need to
    if (mBackStep) {
        safe = false;
        mCurrentStep--;
        mTotalCount++;
        //lets remember where we went and not go there again, **works sorta**
        if (mCurMemSpot==mPreMemSpot) {
            mCurMemSpot=mCurrentStep-1;
            mMemNotes[mMemCount]=mToneRow[mCurrentStep];
            mPreMemSpot=mCurMemSpot;
            mMemCount+=1;
            mMemCount=mMemCount%12;
        } else {
            for (int i=0; i<12; i++) {
                mMemNotes[i]=0;
            }
            mMemCount=0;
            mCurMemSpot=mCurrentStep-1;
            mMemNotes[mMemCount]=mToneRow[mCurrentStep];
            mPreMemSpot=mCurMemSpot;
        }
        
        mToneRow[mCurrentStep]=0;
        mBackStep=false;
        
        cout<<"GOING BACK a step - current note is now: "<<mCurrentStep<<endl;
        cout<<"total steps is: "<<mTotalCount<<endl;
        int octave;
        for (int i=0; i<12; i++) {
            if (mToneRow[i]!=0){
                //changes int to midi note and octave notation
                int start = mToneRow[i]%12 * 2;
                octave=mToneRow[i]/12-1;
                string note=mNotes.substr(start,2);
                std::string::iterator end_pos = std::remove(note.begin(), note.end(), ' ');
                note.erase(end_pos, note.end());
                cout<<note<<octave<<" ";
            }
        }
        cout<<endl<<endl;
    } else {
        //check notes in tone row (check for octaves as well)
        for (int i = 0; i < 12; i++) {
            if (mToneRow[i]==note || mToneRow[i]==note+12 || mToneRow[i]==note-12 || mToneRow[i]==note+24 || mToneRow[i]==note-24) {
                safe = false;
            }
        }
    }
    
    return safe;
}

//make sure current interval is different than the last interval
bool Generator::checkIntervalSafe()
{    
    bool safe = true;
    
    if (mCurrentStep>1) {
        int mPrevInterval = abs(mToneRow[mCurrentStep-2]-mToneRow[mCurrentStep-3]);
        int mCurInterval = abs(mToneRow[mCurrentStep-1]-mToneRow[mCurrentStep-2]);
        if (mPrevInterval==mCurInterval) {
            safe = false;
        }
    }
    return safe;
}

string Generator::getNotes(){
    string notes;
    int octave;
    for (int i=0; i<12; i++) {
        if (mToneRow[i]!=0){
            int start = mToneRow[i]%12 * 2;
            octave=mToneRow[i]/12-1;
            string note=mNotes.substr(start,2);
            std::string::iterator end_pos = std::remove(note.begin(), note.end(), ' ');
            note.erase(end_pos, note.end());
            notes+=note;
            notes+=to_string(octave);
            notes+="-";
        }
    }
    string output = notes.substr(0,notes.size()-1);
    return output;
}