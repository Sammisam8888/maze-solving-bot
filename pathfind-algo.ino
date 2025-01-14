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
#define TRIGGER_DISTANCE 10 // Distance after which CrossRoad Code is Triggered;

const byte mapSize[2] = {MAX_SEGMENTS, MAX_SEGMENTS}; // Map Size {x, y}; Assumes Origin at {0, 0};
const byte start[2] = {0, 0};                         // Starting Coordinate {x, y};
const byte end[2] = {MAX_SEGMENTS, MAX_SEGMENTS};     // Ending Coordinate {x, y};

// Variables Here:
byte isFacing = 0;           // 0 = North, 1 = East, 2 = South, 3 = West;
byte currentPos[2] = {0, 0}; // Current position {x, y};

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
   CrossRoad(byte x, byte y)
   { // Constructor:
      coords[0] = x;
      coords[1] = y;
      visitedWays[0] = visitedWays[1] = visitedWays[2] = 0;
   }

   bool isVisited(byte way) const { return visitedWays[way]; } // 0 = Not Visited, 1 = Visited;

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
   bool waysFound = false; // Check if any way is found;
   while (!waysFound)
   {
      waysFound = checkDistance(0);
      delay(50);
   }
   // If we are Here, then we have found a path.
   Distance distances[3];
   for (int i = 1; i < 4; i++)
   {
      distances[i - 1].sensor = i - 1;           // Set the sensor number;
      distances[i - 1].value = checkDistance(i); // Set the distance;
      delay(50);
   }
   // If we are Here, then we have found all distances.
   // TODO: Everything Else Here; NOTE: Do all changes to "distances.value" ONLY; the changes are automatically reflected in "distances.sensor";
}

byte checkDistance(byte mode)
{
   switch (mode)
   {
   case 0:
      bool waysFound = false;
      for (int i = 0; i < 3; i++)
      {
         if (scanner[i].ping_cm() > TRIGGER_DISTANCE)
         {
            waysFound = true;
         }
      }
      return waysFound;

   default:
      return (round((scanner[mode - 1].ping_cm()) / SEGMENT_LENGTH));
   }
}