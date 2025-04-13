#include "Communication/LoRaDuplex/LoRaDuplex.h"
#include "Display/Display.h"

LoRaDuplex lora;
Display display;

String currentMessage = "";  // Store the message as the user types

void setup()
{
    Serial.begin(115200);  // Initialize serial communication
    lora.setup();          // Setup LoRa sender
    display.setup();       // Setup display

    // Display a welcome message and instructions
    Serial.println("LoRa Communication System");
    Serial.println("Type your message and press Enter to send it.");
    display.printf("LoRa System Ready", 0);  // Display readiness message on the screen
}

void loop()
{
    // Check if there is any input from the user
    if (Serial.available() > 0)
    {
        char incomingChar = Serial.read();  // Read a single character

        // If the character is a newline (Enter), send the message
        if (incomingChar == '\n')
        {
            if (currentMessage.length() > 0)
            {
                // Send the message via LoRa
                lora.sendPacket(currentMessage);

                // Provide feedback to the user
                Serial.print("Message sent: ");
                Serial.println(currentMessage);
                Serial.println("Type another message.");

                // Display the sent message on the screen
                display.printf("Sent: %s", 0, currentMessage.c_str());

                currentMessage = "";  // Reset the message after sending
            }
        }
        // If the character is not Enter, add it to the current message
        else
        {
            currentMessage += incomingChar;

            display.printf("Typing: %s", 0, currentMessage.c_str());
        }
    }
}
