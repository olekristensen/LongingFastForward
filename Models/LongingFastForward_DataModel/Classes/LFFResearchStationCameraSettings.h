#import "_LFFResearchStationCameraSettings.h"

@interface LFFResearchStationCameraSettings : _LFFResearchStationCameraSettings {

IBOutlet NSView *contentView;
IBOutlet NSObjectController* controller;

}

@property (nonatomic, retain) NSView *contentView;
@property (nonatomic, retain) NSObjectController *controller;

-(void) setup;

@end
