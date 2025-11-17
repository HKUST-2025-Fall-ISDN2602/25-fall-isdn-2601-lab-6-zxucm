/*Change all the ? in the code, add code in ???, modify getPosition() */

//L298N Driver Pin 

#define MOTOR_ENA 16  // Replace the ? with the GPIO pin you selected to connect ENA
#define MOTOR_IN1 26
#define MOTOR_IN2 27

//Encoder Pin 
#define ENCODER_PINA 13
#define ENCODER_PINB 14

//Encoder Counter
volatile long encoderCount = 0; 
volatile double position=0; 

// Serial Monitor command for rotation direction
String command;

// interruppt
void IRAM_ATTR encoderInterrupt() {
  if (digitalRead(ENCODER_PINA) != digitalRead(ENCODER_PINB)) {
    encoderCount++;
  } else {
    encoderCount--;
  }
}

//Serial Display Function 
void getState(){
  Serial.print("Count: ");
  Serial.println(encoderCount);
  Serial.print("Position: ");
  Serial.println(getPosition());
  delay(50);
}

// To get the current position
/*Modify this function*/ 
double getPosition() {
  // Calculate the current position based on encoder count
  position = float(encoderCount)*360.0/1030.0; // Replace 1000.0 with the actual counts per revolution

  if (position<0)
  {position = position + 360; // Ensure position is positive
  }
  
  return position;
}

void setup() {
  
/* pin mode for pins connected with L298N driver  */
  pinMode(MOTOR_IN1, OUTPUT);
  pinMode(MOTOR_IN2, OUTPUT);
  pinMode(MOTOR_ENA, OUTPUT);

// encoder A pin mode for interrupt
  pinMode(ENCODER_PINA, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PINA), encoderInterrupt, CHANGE);

/*encoder B pin mode */   
  pinMode(ENCODER_PINB, INPUT_PULLUP);

/* set up baud rate  */
  Serial.begin(115200);

}

void loop() {
 if (Serial.available() > 0) {
        command = Serial.readStringUntil('\n'); // Read the incoming command
        command.trim(); // Remove any leading or trailing whitespace
        if (command == "B") {
             /*Forward rotation direction*/
              digitalWrite(MOTOR_IN1, LOW);
              digitalWrite(MOTOR_IN2, HIGH);
             /*Set a speed for your motor*/
            analogWrite(MOTOR_ENA, 200);

        } 
        else if (command == "F") {
             /*Backward rotation direction*/
              digitalWrite(MOTOR_IN1, HIGH);
              digitalWrite(MOTOR_IN2, LOW);
             /*Set a speed for your motor*/
            analogWrite(MOTOR_ENA, 200);

        }
        } 

  //print the encoderCount and Position  
  getState(); 

   /* Reset encoder count*/
  if (position > 360 || position < 0) {
    encoderCount = 0;
  } 

}
