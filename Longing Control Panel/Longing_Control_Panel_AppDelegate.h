//
//  Longing_Control_Panel_AppDelegate.h
//  Longing Control Panel
//
//  Created by ole kristensen on 23/05/11.
//  Copyright Recoil Performance Group 2011 . All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <BaseTenAppKit/BaseTenAppKit.h>
#import <LongingFastForward/LongingFastForward.h>


@class BXDatabaseContext;
@class BXDatabaseObject;

@interface Longing_Control_Panel_AppDelegate : NSObject 
{
    NSWindow *window;
	IBOutlet NSView * objectView;
    IBOutlet NSWindow *populateDatabaseSheet;
	IBOutlet NSButton *populateDatabaseSheetDeleteFirst;
	IBOutlet BXDatabaseContext *managedObjectContext;
	
	IBOutlet NSArrayController *researchStationController;
	LFFResearchStation *researchStation;
	
	IBOutlet NSToolbarItem * connectionToolbarItem;
    
	NSURL *databaseUrlFromNib;
	
}

@property (nonatomic, retain) IBOutlet NSWindow *window;
@property (nonatomic, retain, readonly) BXDatabaseContext *managedObjectContext;
@property (nonatomic, retain, readonly) LFFResearchStation *researchStation;

- (IBAction)connectionAction:sender;

- populateDatabaseClearExisting:(BOOL) clearExisting;
- (IBAction)closePopulateDatabaseSheet: (id)sender;
- (void)presentPopulateDatabaseSheet;
- (NSString*)populateDatabaseSheetCloseButtonText;
- (void)didEndPopulateDatabaseSheet:(NSWindow *)sheet returnCode:(NSInteger)returnCode contextInfo:(void *)contextInfo;



@end
