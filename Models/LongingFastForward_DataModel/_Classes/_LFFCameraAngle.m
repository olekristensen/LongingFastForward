// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCameraAngle.m instead.

#import "_LFFCameraAngle.h"

@implementation LFFCameraAngleID
@end

@implementation _LFFCameraAngle

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"CameraAngle" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"CameraAngle";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"CameraAngle" inManagedObjectContext:moc_];
}

- (LFFCameraAngleID*)objectID {
	return (LFFCameraAngleID*)[super objectID];
}

+ (NSSet *)keyPathsForValuesAffectingValueForKey:(NSString *)key {
	NSSet *keyPaths = [super keyPathsForValuesAffectingValueForKey:key];
	

	return keyPaths;
}




@dynamic name;






@dynamic defaultProfile;

	

@dynamic client;

	

@dynamic captureSessions;

	
- (NSMutableSet*)captureSessionsSet {
	[self willAccessValueForKey:@"captureSessions"];
	NSMutableSet *result = [self mutableSetValueForKey:@"captureSessions"];
	[self didAccessValueForKey:@"captureSessions"];
	return result;
}
	

@dynamic device;

	

@dynamic settings;

	



@dynamic currentRecordingSession;



@end
