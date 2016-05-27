#include "Euler_angles.h"
#include "math.h"

namespace
{
	const double pi = acos((double)-1);
}

double Euler::DegToRad( double deg )
{
	return deg*pi/180;
}

double Euler::RadToDeg( double rad )
{
	return rad*180/pi;
}

void Euler::CalcMatrixRotation( CMatrixRotation * m, double alpha, double beta, double gamma )
{
	m->k[0][0] = cos( alpha )*cos( gamma ) - sin( alpha )*cos( beta )*sin( gamma );
	m->k[0][1] = sin( alpha )*cos( gamma ) + cos( alpha )*cos( beta )*sin( gamma );
	m->k[0][2] = sin( beta )*sin( gamma );
	
	m->k[1][0] = -cos( alpha )*sin( gamma ) - sin( alpha )*cos( beta )*cos( gamma );
	m->k[1][1] = -sin( alpha )*sin( gamma ) + cos( alpha )*cos( beta )*cos( gamma );
	m->k[1][2] = sin( beta )*cos( gamma );
	
	m->k[2][0] = sin( alpha )*sin( beta );
	m->k[2][1] = -cos( alpha )*sin( beta );
	m->k[2][2] = cos( beta );
}

void Euler::CalcMatrixRotationX( CMatrixRotation * m, double alpha )
{
	m->k[0][0] = 1;
	m->k[0][1] = 0;
	m->k[0][2] = 0;
	
	m->k[1][0] = 0;
	m->k[1][1] = cos( alpha );
	m->k[1][2] = sin( alpha );
	
	m->k[2][0] = 0;
	m->k[2][1] = -sin( alpha );
	m->k[2][2] = cos( alpha );
}

void Euler::CalcMatrixRotationY( CMatrixRotation * m, double alpha )
{
	m->k[0][0] = cos( alpha );
	m->k[0][1] = 0;
	m->k[0][2] = -sin( alpha );
	
	m->k[1][0] = 0;
	m->k[1][1] = 1;
	m->k[1][2] = 0;
	
	m->k[2][0] = sin( alpha );
	m->k[2][1] = 0;
	m->k[2][2] = cos( alpha );
}

void Euler::CalcMatrixRotationZ( CMatrixRotation * m, double alpha )
{
	m->k[0][0] = cos( alpha );
	m->k[0][1] = sin( alpha );
	m->k[0][2] = 0;
	
	m->k[1][0] = -sin( alpha );
	m->k[1][1] = cos( alpha );
	m->k[1][2] = 0;
	
	m->k[2][0] = 0;
	m->k[2][1] = 0;
	m->k[2][2] = 1;
}

Euler::CAngles Euler::operator*( const CMatrixRotation & m, const CAngles & a)
{
	CAngles res;
	for( int i = 0; i < 3; ++i  )
		res[0][i] = m[0][i]*a[0][0] + m[1][i]*a[0][1] + m[2][i]*a[0][2];
	return res;
}

Euler::CAngles Euler::RotateX( const CAngles & a, double alpha )
{
	CMatrixRotation m;
	CalcMatrixRotationX( &m, alpha );
	return m * a;
}

Euler::CAngles Euler::RotateY( const CAngles & a, double alpha )
{
	CMatrixRotation m;
	CalcMatrixRotationY( &m, alpha );
	return m * a;
}

Euler::CAngles Euler::RotateZ( const CAngles & a, double alpha )
{
	CMatrixRotation m;
	CalcMatrixRotationZ( &m, alpha );
	return m * a;
}

double Euler::CalcRoll( const CAngles & a )
{
	return atan2( a.Y(), a.Z() );
}

double Euler::CalcPitch( const CAngles & a )
{
	return atan2( a.X(), a.Z() );
}
