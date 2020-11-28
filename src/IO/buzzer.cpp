#include "IO/buzzer.h"

#include <Arduino.h>

#include "common/helpers.h"

BuzzerAction::BuzzerAction() {
  pinMode(FIRE_PIN, OUTPUT);
}

void BuzzerAction::buzz(const unsigned short freq = 880) {
#ifdef BUZZER_ACTIVE
  digitalWrite(FIRE_PIN, HIGH);
#else
  tone(FIRE_PIN, freq);
#endif
}

void BuzzerAction::no_buzz() {
#ifdef BUZZER_ACTIVE
  digitalWrite(FIRE_PIN, LOW);
#else
  noTone(FIRE_PIN);
#endif
}

void BuzzerAction::run() {
  static unsigned long buzz_lastime = 0;
  static bool buzz_state            = false;

  if (action == warn) {
    if (check_time(buzz_lastime, 5)) {
      buzz_lastime = millis();
      buzz_state   = !buzz_state;
    }
    if (buzz_state)
      buzz();
  } else if (action == alert)
    buzz();
  else {
    buzz_state = false;
    no_buzz();
  }
}