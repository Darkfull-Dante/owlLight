#include <CapacitiveSensor.h>
#include <math.h>

//Constants
const int MAX_LIGHT_CYCLE = 9;
const long CAPACITIVE_TRESHOLD = 20000;
const int HOLD_DELAY = 500;

//lamp status
bool isLampOn = false;
int lightCycle = 0;

//capacitive touch global variable
CapacitiveSensor cs_4_2 = CapacitiveSensor(4,2);
bool isPressed = false;
bool isPressed_Old = false;

//press duration timer variables
int pressedTimer = 0;
int startPressed = 0;
int endPressed = 0;

//LED pins
int[] leds = {11, 10, 9};

//RGB color index
float RGB[3][MAX_LIGHT_CYCLE];

void setup(){
    //Set led pins
    int count = leds.size();

    for (int i = 0; i < count; i++)
    {
        pinMode(i, OUTPUT);
    }

    //set capacitive touch
    cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);

    //start LED off
    for (int i = 0; i < count; i++)
    {
        digitalWrite(i, LOW);
    }
    

    //create RGB color code value
    RGB[0][1] = 255;    RGB[1][1] = 0;      RGB[2][1] = 0;      //red
    RGB[0][2] = 255;    RGB[1][2] = 36;     RGB[2][2] = 0;      //orange
    RGB[0][3] = 255;    RGB[1][3] = 100;    RGB[2][3] = 0;      //yellow
    RGB[0][4] = 0;      RGB[1][4] = 255;    RGB[2][4] = 0;      //green
    RGB[0][5] = 0;      RGB[1][5] = 0;      RGB[2][5] = 255;    //blue
    RGB[0][6] = 128;    RGB[1][6] = 0;      RGB[2][6] = 255;    //purple
    RGB[0][7] = 255;    RGB[1][7] = 0;      RGB[2][7] = 128;    //pink
    RGB[0][8] = 255;    RGB[1][8] = 255;    RGB[2][8] = 255;    //white
    
}

void loop(){

    //read the value of the capacitive button
    long capacitiveTouchValue = cs_4_2.capacitiveSensor(10);

    //determine if button is pressed or not based on capacitance value
    isPressed = capacitiveTouchValue > CAPACITIVE_TRESHOLD;

    if (isPressed != isPressed_Old)
    {
        updateState(isPressed);
    }
    
    //turn LED off if isLampOn = false

    //start rainbow cycle if cycle 0

    //start static cycle if not 0

}

void updateState(bool state){
    
    //check for start and end time of pressed button
    if (state) {
        startPressed = millis();
    }
    else {

        //calculate pressed time
        endPressed = millis();
        pressedTimer = endPressed - startPressed;

        if (pressedTimer >= HOLD_DELAY)
        {
            isLampOn = !isLampOn;
        }
        
    }
    
}