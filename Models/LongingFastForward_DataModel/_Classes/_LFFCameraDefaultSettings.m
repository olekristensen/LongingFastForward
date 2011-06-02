// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCameraDefaultSettings.m instead.

#import "_LFFCameraDefaultSettings.h"

@implementation LFFCameraDefaultSettingsID
@end

@implementation _LFFCameraDefaultSettings

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"CameraDefaultSettings" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"CameraDefaultSettings";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"CameraDefaultSettings" inManagedObjectContext:moc_];
}

- (LFFCameraDefaultSettingsID*)objectID {
	return (LFFCameraDefaultSettingsID*)[super objectID];
}

+ (NSSet *)keyPathsForValuesAffectingValueForKey:(NSString *)key {
	NSSet *keyPaths = [super keyPathsForValuesAffectingValueForKey:key];
	

	return keyPaths;
}




@dynamic name;






@dynamic defaultSettingsForCameraAngles;

	
- (NSMutableSet*)defaultSettingsForCameraAnglesSet {
	[self willAccessValueForKey:@"defaultSettingsForCameraAngles"];
	NSMutableSet *result = [self mutableSetValueForKey:@"defaultSettingsForCameraAngles"];
	[self didAccessValueForKey:@"defaultSettingsForCameraAngles"];
	return result;
}
	





@end
