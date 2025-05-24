// Define the pin number for the button and output (like an LED or relay)
#define BUTTON_PIN 2    // Connected to PD2 (Digital Pin 2)
#define OUTPUT_PIN 7    // Connected to PD7 (Digital Pin 7)

// These variables help with button state tracking and debouncing
bool lastButtonState = LOW;          // Stores the last read state of the button
bool outputState = LOW;              // Keeps track of whether output is ON or OFF
unsigned long lastDebounceTime = 0;  // The last time the button state was changed
const unsigned long debounceDelay = 50; // Time to wait (in ms) for debounce

void setup() {
  // Configure the button pin as input
  // Note: It’s assumed a pulldown resistor is already connected externally
  pinMode(BUTTON_PIN, INPUT);

  // Set the output pin as output
  pinMode(OUTPUT_PIN, OUTPUT);
  digitalWrite(OUTPUT_PIN, LOW); // Start with the output turned OFF

  // Begin serial communication at 9600 bps
  // This is typically used to communicate with a Bluetooth module like HC-05
  Serial.begin(9600);
}

void loop() {
  // --- BUTTON HANDLING WITH DEBOUNCE ---

  // Read the current state of the button
  bool reading = digitalRead(BUTTON_PIN);

  // Check if the button state has changed from the last reading
  if (reading != lastButtonState) {
    // If it has changed, record the current time
    lastDebounceTime = millis();
  }

  // If enough time has passed since the last change (debounce delay),
  // and the button is being pressed (HIGH), and it wasn't pressed before:
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading == HIGH && lastButtonState == LOW) {
      // Toggle the output state (ON -> OFF or OFF -> ON)
      outputState = !outputState;

      // Set the output pin accordingly
      digitalWrite(OUTPUT_PIN, outputState);
    }
  }

  // Save the current reading as the last state for the next loop
  lastButtonState = reading;

  // --- BLUETOOTH COMMAND HANDLING ---

  // Check if any data has been received from the Bluetooth module (via Serial)
  if (Serial.available()) {
    // Read the incoming byte
    char command = Serial.read();

    // If the received character is '1', turn the output ON
    if (command == '1') {
      digitalWrite(OUTPUT_PIN, HIGH);
      outputState = true;
    }
    // If the received character is '0', turn the output OFF
    else if (command == '0') {
      digitalWrite(OUTPUT_PIN, LOW);
      outputState = false;
    }
  }
}
