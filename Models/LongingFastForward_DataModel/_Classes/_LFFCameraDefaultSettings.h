// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCameraDefaultSettings.h instead.

#import <BaseTen/BaseTen.h>
#import "LFFCameraSettings.h"

@class LFFCameraAngle;



@interface LFFCameraDefaultSettingsID : NSManagedObjectID {}
@end


@interface _LFFCameraDefaultSettings : LFFCameraSettings {}
+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_;
+ (NSString*)entityName;
+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_;
- (LFFCameraDefaultSettingsID*)objectID;



@property (nonatomic, retain) NSString *name;

//- (BOOL)validateName:(id*)value_ error:(NSError**)error_;




@property (nonatomic, retain) NSSet* defaultSettingsForCameraAngles;
- (NSMutableSet*)defaultSettingsForCameraAnglesSet;




@end

@interface _LFFCameraDefaultSettings (CoreDataGeneratedAccessors)

- (void)addDefaultSettingsForCameraAngles:(NSSet*)value_;
- (void)removeDefaultSettingsForCameraAngles:(NSSet*)value_;
- (void)addDefaultSettingsForCameraAnglesObject:(LFFCameraAngle*)value_;
- (void)removeDefaultSettingsForCameraAnglesObject:(LFFCameraAngle*)value_;

@end

@interface _LFFCameraDefaultSettings (CoreDataGeneratedPrimitiveAccessors)


- (NSString*)primitiveName;
- (void)setPrimitiveName:(NSString*)value;





- (NSMutableSet*)primitiveDefaultSettingsForCameraAngles;
- (void)setPrimitiveDefaultSettingsForCameraAngles:(NSMutableSet*)value;



@end
