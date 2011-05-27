// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCameraStats.m instead.

#import "_LFFCameraStats.h"

@implementation LFFCameraStatsID
@end

@implementation _LFFCameraStats

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"CameraStats" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"CameraStats";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"CameraStats" inManagedObjectContext:moc_];
}

- (LFFCameraStatsID*)objectID {
	return (LFFCameraStatsID*)[super objectID];
}

+ (NSSet *)keyPathsForValuesAffectingValueForKey:(NSString *)key {
	NSSet *keyPaths = [super keyPathsForValuesAffectingValueForKey:key];
	
	if ([key isEqualToString:@"packetsReceivedValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"packetsReceived"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"driverTypeValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"driverType"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"framesCompletedValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"framesCompleted"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"frameRateValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"frameRate"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"packetsResentValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"packetsResent"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"packetsRequestedValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"packetsRequested"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"framesDroppedValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"framesDropped"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"packetsErroneousValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"packetsErroneous"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"packetsMissedValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"packetsMissed"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}

	return keyPaths;
}




@dynamic packetsReceived;



- (long long)packetsReceivedValue {
	NSNumber *result = [self packetsReceived];
	return [result longLongValue];
}

- (void)setPacketsReceivedValue:(long long)value_ {
	[self setPacketsReceived:[NSNumber numberWithLongLong:value_]];
}

- (long long)primitivePacketsReceivedValue {
	NSNumber *result = [self primitivePacketsReceived];
	return [result longLongValue];
}

- (void)setPrimitivePacketsReceivedValue:(long long)value_ {
	[self setPrimitivePacketsReceived:[NSNumber numberWithLongLong:value_]];
}





@dynamic driverType;



- (long long)driverTypeValue {
	NSNumber *result = [self driverType];
	return [result longLongValue];
}

- (void)setDriverTypeValue:(long long)value_ {
	[self setDriverType:[NSNumber numberWithLongLong:value_]];
}

- (long long)primitiveDriverTypeValue {
	NSNumber *result = [self primitiveDriverType];
	return [result longLongValue];
}

- (void)setPrimitiveDriverTypeValue:(long long)value_ {
	[self setPrimitiveDriverType:[NSNumber numberWithLongLong:value_]];
}





@dynamic framesCompleted;



- (long long)framesCompletedValue {
	NSNumber *result = [self framesCompleted];
	return [result longLongValue];
}

- (void)setFramesCompletedValue:(long long)value_ {
	[self setFramesCompleted:[NSNumber numberWithLongLong:value_]];
}

- (long long)primitiveFramesCompletedValue {
	NSNumber *result = [self primitiveFramesCompleted];
	return [result longLongValue];
}

- (void)setPrimitiveFramesCompletedValue:(long long)value_ {
	[self setPrimitiveFramesCompleted:[NSNumber numberWithLongLong:value_]];
}





@dynamic frameRate;



- (float)frameRateValue {
	NSNumber *result = [self frameRate];
	return [result floatValue];
}

- (void)setFrameRateValue:(float)value_ {
	[self setFrameRate:[NSNumber numberWithFloat:value_]];
}

- (float)primitiveFrameRateValue {
	NSNumber *result = [self primitiveFrameRate];
	return [result floatValue];
}

- (void)setPrimitiveFrameRateValue:(float)value_ {
	[self setPrimitiveFrameRate:[NSNumber numberWithFloat:value_]];
}





@dynamic filterVersion;






@dynamic packetsResent;



- (long long)packetsResentValue {
	NSNumber *result = [self packetsResent];
	return [result longLongValue];
}

- (void)setPacketsResentValue:(long long)value_ {
	[self setPacketsResent:[NSNumber numberWithLongLong:value_]];
}

- (long long)primitivePacketsResentValue {
	NSNumber *result = [self primitivePacketsResent];
	return [result longLongValue];
}

- (void)setPrimitivePacketsResentValue:(long long)value_ {
	[self setPrimitivePacketsResent:[NSNumber numberWithLongLong:value_]];
}





@dynamic packetsRequested;



- (long long)packetsRequestedValue {
	NSNumber *result = [self packetsRequested];
	return [result longLongValue];
}

- (void)setPacketsRequestedValue:(long long)value_ {
	[self setPacketsRequested:[NSNumber numberWithLongLong:value_]];
}

- (long long)primitivePacketsRequestedValue {
	NSNumber *result = [self primitivePacketsRequested];
	return [result longLongValue];
}

- (void)setPrimitivePacketsRequestedValue:(long long)value_ {
	[self setPrimitivePacketsRequested:[NSNumber numberWithLongLong:value_]];
}





@dynamic framesDropped;



- (long long)framesDroppedValue {
	NSNumber *result = [self framesDropped];
	return [result longLongValue];
}

- (void)setFramesDroppedValue:(long long)value_ {
	[self setFramesDropped:[NSNumber numberWithLongLong:value_]];
}

- (long long)primitiveFramesDroppedValue {
	NSNumber *result = [self primitiveFramesDropped];
	return [result longLongValue];
}

- (void)setPrimitiveFramesDroppedValue:(long long)value_ {
	[self setPrimitiveFramesDropped:[NSNumber numberWithLongLong:value_]];
}





@dynamic packetsErroneous;



- (long long)packetsErroneousValue {
	NSNumber *result = [self packetsErroneous];
	return [result longLongValue];
}

- (void)setPacketsErroneousValue:(long long)value_ {
	[self setPacketsErroneous:[NSNumber numberWithLongLong:value_]];
}

- (long long)primitivePacketsErroneousValue {
	NSNumber *result = [self primitivePacketsErroneous];
	return [result longLongValue];
}

- (void)setPrimitivePacketsErroneousValue:(long long)value_ {
	[self setPrimitivePacketsErroneous:[NSNumber numberWithLongLong:value_]];
}





@dynamic packetsMissed;



- (long long)packetsMissedValue {
	NSNumber *result = [self packetsMissed];
	return [result longLongValue];
}

- (void)setPacketsMissedValue:(long long)value_ {
	[self setPacketsMissed:[NSNumber numberWithLongLong:value_]];
}

- (long long)primitivePacketsMissedValue {
	NSNumber *result = [self primitivePacketsMissed];
	return [result longLongValue];
}

- (void)setPrimitivePacketsMissedValue:(long long)value_ {
	[self setPrimitivePacketsMissed:[NSNumber numberWithLongLong:value_]];
}





@dynamic captureSession;

	

@dynamic cameraDevice;

	





@end
