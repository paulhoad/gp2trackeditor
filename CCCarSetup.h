
#ifndef _INCLUDED_CCSETUP_H
#define _INCLUDED_CCSETUP_H

#include "Observer.h"
#include "DataValue.h"

class GPTrack;

class CCCarsetup: public Observer
{
    public:
	int offset;
	GPTrack *track;

	DataValue CCSetpData[19];

	CCCarsetup(GPTrack *gptrack,int myoffset):
	track(gptrack),offset(myoffset), Observer(gptrack,t_CCSetup,NULL)	
	{
	}

	void defaultValues()
	{
		CCFrontWing = 12;
		CCRearWing = 8;
	    CCFirstGear = 28;
	    CCSecondGear = 35;
	    CCThirdGear = 42;
	    CCFourthGear = 50;
        CCFifthGear = 57;
	    CCSixthGear = 64;
        CCFuelConsumption = 5796;
	    CCTyreType = 54;
	    CCAccel = 64;
	    CCTrackGrip = 65;
	    CCAirResist = 64;
		CCBrakeBalance = 64;
	}

	int Open();

	int read(GPTrack *track,int offset);
	int write(GPTrack *track,int offset);
	

	int CCFrontWing;
	int CCRearWing;
	int CCFirstGear;
	int CCSecondGear;
	int CCThirdGear;
	int CCFourthGear;
    int CCFifthGear;
	int CCSixthGear;
    int CCFuelConsumption;
	int CCTyreType;
	int CCAccel;
	int CCTrackGrip;
	int CCAirResist;
	int CCBrakeBalance;
};
#endif