// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFLoggingEntity.m instead.

#import "_LFFLoggingEntity.h"

@implementation LFFLoggingEntityID
@end

@implementation _LFFLoggingEntity

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"LFFLoggingEntity" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"LFFLoggingEntity";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"LFFLoggingEntity" inManagedObjectContext:moc_];
}

- (LFFLoggingEntityID*)objectID {
	return (LFFLoggingEntityID*)[super objectID];
}

+ (NSSet *)keyPathsForValuesAffectingValueForKey:(NSString *)key {
	NSSet *keyPaths = [super keyPathsForValuesAffectingValueForKey:key];
	

	return keyPaths;
}




@dynamic eventLog;

	
- (NSMutableSet*)eventLogSet {
	[self willAccessValueForKey:@"eventLog"];
	NSMutableSet *result = [self mutableSetValueForKey:@"eventLog"];
	[self didAccessValueForKey:@"eventLog"];
	return result;
}
	





@end
