#import "LFFResearchStation.h"

@implementation LFFResearchStation
-(void)awakeFromInsert{
	[super awakeFromInsert];
	NSLog(@"Research station inserted");
}

-(void)awakeFromFetch{
	[super awakeFromFetch];
	NSLog(@"Research station fetched");
}


@end