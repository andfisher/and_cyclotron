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
  _overheat = false;
}

void And_Cyclotron::activate()
{
	Tlc.clear();
	_active = true;
	_overheat = false;
	_current = 0;
	_updateNextLens();
}
void And_Cyclotron::deactivate()
{
	_active = false;
	_overheat = false;
}

void And_Cyclotron::overheat()
{
	Tlc.clear();

	_overheat = true;
	_active = false;
	_swap = false;

	_overheatLensSwap();
}

void And_Cyclotron::update(unsigned long ms)
{
	if (_overheat) {

		if (ms >= _timeToUpdate) {
			_overheatLensSwap();
		}

		Tlc.update();

	} else if (_active) {
		
		if (ms >= _timeToUpdate) {			
			_updateNextLens();
		}

		_clear();

		Tlc.update();
	}
}

/**
 @desc Automatically alternate odd and even lenses
 @return void
*/
void And_Cyclotron::_overheatLensSwap()
{
	for (int i = 0; i < CYCLOTRON_RGB_N; i++) {
		if (_swap) {
			if (i % 2 == 0) {
				Tlc.set((i * 3) + _mode, 4095);
			} else {
				Tlc.set((i * 3) + _mode, 0);
			}
		} else {
			if (i % 2 == 0) {
				Tlc.set((i * 3) + _mode, 0);
			} else {
				Tlc.set((i * 3) + _mode, 4095);
			}
		}
	}
	_swap = ! _swap;
	_extendTime();
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
	
	_extendTime();
}

void And_Cyclotron::_extendTime()
{
	long delay = CYCLOTRON_DELAY * ((long) _speed / 100.0); // Adjust for speed

	_timeToUpdate = millis() + delay;
}

/*
  @desc Set mode (From TVG), Proton, Slime and Stasis.
		Max proton stream will use default Proton colours.
  @param int mode
  @return void
*/
void And_Cyclotron::setMode(int mode)
{
	if (mode >= 3 || mode < 0) {
		_mode = 0;
	} else {
		_mode = mode;
	}
}

/**
  @desc Set speed modifier as a percentage
  @param int speed
  @return void
*/
void And_Cyclotron::setSpeed(int speed)
{
	_speed = speed;
}