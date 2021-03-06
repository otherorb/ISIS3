#ifndef mappt_h
#define mappt_h

#include "Pvl.h"
#include "UserInterface.h"

namespace Isis{
  extern void mappt(Cube *cube, UserInterface &ui, Pvl *log=nullptr, CubeAttributeInput* inAtt = nullptr);
  extern void mappt(UserInterface &ui, Pvl *log= nullptr);
}

#endif
