//
//  LFFController.h
//  LongingFastForward
//
//  Created by ole kristensen on 07/06/11.
//  Copyright 2011 Recoil Performance Group. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <BaseTen/BaseTen.h>


@interface LFFController : NSObject {
	BXDatabaseContext* databaseContext;
}

-(id)initWithContext:databaseContext;

-(int) setupLFFClasses;
-(int) faultLFFClasses;
-(int) faultLFFClassesError: (NSError **) error;

-(int) setupLFFClassesInContext:(BXDatabaseContext*)context;
-(int) faultLFFClassesInContext:(BXDatabaseContext*)context;
-(int) faultLFFClassesInContext:(BXDatabaseContext*)context error: (NSError **) error;


@end
