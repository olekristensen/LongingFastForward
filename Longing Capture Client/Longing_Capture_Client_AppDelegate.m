//
//  Longing_Capture_Client_AppDelegate.m
//  Longing Capture Client
//
//  Created by ole kristensen on 23/05/11.
//  Copyright Recoil Performance Group 2011 . All rights reserved.
//

#import "Longing_Capture_Client_AppDelegate.h"
#import <BaseTen/BaseTen.h>
#import <BaseTen/BXLogger.h>

@interface Longing_Capture_Client_AppDelegate()

+ (void)setupDefaults;
- (void)updateConnectionToolbarItem;

@end


@implementation Longing_Capture_Client_AppDelegate

@synthesize window, managedObjectContext, captureClientController;

+ (void)initialize{
	
    [self setupDefaults];
}

+ (void)setupDefaults
{
    NSString *userDefaultsValuesPath;
    NSDictionary *userDefaultsValuesDict;
    NSDictionary *initialValuesDict;
    NSArray *resettableUserDefaultsKeys;
	
    // load the default values for the user defaults
    userDefaultsValuesPath=[[NSBundle mainBundle] pathForResource:@"UserDefaults"
														   ofType:@"plist"];
    userDefaultsValuesDict=[NSDictionary dictionaryWithContentsOfFile:userDefaultsValuesPath];
	
    // set them in the standard user defaults
    [[NSUserDefaults standardUserDefaults] registerDefaults:userDefaultsValuesDict];
	
    // if your application supports resetting a subset of the defaults to
    // factory values, you should set those values
    // in the shared user defaults controller
    resettableUserDefaultsKeys=[NSArray arrayWithObjects:@"captureClientName",nil];
    initialValuesDict=[userDefaultsValuesDict dictionaryWithValuesForKeys:resettableUserDefaultsKeys];
	
    // Set the initial values in the shared user defaults controller
    [[NSUserDefaultsController sharedUserDefaultsController] setInitialValues:initialValuesDict];
}

- (void) awakeFromNib {
	BXLogSetLevel(kBXLogLevelDebug);
	NSAssert (nil != managedObjectContext, @"Longing_Capture_Client_AppDelegate's outlets were not set up correctly.");
}

#pragma mark Database Delegate

- (void) databaseContextConnectionSucceeded: (BXDatabaseContext *) ctx
{	
	
	NSString * captureClientName = [[NSUserDefaults standardUserDefaults] stringForKey:@"captureClientName"];
	NSString * defaultCaptureClientName = [[[NSUserDefaultsController sharedUserDefaultsController] initialValues] objectForKey:@"captureClientName"];
	
	if ([captureClientName isEqualToString:defaultCaptureClientName ]) {
		captureClientController = [[LFFCaptureClientController alloc] initWithCaptureClientName:@""];
	} else {
		captureClientController = [[LFFCaptureClientController alloc] initWithCaptureClientName:captureClientName];
	}
	
	if (captureClientController != nil) {
		[[NSUserDefaults standardUserDefaults] setValue:[[captureClientController captureClient] name] forKey:@"captureClientName"];
	} else {
		dispatch_async(dispatch_get_main_queue(), ^{[self presentCaptureClientSelectionSheet];});
	}
	
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


- (IBAction) saveAction:(id)sender {

    NSError *error = nil;
    
    if (![[self managedObjectContext] commitEditing]) {
        NSLog(@"%@:%s unable to commit editing before saving", [self class], _cmd);
    }

    if (![[self managedObjectContext] save:&error]) {
        [[NSApplication sharedApplication] presentError:error];
    }
}

#pragma mark Capture Client Selection Sheet

-(void)presentCaptureClientSelectionSheet{
	[NSApp beginSheet: captureClientSelectionSheet
	   modalForWindow: window
		modalDelegate: self
	   didEndSelector: @selector(didEndCaptureClientSelectionSheet:returnCode:contextInfo:)
		  contextInfo: nil];
}

-(NSString*)captureClientSelectionSheetCloseButtonText{
	if (captureClientController == nil) {
		return @"Quit";
	} else {
		return @"Cancel";
	}
}

-(BOOL)captureClientSelectionSheetEnabled{
	if ([[captureClientArrayController arrangedObjects] count] == 0) {
		return NO;
	} else {
		return YES;
	}
}


- (IBAction)closeCaptureClientSelectionSheet: (id)sender
{
    [NSApp endSheet:captureClientSelectionSheet];
}

- (void)didEndCaptureClientSelectionSheet:(NSWindow *)sheet returnCode:(NSInteger)returnCode contextInfo:(void *)contextInfo
{
    [sheet orderOut:self];
	if (captureClientController == nil) {
		[NSApp terminate:self];
	}
}

#pragma mark Toolbar

- (IBAction)connectionAction:sender
{
	[sender setEnabled:NO];
	
	if([managedObjectContext isConnected]){
		[captureClientController setContent:nil];
		[captureClientController setSelectedObjects:nil];
		[managedObjectContext disconnect];
		[managedObjectContext setDatabaseURI:databaseUrlFromNib];
 		[captureClientController release];
		captureClientController = nil;
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

#pragma mark Application Delegate

- (NSString *)applicationSupportDirectory {
	
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
    NSString *basePath = ([paths count] > 0) ? [paths objectAtIndex:0] : NSTemporaryDirectory();
    return [basePath stringByAppendingPathComponent:@"Longing_Capture_Client"];
}

#pragma mark Window Delegate

- (NSUndoManager *)windowWillReturnUndoManager:(NSWindow *)window {
    return [[self managedObjectContext] undoManager];
}

#pragma mark Termination

- (BOOL)applicationShouldTerminateAfterLastWindowClosed:(NSApplication *)theApplication
{
	return YES;
}

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

- (void)dealloc {
	
    [window release];
    [managedObjectContext release];
	[captureClientController release];
    [super dealloc];
}

@end
