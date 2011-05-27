//
//  Longing_Control_Panel_AppDelegate.h
//  Longing Control Panel
//
//  Created by ole kristensen on 23/05/11.
//  Copyright Recoil Performance Group 2011 . All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <BaseTenAppKit/BaseTenAppKit.h>
#import <LongingFastForward/LFFResearchStation.h>


@class BXDatabaseContext;
@class BXDatabaseObject;

@interface Longing_Control_Panel_AppDelegate : NSObject 
{
    NSWindow *window;
	IBOutlet BXDatabaseContext *managedObjectContext;
	LFFResearchStation *researchStation;
	IBOutlet BXSynchronizedArrayController *researchStationController;
	
	IBOutlet NSToolbarItem * connectionToolbarItem;
    
}

@property (nonatomic, retain) IBOutlet NSWindow *window;
@property (nonatomic, retain, readonly) BXDatabaseContext *managedObjectContext;
@property (nonatomic, retain, readonly) LFFResearchStation *researchStation;

- (IBAction)saveAction:sender;
- (IBAction)connectionAction:sender;

@end
