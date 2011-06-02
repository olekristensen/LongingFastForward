// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCameraDeviceSettings.m instead.

#import "_LFFCameraDeviceSettings.h"

@implementation LFFCameraDeviceSettingsID
@end

@implementation _LFFCameraDeviceSettings

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"CameraDeviceSettings" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"CameraDeviceSettings";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"CameraDeviceSettings" inManagedObjectContext:moc_];
}

- (LFFCameraDeviceSettingsID*)objectID {
	return (LFFCameraDeviceSettingsID*)[super objectID];
}

+ (NSSet *)keyPathsForValuesAffectingValueForKey:(NSString *)key {
	NSSet *keyPaths = [super keyPathsForValuesAffectingValueForKey:key];
	

	return keyPaths;
}




@dynamic cameraDevice;

	





@end
