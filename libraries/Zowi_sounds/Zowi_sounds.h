



//***********************************************************************************
//*********************************SOUNDS DEFINES************************************
//***********************************************************************************           


#define PIN_Buzzer 10


// Reference:  This list was adapted from the table located here:
// 		http://www.phy.mtu.edu/~suits/notefreqs.html
#define  note_C0  16.35  //C0
#define  note_Db0 17.32  //C#0/Db0
#define  note_D0  18.35  //D0
#define  note_Eb0 19.45 //D#0/Eb0
#define  note_E0  20.6  //E0
#define  note_F0  21.83  //F0
#define  note_Gb0 23.12  //F#0/Gb0
#define  note_G0  24.5  //G0
#define  note_Ab0 25.96  //G#0/Ab0
#define  note_A0  27.5  //A0
#define  note_Bb0 29.14  //A#0/Bb0
#define  note_B0  30.87  //B0
#define  note_C1  32.7  //C1
#define  note_Db1 34.65  //C#1/Db1
#define  note_D1  36.71  //D1
#define  note_Eb1 38.89  //D#1/Eb1
#define  note_E1  41.2  //E1
#define  note_F1  43.65  //F1
#define  note_Gb1 46.25  //F#1/Gb1
#define  note_G1  49 //G1
#define  note_Ab1 51.91  //G#1/Ab1
#define  note_A1  55  //A1
#define  note_Bb1 58.27  //A#1/Bb1
#define  note_B1  61.74  //B1
#define  note_C2  65.41  //C2 (Middle C)
#define  note_Db2 69.3  //C#2/Db2
#define  note_D2  73.42  //D2
#define  note_Eb2 77.78  //D#2/Eb2
#define  note_E2  82.41  //E2
#define  note_F2  87.31  //F2
#define  note_Gb2 92.5  //F#2/Gb2
#define  note_G2  98  //G2
#define  note_Ab2 103.83  //G#2/Ab2
#define  note_A2  110  //A2
#define  note_Bb2 116.54  //A#2/Bb2
#define  note_B2  123.47  //B2
#define  note_C3  130.81  //C3
#define  note_Db3 138.59  //C#3/Db3
#define  note_D3  146.83  //D3
#define  note_Eb3 155.56  //D#3/Eb3
#define  note_E3  164.81  //E3
#define  note_F3  174.61  //F3
#define  note_Gb3 185  //F#3/Gb3
#define  note_G3  196  //G3
#define  note_Ab3 207.65  //G#3/Ab3
#define  note_A3  220  //A3
#define  note_Bb3 233.08  //A#3/Bb3
#define  note_B3  246.94  //B3
#define  note_C4  261.63  //C4
#define  note_Db4 277.18  //C#4/Db4
#define  note_D4  293.66  //D4
#define  note_Eb4 311.13  //D#4/Eb4
#define  note_E4  329.63  //E4
#define  note_F4  349.23  //F4
#define  note_Gb4 369.99  //F#4/Gb4
#define  note_G4  392  //G4
#define  note_Ab4 415.3  //G#4/Ab4
#define  note_A4  440  //A4
#define  note_Bb4 466.16  //A#4/Bb4
#define  note_B4  493.88  //B4
#define  note_C5  523.25  //C5
#define  note_Db5 554.37  //C#5/Db5
#define  note_D5  587.33  //D5
#define  note_Eb5 622.25  //D#5/Eb5
#define  note_E5  659.26  //E5
#define  note_F5  698.46  //F5
#define  note_Gb5 739.99  //F#5/Gb5
#define  note_G5  783.99  //G5
#define  note_Ab5 830.61  //G#5/Ab5
#define  note_A5  880  //A5
#define  note_Bb5 932.33  //A#5/Bb5
#define  note_B5  987.77  //B5
#define  note_C6  1046.5  //C6
#define  note_Db6 1108.73  //C#6/Db6
#define  note_D6  1174.66  //D6
#define  note_Eb6 1244.51  //D#6/Eb6
#define  note_E6  1318.51  //E6
#define  note_F6  1396.91  //F6
#define  note_Gb6 1479.98  //F#6/Gb6
#define  note_G6  1567.98  //G6
#define  note_Ab6 1661.22  //G#6/Ab6
#define  note_A6  1760  //A6
#define  note_Bb6 1864.66  //A#6/Bb6
#define  note_B6  1975.53  //B6
#define  note_C7  2093  //C7
#define  note_Db7 2217.46  //C#7/Db7
#define  note_D7  2349.32  //D7
#define  note_Eb7 2489.02  //D#7/Eb7
#define  note_E7  2637.02  //E7
#define  note_F7  2793.83  //F7
#define  note_Gb7 2959.96  //F#7/Gb7
#define  note_G7  3135.96  //G7
#define  note_Ab7 3322.44  //G#7/Ab7
#define  note_A7  3520  //A7
#define  note_Bb7 3729.31  //A#7/Bb7
#define  note_B7  3951.07  //B7
#define  note_C8  4186.01  //C8
#define  note_Db8 4434.92  //C#8/Db8
#define  note_D8  4698.64  //D8
#define  note_Eb8 4978.03  //D#8/Eb8


               
void ZowiTone (int speakerPin, float noteFrequency, long noteDuration, int silentDuration)
{

      tone(speakerPin,noteFrequency,noteDuration);
      delayMicroseconds(noteDuration*1000);       //milliseconds to microseconds
      //noTone(speakerPin);
      delayMicroseconds(silentDuration*1000);     //milliseconds to microseconds

} 

