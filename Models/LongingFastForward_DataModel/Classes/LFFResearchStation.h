#import "_LFFResearchStation.h"

@interface LFFResearchStation : _LFFResearchStation {
	
	IBOutlet NSView *contentView;
	IBOutlet NSObjectController* controller;
	IBOutlet NSView *cameraSettingsView;

}

@property (nonatomic, retain) NSView *contentView;
@property (nonatomic, retain) NSObjectController *controller;
@property (nonatomic, retain) NSView *cameraSettingsView;

-(void) setup;

@end
