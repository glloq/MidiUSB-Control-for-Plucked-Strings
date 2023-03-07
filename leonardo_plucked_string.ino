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

void loop() {
  // Check for MIDI messages
  if (MidiUSB.available()) {
    // Read the MIDI message
    midiEventPacket_t midiEvent = MidiUSB.read();
    // CHECK if we'r using one channel
    if (!MIDI_ON){
      // Check if the MIDI channel does not match MIDI_CHANNEL
      if (midiEvent.header != 0 && (midiEvent.header & 0x0F) != MIDI_CHANNEL) {
        // Ignore the MIDI message
        return;
      }
    }
    // Check if the message is a Pitch Bend
    if (midiEvent.byte1 == MIDI_PITCH_BEND) {
      // Map the Pitch Bend value to the Servo position
      int servoPos = map(midiEvent.byte2, 0, 127, SERVO_MIN, SERVO_MAX);
      // Set the Servo position
      myservo.write(servoPos);
    }
    // Check if the message is a Note On
    else if (midiEvent.byte1 == MIDI_NOTE_ON) {
      // Check if the Note is in the NOTES array
      int noteIndex = -1;
      for (int i = 0; i < sizeof(NOTES)/sizeof(int); i++) {
        if (midiEvent.byte2 == NOTES[i]) {
          noteIndex = i;
          break;
        }
      }
      // If the Note is in the NOTES array
      if (noteIndex >= 0) {
        // Activate the Mute if it's not already active
        digitalWrite(MUTE_PIN, LOW);
        // Activate the Note pin
        digitalWrite(NOTES_PIN[noteIndex], LOW);
               // Activate the Strumming magnet for a certain amount of time
        analogWrite(STRUMMING_PIN, MIN_PWM);
        delay(TIME_HIT_DEFAULT);
        analogWrite(STRUMMING_PIN, 0);
      }
    }
    // Check if the message is a Note Off
    else if (midiEvent.byte1 == MIDI_NOTE_OFF) {
      // Check if the Note is in the NOTES array
      int noteIndex = -1;
      for (int i = 0; i < sizeof(NOTES)/sizeof(int); i++) {
        if (midiEvent.byte2 == NOTES[i]) {
          noteIndex = i;
          break;
        }
      }
      // If the Note is in the NOTES array
      if (noteIndex >= 0) {
        // Desactivate the Note pin
        digitalWrite(NOTES_PIN[noteIndex], HIGH);
        // Check if all the Note pins are deactivated
        bool allNotesOff = true;
        for (int i = 0; i < sizeof(NOTES)/sizeof(int); i++) {
          if (digitalRead(NOTES_PIN[i]) == LOW) {
            allNotesOff = false;
            break;
          }
        }
        // If all the Note pins are desactivated, desactivate the Mute
        if (allNotesOff) {
          digitalWrite(MUTE_PIN, HIGH);
        }
      }
    }
  }
}
