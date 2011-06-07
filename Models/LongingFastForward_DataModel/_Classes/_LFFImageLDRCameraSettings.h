// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFImageLDRCameraSettings.h instead.

#import <BaseTen/BaseTen.h>
#import "LFFCameraSettings.h"

@class LFFImageLDR;


@interface LFFImageLDRCameraSettingsID : NSManagedObjectID {}
@end


@interface _LFFImageLDRCameraSettings : LFFCameraSettings {}
+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_;
+ (NSString*)entityName;
+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_;
- (LFFImageLDRCameraSettingsID*)objectID;




@property (nonatomic, retain) LFFImageLDR* imageLDR;
//- (BOOL)validateImageLDR:(id*)value_ error:(NSError**)error_;




@end

@interface _LFFImageLDRCameraSettings (CoreDataGeneratedAccessors)

@end

@interface _LFFImageLDRCameraSettings (CoreDataGeneratedPrimitiveAccessors)



- (LFFImageLDR*)primitiveImageLDR;
- (void)setPrimitiveImageLDR:(LFFImageLDR*)value;



@end
