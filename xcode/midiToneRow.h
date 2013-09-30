//
//  midi.h
//  toneRow
//
//  Created by Aaron Sherwood on 9/29/13.
//
//
#pragma once
#include "MidiOut.h"

class PolyRhythm {
public:
    void                        setup();
    void                        getNotes(int notes[]);
    void                        playNotes();
    bool                        mPlay;
    bool                        mPlayOnce;
    int                         mSinglePlay;

    
private:
    cinder::midi::MidiOut       mMidiOut;
    int                         *mNotes;
    int                         mInversion[12];
    bool                        mTrigger;
    long long                   mMillis;
    int                         mCounter, mNoteCounter1, mNoteCounter2, mChangeNotesCounter;
    int                         mTempo;
    void                        openPort();
    void                        makeInversion();
    int                         randomModulo[4] = {7,5,3,2};
};

void PolyRhythm::openPort(){
    mMidiOut.openPort(0);
}

void PolyRhythm::getNotes(int notes[]){
    mNotes=notes;
    makeInversion();
}

void PolyRhythm::playNotes()
{
    //acts like a metro from max
    if (fabs(ci::app::getElapsedSeconds()*1000 - mMillis) >= mTempo)
        mTrigger=true;
    else
        mTrigger=false;
    
    if (mTrigger) {
        mCounter++;
        if (mPlay) {
            if (!mPlayOnce){
                if (mCounter%32==0){
                    randomModulo[0]=Rand::randInt(3)+6;
                    randomModulo[1]=Rand::randInt(3)+4;
                    randomModulo[2]=Rand::randInt(3)+2;
                    randomModulo[3]=Rand::randInt(3)+1;

                }
                if (mCounter%16==0){
                    mChangeNotesCounter=(mChangeNotesCounter+1)%32;
                }
                //advance notes
                //original row
                switch (mChangeNotesCounter) {
                    case 0:
                        mNoteCounter1=0;
                        break;
                    case 6:
                        mNoteCounter1=1;
                        break;
                    case 8:
                        mNoteCounter1=2;
                        break;
                    case 10:
                        mNoteCounter1=3;
                        break;
                    case 14:
                        mNoteCounter1=4;
                        break;
                    case 16:
                        mNoteCounter1=5;
                        break;
                    case 18:
                        mNoteCounter1=6;
                        break;
                    case 20:
                        mNoteCounter1=7;
                        break;
                    case 22:
                        mNoteCounter1=8;
                        break;
                    case 26:
                        mNoteCounter1=9;
                        break;
                    case 28:
                        mNoteCounter1=10;
                        break;
                    case 30:
                        mNoteCounter1=11;
                        break;
                        
                    default:
                        break;
                }
                //retrograde inversion
                switch (mChangeNotesCounter) {
                    case 2:
                        mNoteCounter2=11;
                        break;
                    case 4:
                        mNoteCounter2=10;
                        break;
                    case 10:
                        mNoteCounter2=9;
                        break;
                    case 12:
                        mNoteCounter2=8;
                        break;
                    case 16:
                        mNoteCounter2=7;
                        break;
                    case 18:
                        mNoteCounter2=6;
                        break;
                    case 20:
                        mNoteCounter2=5;
                        break;
                    case 22:
                        mNoteCounter2=4;
                        break;
                    case 24:
                        mNoteCounter2=3;
                        break;
                    case 28:
                        mNoteCounter2=2;
                        break;
                    case 30:
                        mNoteCounter2=1;
                        break;
                    case 8:
                        mNoteCounter2=0;
                        break;
                        
                    default:
                        break;
                }

                //original row
                if (mCounter%randomModulo[1]==0) {
                    mMidiOut.sendNoteOn(1,mNotes[mNoteCounter1],127);
                }
                //inversion
                if (mCounter%randomModulo[0]==0) {
                    mMidiOut.sendNoteOn(1,mInversion[mNoteCounter2],127);
                }
                //pedal tones
                if (mCounter%randomModulo[3]==0) {
                    mMidiOut.sendNoteOn(1,35,120);
                }
                if (mCounter%randomModulo[2]==0) {
                    mMidiOut.sendNoteOn(1,23,120);
                }
            } else {
                if (mSinglePlay<12) {
                    if (mCounter%4==0) {
                        mMidiOut.sendNoteOn(1,mNotes[mSinglePlay],127);
                        mSinglePlay++;
                    }
                } else {
                    mPlay=false;
                }
            }
        }
        mMillis += mTempo ;
    }
}

void PolyRhythm::setup()
{
    openPort();
    mMillis=ci::app::getElapsedSeconds()*1000;
    mTempo=120;
    mPlay=false;
    mPlayOnce=true;
    mTrigger=false;
    mCounter=mNoteCounter1=mChangeNotesCounter=0;
    mNoteCounter2=11;
    mSinglePlay=0;
}

void PolyRhythm::makeInversion()
{
    //start M3 up and do inverse of intervals
    mInversion[0]=mNotes[0]+4;
    for (int i=1; i<12; i++) {
        int interval=mNotes[i]-mNotes[i-1];
        mInversion[i]=mInversion[i-1]-interval;
    }
//    //print out inversion
//    int octave;
//    string str="C C#D D#E F F#G G#A A#B ";
//    for (int i=0; i<12; i++) {
//        int start = mInversion[i]%12 * 2;
//        octave=mInversion[i]/12-1;
//        string note=str.substr(start,2);
//        std::string::iterator end_pos = std::remove(note.begin(), note.end(), ' ');
//        note.erase(end_pos, note.end());
//        cout<<note<<octave<<" ";
//    }
    
}