/** This is free and unencumbered software released into the public domain.
The authors of ISIS do not claim copyright on the contents of this file.
For more details about the LICENSE terms and the AUTHORS, you will
find files of those names at the top level of this repository. **/

/* SPDX-License-Identifier: CC0-1.0 */

#include "RadarCamera.h"

namespace Isis {
  /**
   * Constructs the RadarCamera object
   *
   * @param lab Pvl label used to create the parent Camera object
   */
  RadarCamera::RadarCamera(Isis::Cube &cube) : Camera(cube) {
  }
};

