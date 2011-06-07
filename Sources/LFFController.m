//
//  LFFController.m
//  LongingFastForward
//
//  Created by ole kristensen on 07/06/11.
//  Copyright 2011 Recoil Performance Group. All rights reserved.
//

#import "LFFController.h"


@implementation LFFController

-(id)initWithContext:(BXDatabaseContext*)context
{
	self = [super init];
	if (self) {
		databaseContext = context;
	}
	return self;
}


-(int) setupLFFClasses
{
	return [self setupLFFClassesInContext:databaseContext];
}

-(int) setupLFFClassesInContext:(BXDatabaseContext*)context
{
	BXEntityDescription * entity;
	int countOfClasses = 0;
	for (entity in [[context databaseObjectModel] entities]){
		NSString * lffClassName = [entity name];
		//Prepend LFF
		lffClassName = [NSString stringWithFormat:@"LFF%@",lffClassName];
		Class lffClass = NSClassFromString(lffClassName);
		[entity setDatabaseObjectClass:[lffClass class]];
		countOfClasses++;
	}
	return countOfClasses;
}

-(int) faultLFFClasses
{
	return [self faultLFFClassesInContext:databaseContext];
}

-(int) faultLFFClassesError: (NSError **) error
{
	return [self faultLFFClassesInContext:databaseContext error:error];
}

-(int) faultLFFClassesInContext:(BXDatabaseContext*)context{
	return [self faultLFFClassesInContext:databaseContext error:nil];
}

-(int) faultLFFClassesInContext:(BXDatabaseContext*)context error: (NSError **) error
{
	BXEntityDescription * entity;
	NSArray * arrayForFaulting;
	int countOfClasses = 0;
	for (entity in [[context databaseObjectModel] entities]){
		arrayForFaulting = [context executeFetchForEntity:entity withPredicate:nil returningFaults:YES updateAutomatically:YES error:error];
		countOfClasses++;
		if (error == nil) {
			countOfClasses++;
		} 

	}
	return countOfClasses;
}

@end
