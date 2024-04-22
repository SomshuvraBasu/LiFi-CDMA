// Define the LED pin
const int ledPin =9;

// Define the LDR pin
const int ldrPin = A1;

// Setup the serial port
void setup() {
  Serial.begin(9600);

  // Set the LED pin as output
  pinMode(ledPin, OUTPUT);

  // Set the LDR pin as input
  pinMode(ldrPin, INPUT);
}

// Main loop
void loop() {
  // Read the value from the LDR pin
  int ldrValue = analogRead(ldrPin);

  // Print the value to the serial port
  Serial.print("LDR Value: ");
  Serial.println(ldrValue);

  // If the LDR value is less than 200, turn on the LED
  if (ldrValue <= 400) {
    digitalWrite(ledPin, HIGH);
  } else {
    digitalWrite(ledPin, LOW);
  }

  // Delay for 1 second
  delay(1000);
} 
