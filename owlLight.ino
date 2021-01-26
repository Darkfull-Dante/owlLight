#include <CapacitiveSensor.h>
#include <math.h>

//Constants
const int MAX_LIGHT_CYCLE = 9;
const long CAPACITIVE_TRESHOLD = 10000;
const int HOLD_DELAY = 750;
const float COMMON_MATH_VARIABLE = 180/PI;
const int PULSE_SPEED = 1;
const int POWER = 255;
const int LED_COUNT = 3;

//lamp status
bool isLampOn = false;
int lightCycle = 0;
float x = 0; //value for the rainbowcycle between 0 and pi

//capacitive touch global variable
CapacitiveSensor cs_4_2 = CapacitiveSensor(4,2);
bool isPressed = false;
bool isPressed_Old = false;

//press duration timer variables
int pressedTimer = 0;
int startPressed = 0;
int endPressed = 0;

//LED pins
int leds[] = {11, 10, 9};
int checkLight = 12;

//RGB color index
float RGB[3][MAX_LIGHT_CYCLE];

void setup(){

    //Set led pins
    for (int i = 0; i < LED_COUNT; i++)
    {
        pinMode(i, OUTPUT);
    }
    
    pinMode(checkLight, OUTPUT);

    Serial.begin(9600);

    //set capacitive touch
    cs_4_2.set_CS_AutocaL_Millis(0xFFFFFFFF);

    //start LED off
    for (int i = 0; i < LED_COUNT; i++)
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
    //Serial.print("C=");
    //Serial.println(capacitiveTouchValue);
    //isPressed = digitalRead(button) == HIGH;

    //determine if button is pressed or not based on capacitance value
    if (isPressed = capacitiveTouchValue < CAPACITIVE_TRESHOLD){ //use lower than when not on a propelly grounded system, higher than if properly grounded
        digitalWrite(checkLight, HIGH);
    }
    else{
        digitalWrite(checkLight, LOW);
    }

    if (isPressed != isPressed_Old)
    {
        updateState(isPressed);
    }

    isPressed_Old = isPressed;
    
    //turn LED off if isLampOn = false
    if (!isLampOn)
    {
        for (int i = 0; i < LED_COUNT; i++)
        {
            digitalWrite(leds[i], LOW);
        }
        
        
    }
    else
    {
        if (lightCycle == 0)
        {
            rainbowCycle();
        }
        else if (lightCycle > 0 && lightCycle < MAX_LIGHT_CYCLE)
        {
            staticCycle(lightCycle);
        }
    }
    
    //start rainbow cycle if cycle 0
    
    
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
        Serial.println(pressedTimer);

        if (pressedTimer >= HOLD_DELAY)
        {
            isLampOn = !isLampOn;
        }
        else if (pressedTimer >= 5 && isLampOn)
        {
            
            //increase the lightCycle
            lightCycle++;

            //if over limit of light cycle revert to 0
            if (lightCycle >= MAX_LIGHT_CYCLE)
            {
                lightCycle = 0;
            }
        }   
    }
}

void rainbowCycle(){

    //Cycle from 0 to pi
    if (x >= PI)
    {
        x = 0;
    }
    else
    {
        x += 0.00001;
    }
    
    //determine value for each LED
    RGB[0][0] = POWER * abs(sin(x*(COMMON_MATH_VARIABLE)));             //RED LED
    RGB[1][0] = POWER * abs(sin((x+PI/3)*(COMMON_MATH_VARIABLE)));      //GREEN LED
    RGB[2][0] = POWER * abs(sin((x+(2*PI)/3)*(COMMON_MATH_VARIABLE)));  //BLUE LED

    /*
    Serial.print("R=");
    Serial.print(RGB[0][0]);
    Serial.print(", G=");
    Serial.print(RGB[1][0]);
    Serial.print(", B=");
    Serial.println(RGB[2][0]);
    */

    //change color of LEDS
    if (isLampOn)
    {

        for (int i = 0; i < LED_COUNT; i++)
        {
            analogWrite(leds[i], RGB[i][0]);
        }
        
    }

    //calculate delay for each color
    for (int i = 0; i < LED_COUNT; i++)
    {
        if (RGB[i][0] < 1)
        {
            delay(20 * PULSE_SPEED);
        }
        else if (RGB[i][0] < 5)
        {
            delay(10 * PULSE_SPEED);
        }
        else if (RGB[i][0] < 10)
        {
            delay(2 * PULSE_SPEED);
        }
        else if (RGB[i][0] < 100)
        {
            delay(1 * PULSE_SPEED);
        }
    }
    
    delay(1);

}

void staticCycle(int cycleNumber){
    if (isLampOn)
    {
        for (int i = 0; i < LED_COUNT; i++)
        {
            analogWrite(leds[i], RGB[i][cycleNumber]);
        }
    }
    
}
