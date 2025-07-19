#pragma once
#include "syati.h"

class LavaSteam : public LiveActor {
public:
    LavaSteam(const char* pName);
    virtual ~LavaSteam();
    virtual void init(const JMapInfoIter& rIter);
    virtual void initAfterPlacement();
    virtual void attackSensor(HitSensor* pSender, HitSensor* pReceiver);
    void startSteam();
    void exeWait();
    void exeSteam();
    void exeSteamEnd();

    s32 mInterval;
    s32 mSignDelay;
    s32 mSteamDelay;
    TVec3f _90;
    TVec3f mEffectSRTVec;
    bool mSteamForeverMode;
};

namespace NrvLavaSteam {
    NERVE(HostTypeSteamEnd);
    NERVE(HostTypeSteam);
    NERVE(HostTypeWait);
    NERVE(HostTypeWaitForSwitchOn);
};