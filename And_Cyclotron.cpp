#include "Arduino.h"
#include <Wire.h> // Include the I2C library (required)
#include "SparkFun_Tlc5940.h"
#include "And_Cyclotron.h"

#define CYCLOTRON_LENS_N 4
#define CYCLOTRON_RGB_N 12
#define CYCLOTRON_DELAY 750

And_Cyclotron::And_Cyclotron()
{
}

And_Cyclotron::And_Cyclotron(int mode, int speed)
{
	begin(mode, speed);
}

void And_Cyclotron::begin(int mode, int speed)
{
  Tlc.init(0);
  Tlc.clear();
  setMode(_mode);
  _speed = speed;
  _current = 0;
}

void And_Cyclotron::activate()
{
	Tlc.clear();
	_active = true;
	_current = 0;
	_updateNextLens();
}
void And_Cyclotron::deactivate()
{
	_active = false;
}
void And_Cyclotron::update(unsigned long ms)
{
	if (_active) {
		
		if (ms >= _timeToUpdate) {			
			_updateNextLens();
		}
		
		_clear();
		
		Tlc.update();
	}
}

void And_Cyclotron::_clear()
{
	for (int i = 0; i < CYCLOTRON_RGB_N; i++) {
		if (i != _current) {
			Tlc.set((i * 3) + _mode, 0);
		}
	}
}

void And_Cyclotron::_updateNextLens()
{
	_current++;

	if (_current >= CYCLOTRON_LENS_N) {
		_current = 0;
	}

	Tlc.set((_current * 3) + _mode, 4095);
	
	int delay = CYCLOTRON_DELAY;
	_timeToUpdate = millis() + delay; // @TODO adjust for speed
}

void And_Cyclotron::setMode(int mode)
{
	if (mode >= 3 || mode < 0) {
		_mode = 0;
	} else {
		_mode = mode;
	}
}

void And_Cyclotron::setSpeed(int speed)
{
	_speed = speed;
}