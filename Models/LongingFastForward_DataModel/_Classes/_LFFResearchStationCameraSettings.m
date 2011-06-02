// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFResearchStationCameraSettings.m instead.

#import "_LFFResearchStationCameraSettings.h"

@implementation LFFResearchStationCameraSettingsID
@end

@implementation _LFFResearchStationCameraSettings

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"ResearchStationCameraSettings" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"ResearchStationCameraSettings";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"ResearchStationCameraSettings" inManagedObjectContext:moc_];
}

- (LFFResearchStationCameraSettingsID*)objectID {
	return (LFFResearchStationCameraSettingsID*)[super objectID];
}

+ (NSSet *)keyPathsForValuesAffectingValueForKey:(NSString *)key {
	NSSet *keyPaths = [super keyPathsForValuesAffectingValueForKey:key];
	

	return keyPaths;
}




@dynamic researchStaion;

	





@end
