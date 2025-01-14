/* Code for the Maze Solver */

// Libraries Here:
#include <NewPing.h>

// Pins Here:
#define ledPin LED_BUILTIN
#define buttonPin A0
#define motor_L1 2 // Motor IN1
#define motor_L2 3 // Motor IN2
#define motor_R1 4 // Motor IN3
#define motor_R2 5 // Motor IN4

const int sensPin[3][2] = {{6, 7}, {8, 9}, {10, 11}}; // {x, y} => {trigPin, echoPin};

// Constants Here:
// #define MAX_SEGMENTS      // Number of Segments in Maze;
#define MAX_DISTANCE 180    // Maximum distance to Measure;
#define TRIGGER_DISTANCE 25 // Distance after which CrossRoad Code is Triggered;
#define TURN_DELAY 250
#define STOP_DELAY 1
#define FWD_DELAY 500
#define sensordelay 50
#define wallgap 5

// const byte mapSize[2] = {MAX_SEGMENTS, MAX_SEGMENTS}; // Map Size {x, y}; Assumes Origin at {0, 0};
const int start[2] = {0, 0}; // Starting Coordinate {x, y};
const int end[2] = {7, 7};   // Ending Coordinate {x, y};

// Variables Here:
byte isFacing = 1;                        // 0 = Left, 1 = Front, 2 = Right, 3 = Back;
int currentPos[2] = {start[0], start[1]}; // Current position {x, y};

// OOP Here:
NewPing scanner[3] = {NewPing(sensPin[0][0], sensPin[0][1], MAX_DISTANCE),
                      NewPing(sensPin[1][0], sensPin[1][1], MAX_DISTANCE),
                      NewPing(sensPin[2][0], sensPin[2][1], MAX_DISTANCE)}; // Order: Left, Front, Right;

struct Distance
{ // New Struct for Defining the Distance related with each Sensor; Perform all changes in '.value' and it reflects on the '.sensor';
   int value = 0;
   byte sensor = 0;
};

void setup()
{
   Serial.begin(9600);
   digitalWrite(motor_L1, LOW);
   digitalWrite(motor_L2, LOW);
   digitalWrite(motor_R1, LOW);
   digitalWrite(motor_R2, LOW);
   // digitalWrite(motor_ENL, HIGH);
   // digitalWrite(motor_ENR, HIGH);
   pinMode(ledPin, OUTPUT);
   // pinMode(motor_L1, OUTPUT);
   // pinMode(motor_L2, OUTPUT);
   // pinMode(motor_R1, OUTPUT);
   // pinMode(motor_R2, OUTPUT);
   for (byte i = 5; i > 0; i--)
   {
      delay(500);
      digitalWrite(ledPin, HIGH);
      delay(500);
      digitalWrite(ledPin, LOW);
      Serial.println(i);
   }
}

