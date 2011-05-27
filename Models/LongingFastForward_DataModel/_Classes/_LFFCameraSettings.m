// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCameraSettings.m instead.

#import "_LFFCameraSettings.h"

@implementation LFFCameraSettingsID
@end

@implementation _LFFCameraSettings

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"CameraSettings" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"CameraSettings";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"CameraSettings" inManagedObjectContext:moc_];
}

- (LFFCameraSettingsID*)objectID {
	return (LFFCameraSettingsID*)[super objectID];
}

+ (NSSet *)keyPathsForValuesAffectingValueForKey:(NSString *)key {
	NSSet *keyPaths = [super keyPathsForValuesAffectingValueForKey:key];
	
	if ([key isEqualToString:@"gainValueValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"gainValue"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"gainAutoRateValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"gainAutoRate"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"exposureAutoTargetValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"exposureAutoTarget"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"exposureAutoOutliersValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"exposureAutoOutliers"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"gainModeValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"gainMode"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"exposureModeValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"exposureMode"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"ISOValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"ISO"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"gainAutoAdjustTolValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"gainAutoAdjustTol"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"exposureAutoAlgValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"exposureAutoAlg"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"whitebalModeValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"whitebalMode"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"exposureAutoAdjustTolValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"exposureAutoAdjustTol"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"gainAutoOutliersValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"gainAutoOutliers"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"whitebalValueRedValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"whitebalValueRed"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"whitebalAutoRateValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"whitebalAutoRate"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"gainAutoTargetValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"gainAutoTarget"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"gainAutoMaxValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"gainAutoMax"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"whitebalValueBlueValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"whitebalValueBlue"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"exposureAutoMaxValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"exposureAutoMax"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"exposureAutoMinValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"exposureAutoMin"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"gainAutoMinValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"gainAutoMin"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"pixelFormatValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"pixelFormat"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"exposureValueValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"exposureValue"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"exposureAutoRateValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"exposureAutoRate"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"whitebalAutoAdjustTolValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"whitebalAutoAdjustTol"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}

	return keyPaths;
}




@dynamic gainValue;



- (long long)gainValueValue {
	NSNumber *result = [self gainValue];
	return [result longLongValue];
}

- (void)setGainValueValue:(long long)value_ {
	[self setGainValue:[NSNumber numberWithLongLong:value_]];
}

- (long long)primitiveGainValueValue {
	NSNumber *result = [self primitiveGainValue];
	return [result longLongValue];
}

- (void)setPrimitiveGainValueValue:(long long)value_ {
	[self setPrimitiveGainValue:[NSNumber numberWithLongLong:value_]];
}





@dynamic gainAutoRate;



- (short)gainAutoRateValue {
	NSNumber *result = [self gainAutoRate];
	return [result shortValue];
}

- (void)setGainAutoRateValue:(short)value_ {
	[self setGainAutoRate:[NSNumber numberWithShort:value_]];
}

- (short)primitiveGainAutoRateValue {
	NSNumber *result = [self primitiveGainAutoRate];
	return [result shortValue];
}

- (void)setPrimitiveGainAutoRateValue:(short)value_ {
	[self setPrimitiveGainAutoRate:[NSNumber numberWithShort:value_]];
}





@dynamic exposureAutoTarget;



- (short)exposureAutoTargetValue {
	NSNumber *result = [self exposureAutoTarget];
	return [result shortValue];
}

- (void)setExposureAutoTargetValue:(short)value_ {
	[self setExposureAutoTarget:[NSNumber numberWithShort:value_]];
}

- (short)primitiveExposureAutoTargetValue {
	NSNumber *result = [self primitiveExposureAutoTarget];
	return [result shortValue];
}

- (void)setPrimitiveExposureAutoTargetValue:(short)value_ {
	[self setPrimitiveExposureAutoTarget:[NSNumber numberWithShort:value_]];
}





@dynamic exposureAutoOutliers;



- (short)exposureAutoOutliersValue {
	NSNumber *result = [self exposureAutoOutliers];
	return [result shortValue];
}

- (void)setExposureAutoOutliersValue:(short)value_ {
	[self setExposureAutoOutliers:[NSNumber numberWithShort:value_]];
}

- (short)primitiveExposureAutoOutliersValue {
	NSNumber *result = [self primitiveExposureAutoOutliers];
	return [result shortValue];
}

- (void)setPrimitiveExposureAutoOutliersValue:(short)value_ {
	[self setPrimitiveExposureAutoOutliers:[NSNumber numberWithShort:value_]];
}





@dynamic gainMode;



- (long long)gainModeValue {
	NSNumber *result = [self gainMode];
	return [result longLongValue];
}

- (void)setGainModeValue:(long long)value_ {
	[self setGainMode:[NSNumber numberWithLongLong:value_]];
}

- (long long)primitiveGainModeValue {
	NSNumber *result = [self primitiveGainMode];
	return [result longLongValue];
}

- (void)setPrimitiveGainModeValue:(long long)value_ {
	[self setPrimitiveGainMode:[NSNumber numberWithLongLong:value_]];
}





@dynamic exposureMode;



- (long long)exposureModeValue {
	NSNumber *result = [self exposureMode];
	return [result longLongValue];
}

- (void)setExposureModeValue:(long long)value_ {
	[self setExposureMode:[NSNumber numberWithLongLong:value_]];
}

- (long long)primitiveExposureModeValue {
	NSNumber *result = [self primitiveExposureMode];
	return [result longLongValue];
}

- (void)setPrimitiveExposureModeValue:(long long)value_ {
	[self setPrimitiveExposureMode:[NSNumber numberWithLongLong:value_]];
}





@dynamic ISO;



- (short)ISOValue {
	NSNumber *result = [self ISO];
	return [result shortValue];
}

- (void)setISOValue:(short)value_ {
	[self setISO:[NSNumber numberWithShort:value_]];
}

- (short)primitiveISOValue {
	NSNumber *result = [self primitiveISO];
	return [result shortValue];
}

- (void)setPrimitiveISOValue:(short)value_ {
	[self setPrimitiveISO:[NSNumber numberWithShort:value_]];
}





@dynamic gainAutoAdjustTol;



- (short)gainAutoAdjustTolValue {
	NSNumber *result = [self gainAutoAdjustTol];
	return [result shortValue];
}

- (void)setGainAutoAdjustTolValue:(short)value_ {
	[self setGainAutoAdjustTol:[NSNumber numberWithShort:value_]];
}

- (short)primitiveGainAutoAdjustTolValue {
	NSNumber *result = [self primitiveGainAutoAdjustTol];
	return [result shortValue];
}

- (void)setPrimitiveGainAutoAdjustTolValue:(short)value_ {
	[self setPrimitiveGainAutoAdjustTol:[NSNumber numberWithShort:value_]];
}





@dynamic exposureAutoAlg;



- (short)exposureAutoAlgValue {
	NSNumber *result = [self exposureAutoAlg];
	return [result shortValue];
}

- (void)setExposureAutoAlgValue:(short)value_ {
	[self setExposureAutoAlg:[NSNumber numberWithShort:value_]];
}

- (short)primitiveExposureAutoAlgValue {
	NSNumber *result = [self primitiveExposureAutoAlg];
	return [result shortValue];
}

- (void)setPrimitiveExposureAutoAlgValue:(short)value_ {
	[self setPrimitiveExposureAutoAlg:[NSNumber numberWithShort:value_]];
}





@dynamic whitebalMode;



- (short)whitebalModeValue {
	NSNumber *result = [self whitebalMode];
	return [result shortValue];
}

- (void)setWhitebalModeValue:(short)value_ {
	[self setWhitebalMode:[NSNumber numberWithShort:value_]];
}

- (short)primitiveWhitebalModeValue {
	NSNumber *result = [self primitiveWhitebalMode];
	return [result shortValue];
}

- (void)setPrimitiveWhitebalModeValue:(short)value_ {
	[self setPrimitiveWhitebalMode:[NSNumber numberWithShort:value_]];
}





@dynamic exposureAutoAdjustTol;



- (short)exposureAutoAdjustTolValue {
	NSNumber *result = [self exposureAutoAdjustTol];
	return [result shortValue];
}

- (void)setExposureAutoAdjustTolValue:(short)value_ {
	[self setExposureAutoAdjustTol:[NSNumber numberWithShort:value_]];
}

- (short)primitiveExposureAutoAdjustTolValue {
	NSNumber *result = [self primitiveExposureAutoAdjustTol];
	return [result shortValue];
}

- (void)setPrimitiveExposureAutoAdjustTolValue:(short)value_ {
	[self setPrimitiveExposureAutoAdjustTol:[NSNumber numberWithShort:value_]];
}





@dynamic gainAutoOutliers;



- (short)gainAutoOutliersValue {
	NSNumber *result = [self gainAutoOutliers];
	return [result shortValue];
}

- (void)setGainAutoOutliersValue:(short)value_ {
	[self setGainAutoOutliers:[NSNumber numberWithShort:value_]];
}

- (short)primitiveGainAutoOutliersValue {
	NSNumber *result = [self primitiveGainAutoOutliers];
	return [result shortValue];
}

- (void)setPrimitiveGainAutoOutliersValue:(short)value_ {
	[self setPrimitiveGainAutoOutliers:[NSNumber numberWithShort:value_]];
}





@dynamic whitebalValueRed;



- (short)whitebalValueRedValue {
	NSNumber *result = [self whitebalValueRed];
	return [result shortValue];
}

- (void)setWhitebalValueRedValue:(short)value_ {
	[self setWhitebalValueRed:[NSNumber numberWithShort:value_]];
}

- (short)primitiveWhitebalValueRedValue {
	NSNumber *result = [self primitiveWhitebalValueRed];
	return [result shortValue];
}

- (void)setPrimitiveWhitebalValueRedValue:(short)value_ {
	[self setPrimitiveWhitebalValueRed:[NSNumber numberWithShort:value_]];
}





@dynamic whitebalAutoRate;



- (short)whitebalAutoRateValue {
	NSNumber *result = [self whitebalAutoRate];
	return [result shortValue];
}

- (void)setWhitebalAutoRateValue:(short)value_ {
	[self setWhitebalAutoRate:[NSNumber numberWithShort:value_]];
}

- (short)primitiveWhitebalAutoRateValue {
	NSNumber *result = [self primitiveWhitebalAutoRate];
	return [result shortValue];
}

- (void)setPrimitiveWhitebalAutoRateValue:(short)value_ {
	[self setPrimitiveWhitebalAutoRate:[NSNumber numberWithShort:value_]];
}





@dynamic gainAutoTarget;



- (short)gainAutoTargetValue {
	NSNumber *result = [self gainAutoTarget];
	return [result shortValue];
}

- (void)setGainAutoTargetValue:(short)value_ {
	[self setGainAutoTarget:[NSNumber numberWithShort:value_]];
}

- (short)primitiveGainAutoTargetValue {
	NSNumber *result = [self primitiveGainAutoTarget];
	return [result shortValue];
}

- (void)setPrimitiveGainAutoTargetValue:(short)value_ {
	[self setPrimitiveGainAutoTarget:[NSNumber numberWithShort:value_]];
}





@dynamic gainAutoMax;



- (long long)gainAutoMaxValue {
	NSNumber *result = [self gainAutoMax];
	return [result longLongValue];
}

- (void)setGainAutoMaxValue:(long long)value_ {
	[self setGainAutoMax:[NSNumber numberWithLongLong:value_]];
}

- (long long)primitiveGainAutoMaxValue {
	NSNumber *result = [self primitiveGainAutoMax];
	return [result longLongValue];
}

- (void)setPrimitiveGainAutoMaxValue:(long long)value_ {
	[self setPrimitiveGainAutoMax:[NSNumber numberWithLongLong:value_]];
}





@dynamic whitebalValueBlue;



- (short)whitebalValueBlueValue {
	NSNumber *result = [self whitebalValueBlue];
	return [result shortValue];
}

- (void)setWhitebalValueBlueValue:(short)value_ {
	[self setWhitebalValueBlue:[NSNumber numberWithShort:value_]];
}

- (short)primitiveWhitebalValueBlueValue {
	NSNumber *result = [self primitiveWhitebalValueBlue];
	return [result shortValue];
}

- (void)setPrimitiveWhitebalValueBlueValue:(short)value_ {
	[self setPrimitiveWhitebalValueBlue:[NSNumber numberWithShort:value_]];
}





@dynamic exposureAutoMax;



- (long long)exposureAutoMaxValue {
	NSNumber *result = [self exposureAutoMax];
	return [result longLongValue];
}

- (void)setExposureAutoMaxValue:(long long)value_ {
	[self setExposureAutoMax:[NSNumber numberWithLongLong:value_]];
}

- (long long)primitiveExposureAutoMaxValue {
	NSNumber *result = [self primitiveExposureAutoMax];
	return [result longLongValue];
}

- (void)setPrimitiveExposureAutoMaxValue:(long long)value_ {
	[self setPrimitiveExposureAutoMax:[NSNumber numberWithLongLong:value_]];
}





@dynamic exposureAutoMin;



- (long long)exposureAutoMinValue {
	NSNumber *result = [self exposureAutoMin];
	return [result longLongValue];
}

- (void)setExposureAutoMinValue:(long long)value_ {
	[self setExposureAutoMin:[NSNumber numberWithLongLong:value_]];
}

- (long long)primitiveExposureAutoMinValue {
	NSNumber *result = [self primitiveExposureAutoMin];
	return [result longLongValue];
}

- (void)setPrimitiveExposureAutoMinValue:(long long)value_ {
	[self setPrimitiveExposureAutoMin:[NSNumber numberWithLongLong:value_]];
}





@dynamic gainAutoMin;



- (long long)gainAutoMinValue {
	NSNumber *result = [self gainAutoMin];
	return [result longLongValue];
}

- (void)setGainAutoMinValue:(long long)value_ {
	[self setGainAutoMin:[NSNumber numberWithLongLong:value_]];
}

- (long long)primitiveGainAutoMinValue {
	NSNumber *result = [self primitiveGainAutoMin];
	return [result longLongValue];
}

- (void)setPrimitiveGainAutoMinValue:(long long)value_ {
	[self setPrimitiveGainAutoMin:[NSNumber numberWithLongLong:value_]];
}





@dynamic pixelFormat;



- (short)pixelFormatValue {
	NSNumber *result = [self pixelFormat];
	return [result shortValue];
}

- (void)setPixelFormatValue:(short)value_ {
	[self setPixelFormat:[NSNumber numberWithShort:value_]];
}

- (short)primitivePixelFormatValue {
	NSNumber *result = [self primitivePixelFormat];
	return [result shortValue];
}

- (void)setPrimitivePixelFormatValue:(short)value_ {
	[self setPrimitivePixelFormat:[NSNumber numberWithShort:value_]];
}





@dynamic exposureValue;



- (long long)exposureValueValue {
	NSNumber *result = [self exposureValue];
	return [result longLongValue];
}

- (void)setExposureValueValue:(long long)value_ {
	[self setExposureValue:[NSNumber numberWithLongLong:value_]];
}

- (long long)primitiveExposureValueValue {
	NSNumber *result = [self primitiveExposureValue];
	return [result longLongValue];
}

- (void)setPrimitiveExposureValueValue:(long long)value_ {
	[self setPrimitiveExposureValue:[NSNumber numberWithLongLong:value_]];
}





@dynamic exposureAutoRate;



- (short)exposureAutoRateValue {
	NSNumber *result = [self exposureAutoRate];
	return [result shortValue];
}

- (void)setExposureAutoRateValue:(short)value_ {
	[self setExposureAutoRate:[NSNumber numberWithShort:value_]];
}

- (short)primitiveExposureAutoRateValue {
	NSNumber *result = [self primitiveExposureAutoRate];
	return [result shortValue];
}

- (void)setPrimitiveExposureAutoRateValue:(short)value_ {
	[self setPrimitiveExposureAutoRate:[NSNumber numberWithShort:value_]];
}





@dynamic whitebalAutoAdjustTol;



- (long long)whitebalAutoAdjustTolValue {
	NSNumber *result = [self whitebalAutoAdjustTol];
	return [result longLongValue];
}

- (void)setWhitebalAutoAdjustTolValue:(long long)value_ {
	[self setWhitebalAutoAdjustTol:[NSNumber numberWithLongLong:value_]];
}

- (long long)primitiveWhitebalAutoAdjustTolValue {
	NSNumber *result = [self primitiveWhitebalAutoAdjustTol];
	return [result longLongValue];
}

- (void)setPrimitiveWhitebalAutoAdjustTolValue:(long long)value_ {
	[self setPrimitiveWhitebalAutoAdjustTol:[NSNumber numberWithLongLong:value_]];
}





@dynamic targetForResearchStation;

	

@dynamic cameraAngle;

	

@dynamic imageLDR;

	





@end
