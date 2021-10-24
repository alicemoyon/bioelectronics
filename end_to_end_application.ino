/* 
  end to end ecg to bpm application
 */
#define NUM_SAMPLES 250
#define LED RED_LED

int threshold; // voltage value (from ADC) above which we consider R peak to be; 2.2 * 16383 / 3.3
unsigned int ecg_data[NUM_SAMPLES];

void setup() {
  analogReadResolution(14); // Default is 10 bits; this switches to 14
  // initialise serial communications
  Serial.begin(57600);
  // initalise the digital pin as an output
  pinMode(LED, OUTPUT);
}

void loop() {
  threshold = 10922;
  unsigned long startTime;
  unsigned long peakTime = millis();
  unsigned long newPeakTime;
  unsigned long peakToPeakInterval;
  double bpm;
  startTime = millis(); // number of milliseconds since board began running the program

  // gather and analyse samples for 15 seconds
  while(millis() - startTime < 15000) {
    // read the input on analog pin A3 NUM_SAMPLES times and store in analog_data array until full
    for(int count = 0; count < NUM_SAMPLES; count++) {
      ecg_data[count] = analogRead(A3);
      // detect R-peaks in the sampled data and calculate BPM
      if (ecg_data[count] > threshold) {
        Serial.print("Found a peak at: ");
        newPeakTime = millis();
        Serial.print(newPeakTime);
        Serial.println("ms");
        peakToPeakInterval = newPeakTime - peakTime;
        bpm = (1 / peakToPeakInterval) * 60000; // interval is measured in milliseconds; there are 60000 milliseconds in a minute
        peakTime = newPeakTime;
        // flash the red LED when an R peak is detected
        digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
      }
      delay(1); // 1ms delay in between reads for stability - investigate if this is necessary
    }
  }

  // Send latest BPM value to ThingSpeak channel via WiFi every 15 seconds
  Serial.println("15 seconds have elapsed. Sending BPM data point to ThingSpeak channel");
  Serial.print("BPM is ");
  Serial.print(bpm);
  Serial.println(" beats per second");
  delay(1000); // long delay for testing only

}
