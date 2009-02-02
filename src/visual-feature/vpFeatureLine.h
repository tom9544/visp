/****************************************************************************
 *
 * $Id: vpFeatureLine.h,v 1.11 2008-09-26 15:21:02 fspindle Exp $
 *
 * Copyright (C) 1998-2006 Inria. All rights reserved.
 *
 * This software was developed at:
 * IRISA/INRIA Rennes
 * Projet Lagadic
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * http://www.irisa.fr/lagadic
 *
 * This file is part of the ViSP toolkit.
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined by Trolltech AS of Norway and appearing in the file
 * LICENSE included in the packaging of this file.
 *
 * Licensees holding valid ViSP Professional Edition licenses may
 * use this file in accordance with the ViSP Commercial License
 * Agreement provided with the Software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Contact visp@irisa.fr if any conditions of this licensing are
 * not clear to you.
 *
 * Description:
 * 2D line visual feature.
 *
 * Authors:
 * Eric Marchand
 *
 *****************************************************************************/


#ifndef vpFeatureLine_H
#define vpFeatureLine_H

/*!
  \file vpFeatureLine.h
  \brief Class that defines 2D line visual feature
*/

#include <visp/vpConfig.h>
#include <visp/vpMatrix.h>
#include <visp/vpBasicFeature.h>


#include <visp/vpHomogeneousMatrix.h>
#include <visp/vpRGBa.h>


/*!
  \class vpFeatureLine
  \ingroup VsFeature2
  \brief Class that defines a 2D line visual feature \f$ s\f$ which is composed by two parameters that are \f$ \rho \f$ and \f$ \theta \f$.

  In this class, the equation line in the image frame is given by :
  \f[ x \times cos(\theta) + y \times sin(\theta) -\rho = 0 \f]
  Here \f$ x \f$ and \f$ y \f$ are the coordinates of a point belonging to the line and they are given in meter. The following image shows the meanings of the distance \f$ \rho \f$ and the angle \f$ \theta \f$.

  \image html vpFeatureLine.gif
  \image latex vpFeatureLine.ps  width=10cm

  You have to note that the \f$ \theta \f$ angle has its value between \f$ -\pi/2 \f$ and \f$ \pi/2 \f$ and that the \f$ \rho \f$ distance can be positive or negative. The conventions are illustrated by the image above.

  The visual features can be set easily from an instance of the classes vpLine, vpMeLine or vpCylinder. For more precision see the class vpFeatureBuilder.

  Once the values of the visual features are set, the interaction() method allows to compute the interaction matrix \f$ L \f$ associated to the visual feature, while the error() method computes the error vector \f$(s - s^*)\f$ between the current visual feature and the desired one.

  The code below shows how to create a eye-in hand visual servoing task using a 2D line feature \f$(\rho,\theta)\f$ that correspond to the 2D equation of a line in the image frame. To control six degrees of freedom, at least four other features must be considered like two other line features for example. First we create a current (\f$s\f$) 2D line feature. Then we set the task to use the interaction matrix associated to the current feature \f$L_s\f$. And finally we compute the camera velocity \f$v=-\lambda \; L_s^+ \; (s-s^*)\f$. The current feature \f$s\f$ is updated in the while() loop.

  \code
#include <visp/vpFeatureLine.h>
#include <visp/vpServo.h>

int main()
{
  vpServo task; // Visual servoing task

  vpFeatureLine sd; //The desired line feature.
  //Sets the desired features rho and theta
  double rhod = 0;
  double thetad = 0;
  //Sets the parameters which describe the equation of a plan in the camera frame : AX+BY+CZ+D=0.
  //The line described by the features belongs to this plan.
  //Normally two plans are needed to describe a line. But to compute the interaction matrix only
  //one equation of the two plans is needed.
  //Notes that the Dd value must not be equal to zero !
  double Ad = 0;
  double Bd = 0;
  double Cd = 1;
  double Dd = -1;
  //Set the line feature thanks to the desired parameters.
  sd.buildfrom(rhod, thetad, Ad,Bd, Cd, Dd);

  vpFeatureLine s; //The current line feature.
  //Sets the current features rho and theta
  double rho;  //You have to compute the value of rho.
  double theta;  //You have to compute the value of theta.
  //Set the line feature thanks to the current parameters.
  s.buildfrom(rho, theta);
  //In this case the parameters A, B, C, D are not needed because the interaction matrix is computed
  //with the desired visual feature.

  // Set eye-in-hand control law. 
  // The computed velocities will be expressed in the camera frame
  task.setServo(vpServo::EYEINHAND_CAMERA);
  // Interaction matrix is computed with the desired visual features sd
  task.setInteractionMatrixType(vpServo::CURRENT);

  // Add the 2D line feature to the task
  task.addFeature(s, sd);

  // Control loop
  while(1) {
    // The new parameters rho and theta mus be computed here.
    
    // Update the current line visual feature
    s.buildfrom(rho, theta);
    
    // compute the control law
    vpColVector v = task.computeControlLaw(); // camera velocity
  }
  return 0;
}
  \endcode

  If you want to build your own control law, this other example shows how to create a current (\f$s\f$) and desired (\f$s^*\f$) 3D \f$\theta u\f$ visual feature, compute the corresponding error vector \f$(s-s^*)\f$ and finally build the interaction matrix \f$L_s\f$.

  \code
#include <visp/vpFeatureLine.h>
#include <visp/vpMatrix.h>

int main()
{
  vpFeatureLine sd; //The desired line feature.
  //Sets the desired features rho and theta
  double rhod = 0;
  double thetad = 0;
  //Sets the parameters which describe the equation of a plan in the camera frame : AX+BY+CZ+D=0.
  double Ad = 0;
  double Bd = 0;
  double Cd = 1;
  double Dd = -1;
  //Set the line feature thanks to the desired parameters.
  sd.buildfrom(rhod, thetad, Ad,Bd, Cd, Dd);

  vpFeatureLine s; //The current line feature.
  //Sets the current features rho and theta
  double rho;  //You have to compute the value of rho.
  double theta;  //You have to compute the value of theta.
  //Sets the parameters which describe the equation of a plan in the camera frame : AX+BY+CZ+D=0.
  double A;  //You have to compute the value of A.
  double B;  //You have to compute the value of B.
  double C;  //You have to compute the value of C.
  double D;  //You have to compute the value of D. D must not be equal to zero !
  //Set the line feature thanks to the current parameters.
  s.buildfrom(rho, theta, A, B, C, D);

  // Compute the interaction matrix L_s for the current line feature
  vpMatrix L = s.interaction();
  // You can also compute the interaction matrix L_s for the desired line feature
  // The corresponding line of code is : vpMatrix L = sd.interaction();

  // Compute the error vector (s-sd) for the ThetaU feature
  s.error(s_star);
}
  \endcode

*/
class VISP_EXPORT vpFeatureLine : public vpBasicFeature
{
  /*!
    attributes and members directly related to the vpBasicFeature needs
    other functionalities ar usefull but not mandatory
  */
private:
  //! FeatureLine depth (required to compute the interaction matrix)
  //!  equation of a plane
  double A,B,C,D ;

public:

