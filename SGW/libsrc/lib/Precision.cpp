// Copyright (c) 2005,�����Ƽ��ɷ����޹�˾������ҵ��
// All rights reserved.

#include "Precision.h"

//##ModelId=425506480319
void Precision::set(int *iValue, int iRatePrecision, int iCalPrecision)
{
	int i, iTemp, iTemp2, iNewValue;

	iNewValue = *iValue;
	iTemp2 = abs (iRatePrecision);
	for (i=0, iTemp=1; i<iTemp2; i++)
		iTemp *= 10;

	if (iRatePrecision != 0) {
		if (iRatePrecision > 0) {
			iNewValue *= iTemp;
		} else {
			iNewValue /= iTemp;

			/* �������� */
			iTemp2 = iTemp/2;
			if ((*iValue)%iTemp >= iTemp2)
				iNewValue++;
		}
	}

	if (iCalPrecision > 0) {
		for (i=0,iTemp=1; i<iCalPrecision; i++)
			iTemp *= 10;

		iNewValue = iNewValue - iNewValue%iTemp;
	}

	*iValue = iNewValue;
}


