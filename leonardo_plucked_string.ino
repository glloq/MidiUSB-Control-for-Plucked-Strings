#include "MIDIUSB.h"
#include "settings.h"
#include <Servo.h>
// Servo object
Servo myservo;

void setup() {
  // Attach Servo to SERVO_PIN
  myservo.attach(SERVO_PIN);

  // Set STRUMMING_PIN and MUTE_PIN as outputs
  pinMode(STRUMMING_PIN, OUTPUT);
  pinMode(MUTE_PIN, OUTPUT);

  // Set NOTES_PIN as outputs and deactivate them
  for (int i = 0; i < sizeof(NOTES)/sizeof(int); i++) {
    pinMode(NOTES_PIN[i], OUTPUT);
    digitalWrite(NOTES_PIN[i], LOW);
  }

  // Set initial Servo position to SERVO_CENTER
  myservo.write(SERVO_CENTER);
}
bool mute_active=true;
void loop() {
  // Check for MIDI messages
  if (MidiUSB.available()) {
    //***********************************************************************************
    // Read the MIDI message
    midiEventPacket_t midiEvent = MidiUSB.read();
    // CHECK if we'r using only one channel
    byte status = midiEvent.header & 0xF0; // Le statut (Note On, Note Off, etc.)
    byte channel = midiEvent.header & 0x0F; // Le canal MIDI
    byte note = midiEvent.byte1; // La note MIDI
    byte velocity = midiEvent.byte2; // La vélocité (pour les messages Note On/Off) 
    
       
    if (MIDI_ON){
      // Check if the MIDI channel does not match MIDI_CHANNEL
    if (channel != MIDI_CHANNEL) {
          // Ignore the MIDI message
        return;
      }
    }
    //************************************************************************************
    // Check if the message is a Pitch Bend
    if (status == MIDI_PITCH_BEND) {
      // Map the Pitch Bend value to the Servo position
      int pitchBendValue = velocity * 128 + note;      
      int servoPos = map(pitchBendValue, 0, 16383, SERVO_MIN, SERVO_MAX);
      // Set the Servo position
      myservo.write(servoPos);
    }
    //*************************************************************************************
    // Check if the message is a Note On
    else if (status == MIDI_NOTE_ON) {
      // Check if the Note is in the NOTES array
      int noteIndex = -1;
      for (int i = 0; i < sizeof(NOTES)/sizeof(int); i++) {
        if (note == NOTES[i]) {
          noteIndex = i;
          break;
        }
      }
      // If the Note is in the NOTES array
      if (noteIndex >= 0) {
        // Activate the Mute if it's not already active
        if(mute_active){
          digitalWrite(MUTE_PIN, HIGH);
          mute_active=false;
        }
        // Activate the Note pin
        digitalWrite(NOTES_PIN[noteIndex], HIGH);
        // maybe add some time here to get the time to hit the note before strumming 
        
        // set velocity
        int pwmValue = map(velocity, 0, 127, MIN_PWM, 255);        
         // Activate the Strumming magnet for a certain amount of time
        analogWrite(STRUMMING_PIN, pwmValue);
        delay(TIME_HIT_DEFAULT);
        analogWrite(STRUMMING_PIN, 0);
      }
    }
    //********************************************************************************************
    // Check if the message is a Note Off
    else if (status == MIDI_NOTE_OFF) {
      // Check if the Note is in the NOTES array
      int noteIndex = -1;
      for (int i = 0; i < sizeof(NOTES)/sizeof(int); i++) {
        if (note == NOTES[i]) {
          noteIndex = i;
          break;
        }
      }
      // If the Note is in the NOTES array
      if (noteIndex >= 0) {
        // Desactivate the Note pin
        digitalWrite(NOTES_PIN[noteIndex], LOW);
        // Check if all the Note pins are deactivated
        bool allNotesOff = true;
        for (int i = 0; i < sizeof(NOTES)/sizeof(int); i++) {
          if (digitalRead(NOTES_PIN[i]) == HIGH) {
            allNotesOff = false;
            break;
          }
        }
        // If all the Note pins are desactivated, activate the Mute
        if (allNotesOff) {
          digitalWrite(MUTE_PIN, LOW);
          mute_active=true;
        }
      }
    }
  }
}
