const int ldrPin = A0;     // LDR is connected to analog pin A1
const int ledPin = 9;     // LED connected to digital pin 9
const int threshold = 700; // Threshold for zero detection

void setup()
{
    Serial.begin(115200);      // Initialize serial communication
    pinMode(ledPin, OUTPUT); // Set LED pin as an output
}

void loop()
{
    int ldr2Value = analogRead(ldrPin);
    // Wait for user input through the serial monitor
    while (!Serial.available())
        ;
    String message = Serial.readStringUntil('\n');

    // Print the message to be sent
    Serial.println("Sending message: " + message);

    // Convert message to binary string
    String binaryString = "";
    for (int i = 0; i < message.length(); i++)
    {
        // Convert each character to its ASCII value
        int asciiValue = int(message.charAt(i));
        // Convert ASCII value to 8-bit binary string
        String asciiBinary = String(asciiValue, BIN);
        // Pad with leading zeros to ensure 8 bits
        while (asciiBinary.length() < 8)
        {
            asciiBinary = "0" + asciiBinary;
        }
        // Append to the binary string
        binaryString += asciiBinary;
    }

    // Transmission and reception interleaved
    String receivedString = ""; // Initialize received string
    for (int i = 0; i < binaryString.length(); i++)
    {
        // Transmit
        if (binaryString.charAt(i) == '1')
        {
            digitalWrite(ledPin, HIGH); // Turn on the LED
            delay(25);                 // Transmit each bit for 25 milliseconds
        }
        else
        {
            digitalWrite(ledPin, LOW); // Turn off the LED
            delay(25);                // Transmit each bit for 25 milliseconds
        }

        // Receive
        int ldrValue = analogRead(ldrPin); // Read the analog value from the LDR
        if (ldrValue < threshold)
        {
            receivedString += '0'; // Below threshold, consider it as '0'
        }
        else
        {
            receivedString += '1'; // Above threshold, consider it as '1'
        }
        // delay(100); // Delay for synchronization
    }

    // End of transmission, print received string
    Serial.print("Received message: ");
    for (int i = 0; i < receivedString.length(); i += 8)
    {
        // Extract each 8-bit chunk and convert to ASCII character
        String chunk = receivedString.substring(i, i + 8);
        char character = char(strtol(chunk.c_str(), NULL, 2));
        Serial.print(character);
    }
    Serial.println();

    // Check if the received message matches the sent message
    if (receivedString == binaryString)
    {
        Serial.println("Communication successful :)");
    }
    else
    {
        Serial.println("Communication failed :(");
    }

    // End of transmission
    digitalWrite(ledPin, LOW);
}
