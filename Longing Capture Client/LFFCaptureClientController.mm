//
//  LFFCaptureClientController.mm
//  Longing Capture Client
//
//  Created by ole kristensen on 31/05/11.
//  Copyright 2011 Recoil Performance Group. All rights reserved.
//

#import "LFFCaptureClientController.h"

@implementation LFFCaptureClientController
@synthesize captureClient, researchStation;


-(id) init {
	return nil;	
}

-(id) initWithCaptureClientName:(NSString*)captureClientName
{
	self = [super init];
	if (self) {
		researchStation = [self researchStation];
		if (researchStation != nil) {
			captureClient = [self captureClientWithName:captureClientName];
			if (captureClient != nil) {
				return self;
			}
		}
	}
	return nil;
}

- (LFFResearchStation *) researchStation {
	
	BXDatabaseContext * managedObjectContext = (BXDatabaseContext*)[[NSApp delegate] managedObjectContext];
	
	if (![managedObjectContext isConnected]) {
		NSLog(@"no connection");
		return nil;
	}
	
	if (researchStation != nil) return researchStation;
	
	NSError * error = nil;
	
	researchStation = [managedObjectContext objectWithID:[[managedObjectContext objectIDsForEntity:[[managedObjectContext databaseObjectModel] entityForTable:@"ResearchStation"] error:&error] objectAtIndex:0] error:&error];
	
	if (error != nil) {
//		NSAlert *theAlert = [NSAlert alertWithError:error];
//		[theAlert runModal]; 
	}	
	
	return researchStation;
	
}

-(LFFCaptureClient*) captureClient {
	
	BXDatabaseContext * managedObjectContext = (BXDatabaseContext*)[[NSApp delegate] managedObjectContext];
	
	if (![managedObjectContext isConnected]) {
		NSLog(@"no connection");
		return nil;
	}
	
	if (captureClient != nil) return captureClient;
	
	return [self captureClientWithName:@""];
	
} 

-(LFFCaptureClient *) captureClientWithName:(NSString*)captureClientName{
	
	BXDatabaseContext * managedObjectContext = (BXDatabaseContext*)[[NSApp delegate] managedObjectContext];
	
	if (![managedObjectContext isConnected]) {
		NSLog(@"no connection");
		return nil;
	}
	
	if (captureClient != nil){
		if (captureClient.name == captureClientName) {
			return captureClient;
		}
	} 
	
	if ([[[self researchStation] captureClients] count] == 0){
		NSLog(@"No Capture Clients in Database!");
		return nil;
	}
	
	NSSet *setOfClients = [[[self researchStation] captureClients] filteredSetUsingPredicate:[NSPredicate predicateWithFormat:@"name == %@", captureClientName]];
	
	if ([setOfClients count] == 0) {
		NSLog(@"No Capture Clients matched the name %s", captureClientName);
		return nil;
	}
	
	if ([setOfClients count] == 1) {
		// this is where we are if everything works
		captureClient = [setOfClients anyObject];
		return captureClient;
	}
	
	if ([setOfClients count] == 1) {
		NSLog(@"More than one Capture Client matched the name %s", captureClientName);
		return nil;
	}	
	return captureClient;
}

@end
