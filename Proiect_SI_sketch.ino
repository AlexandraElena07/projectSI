#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
 
HUSKYLENS huskylens;
SoftwareSerial huskylensSerial(5, 6); // huskylens camera serial Rx, Tx
SoftwareSerial hc05(3, 4);  // bluetooth module serial Rx, Tx

int vibration_input = 7; // vibration sensor signal digital input on the arduino
int vibration_sensor_state = 0; // vibration sensor state flag variable that indicates whether vibration was detected or not 

const int ultrasonic_trigPin = 9; // ultrasonic sensor trig pin on the arduino
const int ultrasonic_echoPin = 10;  // ultrasonic sensor echo pin on the arduinp

long ultrasonic_duration; // variable in which to store duration in which the pulse has been transmitted & reflected
int ultrasonic_distance;  // variable in which to store calculated distance of detected object

bool ultrasonic_is_object_close = 0;  // ultrasonic sensor state flag variable that indicates whether close object was detected or not
unsigned long ultrasonic_milis_when_object_got_close = 0; // variable in which we store time stamp in ms when close object was detected

unsigned long huskylens_milis_when_object_was_detected = 0; // variable in which we store time stamp in ms when object was detected by huskylens camera

unsigned long ultrasonic_current_milis = 0; // current ms time stamp - auxiliary variable that aids in calculating time elapsed since object got close
unsigned long time_elapsed_since_object_first_got_close_milis = 0;  // variable in which we store calculated time elapsed since object got close

unsigned long huskylens_start_milis = 0;  // auxiliary variable which aids in calculating time elapsed since object got detected by huskylens
unsigned long huskylens_end_milis = 0;  // auxiliary variable which aids in calculating time elapsed since object got detected by huskylens
unsigned long time_elapsed_since_object_first_was_detected_milis = 0; // variablein which we store calculated time elapsed since object got detected by huskylens

int number_of_pictures_saved_to_SD_card = 0;  // counter variable that stores number of pictures saved to SD card by husklyens

void huskylens_begin_serial() {
  Serial.println("-------------------------------------");

  huskylensSerial.begin(9600);

  while (!huskylens.begin(huskylensSerial)) {
    Serial.println(F("Begin failed!"));
    Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>Serial 9600)"));
    Serial.println(F("2.Please recheck the connection."));
    delay(100);
  }
}

void huskylens_end_serial() {
  huskylensSerial.end();
}

void huskylens_do_setup() { // setup function for huskylens - set desired algorithm (object tracking) & load model from SD card
  while(!huskylens.writeAlgorithm(ALGORITHM_OBJECT_TRACKING)) {
    Serial.println(F("Error on huskylens object recognition algorithm write attempt."));
  }
  Serial.println(F("Huskylens object recognition algorithm write success."));

  while(!huskylens.loadModelFromSDCard(4)) {
    Serial.println(F("Error on loading wheel model from sd card to huskylens attempt."));
  }
  Serial.println(F("Loading wheel model from sd card to huskylens success.")); 
}

void digital_pin_set_value_with_delay(int digital_pin_no, int mode, int delay_ms) { // function to control arduino digital pin - used for controlling D2 pin which
                                                                                    // triggers the turning on / off of the huskylens camera
  delay(delay_ms);
  digitalWrite(digital_pin_no, mode == 0? LOW : HIGH);
  delay(delay_ms);  
}

void setup() {
  pinMode(vibration_input, INPUT);  // setting sensor pins as input / output
  pinMode(2, OUTPUT);

  pinMode(ultrasonic_trigPin, OUTPUT);
  pinMode(ultrasonic_echoPin, INPUT); 

  Serial.begin(115200); // start serial communication

  digital_pin_set_value_with_delay(2, 1, 1000); // turn on huskylens camera for setup
  huskylens_begin_serial();

  huskylens_do_setup();

  digital_pin_set_value_with_delay(2, 0, 1000); // huskylens setup complete, turn off camera
  huskylens_end_serial();
}

