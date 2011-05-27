// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCameraProfile.m instead.

#import "_LFFCameraProfile.h"

@implementation LFFCameraProfileID
@end

@implementation _LFFCameraProfile

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"CameraProfile" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"CameraProfile";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"CameraProfile" inManagedObjectContext:moc_];
}

- (LFFCameraProfileID*)objectID {
	return (LFFCameraProfileID*)[super objectID];
}

+ (NSSet *)keyPathsForValuesAffectingValueForKey:(NSString *)key {
	NSSet *keyPaths = [super keyPathsForValuesAffectingValueForKey:key];
	

	return keyPaths;
}




@dynamic name;






@dynamic defaultProfileForCameraAngles;

	
- (NSMutableSet*)defaultProfileForCameraAnglesSet {
	[self willAccessValueForKey:@"defaultProfileForCameraAngles"];
	NSMutableSet *result = [self mutableSetValueForKey:@"defaultProfileForCameraAngles"];
	[self didAccessValueForKey:@"defaultProfileForCameraAngles"];
	return result;
}
	





@end
