// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCameraStats.h instead.

#import <BaseTen/BaseTen.h>


@class LFFCaptureSession;
@class LFFCameraDevice;












@interface LFFCameraStatsID : NSManagedObjectID {}
@end

@interface _LFFCameraStats : BXDatabaseObject {}
+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_;
+ (NSString*)entityName;
+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_;
- (LFFCameraStatsID*)objectID;



@property (nonatomic, retain) NSNumber *packetsReceived;

@property long long packetsReceivedValue;
- (long long)packetsReceivedValue;
- (void)setPacketsReceivedValue:(long long)value_;

//- (BOOL)validatePacketsReceived:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSNumber *driverType;

@property long long driverTypeValue;
- (long long)driverTypeValue;
- (void)setDriverTypeValue:(long long)value_;

//- (BOOL)validateDriverType:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSNumber *framesCompleted;

@property long long framesCompletedValue;
- (long long)framesCompletedValue;
- (void)setFramesCompletedValue:(long long)value_;

//- (BOOL)validateFramesCompleted:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSNumber *frameRate;

@property float frameRateValue;
- (float)frameRateValue;
- (void)setFrameRateValue:(float)value_;

//- (BOOL)validateFrameRate:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSString *filterVersion;

//- (BOOL)validateFilterVersion:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSNumber *packetsResent;

@property long long packetsResentValue;
- (long long)packetsResentValue;
- (void)setPacketsResentValue:(long long)value_;

//- (BOOL)validatePacketsResent:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSNumber *packetsRequested;

@property long long packetsRequestedValue;
- (long long)packetsRequestedValue;
- (void)setPacketsRequestedValue:(long long)value_;

//- (BOOL)validatePacketsRequested:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSNumber *framesDropped;

@property long long framesDroppedValue;
- (long long)framesDroppedValue;
- (void)setFramesDroppedValue:(long long)value_;

//- (BOOL)validateFramesDropped:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSNumber *packetsErroneous;

@property long long packetsErroneousValue;
- (long long)packetsErroneousValue;
- (void)setPacketsErroneousValue:(long long)value_;

//- (BOOL)validatePacketsErroneous:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSNumber *packetsMissed;

@property long long packetsMissedValue;
- (long long)packetsMissedValue;
- (void)setPacketsMissedValue:(long long)value_;

//- (BOOL)validatePacketsMissed:(id*)value_ error:(NSError**)error_;




@property (nonatomic, retain) LFFCaptureSession* captureSession;
//- (BOOL)validateCaptureSession:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) LFFCameraDevice* cameraDevice;
//- (BOOL)validateCameraDevice:(id*)value_ error:(NSError**)error_;




@end

@interface _LFFCameraStats (CoreDataGeneratedAccessors)

@end

@interface _LFFCameraStats (CoreDataGeneratedPrimitiveAccessors)


- (NSNumber*)primitivePacketsReceived;
- (void)setPrimitivePacketsReceived:(NSNumber*)value;

- (long long)primitivePacketsReceivedValue;
- (void)setPrimitivePacketsReceivedValue:(long long)value_;




- (NSNumber*)primitiveDriverType;
- (void)setPrimitiveDriverType:(NSNumber*)value;

- (long long)primitiveDriverTypeValue;
- (void)setPrimitiveDriverTypeValue:(long long)value_;




- (NSNumber*)primitiveFramesCompleted;
- (void)setPrimitiveFramesCompleted:(NSNumber*)value;

- (long long)primitiveFramesCompletedValue;
- (void)setPrimitiveFramesCompletedValue:(long long)value_;




- (NSNumber*)primitiveFrameRate;
- (void)setPrimitiveFrameRate:(NSNumber*)value;

- (float)primitiveFrameRateValue;
- (void)setPrimitiveFrameRateValue:(float)value_;




- (NSString*)primitiveFilterVersion;
- (void)setPrimitiveFilterVersion:(NSString*)value;




- (NSNumber*)primitivePacketsResent;
- (void)setPrimitivePacketsResent:(NSNumber*)value;

- (long long)primitivePacketsResentValue;
- (void)setPrimitivePacketsResentValue:(long long)value_;




- (NSNumber*)primitivePacketsRequested;
- (void)setPrimitivePacketsRequested:(NSNumber*)value;

- (long long)primitivePacketsRequestedValue;
- (void)setPrimitivePacketsRequestedValue:(long long)value_;




- (NSNumber*)primitiveFramesDropped;
- (void)setPrimitiveFramesDropped:(NSNumber*)value;

- (long long)primitiveFramesDroppedValue;
- (void)setPrimitiveFramesDroppedValue:(long long)value_;




- (NSNumber*)primitivePacketsErroneous;
- (void)setPrimitivePacketsErroneous:(NSNumber*)value;

- (long long)primitivePacketsErroneousValue;
- (void)setPrimitivePacketsErroneousValue:(long long)value_;




- (NSNumber*)primitivePacketsMissed;
- (void)setPrimitivePacketsMissed:(NSNumber*)value;

- (long long)primitivePacketsMissedValue;
- (void)setPrimitivePacketsMissedValue:(long long)value_;





- (LFFCaptureSession*)primitiveCaptureSession;
- (void)setPrimitiveCaptureSession:(LFFCaptureSession*)value;



- (LFFCameraDevice*)primitiveCameraDevice;
- (void)setPrimitiveCameraDevice:(LFFCameraDevice*)value;


@end
