// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCaptureSessionStats.m instead.

#import "_LFFCaptureSessionStats.h"

@implementation LFFCaptureSessionStatsID
@end

@implementation _LFFCaptureSessionStats

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"CaptureSessionStats" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"CaptureSessionStats";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"CaptureSessionStats" inManagedObjectContext:moc_];
}

- (LFFCaptureSessionStatsID*)objectID {
	return (LFFCaptureSessionStatsID*)[super objectID];
}

+ (NSSet *)keyPathsForValuesAffectingValueForKey:(NSString *)key {
	NSSet *keyPaths = [super keyPathsForValuesAffectingValueForKey:key];
	

	return keyPaths;
}




@dynamic captureSession;

	






@end
