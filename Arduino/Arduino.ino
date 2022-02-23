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
  spe = -1;
  
  delay(100);
  while (Serial.available() > 0)
 {
   //Create a place to hold the incoming message
   static char message[MAX_MESSAGE_LENGTH];
   static unsigned int message_pos = 0;

   //Read the next available byte in the serial receive buffer
   char inByte = Serial.read();
   //delay(2);
   //Message coming in (check not terminating character) and guard for over message size
   if ( inByte != ']')
   {
     message[message_pos] = inByte; //Je mets tous les caractères dans une string du type "["throttle", 100]"
     message_pos++;
   }
   else //le message de la socket est terminé je rentre dans le else
   {
     //Add null character to string
     message[message_pos] = '\0';
     for (i = 0; message[i]!= ','; i++); //j'avance jusqu'a la data de type int correspond à la valeur de l'accel ou le frein
     i ++;
     for (j = 0; message[i]!= '\0'; i++) {
        result[j] = message[i]; //Je mets la valeur dans une string
        j ++;
     }
     result[j] = '\0';
     Serial.println(message[1]);
     if (message[3] == 'o') {
        spe = atof(result);
        if ((spe >= -0.75 && spe <= 0.75) || spe == -1.00) {
            analogWrite(borneENA, 0);
         } else if (spe > 0.75) {
            digitalWrite(borneIN1, HIGH);
            digitalWrite(borneIN2, LOW);
            analogWrite(borneENA, 250);
         } else if (spe < -0.75) {
            digitalWrite(borneIN1, LOW);
            digitalWrite(borneIN2, HIGH);
            analogWrite(borneENA, 250);
         }
     } else if (message[3] == 'a') {
        mem = atof(result);
        if ((mem >= -0.75 && mem <= 0.75) || mem == -1.00) {
            analogWrite(borneENB, 0);
            //Serial.println("null");
        } else if (mem > 0.75) {
            digitalWrite(borneIN3, HIGH);
            digitalWrite(borneIN4, LOW);
            analogWrite(borneENB, 250);
            //Serial.println("plus");
        } else if (mem < -0.75) {
            digitalWrite(borneIN3, LOW);
            digitalWrite(borneIN4, HIGH);
            analogWrite(borneENB, 250);
        }
     }
      //je transforme la valeur de string a int ou float celon ce qui est recherché
     //Serial.println(spe);
     //spe = atof(result);
     message[0] = "\0";
     message_pos = 0;
     // je reset les valeurs à défaut pour reboucler et recevoir de nouvelles valeurs
   }
 }
}
