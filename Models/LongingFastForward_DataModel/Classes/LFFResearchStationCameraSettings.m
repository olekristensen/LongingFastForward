#import "LFFResearchStationCameraSettings.h"

static NSNib *researchStationCameraSettingsNib;

@implementation LFFResearchStationCameraSettings

@synthesize contentView, controller;

+ (void)initialize 
{ 
    researchStationCameraSettingsNib = [[NSNib alloc] initWithNibNamed:@"LFFResearchStationCameraSettings" bundle:[NSBundle bundleWithIdentifier:@"gl.longing.LongingFastForward"]];
}

-(void) setup
{
	NSLog(@"ResearchStationCameraSettings Setup");
	[researchStationCameraSettingsNib instantiateNibWithOwner:self topLevelObjects:nil]; 
	NSAssert(contentView != nil && controller != nil, @"IBOutlets were not set correctly in LFFResearchStationCameraSettings.xib");
	[[self databaseContext]refreshObject:self mergeChanges:NO];
	[controller setContent: self];
}

-(void)awakeFromInsert{
	[super awakeFromInsert];
	[self setup];
}

-(void)awakeFromFetch{
	[super awakeFromFetch];
	[self setup];
}

-(void)dealloc{
	[controller setContent: nil];
	[contentView release];
	[super dealloc];
}

@end
