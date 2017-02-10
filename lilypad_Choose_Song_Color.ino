#include "pitches_color.h"

//Deb's LilyPad board is "LilyPad Arduino" 

// switch switches on/off
// button switches to the start of the next song
// idea: each song is a 2D array of notes with their color components
// [i][0] - is note at position i
// [i][1] - is duration at position i
// [i][2] - is red component at position i
// [i][3] - is green component at position i
// [i][4] - is blue component at position i

class Note {
  int pitch;    // the note's pitch to play
  int duration; // length to play note 0,2,4,8

  //TODO: change this, this is a bad idea to store 3 ints for each node
  //      instead, look up the color for each note when playing the node
  //      more processing is required, but less memory is required for the note arrays
  int red;      // red component of tri-color
  int green;    // green component of tri-color
  int blue;     // blue component of tri-color
  
  public:
  Note( int pitch, int duration ) {
    this->pitch = pitch;
    this->duration = duration;

    switch (pitch) {
      case NOTE_C1: case NOTE_C2: case NOTE_C3: case NOTE_C4:
      case NOTE_C5: case NOTE_C6: case NOTE_C7: case NOTE_C8:
        red = 255; green = 0; blue = 0; break; // C
      case NOTE_CS1: case NOTE_CS2: case NOTE_CS3: case NOTE_CS4:
      case NOTE_CS5: case NOTE_CS6: case NOTE_CS7: case NOTE_CS8:
        red = 204; green = 102; blue = 0; break; // CS
      case NOTE_D1: case NOTE_D2: case NOTE_D3: case NOTE_D4:
      case NOTE_D5: case NOTE_D6: case NOTE_D7: case NOTE_D8:
        red = 200; green = 50; blue = 0; break; // D
      case NOTE_DS1: case NOTE_DS2: case NOTE_DS3: case NOTE_DS4:
      case NOTE_DS5: case NOTE_DS6: case NOTE_DS7: case NOTE_DS8:
        red = 102; green = 24; blue = 0; break; // DS
      case NOTE_E1: case NOTE_E2: case NOTE_E3: case NOTE_E4:
      case NOTE_E5: case NOTE_E6: case NOTE_E7: 
        red = 204; green = 150; blue = 0; break; // E
      case NOTE_F1: case NOTE_F2: case NOTE_F3: case NOTE_F4:
      case NOTE_F5: case NOTE_F6: case NOTE_F7: 
        red = 128; green = 255; blue = 0; break; // F
      case NOTE_FS1: case NOTE_FS2: case NOTE_FS3: case NOTE_FS4:
      case NOTE_FS5: case NOTE_FS6: case NOTE_FS7: 
        red = 0; green = 255; blue = 0; break; // FS
      case NOTE_G1: case NOTE_G2: case NOTE_G3: case NOTE_G4:
      case NOTE_G5: case NOTE_G6: case NOTE_G7: 
        red = 0; green = 255; blue = 255; break; // G
      case NOTE_GS1: case NOTE_GS2: case NOTE_GS3: case NOTE_GS4:
      case NOTE_GS5: case NOTE_GS6: case NOTE_GS7: 
        red = 0; green = 0; blue = 204; break; // GS
      case NOTE_A1: case NOTE_A2: case NOTE_A3: case NOTE_A4:
      case NOTE_A5: case NOTE_A6: case NOTE_A7: 
        red = 0; green = 0; blue = 102; break; // A
      case NOTE_AS1: case NOTE_AS2: case NOTE_AS3: case NOTE_AS4:
      case NOTE_AS5: case NOTE_AS6: case NOTE_AS7:
        red = 102; green = 0; blue = 102; break; // AS
      case NOTE_B0: case NOTE_B1: case NOTE_B2: case NOTE_B3: case NOTE_B4:
      case NOTE_B5: case NOTE_B6: case NOTE_B7: 
        red = 40; green = 0; blue = 40; break; // B
      default:
        this->red = 0;
        this->green = 0;
        this->blue = 0;
    }
  }
  int getPitch() { return pitch; }
  int getDuration() { return duration; }
  int getRed() { return red; }
  int getGreen() { return green; }
  int getBlue() { return blue; }
};

class Song {
  int numNotes;     // the number of notes must match number of durations
  Note** notes;
  int currNote; // TODO: this should be a pointer to an int in the array

