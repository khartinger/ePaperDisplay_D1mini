/**
 *  @filename   :   D1_class_Epd_1in54b.cpp
 *  @brief      :   Implements for e-paper library
 *  @author     :   Yehui from Waveshare
 *
 *  Copyright (C) Waveshare      August   10 2017
 *  Changes by    Karl Hartinger April    01 2018
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

#include <stdlib.h>
#include "D1_class_Epd_1in54red.h"

Epd_::~Epd_() {
};

Epd_::Epd_() {
    width = EPD_WIDTH;
    height = EPD_HEIGHT;
    colors = EPD_COLORS;
    conn = new EpdConnection();
};

Epd_::Epd_(EpdConnection &connection) {
    width = EPD_WIDTH;
    height = EPD_HEIGHT;
    colors = EPD_COLORS;
    conn = &connection;
};

bool Epd_::init(void) {                     // NEW 180411
    /* EPD hardware init start */
    Reset();
    SendCommand(POWER_SETTING);
    SendData(0x07);
    SendData(0x00);
    SendData(0x08);
    SendData(0x00);
    SendCommand(BOOSTER_SOFT_START);
    SendData(0x07);
    SendData(0x07);
    SendData(0x07);
    SendCommand(POWER_ON);

    WaitUntilIdle();

    SendCommand(PANEL_SETTING);
    SendData(0xcf);
    SendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
    SendData(0x17);
    SendCommand(PLL_CONTROL);
    SendData(0x39);
    SendCommand(TCON_RESOLUTION);
    SendData(0xC8);
    SendData(0x00);
    SendData(0xC8);
    SendCommand(VCM_DC_SETTING_REGISTER);
    SendData(0x0E);

    SetLutBw();
    SetLutRed();
    /* EPD hardware init end */

    return true;
}

/**
 *  @brief: basic function for sending commands
 */
void Epd_::SendCommand(unsigned char command) {
    conn->sendCommand(command);
}

/**
 *  @brief: basic function for sending data
 */
void Epd_::SendData(unsigned char data) {
    conn->sendData(data);
}

/**
 *  @brief: Wait until the busy_pin goes HIGH
 */
void Epd_::WaitUntilIdle(void) {
 conn->waitUntilIdle();
}

/**
 *  @brief: module reset.
 *          often used to awaken the module in deep sleep,
 *          see Epd_::Sleep();
 */
void Epd_::Reset(void) {
 conn->resetDisplay();
}

void Epd_::sleep(void) {
 Sleep();
}

void Epd_::wakeup(void) {
 init();
}

/**
 *  @brief: set the look-up tables
 */
void Epd_::SetLutBw(void) {
    unsigned int count;     
    SendCommand(0x20);         //g vcom
    for(count = 0; count < 15; count++) {
        SendData(lut_vcom0[count]);
    } 
    SendCommand(0x21);        //g ww --
    for(count = 0; count < 15; count++) {
        SendData(lut_w[count]);
    } 
    SendCommand(0x22);         //g bw r
    for(count = 0; count < 15; count++) {
        SendData(lut_b[count]);
    } 
    SendCommand(0x23);         //g wb w
    for(count = 0; count < 15; count++) {
        SendData(lut_g1[count]);
    } 
    SendCommand(0x24);         //g bb b
    for(count = 0; count < 15; count++) {
        SendData(lut_g2[count]);
    } 
}

void Epd_::SetLutRed(void) {
    unsigned int count;     
    SendCommand(0x25);
    for(count = 0; count < 15; count++) {
        SendData(lut_vcom1[count]);
    } 
    SendCommand(0x26);
    for(count = 0; count < 15; count++) {
        SendData(lut_red0[count]);
    } 
    SendCommand(0x27);
    for(count = 0; count < 15; count++) {
        SendData(lut_red1[count]);
    } 
}

void Epd_::display(const unsigned char* frame_buffer_black, const unsigned char* frame_buffer_red) {
    DisplayFrame(frame_buffer_black, frame_buffer_red);
}

