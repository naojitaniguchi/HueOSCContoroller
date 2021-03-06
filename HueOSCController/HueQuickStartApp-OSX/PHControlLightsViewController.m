/*******************************************************************************
 Copyright (c) 2013 Koninklijke Philips N.V.
 All Rights Reserved.
 ********************************************************************************/

#import "PHControlLightsViewController.h"
#import <HueSDK_OSX/HueSDK.h>
#import "AppDelegate.h"

#define MAX_HUE 65535

@interface PHControlLightsViewController ()
    @property (nonatomic,weak) IBOutlet NSTextField *bridgeMacLabel;
    @property (nonatomic,weak) IBOutlet NSTextField *bridgeIpLabel;
    @property (nonatomic,weak) IBOutlet NSTextField *bridgeLastHeartbeatLabel;
    @property (nonatomic,weak) IBOutlet NSButton *randomLightsButton;
@end

@implementation PHControlLightsViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {

    }
    return self;
}

- (void)loadView{
    [super loadView];
    
    PHNotificationManager *notificationManager = [PHNotificationManager defaultManager];
    // Register for the local heartbeat notifications
    [notificationManager registerObject:self withSelector:@selector(localConnection) forNotification:LOCAL_CONNECTION_NOTIFICATION];
    [notificationManager registerObject:self withSelector:@selector(noLocalConnection) forNotification:NO_LOCAL_CONNECTION_NOTIFICATION];
    
    [self noLocalConnection];
}

- (void)localConnection{
    [self loadConnectedBridgeValues];
}

- (void)noLocalConnection{
    self.bridgeLastHeartbeatLabel.stringValue = NSLocalizedString(@"Not connected", @"");
    [self.bridgeLastHeartbeatLabel setEnabled:NO];
    self.bridgeIpLabel.stringValue = NSLocalizedString(@"Not connected", @"");
    [self.bridgeIpLabel setEnabled:NO];
    self.bridgeMacLabel.stringValue = NSLocalizedString(@"Not connected", @"");
    [self.bridgeMacLabel setEnabled:NO];
    
    [self.randomLightsButton setEnabled:NO];
}

- (void)loadConnectedBridgeValues{
    PHBridgeResourcesCache *cache = [PHBridgeResourcesReader readBridgeResourcesCache];
    
    // Check if we have connected to a bridge before
    if (cache != nil && cache.bridgeConfiguration != nil && cache.bridgeConfiguration.ipaddress != nil){
        
        // Set the ip address of the bridge
        self.bridgeIpLabel.stringValue = cache.bridgeConfiguration.ipaddress;
        
        // Set the mac adress of the bridge
        self.bridgeMacLabel.stringValue = cache.bridgeConfiguration.mac;
        
        // Check if we are connected to the bridge right now
        if (NSAppDelegate.phHueSDK.localConnected) {
            
            // Show current time as last successful heartbeat time when we are connected to a bridge
            NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
            [dateFormatter setDateStyle:NSDateFormatterNoStyle];
            [dateFormatter setTimeStyle:NSDateFormatterMediumStyle];
            
            self.bridgeLastHeartbeatLabel.stringValue = [NSString stringWithFormat:@"%@",[dateFormatter stringFromDate:[NSDate date]]];
            
            [self.randomLightsButton setEnabled:YES];
        } else {
            self.bridgeLastHeartbeatLabel.stringValue = NSLocalizedString(@"Waiting...", @"");
            [self.randomLightsButton setEnabled:NO];
        }
    }
}

- (IBAction)selectOtherBridge:(id)sender{
    [NSAppDelegate searchForBridgeLocal];
}

int hueAngle = 0 ;

- (IBAction)randomizeColoursOfConnectLights:(id)sender{
    [self.randomLightsButton setEnabled:NO];
    
    PHBridgeResourcesCache *cache = [PHBridgeResourcesReader readBridgeResourcesCache];
    id<PHBridgeSendAPI> bridgeSendAPI = [[[PHOverallFactory alloc] init] bridgeSendAPI];
    
    for (PHLight *light in cache.lights.allValues) {
        
        PHLightState *lightState = [[PHLightState alloc] init];


        int hv = (int)(((float)hueAngle/360.0) * 65535.0) ;
        NSLog(@"randomizeColoursOfConnectLights hueAngle %d hv %d", hueAngle, hv );
        // [lightState setHue:[NSNumber numberWithInt:hv]];
        
        // [lightState setHue:[NSNumber numberWithInt:46920]];

        [lightState setHue:[NSNumber numberWithInt:arc4random() % MAX_HUE]];
        // [lightState setHue:[NSNumber numberWithInt:32767]];
        [lightState setBrightness:[NSNumber numberWithInt:254]];
        [lightState setSaturation:[NSNumber numberWithInt:254]];
        
        // Send lightstate to light
        [bridgeSendAPI updateLightStateForId:light.identifier withLighState:lightState completionHandler:^(NSArray *errors) {
            if (errors != nil) {
                NSString *message = [NSString stringWithFormat:@"%@: %@", NSLocalizedString(@"Errors", @""), errors != nil ? errors : NSLocalizedString(@"none", @"")];
                
                NSLog(@"Response: %@",message);
            }
            
            [self.randomLightsButton setEnabled:YES];
        }];
    }
    hueAngle += 30 ;
    if ( hueAngle > 360 ){
        hueAngle = 0 ;
    }
}

@end
