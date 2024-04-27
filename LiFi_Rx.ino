const int ldrPin = A0;      // LDR is connected to analog pin A0
const int threshold = 700;  // Threshold for zero detection

void setup() {
  Serial.begin(115200);     // Initialize serial communication
}

void loop() {
  String receivedString = "";  // Initialize received string
  
  // Receive binary string
  receiveBinary(receivedString);
  
  // Convert binary string to message
  String message = binaryToMessage(receivedString);
  
  // Print received message
  Serial.println("Received message: " + message);
}

// Function to receive binary string
void receiveBinary(String &receivedString) {
  for (int i = 0; i < 8 * sizeof(String); i++) {
    // Receive each bit
    int ldrValue = analogRead(ldrPin);  // Read the analog value from the LDR
    if (ldrValue < threshold) {
      receivedString += '0';  // Below threshold, consider it as '0'
    } else {
      receivedString += '1';  // Above threshold, consider it as '1'
    }
    delay(25);  // Wait before reading the next bit
  }
}

// Function to convert binary string to message
String binaryToMessage(String binaryString) {
  String message = "";
  for (int i = 0; i < binaryString.length(); i += 8) {
    // Extract each 8-bit chunk and convert to ASCII character
    String chunk = binaryString.substring(i, i + 8);
    char character = char(strtol(chunk.c_str(), NULL, 2));
    message += character;
  }
  return message;
}
