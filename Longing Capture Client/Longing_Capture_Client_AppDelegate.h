//
//  Longing_Capture_Client_AppDelegate.h
//  Longing Capture Client
//
//  Created by ole kristensen on 23/05/11.
//  Copyright Recoil Performance Group 2011 . All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class BXDatabaseContext;

@interface Longing_Capture_Client_AppDelegate : NSObject 
{
    NSWindow *window;
	IBOutlet BXDatabaseContext *managedObjectContext;
    
}

@property (nonatomic, retain) IBOutlet NSWindow *window;

@property (nonatomic, retain, readonly) BXDatabaseContext *managedObjectContext;

- (IBAction)saveAction:sender;

@end
