



//***********************************************************************************
//*********************************MOUTHS DEFINES************************************
//***********************************************************************************

#define smile_code                 0b00000000100001010010001100000000
#define sad_code             	   0b00000000001100010010100001000000
#define happyOpenMouth_code        0b00000000111111010010001100000000 
#define happyClosedMouth_code      0b00000000111111011110000000000000
#define sadOpenMouth_code          0b00000000001100010010111111000000
#define sadClosedMouth_code        0b00000000001100011110110011000000
#define vamp1_code                 0b00111111101101101101010010000000
#define vamp2_code                 0b00111111101101010010000000000000
#define bigSurprise_code           0b00001100010010100001010010001100
#define smallSurprise_code         0b00000000000000001100001100000000
#define confused_code              0b00000000001000010101100010000000
#define tongueOut_code             0b00000000111111000101000101000010
#define heart_code                 0b00010010101101100001010010001100
#define lineMouth_code             0b00000000000000111111000000000000
#define diagonalMouth_code         0b00100000010000001000000100000010

#define okMouth_code               0b00000001000010010100001000000000
#define xMouth_code                0b00100001010010001100010010100001
#define interrogation_code         0b00001100010010000100000100000100

#define zero_code                  0b00001100010010010010010010001100
#define one_code                   0b00000100001100000100000100001110
#define two_code                   0b00001100010010000100001000011110
#define three_code                 0b00001100010010000100010010001100
#define four_code                  0b00010010010010011110000010000010
#define five_code                  0b00011110010000011100000010011100
#define six_code                   0b00000100001000011100010010001100
#define seven_code                 0b00011110000010000100001000010000
#define eight_code                 0b00001100010010001100010010001100
#define nine_code                  0b00001100010010001110000010001110


#define thunder_code               0b00000100001000011100001000010000
#define culito_code                0b00000000100001101101010010000000




//Mouths sorted by numbers, and after, by happy to sad mouths
#define zero 				0
#define one  				1                
#define two     			2               
#define three               3  
#define four                4
#define five                5 
#define six                 6 
#define seven               7  
#define eight               8 
#define nine   				9
#define smile               10                  
#define happyOpenMouth      11  
#define happyClosedMouth    12  
#define heart      			13
#define bigSurprise         14  
#define smallSurprise 		15
#define tongueOut 			16
#define vamp1               17  
#define vamp2               18  
#define lineMouth        	19
#define confused            20  
#define diagonalMouth       21          
#define sad         		22
#define sadOpenMouth        23 
#define sadClosedMouth  	24
         
#define okMouth             25 
#define xMouth              26
#define interrogation       27

#define thunder		       	28
#define culito       		29
               
               

unsigned long int mouthType[]={zero_code,one_code,two_code,three_code,four_code,five_code,six_code,seven_code,eight_code,
	nine_code,smile_code,happyOpenMouth_code,happyClosedMouth_code,heart_code,bigSurprise_code,smallSurprise_code,tongueOut_code,
	vamp1_code,vamp2_code,lineMouth_code,confused_code,diagonalMouth_code,sad_code,sadOpenMouth_code,sadOpenMouth_code,
	okMouth_code, xMouth_code,interrogation_code,thunder_code,culito_code};



//**ANIMATIONS***
unsigned long int adivinawi[]={
		0b00100001000000000000000000100001,
		0b00010010100001000000100001010010,
     	0b00001100010010100001010010001100,
        0b00000000001100010010001100000000,
        0b00000000000000001100000000000000,
        0b00000000000000000000000000000000
};


unsigned long int littleUuh[]={
		0b00000000000000001100001100000000,
		0b00000000000000000110000110000000,
		0b00000000000000000011000011000000,
		0b00000000000000000110000110000000,
		0b00000000000000001100001100000000,
		0b00000000000000011000011000000000,
		0b00000000000000110000110000000000,
		0b00000000000000011000011000000000
		
};



unsigned long int wave[]={
		0b00001100010010100001000000000000,
		0b00000110001001010000100000000000,
		0b00000011000100001000010000100000,
		0b00000001000010000100001000110000,
		0b00000000000001000010100100011000,
		0b00000000000000100001010010001100,
		0b00000000100000010000001001000110,
		0b00100000010000001000000100000011,
		0b00110000001000000100000010000001,
		0b00011000100100000010000001000000
			
};



unsigned long int zzzZ[]={
		0b00011111000010000100001000011111,
		0b00000000001111000010000100001111,
		0b00000000000000000111000010000111,
		0b00000000000000000000000011000011
				
};

unsigned long int dreamMouth[]={
		0b00000000000000000000110000110000,
		0b00000000000000010000101000010000,  //igual
		0b00000000011000100100100100011000,
		0b00000000000000010000101000010000   //igual
				
};


unsigned long int spiral[]={
		0b00111110100000101110100010111110,  //G
		0b00101110101010101010100010111110,
		0b00111110100010111010000010111110,
		0b00111110100010101010101010111010
				
};	

