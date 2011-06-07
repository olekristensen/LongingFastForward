// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCameraDeviceStats.m instead.

#import "_LFFCameraDeviceStats.h"

@implementation LFFCameraDeviceStatsID
@end

@implementation _LFFCameraDeviceStats

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"CameraDeviceStats" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"CameraDeviceStats";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"CameraDeviceStats" inManagedObjectContext:moc_];
}

- (LFFCameraDeviceStatsID*)objectID {
	return (LFFCameraDeviceStatsID*)[super objectID];
}

+ (NSSet *)keyPathsForValuesAffectingValueForKey:(NSString *)key {
	NSSet *keyPaths = [super keyPathsForValuesAffectingValueForKey:key];
	

	return keyPaths;
}




@dynamic cameraDevice;

	






@end