void loop()
{
   Distance inputs[3];
   for (byte i = 0; i < 3; i++)
   {
      inputs[i] = getDistance(0)[i];
   }
   Serial.println("Inputs: " + String(inputs[0].value) + " " + String(inputs[1].value) + " " + String(inputs[2].value));
   byte waysFound = 0;
   for (byte i = 0; i < 3; i++)
   {
      if (inputs[i].value > 0)
         waysFound++;
   }
   Serial.println("Ways Found: " + String(waysFound));
   if (waysFound < 2)
   { // No CrossRoads found; Focus on staying in the Middle;
      if (waysFound != 0)
      { // Exactly One Way was Found;
         if (inputs[0].value == 1)
         {
            // Go LEFT;
            move(0);
         }
         else if (inputs[1].value == 1)
         {
            // Go FORWARD;
            move(1);
            Distance distance[3];
            for (byte i = 0; i < 3; i++)
            {
               distance[i].sensor = i;
               distance[i].value = getDistance(1)[i].value;
            }

            if (distance[0].value > 5)
               Serial.println("LFT");
            digitalWrite(motor_L1, HIGH);
            digitalWrite(motor_L2, LOW);
            digitalWrite(motor_R1, HIGH);
            digitalWrite(motor_R2, LOW);
            delay(TURN_DELAY);
            // // Stop the Motors;
            Serial.println("STOP");
            digitalWrite(motor_L1, LOW);
            digitalWrite(motor_L2, LOW);
            digitalWrite(motor_R1, LOW);
            digitalWrite(motor_R2, LOW);
            delay(STOP_DELAY);
         }
         else
         {
            // Go RIGHT;
            move(2);
         }
      }
      else
      { // MALFUNCTION!
         // U Turn;
         move(3);
      }
      for (byte i = 0; i < 3; i++)
      {
         inputs[i] = getDistance(0)[i];
      }
      Serial.println("Inputs: " + String(inputs[0].value) + " " + String(inputs[1].value) + " " + String(inputs[2].value));
      waysFound = 0;
      for (byte i = 0; i < 3; i++)
      {
         if (inputs[i].value > 0)
            waysFound++;
      }
      Serial.println("Ways Found: " + String(waysFound));
      // Stop the Bot if it reaches end;
      if (currentPos[0] == end[0] && currentPos[1] == end[1])
      {
         digitalWrite(motor_L1, LOW);
         digitalWrite(motor_L2, LOW);
         digitalWrite(motor_R1, LOW);
         digitalWrite(motor_R2, LOW);
         Serial.println("End");
         digitalWrite(ledPin, HIGH);
         while (1)
            ;
      }
   }
   else
   {
      // If we are here, we have Found a CrossRoad; - astar
      Distance F[3];
      for (byte i = 0; i < 3; i++)
      {
         F[i].sensor = i;
         if (inputs[i].value > 0)
         {
            switch (i)
            {
            case 0: // LEFT
               F[0].value = (abs(start[0] - (currentPos[0] - 1)) + abs(start[1] - currentPos[1])) + (abs(end[0] - (currentPos[0] - 1)) + abs(end[1] - currentPos[1]));
               break;
            case 1: // FRONT
               F[1].value = (abs(start[0] - currentPos[0]) + abs(start[1] - (currentPos[1] + 1))) + (abs(end[0] - currentPos[0]) + abs(end[1] - (currentPos[1] + 1)));
               break;
            case 2: // RIGHT
               F[2].value = (abs(start[0] - (currentPos[0] + 1)) + abs(start[1] - currentPos[1])) + (abs(end[0] - (currentPos[0] + 1)) + abs(end[1] - currentPos[1]));
               break;
               // default:
               // F[i].value = 254;
            }
         }
         else
            F[i].value = INT16_MAX;
      }
      Serial.println("F Values: " + String(F[0].value) + ", " + String(F[1].value) + ", " + String(F[2].value));
      Serial.println("F Sensors: " + String(F[0].sensor) + ", " + String(F[1].sensor) + ", " + String(F[2].sensor));
      // Sort the Array in Ascending Order;
      for (byte i = 0; i < 3; i++)
      {
         for (byte j = i; j < 3; j++)
         {
            if (F[i].value > F[j].value)
            {
               Distance temp = F[i];
               F[i] = F[j];
               F[j] = temp;
            }
         }
      }
      if (F[0].value == F[1].value)
      {
         F[0].value -= (abs(start[0] - (currentPos[0] - 1)) + abs(start[1] - currentPos[1]));
         F[1].value -= (abs(start[0] - currentPos[0]) + abs(start[1] - (currentPos[1] + 1)));
      }
      if (F[0].value > F[1].value)
      {
         Distance temp = F[0];
         F[0] = F[1];
         F[1] = temp;
      }
      // Serial.println("F Values: " + String(F[0].value) + ", " + String(F[1].value) + ", " + String(F[2].value));
      // Serial.println("F Sensors: " + String(F[0].sensor) + ", " + String(F[1].sensor) + ", " + String(F[2].sensor));
      // TODO: If same F Values, Check for Lowest H value;
      // TODO: If same H values, Check for Lowest G value;
      // TODO: If same G values, Choose Path value Randomly;
      // Correct the Orientation;
      if (isFacing == 0)
      {
         Serial.println("RGT");
         digitalWrite(motor_L1, LOW);
         digitalWrite(motor_L2, HIGH);
         digitalWrite(motor_R1, LOW);
         digitalWrite(motor_R2, HIGH);
         delay(TURN_DELAY);
         // // Stop the Motors;
         Serial.println("STOP");
         digitalWrite(motor_L1, LOW);
         digitalWrite(motor_L2, LOW);
         digitalWrite(motor_R1, LOW);
         digitalWrite(motor_R2, LOW);
         delay(STOP_DELAY * 2);
         if (isFacing == 3)
            isFacing = 0;
         else
            isFacing++;
      }
      else if (isFacing == 2)
      {
         Serial.println("LFT");
         digitalWrite(motor_L1, HIGH);
         digitalWrite(motor_L2, LOW);
         digitalWrite(motor_R1, HIGH);
         digitalWrite(motor_R2, LOW);
         delay(TURN_DELAY);
         // // Stop the Motors;
         Serial.println("STOP");
         digitalWrite(motor_L1, LOW);
         digitalWrite(motor_L2, LOW);
         digitalWrite(motor_R1, LOW);
         digitalWrite(motor_R2, LOW);
         delay(STOP_DELAY * 2);
         if (isFacing == 0)
            isFacing = 3;
         else
            isFacing--;
      }
      else if (isFacing == 3)
      {
         Serial.println("BCK");
         digitalWrite(motor_L1, LOW);
         digitalWrite(motor_L2, HIGH);
         digitalWrite(motor_R1, LOW);
         digitalWrite(motor_R2, HIGH);
         delay(TURN_DELAY * 2);
         // Stop the Motors;
         Serial.println("STOP");
         digitalWrite(motor_L1, LOW);
         digitalWrite(motor_L2, LOW);
         digitalWrite(motor_R1, LOW);
         digitalWrite(motor_R2, LOW);
         delay(STOP_DELAY * 2);
         isFacing = isFacing + 2;
         if (isFacing > 3)
            isFacing = isFacing - 4;
      }
      move(F[0].sensor);
      // Stop the Bot if if reaches end;
      if (currentPos[0] == end[0] && currentPos[1] == end[1])
      {
         digitalWrite(motor_L1, LOW);
         digitalWrite(motor_L2, LOW);
         digitalWrite(motor_R1, LOW);
         digitalWrite(motor_R2, LOW);
         Serial.println("End");
         digitalWrite(ledPin, HIGH);
         while (1)
            ;
      }
   }
}

