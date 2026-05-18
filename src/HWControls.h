// This optional setting causes Encoder to use more optimized code,
// It must be defined before Encoder.h is included.
#define ENCODER_OPTIMIZE_INTERRUPTS
#include <Encoder.h>
#include <Bounce.h>
#include "TButton.h"

#include "Rotary.h"
#include "RotaryEncOverMCP.h"

// Pins for MCP23017
#define GPA0 0
#define GPA1 1
#define GPA2 2
#define GPA3 3
#define GPA4 4
#define GPA5 5
#define GPA6 6
#define GPA7 7
#define GPB0 8
#define GPB1 9
#define GPB2 10
#define GPB3 11
#define GPB4 12
#define GPB5 13
#define GPB6 14
#define GPB7 15

#define OSC1_LEVEL_BUTTON 0
#define OSC2_LEVEL_BUTTON 1
#define PORTAMENTO_BUTTON 2

void RotaryEncoderChanged(bool clockwise, int id);

void mainButtonChanged(Button *btn, bool released);

Adafruit_MCP23017 mcp1;
Adafruit_MCP23017 mcp2;
Adafruit_MCP23017 mcp3;
Adafruit_MCP23017 mcp4;
Adafruit_MCP23017 mcp5;

//Array of pointers of all MCPs
Adafruit_MCP23017 *allMCPs[] = { &mcp1, &mcp2, &mcp3, &mcp4, &mcp5 };

/* Array of all rotary encoders and their pins */
RotaryEncOverMCP rotaryEncoders[] = {
  RotaryEncOverMCP(&mcp1, 1, 0, &RotaryEncoderChanged, 1),
  RotaryEncOverMCP(&mcp1, 3, 2, &RotaryEncoderChanged, 2),
  RotaryEncOverMCP(&mcp1, 5, 4, &RotaryEncoderChanged, 3),
  RotaryEncOverMCP(&mcp1, 8, 9, &RotaryEncoderChanged, 4),
  RotaryEncOverMCP(&mcp1, 10, 11, &RotaryEncoderChanged, 5),
  RotaryEncOverMCP(&mcp1, 12, 13, &RotaryEncoderChanged, 6),

  RotaryEncOverMCP(&mcp2, 1, 0, &RotaryEncoderChanged, 7),
  RotaryEncOverMCP(&mcp2, 3, 2, &RotaryEncoderChanged, 8),
  RotaryEncOverMCP(&mcp2, 5, 4, &RotaryEncoderChanged, 9),
  RotaryEncOverMCP(&mcp2, 8, 9, &RotaryEncoderChanged, 10),
  RotaryEncOverMCP(&mcp2, 10, 11, &RotaryEncoderChanged, 11),
  RotaryEncOverMCP(&mcp2, 12, 13, &RotaryEncoderChanged, 12),

  RotaryEncOverMCP(&mcp3, 1, 0, &RotaryEncoderChanged, 13),
  RotaryEncOverMCP(&mcp3, 3, 2, &RotaryEncoderChanged, 14),
  RotaryEncOverMCP(&mcp3, 5, 4, &RotaryEncoderChanged, 15),
  RotaryEncOverMCP(&mcp3, 8, 9, &RotaryEncoderChanged, 16),
  RotaryEncOverMCP(&mcp3, 10, 11, &RotaryEncoderChanged, 17),
  RotaryEncOverMCP(&mcp3, 12, 13, &RotaryEncoderChanged, 18),

  RotaryEncOverMCP(&mcp4, 1, 0, &RotaryEncoderChanged, 19),
  RotaryEncOverMCP(&mcp4, 3, 2, &RotaryEncoderChanged, 20),
  RotaryEncOverMCP(&mcp4, 5, 4, &RotaryEncoderChanged, 21),
  RotaryEncOverMCP(&mcp4, 8, 9, &RotaryEncoderChanged, 22),
  RotaryEncOverMCP(&mcp4, 10, 11, &RotaryEncoderChanged, 23),
  RotaryEncOverMCP(&mcp4, 12, 13, &RotaryEncoderChanged, 24),

  RotaryEncOverMCP(&mcp5, 1, 0, &RotaryEncoderChanged, 25),
  RotaryEncOverMCP(&mcp5, 3, 2, &RotaryEncoderChanged, 26),
  RotaryEncOverMCP(&mcp5, 5, 4, &RotaryEncoderChanged, 27),
  RotaryEncOverMCP(&mcp5, 8, 9, &RotaryEncoderChanged, 28),
  RotaryEncOverMCP(&mcp5, 10, 11, &RotaryEncoderChanged, 29),
  RotaryEncOverMCP(&mcp5, 12, 13, &RotaryEncoderChanged, 30),
  RotaryEncOverMCP(&mcp5, 6, 14, &RotaryEncoderChanged, 31),

};

