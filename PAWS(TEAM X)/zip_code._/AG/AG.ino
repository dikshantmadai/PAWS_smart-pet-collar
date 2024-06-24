#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

/* ADXL345 Accelerometer */
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);

/* NEO-6M GPS Module */
SoftwareSerial gpsSerial(4, 3);  // RX, TX for GPS module
TinyGPSPlus gps;

void displaySensorDetails(void)
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print("Sensor:       "); Serial.println(sensor.name);
  Serial.print("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  Serial.print("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void displayDataRate(void)
{
  Serial.print("Data Rate:    ");

  switch (accel.getDataRate())
  {
    case ADXL345_DATARATE_3200_HZ:
      Serial.print("3200 ");
      break;
    // ... other cases
    default:
      Serial.print("???? ");
      break;
  }
  Serial.println(" Hz");
}

void displayRange(void)
{
  Serial.print("Range:         +/- ");

  switch (accel.getRange())
  {
    case ADXL345_RANGE_16_G:
      Serial.print("16 ");
      break;
    // ... other cases
    default:
      Serial.print("?? ");
      break;
  }
  Serial.println(" g");
}

void setup()
{
  Serial.begin(9600);
  gpsSerial.begin(9600);  // Initialize the software serial for GPS module

  while (!Serial)
  {
    ; // Wait for serial port to connect
  }

  Serial.println("Accelerometer and GPS Test");
  Serial.println("");

  /* Initialise the accelerometer sensor */
  if (!accel.begin())
  {
    /* There was a problem detecting the ADXL345 ... check your connections */
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
    while (1);
  }

  accel.setRange(ADXL345_RANGE_16_G);

  displaySensorDetails();
  displayDataRate();
  displayRange();
  Serial.println("");
}

void loop()
{
  /* Get accelerometer data */
  sensors_event_t event;
  accel.getEvent(&event);

  /* Display accelerometer data */
  Serial.print("X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.println(" m/s^2 ");

  /* Get GPS data */
  while (gpsSerial.available() > 0)
  {
    if (gps.encode(gpsSerial.read()))
    {
      if (gps.location.isValid())
      {
        Serial.print("Latitude: ");
        Serial.println(gps.location.lat(), 6);
        Serial.print("Longitude: ");
        Serial.println(gps.location.lng(), 6);
      }
    }
  }
} // <-- Add this closing brace

