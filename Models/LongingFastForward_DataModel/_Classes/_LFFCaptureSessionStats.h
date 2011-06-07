// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCaptureSessionStats.h instead.

#import <BaseTen/BaseTen.h>
#import "LFFCameraStats.h"

@class LFFCaptureSession;


@interface LFFCaptureSessionStatsID : NSManagedObjectID {}
@end


@interface _LFFCaptureSessionStats : LFFCameraStats {}
+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_;
+ (NSString*)entityName;
+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_;
- (LFFCaptureSessionStatsID*)objectID;




@property (nonatomic, retain) LFFCaptureSession* captureSession;
//- (BOOL)validateCaptureSession:(id*)value_ error:(NSError**)error_;




@end

@interface _LFFCaptureSessionStats (CoreDataGeneratedAccessors)

@end

@interface _LFFCaptureSessionStats (CoreDataGeneratedPrimitiveAccessors)



- (LFFCaptureSession*)primitiveCaptureSession;
- (void)setPrimitiveCaptureSession:(LFFCaptureSession*)value;



@end
