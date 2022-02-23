#define borneENA        9
#define borneIN1        5
#define borneIN2        6
#define borneENB        3
#define borneIN3        10
#define borneIN4        11

void setup(){
  pinMode(borneENA, OUTPUT);
  pinMode(borneIN1, OUTPUT);
  pinMode(borneIN2, OUTPUT);
  pinMode(borneENB, OUTPUT);
  pinMode(borneIN3, OUTPUT);
  pinMode(borneIN4, OUTPUT);
  digitalWrite(borneIN1, LOW);
  digitalWrite(borneIN2, LOW);
  digitalWrite(borneIN3, LOW);
  digitalWrite(borneIN4, LOW);
  analogWrite(borneENA, 0);
  analogWrite(borneENB, 0);
  Serial.begin(19200);
  while (!Serial);
}

const unsigned int MAX_MESSAGE_LENGTH = 75;
bool check = true;
int i = 0;
int j = 0;
float spe = -500.0;
float mem = -500.0;
static char result[6];

void loop() {
  
  delay(100);
  while (Serial.available() > 0)
 {
   //Create a string to hold the incoming message
   static char message[MAX_MESSAGE_LENGTH];
   static unsigned int message_pos = 0;

   //Read the next available byte in the serial receive buffer
   char inByte = Serial.read();
   //Message coming in (check terminating character) and guard for over message size
   if ( inByte != ']')
   {
     message[message_pos] = inByte; //Put the message in the string like => "["throttle", 100]"
     message_pos++;
   }
   else //end of message
   {
     message[message_pos] = '\0'; // Add null character to string
     for (i = 0; message[i!= ','; i++); // Go to the value of the incoming data
     i ++;
     for (j = 0; message[i]!= '\0'; i++) {
        result[j] = message[i]; // Put the value in a separate string
        j ++;
     }
     result[j] = '\0'; // Add Null character
     Serial.println(message[1]);
     if (message[3] == 'o') { // Checking name of incoming data for Longitudinal Forces
        spe = atof(result); // Transform the value to float
        if ((spe >= -0.75 && spe <= 0.75) || spe == -1.00) { // Checking data to see if we should start the motor
            analogWrite(borneENA, 0);
         } else if (spe > 0.75) { // Positive Longitudinal Forces
            digitalWrite(borneIN1, HIGH);
            digitalWrite(borneIN2, LOW);
            analogWrite(borneENA, 250); // Set direction of rotation and applying speed to the motor
         } else if (spe < -0.75) { // Negative Longitudinal Forces
            digitalWrite(borneIN1, LOW);
            digitalWrite(borneIN2, HIGH);
            analogWrite(borneENA, 250);// Set direction of rotation and applying speed to the motor
         }
     } else if (message[3] == 'a') { // Checking name of incoming data for Lateral Forces
        mem = atof(result); // Transform the value to float
        if ((mem >= -0.75 && mem <= 0.75) || mem == -1.00) { 
            analogWrite(borneENB, 0);
        } else if (mem > 0.75) { // Positive Lateral Forces
            digitalWrite(borneIN3, HIGH);
            digitalWrite(borneIN4, LOW);
            analogWrite(borneENB, 250); // Set direction of rotation and applying speed to the motor
        } else if (mem < -0.75) { // Negative Lateral Forces
            digitalWrite(borneIN3, LOW);
            digitalWrite(borneIN4, HIGH);
            analogWrite(borneENB, 250); // Set direction of rotation and applying speed to the motor
        }
     }
     message[0] = "\0"; // reset data value 
     message_pos = 0;
   }
 }
}
