// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFResearchStation.m instead.

#import "_LFFResearchStation.h"

@implementation LFFResearchStationID
@end

@implementation _LFFResearchStation

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"ResearchStation" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"ResearchStation";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"ResearchStation" inManagedObjectContext:moc_];
}

- (LFFResearchStationID*)objectID {
	return (LFFResearchStationID*)[super objectID];
}

+ (NSSet *)keyPathsForValuesAffectingValueForKey:(NSString *)key {
	NSSet *keyPaths = [super keyPathsForValuesAffectingValueForKey:key];
	

	return keyPaths;
}




@dynamic location;






@dynamic cameraSettingsTarget;

	

@dynamic captureClients;

	
- (NSMutableSet*)captureClientsSet {
	[self willAccessValueForKey:@"captureClients"];
	NSMutableSet *result = [self mutableSetValueForKey:@"captureClients"];
	[self didAccessValueForKey:@"captureClients"];
	return result;
}
	





@end
