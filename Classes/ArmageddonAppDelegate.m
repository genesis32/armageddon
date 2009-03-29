//
//  ArmageddonAppDelegate.m
//  Armageddon
//
//  Created by ddm on 1/10/09.
//  Copyright __MyCompanyName__ 2009. All rights reserved.
//

#import "ArmageddonAppDelegate.h"
#import "EAGLView.h"

@implementation ArmageddonAppDelegate

@synthesize window;
@synthesize glView;

- (void)applicationDidFinishLaunching:(UIApplication *)application {

	glView.animationInterval = 1.0 / 60.0;
	[glView startAnimation];
}


- (void)applicationWillResignActive:(UIApplication *)application {
	glView.animationInterval = 1.0 / 5.0;
}


- (void)applicationDidBecomeActive:(UIApplication *)application {
	glView.animationInterval = 1.0 / 60.0;
}


- (void)dealloc {
	[window release];
	[glView release];
	[super dealloc];
}

-(BOOL)shouldAutorotateToInterfaceOrientation:
(UIInterfaceOrientation)interfaceOrientation
{
	return (interfaceOrientation == UIInterfaceOrientationLandscapeLeft ||
			interfaceOrientation ==	UIInterfaceOrientationLandscapeRight);
}


@end
