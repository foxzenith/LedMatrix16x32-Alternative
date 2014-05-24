/*
#include <gamma.h>
#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#define F2(progmem_ptr) (const __FlashStringHelper *)progmem_ptr

#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);
uint8_t color_r = 0;
void setup() {
	matrix.begin();
	matrix.setTextWrap(false); // Allow text to run off right edge
	matrix.setTextSize(2);
}
void lohop() {
	byte i;
	color_r = (color_r + 1) % 8;
	matrix.drawPixel(1, 0, matrix.Color333(color_r, 0, 0));
	matrix.swapBuffers(true);
	delay(100);

	
}*/






#define CLK 8  // MUST be on PORTB! (Use pin 11 on Mega)
#define LAT A3
#define OE  9

#define A   A0
#define B   A1
#define C   A2

#define R1   2
#define G1   3
#define B1   4
#define R2   5
#define G2   6
#define B2   7

#define DATAPORT PORTD
#define DATADIR  DDRD
#define SCLKPORT PORTB

int nBanks = 4;
int nbytesInDatarow = 64;
uint8_t nPlanes = 3; //Minimum 2

unsigned char bank = nBanks - 1;
unsigned char plane = nPlanes - 1;

uint8_t *matrixbuff[2];
uint8_t sclkpin;
volatile uint8_t *buffptr;


long previousMillis = 0;        // will store last time LED was updated
long interval = 20;           // interval at which to blink (milliseconds)

uint8_t pixel_x_pos = 0;
uint8_t pixel_y_pos = 0;

void setup()
{



	pinMode(CLK, OUTPUT);
	pinMode(LAT, OUTPUT);
	pinMode(OE , OUTPUT);

	pinMode(A, OUTPUT);
	pinMode(B, OUTPUT);
	pinMode(C, OUTPUT);

	pinMode(R1, OUTPUT);
	pinMode(G1, OUTPUT);
	pinMode(B1, OUTPUT);
	pinMode(R2, OUTPUT);
	pinMode(G2, OUTPUT);
	pinMode(B2, OUTPUT);

	/// TEST START ///
	digitalWrite(CLK, LOW); 
	digitalWrite(LAT, LOW);
	digitalWrite(OE, HIGH); //Disable output
	
	// choose bank
	digitalWrite(A, LOW);
	digitalWrite(B, LOW);
	digitalWrite(C, LOW);


	
	bool dbuf = false;
	int buffsize = nbytesInDatarow * nPlanes * nBanks;
	int allocsize = (dbuf == true) ? (buffsize * 2) : buffsize;
	if (NULL == (matrixbuff[0] = (uint8_t *)malloc(allocsize))) return;
	memset(matrixbuff[0], 0, allocsize);
	// If not double-buffered, both buffers then point to the same address:
	matrixbuff[1] = (dbuf == true) ? &matrixbuff[0][buffsize] : matrixbuff[0];

	sclkpin = digitalPinToBitMask(CLK);
	buffptr = matrixbuff[0];

	dumpMatrix();
	
	/*matrixbuff[0][0] = 0b00000100;
	matrixbuff[0][1] = 0b00000100;
	matrixbuff[0][2] = 0b00000100;
	matrixbuff[0][3] = 0b00001000;*/
	
	matrixbuff[0][32] = 0b00000100;
	matrixbuff[0][33] = 0b00000100;
	matrixbuff[0][34] = 0b00000100;
	matrixbuff[0][35] = 0b00001000;
	
	
	matrixbuff[0][64] = 0b00000100;
	matrixbuff[0][65] = 0b00000100;
	matrixbuff[0][66] = 0b00000100;
	matrixbuff[0][67] = 0b00001000;
	
	matrixbuff[0][124] = 0b00000100;
	matrixbuff[0][125] = 0b00000100;
	matrixbuff[0][126] = 0b00000100;
	matrixbuff[0][127] = 0b00001000;


	matrixbuff[0][128] = 0b00000100;
	matrixbuff[0][129] = 0b00000100;
	matrixbuff[0][130] = 0b00000100;
	matrixbuff[0][131] = 0b00001000;

	/*
	matrixbuff[0][32] = 0b00000100;
	matrixbuff[0][33] = 0b00000100;
	matrixbuff[0][34] = 0b00000100;
	matrixbuff[0][35] = 0b00001000;
	*/

	/*

	matrixbuff[0][384] = 0b00000100;
	*/
	dumpMatrix();
	

}