void ZowiTone_OLD(int speakerPin, float noteFrequency, long noteDuration, long silentDuration)
{    
  int x;
  // Convert the frequency to microseconds
  float microsecondsPerWave = 1000000/noteFrequency;
  // Calculate how many HIGH/LOW cycles there are per millisecond
  float millisecondsPerCycle = 1000/(microsecondsPerWave * 2);
  // Multiply noteDuration * number or cycles per millisecond
  float loopTime = noteDuration * millisecondsPerCycle;
  // Play the note for the calculated loopTime.
  for (x=0;x<loopTime;x++)   
  {   
              digitalWrite(speakerPin,HIGH); 
              delayMicroseconds(microsecondsPerWave); 
              digitalWrite(speakerPin,LOW); 
              delayMicroseconds(microsecondsPerWave); 
  }
  delay(silentDuration); 
}  


void ZowiMelody(float melodyNotes[], int melodySize){

     for (int i=0;i<=melodySize;i=i+3) //por cada "fila"
     {
        ZowiTone(PIN_Buzzer, melodyNotes[i],melodyNotes[i+1],melodyNotes[i+2]);
     }

}

//Example melody code:
//    int sssize = (sizeof(connectionNotes)/sizeof(float));
//    ZowiMelody(connectionNotes, sssize);


 


//-- MELODIES VECTORS
//--------------------------------------------------------
float connectionNotes[]={
		note_E6,50,30,   //Note, NoteDuration, SilentDuration
		note_E7,50,30,
		note_A7,50,30
};

float disconnetionNotes[]={
		note_E6,50,30,   //Note, NoteDuration, SilentDuration
		note_A7,50,30,
		note_E7,50,30
};


float fart_6_Notes[]={
		note_C1,90,0,   //Note, NoteDuration, SilentDuration
		note_E1,90,0,
		note_D2,80,0,
		note_F2,80,0,
		note_E3,85,0,
		note_G3,85,0,
		note_F4,90,0,
		note_A3,95,0,
		note_C3,200,0
};



//-- SOUNDS FUNCTIONS
//--------------------------------------------------------
void S_connection()
{
         ZowiTone(PIN_Buzzer, note_E6,50,30);
         ZowiTone(PIN_Buzzer, note_E7,50,30);
         ZowiTone(PIN_Buzzer, note_A7,50,30);
}

void S_disconnection()
{
         ZowiTone(PIN_Buzzer, note_E6,50,30);
         ZowiTone(PIN_Buzzer, note_A7,50,30);
         ZowiTone(PIN_Buzzer, note_E7,50,30);
}


