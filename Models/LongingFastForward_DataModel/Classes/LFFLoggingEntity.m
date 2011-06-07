#import "LFFLoggingEntity.h"

@implementation LFFLoggingEntity

-(void)logEvent:(NSString*)logString{
	
	//FIXME: Inheritance
	
/**	NSLog(@"LOGGING: %@", logString);
	NSDictionary* values = [NSDictionary dictionaryWithObjectsAndKeys:
							[NSDate date], @"time",
							logString, @"message",
							self, @"loggingEntity",
							nil];
	
	BXEntityDescription * entity = [[[self databaseContext] databaseObjectModel] entityForTable: @"Event"];
	
	LFFEvent *newEvent = [[self databaseContext] createObjectForEntity: entity
													   withFieldValues: values error: NULL];
	
	[self addEventLogObject:newEvent];
 **/
}

-(void)awakeFromInsert{
	[super awakeFromInsert];
	[self logEvent:[NSString stringWithFormat:@"Created: %@", [self description]]];
}


@end
