#include <stdlib.h>
#include <math.h>
//=================================================
//--------------- Cac hang so ---------------------
//=================================================
const unsigned char maxSpeed = 255;  // Toc do toi da se chinh la fixDirError + maxSpeed

const unsigned char motorLock = 2;
const unsigned int motorLockRepeat = 2000;

const float radian = 3.141592653589793238462643/180;
//=================================================
//------ Bien toan cuc de dieu khien ------
//=================================================
char _robotChange = 0, _robotRotatePoint=0;
unsigned char _robotCurrentSpeed=0, _robotRunSpeed=0;
int _robotAccStop=0, _robotRunAcc_Timer=0, _robotStop_Timer=0;
int _robotAngle=30000, _robotRotateAngle=0, _robotIMUInit=0;
float _robotRotate=0;
unsigned char _accRun = 0, _accCustom = 0, _robotAngleCounterFix=0, _accHuyOy = 0;

char _IMU_Reading=0, _IMU_Cache=0;
int _robotIMUAngle=0, _IMUAngle_Before=0;

int _IMU_ReadingFix=0;

float _robotRunFL=0, _robotRunFR=0, _robotRunRL=0, _robotRunRR=0;

extern vs16 IMU;
//==========================================================================================
//==========================================================================================
//==========================================================================================
void setMotor(int speed_FL, int speed_FR, int speed_RL, int speed_RR)
{
	if(!speed_FL) mor_10h = motorLock;
	else if(speed_FL > 0)
 	{
	 	mor_10h = speed_FL;
		mor_10h_next;
 	}else{
		mor_10h = -speed_FL;
		mor_10h_back;
 	}

	if(!speed_FR) mor_2h = motorLock;
	else if(speed_FR > 0)
 	{
	 	mor_2h = speed_FR;
		mor_2h_next;
 	}else{
		mor_2h = -speed_FR;
		mor_2h_back;
 	}

	if(!speed_RL) mor_8h = motorLock;
	else if(speed_RL > 0)
 	{
	 	mor_8h = speed_RL;
		mor_8h_next;
 	}else{
		mor_8h = -speed_RL;
		mor_8h_back;
 	}

 	if(!speed_RR) mor_4h = motorLock;
	else if(speed_RR > 0)
 	{
	 	mor_4h = speed_RR;
		mor_4h_next;
 	}else{
		mor_4h = -speed_RR;
		mor_4h_back;
 	}
}
//------------------------------------------------------------------------------
void setAllMotor(int speed)
{
	mor_10h = speed;
	mor_2h = speed;
	mor_8h = speed;
	mor_4h = speed;
}
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
void readIMU(void)
{
	_robotIMUAngle = IMU;	
}
//------------------------------------------------------------------------------

