#ifndef _EULER_ANGLES_H_
#define _EULER_ANGLES_H_

#pragma anon_unions

namespace Euler
{
	struct CAngles
	{
		double axis[1][3];
		const double * operator[]( unsigned int i ) const {return axis[i];}
		double * operator[]( unsigned int i ) {return axis[i];}
		double X() const { return axis[0][0]; }
		double Y() const { return axis[0][1]; }
		double Z() const { return axis[0][2]; }
		void X( double a ){ axis[0][0] = a; };
		void Y( double a ){ axis[0][1] = a; };
		void Z( double a ){ axis[0][2] = a; };
	};

	struct CMatrixRotation
	{
		double k[3][3];
		const double * operator[]( unsigned int i ) const {return k[i];}
		double * operator[]( unsigned int i ) {return k[i];}
	};
	CAngles operator * ( const CMatrixRotation &, const CAngles & );
	
	double DegToRad( double );
	double RadToDeg( double );
	
	void CalcMatrixRotation( CMatrixRotation *, double alpha, double beta, double gamma );
	void CalcMatrixRotationX( CMatrixRotation *, double alpha );
	void CalcMatrixRotationY( CMatrixRotation *, double alpha );
	void CalcMatrixRotationZ( CMatrixRotation *, double alpha );
	CAngles RotateX( const CAngles &, double alpha );
	CAngles RotateY( const CAngles &, double alpha );
	CAngles RotateZ( const CAngles &, double alpha );
	
	double CalcRoll( const CAngles & );
	double CalcPitch( const CAngles & );
}

#endif
