#import "LFFResearchStation.h"
#import "LFFResearchStationCameraSettings.h"

static NSNib *researchStationNib;

@implementation LFFResearchStation

@synthesize contentView, controller, cameraSettingsView;

+ (void)initialize 
{ 
    researchStationNib = [[NSNib alloc] initWithNibNamed:@"LFFResearchStation" bundle:[NSBundle bundleWithIdentifier:@"gl.longing.LongingFastForward"]];
}

-(void) setup
{
	NSLog(@"ResearchStation Setup");
	[researchStationNib instantiateNibWithOwner:self topLevelObjects:nil]; 
	NSAssert(contentView != nil && controller != nil, @"IBOutlets were not set correctly in LFFResearchStation.xib");
	[[self databaseContext]refreshObject:self mergeChanges:NO];
	[[self cameraSettingsTarget] setup];
	[controller setContent: self];
	
	[[cameraSettingsView subviews] makeObjectsPerformSelector:@selector(removeFromSuperview)];
	[cameraSettingsView addSubview:[[self cameraSettingsTarget] contentView]];
	
}

-(void)awakeFromInsert{
	[super awakeFromInsert];
	[self setup];
}

-(void)awakeFromFetch{
	[super awakeFromFetch];
	[self setup];
}

-(void)didTurnIntoFault
{
	[[self cameraSettingsTarget] faultKey:nil];
}


-(void)dealloc{
	[controller setContent: nil];
	[contentView release];
	[cameraSettingsView release];
	[super dealloc];
}

@end