  void init() ;
  vpFeatureLine() ;
  virtual ~vpFeatureLine() { ; }

public:

  void setRhoTheta(const double rho, const double theta) ;
  void setABCD(const double A, const double B,
	       const double C, const double D) ;

  //  void buildFrom(const vpLine &l) ;
  //  void buildFrom(const vpCylinder &c, const int l) ;
  void buildFrom(const double rho, const double theta) ;
  void buildFrom(const double rho, const double theta,
		 const double A, const double B,
		 const double C, const double D) ;


  /*!
    Return the \f$ \rho \f$ subset value of the visual feature \f$ s \f$.
  */
  double getRho() const  { return s[0] ; }

  /*!
    Return the \f$ \theta \f$ subset value of the visual feature \f$ s \f$.
  */
  double getTheta() const { return s[1] ; }


public:

  /*! 

    Function used to select the \f$ \rho \f$ subset of the line visual feature.

    This function is to use in conjunction with interaction() in order to compute the interaction matrix associated to \f$ \rho \f$.

    This function is also useful in the vpServo class to indicate that a subset of the visual feature is to use in the control law:

    \code 
    vpFeatureLine s;
    vpServo task;
    ...
    // Add the (rho) subset features from the 2D line
    task.addFeature(s, vpFeatureLine::selectRho);
    \endcode
  */
  inline static int selectRho()  { return FEATURE_LINE[0] ; }

  /*! 

    Function used to select the \f$ \theta \f$ subset of the line visual feature.

    This function is to use in conjunction with interaction() in order to compute the interaction matrix associated to \f$ \theta \f$.

    This function is also useful in the vpServo class to indicate that a subset of the visual feature is to use in the control law:

    \code 
    vpFeatureLine s;
    vpServo task;
    ...
    // Add the (rho) subset features from the 2D line
    task.addFeature(s, vpFeatureLine::selectTheta);
    \endcode
  */
  inline static int selectTheta()  { return FEATURE_LINE[1] ; }

  vpMatrix  interaction(const int select = FEATURE_ALL) const;

  vpColVector error(const vpBasicFeature &s_star,
		    const int select = FEATURE_ALL)  ;

  //vpColVector error(const int select = FEATURE_ALL)  ;

  void print(const int select= FEATURE_ALL) const ;

  vpFeatureLine *duplicate() const ;


public:
  void display(const vpCameraParameters &cam,
	       vpImage<unsigned char> &I,
	       vpColor::vpColorType color=vpColor::green) const ;
  void display(const vpCameraParameters &cam,
               vpImage<vpRGBa> &I,
               vpColor::vpColorType color=vpColor::green) const ;

} ;



#endif

/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */
