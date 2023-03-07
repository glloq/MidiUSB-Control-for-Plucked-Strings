# --------------   WORK IN PROGRESS  -----------------

code fait pour pouvoir jouer 15 accords sur une code avec plusieurs electroaimants lineaire :
  - 15 pour les accords
  - 1 pour le silencieux (normalement actif si non alimenté)
  - 1 pour le "grattage" de la corde (il faut prevoir un mecanisme pour n'actionner qu'une fois la corde sur le cycle entrée/sortie de la tige)
  
il y a aussi la possibilité d'utiliser les messages pitchbend avec un servomoteur qui controle la monté/descente d'un demis ton


Les réglages possibles du projet sont a faire dans settings.h:
   - Les broches utilisées pour le grattage, le silencieux et le servomoteur (STRUMMING_PIN, MUTE_PIN, SERVO_PIN)
   - Les paramètres du servomoteur, tels que la position centrale et les deux extrêmes (SERVO_CENTER, SERVO_MIN, SERVO_MAX)
   - Les différentes notes MIDI qui peuvent être jouées (NOTES)
   - Les broches associées à chaque note MIDI (NOTES_PIN)
   - La valeur minimale du PWM pour actionner l'électroaimant de grattage (MIN_PWM)
   - La durée pendant laquelle l'électroaimant de grattage est activé (TIME_HIT_DEFAULT)
   - L'activation ou la désactivation de la lecture de messages MIDI en fonction du canal MIDI spécifié (MIDI_ON, MIDI_CHANNEL)
