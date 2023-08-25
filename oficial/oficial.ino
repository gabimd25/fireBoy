// Simple 2 Wheel robot demonstrating the use of the ESP32 and the TB6612FNG Motor Controller

// This is a modified version of the sparkfun Dual Motor Controller TB6612FNG example sketch
// Which can be found here. https://www.sparkfun.com/products/14450?gclid=Cj0KCQiA6LyfBhC3ARIsAG4gkF8E3EeZWggXUgO7oEWK2d7iwPDjHLctu7CT5xEef9ki6SXmh0vvJ7waAuB7EALw_wcB
// The Library has been modified for the ESP32 by Pablo López
// It can be downloaded from Github https://github.com/pablopeza/TB6612FNG_ESP32
// Download the Zip file and install it in the Arduino IDE by clicking Sketch/ Include Library / Add Zip Library

// #include <Arduino.h>
#include <TB6612_ESP32.h>
#include "WiFi.h"
#include "seperatestring.h"
#include "AsyncUDP.h"


// *** Wiring connections from ESP32 to TB6612FNG Motor Controller ***
#define PWMA 12
#define AIN1 14
#define AIN2 27

#define BIN1 33 // ESP32 Pin D12 to TB6612FNG Pin BIN1
#define BIN2 25 // ESP32 Pin D27 to TB6612FNG Pin BIN2
#define PWMB 32 // ESP32 Pin D25 to TB6612FNG Pin PWMB
#define STBY 26 // ESP32 Pin D33 to TB6612FNG Pin STBY

#define CommandArray_SIZE   20


// these constants are used to allow you to make your motor configuration
// line up with function names like forward.  Value can be 1 or -1
const int offsetA = 1;
const int offsetB = 1;

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY,5000 ,8,1 );
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY,5000 ,8,2 );


char *CommandArray [CommandArray_SIZE]; // this is reused each call
char *strData = NULL; // this is allocated in separate and needs to be free( ) eventually

const char * ssid = "Gabi25";//Change your wifi name
const char * password = "milly123";//Change your wifi password
const char * BroadcastData = "";
AsyncUDP udp;
String PhrasedData = "";

int Speed=255;
int count = 1;

void setup()
{
  Serial.begin(115200);
  //-startwifi  
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED) {
        Serial.println("WiFi Failed");
        while(1) {
            vTaskDelay(500/ portTICK_RATE_MS);
        }
    }
      // Recieve data through wifi
  if(udp.listen(1234)) {//UDP port number if there is any problem change this.
        Serial.print("UDP Listening on IP: ");
        Serial.println(WiFi.localIP());
        udp.onPacket([] (AsyncUDPPacket packet) {      
            /*
            Serial.print(", From: ");
            Serial.print(packet.remoteIP());
            Serial.print(":");
            Serial.print(", Length: ");
            Serial.print(packet.length());
            Serial.print(", Data: ");
            Serial.write(packet.data(), packet.length());
            Serial.println();
            */
            //Data phrasing making recieved readable into string
            //packet.printf("Got %u bytes of data", packet.length());
            char* tmpStr = (char*) malloc(packet.length() + 1);
            memcpy(tmpStr, packet.data(), packet.length());
            tmpStr[packet.length()] = '\0'; // ensure null termination
            PhrasedData = String(tmpStr);
            free(tmpStr); // Strign(char*) creates a copy so we can delete our one
            Serial.println(PhrasedData);   
        });      
  }
}

void loop()
{

     //--delay(2000); // Short delay to catch your robot
      
     //Use of the drive function which takes as arguements the speed
     //and optional duration.  A negative speed will cause it to go
     //backwards.  Speed can be from -255 to 255.  Also use of the
     //brake function which takes no arguements.
     //motor1.drive(255,2000);       // Turn Motor 1 for 2 seconds at full speed
     //motor1.drive(-255,2000);
     //motor1.brake();
     //delay(2000);


     //Use of the forward function, which takes as arguements two motors
     //and optionally a speed.  If a negative number is used for speed
     //it will go backwards
     //--forward(motor1, motor2, 50);        // Forward Motor 1 and Motor 2 for 1 seconds at full speed
     //delay(1000);

     // Brake
     //brake(motor1, motor2);     // Stop Motor 1 and Motor 2 for 2 seconds 
     //delay(2000);       

     //Use of the back function, which takes as arguments two motors
     //and optionally a speed.  Either a positive number or a negative
     //number for speed will cause it to go backwards
     //back(motor1, motor2, -255);         // Reverse Motor 1 and Motor 2 for 1 seconds at full speed
     //delay(1000);

     // Brake again
     //brake(motor1, motor2);       // Stop Motor 1 and Motor 2 for 2 seconds 
     //delay(2000);  

     //Use of the left and right functions which take as arguements two
     //motors and a speed.  This function turns both motors to move in
     //the appropriate direction.  For turning a single motor use drive.
    //  left(motor1, motor2, 255);
    //  delay(2000);
    //  brake(motor1, motor2);
    //  delay(1000); 
    //  right(motor1, motor2, 255);
    //  delay(2000);

     // Brake again
    //  brake(motor1, motor2);       // Stop Motor 1 and Motor 2 for 2 seconds 
    //  delay(2000); 


  Plzgetstring(PhrasedData);// Get the incoming data and seprerate the commands into array.
  Speed = (String(CommandArray [1])).toInt();//Speed of the robot
  if(String(CommandArray [0])=="f"|| String(CommandArray [0])=="F")
  {
    Serial.println("To aqui na pra frente");
    forward(motor1, motor2, 255);        // Forward Motor 1 and Motor 2 for 1 seconds at full speed
  }
  if(String(CommandArray [0])=="b"|| String(CommandArray [0])=="B")
  {
    Serial.println("To aqui na pra trás"); 
    forward(motor1, motor2, -255);
  }
  if(String(CommandArray [0])=="L"|| String(CommandArray [0])=="l")
  {
    Serial.println("To aqui na pra frente"); 
    left(motor1, motor2, 255);
  }
  if(String(CommandArray [0])=="R"|| String(CommandArray [0])=="r")
  {
    Serial.println("To aqui na pra frente"); 
    right(motor1, motor2, 255);
  }
  if(String(CommandArray [0])=="S" || String(CommandArray [0])=="s")
  {
    Serial.println("To aqui na parada"); 
    brake(motor1, motor2);       // Stop Motor 1 and Motor 2 for 2 seconds 
  } 
}

void Plzgetstring(String phraseData)
{
  String str;
  int N = 0;
  str = phraseData;
  //Serial.print(F("input str:")); Serial.println(str);
  N = separate (str, CommandArray, CommandArray_SIZE,  &strData);
  // for (int n = 0; n < N; n++) {
    // Serial.println (CommandArray [n]);
  // }
  freeData(&strData);
}