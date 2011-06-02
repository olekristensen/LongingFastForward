//
//  Longing_Capture_Client_AppDelegate.h
//  Longing Capture Client
//
//  Created by ole kristensen on 23/05/11.
//  Copyright Recoil Performance Group 2011 . All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "LFFCaptureClientController.h";

@class BXDatabaseContext;

@interface Longing_Capture_Client_AppDelegate : NSObject 
{
    NSWindow *window;
	IBOutlet BXDatabaseContext *managedObjectContext;
	IBOutlet BXSynchronizedArrayController *captureClientArrayController;
    LFFCaptureClientController *captureClientController;
	IBOutlet NSWindow *captureClientSelectionSheet;
	
	IBOutlet NSToolbarItem * connectionToolbarItem;
	
	NSURL *databaseUrlFromNib;


}

+ (void)setupDefaults;

@property (nonatomic, retain) IBOutlet NSWindow *window;

@property (nonatomic, retain, readonly) BXDatabaseContext *managedObjectContext;
@property (nonatomic, retain, readonly) LFFCaptureClientController *captureClientController;

- (IBAction)saveAction:sender;
- (IBAction)connectionAction:sender;


#pragma mark Capture Client Selection Sheet

- (IBAction)closeCaptureClientSelectionSheet: (id)sender;
- (void)presentCaptureClientSelectionSheet;
- (NSString*)captureClientSelectionSheetCloseButtonText;
- (BOOL)captureClientSelectionSheetEnabled;


@end
