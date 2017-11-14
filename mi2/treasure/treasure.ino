// do #defines BEFORE #includes
#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft

#include <FFT.h> // include the library

boolean toggle2 = 0;
int fft_i = 0;
int count = 0;

void fastAdcSetup(int pin) {
  //ADCSRA = 0xe5; // set the adc to free running mode
  //ADMUX = 0x40; // use adc0
  //DIDR0 = 0x07; // turn off the digital input for adc0
  DIDR0 = 0x3F; // digital inputs disabled
  ADMUX = 0x40; // measuring on ADC0, use the internal 1.1 reference
  ADCSRA = 0xAC; // AD-converter on, interrupt enabled, prescaler = 16
  ADCSRB = 0x40; // AD channels MUX on, free running mode
  bitWrite(ADCSRA, 6, 1); // Start the conversion by setting bit 6 (=ADSC) in ADCSRA
  sei(); // set interrupt flag
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

ISR(ADC_vect) {
  int port;
    if (ADMUX == 0x40) {
      port = 0;
      ADMUX = 0x41;
    }else if(ADMUX == 0x41) {
      port = 1;
      ADMUX = 0x42;
    }else if(ADMUX == 0x42) {
       port = 2;
       ADMUX = 0x44;
    } 
     
    int val = ADCL; // store lower byte ADC
    val += ADCH << 8; // store higher bytes ADC
   
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

  int max_data = 0;
  int max_bin = 0;
    
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
      
      //added for mi4
      if(i > 20 && fft_log_out[i] > max_data) {
        max_bin = i;
        max_data = fft_log_out[i];
      }      
      Serial.println("max_bin in loop");
      Serial.println(max_bin);
      delay(10000);
      // }
      if (i==127) {
        Serial.println("Stop");
        //delay(100000000);
      }
    }

    int foundTreasure = -1;
    Serial.println("max_bin");
    Serial.println(max_bin);
    
    // 7 kHz
    if(max_bin > 40 && max_bin < 65) { 
      foundTreasure = 0;
      Serial.println("7kHz");
    // 12 kHz
    } else if (max_bin >= 65 && max_bin < 100){
      foundTreasure = 1;
      Serial.println("12kHz");
    // 17 kHz
    } else if (max_bin >= 100 && max_bin < 130){
      foundTreasure = 2;
      Serial.println("17kHz");
    } else {
      foundTreasure = -1;
      Serial.println("No treasure detected");
    }




    
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
