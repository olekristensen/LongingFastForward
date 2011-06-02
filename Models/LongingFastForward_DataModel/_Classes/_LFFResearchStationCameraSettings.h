// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFResearchStationCameraSettings.h instead.

#import <BaseTen/BaseTen.h>
#import "LFFCameraSettings.h"

@class LFFResearchStation;


@interface LFFResearchStationCameraSettingsID : NSManagedObjectID {}
@end

@interface _LFFResearchStationCameraSettings : LFFCameraSettings {}
+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_;
+ (NSString*)entityName;
+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_;
- (LFFResearchStationCameraSettingsID*)objectID;




@property (nonatomic, retain) LFFResearchStation* researchStaion;
//- (BOOL)validateResearchStaion:(id*)value_ error:(NSError**)error_;




@end

@interface _LFFResearchStationCameraSettings (CoreDataGeneratedAccessors)

@end

@interface _LFFResearchStationCameraSettings (CoreDataGeneratedPrimitiveAccessors)



- (LFFResearchStation*)primitiveResearchStaion;
- (void)setPrimitiveResearchStaion:(LFFResearchStation*)value;


@end
