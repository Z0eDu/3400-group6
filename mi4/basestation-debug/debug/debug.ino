void setup() {
  // put your setup code here, to run once:
  pinMode(8, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
}

void display_data(unsigned short got_data, bool init = true) {
  if (init) {
    digitalWrite(4, LOW);
    // Location bits
    digitalWrite(A2, HIGH);
    digitalWrite(A1, HIGH);
    digitalWrite(A0, HIGH);
    digitalWrite(2, HIGH);
    digitalWrite(3, HIGH);
    // Clock high
    digitalWrite(4, HIGH);
  }

  // Clock low
  const int foo[] = {0, 0, 1, 2, 2};
  for (int x = 0; x < 5; x++) {
    digitalWrite(4, LOW);

    // State bits
    (got_data >> ( 0 + 3 * foo[x])) & 0x1 ? digitalWrite(5, HIGH) : digitalWrite(5, LOW);
    (got_data >> ( 1 + 3 * foo[x])) & 0x1 ? digitalWrite(6, HIGH) : digitalWrite(6, LOW);
    (got_data >> ( 2 + 3 * foo[x])) & 0x1 ? digitalWrite(7, HIGH) : digitalWrite(7, LOW);
    // Location bits

    bitRead(got_data, 9)   ? digitalWrite(A2, HIGH) : digitalWrite(A2, LOW);
    bitRead(got_data, 10)  ? digitalWrite(A1, HIGH) : digitalWrite(A1, LOW);
    bitRead(got_data, 11)  ? digitalWrite(A0, HIGH) : digitalWrite(A0, LOW);
    bitRead(got_data, 12)  ? digitalWrite(2, HIGH) : digitalWrite(2, LOW);
    bitRead(got_data, 13)  ? digitalWrite(3, HIGH) : digitalWrite(3, LOW);

    // Clock high
    digitalWrite(4, HIGH);
  }
  digitalWrite(4, LOW);
}

void display_done_signal() {
  display_data(30 << 9, false);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned short  got_data;

  for (int fun = 0; fun < 10; fun ++) {
    for (int i = 0; i < 20; i++) {
      int j  = (i + fun) % 20;
      unsigned short got_data = i << 9 | (j % 4) << 6 | (j % 16 << 2) | (j % 4); //(i == 7 || i == 5 || i == 9) ? i << 9 | 0b001000011 : i << 9;
      display_data(got_data);
    }
    delay(500);
  }

  while (true) {
    display_done_signal();
  }

}
