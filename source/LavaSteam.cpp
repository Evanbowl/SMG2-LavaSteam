#include "LavaSteam.h"
#include "revolution/types.h"

LavaSteam::LavaSteam(const char* pName) : LiveActor(pName) {
    mInterval = 90;
    mSignDelay = 120;
    mSteamDelay = 90;
    _90.x = 0.0f;
    _90.y = 1.0f;
    _90.z = 0.0f;
    mEffectSRTVec = TVec3f(1.0f, 1.0f, 1.0f);
    mSteamForeverMode = false;
}

void LavaSteam::init(const JMapInfoIter& rIter) {
    MR::processInitFunction(this, rIter, false);
    MR::getJMapInfoArg0NoInit(rIter, &mInterval);
    MR::getJMapInfoArg1NoInit(rIter, &mSignDelay);
    MR::getJMapInfoArg2NoInit(rIter, &mSteamDelay);
    MR::getJMapInfoArg3NoInit(rIter, &mSteamForeverMode);

    initHitSensor(1);

    MR::setEffectHostSRT(this, "Sign", &mTranslation, &mRotation, &mEffectSRTVec);
    HitSensor* pSensor = MR::addHitSensorMapObj(this, "body", 8, 250.0f, TVec3f(0.0f, 250.0f, 0.0f));
    
    MR::setClippingTypeSphere(this, 250.0f, &pSensor->mPosition);
    initNerve(&NrvLavaSteam::HostTypeWait::sInstance, 0);
    
    if (MR::useStageSwitchReadA(this, rIter)) {
        setNerve(&NrvLavaSteam::HostTypeWaitForSwitchOn::sInstance);
        MR::listenStageSwitchOnA(this, MR::Functor(this, &LavaSteam::startSteam));
    }
    else if (MR::tryRegisterDemoCast(this, rIter)) {
        setNerve(&NrvLavaSteam::HostTypeWaitForSwitchOn::sInstance);
        MR::registerDemoActionFunctor(this, MR::Functor(this, &LavaSteam::startSteam), 0);
    }
    else if (mSteamForeverMode)
        setNerve(&NrvLavaSteam::HostTypeSteam::sInstance);
    //MR::useStageSwitchSleep(this, rIter);
    makeActorAppeared();
}

void LavaSteam::initAfterPlacement() {
    TMtx34f mtx;
    mtx.identity();
    TVec3f vec = mRotation*0.017453292f;

    f32 vecz = vec.z;
    f32 vecy = vec.y;
    f32 vecx = vec.x;

    f32 cosz = cos(vecz);
    f32 cosy = cos(vecy);
    f32 cosx = cos(vecx);
    f32 siny = sin(vecy);
    f32 sinz = sin(vecz);
    f32 sinx = sin(vecx); 


    mtx.mMtx[0][0] = (cosy*cosz);
    f32 v2 = (sinx*cosy);
    mtx.mMtx[2][1] = v2;
    mtx.mMtx[1][0] = (cosy*sinz);
    f32 v1 = ((cosx * cosz) + ((sinx * siny) * sinz));
    mtx.mMtx[1][1] = v1;
    f32 v0 = (((sinx * siny) * cosz) - (cosx * sinz));
    mtx.mMtx[0][1] = v0;
    mtx.mMtx[0][2] = ((cosx * cosz) * siny) + (sinx * sinz);
    mtx.mMtx[2][2] = (cosx * cosy);
    mtx.mMtx[1][2] = ((cosx * sinz) * siny) - (sinx * cosz);
    mtx.mMtx[2][0] = -siny;

    _90.set(v0, v1, v2);
    MR::normalize(&_90);
}

