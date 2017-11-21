#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10
RF24 radio(9, 10);

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0x0000000012LL, 0x0000000013LL };

void setup(void)
{
  // Output pin setup
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


  Serial.begin(9600);
  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15, 15);
  radio.setAutoAck(true);
  // set the channel
  radio.setChannel(0x50);
  // set the power
  // RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM, and RF24_PA_HIGH=0dBm.
  radio.setPALevel(RF24_PA_MIN);
  //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
  radio.setDataRate(RF24_250KBPS);

  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1, pipes[0]);

  radio.startListening();
  radio.printDetails();
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


void loop(void)
{
  // if there is data ready
  if ( radio.available() )
  {
    unsigned short got_data;
    radio.read(&got_data, sizeof(got_data));
    Serial.println(got_data);
    if (got_data >> 9 == 30) {
      display_done_signal();
    }
    else {
      display_data(got_data);
    }
  }
}
