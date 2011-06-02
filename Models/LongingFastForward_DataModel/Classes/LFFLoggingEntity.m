#import "LFFLoggingEntity.h"

@implementation LFFLoggingEntity

-(void)logEvent:(NSString*)logString{
	
	
	NSLog(@"LOGGING \n%s", logString);
	NSDictionary* values = [NSDictionary dictionaryWithObjectsAndKeys:
							[NSDate date], @"time",
							logString, @"message",
							self, @"loggingEntity",
							nil];
	
	BXEntityDescription * entity = [[self databaseObjectModel] entityForTable: @"Event"];
	
	LFFEvent *newEvent = [[self databaseContext] createObjectForEntity: entity
													   withFieldValues: values error: NULL];
	
	[self addEventLogObject:newEvent];
}

-(void)awakeFromInsert{
	NSLog(@"AWAKE FROM INSERT \n%s", [self description]);

	[super awakeFromInsert];
	[self logEvent:[NSString stringWithFormat:@"Created: %s", [self description]]];
}


@end
