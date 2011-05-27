// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFEvent.h instead.

#import <BaseTen/BaseTen.h>


@class LFFLoggingEntity;




@interface LFFEventID : NSManagedObjectID {}
@end

@interface _LFFEvent : BXDatabaseObject {}
+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_;
+ (NSString*)entityName;
+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_;
- (LFFEventID*)objectID;



@property (nonatomic, retain) NSString *message;

//- (BOOL)validateMessage:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSDate *time;

//- (BOOL)validateTime:(id*)value_ error:(NSError**)error_;




@property (nonatomic, retain) LFFLoggingEntity* loggingEntity;
//- (BOOL)validateLoggingEntity:(id*)value_ error:(NSError**)error_;




@end

@interface _LFFEvent (CoreDataGeneratedAccessors)

@end

@interface _LFFEvent (CoreDataGeneratedPrimitiveAccessors)


- (NSString*)primitiveMessage;
- (void)setPrimitiveMessage:(NSString*)value;




- (NSDate*)primitiveTime;
- (void)setPrimitiveTime:(NSDate*)value;





- (LFFLoggingEntity*)primitiveLoggingEntity;
- (void)setPrimitiveLoggingEntity:(LFFLoggingEntity*)value;


@end
