//
//  Longing_Control_Panel_AppDelegate.m
//  Longing Control Panel
//
//  Created by ole kristensen on 23/05/11.
//  Copyright Recoil Performance Group 2011 . All rights reserved.
//

#import "Longing_Control_Panel_AppDelegate.h"
#import <BaseTen/BaseTen.h>
#import <BaseTen/BXLogger.h>

@implementation Longing_Control_Panel_AppDelegate

@synthesize window, managedObjectContext, researchStation;

- (void) awakeFromNib
{
	BXLogSetLevel(kBXLogLevelDebug);

	NSAssert (nil != managedObjectContext, @"Longing_Control_Panel_AppDelegate's outlets were not set up correctly.");
	[managedObjectContext setLogsQueries:YES];
	
	[self updateConnectionToolbarItem];
}

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
	return YES;
}

- (void) databaseContextConnectionSucceeded: (BXDatabaseContext *) ctx
{	
	NSLog(@"databaseContextConnectionSucceeded");
	researchStation = [self researchStation];
	[self updateConnectionToolbarItem];

}

- (void) databaseContext: (BXDatabaseContext *) context lostConnection: (NSError *) error
{
	NSLog(@"lostConnection");
	//FIXME: do something about this; not just logging.
	if ([NSApp presentError: error])
		BXLogInfo (@"Reconnected.");
	else
	{
		BXLogInfo (@"Failed to reconnect.");
		[context setAllowReconnecting: NO];
	}
	[self updateConnectionToolbarItem];
}



- (NSObjectController *) researchStation {
	
	if (researchStation) return researchStation;
	
	[researchStationController fetch:nil];
	
	if ([[researchStationController arrangedObjects] count] == 0) {
		
		NSLog(@"count of objects %i",[[researchStationController arrangedObjects] count]);
		
		BXEntityDescription* entity;
		NSDictionary* values;
		
		entity= [[managedObjectContext databaseObjectModel] entityForTable: @"CameraSettings"];
		values = [[NSDictionary alloc]init];
		NSManagedObject *newCameraSettings = [managedObjectContext createObjectForEntity: entity withFieldValues: values error: NULL];
		
		
		entity = [[managedObjectContext databaseObjectModel] entityForTable: @"ResearchStation"];
		values = [NSDictionary dictionaryWithObject: newCameraSettings forKey: @"cameraSettingsTarget"];
		researchStation = [managedObjectContext createObjectForEntity: entity withFieldValues: values error: NULL];
		
		return researchStation;
		
	}
	
	// clear superflouus research station entries
	
	if ([[researchStationController arrangedObjects] count] > 0) {
		
		[researchStationController removeObjectsAtArrangedObjectIndexes:[NSIndexSet indexSetWithIndexesInRange:NSMakeRange(1, [[researchStationController arrangedObjects] count]-1)]];
		researchStation = [[researchStationController arrangedObjects] objectAtIndex:0];
	}
	
	[researchStation faultKey:@"location"];
	
	return researchStation;
}


/**
 Returns the support directory for the application, used to store the Core Data
 store file.  This code uses a directory named "Longing_Control_Panel" for
 the content, either in the NSApplicationSupportDirectory location or (if the
 former cannot be found), the system's temporary directory.
 */

- (NSString *)applicationSupportDirectory {
	
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
    NSString *basePath = ([paths count] > 0) ? [paths objectAtIndex:0] : NSTemporaryDirectory();
    return [basePath stringByAppendingPathComponent:@"Longing_Control_Panel"];
}

/**
 Returns the NSUndoManager for the application.  In this case, the manager
 returned is that of the managed object context for the application.
 */

- (NSUndoManager *)windowWillReturnUndoManager:(NSWindow *)window {
    return [[self managedObjectContext] undoManager];
}


/**
 Performs the save action for the application, which is to send the save:
 message to the application's managed object context.  Any encountered errors
 are presented to the user.
 */

- (IBAction) saveAction:(id)sender {
	
    NSError *error = nil;
    
    if (![[self managedObjectContext] commitEditing]) {
        NSLog(@"%@:%s unable to commit editing before saving", [self class], _cmd);
    }
	
    if (![[self managedObjectContext] save:&error]) {
        [[NSApplication sharedApplication] presentError:error];
    }
}


/**
 Implementation of the applicationShouldTerminate: method, used here to
 handle the saving of changes in the application managed object context
 before the application terminates.
 */

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *)sender {
	
    if (!managedObjectContext) return NSTerminateNow;
	
	if (![managedObjectContext isConnected] ) return NSTerminateNow;
	
    NSError *error = nil;
    if (![managedObjectContext save:&error]) {
		
        // This error handling simply presents error information in a panel with an 
        // "Ok" button, which does not include any attempt at error recovery (meaning, 
        // attempting to fix the error.)  As a result, this implementation will 
        // present the information to the user and then follow up with a panel asking 
        // if the user wishes to "Quit Anyway", without saving the changes.
		
        // Typically, this process should be altered to include application-specific 
        // recovery steps.  
		
        BOOL result = [sender presentError:error];
        if (result) return NSTerminateCancel;
		
        NSString *question = NSLocalizedString(@"Could not save changes while quitting.  Quit anyway?", @"Quit without saves error question message");
        NSString *info = NSLocalizedString(@"Quitting now will lose any changes you have made since the last successful save", @"Quit without saves error question info");
        NSString *quitButton = NSLocalizedString(@"Quit anyway", @"Quit anyway button title");
        NSString *cancelButton = NSLocalizedString(@"Cancel", @"Cancel button title");
        NSAlert *alert = [[NSAlert alloc] init];
        [alert setMessageText:question];
        [alert setInformativeText:info];
        [alert addButtonWithTitle:quitButton];
        [alert addButtonWithTitle:cancelButton];
		
        NSInteger answer = [alert runModal];
        [alert release];
        alert = nil;
        
        if (answer == NSAlertAlternateReturn) return NSTerminateCancel;
		
    }
	
    return NSTerminateNow;
}

- (IBAction)connectionAction:sender
{
	[sender setEnabled:NO];

	if([managedObjectContext isConnected]){
		[researchStationController setContent:nil];
		[managedObjectContext disconnect];
 		[researchStation release];
		researchStation = nil;
		NSLog(@"disconnecting");
	} else {
//		if ([managedObjectContext canConnect]){
			[managedObjectContext connect:nil];
//		}
		NSLog(@"connecting");

	}
	[self updateConnectionToolbarItem];
	
}

- (void) updateConnectionToolbarItem{
	if([managedObjectContext isConnected]){
		[connectionToolbarItem setImage:[NSImage imageNamed:@"LFFConnected"]];
		[connectionToolbarItem setLabel:@"Disconnect"];
		[connectionToolbarItem setEnabled:YES];
		NSLog(@"ToolbarConnected");
	} else {
		[connectionToolbarItem setImage:[NSImage imageNamed:@"LFFDisconnected"]];
		[connectionToolbarItem setLabel:@"Connect"];
		[connectionToolbarItem setEnabled:YES];
		NSLog(@"ToolbarDisconnected");
	}
}


/**
 Implementation of dealloc, to release the retained variables.
 */

- (void)dealloc {
	
    [window release];
    [managedObjectContext release];
	[researchStation release];
	[researchStationController release];
	
    [super dealloc];
}

@end
