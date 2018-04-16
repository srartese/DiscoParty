/* Sara Artese
  This progeam takes feedback from the mic for clapping and
  turns on a disco party.
*/

// MUSIC VARIABLES
#define speakerPin 8
// Tempo (beats per minute)
#define bpm 200
// Gap between notes. This is expressed as percentage of time between 2 beats.
#define noteGapPercentage 2
// Time between two beats in microseconds (equal to length of a quarter note)
#define beatDuration (60.0 / bpm) * 1000000L
// Time of the gap between two notes in microseconds
#define noteGap beatDuration * (noteGapPercentage / 100.0)


uint8_t notes[][2] = {
  {35, 4}, {30, 4}, {40, 4}, {42, 4}, {46, 4}, {44, 4}, {42, 2},
  {40, 4}, {45, 4}, {39, 4}, {33, 4}, {32, 4}, {37, 4}, {35, 2},
  {42, 4}, {46, 4}, {48, 4}, {30, 4}, {39, 4}, {40, 4}, {37, 2},
  {45, 4}, {49, 4}, {40, 4}, {40, 4}, {37, 4}, {39, 4}, {37, 2},
  {40, 4}, {45, 4}, {39, 4}, {33, 4}, {32, 4}, {37, 4}, {35, 2},
  {42, 4}, {46, 4}, {48, 4}, {30, 4}, {39, 4}, {40, 4}, {37, 2}
};

// MIC VARIABLES
const int sampleWindow = 50;
unsigned int sample;

//LED VARIABLES
const int ledRed =  12;
const int ledGreen = 2;
const int ledYellow = 4;
const int ledGreen2 = 11;
const int ledRed2 = 7;

// OTHER VARIABLES
boolean partyStarted = false;
int randNumber;

void setup() {
  Serial.begin(9600);
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  pinMode(ledGreen2, OUTPUT);
  pinMode(ledRed2, OUTPUT);

  pinMode(speakerPin, OUTPUT);
}


void loop() {
  clapSensor();
}

void clapSensor() {
  unsigned long startMillis = millis(); // Start of sample window
  unsigned int peakToPeak = 0;   // peak-to-peak level

  unsigned int signalMax = 0;
  unsigned int signalMin = 1024;

  // collect data for 50 mS
  while (millis() - startMillis < sampleWindow)
  {
    sample = analogRead(0);
    if (sample < 1024)  // toss out spurious readings
    {
      if (sample > signalMax)
      {
        signalMax = sample;  // save just the max levels
      }
      else if (sample < signalMin)
      {
        signalMin = sample;  // save just the min levels
      }
    }
  }
  peakToPeak = signalMax - signalMin;  // max - min = peak-peak amplitude
  double volts = (peakToPeak * 5.0) / 1024;  // convert to volts

  Serial.println(signalMax);

  if (signalMax > 650)
    checkParty();
}

void checkParty() {
  if (partyStarted == false) {
    partyStarted =  true;
    startParty();
  }
  else
    endParty();
}

void startParty() {
  // Iterate over the notes array
  while (partyStarted == true) {
    for (int i = 0; i < (sizeof(notes) / sizeof(*notes)); i++) {
      clapSensor();
      playNote(notes[i][0], notes[i][1]);
      discoLights();
      if (partyStarted == false)
        break;
    }
  }
  turnOffLights();
}

void endParty() {
   partyStarted = false;
}

void turnOffLights(){
    digitalWrite(ledRed, LOW);
    digitalWrite(ledGreen, LOW);
    digitalWrite(ledYellow, LOW);
    digitalWrite(ledRed2, LOW);
    digitalWrite(ledGreen2, LOW);
}

void discoLights() {
      // turn all LEDs off
  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, LOW);
  digitalWrite(ledYellow, LOW);
  digitalWrite(ledRed2, LOW);
  digitalWrite(ledGreen2, LOW); 
  randNumber = random(10);
  
  if (randNumber == 0) {
    // turn LED on:
 digitalWrite(ledRed, HIGH);
  } 
   if (randNumber == 2) {
    // turn LED on:
    digitalWrite(ledYellow, HIGH);
  }
   if (randNumber == 1) {
    // turn LED on:
    digitalWrite(ledGreen, HIGH);
  }
  if (randNumber == 3) {
    // turn LED on:
     digitalWrite(ledRed2, HIGH);
  }
  if (randNumber == 4) {
    // turn LED on:
    digitalWrite(ledGreen2, HIGH);
  }
  if (randNumber == 5) {
    // turn LED on:
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledRed, HIGH);
  }
   if (randNumber == 6) {
    // turn LED on:
    digitalWrite(ledGreen2, HIGH);
    digitalWrite(ledRed, HIGH);
  }
    if (randNumber == 7) {
    // turn LED on:
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledRed, HIGH);
  }
   if (randNumber == 8) {
    // turn LED on:
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledRed, HIGH);
     digitalWrite(ledGreen2, HIGH);
    digitalWrite(ledRed, HIGH);
  }
  if (randNumber == 9) {
    // turn LED on:
    digitalWrite(ledGreen, HIGH);
    digitalWrite(ledYellow, HIGH);
    digitalWrite(ledGreen2, HIGH);
    digitalWrite(ledRed, HIGH);
  }
}

void playNote(uint8_t keyNumber, uint8_t noteType) {
  long halfPeriod = getPeriodForKey(keyNumber) / 2;
  long noteDuration = beatDuration * (4.0 / noteType);
  long elapsed = 0;

  // While we have a note to play
  while (halfPeriod > 0 && elapsed < (noteDuration - noteGap)) {
    // Set speakerPin high for half of the period
    digitalWrite(speakerPin, HIGH);
    wait(halfPeriod);

    // Set speakerPin low for half of the period
    digitalWrite(speakerPin, LOW);
    wait(halfPeriod);

    // Update the amount of time that has elapsed
    elapsed += halfPeriod * 2;
  }
  wait(noteDuration - elapsed);
}


long getPeriodForKey(uint8_t keyNumber) {
  // If the key is between 1 and 88
  if (keyNumber >= 1 && keyNumber <= 88) {
    // Return the period (one second divided by the frequency of the key)
    return 1000000L / (pow(2.0, (keyNumber - 49.0) / 12.0) * 440.0);
  }

  // Otherwise return zero
  return 0;
}

void wait(long us) {
  // First delay for the number of whole milliseconds using delay()
  delay(us / 1000);
  // Then delay for the remainder of microseconds using delayMicroseconds()
  delayMicroseconds(us % 1000);
}


