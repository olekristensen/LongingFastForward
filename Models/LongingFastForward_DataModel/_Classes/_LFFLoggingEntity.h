// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFLoggingEntity.h instead.

#import <BaseTen/BaseTen.h>


@class LFFEvent;


@interface LFFLoggingEntityID : NSManagedObjectID {}
@end


@interface _LFFLoggingEntity : BXDatabaseObject {}
+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_;
+ (NSString*)entityName;
+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_;
- (LFFLoggingEntityID*)objectID;




@property (nonatomic, retain) NSSet* eventLog;
- (NSMutableSet*)eventLogSet;




@end

@interface _LFFLoggingEntity (CoreDataGeneratedAccessors)

- (void)addEventLog:(NSSet*)value_;
- (void)removeEventLog:(NSSet*)value_;
- (void)addEventLogObject:(LFFEvent*)value_;
- (void)removeEventLogObject:(LFFEvent*)value_;

@end

@interface _LFFLoggingEntity (CoreDataGeneratedPrimitiveAccessors)



- (NSMutableSet*)primitiveEventLog;
- (void)setPrimitiveEventLog:(NSMutableSet*)value;



@end
