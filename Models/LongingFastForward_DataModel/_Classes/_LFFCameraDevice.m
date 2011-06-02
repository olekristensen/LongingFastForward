// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCameraDevice.m instead.

#import "_LFFCameraDevice.h"

@implementation LFFCameraDeviceID
@end

@implementation _LFFCameraDevice

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"CameraDevice" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"CameraDevice";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"CameraDevice" inManagedObjectContext:moc_];
}

- (LFFCameraDeviceID*)objectID {
	return (LFFCameraDeviceID*)[super objectID];
}

+ (NSSet *)keyPathsForValuesAffectingValueForKey:(NSString *)key {
	NSSet *keyPaths = [super keyPathsForValuesAffectingValueForKey:key];
	
	if ([key isEqualToString:@"uniqueIDValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"uniqueID"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}

	return keyPaths;
}




@dynamic firmwareVersion;






@dynamic partNumber;






@dynamic vendorName;






@dynamic hostEthAddress;






@dynamic uniqueID;



- (long long)uniqueIDValue {
	NSNumber *result = [self uniqueID];
	return [result longLongValue];
}

- (void)setUniqueIDValue:(long long)value_ {
	[self setUniqueID:[NSNumber numberWithLongLong:value_]];
}

- (long long)primitiveUniqueIDValue {
	NSNumber *result = [self primitiveUniqueID];
	return [result longLongValue];
}

- (void)setPrimitiveUniqueIDValue:(long long)value_ {
	[self setPrimitiveUniqueID:[NSNumber numberWithLongLong:value_]];
}





@dynamic name;






@dynamic modelName;






@dynamic serialNumber;






@dynamic deviceIpAddress;






@dynamic hostIpAddress;






@dynamic deviceEthAddress;






@dynamic settings;

	

@dynamic cameraAngle;

	

@dynamic stats;

	





@end
