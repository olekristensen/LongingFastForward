// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCameraDeviceStats.h instead.

#import <BaseTen/BaseTen.h>
#import "LFFCameraStats.h"

@class LFFCameraDevice;


@interface LFFCameraDeviceStatsID : NSManagedObjectID {}
@end


@interface _LFFCameraDeviceStats : LFFCameraStats {}
+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_;
+ (NSString*)entityName;
+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_;
- (LFFCameraDeviceStatsID*)objectID;




@property (nonatomic, retain) LFFCameraDevice* cameraDevice;
//- (BOOL)validateCameraDevice:(id*)value_ error:(NSError**)error_;




@end

@interface _LFFCameraDeviceStats (CoreDataGeneratedAccessors)

@end

@interface _LFFCameraDeviceStats (CoreDataGeneratedPrimitiveAccessors)



- (LFFCameraDevice*)primitiveCameraDevice;
- (void)setPrimitiveCameraDevice:(LFFCameraDevice*)value;



@end