void S_happy() 
{
		for (int i=2349; i<3698; i=i*1.05) {
            ZowiTone(PIN_Buzzer,i,8,8);
       }

       for (int i=3698; i>2349; i=i/1.05) {
            ZowiTone(PIN_Buzzer,i,15,8);
       }

        for (int i=3349; i<4978; i=i*1.05) {
            ZowiTone(PIN_Buzzer,i,8,8);
       }

       for (int i=4978; i>3349; i=i/1.05) {
            ZowiTone(PIN_Buzzer,i,15,8);
       } 
}	

void S_superHappy()
{
	    for (int i=2000; i<6000; i=i*1.05) {
          ZowiTone(PIN_Buzzer,i,8,0);
        }
        delay(50);
        for (int i=6000; i>2000; i=i/1.05) {
          ZowiTone(PIN_Buzzer,i,15,0);
        }
}

void S_sad() 
{          
        ZowiTone(PIN_Buzzer, note_G6,20,0);
        for (int i=1567; i<1768; i=i*1.02) {
        	ZowiTone(PIN_Buzzer,i,15,0);
        }
        delay(10);
        for (int i=1768; i>698; i=i/1.02) {
        	ZowiTone(PIN_Buzzer,i,20,0);
        }
        ZowiTone(PIN_Buzzer, note_F5,70,0);
        ZowiTone(PIN_Buzzer, note_E5,50,0);
        ZowiTone(PIN_Buzzer, note_D5,30,0);
}


void S_confused() 
{
      
      for (int i=2000; i<3500; i=i*1.02) {
          ZowiTone(PIN_Buzzer,i,8,0);
      } 

      for (int i=3500; i>1000; i=i/1.02) {
          ZowiTone(PIN_Buzzer,i,8,0);
      }

      for (int i=1000; i<4000; i=i*1.05) {
          ZowiTone(PIN_Buzzer,i,8,10);
      } 


}  

void S_question()
{
          ZowiTone(PIN_Buzzer, note_C6,100,100); //1046.5

          for (int i=1046; i<1568; i=i*1.02) {  //1567.98  //G6
            ZowiTone(PIN_Buzzer,i,12,0);
          }  
          delay(20);

          ZowiTone(PIN_Buzzer, note_G6,100,50); //1567.98  //G6

          for (int i=1568; i>1046; i=i/1.02) {  
            ZowiTone(PIN_Buzzer,i,12,0);
          }
          delay(30);

          for (int i=1046; i>784; i=i/1.02) {  //783.99  //G5
            ZowiTone(PIN_Buzzer,i,12,0);
          }
          delay(20);


          ZowiTone(PIN_Buzzer, note_G5,100,50); //783.99  //G5


          for (int i=784; i<1567; i=i*1.02) { //G6
                  ZowiTone(PIN_Buzzer,i,10,0);
          } 

}


void S_lowAlarm()
{
		for (int i=2349; i<3520; i=i*1.02) {
          	ZowiTone(PIN_Buzzer,i,12,0);
        }
        delay(30);
        for (int i=2220; i<3720; i=i*1.02) {
          	ZowiTone(PIN_Buzzer,i,10,0);
        }
}          


void S_sleeping() //Ronquido
{
    for (int i=100; i<500; i=i*1.04) {
            ZowiTone(PIN_Buzzer,i,10,10);
        }
        delay(500);

        for (int i=400; i>100; i=i/1.04) {
            ZowiTone(PIN_Buzzer,i,10,0);
        }

}  


void S_surprise() 
{          
      for (int i=1800; i<3500; i=i*1.02) {
        ZowiTone(PIN_Buzzer,i,18,0);
      }
      for (int i=3500; i>1800; i=i/1.02) {
        ZowiTone(PIN_Buzzer,i,13,0);
      }
}

void S_OhOoh() 
{
      
      for (int i=880; i<2000; i=i*1.04) {  //A5 = 880
          ZowiTone(PIN_Buzzer,i,8,3);
      } 
      delay(200);

      for (int i=880; i<2000; i=i*1.04) {
          ZowiTone(PIN_Buzzer,note_B5,5,10);
      }
}  

void S_OhOoh2() 
{
      
      for (int i=1880; i<3000; i=i*1.03) {  //A5 = 880
          ZowiTone(PIN_Buzzer,i,8,3);
      } 
      delay(200);

      for (int i=1880; i<3000; i=i*1.03) {
          ZowiTone(PIN_Buzzer,note_C6,10,10);
      }
} 