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
#define SEGMENT_LENGTH 20   // Length in cm;
#define MAX_SEGMENTS 10     // Number of Segments in Maze;
#define MAX_DISTANCE 100    // Maximum distance to Measure;
#define TRIGGER_DISTANCE 15 // Distance after which CrossRoad Code is Triggered;
#define deg90delay 2000    //this is the delay for moving left and right - 90 degree, 20 cm 
#define fwd0delay 2000     //this is the delay for moving forward - 0 degree, 20 cm
#define deg180delay 2000   //this is the delay for the bot to move in reverse direction - 180 degree, 20 cm

const byte mapSize[2] = {MAX_SEGMENTS, MAX_SEGMENTS}; // Map Size {x, y}; Assumes Origin at {0, 0};
const byte start[2] = {0, 0};                         // Starting Coordinate {x, y};
const byte end[2] = {MAX_SEGMENTS, MAX_SEGMENTS};     // Ending Coordinate {x, y};

// Global Variables Here:
byte isFacing = 0;           // 0 = North, 1 = East, 2 = South, 3 = West;
byte currentPos[2] = {0, 0}; // Current position {x, y};
byte dirsign=1;

// OOP Here:
NewPing scanner[3] = {NewPing(sensPin[0][0], sensPin[0][1], MAX_DISTANCE),
                      NewPing(sensPin[1][0], sensPin[1][1], MAX_DISTANCE),
                      NewPing(sensPin[2][0], sensPin[2][1], MAX_DISTANCE)}; // Order: Left, Front, Right;

class CrossRoad
{ // Makes the CrossRoad thing so easy to access;
private:
   byte coords[2] = {0};      // Coordinates {x, y} wrt Origin;
   bool visitedWays[3] = {0}; // 0 = Not Visited, 1 = Visited; {Left, Front, Right};
public:
   checkpoint(byte x, byte y)
   { 
      coords[0] = x;
      coords[1] = y;
      visitedWays[0] = visitedWays[1] = visitedWays[2] = 0;
   }

   bool isVisited(byte way) const { return visitedWays[way]; } // 0 = Not Visited, 1 = Visited; {Left, Front, Right}(way=0,1,2);

   void markVisited(byte way) { visitedWays[way] = 1; } // Sets the Way as Visited;

   byte getCoords(byte axis) const { return coords[axis]; } // Returns the Coordinates of that Axis;
};

struct Distance
{ // New Struct for Defining the Distance related with each Sensor; Perform all changes in '.value' and it reflects on the '.sensor';
   byte value = 0;
   byte sensor = 0;
};

void setup()
{
}

void loop()
{
   byte waysFound = 0; // Check if any way is found;
   byte way = 1;      // Current Way;

   while (waysFound < 2)
   { // Check if atleast 2 Ways (CrossRoads) are found;
      waysFound = checkDistance(0);
      if (waysFound < 1)
      { // Weird, all Paths are Closed;
        // MALFUNCTION!!!! (Or Probably Look Around); (Trishna) [OPTIONAL]
         move(3); //reverse turn and move
      }
      else if (waysfound==1)
      { // We still have a Way to Go;
         if (checkDistance(2) > TRIGGER_DISTANCE)
         { // Front Sensor has a Path;
           // Move Forward one Segment & Don't Hit Side Walls! (Trishna)
            move(1); //front side only way - move forward
         }
         else if (checkDistance(1) > TRIGGER_DISTANCE)
         { // We have a COMPULSORY TURN;
           move(0); //left side turn - only way
         }
         else if (checkDistance(3) > TRIGGER_DISTANCE){
            move(3); //right side turn - only way
         }
      }
      delay(50);
   }
   // If we are Here, then we have found a CrossRoad.
   // Turn and (i) Save this for 2nd try; 
   //(ii) Check for a Crossroad AFTER crossing one Segment (Hint: use DELAYS); (Trishna)
   
   byte crossnum=0;
   CrossRoad obj[100];
   obj[crossnum].checkpoint(currentPos[0],currentPos[1]); //0-xcoord,1-ycoord
   crossnum++;

   Distance distances[3];
   for (int i = 1; i < 4; i++)
   {
      distances[i - 1].sensor = i - 1;           // Set the sensor number;
      distances[i - 1].value = checkDistance(i); // Set the distance;
      delay(50);
   }
   // If we are Here, then we have found all distances.
   Distance *F = calculateA(distances);
   // If we are here, we have the Array of F Values sorted in Ascending Order.
   // Choose a Path & Move ONE segment; (Trishna)
}

byte checkDistance(byte mode)
{
   switch (mode)
   {
   case 0:
      byte waysFound = 0;
      for (int i = 0; i < 3; i++)
      {
         if (scanner[i].ping_cm() > TRIGGER_DISTANCE)
         {
            waysFound++;
         }
      }
      return waysFound;
      break;
   default:
      return (round((scanner[mode - 1].ping_cm()))); // / SEGMENT_LENGTH));
   }
}

