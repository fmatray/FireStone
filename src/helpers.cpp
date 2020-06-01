#include "helpers.h"

/*
    Serial helpers
*/
void title(String message) {
  Serial.print("--------- ");
  printdatetime();
  Serial.print(" -- ");
  Serial.print(message);
  Serial.println(" ---------");
}

void fail(String message) {
  title("Fail");
  Serial.println(message);
  pinMode(LED_BUILTIN, OUTPUT);
  while (1)
    blink(LED_BUILTIN);
}

void blink(char pin) {
  digitalWrite(pin, HIGH);
  delay(500);
  digitalWrite(pin, LOW);
  delay(500);
}
/*
    Time helpers
*/
bool check_time(unsigned long lasttime, unsigned int interval) {
  return check_time_ms(lasttime, interval * 1000);
}

bool check_time_ms(unsigned long lasttime, unsigned int interval) {
  unsigned long now_millis;

  now_millis = millis();

  return (now_millis - lasttime > interval || lasttime == 0 ||
          now_millis < lasttime);
}

/*
    Datetime helpers
*/
String rtc_date() {
  return twodigits(rtc.getDay()) + "/" + twodigits(rtc.getMonth()) + "/" +
         twodigits(rtc.getYear());
}
String rtc_time() {
  return twodigits(rtc.getHours()) + ":" + twodigits(rtc.getMinutes()) + ":" +
         twodigits(rtc.getSeconds());
}

void printdatetime() {
  Serial.print(rtc_date());
  Serial.print(" ");
  Serial.print(rtc_time());
}

void printlndatetime() {
  printdatetime();
  Serial.println("");
}

void print2digits(short digits) { Serial.print(twodigits(digits)); }
String twodigits(short digits) {
  return (digits < 10 ? "0" : "") + String(digits);
}

/*
 * Debounce helper
 */
bool debounce(unsigned long *lasttime, bool *last_state, bool *button_state,
              const bool reading, const unsigned short interval) {
  bool debounced = false;

  if (reading != *last_state)
    *lasttime = millis();
  if (check_time_ms(*lasttime, interval)) {
    if (reading != *button_state) {
      *button_state = reading;
      debounced     = true;
    }
  }
  *last_state = reading;
  return debounced;
}
/*
 * Status helper
 */
String status_str(status_e _status) {
  switch (_status) {
    case NO_CHECK:
      return "No check";
    case OFF:
      return "Off";
    case OK:
      return "OK";
    case IDLE:
      return "Idle";
    case ERROR:
      return "Error";
    case WARNING:
      return "Warning";
    case ALERT:
      return "Alert";
  }
  return "UNKOWN STATUS";
}