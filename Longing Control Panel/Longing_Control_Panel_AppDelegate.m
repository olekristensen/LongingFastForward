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

@interface Longing_Control_Panel_AppDelegate()

+ (void)setupDefaults;
- (void)setupLFFClasses;
- (void)updateConnectionToolbarItem;
- (void)showContentViewForObject:(id)object;
- (BOOL)populateDatabaseHadError:(NSError*)theError;
- (void)disconnect;

@end

@implementation Longing_Control_Panel_AppDelegate

@synthesize window, managedObjectContext, researchStation;

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
    resettableUserDefaultsKeys=[NSArray arrayWithObjects:@"defaultNumberOfCaptureClients", @"defaultResearchStationLocation", nil];
    initialValuesDict=[userDefaultsValuesDict dictionaryWithValuesForKeys:resettableUserDefaultsKeys];
	
    // Set the initial values in the shared user defaults controller
    [[NSUserDefaultsController sharedUserDefaultsController] setInitialValues:initialValuesDict];
}

- (void) awakeFromNib
{
	NSAssert (nil != managedObjectContext, @"Longing_Control_Panel_AppDelegate's outlets were not set up correctly.");
	
	//BXLogSetLevel(kBXLogLevelDebug);
	//[managedObjectContext setLogsQueries:YES];
	
	databaseUrlFromNib = [managedObjectContext databaseURI];
	[managedObjectContext setDelegate:self];
	[managedObjectContext setModalWindow:window];
	
	lffController = [[LFFController alloc]initWithContext:managedObjectContext];
	
	[self updateConnectionToolbarItem];
}

#pragma mark Database Delegate

- (void) databaseContextConnectionSucceeded: (BXDatabaseContext *) ctx
{	
	NSLog(@"databaseContextConnectionSucceeded");
	
	[self setupLFFClasses];
	[self updateConnectionToolbarItem];
}

-(void) setupLFFClasses
{
	NSError * theError = nil;
	
	int classesSetup = 0;
	int classesFaulted = 0;
	
	classesSetup = [lffController setupLFFClasses];
	if (classesSetup > 0) {
		classesFaulted = [lffController faultLFFClassesError:&theError];
	}
	
	if(classesSetup > 0 && classesFaulted > 0 && classesSetup == classesFaulted ){
		[researchStationController setDatabaseContext:managedObjectContext];
		researchStation = [self researchStation];
		if (researchStation == nil) {
			dispatch_async(dispatch_get_main_queue(), ^{[self presentPopulateDatabaseSheet];});
		}
	} else {
		NSAlert * noClassesAlert;
		if (theError != nil) {
			noClassesAlert = [NSAlert alertWithError:theError];
			[noClassesAlert addButtonWithTitle:@"Quit"];
		} else {
			noClassesAlert = [NSAlert alertWithMessageText:@"The LFF object model was not found in the database"
											 defaultButton:@"Quit"
										   alternateButton:@"Try Again" 
											   otherButton:nil 
								 informativeTextWithFormat:@"You will have to setup the object model in the database using the BaseTen Assistant"
							  ];
		}
		
		dispatch_async(dispatch_get_main_queue(), ^{
			[noClassesAlert beginSheetModalForWindow:[self window] 
									   modalDelegate:self 
									  didEndSelector:@selector(noClassesAlertDidEnd:returnCode:contextInfo:) 
										 contextInfo:nil];
		});
	}
}

