const int invalidIndex = -1;

// Channel 1 variables
const int ch1_pin = 13; // channel 1 solenoid pin on arduino
unsigned long ch1_lastStepMillis_ms = 0; // timestamp for last step on channel 1
int ch1_currentIndex = invalidIndex; // current step being played. invalidIndex specifies that the sequence is not being played

// Channel 2 variables
const int ch2_pin =  12; // channel 2 solenoid pin on arduino
unsigned long ch2_lastStepMillis_ms = 0; // entry  timestamp for last step on channel 2
int ch2_currentIndex = invalidIndex; // current step being played. invalidIndex specifies that the sequence is not being played

struct Contact
{
  // State of Solenoid, 0 = deactive, 1 = active
  int solState;
  // Duration of state in milliseconds
  int duration_ms; 
};

// Define each contact type
Contact tap90 = { HIGH, 90 }; // Short Tap
Contact tap110 = { HIGH, 110 }; // Long Tap


Contact pause715 = { LOW, 715};
Contact pause560 = { LOW, 560};
Contact pause510 = { LOW, 510};
Contact pause500 = { LOW, 500};
Contact pause490 = { LOW, 500};
Contact pause450 = { LOW, 450};
Contact pause420 = { LOW, 450};
Contact pause410 = { LOW, 410};
Contact pause400 = { LOW, 400};
Contact pause350 = { LOW, 320};
Contact pause320 = { LOW, 320};
Contact pause245 = { LOW, 245};
Contact pause170 = { LOW, 170 };
Contact pause150 = { LOW, 150 };
Contact pause135 = { LOW, 135 };
Contact pause115 = { LOW, 115 };
  
// Define sequence for channel 1
Contact ch1_seq[] = { 
  tap110, pause400, 
  pause500, 
  tap110, pause450, 
  pause410, 
  tap110, pause135, 
  pause715, 
  tap110, pause400, 
  pause490 
  };
  
const int ch1_seqLen = sizeof(ch1_seq) / sizeof(Contact);

// Define sequence for channel 2
Contact ch2_seq[] = { 
  pause510, 
  tap90, pause150, 
  tap90, pause170, 
  pause560, 
  tap90, pause320, 
  pause245, 
  tap90, pause400, 
  tap90, pause135, 
  pause510, 
  tap90, pause400 
  };
  
const int ch2_seqLen = sizeof(ch2_seq) / sizeof(Contact);

// Button variables
const int btnPin = 2;

void setup()
{
  Serial.begin(9600);
  Serial.println("Starting Cojun player");
  // Set the digital pins for solenoids as output
  pinMode(ch1_pin, OUTPUT);
  pinMode(ch2_pin, OUTPUT);

  // Set the button pin
  pinMode(btnPin, INPUT);
}

void loop()
{
  // Store current milliseconds
  unsigned long currentMillis = millis();

  // Handle button
  if(digitalRead(btnPin) == 1)
  {
    // Button is pressed, reset channel variables and return
    Serial.println("Button pressed");

    // Deactivating solenoids
    digitalWrite(ch1_pin, LOW);
    digitalWrite(ch2_pin, LOW);

    ch1_currentIndex = 0;
    ch1_lastStepMillis_ms = currentMillis;
    ch2_currentIndex = 0;
    ch2_lastStepMillis_ms = currentMillis;

    return; // To prevent starting the sequence before releasing the button
  }
 
  // Handle channel 1
  if(ch1_currentIndex >= ch1_seqLen)
  {
    // Channel 1 sequence is finished, set current index to invalid state and deactivate solenoid
    Serial.println("Channel 1 sequence finished");
    ch1_currentIndex = invalidIndex;
    digitalWrite(ch1_pin, LOW);
  }

  if(ch1_currentIndex != invalidIndex)
  {
    // Channel 1 sequence is being played
    
    // Set correct pin state without condition
    digitalWrite(ch1_pin, ch1_seq[ch1_currentIndex].solState);

    // Tracks the timeout period
    unsigned long ch1_currentStateTimeout = ch1_lastStepMillis_ms + ch1_seq[ch1_currentIndex].duration_ms;
    if(currentMillis >= ch1_currentStateTimeout)
    {
      // Current step finished
      ch1_currentIndex++;

      if(ch1_currentIndex < ch1_seqLen)
      {
        Serial.print("Sequence 1 going to step ");
        Serial.println(ch1_currentIndex);
        ch1_lastStepMillis_ms = currentMillis;
      }
    }
  }
 
  // Handle channel 2
  if(ch2_currentIndex >= ch2_seqLen)
  {
    // Channel 2 sequence is finished, set current index to invalid state and deactivate solenoid
    Serial.println("Channel 2 sequence finished");
    ch2_currentIndex = invalidIndex;
    digitalWrite(ch2_pin, LOW);
  }

  if(ch2_currentIndex != invalidIndex)
  {
    // Channel 2 sequence is being played

    // Set correct pin state without condition
    digitalWrite(ch2_pin, ch2_seq[ch2_currentIndex].solState);

    unsigned long ch2_currentStateTimeout = ch2_lastStepMillis_ms + ch2_seq[ch2_currentIndex].duration_ms;
    if(currentMillis >= ch2_currentStateTimeout)
    {
      // Current step finished
      ch2_currentIndex++;

      if(ch2_currentIndex < ch2_seqLen)
      {
        Serial.print("Sequence 2 going to step ");
        Serial.println(ch2_currentIndex);
        ch2_lastStepMillis_ms = currentMillis;
      }
    }
  }
}