void loop()
{
	unsigned long currentMillis = millis();

	if (currentMillis - previousMillis > interval) {
		previousMillis = currentMillis;

		//matrixbuff[0][pixel_x_pos] = 0;
		drawPixel((int16_t)pixel_x_pos, (int16_t)pixel_y_pos, 0);

		//Running pixel
		if (++pixel_x_pos >= 32){
			pixel_x_pos = 0;
			if (++pixel_y_pos >= 16){
				pixel_y_pos = 0;
			}
		}

		//matrixbuff[0][pixel_x_pos] = 0b00000100;
		drawPixel((int16_t)pixel_x_pos, (int16_t)pixel_y_pos, Color333(0, 7, 0));
		//dumpMatrix();
		//delay(10);
	}
	
	
	
	/*
	int row = 0;
	//SET DATA
	if (row == 0){
		buffptr = matrixbuff[0];
		digitalWrite(R1, HIGH);
		digitalWrite(G1, LOW);
		digitalWrite(B1, LOW);
		digitalWrite(R2, HIGH);
		digitalWrite(G2, LOW);
		digitalWrite(B2, LOW);
	}
	if (row == 1){
		digitalWrite(R1, LOW);
		digitalWrite(G1, LOW);
		digitalWrite(B1, LOW);
		digitalWrite(R2, LOW);
		digitalWrite(G2, HIGH);
		digitalWrite(B2, LOW);
	}
	if (row == 2){
		digitalWrite(R1, LOW);
		digitalWrite(G1, LOW);
		digitalWrite(B1, LOW);
		digitalWrite(R2, LOW);
		digitalWrite(G2, LOW);
		digitalWrite(B2, HIGH);
	}
	if (row == 3){
		digitalWrite(R1, LOW);
		digitalWrite(G1, LOW);
		digitalWrite(B1, LOW);
		digitalWrite(R2, LOW);
		digitalWrite(G2, LOW);
		digitalWrite(B2, LOW);
	}
	

	//Clock out data
	for (int j = 0; j < 2; j++){
		for (int i = 0; i < 32; i++){
			digitalWrite(CLK, HIGH);
			digitalWrite(CLK, LOW);
			digitalWrite(R1, LOW);
			digitalWrite(G1, LOW);
			digitalWrite(B1, LOW);
			digitalWrite(R2, LOW);
			digitalWrite(G2, LOW);
			digitalWrite(B2, LOW);
		}
	}

	//LATCH
	//Disable Output during bank switching
	digitalWrite(OE, HIGH);
	digitalWrite(LAT, HIGH);
	bank = (bank + 1) % 4;
	while (bank != row){
		bank = (bank + 1) % 4;
		if (bank & 0x1)
			digitalWrite(A, HIGH);
		else
			digitalWrite(A, LOW);

		if (bank & 0x2)
			digitalWrite(B, HIGH);
		else
			digitalWrite(B, LOW);
	}
	digitalWrite(LAT, LOW);
	digitalWrite(OE, LOW);
	*/

	updateDisplay();
	
	
	
}

