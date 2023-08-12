#include <ErriezLCDKeypadShield.h>
LCDKeypadShield shield;
const int testcount=50;
const byte interruptPin = 2; // Input from the IR phototransistor.
int ResetButton = 1; // An additional push-button for zeroing measurements

float s_shutters[19]={0.000125,0.00025,0.0005,0.001,0.002,0.004,0.008,0.0166,0.03333,0.06666,0.125,0.25,0.5,1,2,4,8,15,30};
char *d_shutters[19]={"1/8000","1/4000","1/2000","1/1000","1/500","1/250","1/125","1/60","1/30","1/15","1/8","1/4","1/2","1","2","4","8","15","30"};
float shutters[50]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
float average=0,sum=0,ev=0;
int i=0,j=0,k=0,Button=0,m=0;
long n=0;
boolean flag=0;
char mode=0;


void setup()
{
TCCR1A=0x00;
TCCR1B=0x00;
TCNT1H=0x00;
TCNT1L=0x00;
ICR1H=0x00;
ICR1L=0x00;
OCR1AH=0x00;
OCR1AL=0x00;
OCR1BH=0x00;
OCR1BL=0x00;
TIMSK1=(0<<ICIE1) | (0<<OCIE1B) | (0<<OCIE1A) | (1<<TOIE1);
  

 pinMode(10,OUTPUT);
 digitalWrite(10, HIGH);
 pinMode(ResetButton, INPUT_PULLUP);
 shield.setCursor(1,1);
 shield.print(F("Shutter Tester"));
 delay(1500);
 shield.clear();
 pinMode(interruptPin, INPUT_PULLUP);
 attachInterrupt(digitalPinToInterrupt(interruptPin), shutter_trigger , CHANGE);
interrupts();  
}


ISR(TIMER1_OVF_vect)       
{
 n++;
}

void loop()
{

lcd();
      if(digitalRead(ResetButton)==0)
      {
        delay(50);
        if(digitalRead(ResetButton)==0) Reset();
      }
      while(digitalRead(ResetButton)==0);
       Button=shield.getButtons();
       delay(50);
       if(shield.getButtons()==Button)
       {
              switch (shield.getButtons()) {
        case ButtonRight: //Averaging Mode
        mode=1;
            break;
        case ButtonLeft: //Normal Mode
        mode=0;
            break;
        case ButtonUp:
            if(i>0) i--;
            break;
        case ButtonDown:
            if(i<(testcount-1)) i++; 
            break;
        case ButtonSelect: //Deviation calculation mode
      mode=2;
            break;
        default:
            break;
    } 
       }
lcd();
  
       while(shield.getButtons()!=ButtonNone);


 }

void Reset(){ //For Zeoring the measurements
 noInterrupts(); 
 TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
  n=0;
  TCNT1= 0;
  flag=0;
  j=0;k=0;Button=0;m=0;mode=0;
  for(i=0;i<50;i++){shutters[i]=0;}
  i=0;
  lcd();
 interrupts();  
}

void lcd(){   //Main lcd function dealing with displaying information based on the current mode
    if(mode==0) {
  shield.setCursor(0, 0);
  shield.print(F("T"));
  shield.print(i+1);
  shield.print(F(": "));
  shield.print(shutters[i],0);
  shield.print(F(" uS             ")); 
  shield.setCursor(0, 1);
  print_shutter(shutters[i]);
  }
  else if(mode==1){
  shield.setCursor(0, 0);
  shield.print(F("T"));
  shield.print(i+1);
  shield.print(F(": "));
  print_shutter(shutters[i]);
  shield.setCursor(0, 1);  
           
            for(k=0;k<testcount;k++)
               {
                  if(shutters[k]!=0) {m++; sum=sum+shutters[k];}
                }
               average=sum/m;
                 m=0;
                 sum=0;
                 shield.print(F("Av: "));
                  print_shutter(average);    
  }
  else {
  shield.setCursor(0, 0);
  shield.print(F("T"));
  shield.print(i+1);
  shield.print(F(": "));
  print_shutter(shutters[i]);
  shield.setCursor(0, 1);
    j=round(log(shutters[i]/125)/log(2));
    ev=log(shutters[i]/1000000/s_shutters[j])/log(2);

   shield.print(d_shutters[j]);shield.print(F("S ")); 
   if(ev>0) shield.print(F("+")); 
   if(abs(ev)==float(-0)) shield.print(0); else shield.print(ev);
   shield.print(F("Ev   "));
  }
}


void print_shutter(float i){  //Displays each measurement
  float shutter=0;
  if(i<=500000 && i!=0) {
    shutter = 1000000/i;
    shield.print(F("1/"));
    shield.print(shutter);
    shield.print(F("S           "));
    }
  else
  {
  shutter = i/1000000;
  shield.print(shutter);
  shield.print(F("S           ")); }
}

void shutter_trigger(){   //Activates when a change on interrupt pin is seen
  flag=!flag;
  if(flag==1){
  n=0;
   TCNT1= 0;
  TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (1<<CS11) | (0<<CS10);
  }
  else{
    
    TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
    noInterrupts(); 
    for(j=0;j<testcount;j++) if(shutters[j]==0) break;
    if(j<testcount) { shutters[j]=float(n*65536+TCNT1)/2;
     i=j;
    }
    interrupts(); 
  }
}


     


 