- (void)noClassesAlertDidEnd:(NSAlert *)alert returnCode:(NSInteger)returnCode contextInfo:(void *)contextInfo
{
	if (returnCode == NSAlertDefaultReturn) {
		[[NSApplication sharedApplication] terminate:self];
	}
	if (returnCode == NSAlertAlternateReturn) {
		[self setupLFFClasses];
	}
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

- (void) databaseContext: (BXDatabaseContext *) context 
				hadError: (NSError *) anError 
		  willBePassedOn: (BOOL) willBePassedOn
{
	if (!willBePassedOn) {
		NSAlert * databaseContextErrorAlert = [NSAlert alertWithError:anError];
		
		[databaseContextErrorAlert addButtonWithTitle:@"Quit"];
		[databaseContextErrorAlert addButtonWithTitle:@"Continue"];
		
		dispatch_async(dispatch_get_main_queue(), ^{
			[databaseContextErrorAlert beginSheetModalForWindow:[self window] 
												  modalDelegate:self 
												 didEndSelector:@selector(databaseContextErrorAlertDidEnd:returnCode:contextInfo:) 
													contextInfo:nil];
		});
	}
}

- (void)databaseContextErrorAlertDidEnd:(NSAlert *)alert returnCode:(NSInteger)returnCode contextInfo:(void *)contextInfo{
	
	if (returnCode == NSAlertFirstButtonReturn) {
		[[NSApplication sharedApplication] terminate:self];
	}
}


# pragma mark Database Object Management

- (LFFResearchStation *) researchStation {
	
	if (![managedObjectContext isConnected]) {
		return nil;
	} else {
		if (researchStation) return researchStation;
		
		[researchStationController fetch:nil];
		
		if ([[researchStationController arrangedObjects] count] > 0) {
			if ([[researchStationController arrangedObjects] count] > 1) {
				[researchStationController removeObjectsAtArrangedObjectIndexes:
				 [NSIndexSet indexSetWithIndexesInRange:NSMakeRange(1, [[researchStationController arrangedObjects] count]-1)]
				 ];
			}
			researchStation = [[researchStationController arrangedObjects] objectAtIndex:0];
			[researchStation setup];
			[self showContentViewForObject:researchStation];
			return researchStation;
		} else {
			return nil;
		}
	}
	
}

-(void)showContentViewForObject:(id)object
{
	if ([object respondsToSelector:@selector(contentView)]) {
		[[objectView subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
		[objectView addSubview:[object contentView]];
		[[object contentView] setFrame:[objectView bounds]];
	}
}

-(LFFResearchStation *) populateDatabaseClearExisting:(BOOL) clearExisting {
	
	if (clearExisting) {
		
		//FIXME: clear database
		
	}
	
	if (researchStation != nil) {
		
		return researchStation;
		
	} else {
		
		//Make the connection transactional, so we can roll back in case of errors.
		[managedObjectContext disconnect];
		[managedObjectContext setAutocommits:NO];
		[managedObjectContext connect:nil];
		
		BXEntityDescription* entity;
		NSArray * angleNames = [NSArray arrayWithObjects:
								@"North",@"East",@"South",@"West",nil];
		
		int iAngles = 0;
		int numCaptureClients = [[[NSUserDefaults standardUserDefaults] stringForKey:@"defaultNumberOfCaptureClients"] intValue];		
		
		// CREATE RESEARCH STATION
		NSError * researchStationError = nil;
		entity = [[managedObjectContext databaseObjectModel] entityForTable: @"ResearchStation"];
		NSDictionary* researchStationValues = [NSDictionary dictionaryWithObjectsAndKeys:
											   [[NSUserDefaults standardUserDefaults] stringForKey:@"defaultResearchStationLocation"], @"location",
											   nil];
		
		LFFResearchStation* newResearchStation = [managedObjectContext createObjectForEntity: entity 
																			 withFieldValues: researchStationValues 
																					   error: &researchStationError];
		if ([self populateDatabaseHadError:researchStationError] || 
			newResearchStation == nil) {
			return nil;
		} else {
			
			NSLog(@"Created: %@", newResearchStation);
			
			// CREATE RESEARCH STATION CAMERA SETTINGS
			NSError * researchStationCameraSettingsError = nil;
			entity= [[managedObjectContext databaseObjectModel] entityForTable: @"ResearchStationCameraSettings"];
			NSDictionary* researchStationCameraSettingsValues = [NSDictionary dictionaryWithObjectsAndKeys:
																 newResearchStation, @"researchStation",
																 nil];
			LFFResearchStationCameraSettings *newResearchStationCameraSettings = [managedObjectContext createObjectForEntity: entity 
																											 withFieldValues: researchStationCameraSettingsValues 
																													   error: &researchStationCameraSettingsError];
			if ([self populateDatabaseHadError:researchStationCameraSettingsError] || 
				newResearchStationCameraSettings == nil) {
				return nil;
			} else {
				
				NSLog(@"Created: %@", newResearchStationCameraSettings);
				
				//CREATE DEFAULT SETTINGS
				//FIXME: Put in some meaningful values for useless camera settings etc...  
				NSError * cameraDefaultSettingsError = nil;
				entity= [[managedObjectContext databaseObjectModel] entityForTable: @"CameraDefaultSettings"];
				NSDictionary* cameraDefaultSettingsValues = [NSDictionary dictionaryWithObjectsAndKeys:
															 @"Default Useless Setting", @"name",
															 nil];
				LFFCameraDefaultSettings *newCameraDefaultSettings = [managedObjectContext createObjectForEntity: entity 
																								 withFieldValues: cameraDefaultSettingsValues 
																										   error: &cameraDefaultSettingsError];
				if ([self populateDatabaseHadError:cameraDefaultSettingsError] || 
					newCameraDefaultSettings == nil) {
					return nil;
				} else {
					
					NSLog(@"Created: %@", newCameraDefaultSettings);
					
					for (NSString* angleName in angleNames){
						
						// CREATE CAMERA ANGLE
						NSError * cameraAngleError = nil;
						entity= [[managedObjectContext databaseObjectModel] entityForTable: @"CameraAngle"];
						NSDictionary* cameraAngleValues = [NSDictionary dictionaryWithObjectsAndKeys:
														   angleName, @"name",
														   newCameraDefaultSettings, @"defaultSettings",
														   nil];
						LFFCameraAngle *newCameraAngle = [managedObjectContext createObjectForEntity: entity 
																					 withFieldValues: cameraAngleValues
																							   error: &cameraAngleError];
						
						
						if ([self populateDatabaseHadError:cameraAngleError] || 
							newCameraAngle == nil) {
							return nil;
						} else {
							
							NSLog(@"Created: %@", newCameraAngle);
							
							// CREATE CAMERA ANGLE SETTINGS
							NSError * cameraAngleSettingsError = nil;
							entity= [[managedObjectContext databaseObjectModel] entityForTable: @"CameraAngleSettings"];
							NSDictionary* cameraAngleSettingsValues = [NSDictionary dictionaryWithObjectsAndKeys:
																	   newCameraAngle, @"cameraAngle",
																	   nil];
							LFFCameraAngleSettings *newCameraAngleSettings = [managedObjectContext createObjectForEntity: entity
																										 withFieldValues: cameraAngleSettingsValues 
																												   error: &cameraAngleSettingsError];
							
							if ([self populateDatabaseHadError:cameraAngleSettingsError] || 
								newCameraAngleSettings == nil) {
								return nil;
							} else {
								
								NSLog(@"Created: %@", newCameraAngleSettings);
								
								// CREATE CAPTURE CLIENTS
								
								if(iAngles < numCaptureClients){
									NSError * captureClientError = nil;
									entity= [[managedObjectContext databaseObjectModel] entityForTable: @"CaptureClient"];
									NSDictionary* captureClientValues = [NSDictionary dictionaryWithObjectsAndKeys:
																		 newCameraAngle, @"cameraAngle",
																		 @"0.0.0.0", @"ipAddress",
																		 angleName, @"name",
																		 newResearchStation, @"researchStation",
																		 @"/Users/longing/LFFDataStore", @"datastoreURL",
																		 nil];
									
									LFFCaptureClient * newCaptureClient = [managedObjectContext createObjectForEntity: entity 
																									  withFieldValues: captureClientValues 
																												error: &captureClientError];
									if ([self populateDatabaseHadError:captureClientError] || 
										newCaptureClient == nil) {
										return nil;
									} else {
										NSLog(@"Created: %@", newCaptureClient);
									}								
								}
							}
						}
						iAngles++;
					}
					
				}								
			}
		}
		// if we get this far, we succeeded in making the objects.
		
		NSError * saveError = nil;
		
		[managedObjectContext save:&saveError];
		
		if ([self populateDatabaseHadError:saveError]) {
			return nil;
		} else {
			// we've committed the objects, and can revert to autocommit mode.
			[managedObjectContext disconnect];
			[managedObjectContext setAutocommits:YES];
			[managedObjectContext connect:nil];
			
			NSAlert* populateDatabaseFinishedAlert = [NSAlert alertWithMessageText:@"Database populated"
																defaultButton:@"OK"
															  alternateButton:nil
																  otherButton:nil
													informativeTextWithFormat:@"All objects were created sucessfully"
								];
			[populateDatabaseFinishedAlert setAlertStyle:NSInformationalAlertStyle];
			[populateDatabaseFinishedAlert beginSheetModalForWindow:window
													  modalDelegate:self 
													 didEndSelector:nil 
														contextInfo:nil];
			
			return newResearchStation;
		}
	}
	return nil;
}

-(BOOL) populateDatabaseHadError:(NSError*)theError
{	
	if (theError != nil) {
		
		NSAlert * populateDatabaseErrorAlert = [NSAlert alertWithError:theError];
		
		[populateDatabaseErrorAlert addButtonWithTitle:@"Reconnect"];
		[populateDatabaseErrorAlert addButtonWithTitle:@"Quit"];
		[populateDatabaseErrorAlert setAlertStyle:NSCriticalAlertStyle];
		//A nice way to change button bezel styles
		//
		//for(NSButton * button in [populateDatabaseErrorAlert buttons]){
		//	[button setBezelStyle:NSTexturedRoundedBezelStyle];
		//}
		[populateDatabaseErrorAlert setMessageText:@"There was an error populating the database"];
		[populateDatabaseErrorAlert setInformativeText:[[populateDatabaseErrorAlert informativeText] stringByAppendingString:@"\nThe database has been rolled back and you have been disconnected"]];
		[populateDatabaseErrorAlert beginSheetModalForWindow:[self window] 
											   modalDelegate:self 
											  didEndSelector:@selector(populateDatabaseErrorAlertDidEnd:returnCode:contextInfo:) 
												 contextInfo:nil];
		
		//Rollback should not be optional, we don't want shadow objects in the database 
		[managedObjectContext rollback];
		[managedObjectContext disconnect];
		[managedObjectContext setAutocommits:YES];
		//And there's no reason to remain connected.
		[self disconnect];
		return YES;
	}
	return NO;
}

- (void)populateDatabaseErrorAlertDidEnd:(NSAlert *)alert returnCode:(NSInteger)returnCode contextInfo:(void *)contextInfo{
	
	if (returnCode == NSAlertFirstButtonReturn) {
		[self connectionAction:connectionToolbarItem];
	}
	if (returnCode == NSAlertSecondButtonReturn) {
		[[NSApplication sharedApplication] terminate:self];
	}
}


#pragma mark Populate Database Sheet

-(void)presentPopulateDatabaseSheet{
	
	[populateDatabaseSheetDeleteFirst setState:NSOffState];
	
	[NSApp beginSheet: populateDatabaseSheet
	   modalForWindow: window
		modalDelegate: self
	   didEndSelector: @selector(didEndPopulateDatabaseSheet:returnCode:contextInfo:)
		  contextInfo: nil];
}

-(NSString*)populateDatabaseSheetCloseButtonText{
	if (researchStation == nil) {
		return @"Quit";
	} else {
		return @"Cancel";
	}
}

- (IBAction)closePopulateDatabaseSheet: (id)sender
{
	NSInteger returnCode = [sender tag] + [populateDatabaseSheetDeleteFirst state];
	
	// "Cancel"/"Quit" tag	= 100
	// "Populate" tag		= 200
	
	[NSApp endSheet:populateDatabaseSheet returnCode:returnCode];
}

- (void)didEndPopulateDatabaseSheet:(NSWindow *)sheet returnCode:(NSInteger)returnCode contextInfo:(void *)contextInfo
{
	[sheet orderOut:self];
	
	NSLog(@"didEndPoulate returnCode: %i", returnCode);
	
	switch (returnCode) {
		case 100:
		case 101:
			if (researchStation == nil) {
				[NSApp terminate:self];
			}
			break;
		case 201:
			[self populateDatabaseClearExisting:YES];
			break;
		case 200:
			[self populateDatabaseClearExisting:NO];
			break;
		default:
			break;
	}
	
}

#pragma mark Toolbar

- (IBAction)connectionAction:sender
{
	[sender setEnabled:NO];
	
	if([managedObjectContext isConnected]){
		[self disconnect];
	} else {
		//FIXME	why does can connect not work?
		// if ([managedObjectContext canConnect]){
		[managedObjectContext connect:nil];
		//		}
		NSLog(@"connecting");
		
	}
	[self updateConnectionToolbarItem];
	
}

-(void)disconnect{
	[[objectView subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
	[researchStationController setContent:nil];
	[researchStationController setSelectedObjects:nil];
	[managedObjectContext disconnect];
	[managedObjectContext setDatabaseURI:databaseUrlFromNib];
	[researchStation release];
	researchStation = nil;
	[self updateConnectionToolbarItem];
	NSLog(@"disconnected");
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
	[researchStation release];
	
	[super dealloc];
}

@end
