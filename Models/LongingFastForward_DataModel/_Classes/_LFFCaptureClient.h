// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCaptureClient.h instead.

#import <BaseTen/BaseTen.h>
#import "LFFLoggingEntity.h"

@class LFFCameraAngle;
@class LFFCaptureSession;
@class LFFResearchStation;





@interface LFFCaptureClientID : NSManagedObjectID {}
@end

@interface _LFFCaptureClient : LFFLoggingEntity {}
+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_;
+ (NSString*)entityName;
+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_;
- (LFFCaptureClientID*)objectID;



@property (nonatomic, retain) NSString *ipAddress;

//- (BOOL)validateIpAddress:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSString *name;

//- (BOOL)validateName:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSString *datastoreURL;

//- (BOOL)validateDatastoreURL:(id*)value_ error:(NSError**)error_;




@property (nonatomic, retain) LFFCameraAngle* cameraAngle;
//- (BOOL)validateCameraAngle:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSSet* captureSessions;
- (NSMutableSet*)captureSessionsSet;



@property (nonatomic, retain) LFFResearchStation* researchStation;
//- (BOOL)validateResearchStation:(id*)value_ error:(NSError**)error_;




@end

@interface _LFFCaptureClient (CoreDataGeneratedAccessors)

- (void)addCaptureSessions:(NSSet*)value_;
- (void)removeCaptureSessions:(NSSet*)value_;
- (void)addCaptureSessionsObject:(LFFCaptureSession*)value_;
- (void)removeCaptureSessionsObject:(LFFCaptureSession*)value_;

@end

@interface _LFFCaptureClient (CoreDataGeneratedPrimitiveAccessors)


- (NSString*)primitiveIpAddress;
- (void)setPrimitiveIpAddress:(NSString*)value;




- (NSString*)primitiveName;
- (void)setPrimitiveName:(NSString*)value;




- (NSString*)primitiveDatastoreURL;
- (void)setPrimitiveDatastoreURL:(NSString*)value;





- (LFFCameraAngle*)primitiveCameraAngle;
- (void)setPrimitiveCameraAngle:(LFFCameraAngle*)value;



- (NSMutableSet*)primitiveCaptureSessions;
- (void)setPrimitiveCaptureSessions:(NSMutableSet*)value;



- (LFFResearchStation*)primitiveResearchStation;
- (void)setPrimitiveResearchStation:(LFFResearchStation*)value;


@end