Distance *getDistance(bool mode)
{
   static Distance distance[3];
   if (mode == 0)
   { // Low Accuracy Mode; Only tells if there was a Way or Not;
      for (byte i = 0; i < 3; i++)
      {
         distance[i].sensor = i;
         distance[i].value = ((scanner[i].ping_cm() > TRIGGER_DISTANCE) ? 1 : 0);
         delay(sensordelay);
      }
   }
   else
   { // High Accuracy Mode; Tells the Exact Distance from each Sensor;
      for (byte i = 0; i < 3; i++)
      {
         distance[i].sensor = i;
         distance[i].value = ((byte)scanner[i].ping_cm());
         
         delay(sensordelay); //sensor delay to prevent interference
         if (i!=1){
         checkwall(distance[i].value, i);}
      }
   }
   return distance;
}

void move(int way)
{
   switch (way)
   {
   case 0: // Left;
      // move(1);
      Serial.println("LFT");
      digitalWrite(motor_L1, HIGH);
      digitalWrite(motor_L2, LOW);
      digitalWrite(motor_R1, HIGH);
      digitalWrite(motor_R2, LOW);

      delay(TURN_DELAY);
      // // Stop the Motors;
      Serial.println("STOP");
      digitalWrite(motor_L1, LOW);
      digitalWrite(motor_L2, LOW);
      digitalWrite(motor_R1, LOW);
      digitalWrite(motor_R2, LOW);
      delay(STOP_DELAY);
      if (isFacing == 0)
         isFacing = 3;
      else
         isFacing--;
      // // Move Forward 1 Segment;
      move(1);
      break;
   case 1: // Front;
      Serial.println("FWD");
      digitalWrite(motor_L1, LOW);
      digitalWrite(motor_L2, HIGH);
      digitalWrite(motor_R1, HIGH);
      digitalWrite(motor_R2, LOW);
      delay(FWD_DELAY);
      // // Stop the Motors;
      Serial.println("STOP");
      digitalWrite(motor_L1, LOW);
      digitalWrite(motor_L2, LOW);
      digitalWrite(motor_R1, LOW);
      digitalWrite(motor_R2, LOW);
      if (isFacing == 0)
      {
         currentPos[0]--;
      }
      else if (isFacing == 1)
      {
         currentPos[1]++;
      }
      else if (isFacing == 2)
      {
         currentPos[0]++;
      }
      else
      {
         currentPos[1]--;
      }
      // delay(STOP_DELAY);
      break;
   case 2: // Right;
      // move(1);
      Serial.println("RGT");
      digitalWrite(motor_L1, LOW);
      digitalWrite(motor_L2, HIGH);
      digitalWrite(motor_R1, LOW);
      digitalWrite(motor_R2, HIGH);
      delay(TURN_DELAY);
      // // Stop the Motors;
      Serial.println("STOP");
      digitalWrite(motor_L1, LOW);
      digitalWrite(motor_L2, LOW);
      digitalWrite(motor_R1, LOW);
      digitalWrite(motor_R2, LOW);
      delay(STOP_DELAY);
      if (isFacing == 3)
         isFacing = 0;
      else
         isFacing++;
      // Move Forward 1 Segment;
      move(1);
      break;
   case 4: // Back;
      Serial.println("CSTM");
      move(3);
      delay(TURN_DELAY);
      move(2);
   default: // Error; Do a U Turn;
      Serial.println("BCK");
      digitalWrite(motor_L1, LOW);
      digitalWrite(motor_L2, HIGH);
      digitalWrite(motor_R1, LOW);
      digitalWrite(motor_R2, HIGH);
      delay(TURN_DELAY * 2);
      // Stop the Motors;
      Serial.println("STOP");
      digitalWrite(motor_L1, LOW);
      digitalWrite(motor_L2, LOW);
      digitalWrite(motor_R1, LOW);
      digitalWrite(motor_R2, LOW);
      delay(STOP_DELAY * 2);
      isFacing = isFacing + 2;
      if (isFacing > 3)
         isFacing = isFacing - 4;
      // Move Forward 1 Segment;
      move(1);
      break;
   }
   Serial.println("Coords: " + String(currentPos[0]) + ", " + String(currentPos[1]));
}

