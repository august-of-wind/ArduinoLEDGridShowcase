  
  // Sample Arduino Duemilanove interface code for Sparkfun's
  // LED Matrix - Serial Interface - Red/Green/Blue
  // http://www.sparkfun.com/commerce/product_info.php?products_id=760
  //
  // Written by:
  // Fabio Pugliese Ornellas
  // fabio.ornellas@gmail.com
  
  // **Modified to showcase device capability by:
  // **August Meyer 3.21.15
  // **All documentation starting with ** is written by August Meyer.
  
  //**I used this code (attributed above) as a basis for my own experiments working with the RGB LED Grid component.
  //**The code that is not mine is simply code (very generously written by Fabio Pugliese Ornellas) that sets the grid
  //  to be in singular, non-daisy-chained mode, and sets up a method for sending char arrays of color values to the LED Grid index
  //  by index.
  
  
  // Pin definitions for Duemilanove.
  // If you have a different board, check its datasheet SPI
  // documentation for the corret pins.
  
  //**Testing was done on Arduino Uno, which has the same SPI definitions as Duemilanove
  
  
  #define PIN_SCK  13
  #define PIN_MISO 12
  #define PIN_MOSI 11
  #define PIN_SS   10
  // Send a single byte via SPI, taken from Atmel's datasheet example.
  void sendChar(char cData){
    SPDR = cData;
    while(!(SPSR&(1<<SPIF)));
  }
  
  // Send a full frame to the LED matrix
  void sendFrame(char *frame) {
    // Assert SS
    digitalWrite(PIN_SS, LOW);
    // delay as the LED Matrix datasheet's recommends
    delayMicroseconds(500);
    // send the full buffer
    for(int i=0;i<64;i++) {
      char c;
      c=*(frame+i);
      // This is needed because sending a '%' will reconfigure the
      // board for daisy chain operation
      if('%'==c)
        sendChar((2<<5)&(1<<2)&1); // similar color
      else
        sendChar(c);
    }
    // de-assert SS
    digitalWrite(PIN_SS, HIGH);
    // The LED Matrix datasheet's recommends this delay on daisy
    // chain configurations
    //delayMicroseconds(10);
  }
  
  void setup(){
    // SPI communication setup. Please refer to Atmel's datasheets
    // to understand all this. Basically it sets up the Arduino as
    // master and configure the appropriate clock speed.
  
    // Configuration register
    SPCR=(1<<SPE)|(1<<MSTR)|(1<<SPR1)|(1<<SPR0);
    // Status register
    SPSR = SPSR & B11111110;
  
    // setup pins
    pinMode(PIN_SCK, OUTPUT);
    digitalWrite(PIN_SCK, LOW); // ensure clock low
    pinMode(PIN_MOSI, OUTPUT);
    pinMode(PIN_SS, OUTPUT);
    digitalWrite(PIN_SS, HIGH); // de-assert SS
    delayMicroseconds(500); // delay as the LED Matrix datasheet's recommends
  
    // This section reconfigure the board to no daisy chain
    // operation.
    // This can be confirmed by an RRGGBB pattern beginning at a
    // corner, with no black LEDs before the first LED, when the
    // matrix is powered on.
    // Warning: this will take effect only after first run, power
    // off and power on!
    digitalWrite(PIN_SS, LOW);
    delayMicroseconds(500);
    sendChar('%');
    sendChar(1);
    digitalWrite(PIN_SS, HIGH);
    delayMicroseconds(10);
  }
  
  //**The following two methods, makeFrown() and makeSmile() do as their titles would imply - they fill the LED Grid with colored smiley and frowny faces.
  // To do this, I fill the frameBuffer with colors unique to their place on the grid - inserting one color into the LED's that make up the facial features, and another color into the LED's that make up
  // the rest of the face (i.e., the rest of the grid). I then invoke sendFrame(frameBuffer), which sends the frame of color values to the LED grid itself. I then delay for 200 milliseconds, as is recommended by the sources
  // I found online, apparently in order to decrease risk of corrupted images.
  
  void makeFrown(){
    //**colors chosen to pop out for easy visibility
      int color1 = 2;
      int color2 = 220;
      char frameBuffer[64];
      //**EYES: Color eyes with color2
      frameBuffer[41] = color2;
      frameBuffer[42] = color2;
      frameBuffer[45] = color2;
      frameBuffer[46] = color2;
      frameBuffer[49] = color2;
      frameBuffer[50] = color2;
      frameBuffer[53] = color2;
      frameBuffer[54] = color2;
      
      //**FROWN: Color frown with color2
      frameBuffer[1] = color2;
      frameBuffer[6] = color2;
      frameBuffer[9] = color2;
      frameBuffer[14] = color2;
      frameBuffer[18] = color2;
      frameBuffer[21] = color2;
      frameBuffer[27] = color2;
      frameBuffer[28] = color2;
      //**Make everything else a solid, different color.
      for(int i=0; i<= 63; i++){
        if(i!=1 && i!=6 && i!=9 && i!=14 && i!=18 && i!=21 && i!=27 && i!=28 && i!=41 && i!=42 && i!=45 && i!=46 && i!= 49 && i!=50 && i!=53 && i!=54){
          frameBuffer[i] = color1;
        }
      }
      //**send the frameBuffer to the grid
      sendFrame(frameBuffer);
      //**delay, as recommended
      delay(200);
  }
  
  void makeSmile(){
    //**colors chosen to pop out for easy visibility
      int color1 = 2;
      int color2 = 220;
      char frameBuffer[64];
      //**EYES: Color eyes with color2
      frameBuffer[41] = color2;
      frameBuffer[42] = color2;
      frameBuffer[45] = color2;
      frameBuffer[46] = color2;
      frameBuffer[49] = color2;
      frameBuffer[50] = color2;
      frameBuffer[53] = color2;
      frameBuffer[54] = color2;
      
      //**SMILE: Color smile with color2
      frameBuffer[3] = color2;
      frameBuffer[4] = color2;
      frameBuffer[10] = color2;
      frameBuffer[13] = color2;
      frameBuffer[17] = color2;
      frameBuffer[22] = color2;
      frameBuffer[25] = color2;
      frameBuffer[30] = color2;
     //**Make everything else a solid, different color
      for(int i=0; i<=63; i++){
        if(i!=3 && i!=4 && i!=10 && i!=13 && i!=17 && i!=22 && i!=25 && i!=30 && i!=41 && i!=42 && i!=45 && i!=46 && i!= 49 && i!=50 && i!=53 && i!=54){
          frameBuffer[i] = color1;
        }
      }
      //**send the frameBuffer to the grid
      sendFrame(frameBuffer);
      //**delay, as recommended
      delay(200);
  }
  
 //**This method is inteded to showcase all 255 colors the LED grid can display - in addition to no color at all.
 //  I do this by incrementing color values from 0 to 255, filling the grid up with 64 unique colors at a time by sending 3 unique frameBuffers.
  void display255(){
   char frameBuffer[64];
   //**color starts at 0...
   int color = 0;
   //**cycle through the first 63 colors, in addition to no color at all.
   for(int i=0; i<=63;i++){
     frameBuffer[i] = color;
     color++;
   }
   //**send the full-colored frame to the grid.
   sendFrame(frameBuffer);
   //**delay for 1 second for visibility purposes.
   delay(1000);
   
   //**start a new cycle, this time with the 64th color
   int color1 = 64;
   for(int i=0; i<=63;i++){
     frameBuffer[i] = color1;
     color1++;
   }
   //**send the second cycle of colors to the grid
   sendFrame(frameBuffer);
   //**delay for 1 second for visibility purposes
   delay(1000);
   
   //**start a new cycle, this time with the 191th color
   int color2 = 191;
   for(int i=0; i<=63;i++){
     frameBuffer[i] = color2;
     color2++;
   }
   //**send the final cycle to the grid, thus showcasing all 255 colors + no color at all.
   sendFrame(frameBuffer);
   //**Delay for 200 miliseconds, as per documentation to reduce risk of corrupted image.
   delay(200);
  }
  
  void loop(){
    //**I now invoke makeSmil(), makeFrown(), then display255() in quick succession to showcase a simple animation, then a cycle of all displayable colors.
      makeSmile();
      
      delay(200);
      
      makeFrown();
      
      delay(200);
      
      display255();
      
      delay(200);
    }
