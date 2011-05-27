// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCaptureClient.m instead.

#import "_LFFCaptureClient.h"

@implementation LFFCaptureClientID
@end

@implementation _LFFCaptureClient

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"CaptureClient" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"CaptureClient";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"CaptureClient" inManagedObjectContext:moc_];
}

- (LFFCaptureClientID*)objectID {
	return (LFFCaptureClientID*)[super objectID];
}

+ (NSSet *)keyPathsForValuesAffectingValueForKey:(NSString *)key {
	NSSet *keyPaths = [super keyPathsForValuesAffectingValueForKey:key];
	

	return keyPaths;
}




@dynamic ipAddress;






@dynamic name;






@dynamic datastoreURL;






@dynamic cameraAngle;

	

@dynamic captureSessions;

	
- (NSMutableSet*)captureSessionsSet {
	[self willAccessValueForKey:@"captureSessions"];
	NSMutableSet *result = [self mutableSetValueForKey:@"captureSessions"];
	[self didAccessValueForKey:@"captureSessions"];
	return result;
}
	

@dynamic researchStation;

	





@end
