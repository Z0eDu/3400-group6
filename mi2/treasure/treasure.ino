// do #defines BEFORE #includes
#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft

#include <FFT.h> // include the library

boolean toggle2 = 0;
int fft_i = 0;
int count = 0;

void fastAdcSetup(int pin) {
  ADCSRA = 0xe5; // set the adc to free running mode
  ADMUX = 0x40; // use adc0
  DIDR0 = 0x01; // turn off the digital input for adc0
}

int fastAdcRead() {
  while(!(ADCSRA & 0x10)); // wait for adc to be ready
  ADCSRA = 0xf5; // restart adc
  byte m = ADCL; // fetch adc data
  byte j = ADCH;
  int k = (j << 8) | m; // form into an int
  k -= 0x0200; // form into a signed int
  k <<= 6; // form into a 16b signed int
  return k;
}

void setup() {
  pinMode(9, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);
  Serial.begin(9600);
  fastAdcSetup(0x40);

  cli();
  // put your setup code here, to run once:
  TCCR0A = 0;// set entire TCCR1A register to 0
  TCCR0B = 0;// same for TCCR1B
  TCNT0  = 0;//initialize counter value to 0
  // set compare match register for 8khz increments
  OCR0A = 51;// = (16*10^6) / (8000*8) - 1 (must be <256)
  // turn on CTC mode
  TCCR0A |= (1 << WGM21);
  // Set CS21 bit for 8 prescaler
  TCCR0B |= (1 << CS21);
  // enable timer compare interrupt
  TIMSK0 |= (1 << OCIE2A);

  sei();
}

ISR(TIMER0_COMPA_vect){//timer1 interrupt 8kHz toggles pin 9
//generates pulse wave of frequency 8kHz/2 = 4kHz (takes two cycles for full wave- toggle high then toggle low)
  if (toggle2){
    digitalWrite(9,HIGH);
    toggle2 = 0;
  }
  else{
    digitalWrite(9,LOW);
    toggle2 = 1;
  }

  if (fft_i == 512){
    fft_window(); // window the data for better frequency response
    fft_reorder(); // reorder the data before doing the fft
    fft_run(); // process the data in the fft
    fft_mag_log(); // take the output of the fft

    fft_i = 0;
   Serial.println("Start");

    for (byte i = 0; i <FFT_N/2; i++){
      // if(i == 20 || i == 21 || i == 22){
      Serial.println(fft_log_out[i]);// send out the data
      // }
    }
    Serial.println("Stop");
    
    if(fft_log_out[20] >= 50 || fft_log_out[21] >= 50 ||fft_log_out[22] >= 50){
      count ++;
    } else{
      count = 0;
    }
    if (count >= 15) {
      digitalWrite(13,HIGH);
    }
    else {
      digitalWrite(13, LOW);
    }
  } else{
    fft_input[fft_i] = fastAdcRead();
    fft_input[fft_i+1] = 0;
    fft_i += 2;
  }

}

void loop() {
  // put your main code here, to run repeatedly:


}
