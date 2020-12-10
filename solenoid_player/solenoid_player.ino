
  // sets the Solenoid pin on arduino
const int solPin =  13;          

  // solState used to set the Solenoid
int solState = 0;             

  // will store last time Solenoid was updated
unsigned long previousMillis = 0;

  // contact struct
struct Contact
{
    // state of Solenoid
  int solState;
    // duration of contact in milliseconds
  int duration_ms; 

};

  // define each contact type
Contact shortTap = { 1, 100 };
Contact longTap = { 1, 200 };
Contact shortPause = { 0, 100};
Contact longPause = { 0, 200};
  
  // define sequence
Contact seq[] = { shortTap, shortPause, shortTap, longPause, longTap, shortPause, shortTap, shortPause, shortTap, shortPause };

void setup() {
  
  // set the digital pin as output:
  pinMode(solPin, OUTPUT);

}

void loop() {

    // store sequence length
  int seqLen = sizeof(seq) / sizeof(seq[0]);
  
    //Play sequence
  for(int i = 0; i<seqLen; i++)
  {
      // store current time
    unsigned long currentMillis = millis();
      // compares current time with last recorded time that Solenoid was updated
    if (currentMillis - previousMillis >= seq[i].duration_ms) {
        // set the Solenoid with the solState of the Contact
      digitalWrite(solPin, seq[i].solState);
      previousMillis = currentMillis;
    }
  }
}
