/****************************************************************************
 *
 * $Id$
 *
 *
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2011 by INRIA. All rights reserved.
 *
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ViSP with software that can not be combined with the GNU
 * GPL, please contact INRIA about acquiring a ViSP Professional
 * Edition License.
 *
 * See http://www.irisa.fr/lagadic/visp/visp.html for more information.
 *
 * This software was developed at:
 * INRIA Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 * http://www.irisa.fr/lagadic
 *
 * If you have questions regarding the use of this file, please contact
 * INRIA at visp@inria.fr
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 * Description:
 * API for using a Microsoft Kinect device
 * Requires libfreenect as a third party library
 *
 * Authors:
 * Celine Teuliere
 *
 *****************************************************************************/


#ifndef __VP_KINECT__
#define __VP_KINECT__

#include <visp/vpConfig.h>
#if defined(VISP_HAVE_LIBFREENECT) && defined(VISP_HAVE_PTHREAD)

#include <iostream>
#include <libfreenect.hpp>

#include <visp/vpMutex.h> // need pthread
#include <visp/vpImage.h>
#include <visp/vpCameraParameters.h>

/*!

  \class vpKinect

  \ingroup KinectDriver

  \brief Driver for the Kinect device.

  The following example shows how to use this class to acquire data
  (depth map and color image) from a Kinect.

  \code
#include <visp/vpConfig.h>
#include <visp/vpImage.h>
#include <visp/vpKinect.h>

int main() {
#ifdef VISP_HAVE_LIBFREENECT
  // Init Kinect device
  Freenect::Freenect<vpKinect> freenect;
  vpKinect * kinect = &freenect.createDevice(0);
  kinect->start(); // Start acquisition thread

  // Set tilt angle
  float angle = -5;
  kinect->setTiltAngle(angle);

  vpImage<unsigned char> I(480,640);
  vpImage<vpRGBa> Irgb(480,640);
  vpImage<float> dmap(480,640);

  // Acquisition loop
  for (int i=0; i<100; i++)
    {
      kinect->updateState();
      kinect->getDepthMap(dmap,I);
      kinect->getRGB(Irgb);
    }
  kinect->stop();//!stop acquisition thread
#endif
  return 0;
}
  \endcode
*/
class VISP_EXPORT vpKinect : public Freenect::FreenectDevice
{
public:
	vpKinect(freenect_context *ctx, int index);
	virtual ~vpKinect();

	void start();
	void stop();

	void setTiltAngle(float angle);

	bool getDepthMap(vpImage<float>& map, vpImage<unsigned char>& Imap);
	bool getRGB(vpImage<vpRGBa>& IRGB);


	inline void getIRCamParameters(vpCameraParameters &cam) const {
	  cam = IRcam;
	}
	inline void getRGBCamParameters(vpCameraParameters &cam) const {
	  cam = RGBcam;
	}
	inline void setIRCamParameters(const vpCameraParameters &cam) {
	  IRcam = cam;
	}
	inline void setRGBCamParameters(const vpCameraParameters &cam) {
	  RGBcam = cam;
	}

private:
	//!Instantiation of Freenect virtual functions
	// Do not call directly even in child
	void VideoCallback(void* rgb, uint32_t timestamp);

	// Do not call directly even in child
	void DepthCallback(void* depth, uint32_t timestamp);

private:
	vpMutex m_rgb_mutex;
	vpMutex m_depth_mutex;

	bool m_new_rgb_frame;
	bool m_new_depth_frame;

	unsigned height,width;
	vpCameraParameters RGBcam, IRcam;//intrinsic parameters of the two cameras

	//Acces protected by a mutex
	vpImage<float> dmap;
	vpImage<unsigned char> Idmap;
	vpImage<vpRGBa> IRGB;
};



#endif//ViSP has libfreenect

#endif