void checkwall(byte walldistance, byte sensor)
{
  
  if (walldistance<wallgap && walldistance>wallgap/2){
    Serial.println("SLOW TRIGGER TOWARDS WALL");
      checkwallmove(0,i);
      
      // digitalWrite(motor_L1, LOW);
      // digitalWrite(motor_L2, HIGH);
      // digitalWrite(motor_R1, LOW);
      // digitalWrite(motor_R2, HIGH);
      // delay(wall_near_delay);
    else if (walldistance<=wallgap/2 && walldistance>0){
      Serial.println("BIG TRIGGER TOWARDS WALL");
      checkwallmove(1,i);
      // digitalWrite(motor_L1, LOW);
      // digitalWrite(motor_L2, HIGH);
      // digitalWrite(motor_R1, LOW);
      // digitalWrite(motor_R2, HIGH);
      // delay(wall_far_delay);
    }
  }
}
int walldelay(byte situation){
   switch (situation){
      case 1:
         return 25;
      case 2:
         return 50;
   }
}
void checkwallmove(byte situation, byte sensor){
   switch (sensor){
      case 0 :
         //move right for small duration
         Serial.println("RIGHT");
         digitalWrite(motor_L1, LOW);
         digitalWrite(motor_L2, HIGH);
         digitalWrite(motor_R1, LOW);
         digitalWrite(motor_R2, HIGH);
         delay(walldelay(situation));
         break;
      case 2:
         //move left for small duration
         Serial.println("LFT");
         digitalWrite(motor_L1, HIGH);
         digitalWrite(motor_L2, LOW);
         digitalWrite(motor_R1, HIGH);
         digitalWrite(motor_R2, LOW);
         delay(walldelay(situation));
         break;
   }


}