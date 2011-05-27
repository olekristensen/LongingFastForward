// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFImageLDR.m instead.

#import "_LFFImageLDR.h"

@implementation LFFImageLDRID
@end

@implementation _LFFImageLDR

+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription insertNewObjectForEntityForName:@"ImageLDR" inManagedObjectContext:moc_];
}

+ (NSString*)entityName {
	return @"ImageLDR";
}

+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_ {
	NSParameterAssert(moc_);
	return [NSEntityDescription entityForName:@"ImageLDR" inManagedObjectContext:moc_];
}

- (LFFImageLDRID*)objectID {
	return (LFFImageLDRID*)[super objectID];
}

+ (NSSet *)keyPathsForValuesAffectingValueForKey:(NSString *)key {
	NSSet *keyPaths = [super keyPathsForValuesAffectingValueForKey:key];
	

	return keyPaths;
}




@dynamic time;






@dynamic thumbnailURL;






@dynamic histogram;






@dynamic pixels;






@dynamic url;






@dynamic imageHDR;

	

@dynamic cameraSettings;

	





@end
