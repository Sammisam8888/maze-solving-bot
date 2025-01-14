#define PLAGRARISM // Uncomment this Line to enable the Library & Disable the Manual Calculation;
#ifdef PLAGRARISM
#include <NewPing.h>
#endif

#define SONAR_NUM 3
// Number of sensors.

#define MAX_DISTANCE 100 // Maximum distance (in cm) to ping.
const int SONAR_PIN[SONAR_NUM][2] = {{4, 5}, {6, 7}, {8, 9}};

// Pin Numbers Stored as 2D Array;
unsigned int SONAR_DISTANCE[SONAR_NUM] = {0};

// Variable Storing Distance;
#ifdef PLAGRARISM
NewPing sonar[SONAR_NUM] = {
    // Sensor object array.
    NewPing(SONAR_PIN[0][0], SONAR_PIN[0][1], MAX_DISTANCE), // Syntax: NewPing(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE)

    NewPing(SONAR_PIN[1][0], SONAR_PIN[1][1], MAX_DISTANCE),

    NewPing(SONAR_PIN[2][0], SONAR_PIN[2][1], MAX_DISTANCE),
};

#endif
void setup()
{

   Serial.begin(9600);

   // Open serial monitor at 9600 baud to see ping results.

   while (!Serial)

   // Wait for Slower Boards to catch up;      ;

#ifndef PLAGRARISM                        // Library Code Activated, this not required;
      for (int i = 0; i < SONAR_NUM; i++) // For every sensor;

      {

         for (int j = 0; j < 2; j++) // For every pin of the sensor;
         {
            pinMode(SONAR_PIN[i][0], OUTPUT); // Setting Trig Pin as Output;
            pinMode(SONAR_PIN[i][1], INPUT);  // Setting Echo Pin as Input;      }   }
#endif
            Serial.println(F("Ready!"));
         }
         void loop()
         {
#ifdef PLAGRARISM // Library Code;
            for (uint8_t i = 0; i < SONAR_NUM; i++)
            {             // Loop through each sensor and display results.
               delay(30); // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
               Serial.print(i + 1);
               Serial.print("=");
               Serial.print(sonar[i].ping_cm());
               Serial.print("cm ");
            }
            Serial.println();
#else // Actual Code;
   for (int i = 0; i < SONAR_NUM; i++) // For every sensor;
   {
      calculateDistance(i); // Distance calculating Function;
      // Print distance calculation for Debugging/Processing;
      Serial.print(i + 1);
      Serial.print(F("="));
      Serial.print(SONAR_DISTANCE[i]);
      Serial.print(F("cm "));
   }
   Serial.println();
#endif }
#ifndef PLAGRARISM // Library Code Activated, this not required;
            void calculateDistance(int NUM)
            {
               int trigPin = SONAR_PIN[NUM][0];
               int echoPin = SONAR_PIN[NUM][1];
               digitalWrite(trigPin, LOW);                     // For a clear Signal;
               delayMicroseconds(2);                           // Wait for Processor to Catch Up;
               digitalWrite(trigPin, HIGH);                    // Start the Signal;
               delayMicroseconds(10);                          // Wait for Signal to Propagate;
               digitalWrite(trigPin, LOW);                     // Stop the Signal;
               unsigned int duration = pulseIn(echoPin, HIGH); // Get ready to Receive the Signal;
               unsigned int distance = duration * 0.034 / 2;   // Speed of Sound ~ 343 m/s = 0.0343 cm/uS;
               SONAR_DISTANCE[NUM] = distance;                 // Save the Value in the Array;
            }
#endif