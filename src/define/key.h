#ifndef ZODIA_DEFINE_KEY_H_
#define ZODIA_DEFINE_KEY_H_

#include <cstdint>

enum KeyName {
  // letter keys
  kKeyA, kKeyB, kKeyC, kKeyD, kKeyE, kKeyF, kKeyG, kKeyH, kKeyI, kKeyJ,
  kKeyK, kKeyL, kKeyM, kKeyN, kKeyO, kKeyP, kKeyQ, kKeyR, kKeyS, kKeyT,
  kKeyU, kKeyV, kKeyW, kKeyX, kKeyY, kKeyZ,
  // digit keys
  kKey0, kKey1, kKey2, kKey3, kKey4,
  kKey5, kKey6, kKey7, kKey8, kKey9,
  // other keys
  kKeyBackspace, kKeySpace, kKeyTab, kKeyEnter,
  kKeyShiftL, kKeyShiftR, kKeyCtrlL, kKeyCtrlR, kKeyAltL, kKeyAltR,
  kKeyArrowU, kKeyArrowD, kKeyArrowL, kKeyArrowR,
};

using KeyStatus = std::uint64_t;

// some helper macros
#define KEY_PRESSED(status, key)              \
    ((status) & (1 << (key)))
#define SET_KEY_STATUS(status, pressed, key)  \
    status |= (!!(pressed) & 1) << (key)

#endif  // ZODIA_DEFINE_KEY_H_
