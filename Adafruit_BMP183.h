/*************************************************** 
  This is a library for the Adafruit BMP183 Barometric Pressure + Temp sensor

  Designed specifically to work with the Adafruit BMP183 Breakout 
  ----> http://www.adafruit.com/products/1900

  These sensors use SPI to communicate, 4 pins are required to  
  interface
  Adafruit invests time and resources providing this open source code, 
  please support Adafruit and open-source hardware by purchasing 
  products from Adafruit!

  Written by Limor Fried/Ladyada for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
 ****************************************************/

#ifndef ADAFRUIT_BMP183_H
#define ADAFRUIT_BMP183_H

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include "SPI.h"

// use the 'default' datasheet numbers to test calculations
#define BMP183_DEBUG 0

/*=========================================================================
    REGISTERS
    -----------------------------------------------------------------------*/
    enum
    {
      BMP183_REGISTER_CAL_AC1            = 0xAA,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CAL_AC2            = 0xAC,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CAL_AC3            = 0xAE,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CAL_AC4            = 0xB0,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CAL_AC5            = 0xB2,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CAL_AC6            = 0xB4,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CAL_B1             = 0xB6,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CAL_B2             = 0xB8,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CAL_MB             = 0xBA,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CAL_MC             = 0xBC,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CAL_MD             = 0xBE,  // R   Calibration data (16 bits)
      BMP183_REGISTER_CHIPID             = 0xD0,
      BMP183_REGISTER_VERSION            = 0xD1,
      BMP183_REGISTER_SOFTRESET          = 0xE0,
      BMP183_REGISTER_CONTROL            = 0xF4,
      BMP183_REGISTER_TEMPDATA           = 0xF6,
      BMP183_REGISTER_PRESSUREDATA       = 0xF6,
      BMP183_REGISTER_READTEMPCMD        = 0x2E,
      BMP183_REGISTER_READPRESSURECMD    = 0x34
    };
/*=========================================================================*/

/*=========================================================================
    MODE SETTINGS
    -----------------------------------------------------------------------*/
    typedef enum
    {
      BMP183_MODE_ULTRALOWPOWER          = 0,
      BMP183_MODE_STANDARD               = 1,
      BMP183_MODE_HIGHRES                = 2,
      BMP183_MODE_ULTRAHIGHRES           = 3
    } bmp183_mode_t;
/*=========================================================================*/

class Adafruit_BMP183 {
 public:
  explicit Adafruit_BMP183(int8_t SPICS);
  Adafruit_BMP183(int8_t SPICLK, int8_t SPIMISO, int8_t SPIMOSI, int8_t SPICS);

  boolean begin(bmp183_mode_t mode = BMP183_MODE_ULTRAHIGHRES);  // by default go highres
  
  // getTemperature() blocks for 5ms (delay needed for BMP183 to complete
  // its measurement).
  float getTemperature(void);
  // Async variant which returns immediately after initiating temperature
  // reading. Returns number of milliseconds caller must wait before finishing
  // the operation.  Note: after calling startGetTemperature(), do not call
  // any other methods on this object before calling finishGetTemperature(), 
  uint8_t startGetTemperature(void);  // Returns number of milliseconds.
  float finishGetTemperature(void);
  
  // getPressure() blocks for up to 26ms (delay needed for BMP183 to
  // complete its measurement).
  int32_t getPressure(void);  // Units: Pa.
  // Async variant; see startGetTemperature().
  uint8_t startGetPressure(void);
  int32_t finishGetPressure(void);
  
  // getPressure() uses a temperature measurement as part of the
  // calibration correction; this adds an additional 5ms to the delay.
  // Many applications periodically measure temperature in addition to
  // pressure; in such cases getPressure() can reuse the most
  // recent temperature measurement.
  // Call this after begin().
  void reuseTemperature(bool reuseTemp) {  // Initially false.
  	_reuse_temp = reuseTemp;
  }

  // Read pressure and compute altitude in meters.  sealevelPressure arg
  // must be in Pa.
  float getAltitude(float sealevelPressure = 101325); // std atmosphere
  // Given a getPressure() reading, compute altitude.  Arguments must be in
  // same units.
  static float computeAltitude(float pressure, float sealevelPressure = 101325);
  
  uint16_t readRawTemperature(void);
  uint32_t readRawPressure(void);
  
 private:
  uint8_t startReadRawTemperature(void);
  uint16_t finishReadRawTemperature(void);
  uint8_t startReadRawPressure(void);
  uint32_t finishReadRawPressure(void);
  
  uint8_t SPIxfer(uint8_t x);
  uint8_t read8(uint8_t addr);
  uint16_t read16(uint8_t addr);
  void write8(uint8_t addr, uint8_t data);

  // Most recent temperature reading, needed for applying calibration when
  // reading pressure.
  bool _reuse_temp;
  uint16_t _ut_cache;

  int8_t _cs, _clk, _miso, _mosi;

  uint8_t oversampling;

  int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
  uint16_t ac4, ac5, ac6;
};

#endif //  ADAFRUIT_BMP183_H
