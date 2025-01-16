const int potPins[] = {A0, A1, A2, A3};  // Array of analog input pins connected to potentiometers
const int motorPins[] = {3, 9, 10, 11};  // Array of PWM output pins connected to motor drivers
int potValues[4];                        // Array to store the read values of each potentiometer
int motorSpeeds[4];                      // Array to store the mapped PWM values for each motor

void setup() {
  // Set motor pins as output mode
  for (int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);
  }
  Serial.begin(9600); // Initialize serial communication
}

void loop() {
  for (int i = 0; i < 4; i++) {
    // Read the analog value from the potentiometer (0 - 1023)
    potValues[i] = analogRead(potPins[i]);

    // Map the potentiometer value to the PWM range (0 - 255)
    motorSpeeds[i] = map(potValues[i], 0, 1023, 0, 255);

    // Output PWM signal to the corresponding motor driver pin
    analogWrite(motorPins[i], motorSpeeds[i]);
  }

  // Display the values of each potentiometer and the corresponding PWM values in the serial monitor
  Serial.println("Potentiometer Values and Motor Speeds:");
  for (int i = 0; i < 4; i++) {
    Serial.print("A");
    Serial.print(i);
    Serial.print(": ");
    Serial.print(potValues[i]);
    Serial.print(" -> PWM: ");
    Serial.println(motorSpeeds[i]);
  }
  
  delay(500); // Delay to observe value changes
}
