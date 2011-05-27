// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFImageHDR.h instead.

#import <BaseTen/BaseTen.h>
#import "LFFLoggingEntity.h"

@class LFFImageHDR;
@class LFFImageHDR;
@class LFFCaptureSession;
@class LFFImageLDR;










@interface LFFImageHDRID : NSManagedObjectID {}
@end

@interface _LFFImageHDR : LFFLoggingEntity {}
+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_;
+ (NSString*)entityName;
+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_;
- (LFFImageHDRID*)objectID;



@property (nonatomic, retain) NSNumber *isIntervalSnapshot;

@property BOOL isIntervalSnapshotValue;
- (BOOL)isIntervalSnapshotValue;
- (void)setIsIntervalSnapshotValue:(BOOL)value_;

//- (BOOL)validateIsIntervalSnapshot:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSString *thumbnailURL;

//- (BOOL)validateThumbnailURL:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSData *histogram;

//- (BOOL)validateHistogram:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSDate *integrationDuration;

//- (BOOL)validateIntegrationDuration:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSNumber *differenceToFormerImageHDR;

@property double differenceToFormerImageHDRValue;
- (double)differenceToFormerImageHDRValue;
- (void)setDifferenceToFormerImageHDRValue:(double)value_;

//- (BOOL)validateDifferenceToFormerImageHDR:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSDate *time;

//- (BOOL)validateTime:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSNumber *isIntervalAverage;

@property BOOL isIntervalAverageValue;
- (BOOL)isIntervalAverageValue;
- (void)setIsIntervalAverageValue:(BOOL)value_;

//- (BOOL)validateIsIntervalAverage:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSString *url;

//- (BOOL)validateUrl:(id*)value_ error:(NSError**)error_;




@property (nonatomic, retain) LFFImageHDR* nextImageHDR;
//- (BOOL)validateNextImageHDR:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) LFFImageHDR* formerImageHDR;
//- (BOOL)validateFormerImageHDR:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) LFFCaptureSession* captureSession;
//- (BOOL)validateCaptureSession:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSSet* imagesLDR;
- (NSMutableSet*)imagesLDRSet;




@end

@interface _LFFImageHDR (CoreDataGeneratedAccessors)

- (void)addImagesLDR:(NSSet*)value_;
- (void)removeImagesLDR:(NSSet*)value_;
- (void)addImagesLDRObject:(LFFImageLDR*)value_;
- (void)removeImagesLDRObject:(LFFImageLDR*)value_;

@end

@interface _LFFImageHDR (CoreDataGeneratedPrimitiveAccessors)


- (NSNumber*)primitiveIsIntervalSnapshot;
- (void)setPrimitiveIsIntervalSnapshot:(NSNumber*)value;

- (BOOL)primitiveIsIntervalSnapshotValue;
- (void)setPrimitiveIsIntervalSnapshotValue:(BOOL)value_;




- (NSString*)primitiveThumbnailURL;
- (void)setPrimitiveThumbnailURL:(NSString*)value;




- (NSData*)primitiveHistogram;
- (void)setPrimitiveHistogram:(NSData*)value;




- (NSDate*)primitiveIntegrationDuration;
- (void)setPrimitiveIntegrationDuration:(NSDate*)value;




- (NSNumber*)primitiveDifferenceToFormerImageHDR;
- (void)setPrimitiveDifferenceToFormerImageHDR:(NSNumber*)value;

- (double)primitiveDifferenceToFormerImageHDRValue;
- (void)setPrimitiveDifferenceToFormerImageHDRValue:(double)value_;




- (NSDate*)primitiveTime;
- (void)setPrimitiveTime:(NSDate*)value;




- (NSNumber*)primitiveIsIntervalAverage;
- (void)setPrimitiveIsIntervalAverage:(NSNumber*)value;

- (BOOL)primitiveIsIntervalAverageValue;
- (void)setPrimitiveIsIntervalAverageValue:(BOOL)value_;




- (NSString*)primitiveUrl;
- (void)setPrimitiveUrl:(NSString*)value;





- (LFFImageHDR*)primitiveNextImageHDR;
- (void)setPrimitiveNextImageHDR:(LFFImageHDR*)value;



- (LFFImageHDR*)primitiveFormerImageHDR;
- (void)setPrimitiveFormerImageHDR:(LFFImageHDR*)value;



- (LFFCaptureSession*)primitiveCaptureSession;
- (void)setPrimitiveCaptureSession:(LFFCaptureSession*)value;



- (NSMutableSet*)primitiveImagesLDR;
- (void)setPrimitiveImagesLDR:(NSMutableSet*)value;


@end
