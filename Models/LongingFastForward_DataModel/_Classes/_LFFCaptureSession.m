// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCaptureSession.m instead.

#import "_LFFCaptureSession.h"

@implementation LFFCaptureSessionID
@end

@implementation _LFFCaptureSession

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"CaptureSession" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"CaptureSession";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"CaptureSession" inManagedObjectContext:moc_];
}

- (LFFCaptureSessionID*)objectID {
	return (LFFCaptureSessionID*)[super objectID];
}

+ (NSSet *)keyPathsForValuesAffectingValueForKey:(NSString *)key {
	NSSet *keyPaths = [super keyPathsForValuesAffectingValueForKey:key];
	
	if ([key isEqualToString:@"recordingValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"recording"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}

	return keyPaths;
}




@dynamic startTime;






@dynamic endTime;






@dynamic recording;



- (BOOL)recordingValue {
	NSNumber *result = [self recording];
	return [result boolValue];
}

- (void)setRecordingValue:(BOOL)value_ {
	[self setRecording:[NSNumber numberWithBool:value_]];
}

- (BOOL)primitiveRecordingValue {
	NSNumber *result = [self primitiveRecording];
	return [result boolValue];
}

- (void)setPrimitiveRecordingValue:(BOOL)value_ {
	[self setPrimitiveRecording:[NSNumber numberWithBool:value_]];
}





@dynamic cameraAngle;

	

@dynamic client;

	

@dynamic stats;

	

@dynamic images;

	
- (NSMutableSet*)imagesSet {
	[self willAccessValueForKey:@"images"];
	NSMutableSet *result = [self mutableSetValueForKey:@"images"];
	[self didAccessValueForKey:@"images"];
	return result;
}
	





@end
