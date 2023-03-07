// Pins
#define STRUMMING_PIN 10
#define MUTE_PIN 11
#define SERVO_PIN 9

// Servo settings
#define SERVO_CENTER 90
#define SERVO_MIN 0
#define SERVO_MAX 180

// Notes
const int NOTES[] = {28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42};
const int NOTES_PIN[] = {13, 12, 8, 7, 6, 5, 4, 3, 2, A0, A1, A2, A3, A4, A5};

// Constants
#define MIN_PWM 50 // Minimum PWM value for strumming
#define TIME_HIT_DEFAULT 8 // Time in ms to hit string with strumming magnet

// MIDI settings
#define MIDI_NOTE_ON 0x90
#define MIDI_NOTE_OFF 0x80
#define MIDI_PITCH_BEND 0xE0

#define MIDI_ON 1 //lit tout les canneaux midi si mis a 0
#define MIDI_CHANNEL 1 // canal midi selectionné
