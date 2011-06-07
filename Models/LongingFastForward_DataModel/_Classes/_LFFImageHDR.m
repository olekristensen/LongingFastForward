// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFImageHDR.m instead.

#import "_LFFImageHDR.h"

@implementation LFFImageHDRID
@end

@implementation _LFFImageHDR

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"ImageHDR" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"ImageHDR";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"ImageHDR" inManagedObjectContext:moc_];
}

- (LFFImageHDRID*)objectID {
	return (LFFImageHDRID*)[super objectID];
}

+ (NSSet *)keyPathsForValuesAffectingValueForKey:(NSString *)key {
	NSSet *keyPaths = [super keyPathsForValuesAffectingValueForKey:key];
	
	if ([key isEqualToString:@"isIntervalSnapshotValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"isIntervalSnapshot"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"differenceToFormerImageHDRValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"differenceToFormerImageHDR"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}
	if ([key isEqualToString:@"isIntervalAverageValue"]) {
		NSSet *affectingKey = [NSSet setWithObject:@"isIntervalAverage"];
		keyPaths = [keyPaths setByAddingObjectsFromSet:affectingKey];
	}

	return keyPaths;
}




@dynamic isIntervalSnapshot;



- (BOOL)isIntervalSnapshotValue {
	NSNumber *result = [self isIntervalSnapshot];
	return [result boolValue];
}

- (void)setIsIntervalSnapshotValue:(BOOL)value_ {
	[self setIsIntervalSnapshot:[NSNumber numberWithBool:value_]];
}

- (BOOL)primitiveIsIntervalSnapshotValue {
	NSNumber *result = [self primitiveIsIntervalSnapshot];
	return [result boolValue];
}

- (void)setPrimitiveIsIntervalSnapshotValue:(BOOL)value_ {
	[self setPrimitiveIsIntervalSnapshot:[NSNumber numberWithBool:value_]];
}





@dynamic thumbnailURL;






@dynamic histogram;






@dynamic integrationDuration;






@dynamic differenceToFormerImageHDR;



- (double)differenceToFormerImageHDRValue {
	NSNumber *result = [self differenceToFormerImageHDR];
	return [result doubleValue];
}

- (void)setDifferenceToFormerImageHDRValue:(double)value_ {
	[self setDifferenceToFormerImageHDR:[NSNumber numberWithDouble:value_]];
}

- (double)primitiveDifferenceToFormerImageHDRValue {
	NSNumber *result = [self primitiveDifferenceToFormerImageHDR];
	return [result doubleValue];
}

- (void)setPrimitiveDifferenceToFormerImageHDRValue:(double)value_ {
	[self setPrimitiveDifferenceToFormerImageHDR:[NSNumber numberWithDouble:value_]];
}





@dynamic time;






@dynamic isIntervalAverage;



- (BOOL)isIntervalAverageValue {
	NSNumber *result = [self isIntervalAverage];
	return [result boolValue];
}

- (void)setIsIntervalAverageValue:(BOOL)value_ {
	[self setIsIntervalAverage:[NSNumber numberWithBool:value_]];
}

- (BOOL)primitiveIsIntervalAverageValue {
	NSNumber *result = [self primitiveIsIntervalAverage];
	return [result boolValue];
}

- (void)setPrimitiveIsIntervalAverageValue:(BOOL)value_ {
	[self setPrimitiveIsIntervalAverage:[NSNumber numberWithBool:value_]];
}





@dynamic url;






@dynamic nextImageHDR;

	

@dynamic formerImageHDR;

	

@dynamic captureSession;

	

@dynamic imagesLDR;

	
- (NSMutableSet*)imagesLDRSet {
	[self willAccessValueForKey:@"imagesLDR"];
	NSMutableSet *result = [self mutableSetValueForKey:@"imagesLDR"];
	[self didAccessValueForKey:@"imagesLDR"];
	return result;
}
	






@end
