

void setup() {
  // put your setup code here, to run once:
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);

  digitalWrite(3, LOW);
  digitalWrite(4, LOW);

  Serial.begin(9600);
}

void loop() {
  Serial.println("Start");
  // put your main code here, to run repeatedly:
  if (digitalRead(5) == HIGH){
    digitalWrite(3, HIGH);
    Serial.println("5 = 1");
  } else {
    digitalWrite(3, LOW);
    Serial.println("5 = 0");
  }
  if (digitalRead(6) == HIGH){
    digitalWrite(4, HIGH);
    Serial.println("6 = 1");
  } else{
    digitalWrite(4,LOW);
    Serial.println("6 = 0");
  }
}
