const int ledPin = 9;       // LED connected to digital pin 9
const int threshold = 700;  // Threshold for zero detection

void setup() {
  Serial.begin(115200);     // Initialize serial communication
  pinMode(ledPin, OUTPUT);   // Set LED pin as an output
}

void loop() {
  Serial.println("Enter the message to send:");
  
  while (!Serial.available()) {
    // Wait for user input through the serial monitor
  }

  String message = Serial.readStringUntil('\n');  // Read message from serial monitor
  Serial.println("Sending message: " + message);  // Print the message to be sent

  // Convert message to binary string
  String binaryString = messageToBinary(message);

  // Transmit binary string
  transmitBinary(binaryString);
}

// Function to convert message to binary string
String messageToBinary(String message) {
  String binaryString = "";
  for (int i = 0; i < message.length(); i++) {
    // Convert each character to its ASCII value
    int asciiValue = int(message.charAt(i));
    // Convert ASCII value to 8-bit binary string
    String asciiBinary = String(asciiValue, BIN);
    // Pad with leading zeros to ensure 8 bits
    while (asciiBinary.length() < 8) {
      asciiBinary = "0" + asciiBinary;
    }
    // Append to the binary string
    binaryString += asciiBinary;
  }
  return binaryString;
}

// Function to transmit binary string
void transmitBinary(String binaryString) {
  for (int i = 0; i < binaryString.length(); i++) {
    // Transmit each bit
    if (binaryString.charAt(i) == '1') {
      digitalWrite(ledPin, HIGH); // Turn on the LED
      delay(25);                   // Transmit each bit for 25 milliseconds
    } else {
      digitalWrite(ledPin, LOW);  // Turn off the LED
      delay(25);                   // Transmit each bit for 25 milliseconds
    }
  }
}
