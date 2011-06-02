// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCameraDeviceSettings.h instead.

#import <BaseTen/BaseTen.h>
#import "LFFCameraSettings.h"

@class LFFCameraDevice;


@interface LFFCameraDeviceSettingsID : NSManagedObjectID {}
@end

@interface _LFFCameraDeviceSettings : LFFCameraSettings {}
+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_;
+ (NSString*)entityName;
+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_;
- (LFFCameraDeviceSettingsID*)objectID;




@property (nonatomic, retain) LFFCameraDevice* cameraDevice;
//- (BOOL)validateCameraDevice:(id*)value_ error:(NSError**)error_;




@end

@interface _LFFCameraDeviceSettings (CoreDataGeneratedAccessors)

@end

@interface _LFFCameraDeviceSettings (CoreDataGeneratedPrimitiveAccessors)



- (LFFCameraDevice*)primitiveCameraDevice;
- (void)setPrimitiveCameraDevice:(LFFCameraDevice*)value;


@end
