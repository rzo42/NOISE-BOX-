//========================================================================
//  PROJECT:NOISE BOX
//  AURTHOR:RYAN LISTON
//  TARGET:ARDUINO UNO R3
//  DATE:1/9/2022
//  DESCRIPTION:granular random tone generator
//  FEATURES:
//      generates random tones in granular samples
//      lfo shape generator 
//      8 lfo shapes:flat(none)/sine/square/triangle/ramp/saw/pulse/random
//      low tone cutoff control
//      high tone cutoff control 
//      lfo speed contol
//      lfo shape select button
//      sample speed (grain length)  control
//  PART LIST (all parts available in the Arduno Starter Kit):
//      1 Arduino Uno R3
//      1 breadboard
//      3 potentiometers
//      3 button switches
//      1 mosfet trasistor
//      1 piezo speaker
//      1 220 ohm resistor
//      3 10k ohm resistors
//      wires
//========================================================================

//PIN DEFINITIONS.........................................................

//analog input
#define spd_pot A0      //sample speed control potentiometer anaolog input @ pin A0  
#define hi_pot A1       //high end tone cut off ("low pass filtr") control potentiometer analog input @ pin A1
#define low_pot A2      //low end tone cut off ("low pass filtr") control potentiometer analog input @ pin A2
                      
//analog output
#define lfo_out 9       //lfo pwm output @ pin 9

//digital input
#define sp_down_b 4     //sample speed down  button digital input @ pin 4
#define sp_up_b 2       //sample speed up  button digital input @ pin 2 
#define shape_sel_b 7   //lfo shape selector button digital input @ pin 7
 
//tone output
#define tone_out 10     //tone output @ pin 10 

//VARIABLE DECLARATION...............................................................

const float _pi = 3.141593; //constant for pi 
unsigned int _low;          //low cuttoff level register
unsigned int _hi;           //high cutoff level register
unsigned int _spd;          //lfo speed level register
unsigned int sp_spd=0;      //sample length (speed) register
unsigned int _count=0;      //lfo clock timer
unsigned int lfo_shape=0;   //lfo shape register
float _volume=0;            //float for calculating lfo shape volume level
unsigned int _vol;          //regiser for lfo shape volume level

//------------------------------------------------------------------------

//INITIALIZE PINS FOR INPUT & OUTPUT

void setup() {
pinMode(low_pot,INPUT);pinMode(hi_pot,INPUT);pinMode(spd_pot,INPUT);pinMode(lfo_out,OUTPUT);
pinMode(shape_sel_b,INPUT);pinMode(sp_down_b,INPUT);pinMode(sp_up_b,INPUT);}

//------------------------------------------------------------------------

//MAIN LOOP......................................................................

void loop() {

//USER INPUT

//read potentiometers
_low=analogRead(low_pot)*7;       //low cuttoff knob
_hi=analogRead(hi_pot)*8;         //high cutoff knob
_spd=(analogRead(spd_pot)/4)+2;   //lfo speed knob

//shape select button 
if(digitalRead(shape_sel_b)==HIGH){if (lfo_shape==7){lfo_shape=0;}else{lfo_shape++;}delay(250);}

//sample length down button
if(digitalRead(sp_down_b)==HIGH&&sp_spd<256){sp_spd++;if(sp_spd>256){sp_spd=256;}delay(100);}

//sample length up button
if(digitalRead(sp_up_b)==HIGH&&sp_spd>0){sp_spd--;if(sp_spd<0||sp_spd>512){sp_spd=0;}delay(100);}
  
//LFO...............................................................

//lfo clock
if (_count<_spd){_count++;}else {_count=0;}

//LFO SHAPES
 
switch(lfo_shape){

//lfo_shape = 0 = off
case 0:_volume=255;break;

//lfo_shape = 1 = sine
case 1:_volume=128*sin(((2* _pi)/_spd)*_count)+127;break;

//lfo_shape = 2 = square
case 2:if(_count<_spd/2){_volume=255;}else{_volume=0;}break;

//lfo_shape = 3 = trangle
case 3:if(_count<_spd/2){_volume=_count*(256/(_spd+1))*2;}else{_volume=255-(_count*(256/(_spd+1))*2);}break;
  
//lfo_shape = 4 = saw
case 4:_volume=255-(_count*(255/(_spd+1)));break;

//lfo_shape = 5 = ramp  
case 5:_volume=_count*(256/(_spd+1));break;

//lfo_shape = 6 = pulse
case 6:if(_count<(_spd/10)+1){_volume=255;}else{_volume=0;}break;

//lfo_shape = 7 = random
case 7:_volume=random(0,255);break;}

//save current lfo volume level as integer  
_vol=_volume; 

//PLAY SAMPLE.................................................

analogWrite(lfo_out,_vol);        //send current lfo volume level
tone(tone_out,random(7161-_low,8184-_hi));    //calculate and send tone
delay(sp_spd);          //hold sample for a delay of <sp_speed> long 
}
//END MAIN LOOP................................................
//**************************************************************