  public:
  Song(int n, int pitch[], int duration[]) {
    this->notes = new Note *[75];
    numNotes = 0;
    for (int i=0; i < n; i++ ) {
      Note* note = new Note(pitch[i],duration[i]);
      this->notes[numNotes++] = note;
    }
    this->currNote = 0;
  }

  ~Song() {
    for (int i=0; i < numNotes; i++) {
      delete notes[i];
    }
    delete [] notes;
  }
  int getNumNotes() { return numNotes; }
  Note* nextNote() { 
    if (currNote >= numNotes) currNote = 0;
    return notes[currNote++];
  }
};

//button is on pinA5
int buttonPin = A5;

//the switch is on pin2
int switchPin = 2;

//the buzzer is on pin 7
int buzzer = 7;

//the led to light is on pin 5
int ledPin = 5;

// connect the tri-color pins
int redPin   =  9;  // R petal on RGB LED module connected to digital pin  9 
int greenPin = 11;  // G petal on RGB LED module connected to digital pin 11
int bluePin  = 10;  // B petal on RGB LED module connected to digital pin 10

int jingle_bells_n = 64;
int jingle_bells_pitch[] = {
NOTE_FS5,NOTE_FS5,NOTE_FS5,0,
NOTE_FS5,NOTE_FS5,NOTE_FS5,0,
NOTE_FS5,NOTE_A5,NOTE_D5,NOTE_E5,
NOTE_FS5,0,0,0,

NOTE_G5,NOTE_G5,NOTE_G5,NOTE_G5,
NOTE_G5,NOTE_FS5,NOTE_FS5,NOTE_FS5,
NOTE_FS5,NOTE_E5,NOTE_E5,NOTE_E5,
NOTE_FS5,0,NOTE_A5,0,

NOTE_FS5,NOTE_FS5,NOTE_FS5,0,
NOTE_FS5,NOTE_FS5,NOTE_FS5,0,
NOTE_FS5,NOTE_A5,NOTE_D5,NOTE_E5,
NOTE_FS5,0,0,0,

NOTE_G5,NOTE_G5,NOTE_G5,NOTE_G5,
NOTE_G5,NOTE_FS5,NOTE_FS5,NOTE_FS5,
NOTE_A5,NOTE_A5,NOTE_G5,NOTE_E5,
NOTE_D5,0,0,0
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int jingle_bells_duration[] = {
2,2,2,4,
2,2,2,4,
2,2,2,2,
2,4,4,4,

2,2,2,4,
2,2,2,4,
2,2,2,2,
2,4,4,4,

2,2,2,4,
2,2,2,4,
2,2,2,2,
2,4,4,4,

2,2,2,4,
2,2,2,4,
2,2,2,2,
2,4,4,4
};

int frozen_n = 75; // number of notes in frozen's letitgo
// we'll store all the notes for 'let it go' in a list:
int frozen_pitch[] = {
NOTE_F4, NOTE_G4, NOTE_GS4, NOTE_GS4, 0, NOTE_DS4, NOTE_DS4, NOTE_AS4, NOTE_AS4, 0, NOTE_GS4, NOTE_F4, 

//Let it go -- let it go. -- Can't
NOTE_F4, NOTE_F4, NOTE_F4, NOTE_F4, NOTE_G4, NOTE_AS4, NOTE_AS4, 0, NOTE_F4, NOTE_G4, NOTE_GS4, NOTE_GS4, 0, NOTE_DS4, 

NOTE_C5, NOTE_C5, 
//hold it back an-y more--  Let it go -- let it go-----
NOTE_AS4, 0, NOTE_GS4, NOTE_AS4, NOTE_C5, NOTE_C5, NOTE_CS5, NOTE_C5, NOTE_AS4, NOTE_GS4, NOTE_AS4, 

NOTE_GS4, 0, 
//-- Turn a-way- and slam - the - door
NOTE_DS5, NOTE_C5, NOTE_C5, NOTE_C5, NOTE_AS4, 0, NOTE_GS4, NOTE_GS4, NOTE_DS5, NOTE_DS5, 0, NOTE_C5, NOTE_C5, 

NOTE_GS4, 
//I - don't care - What they're going to - say --
0, 0, NOTE_GS4, NOTE_GS4, NOTE_G4, 0, NOTE_DS4, NOTE_DS4, NOTE_DS4, 0, NOTE_CS4, 
//-- let the storm rage on -- The
NOTE_CS4, NOTE_CS4, NOTE_C4, NOTE_CS4, NOTE_C4, NOTE_CS4, NOTE_CS4, NOTE_C4, NOTE_GS3, 
//cold never bothered me anyway.
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int frozen_duration[] = {
8, 8, 8, 2, 8, 8, 8, 8, 2, 4, 8, 8, 
8, 8, 4, 8, 4, 8, 2, 8, 8, 8, 8, 2, 8, 8, 8, 8, 
2, 8, 8, 8, 4, 4, 4, 8, 8, 8, 8, 4, 2, 2, 
4, 8, 8, 4, 2, 4, 4, 4, 4, 8, 8, 4, 2, 
4, 4, 8, 8, 4, 8, 8, 4, 1, 8, 8, 
4, 8, 8, 8, 8, 8, 8, 8, 4};

int numSongs = 2;  // the number of songs to choose from
int songIndex = 0; // play the first song first
Song** song = new Song*[numSongs];       // array of songs { frozen, hotcrossbuns};
//TODO: check the actual size of this array
// 2x75x5x(2 bytes) = 150*5*2= 750*2= 1500k (must be well under 2k=may not have enough room for 1500k here)

int buttonValue = HIGH;  // unpressed, LOW is pressed

void setup() { 
  Serial.begin(9600);

  // Tri-color
  pinMode(redPin, OUTPUT);    // sets the redPin to be an output
  pinMode(greenPin, OUTPUT);  // sets the greenPin to be an output
  pinMode(bluePin, OUTPUT);   // sets the bluePin to be an input

  //Button turns on LED5
  pinMode(ledPin, OUTPUT);       // sets the led to be an output
  pinMode(buttonPin, INPUT);     // set the button as an input
  digitalWrite(buttonPin, HIGH); // enable the pull-up resistor

  //Switch
  pinMode(switchPin, INPUT);     // set the button as an input
  digitalWrite(switchPin, HIGH); // enable the pull-up resistor

  //Buzzer
  pinMode(buzzer, OUTPUT);    // set the buzzer pins as outputs

  //Song list
  song[0] = new Song( jingle_bells_n, jingle_bells_pitch, jingle_bells_duration);
  //TODO: fix memory usage and references so that songs fit in memory
  //      and can be chosen by push button
  //song[1] = new Song(       frozen_n,       frozen_pitch,       frozen_duration);
  Serial.print("End of setup");
}

void loop() {
  color(0,0,0); // turn color off

  //if the button is pressed, light LED (and switch song)
  if (digitalRead(buttonPin)==LOW) { // pressed
    //Serial.println("button pressed");
    songIndex = songIndex+1;
    if (songIndex >= numSongs) songIndex=0;
    Serial.print(songIndex);
    Serial.print(" ");
    digitalWrite(ledPin,HIGH);
  } else {
    digitalWrite(ledPin,LOW);
  }
  
  //if the switch is on, play the song once
  if(digitalRead(switchPin) == 0)
  {  
    //for every note in the song, play it for the proper length and set the color
      //for (int thisNote = 0; thisNote < 8 ; thisNote++) {

      Note* note = (song[songIndex])->nextNote();
  
      // to calculate the note duration, take one second (1000)
      //divided by the note type.
      //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
      int noteDuration = 1000/note->getDuration();
      color(note->getRed(),note->getGreen(),note->getBlue());
      tone(buzzer, note->getPitch(), noteDuration);
  
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 20% seems to work well:
      int pauseBetweenNotes = noteDuration * 1.2;
      delay(pauseBetweenNotes);
      
      // stop the tone playing:
      noTone(buzzer);
      //}
  }

  // Turn the color off after the song stops
  color(0,0,0);
  
}

void color (unsigned char red, unsigned char green, unsigned char blue)   //the color generating function 
{    
     analogWrite(redPin, 255-red);   
     analogWrite(greenPin, 255-green);       
     analogWrite(bluePin, 255-blue);   
}

//code adapted from http://www.instructables.com/id/Musical-Snow-Globe/
