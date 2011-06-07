// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFResearchStation.h instead.

#import <BaseTen/BaseTen.h>
#import "LFFLoggingEntity.h"

@class LFFResearchStationCameraSettings;
@class LFFCaptureClient;



@interface LFFResearchStationID : NSManagedObjectID {}
@end


@interface _LFFResearchStation : LFFLoggingEntity {}
+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_;
+ (NSString*)entityName;
+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_;
- (LFFResearchStationID*)objectID;



@property (nonatomic, retain) NSString *location;

//- (BOOL)validateLocation:(id*)value_ error:(NSError**)error_;




@property (nonatomic, retain) LFFResearchStationCameraSettings* cameraSettingsTarget;
//- (BOOL)validateCameraSettingsTarget:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSSet* captureClients;
- (NSMutableSet*)captureClientsSet;




@end

@interface _LFFResearchStation (CoreDataGeneratedAccessors)

- (void)addCaptureClients:(NSSet*)value_;
- (void)removeCaptureClients:(NSSet*)value_;
- (void)addCaptureClientsObject:(LFFCaptureClient*)value_;
- (void)removeCaptureClientsObject:(LFFCaptureClient*)value_;

@end

@interface _LFFResearchStation (CoreDataGeneratedPrimitiveAccessors)


- (NSString*)primitiveLocation;
- (void)setPrimitiveLocation:(NSString*)value;





- (LFFResearchStationCameraSettings*)primitiveCameraSettingsTarget;
- (void)setPrimitiveCameraSettingsTarget:(LFFResearchStationCameraSettings*)value;



- (NSMutableSet*)primitiveCaptureClients;
- (void)setPrimitiveCaptureClients:(NSMutableSet*)value;



@end
