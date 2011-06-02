// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCameraAngleSettings.h instead.

#import <BaseTen/BaseTen.h>
#import "LFFCameraSettings.h"

@class LFFCameraAngle;


@interface LFFCameraAngleSettingsID : NSManagedObjectID {}
@end

@interface _LFFCameraAngleSettings : LFFCameraSettings {}
+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_;
+ (NSString*)entityName;
+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_;
- (LFFCameraAngleSettingsID*)objectID;




@property (nonatomic, retain) LFFCameraAngle* cameraAngle;
//- (BOOL)validateCameraAngle:(id*)value_ error:(NSError**)error_;




@end

@interface _LFFCameraAngleSettings (CoreDataGeneratedAccessors)

@end

@interface _LFFCameraAngleSettings (CoreDataGeneratedPrimitiveAccessors)



- (LFFCameraAngle*)primitiveCameraAngle;
- (void)setPrimitiveCameraAngle:(LFFCameraAngle*)value;


@end
