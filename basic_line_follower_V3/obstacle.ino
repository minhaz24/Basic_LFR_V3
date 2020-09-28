void obstacle(int a) {
  if (mi2 - mi4 > 5) {
    sf = sonarf.ping_cm();
    mi3 = mi2;
  }
  if (sf < object_boundary && sf > 0) {
    digitalWrite(calout, HIGH);
    braking_mechanism();
    (a == 1) ? motorSpeedR(6 * spr, 6 * spl) : motorSpeedL(6 * spr, 6 * spl);
    delay(turn);
    (a == 2) ? motorSpeedR(6 * spr, 6 * spl) : motorSpeedL(6 * spr, 6 * spl);
    delay(br);
    (a == 1) ? mos(10 * spr, 2.5 * spl) : mos(2.5 * spr, 10 * spl);
    check();
    while (sum == 0) check();
    while (sum != 0) check();
    delay(btd);
    braking_mechanism();
    (a == 1) ? motorSpeedR(6 * spr, 6 * spl) : motorSpeedL(6 * spr, 6 * spl);
    check();
    while (sensor[2] == 0 && sensor[3] == 0) check();
    (a == 2) ? motorSpeedR(6 * spr, 6 * spl) : motorSpeedL(6 * spr, 6 * spl);
    delay(br);
    digitalWrite(calout, LOW);
  }
}
