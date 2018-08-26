
#ifndef And_Cyclotron_h
#define And_Cyclotron_h

#include "Arduino.h"
#include <Wire.h> // Include the I2C library (required)
#include "SparkFun_Tlc5940.h"

class And_Cyclotron {
	public:
		And_Cyclotron(int speed, int mode);
		And_Cyclotron(void);
		void begin(int speed, int mode);
		void activate();
		void deactivate();
		void overheat();
		void update(unsigned long ms);
		void setMode(int mode);
		void setSpeed(int speed);
	private:
		int _mode;
		int _speed;
		int _current;
		unsigned long _timeToUpdate;
		bool _active;
		bool _overheat;
		bool _swap;
		void _clear();
		void _overheatLensSwap();
		void _updateNextLens();
		void _extendTime();
		void _fadeOutLens(unsigned long ms);
};
typedef And_Cyclotron;

#endif