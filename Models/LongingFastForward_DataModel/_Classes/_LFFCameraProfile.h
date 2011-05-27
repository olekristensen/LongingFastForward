// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCameraProfile.h instead.

#import <BaseTen/BaseTen.h>
#import "LFFCameraSettings.h"

@class LFFCameraAngle;



@interface LFFCameraProfileID : NSManagedObjectID {}
@end

@interface _LFFCameraProfile : LFFCameraSettings {}
+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_;
+ (NSString*)entityName;
+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_;
- (LFFCameraProfileID*)objectID;



@property (nonatomic, retain) NSString *name;

//- (BOOL)validateName:(id*)value_ error:(NSError**)error_;




@property (nonatomic, retain) NSSet* defaultProfileForCameraAngles;
- (NSMutableSet*)defaultProfileForCameraAnglesSet;




@end

@interface _LFFCameraProfile (CoreDataGeneratedAccessors)

- (void)addDefaultProfileForCameraAngles:(NSSet*)value_;
- (void)removeDefaultProfileForCameraAngles:(NSSet*)value_;
- (void)addDefaultProfileForCameraAnglesObject:(LFFCameraAngle*)value_;
- (void)removeDefaultProfileForCameraAnglesObject:(LFFCameraAngle*)value_;

@end

@interface _LFFCameraProfile (CoreDataGeneratedPrimitiveAccessors)


- (NSString*)primitiveName;
- (void)setPrimitiveName:(NSString*)value;





- (NSMutableSet*)primitiveDefaultProfileForCameraAngles;
- (void)setPrimitiveDefaultProfileForCameraAngles:(NSMutableSet*)value;


@end
