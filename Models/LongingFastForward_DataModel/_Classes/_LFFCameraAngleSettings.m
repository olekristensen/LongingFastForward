// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCameraAngleSettings.m instead.

#import "_LFFCameraAngleSettings.h"

@implementation LFFCameraAngleSettingsID
@end

@implementation _LFFCameraAngleSettings

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"CameraAngleSettings" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"CameraAngleSettings";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"CameraAngleSettings" inManagedObjectContext:moc_];
}

- (LFFCameraAngleSettingsID*)objectID {
	return (LFFCameraAngleSettingsID*)[super objectID];
}

+ (NSSet *)keyPathsForValuesAffectingValueForKey:(NSString *)key {
	NSSet *keyPaths = [super keyPathsForValuesAffectingValueForKey:key];
	

	return keyPaths;
}




@dynamic cameraAngle;

	






@end
