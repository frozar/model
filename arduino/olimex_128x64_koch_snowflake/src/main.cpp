/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_I2CDevice.h>
#include <math.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void rotVec2(float vec[2], float alpha, float *vecRot)
{
  float x = vec[0];
  float y = vec[1];
  float xRot = x * cos(alpha) - y * sin(alpha);
  float yRot = x * sin(alpha) + y * cos(alpha);
  vecRot[0] = xRot;
  vecRot[1] = yRot;
}

const int nbExtremyties = 5;
const int dim = 2;
const int16_t yMax = SCREEN_HEIGHT - 1;

void nextKochSnoflake(float x0Screen,
                      float y0Screen,
                      float x1Screen,
                      float y1Screen,
                      float ptPath[nbExtremyties][dim])
{
  float x0Cart = x0Screen;
  float x1Cart = x1Screen;
  float y0Cart = -y0Screen + yMax;
  float y1Cart = -y1Screen + yMax;

  float vect[2] = {x1Cart - x0Cart, y1Cart - y0Cart};
  float alpha = atan2(vect[1], vect[0]);
  float vectLen = sqrt(vect[0] * vect[0] + vect[1] * vect[1]);
  float xLen = vectLen;
  float xInc = xLen / 3;
  float yMid = sqrt(3) / 2 * xInc;

  float v[4][2] = {{xInc, 0},
                   {xInc / 2, yMid},
                   {xInc / 2, -yMid},
                   {xInc, 0}};

  float vRot[4][2] = {{0, 0},
                      {0, 0},
                      {0, 0},
                      {0, 0}};

  for (int i = 0; i < 4; ++i)
  {
    rotVec2(v[i], alpha, vRot[i]);
  }

  // Flip y to adapt screen coordinate system
  for (int i = 0; i < 4; ++i)
  {
    vRot[i][1] = -vRot[i][1];
  }

  ptPath[0][0] = x0Screen;
  ptPath[0][1] = y0Screen;
  for (int i = 0; i < 4; ++i)
  {
    ptPath[i + 1][0] = ptPath[i][0] + vRot[i][0];
    ptPath[i + 1][1] = ptPath[i][1] + vRot[i][1];
  }
}

int sumNbSegment(int order)
{
  if (order < 1)
  {
    return 1;
  }
  else
  {
    int sum = 0;
    for (int i = 0; i < order; ++i)
    {
      sum += pow(4, i);
    }
    return sum;
  }
}

void kochSnowflake(int16_t x0Screen,
                   int16_t y0Screen,
                   int16_t x1Screen,
                   int16_t y1Screen,
                   int order)
{
  display.clearDisplay();

  printf("order %d\n", order);
  const int totalNbSegment = sumNbSegment(order);
  // printf("totalNbSegment %d\n", totalNbSegment);

  int nbSegmentPerOrder[order];
  for (int i = 0; i < order; ++i)
  {
    nbSegmentPerOrder[i] = pow(4, i);
    // printf("nbSegmentPerOrder[%d]: %d\n", i, nbSegmentPerOrder[i]);
  }

  int parentMapping[totalNbSegment][2];
  for (int j = 1; j < totalNbSegment; ++j)
  {
    int toSub = 0;
    int currentTotal = 0;
    int iBreak = 0;
    for (int i = 0; i < order; ++i)
    {
      currentTotal += nbSegmentPerOrder[i];
      if (j < currentTotal)
      {
        toSub = currentTotal - nbSegmentPerOrder[i];
        iBreak = std::max(i - 1, 0);
        break;
      }
    }
    int idxParentOffset = 0;
    for (int i = 0; i < iBreak; ++i)
    {
      idxParentOffset += nbSegmentPerOrder[i];
    }

    int jNormalised = (j - toSub);
    int quot = jNormalised / 4;
    int parentIdx = quot + idxParentOffset;
    int subIdx = jNormalised % 4;
    parentMapping[j][0] = parentIdx;
    parentMapping[j][1] = subIdx;
  }
  // printf("\n");

  // Initialisation of path points
  float ptPath[totalNbSegment][nbExtremyties][dim];
  nextKochSnoflake(x0Screen,
                   y0Screen,
                   x1Screen,
                   y1Screen, ptPath[0]);

  for (int j = 1; j < totalNbSegment; ++j)
  {

    const int parentIdx = parentMapping[j][0];
    const int subIdx = parentMapping[j][1];
    nextKochSnoflake(ptPath[parentIdx][subIdx][0],
                     ptPath[parentIdx][subIdx][1],
                     ptPath[parentIdx][subIdx + 1][0],
                     ptPath[parentIdx][subIdx + 1][1],
                     ptPath[j]);
  }

  int jStart = 0;
  int jEnd = 0;
  for (int i = 0; i < order; ++i)
  {
    if (i < (order - 1))
    {
      jStart += nbSegmentPerOrder[i];
    }
    jEnd += nbSegmentPerOrder[i];
  }

  for (int j = jStart; j < jEnd; ++j)
  {
    for (int i = 0; i < 4; ++i)
    {
      const int16_t i0 = i;
      const int16_t i1 = i0 + 1;
      display.drawLine(ptPath[j][i0][0], ptPath[j][i0][1],
                       ptPath[j][i1][0], ptPath[j][i1][1], SSD1306_WHITE);
      display.display();
      delay(100);
    }
  }
}

void setup()
{
  Serial.begin(115200);

  while (!Serial)
    ;

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  int16_t x0Screen;
  int16_t x1Screen;
  int16_t y0Screen;
  int16_t y1Screen;
  for (;;)
  {
    x0Screen = 1;
    x1Screen = x0Screen + 42 * 3;
    y0Screen = yMax - 14;
    y1Screen = y0Screen - 0;
    for (int i = 1; i < 4; ++i)
    {
      kochSnowflake(x0Screen,
                    y0Screen,
                    x1Screen,
                    y1Screen, i);
      delay(1000);
    }
  }
}

void loop()
{
}
