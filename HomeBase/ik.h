#pragma once
#include "stdafx.h"
#include <iostream>     // std::cout
#include <algorithm>    // std::max
#include <math.h>
#include "SDL.h"
#include <vector>
typedef struct {
	SDL_Point p1 = { 0,0 };
	SDL_Point p2 = { 0,0 };
	bool angle;
	int idxParent = 0;
	int idxChild = 0;
}SSource_Bone;
typedef struct {
	std::vector<SSource_Bone> bones;
} SSource_Amature;
double maxdouble(double val1, double val2) {
	if (val1 > val2)
		return val1;
	else if (val2 > val1)
		return val2;
	else return 1;
}
double mindouble(double val1, double val2) {
	if (val1 < val2)
		return val1;
	else if (val2 < val1)
		return val2;
	else return 1;
}

int solveIK
(
	double *angle1,   // Angle of bone 1
	double *angle2,   // Angle of bone 2
	bool solvePosAngle2, // Solve for positive angle 2 instead of negative angle 2
	double length1,      // Length of bone 1. Assumed to be >= zero
	double length2,      // Length of bone 2. Assumed to be >= zero
	double targetX,      // Target x position for the bones to reach
	double targetY       // Target y position for the bones to reach
)
{

	const double epsilon = 0.0001; // used to prevent division by small numbers

	bool foundValidSolution = true;

	double targetDistSqr = (targetX*targetX + targetY*targetY);

	//===
	// Compute a new value for angle2 along with its cosine
	double sinAngle2;
	double cosAngle2;

	double cosAngle2_denom = 2 * length1*length2;
	if (cosAngle2_denom > epsilon)
	{
		cosAngle2 = (targetDistSqr - length1*length1 - length2*length2)
			/ (cosAngle2_denom);

		// if our result is not in the legal cosine range, we can not find a
		// legal solution for the target
		if ((cosAngle2 < -1.0) || (cosAngle2 > 1.0))
			foundValidSolution = false;

		// clamp our value into range so we can calculate the best
		// solution when there are no valid ones
		cosAngle2 = maxdouble(-1, mindouble(1, cosAngle2));

		// compute a new value for angle2
		*angle2 = acos(cosAngle2);

		// adjust for the desired bend direction
		if (!solvePosAngle2)
			*angle2 = -(*angle2);

		// compute the sine of our angle
		sinAngle2 = sin(*angle2);
	}
	else
	{
		// At least one of the bones had a zero length. This means our
		// solvable domain is a circle around the origin with a radius
		// equal to the sum of our bone lengths.
		double totalLenSqr = (length1 + length2) * (length1 + length2);
		if (targetDistSqr < (totalLenSqr - epsilon)
			|| targetDistSqr >(totalLenSqr + epsilon))
		{
			foundValidSolution = false;
		}

		// Only the value of angle1 matters at this point. We can just
		// set angle2 to zero. 
		*angle2 = 0.0;
		cosAngle2 = 1.0;
		sinAngle2 = 0.0;
	}

	//===
	// Compute the value of angle1 based on the sine and cosine of angle2
	double triAdjacent = length1 + length2*cosAngle2;
	double triOpposite = length2*sinAngle2;

	double tanY = targetY*triAdjacent - targetX*triOpposite;
	double tanX = targetX*triAdjacent + targetY*triOpposite;

	// Note that it is safe to call Atan2(0,0) which will happen if targetX and
	// targetY are zero
	*angle1 = atan2(tanY, tanX);

	return foundValidSolution;
}
