void braking_adjust() {
a:
  int list = 9;
  int p = 0;
  int q = 0;
  int temp = 0;
  while (digitalRead(switchin) == LOW) {
    delay(1);
    p = map(analogRead(9), 20, 1023, 1, list);
    if (temp != p) {
      temp = p;
      if (temp == 1) {
        display.clearDisplay();
        text("TURN", 41, 2);
        text("DELAY", 35, 24);
        q = EEPROM.read(11);
        String value = String(q, 10);
        if (q < 10) text(value, 59, 46);
        else if ( q < 100) text(value, 53, 46);
        else text(value, 47, 46);
        display.display();
      }
      else if (temp == 2) {
        display.clearDisplay();
        text("END LINE", 17, 2);
        text("DELAY", 35, 24);
        q = EEPROM.read(12);
        String value = String(q, 10);
        if (q < 10) text(value, 59, 46);
        else if ( q < 100) text(value, 53, 46);
        else text(value, 47, 46);
        display.display();
      }
      else if (temp == 3) {
        display.clearDisplay();
        text("BRAKE", 35, 2);
        text("DIVIDER", 23, 24);
        q = EEPROM.read(13);
        String value = String(q, 10);
        if (q < 10) text(value, 59, 46);
        else if ( q < 100) text(value, 53, 46);
        else text(value, 47, 46);
        display.display();
      }
      else if (temp == 4) {
        display.clearDisplay();
        text("LOWEST", 29, 2);
        text("BRAKE", 35, 24);
        q = EEPROM.read(14);
        String value = String(q, 10);
        if (q < 10) text(value, 59, 46);
        else if ( q < 100) text(value, 53, 46);
        else text(value, 47, 46);
        display.display();
      }
      else if (temp == 5) {
        display.clearDisplay();
        text("HIGHEST", 23, 2);
        text("BRAKE", 35, 24);
        q = EEPROM.read(15);
        String value = String(q, 10);
        if (q < 10) text(value, 59, 46);
        else if ( q < 100) text(value, 53, 46);
        else text(value, 47, 46);
        display.display();
      }
      else if (temp == 6) {
        display.clearDisplay();
        text("  OBJECT  ", 04, 2);
        text(" BOUNDARY ", 04, 24);
        q = EEPROM.read(16);
        String value = String(q, 10);
        if (q < 10) text(value, 59, 46);
        else if ( q < 100) text(value, 53, 46);
        else text(value, 47, 46);
        display.display();
      }
      else if (temp == 7) {
        display.clearDisplay();
        text("   WALL   ", 04, 2);
        text(" BOUNDARY ", 04, 24);
        q = EEPROM.read(17);
        String value = String(q, 10);
        if (q < 10) text(value, 59, 46);
        else if ( q < 100) text(value, 53, 46);
        else text(value, 47, 46);
        display.display();
      }
      else if (temp == 8) {
        display.clearDisplay();
        text("   WALL   ", 04, 2);
        text(" MIDPOINT ", 04, 24);
        q = EEPROM.read(18);
        String value = String(q, 10);
        if (q < 10) text(value, 59, 46);
        else if ( q < 100) text(value, 53, 46);
        else text(value, 47, 46);
        display.display();
      }
      else if (temp == 9) {
        display.clearDisplay();
        text("CALLIBRATE", 04, 2);
        text("PROPORTION", 04, 24);
        q = EEPROM.read(19);
        String value = String(q, 10);
        if (q < 10) text(value, 59, 46);
        else if ( q < 100) text(value, 53, 46);
        else text(value, 47, 46);
        display.display();
      }
    }
    if (digitalRead(calin) == HIGH) {
      display.clearDisplay();
      text("BYE!!!", 23, 24);
      display.display();
      return;
    }
  }

  while (digitalRead(switchin) == HIGH) {
    delay(1);
    digitalWrite(calout, HIGH);
  }
  digitalWrite(calout, LOW);

  while (1) {
    if (temp == 1) p = map(analogRead(9), 0, 1023, 0, 100);
    else if (temp == 2) p = map(analogRead(9), 0, 1023, 100, 200);
    else if (temp == 3) p = map(analogRead(9), 0, 1023, 1, 9);
    else if (temp == 4) p = map(analogRead(9), 0, 1023, 0, 100);
    else if (temp == 5) p = map(analogRead(9), 0, 1023, 50, 200);
    else if (temp >= 6 && temp <= 8) p = map(analogRead(9), 0, 1023, 0, 30);
    else if (temp == 9) p = map(analogRead(9), 0, 1023, 1, 9);
    display.clearDisplay();
    String value = String(p, 10);
    text("ADJUST:", 5, 24);
    text(value, 90, 24);
    display.display();
    if (digitalRead(calin) == HIGH) {
      display.clearDisplay();
      text("BYE!!!", 29, 24);
      display.display();
      goto a;
    }
    if (digitalRead(switchin) == HIGH) {
      delay(100);
      while (digitalRead(switchin) == HIGH) digitalWrite(calout, HIGH);
      digitalWrite(calout, LOW);
      display.clearDisplay();
      text("OKAY!!!", 29, 24);
      display.display();
      EEPROM.write(temp + 10, p);
      delay(10);
      btd = EEPROM.read(11);
      mtd = EEPROM.read(12);
      d = EEPROM.read(13);
      base = EEPROM.read(14) * d;
      peak = EEPROM.read(15) * d;
      cl = base;
      brake = cl / d;
      object_boundary = EEPROM.read(16);
      wall_boundary = EEPROM.read(17);
      midpoint = EEPROM.read(18);
      cal_p = EEPROM.read(19) * 0.1;
      goto a;
    }
  }
}

void braking_mechanism() {
  motorSpeedB(10 * spr, 10 * spl);
  brake = cl / d;
  delay(brake);
  k30 = 0;
  k90 = 0;
  mov = 0;
  cross = 0;
  cl = base;
  brake = cl / d;
  k = 0;
}