Distance *calculateA(Distance *distances)
{
   Distance x[3];
   for (byte i = 0; i < 3; i++)
   {
      x[i].value = distances[i].value;
      x[i].sensor = distances[i].sensor;
      
   }
   byte arr[3];
   for (byte i = 0; i < 3; i++)
   {
      if(x[i].value>TRIGGER_DISTANCE)
         arr[i]=1;
      else 
         arr[i]=0;
   }
   // Access this like x[i].value = 0 and stuff;
   
   astaralgo(&currentPos, &arr);
   
   // SAMUEL!!! DO STUFF HEREEEE (Like the Actual Algorithm); - done in astaralgo function
   // TODO: Everything Else Here; NOTE: Do all changes to "distances.value" ONLY; the changes are automatically reflected in "distances.sensor";
   return x;
}

byte astaralgo(auto *locator,byte* a){
   byte xcoord=*locator[0];
   byte ycoord=*locator[1];
   int arr[3]; //left-0,front-1,right-2
   for (i=0;i<3;i++){
      if (*a==1){
      switch(i){
         case 0: //left dir
               xord=xcoord-1;
               yord=ycoord;
               break;
         case 1: //front dir
            xord=xcoord;
            yord=ycoord+1;
            break;
         case 2: //right dir
            xord=xcoord+1;
            yord=ycoord;
            break;
         default:
            break;
      }
      a++;
      int g=modulus(xord-start[0])+modulus(yord-start[1]);
      int h=modulus(xord-end[0])+modulus(yord-end[1]);
      int f=g+h;
      arr[i]=f;}
      else 
         a++;
         continue;
         arr[i]=10000;
   }
   }
   byte min=arr[0];
   byte minindex=0;
   for (i=1;i<3;i++){
      if (arr[i]<min){
         min=arr[i];
         minindex=i;
      }
   }
   move (minindex);
   }
}
void move(byte x)
{
   switch (x)
   {
   case 0: // move Left;
      digitalWrite(motor_L1, HIGH);
      digitalWrite(motor_L2, LOW);
      digitalWrite(motor_R1, LOW);
      digitalWrite(motor_R2, HIGH);
      isFacing--;
      dircheck(isFacing);
      direction(isFacing);
      currentPos[0]+=dirsign*1;
      delay(deg90delay);
      //move forward 1 block in left direction
      digitalWrite(motor_L1, HIGH);
      digitalWrite(motor_L2, LOW);
      digitalWrite(motor_R1, HIGH);
      digitalWrite(motor_R2, LOW);
      delay(90degdelay);
      
      break;
   case 1: // move Forward;
      digitalWrite(motor_L1, HIGH);
      digitalWrite(motor_L2, LOW);
      digitalWrite(motor_R1, HIGH);
      digitalWrite(motor_R2, LOW);
      delay(fwd0delay);
      currentPos[1]+=dirsign*1;
      
      break;
   case 2: // move Right;
      digitalWrite(motor_L1, LOW);
      digitalWrite(motor_L2, HIGH);
      digitalWrite(motor_R1, HIGH);
      digitalWrite(motor_R2, LOW);
      currentPos[0]+=dirsign*1;
      isFacing++;
      dircheck(isFacing);
      direction(isFacing);
      delay(deg90delay);
      //move forward 1 block in right direction
      digitalWrite(motor_L1, HIGH);
      digitalWrite(motor_L2, LOW);
      digitalWrite(motor_R1, HIGH);
      digitalWrite(motor_R2, LOW);
      delay(fwd0delay);
      currentPos[1]+=dirsign*1;

      break;
   case 3: // move reverse-u turn;
      digitalWrite(motor_L1, LOW);
      digitalWrite(motor_L2, HIGH);
      digitalWrite(motor_R1, HIGH);
      digitalWrite(motor_R2, LOW);
      delay(deg180delay);
      isFacing-=2;
      dircheck(isFacing);
      //move forward 1 block in backward direction
      digitalWrite(motor_L1, HIGH);
      digitalWrite(motor_L2, LOW);
      digitalWrite(motor_R1, HIGH);
      digitalWrite(motor_R2, LOW);
      delay(fwd0delay);
      currentPos[0]+=dirsign*1;


      break;
   default: // ERRRORORORORORRRRR;
      break;
   }
}
void swap(byte m,byte n){
   byte temp=m;
   m=n;
   n=temp;
}
void direction(isFacing){
   if (isFacing==3 || isFacing==1)
         swap(currentPos[0],currentPos[1]); //currentpos of x and y are interchanged for changing axis by 90degree - clockwise now 0 will point to y and 1 will point to x
      if (isFacing==3 || isFacing ==2)
         dirsign=-1; //if the bot is facing any of the direction like south or west then the dirsign will be -1
      else
         dirsign=1;

}
void dircheck(isFacing){
   if (isFacing==4)
      isFacing=0;   
   else if (isFacing==-1)
      isFacing=3;
}
int modulus(int val){
   if (val<0)
      return -val;
   else 
      return val;
}