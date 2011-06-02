// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFImageLDRCameraSettings.m instead.

#import "_LFFImageLDRCameraSettings.h"

@implementation LFFImageLDRCameraSettingsID
@end

@implementation _LFFImageLDRCameraSettings

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"ImageLDRCameraSettings" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"ImageLDRCameraSettings";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"ImageLDRCameraSettings" inManagedObjectContext:moc_];
}

- (LFFImageLDRCameraSettingsID*)objectID {
	return (LFFImageLDRCameraSettingsID*)[super objectID];
}

+ (NSSet *)keyPathsForValuesAffectingValueForKey:(NSString *)key {
	NSSet *keyPaths = [super keyPathsForValuesAffectingValueForKey:key];
	

	return keyPaths;
}




@dynamic imageLDR;

	





@end
