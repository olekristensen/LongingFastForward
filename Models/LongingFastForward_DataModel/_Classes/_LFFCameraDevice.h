// DO NOT EDIT. This file is machine-generated and constantly overwritten.
// Make changes to LFFCameraDevice.h instead.

#import <BaseTen/BaseTen.h>


@class LFFCameraStats;
@class LFFCameraAngle;













@interface LFFCameraDeviceID : NSManagedObjectID {}
@end

@interface _LFFCameraDevice : BXDatabaseObject {}
+ (id)insertInManagedObjectContext:(NSManagedObjectContext*)moc_;
+ (NSString*)entityName;
+ (NSEntityDescription*)entityInManagedObjectContext:(NSManagedObjectContext*)moc_;
- (LFFCameraDeviceID*)objectID;



@property (nonatomic, retain) NSString *firmwareVersion;

//- (BOOL)validateFirmwareVersion:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSString *partNumber;

//- (BOOL)validatePartNumber:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSString *vendorName;

//- (BOOL)validateVendorName:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSString *hostEthAddress;

//- (BOOL)validateHostEthAddress:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSNumber *uniqueID;

@property long long uniqueIDValue;
- (long long)uniqueIDValue;
- (void)setUniqueIDValue:(long long)value_;

//- (BOOL)validateUniqueID:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSString *name;

//- (BOOL)validateName:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSString *modelName;

//- (BOOL)validateModelName:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSString *serialNumber;

//- (BOOL)validateSerialNumber:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSString *deviceIpAddress;

//- (BOOL)validateDeviceIpAddress:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSString *hostIpAddress;

//- (BOOL)validateHostIpAddress:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) NSString *deviceEthAddress;

//- (BOOL)validateDeviceEthAddress:(id*)value_ error:(NSError**)error_;




@property (nonatomic, retain) LFFCameraStats* stats;
//- (BOOL)validateStats:(id*)value_ error:(NSError**)error_;



@property (nonatomic, retain) LFFCameraAngle* cameraAngle;
//- (BOOL)validateCameraAngle:(id*)value_ error:(NSError**)error_;




@end

@interface _LFFCameraDevice (CoreDataGeneratedAccessors)

@end

@interface _LFFCameraDevice (CoreDataGeneratedPrimitiveAccessors)


- (NSString*)primitiveFirmwareVersion;
- (void)setPrimitiveFirmwareVersion:(NSString*)value;




- (NSString*)primitivePartNumber;
- (void)setPrimitivePartNumber:(NSString*)value;




- (NSString*)primitiveVendorName;
- (void)setPrimitiveVendorName:(NSString*)value;




- (NSString*)primitiveHostEthAddress;
- (void)setPrimitiveHostEthAddress:(NSString*)value;




- (NSNumber*)primitiveUniqueID;
- (void)setPrimitiveUniqueID:(NSNumber*)value;

- (long long)primitiveUniqueIDValue;
- (void)setPrimitiveUniqueIDValue:(long long)value_;




- (NSString*)primitiveName;
- (void)setPrimitiveName:(NSString*)value;




- (NSString*)primitiveModelName;
- (void)setPrimitiveModelName:(NSString*)value;




- (NSString*)primitiveSerialNumber;
- (void)setPrimitiveSerialNumber:(NSString*)value;




- (NSString*)primitiveDeviceIpAddress;
- (void)setPrimitiveDeviceIpAddress:(NSString*)value;




- (NSString*)primitiveHostIpAddress;
- (void)setPrimitiveHostIpAddress:(NSString*)value;




- (NSString*)primitiveDeviceEthAddress;
- (void)setPrimitiveDeviceEthAddress:(NSString*)value;





- (LFFCameraStats*)primitiveStats;
- (void)setPrimitiveStats:(LFFCameraStats*)value;



- (LFFCameraAngle*)primitiveCameraAngle;
- (void)setPrimitiveCameraAngle:(LFFCameraAngle*)value;


@end
