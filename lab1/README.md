# Lab 1

### Teams
**Team 1**: Ayomi, Emily, Jacob
**Team 2**: Drew, Eric, Joo Yeon

## Reading the Potentiometer


``` arduino
const int analogInPin = A3;  // Analog input pin that the potentiometer is attached to
int sensorValue = 0;        // value read from the pot

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // read the analog in value:
  sensorValue = analogRead(analogInPin);

  // print the results to the Serial Monitor:
  Serial.print("sensor = ");
  Serial.println(sensorValue);
  
  // wait 0.5 s
  delay(500);
}
```


## Work Distribution
* Jacob: reading the potentiometer
