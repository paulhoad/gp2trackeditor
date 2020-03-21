
#include "stdafx.h"
#include "CCCarSetup.h"
#include "GPTrack.h"
#include "CCSetupEdit.h"

int CCCarsetup::Open()
{
  CCCSetupEdit *dlg = new CCCSetupEdit(track);

  dlg->m_FrontWing = CCFrontWing;
  dlg->m_RearWing = CCRearWing;
  dlg->m_Gear1 = CCFirstGear;
  dlg->m_Gear2 = CCSecondGear;
  dlg->m_Gear3 = CCThirdGear;
  dlg->m_Gear4 = CCFourthGear;
  dlg->m_Gear5 = CCFifthGear;
  dlg->m_Gear6 = CCSixthGear;
  dlg->m_Tyre = CCTyreType;
  dlg->m_Air = CCAirResist;
  dlg->m_Grip = CCTrackGrip;
  dlg->m_Fuel = CCFuelConsumption;
  dlg->m_Accel = CCAccel;
  dlg->m_BrakeBalance = CCBrakeBalance;

  int result;

  if (dlg) {
    result = dlg->DoModal();

    if (result != IDCANCEL) {
      CCFrontWing = dlg->m_FrontWing;
      CCRearWing = dlg->m_RearWing;
      CCFirstGear = dlg->m_Gear1;
      CCSecondGear = dlg->m_Gear2;
      CCThirdGear = dlg->m_Gear3;
      CCFourthGear = dlg->m_Gear4;
      CCFifthGear = dlg->m_Gear5;
      CCSixthGear = dlg->m_Gear6;
      CCTyreType = dlg->m_Tyre;
      CCAirResist = dlg->m_Air;
      CCTrackGrip = dlg->m_Grip;
      CCFuelConsumption = dlg->m_Fuel;
      CCBrakeBalance = dlg->m_BrakeBalance;

      if (dlg->m_Accel > 128) {
        AfxMessageBox("Accel == -ve car may go backwards",
          MB_OK | MB_ICONWARNING);
      }
      CCAccel = dlg->m_Accel;
      track->regenerate = TRUE;
    }
  }
  // write(track,offset);
  return result;
}

int CCCarsetup::read(GPTrack *track, int offset)
{
  int start = offset;

  // read all data
  for (int i = 0; i < 19; i++) {
    CCSetpData[i].SetValue(track->ReadPCWord(start + (i * 2)));
    CCSetpData[i].SetDesc("Unknown CC Data");
  }

  // read specifics from structure
  CCFrontWing = track->ReadPCByte(offset) - 0x97;
  offset += 1;
  CCRearWing = track->ReadPCByte(offset) - 0x97;
  offset += 1;
  CCSetpData[0].SetDesc("Front Wing + 0x97/ Rear Wing + 0x97");

  CCFirstGear = track->ReadPCByte(offset) - 0x97;
  offset += 1;
  CCSecondGear = track->ReadPCByte(offset) - 0x97;
  offset += 1;
  CCSetpData[1].SetDesc("1st Gear + 0x97/ 2nd Gear + 0x97");

  CCThirdGear = track->ReadPCByte(offset) - 0x97;
  offset += 1;
  CCFourthGear = track->ReadPCByte(offset) - 0x97;
  offset += 1;
  CCSetpData[2].SetDesc("3rd Gear + 0x97/ 4th Gear + 0x97");

  CCFifthGear = track->ReadPCByte(offset) - 0x97;
  offset += 1;
  CCSixthGear = track->ReadPCByte(offset) - 0x97;
  offset += 1;
  CCSetpData[3].SetDesc("5th Gear + 0x97/ 6th Gear + 0x97");

  CCTyreType = track->ReadPCByte(offset);
  offset += 1;
  CCSetpData[4].SetDesc("Trye Type / Unk");

  CCTrackGrip = track->ReadPCByte(start + 11);
  CCBrakeBalance = track->ReadPCByte(start + 13);
  CCSetpData[5].SetDesc("Track Grip / BrakeBalance");
  CCSetpData[6].SetDesc("Unk / Unk");

  CCSetpData[7].SetDesc("Unk / Unk");
  CCSetpData[8].SetDesc("Unk / Unk");

  CCAccel = track->ReadPCByte(start + 19);
  CCSetpData[9].SetDesc("Acceleration / Unk");

  CCAirResist = track->ReadPCByte(start + 21);
  CCSetpData[10].SetDesc("Air Resist / Unk");

  CCSetpData[11].SetDesc("Unk / Unk");
  CCSetpData[12].SetDesc("Unk / Fuel (low byte)");
  CCFuelConsumption = track->ReadPCWord(start + 26);
  CCSetpData[13].SetDesc("Unk / Fuel (hi byte)");
  CCSetpData[14].SetDesc("Unk / Unk");
  CCSetpData[15].SetDesc("Unk / Unk");
  CCSetpData[16].SetDesc("Unk / Unk");
  CCSetpData[17].SetDesc("Unk / Unk");
  CCSetpData[18].SetDesc("Unk / Unk");
  return offset;
}

int CCCarsetup::write(GPTrack *track, int offset)
{
  int start = offset;

  // write all old setup data out
  for (int i = 0; i < 19; i++) {
    track->WritePCWord(start + (i * 2), CCSetpData[i].GetValue());
  }

  // overwrite with modifications
  track->WritePCByte(offset, (CCFrontWing + 0x97));
  offset += 1;
  track->WritePCByte(offset, (CCRearWing + 0x97));
  offset += 1;
  track->WritePCByte(offset, (CCFirstGear + 0x97));
  offset += 1;
  track->WritePCByte(offset, (CCSecondGear + 0x97));
  offset += 1;
  track->WritePCByte(offset, (CCThirdGear + 0x97));
  offset += 1;
  track->WritePCByte(offset, (CCFourthGear + 0x97));
  offset += 1;
  track->WritePCByte(offset, (CCFifthGear + 0x97));
  offset += 1;
  track->WritePCByte(offset, (CCSixthGear + 0x97));
  offset += 1;
  track->WritePCByte(offset, (CCTyreType));
  offset += 1;

  track->WritePCByte(start + 11, (CCTrackGrip));
  track->WritePCByte(start + 12, (CCBrakeBalance));
  track->WritePCByte(start + 19, (CCAccel));
  track->WritePCByte(start + 21, (CCAirResist));
  track->WritePCWord(start + 26, (CCFuelConsumption));

  // size of setupdata
  return (start + 0x26);
}