void loop() {
  // ULTRASONIC SENSOR  

  // clears the trigPin
  digitalWrite(ultrasonic_trigPin, LOW);
  delayMicroseconds(2);

  // sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(ultrasonic_trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(ultrasonic_trigPin, LOW);
  
  // reads the echoPin, returns the sound wave travel time in microseconds
  ultrasonic_duration = pulseIn(ultrasonic_echoPin, HIGH);

  // calculating the distance to object
  ultrasonic_distance = ultrasonic_duration * 0.034 / 2;
  
  // prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(ultrasonic_distance);

  if(!ultrasonic_is_object_close && ultrasonic_distance < 30) { // if object is not already close & distance to nearby object is less than 30cm
    Serial.print("Distance: ");
    Serial.println(ultrasonic_distance);

    Serial.println("OBJECT IS CLOSE");
    ultrasonic_milis_when_object_got_close = millis();  // set timestamp when object got close
    ultrasonic_is_object_close = 1; // set object close flag to true
  }

  if(ultrasonic_is_object_close) {  // if object has been detected as being close
    digital_pin_set_value_with_delay(2, 1, 1000); // turn on huskylens camera by setting D2 pin to HIGH
    huskylens_begin_serial();

    ultrasonic_current_milis = millis();

    time_elapsed_since_object_first_got_close_milis = ultrasonic_current_milis - ultrasonic_milis_when_object_got_close;  // calculate time since object got close

    while (time_elapsed_since_object_first_got_close_milis <= 180000) { // loop for 3 minutes since object got close

      while (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
      if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
    
      else if(!huskylens.available()) { // available() returns the count of blocks and arrows available to read - so if it's 0, not block that matches model is detected
        Serial.println(F("No block or arrow appears on the screen!"));        
      }
      
      else {
        Serial.println(F("###########"));
        while (huskylens.available()) { // block or arrow appeared on the screen
          HUSKYLENSResult result = huskylens.read();  // read() reads blocks or arrows
        
          if (result.command == COMMAND_RETURN_BLOCK) { // command is used to determine whether detected instance is arrow or block
            Serial.println(String()+F("Block:xCenter=")+result.xCenter+F(",yCenter=")+result.yCenter+F(",width=")+result.width+F(",height=")+result.height+F(",ID=")+result.ID);

            huskylens_start_milis = millis();
            huskylens_end_milis = huskylens_start_milis;

            time_elapsed_since_object_first_was_detected_milis = huskylens_end_milis - huskylens_start_milis; // calculate time since object was detected by huskylens
            while (time_elapsed_since_object_first_was_detected_milis <= 60000) { // loop for 1 minute since object was detected by huskylens

              // VIBRATION SENSOR
              Serial.print("Vibration status: ");

              vibration_sensor_state = digitalRead(vibration_input);  // read vibration state

              if (vibration_sensor_state == 1) {  // print vibration state
                Serial.println("Sensing vibration");
              } else {
                Serial.println("No vibration");
              }

              if (vibration_sensor_state == 1) {  // vibration is detected

                while(number_of_pictures_saved_to_SD_card < 5) {  // save 5 pictures to SD card by looping until counter (initially 0, incremented on every loop) is 5
                  while(!huskylens.savePictureToSDCard()) { // try to save picture to SD card
                    Serial.println(F("Error on save picture to SD card attempt")); 
                    delay(100);
                    break;                  
                  } 
                    Serial.println(F("Saving picture to SD card..."));
                    number_of_pictures_saved_to_SD_card++;
                    delay(500);
                                
                }                

                huskylens_end_serial(); // end huskylens serial so bluetooth module serial can begin
                delay(500);

                hc05.begin(19200);  // begin bluetooth module serial
                delay(500);
                hc05.println(" Scratched car!");

                delay(500);
                hc05.end(); // end bluetooth module serial so huskylens serial can begin

                delay(500);
                huskylens_begin_serial(); // begin huskylens serial
              }

              huskylens_end_milis = millis();
              time_elapsed_since_object_first_was_detected_milis = huskylens_end_milis - huskylens_start_milis; // update time elapsed since object was detected by huskylens
            }
            delay(50);
          }
        } 
      } 

      time_elapsed_since_object_first_got_close_milis = ultrasonic_current_milis - ultrasonic_milis_when_object_got_close;  // update time elapsed since object got close
    } 

    digital_pin_set_value_with_delay(2, 0, 1000); // 3 minutes since objec got close elapsed -> turn off huskylens camera
    huskylens_end_serial();

    number_of_pictures_saved_to_SD_card = 0;  // reset counters and flags
    ultrasonic_is_object_close = 0;
    ultrasonic_milis_when_object_got_close = 0;
  }
}