Button osc1_level_Button = Button(&mcp1, 6, OSC1_LEVEL_BUTTON, &mainButtonChanged);
Button osc2_level_Button = Button(&mcp1, 14, OSC2_LEVEL_BUTTON, &mainButtonChanged);
Button portamento_Button = Button(&mcp4, 14, PORTAMENTO_BUTTON, &mainButtonChanged);

// after your rotaryEncoders[] definition
constexpr size_t NUM_MCP = sizeof(allMCPs) / sizeof(allMCPs[0]);
constexpr int numMCPs = (int)(sizeof(allMCPs) / sizeof(*allMCPs));
constexpr int numEncoders = (int)(sizeof(rotaryEncoders) / sizeof(*rotaryEncoders));

Button *mainButtons[] = {
  &osc1_level_Button, &osc2_level_Button, &portamento_Button
};

Button *allButtons[] = {
  &osc1_level_Button, &osc2_level_Button, &portamento_Button
};

// an array of vectors to hold pointers to the encoders on each MCP
std::vector<RotaryEncOverMCP *> encByMCP[NUM_MCP];

//Teensy 4.1 Pins

#define POLY1_SW 33
#define POLY2_SW 34
#define UNISON_SW 35
#define CHORUS_SW 40
#define BEND_VCF_SW 18
#define KBDTRACK_SW 19
#define POLARITY_SW 7

#define POLY1_LED 36
#define POLY2_LED 37
#define UNISON_LED 38
#define SAVE_LED 39
#define CHORUS_LED 28
#define BEND_VCF_LED 21
#define KBDTRACK_RED_LED 22
#define KBDTRACK_GREEN_LED 20
#define VCF_POLARITY_LED 8

#define RECALL_SW 15
#define SAVE_SW 41
#define SETTINGS_SW 12
#define BACK_SW 10

#define ENCODER_PINA 4
#define ENCODER_PINB 5

#define DEBOUNCE 30

static long encPrevious = 0;

//These are pushbuttons and require debouncing

TButton saveButton{ SAVE_SW, LOW, HOLD_DURATION, DEBOUNCE, CLICK_DURATION };
TButton settingsButton{ SETTINGS_SW, LOW, HOLD_DURATION, DEBOUNCE, CLICK_DURATION };
TButton backButton{ BACK_SW, LOW, HOLD_DURATION, DEBOUNCE, CLICK_DURATION };
TButton recallButton{ RECALL_SW, LOW, HOLD_DURATION, DEBOUNCE, CLICK_DURATION }; // on encoder

TButton poly1Button{ POLY1_SW, LOW, HOLD_DURATION, DEBOUNCE, CLICK_DURATION };
TButton poly2Button{ POLY2_SW, LOW, HOLD_DURATION, DEBOUNCE, CLICK_DURATION };
TButton unisonButton{ UNISON_SW, LOW, HOLD_DURATION, DEBOUNCE, CLICK_DURATION }; 

TButton chorusButton{ CHORUS_SW, LOW, HOLD_DURATION, DEBOUNCE, CLICK_DURATION };
TButton bendvcfButton{ BEND_VCF_SW, LOW, HOLD_DURATION, DEBOUNCE, CLICK_DURATION };
TButton kbdtrackButton{ KBDTRACK_SW, LOW, HOLD_DURATION, DEBOUNCE, CLICK_DURATION }; 
TButton polarityButton{ POLARITY_SW, LOW, HOLD_DURATION, DEBOUNCE, CLICK_DURATION }; 

Encoder encoder(ENCODER_PINB, ENCODER_PINA);  //This often needs the pins swapping depending on the encoder

void setupHardware() {

  //Switches
  pinMode(RECALL_SW, INPUT_PULLUP);  //On encoder
  pinMode(SAVE_SW, INPUT_PULLUP);
  pinMode(SETTINGS_SW, INPUT_PULLUP);
  pinMode(BACK_SW, INPUT_PULLUP);

  pinMode(POLY1_SW, INPUT_PULLUP);
  pinMode(POLY2_SW, INPUT_PULLUP);
  pinMode(UNISON_SW, INPUT_PULLUP);

  pinMode(CHORUS_SW, INPUT_PULLUP);
  pinMode(BEND_VCF_SW, INPUT_PULLUP);
  pinMode(KBDTRACK_SW, INPUT_PULLUP);
  pinMode(POLARITY_SW, INPUT_PULLUP);

  pinMode(POLY1_LED, OUTPUT);
  pinMode(POLY2_LED, OUTPUT);
  pinMode(UNISON_LED, OUTPUT);
  pinMode(SAVE_LED, OUTPUT);

  pinMode(CHORUS_LED, OUTPUT);
  pinMode(BEND_VCF_LED, OUTPUT);
  pinMode(KBDTRACK_RED_LED, OUTPUT);
  pinMode(KBDTRACK_GREEN_LED, OUTPUT);
  pinMode(VCF_POLARITY_LED, OUTPUT);
}
