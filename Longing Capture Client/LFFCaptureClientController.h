//
//  LFFCaptureClientController.h
//  Longing Capture Client
//
//  Created by ole kristensen on 31/05/11.
//  Copyright 2011 Recoil Performance Group. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import <LongingFastForward/LongingFastForward.h>
#import <BaseTenAppKit/BaseTenAppKit.h>


@interface LFFCaptureClientController : NSObject {
	
	LFFResearchStation * researchStation;
	LFFCaptureClient * captureClient;

}

@property (nonatomic, retain, readonly) LFFResearchStation *researchStation;
@property (nonatomic, retain, readonly) LFFCaptureClient * captureClient;


-(id) initWithCaptureClientName:(NSString*)captureClientName;
-(LFFCaptureClient *) captureClientWithName:(NSString*)captureClientName;

@end
