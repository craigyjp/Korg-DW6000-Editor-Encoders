//Values below are just for initialising and will be changed when synth is initialised to current panel controls & EEPROM settings
byte midiChannel = MIDI_CHANNEL_OMNI;//(EEPROM)
byte midiOutCh = 1;//(EEPROM)

unsigned long lastDisplayTriggerTime = 0;
bool waitingToUpdate = false;
const unsigned long displayTimeout = 5000;  // e.g. 5 seconds
bool halfStepMode = false;

char buffer[12];

int MIDIThru = midi::Thru::Off;//(EEPROM)
String patchName = INITPATCHNAME;
String bankdir = "/Bank";
boolean encCW = true; //This is to set the encoder to increment when turned CW - Settings Option
boolean recallPatchFlag = true;
boolean loadFactory = false;
boolean loadFromDW = false;
boolean dataInProgress = false;
int currentSendPatch = 0;
boolean saveCurrent = false;
boolean saveAll = false;
boolean updateParams = false;  //(EEPROM)
int bankselect = 0;
int old_value = 0;
int old_param_offset = 0;

// adding encoders
bool rotaryEncoderChanged(int id, bool clockwise, int speed);
#define NUM_ENCODERS 31
unsigned long lastTransition[NUM_ENCODERS + 1];
boolean accelerate = true;
int speed = 1;
int value = 0;
float lastSpeed[NUM_ENCODERS + 1] = { 0 }; // Or whatever your encoder count is

int osc1_octave = 0;
int osc1_waveform = 0;
int osc1_level = 0;

int osc2_octave = 0;
int osc2_waveform = 0;
int osc2_level = 0;
int osc2_interval = 0;
int osc2_detune = 0;

int wave_bank = 0;
int wave_banka = 0;
int wave_bankb = 0;
int temp_wave_bank = 0;

int noise = 0;

int vcf_cutoff = 0;
int vcf_res = 0;
int vcf_kbdtrack = 0;
int vcf_polarity = 0;
int vcf_eg_intensity = 0;

int chorus = 0;

int vcf_attack =  0;
int vcf_decay =  0;
int vcf_breakpoint = 0;
int vcf_slope = 0;
int vcf_sustain = 0;
int vcf_release = 0;

int vca_attack = 0;
int vca_decay = 0;
int vca_breakpoint = 0;
int vca_slope = 0;
int vca_sustain = 0;
int vca_release = 0;

int mg_frequency = 0;
int mg_delay = 0;
int mg_osc = 0;
int mg_vcf = 0;

int bend_osc = 0;
int bend_vcf = 0;
int bend_vcfstr = 0;

int glide_time = 0;

int poly1 = 0;
int poly2 = 0;
int unison = 0;
int polymode = 0;

int returnvalue = 0;

//Pick-up - Experimental feature
//Control will only start changing when the Knob/MIDI control reaches the current parameter value
//Prevents jumps in value when the patch parameter and control are different values
boolean pickUp = false;//settings option (EEPROM)
boolean pickUpActive = false;
#define TOLERANCE 2 //Gives a window of when pick-up occurs, this is due to the speed of control changing and Mux reading
