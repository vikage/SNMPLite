//
//  ViewController.m
//  SNMPLiteDemo
//
//  Created by thanhvu on 7/14/19.
//  Copyright © 2019 thanhvu. All rights reserved.
//

#import "ViewController.h"
#import <SNMPLite/SNMPLite.h>
@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [[SNMPLiteManager shared] getOid:@"1.3.6.1.2.1.1.6.0" address:@"192.168.100.192" retry:3 timeout:1000 completeHandler:^(NSDictionary *result) {
        NSLog(@"Result %@", result);
    }];
    // Do any additional setup after loading the view.
}


@end
