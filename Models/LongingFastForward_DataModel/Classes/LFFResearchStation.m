#import "LFFResearchStation.h"


static NSNib *researchStationNib;

@implementation LFFResearchStation

@synthesize contentView, controller, cameraSettingsView;

+ (void)initialize 
{ 
    researchStationNib = [[NSNib alloc] initWithNibNamed:@"LFFResearchStation" bundle:[NSBundle bundleWithIdentifier:@"gl.longing.LongingFastForward"]];
}

-(void) setup
{
	
	[researchStationNib instantiateNibWithOwner:self topLevelObjects:nil]; 
	NSAssert(contentView != nil, @"IBOutlets were not set correctly in LFFResearchStation.xib");
	
	[controller setContent: self];
//	[view setDelegate:self];
		
}

-(void)awakeFromInsert{
	[super awakeFromInsert];
	[self setup];
	NSLog(@"Research station inserted");
}

-(void)awakeFromFetch{
	[super awakeFromFetch];
	[self setup];
	NSLog(@"Research station fetched");
}


@end