void LavaSteam::attackSensor(HitSensor* pSender, HitSensor* pReceiver) {
    TVec3f stack_64;
    TVec3f stack_58;
    f32 stack_2C, stack_54;
    
    if (!isNerve(&NrvLavaSteam::HostTypeWait::sInstance) && !isNerve(&NrvLavaSteam::HostTypeWaitForSwitchOn::sInstance)) {
        if (MR::isSensorPlayerOrRide(pReceiver)) {
            f32 rad = pReceiver->mRadius;
            f32 f31 = rad*0.69999999f;
            
            if (isNerve(&NrvLavaSteam::HostTypeSteam::sInstance)) {
                TVec3f stack_30 =_90*400.0f;
                TVec3f stack_3C = TVec3f(mTranslation);
                
                JMathInlineVEC::PSVECAdd2((Vec*)&stack_3C, (Vec*)&stack_30, (Vec*)&stack_3C);
                
                TVec3f stack_48;
                stack_48.set(stack_3C);

                stack_54 = 70.0;

                TVec3f stack_20;
                stack_20.set(pReceiver->mPosition);
                
                f32 dist = JMathInlineVEC::PSVECSquareDistance((Vec*)&stack_48, (Vec*)&stack_20);
                f32 s = (stack_54+f31);
                stack_2C = f31;
                
                if ((dist <= s*s) & 3 && MR::sendMsgEnemyAttackFire(pReceiver, pSender))
                    return;
            }
        
            if (isNerve(&NrvLavaSteam::HostTypeSteam::sInstance)) {
                TVec3f stack_14 = _90*330.0f;

                stack_58.set(mTranslation);
                stack_64.set(stack_14);

                TVec3f stack_8;
                JMathInlineVEC::PSVECSubtract((Vec*)&pReceiver->mPosition, (Vec*)&stack_58, (Vec*)&stack_8);
                f32 sq1 = stack_8.squared();
                f32 sq2 = stack_64.squared();
                f32 _64dot8 = stack_64.dot(stack_8);
                f32 ff1 = sq2*sq1;
                f32 ff2 = _64dot8/sq2;
                f32 squ3 = JGeometry::TUtil<f32>::sqrt((ff1-(_64dot8*_64dot8))/sq2);

                if (0.0f <= ff2 && ff2 <= 1.0f && squ3 < (10.0f+f31) && MR::sendMsgEnemyAttackFire(pReceiver, pSender))
                    return;
            }
        }
    }
}

void LavaSteam::startSteam() {
    setNerve(&NrvLavaSteam::HostTypeSteam::sInstance);
}

void LavaSteam::exeWait() {
    if (MR::isFirstStep(this)) {
        MR::invalidateHitSensors(this);
        MR::emitEffect(this, "Sign");
        mEffectSRTVec.setAll(1.0f);
    }

    if (MR::isGreaterStep(this, mSignDelay-38) && MR::isLessStep(this, mSignDelay-30)) {
        int step = getNerveStep();
        f32 f = MR::getEaseInValue(((mSignDelay-30)-step)*0.125f, 0.001f, 1.0f, 1.0f);
        mEffectSRTVec.setAll(f);
    }

    if (MR::isStep(this, mSignDelay))
        MR::forceDeleteEffect(this, "Sign");

    if (MR::isStep(this, mSignDelay+30))
        setNerve(&NrvLavaSteam::HostTypeSteam::sInstance);
}

void LavaSteam::exeSteam() {
    if (MR::isFirstStep(this)) {
        MR::emitEffect(this, "Steam");
        MR::validateHitSensors(this);
    }

    MR::startLevelSound(this, "SE_OJ_LV_LAVA_STEAM_OUT", -1, -1, -1);

    if (MR::isStep(this, mSteamDelay) && !mSteamForeverMode) {
        MR::deleteEffect(this, "Steam");
        setNerve(&NrvLavaSteam::HostTypeSteamEnd::sInstance);
    }
}

void LavaSteam::exeSteamEnd() {
    if (MR::isStep(this, mInterval))
        setNerve(&NrvLavaSteam::HostTypeWait::sInstance);
}

LavaSteam::~LavaSteam() {

}

namespace NrvLavaSteam {
    void HostTypeWait::execute(Spine* pSpine) const {
        ((LavaSteam*)pSpine->mExecutor)->exeWait();
    }

    void HostTypeWaitForSwitchOn::execute(Spine* pSpine) const {
    }
    
    void HostTypeSteam::execute(Spine* pSpine) const {
        ((LavaSteam*)pSpine->mExecutor)->exeSteam();
    }
    
    void HostTypeSteamEnd::execute(Spine* pSpine) const {
        ((LavaSteam*)pSpine->mExecutor)->exeSteamEnd();
    }

    HostTypeWait(HostTypeWait::sInstance);
    HostTypeWaitForSwitchOn(HostTypeWaitForSwitchOn::sInstance);
    HostTypeSteam(HostTypeSteam::sInstance);
    HostTypeSteamEnd(HostTypeSteamEnd::sInstance);
}
