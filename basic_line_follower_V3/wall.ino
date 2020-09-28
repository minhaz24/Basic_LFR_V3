void wall_follow() {
  digitalWrite(calout, HIGH);
  int flag = 0;
  while (sum == 0) {
    sonar();
    check();
    if (sf <= wall_boundary && sf > 5) {
      motorSpeedB(6 * spr, 6 * spl);
      delay(90);
      digitalWrite(d90, HIGH);
      digitalWrite(d30, HIGH);
      (sl <= 30 && sl >= 5) ? motorSpeedR(6 * spr, 6 * spl) : motorSpeedL(6 * spr, 6 * spl);
      delay(turn);
      (sl <= 30 && sl >= 5) ? motorSpeedL(6 * spr, 6 * spl) : motorSpeedR(6 * spr, 6 * spl);
      delay(50);
      sonar();
      mos(9 * spr, 9 * spl);
      sf = 0;
      digitalWrite(d90, LOW);
      digitalWrite(d30, LOW);
    }

    else if (sl <= midpoint - 2 && sl > 5) mos(50, 200);
    else if (sl >= midpoint + 2 && sl <= 30) mos(200, 50);
    else if (sr <= midpoint - 2 && sr > 5) mos(200, 50);
    else if (sr >= midpoint + 2 && sr <= 30) mos(50, 200);
    else mos(9 * spr, 9 * spl);
  }
  digitalWrite(calout, LOW);
}

int wall_trigger(int a) {
  if (mi2 - mi3 >= 5) {
    (a==2)? sl = sonarl.ping_cm() : sr = sonarr.ping_cm();
    mi3 = mi2;
  }
  if((a == 2 && sl>5 && sl<25) || (a == 1 && sr>5 && sr<25)) return 1;
  else return 0;
}
