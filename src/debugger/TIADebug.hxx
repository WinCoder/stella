//============================================================================
//
//   SSSS    tt          lll  lll
//  SS  SS   tt           ll   ll
//  SS     tttttt  eeee   ll   ll   aaaa
//   SSSS    tt   ee  ee  ll   ll      aa
//      SS   tt   eeeeee  ll   ll   aaaaa  --  "An Atari 2600 VCS Emulator"
//  SS  SS   tt   ee      ll   ll  aa  aa
//   SSSS     ttt  eeeee llll llll  aaaaa
//
// Copyright (c) 1995-2017 by Bradford W. Mott, Stephen Anthony
// and the Stella Team
//
// See the file "License.txt" for information on usage and redistribution of
// this file, and for a DISCLAIMER OF ALL WARRANTIES.
//============================================================================

#ifndef TIA_DEBUG_HXX
#define TIA_DEBUG_HXX

class Debugger;
class TiaDebug;
class TIA;

#include "DebuggerSystem.hxx"
#include "DelayQueueIterator.hxx"
#include "bspf.hxx"

// Function type for TIADebug instance methods
class TIADebug;
using TiaMethod = int (TIADebug::*)() const;

// Indices for various IntArray in TiaState
enum {
  P0, P1, M0, M1, BL
};

class TiaState : public DebuggerState
{
  public:
    IntArray ram;
    IntArray coluRegs;
    IntArray gr;
    IntArray pos;
    IntArray hm;
    IntArray pf;
    IntArray size;
    IntArray aud;
};

class TIADebug : public DebuggerSystem
{
  public:
    TIADebug(Debugger& dbg, Console& console);
    TIA& tia() const { return myTIA; }

    const DebuggerState& getState() override;
    const DebuggerState& getOldState() override { return myOldState; }

    void saveOldState() override;
    string toString() override;

    // TIA byte (or part of a byte) registers
    uInt8 nusiz0(int newVal = -1);
    uInt8 nusiz1(int newVal = -1);
    uInt8 nusizP0(int newVal = -1);
    uInt8 nusizP1(int newVal = -1);
    uInt8 nusizM0(int newVal = -1);
    uInt8 nusizM1(int newVal = -1);
    const string& nusizP0String() { return nusizStrings[nusizP0()]; }
    const string& nusizP1String() { return nusizStrings[nusizP1()]; }

    uInt8 coluP0(int newVal = -1);
    uInt8 coluP1(int newVal = -1);
    uInt8 coluPF(int newVal = -1);
    uInt8 coluBK(int newVal = -1);

    uInt8 sizeBL(int newVal = -1);
    uInt8 ctrlPF(int newVal = -1);

    uInt8 pf0(int newVal = -1);
    uInt8 pf1(int newVal = -1);
    uInt8 pf2(int newVal = -1);

    uInt8 grP0(int newVal = -1);
    uInt8 grP1(int newVal = -1);
    uInt8 posP0(int newVal = -1);
    uInt8 posP1(int newVal = -1);
    uInt8 posM0(int newVal = -1);
    uInt8 posM1(int newVal = -1);
    uInt8 posBL(int newVal = -1);
    uInt8 hmP0(int newVal = -1);
    uInt8 hmP1(int newVal = -1);
    uInt8 hmM0(int newVal = -1);
    uInt8 hmM1(int newVal = -1);
    uInt8 hmBL(int newVal = -1);

    uInt8 audC0(int newVal = -1);
    uInt8 audC1(int newVal = -1);
    uInt8 audF0(int newVal = -1);
    uInt8 audF1(int newVal = -1);
    uInt8 audV0(int newVal = -1);
    uInt8 audV1(int newVal = -1);

    // TIA bool registers
    bool refP0(int newVal = -1);
    bool refP1(int newVal = -1);
    bool enaM0(int newVal = -1);
    bool enaM1(int newVal = -1);
    bool enaBL(int newVal = -1);

    bool vdelP0(int newVal = -1);
    bool vdelP1(int newVal = -1);
    bool vdelBL(int newVal = -1);

    bool resMP0(int newVal = -1);
    bool resMP1(int newVal = -1);

    bool refPF(int newVal = -1);
    bool scorePF(int newVal = -1);
    bool priorityPF(int newVal = -1);

    // Collision registers
    bool collM0_P1() const { return collision(Cx_M0P1); }
    bool collM0_P0() const { return collision(Cx_M0P0); }
    bool collM1_P0() const { return collision(Cx_M1P0); }
    bool collM1_P1() const { return collision(Cx_M1P1); }
    bool collP0_PF() const { return collision(Cx_P0PF); }
    bool collP0_BL() const { return collision(Cx_P0BL); }
    bool collP1_PF() const { return collision(Cx_P1PF); }
    bool collP1_BL() const { return collision(Cx_P1BL); }
    bool collM0_PF() const { return collision(Cx_M0PF); }
    bool collM0_BL() const { return collision(Cx_M0BL); }
    bool collM1_PF() const { return collision(Cx_M1PF); }
    bool collM1_BL() const { return collision(Cx_M1BL); }
    bool collBL_PF() const { return collision(Cx_BLPF); }
    bool collP0_P1() const { return collision(Cx_P0P1); }
    bool collM0_M1() const { return collision(Cx_M0M1); }

    // TIA strobe registers
    void strobeWsync() { mySystem.poke(WSYNC, 0); }
    void strobeRsync() { mySystem.poke(RSYNC, 0); } // not emulated!
    void strobeResP0() { mySystem.poke(RESP0, 0); }
    void strobeResP1() { mySystem.poke(RESP1, 0); }
    void strobeResM0() { mySystem.poke(RESM0, 0); }
    void strobeResM1() { mySystem.poke(RESM1, 0); }
    void strobeResBL() { mySystem.poke(RESBL, 0); }
    void strobeHmove() { mySystem.poke(HMOVE, 0); }
    void strobeHmclr() { mySystem.poke(HMCLR, 0); }
    void strobeCxclr() { mySystem.poke(CXCLR, 0); }

    // Read-only internal TIA state
    int scanlines() const;
    int frameCount() const;
    int clocksThisLine() const;
    bool vsync() const;
    bool vblank() const;
    int vsyncAsInt() const  { return int(vsync());  } // so we can use _vsync pseudo-register
    int vblankAsInt() const { return int(vblank()); } // so we can use _vblank pseudo-register

    shared_ptr<DelayQueueIterator> delayQueueIterator() const;

  private:
    /** Display a color patch for color at given index in the palette */
    string colorSwatch(uInt8 c);

    /** Get specific bits in the collision register (used by collXX_XX) */
    bool collision(CollisionBit id) const;

    string audFreq(uInt8 div);
    string booleanWithLabel(string label, bool value);

  private:
    TiaState myState;
    TiaState myOldState;

    TIA& myTIA;

    string nusizStrings[8];

  private:
    // Following constructors and assignment operators not supported
    TIADebug() = delete;
    TIADebug(const TIADebug&) = delete;
    TIADebug(TIADebug&&) = delete;
    TIADebug& operator=(const TIADebug&) = delete;
    TIADebug& operator=(TIADebug&&) = delete;
};

#endif
