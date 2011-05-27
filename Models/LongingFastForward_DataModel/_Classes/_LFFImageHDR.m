// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFImageHDR.m instead.

#import "_LFFImageHDR.h"

@implementation LFFImageHDRID
@end

@implementation _LFFImageHDR

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

- (LFFImageHDRID*)objectID {
	return (LFFImageHDRID*)[super objectID];
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
