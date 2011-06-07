// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCaptureSession.h instead.

#import <BaseTen/BaseTen.h>
#import "LFFLoggingEntity.h"

@class LFFCameraAngle;
@class LFFCaptureClient;
@class LFFCaptureSessionStats;
@class LFFImageHDR;





@interface LFFCaptureSessionID : NSManagedObjectID {}
@end


@interface _LFFCaptureSession : LFFLoggingEntity {}
+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_;
+ (NSString*)entityName;
+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_;
- (LFFCaptureSessionID*)objectID;



@property (nonatomic, retain) NSDate *startTime;

//- (BOOL)validateStartTime:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSDate *endTime;

//- (BOOL)validateEndTime:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSNumber *recording;

@property BOOL recordingValue;
- (BOOL)recordingValue;
- (void)setRecordingValue:(BOOL)value_;

//- (BOOL)validateRecording:(id*)value_ error:(NSError**)error_;




@property (nonatomic, retain) LFFCameraAngle* cameraAngle;
//- (BOOL)validateCameraAngle:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) LFFCaptureClient* client;
//- (BOOL)validateClient:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) LFFCaptureSessionStats* stats;
//- (BOOL)validateStats:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSSet* images;
- (NSMutableSet*)imagesSet;




@end

@interface _LFFCaptureSession (CoreDataGeneratedAccessors)

- (void)addImages:(NSSet*)value_;
- (void)removeImages:(NSSet*)value_;
- (void)addImagesObject:(LFFImageHDR*)value_;
- (void)removeImagesObject:(LFFImageHDR*)value_;

@end

@interface _LFFCaptureSession (CoreDataGeneratedPrimitiveAccessors)


- (NSDate*)primitiveStartTime;
- (void)setPrimitiveStartTime:(NSDate*)value;




- (NSDate*)primitiveEndTime;
- (void)setPrimitiveEndTime:(NSDate*)value;




- (NSNumber*)primitiveRecording;
- (void)setPrimitiveRecording:(NSNumber*)value;

- (BOOL)primitiveRecordingValue;
- (void)setPrimitiveRecordingValue:(BOOL)value_;





- (LFFCameraAngle*)primitiveCameraAngle;
- (void)setPrimitiveCameraAngle:(LFFCameraAngle*)value;



- (LFFCaptureClient*)primitiveClient;
- (void)setPrimitiveClient:(LFFCaptureClient*)value;



- (LFFCaptureSessionStats*)primitiveStats;
- (void)setPrimitiveStats:(LFFCaptureSessionStats*)value;



- (NSMutableSet*)primitiveImages;
- (void)setPrimitiveImages:(NSMutableSet*)value;



@end