void drawPixel(int16_t x, int16_t y, uint16_t c) {
	uint8_t r, g, b, bit, limit, *ptr;
	// Adafruit_GFX uses 16-bit color in 5/6/5 format, while matrix needs
	// 4/4/4.  Pluck out relevant bits while separating into R,G,B:
	r = c >> 12;        // RRRRrggggggbbbbb
	g = (c >> 7) & 0xF; // rrrrrGGGGggbbbbb
	b = (c >> 1) & 0xF; // rrrrrggggggBBBBb

	// Loop counter stuff
	limit = 1 << nPlanes;

	
	uint16_t baseaddr = (y % 4) * nbytesInDatarow * nPlanes;
	if (y%8 < 4){
		baseaddr += ((x >> 3) + 1) * 16 - 1 - x % 8;
	}
	else{
		baseaddr += (x >> 3) * 16 + x % 8;
	}
	ptr = &matrixbuff[0][baseaddr];
	if (y < 8){
		for (bit = 1; bit < limit; bit <<= 1) {
			*ptr &= ~B00011100;             // Mask out R,G,B in one op
			if (r & bit) *ptr |= B00000100;  // Plane N R: bit 2
			if (g & bit) *ptr |= B00001000;  // Plane N G: bit 3
			if (b & bit) *ptr |= B00010000;  // Plane N B: bit 4
			ptr += 64;                  // Advance to next bit plane
		}
	}
	else{
		
		for (bit = 1; bit < limit; bit <<= 1) {
		*ptr &= ~B11100000;             // Mask out R,G,B in one op
		if (r & bit) *ptr |= B00100000;  // Plane N R: bit 5
		if (g & bit) *ptr |= B01000000;  // Plane N G: bit 6
		if (b & bit) *ptr |= B10000000;  // Plane N B: bit 7
		ptr += 64;                  // Advance to next bit plane
		}
	}
	

	

}

void updateDisplay(void){
	//Disable Output during bank switching
	digitalWrite(OE, HIGH);
	digitalWrite(LAT, HIGH);

	plane++;
	if (plane >= nPlanes){
		plane = 0;
		bank++;
		if (bank >= nBanks){
			bank = 0;
			/*Swapping*/
			buffptr = matrixbuff[0];
		}
	}
	else if (plane == 1){
		// Plane 0 was loaded on prior interrupt invocation and is about to
		// latch now, so update the row address lines before we do that:

		//TEST BANKS!

		if (bank & 0x1)
			digitalWrite(A, HIGH);
		else
			digitalWrite(A, LOW);

		if (bank & 0x2)
			digitalWrite(B, HIGH);
		else
			digitalWrite(B, LOW);
	}
	


	digitalWrite(LAT, LOW);
	digitalWrite(OE, LOW);


	uint8_t tock = SCLKPORT;
	uint8_t tick = tock | sclkpin;
	
	uint8_t *ptr;
	ptr = (uint8_t *)buffptr;


	#define pew asm volatile(               \
	"ld  __tmp_reg__, %a[ptr]+"    "\n\t"   \
	"out %[data]    , __tmp_reg__" "\n\t"   \
	"out %[clk]     , %[tick]"     "\n\t"   \
	"out %[clk]     , %[tock]"     "\n"     \
	:: [ptr]  "e" (ptr),                    \
	[data] "I" (_SFR_IO_ADDR(DATAPORT)),	\
	[clk]  "I" (_SFR_IO_ADDR(SCLKPORT)),	\
	[tick] "r" (tick),						\
	[tock] "r" (tock));

	// Loop is unrolled for speed:
	pew pew pew pew pew pew pew pew
	pew pew pew pew pew pew pew pew
	pew pew pew pew pew pew pew pew
	pew pew pew pew pew pew pew pew
	pew pew pew pew pew pew pew pew
	pew pew pew pew pew pew pew pew
	pew pew pew pew pew pew pew pew
	pew pew pew pew pew pew pew pew

	buffptr += 0;

}

// Promote 3/3/3 RGB to Adafruit_GFX 5/6/5
uint16_t Color333(uint8_t r, uint8_t g, uint8_t b) {
	// RRRrrGGGgggBBBbb
	return ((r & 0x7) << 13) | ((r & 0x6) << 10) |
		((g & 0x7) << 8) | ((g & 0x7) << 5) |
		((b & 0x7) << 2) | ((b & 0x6) >> 1);
}

void dumpMatrix(void) {

	int i, buffsize = nbytesInDatarow * nPlanes * nBanks;

	Serial.print(" \n --- \n{\n");

	for (i = 0; i<buffsize; i++) {
		if (matrixbuff[0][i] < 0x10) Serial.print(' ');
		if (matrixbuff[0][i] == 0) Serial.print(' ');
		else Serial.print(matrixbuff[0][i], HEX);
		if (i < (buffsize - 1)) {
			if ((i & 63) == 63) Serial.print(",\n  ");
			else             Serial.print(',');
		}
	}
	Serial.println("\n};");
}
