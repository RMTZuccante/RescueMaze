@echo %off

if exist Robot\libraries (
	FOR /R "Robot\libraries\" %%i IN (*) DO MOVE "%%i" "Robot"
	rd /s /q "Robot\libraries\"
) else (

	md Robot\libraries\Debug
	md Robot\libraries\Matrix
	md Robot\libraries\Robot\Cagamattoni
	md Robot\libraries\Robot\Color
	md Robot\libraries\Robot\Moviment\IMU\MadgwickAHRS
	md Robot\libraries\Robot\Moviment\IMU\MPU6050
	md Robot\libraries\Robot\Moviment\Motor
	md Robot\libraries\Robot\RGB
	md Robot\libraries\Robot\Temperature
	md Robot\libraries\Robot\VL53L0X

	cd Robot\

	move Debug.* libraries\Debug
	move Matrix.* libraries\Matrix
	move Robot.h libraries\Robot
	move Robot.cpp libraries\Robot
	move Cagamattoni.* libraries\Robot\Cagamattoni
	move Color.* libraries\Robot\Color
	move Moviment.* libraries\Robot\Moviment
	move Motor.* libraries\Robot\Moviment\Motor
	move IMU.* libraries\Robot\Moviment\IMU
	move MadgwickAHRS.* libraries\Robot\Moviment\IMU\MadgwickAHRS
	move MPU6050.* libraries\Robot\Moviment\IMU\MPU6050
	move RGB.* libraries\Robot\RGB
	move Temperature.* libraries\Robot\Temperature
	move VL53L0X.* libraries\Robot\VL53L0X
	move *.h libraries

	cd..
)