/**
 *  @filename   :   D1_class_Epd_2in9bw.h (epd2in9.h)
 *  @brief      :   Header file for e-paper display library epd2in9.cpp
 *  @author     :   Yehui from Waveshare
 *  @update     :   Christian & Karl Hartinger, May 27, 2018
 *  
 *  Copyright (C) Waveshare     September 5 2017
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documnetation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to  whom the Software is
 * furished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS OR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#ifndef D1_CLASS_EPD_2IN9_H                 // NEW 180414
#define D1_CLASS_EPD_2IN9_H                 // NEW 180414

//#include "epdif.h"                        // NEW 180414
#include "D1_class_EpdConnection.h"         // NEW 180414

// Display resolution
#define EPD_WIDTH       128
#define EPD_HEIGHT      296
#define EPD_COLORS        2                 // NEW 180414

// EPD2IN9 commands
#define DRIVER_OUTPUT_CONTROL                       0x01
#define BOOSTER_SOFT_START_CONTROL                  0x0C
#define GATE_SCAN_START_POSITION                    0x0F
#define DEEP_SLEEP_MODE                             0x10
#define DATA_ENTRY_MODE_SETTING                     0x11
#define SW_RESET                                    0x12
#define TEMPERATURE_SENSOR_CONTROL                  0x1A
#define MASTER_ACTIVATION                           0x20
#define DISPLAY_UPDATE_CONTROL_1                    0x21
#define DISPLAY_UPDATE_CONTROL_2                    0x22
#define WRITE_RAM                                   0x24
#define WRITE_VCOM_REGISTER                         0x2C
#define WRITE_LUT_REGISTER                          0x32
#define SET_DUMMY_LINE_PERIOD                       0x3A
#define SET_GATE_TIME                               0x3B
#define BORDER_WAVEFORM_CONTROL                     0x3C
#define SET_RAM_X_ADDRESS_START_END_POSITION        0x44
#define SET_RAM_Y_ADDRESS_START_END_POSITION        0x45
#define SET_RAM_X_ADDRESS_COUNTER                   0x4E
#define SET_RAM_Y_ADDRESS_COUNTER                   0x4F
#define TERMINATE_FRAME_READ_WRITE                  0xFF

extern const unsigned char lut_full_update[];
extern const unsigned char lut_partial_update[];

// NEW 180401: class name changed from Epd to Epd_
class Epd_ {
public:
    int width;
    int height;
    int colors;                             // NEW 180414
    EpdConnection *conn;                    // NEW 180414

    Epd_();
    Epd_(EpdConnection &connection);        // NEW 180414
    ~Epd_();
    bool init();                            // NEW 180414
    bool reset();                           // NEW 180414
    bool isBusy();                          // NEW 180414
    void display(
         const unsigned char* frame_buffer_black,
         const unsigned char* frame_buffer_red); // NEW 180414
    void setFullUpdate(bool full);          // NEW 180414
    void sleep();                           // NEW 180414
    void wakeup();                          // NEW 180414

  void displayNoWait(
       const unsigned char* frame_buffer_black, 
       const unsigned char* frame_buffer_red); //NEW 180527
  void DisplayFrameNoWait(void);            // NEW 180527

protected:                                  // NEW 180414
    //int  Init(const unsigned char* lut);  // NEW 180414
    void SendCommand(unsigned char command);
    void SendData(unsigned char data);
    void WaitUntilIdle(void);
    void Reset(void);
    void SetFrameMemory(
        const unsigned char* image_buffer,
        int x,
        int y,
        int image_width,
        int image_height
    );
    void SetFrameMemory(const unsigned char* image_buffer);
    void ClearFrameMemory(unsigned char color);
    void DisplayFrame(void);
    void Sleep(void);

private:
    unsigned int reset_pin;
    unsigned int dc_pin;
    unsigned int cs_pin;
    unsigned int busy_pin;
    const unsigned char* lut;

    void SetLut(const unsigned char* lut);
    void SetMemoryArea(int x_start, int y_start, int x_end, int y_end);
    void SetMemoryPointer(int x, int y);
};

#endif /* EPD2IN9_H */

/* END OF FILE */