//==========================================================================================
//==========================================================================================
//------------------------------------------------------------------------------
int roundMe(float num)
{
	if(num < 0) return num - 0.5;
	else return num + 0.5;
}
//------------------------------------------------------------------------------
float maxMe(float N1, float N2, float N3, float N4)
{
	float _maxNum=1;
	if(N1<0) N1 = -N1;
	if(N2<0) N2 = -N2;
	if(N3<0) N3 = -N3;
	if(N4<0) N4 = -N4;

	if(N1>_maxNum) _maxNum = N1;
	if(N2>_maxNum) _maxNum = N2;
	if(N3>_maxNum) _maxNum = N3;
	if(N4>_maxNum) _maxNum = N4;

	return _maxNum;
}
//------------------------------------------------------------------------------
void calculateMotor(float rotate)
{
	unsigned char maxTemp=0;
	float angle;

	if(_robotAngle == 30000)// khi robot quay tai cho
	{
		_robotRunFL = _robotRunFR = _robotRunRL = _robotRunRR = 0;
	}
					_robotRunFL += rotate;
					_robotRunFR -= rotate;
					_robotRunRL += rotate;
					_robotRunRR -= rotate;

	//----------- tinh toc do toi da ----------------
	maxTemp = (int)(maxSpeed/maxMe(_robotRunFL, _robotRunFR, _robotRunRL, _robotRunRR));
	if(_robotRunSpeed > maxTemp)
	{
		_robotRunSpeed = maxTemp;
		if(_robotCurrentSpeed > _robotRunSpeed) _robotCurrentSpeed = _robotRunSpeed;
    }
	//------------------------------------------------
	_robotChange = 1;
}
//------------------------------------------------------------------------------
void robotStop(unsigned int acc)
{
	_robotRunSpeed = 0;
	_robotAccStop = acc;

    if(!acc)
			{
        _robotCurrentSpeed = 0;

		_robotAngle = 30000;

		_robotRotate = 0;
	    _robotRotatePoint = 0;
	    _robotRotateAngle = 0;
		_robotAngleCounterFix = 0;
        _accCustom = 0;

		calculateMotor(0);
    }
}
//------------------------------------------------------------------------------
void robotRotateStop()
{
	if(_robotRotate || _robotRotatePoint || _robotRotateAngle)
    {
        _robotRotate = 0;
        _robotRotatePoint = 0;
        _robotRotateAngle = 0;
        _robotAngleCounterFix = 0;

        _robotIMUInit = _robotIMUAngle;

        calculateMotor(_robotRotate);
    }
}
//------------------------------------------------------------------------------
void robotSetACC(unsigned char N)
{
    _accCustom=N;
}
//------------------------------------------------------------------------------
char robotIsRun()
{
    return (_robotAngle == 30000)? 0: 1;
}
//------------------------------------------------------------------------------
void robotRun(int angle, unsigned char maxSpeed)
{
    if(_robotAngle != angle || _robotRunSpeed != maxSpeed )
    {
        _accRun = 50 - _accHuyOy; //(_accCustom? _accCustom: 32);		// bien gia toc lon la giam gia toc
        _robotRunSpeed = maxSpeed;

        _robotIMUInit = _robotIMUAngle;

        _robotAngle = angle;
        _robotStop_Timer = 0;

        calculateMotor(_robotRotate);
    }
}
//------------------------------------------------------------------------------
char robotRotate(int rotateAngle, float rotate, char rotatePoint)
{
    //--------------------------------------------
    if(_robotAngleCounterFix > 0)
    {
        if(!_robotRotate)
        {
            _robotRunSpeed = 5;
             _robotCurrentSpeed = 3;
             _accRun = 1;

            if(_robotIMUAngle > rotateAngle) _robotRotate = -1;
            else _robotRotate = 1;

        }
        else if(_robotAngleCounterFix++ > 25)
            {
                robotStop(0);
                return 0;
            }
    }
    //--------------------------------------------
    if(_robotRotateAngle != rotateAngle || _robotRotatePoint != rotatePoint)
    {
        if(_robotAngle == 30000)
        {
            _robotRunSpeed = 50;
             _robotCurrentSpeed = 3;
            _accRun = 80;//(_accCustom? _accCustom: 50);

            if(rotateAngle < 30000 && ((rotate > 0 && _robotIMUAngle > rotateAngle) || (rotate < 0 && _robotIMUAngle < rotateAngle ))) rotate = - rotate;
        }

        _robotRotateAngle = rotateAngle;
        _robotRotatePoint = rotatePoint;
        _robotRotate = rotate;
        _robotStop_Timer = 0;

        _robotAngleCounterFix = 0;

        calculateMotor(_robotRotate);
    }
    //--------------------------------------------
    return 1;
}
//-----------------------------------------------------------------
void robotDirectionAnalytics()
{
    // Khi chay duong cong hoac khong chay hoac quay tu do thi thoat
    if( !_robotCurrentSpeed || _robotRotateAngle >= 30000) return;
	
    // Khi co lenh xoay
    if(_robotRotate)
    {
        // Khi chi xoay tai cho
        if(_robotAngle == 30000)
        {
            if((_robotRotate > 0 && _robotIMUAngle > _robotRotateAngle - ( _robotCurrentSpeed*_robotRotate*3 + 100)) ||
               (_robotRotate < 0 && _robotIMUAngle < _robotRotateAngle + (-_robotCurrentSpeed*_robotRotate*3 + 100)))
            {
                // Khi dung goc thi dung robot, khoa banh xe, va bat dau Fix Angle
                if(_robotRotateAngle - 10 < _robotIMUAngle && _robotIMUAngle < _robotRotateAngle + 10)
                {
                     _robotCurrentSpeed = 0;
                    _robotRotate = 0;
                    calculateMotor(0);

                    _robotAngleCounterFix = 1;
                    return;
                }
                _robotCurrentSpeed = 10;
                //------------------------------------------
                calculateMotor(((_robotRotateAngle > _robotIMUAngle)?1:-1)* fminf((5 + abs(_robotRotateAngle - _robotIMUAngle)/(float)50),10)/_robotCurrentSpeed);
            }
						else calculateMotor(_robotRotate);
        }

        return;
    }

    // Khi chay thang
    calculateMotor(0.005*(_robotIMUInit - _robotIMUAngle));
}
//------------------------------------------------------------------------------
void robotAnalytics(void)//------ Dat trong Interup Timer
{
    readIMU();
    //======= Gia toc di chuyen ==========
    if(_robotRunSpeed > 0)// Robot dang duoc ra lenh chay
    {
        if(_robotCurrentSpeed != _robotRunSpeed)
        {
            if(_robotCurrentSpeed < _robotRunSpeed){
                if(++_robotRunAcc_Timer > _accRun - (unsigned char)_robotCurrentSpeed)//--- Phuong trinh Gia toc --------
                {
                    _robotCurrentSpeed++;
                    _robotChange = 1;
                       _robotRunAcc_Timer = 0;
                   }
            }else{
                if(++_robotRunAcc_Timer > fminf((unsigned char)_robotCurrentSpeed/3,10))//--- Phuong trinh Giam toc --------
                {
                    _robotCurrentSpeed--;
                    _robotChange = 1;
                    _robotRunAcc_Timer = 0;
                }
           }
        }
    }
    else  //-------- Robot duoc lenh Stop -----------
        if(_robotCurrentSpeed > 0) //-------- Giam toc do -----------
        {
            if(++_robotRunAcc_Timer > _robotAccStop)
            {
                _robotCurrentSpeed--;
                _robotChange = 1;
                _robotRunAcc_Timer = 0;
            }
        }
    //======= Fix robot theo IMU ==========
    if(abs(_IMUAngle_Before - _robotIMUAngle)>3)
    {
        robotDirectionAnalytics();
        _IMUAngle_Before = _robotIMUAngle;
    }

    //======= Thiet lap thong so dong co ==========
    if(_robotChange) setMotor(roundMe(_robotCurrentSpeed * _robotRunFL), roundMe(_robotCurrentSpeed * _robotRunFR), roundMe(_robotCurrentSpeed * _robotRunRL), roundMe(_robotCurrentSpeed * _robotRunRR));

    //======= Khoa banh xe N chu ky roi tha tu do ==========
    if(_robotRunSpeed == 0 && _robotCurrentSpeed == 0)
    {

        _robotRotate = 0;
        _robotRotatePoint = 0;
        _robotAngle = 30000;
        _robotRotateAngle = 0;
        _robotAngleCounterFix = 0;
        _accCustom=0;

        if(++_robotStop_Timer < motorLockRepeat)
        {
            setAllMotor(motorLock);
        }else
        {
            setAllMotor(0);
            _robotStop_Timer = motorLockRepeat;
        }
    }
    //------------------------------------------------------

}
//==========================================================================================
//==========================================================================================
//==========================================================================================
