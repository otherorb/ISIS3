/**
 * @file
 *
 *   Unless noted otherwise, the portions of Isis written by the USGS are public
 *   domain. See individual third-party library and package descriptions for 
 *   intellectual property information,user agreements, and related information.
 *
 *   Although Isis has been used by the USGS, no warranty, expressed or implied,
 *   is made by the USGS as to the accuracy and functioning of such software 
 *   and related material nor shall the fact of distribution constitute any such 
 *   warranty, and no responsibility is assumed by the USGS in connection 
 *   therewith.
 *
 *   For additional information, launch
 *   $ISISROOT/doc//documents/Disclaimers/Disclaimers.html in a browser or see 
 *   the Privacy &amp; Disclaimers page on the Isis website,
 *   http://isis.astrogeology.usgs.gov, and the USGS privacy and disclaimers on
 *   http://www.usgs.gov/privacy.html.
 */

#include "NewHorizonsLorriDistortionMap.h"
#include "CameraFocalPlaneMap.h"

using namespace std;

namespace Isis {

  /**
   *  
   * Constructs a Distortion Map object for the New Horizons LORRI Camera. 
   *  
   * @param parent Pointer to parent Camera object
   * @param e2
   * @param e2
   * @param e2
   * @param zDirection passed on to the parrent
   * 
   * @internal
   *   @history 2014-06-15 Stuart Sides - Original version
   */

  NewHorizonsLorriDistortionMap::NewHorizonsLorriDistortionMap(Camera *parent, double e2, double e5,
      double e6, double zDirection) : CameraDistortionMap(parent, zDirection) {
    p_e2 = e2;
    p_e5 = e5;
    p_e6 = e6;
  }


  /**
   * Compute undistorted focal plane x/y
   *
   * Compute undistorted focal plane x/y given a distorted focal plane x/y. After calling this 
   * method, you can obtain the undistorted x/y via the UndistortedFocalPlaneX and 
   * UndistortedFocalPlaneY methods 
   *
   * @param dx Distorted focal plane x, in millimeters
   * @param dy Distorted focal plane y, in millimeters
   *
   * @return whether the conversion was successful
   */
  bool NewHorizonsLorriDistortionMap::SetFocalPlane(const double dx, const double dy) {

    p_focalPlaneX = dx;
    p_focalPlaneY = dy;

    // Reducing to the principle point offset (xp,yp)
    double x = dx;
    double y = dy;

    // r is the distance between the principal point and the measured point on the image
    double rr = x * x + y * y;

    //  dr is the radial distortion contribution
    double dr = 1.0 + rr * p_e2 + y * p_e5 + x * p_e6;

    // Image coordinates corrected for distortion
    p_undistortedFocalPlaneX = x * dr;
    p_undistortedFocalPlaneY = y * dr;

    return true;

  }


  /**
   * Compute distorted focal plane x/y
   *
   * Compute distorted focal plane x/y given an undistorted focal plane x/y.
   * After calling this method, you can obtain the distorted x/y via the
   * FocalPlaneX and FocalPlaneY methods
   *
   * @param ux Undistorted focal plane x, in millimeters
   * @param uy Undistorted focal plane y, in millimeters
   *
   * @return whether the conversion was successful
   */
  bool NewHorizonsLorriDistortionMap::SetUndistortedFocalPlane(const double ux, const double uy) {

    // Image coordinates prior to introducing distortion
    p_undistortedFocalPlaneX = ux;
    p_undistortedFocalPlaneY = uy;

    double xt = ux;
    double yt = uy;

    double xx, yy, xy, rr;
    double xdistortion, ydistortion;
    double xdistorted, ydistorted;
    double xprevious, yprevious;

    xprevious = 1000000.0;
    yprevious = 1000000.0;

    double tolerance = 0.000001;

    bool bConverged = false;

    // Iterating to introduce distortion...
    // We stop when the difference between distorted coordinates
    // in successive iterations is at or below the given tolerance
    for (int i = 0; i < 50; i++) {
      xx = xt * xt;
      yy = yt * yt;
      xy = xt * yt;
      rr = xx + yy;

      // Distortion at the current point location
      xdistortion = xt * rr * p_e2 + xy * p_e5 + xx * p_e6;
      ydistortion = yt * rr * p_e2 + yy * p_e5 + xy * p_e6;

      // Updated image coordinates
      xt = ux - xdistortion;
      yt = uy - ydistortion;

      // Distorted point corrected for principal point
      xdistorted = xt; // No PP for Lorrie
      ydistorted = yt; // No PP for Lorrie

      // Check for convergence
      if ((fabs(xt - xprevious) <= tolerance) && (fabs(yt - yprevious) <= tolerance)) {
        bConverged = true;
        break;
      }

      xprevious = xt;
      yprevious = yt;
    }

    if (bConverged) {
      p_focalPlaneX = xdistorted;
      p_focalPlaneY = ydistorted;
    }

    return bConverged;

  }
}