void Epd_::DisplayFrame(const unsigned char* frame_buffer_black, const unsigned char* frame_buffer_red) {
    unsigned char temp;
    if (frame_buffer_black != NULL) {
        SendCommand(DATA_START_TRANSMISSION_1);
        delay(2);
        for (int i = 0; i < this->width * this->height / 8; i++) {
            temp = 0x00;
            for (int bit = 0; bit < 4; bit++) {
//                if ((pgm_read_byte(&frame_buffer_black[i]) & (0x80 >> bit)) != 0) {
if ((frame_buffer_black[i] & (0x80 >> bit)) != 0) {
                    temp |= 0xC0 >> (bit * 2);
                }
            }
            SendData(temp);
            temp = 0x00;
            for (int bit = 4; bit < 8; bit++) {
                //if ((pgm_read_byte(&frame_buffer_black[i]) & (0x80 >> bit)) != 0) {
if ((frame_buffer_black[i] & (0x80 >> bit)) != 0) {
                    temp |= 0xC0 >> ((bit - 4) * 2);
                }
            }
            SendData(temp);
        }
        delay(2);
    }
    if (frame_buffer_red != NULL) {
        SendCommand(DATA_START_TRANSMISSION_2);
        delay(2);
        for (int i = 0; i < this->width * this->height / 8; i++) {
            //SendData(pgm_read_byte(&frame_buffer_red[i]));
temp=frame_buffer_red[i];
SendData(temp);
        }
        delay(2);
    }
    SendCommand(DISPLAY_REFRESH);
    WaitUntilIdle();
}

/**
 *  @brief: After this command is transmitted, the chip would enter the 
 *          deep-sleep mode to save power. 
 *          The deep sleep mode would return to standby by hardware reset. 
 *          The only one parameter is a check code, the command would be
 *          executed if check code = 0xA5. 
 *          You can use Epd_::Init() to awaken
 */
void Epd_::Sleep() {
    SendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
    SendData(0x17);
    SendCommand(VCM_DC_SETTING_REGISTER);         //to solve Vcom drop
    SendData(0x00);
    SendCommand(POWER_SETTING);         //power setting
    SendData(0x02);        //gate switch to external
    SendData(0x00);
    SendData(0x00);
    SendData(0x00);
    WaitUntilIdle();
    SendCommand(POWER_OFF);         //power off
}

const unsigned char lut_vcom0[] =
{
    0x0E, 0x14, 0x01, 0x0A, 0x06, 0x04, 0x0A, 0x0A,
    0x0F, 0x03, 0x03, 0x0C, 0x06, 0x0A, 0x00
};

const unsigned char lut_w[] =
{
    0x0E, 0x14, 0x01, 0x0A, 0x46, 0x04, 0x8A, 0x4A,
    0x0F, 0x83, 0x43, 0x0C, 0x86, 0x0A, 0x04
};

const unsigned char lut_b[] = 
{
    0x0E, 0x14, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A,
    0x0F, 0x83, 0x43, 0x0C, 0x06, 0x4A, 0x04
};

const unsigned char lut_g1[] = 
{
    0x8E, 0x94, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A,
    0x0F, 0x83, 0x43, 0x0C, 0x06, 0x0A, 0x04
};

const unsigned char lut_g2[] = 
{
    0x8E, 0x94, 0x01, 0x8A, 0x06, 0x04, 0x8A, 0x4A,
    0x0F, 0x83, 0x43, 0x0C, 0x06, 0x0A, 0x04
};

const unsigned char lut_vcom1[] = 
{
    0x03, 0x1D, 0x01, 0x01, 0x08, 0x23, 0x37, 0x37,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char lut_red0[] = 
{
    0x83, 0x5D, 0x01, 0x81, 0x48, 0x23, 0x77, 0x77,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char lut_red1[] = 
{
    0x03, 0x1D, 0x01, 0x01, 0x08, 0x23, 0x37, 0x37,
    0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

/* END OF FILE */


