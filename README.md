# SNMPLite

[![CocoaPods compatible](https://img.shields.io/badge/CocoaPods-compatible-green.svg?style=flat)](https://cocoapods.org)
SNMPLite is iOS Framework, current perform SNMP v2 get commands only.

# Installation
### From CocoaPods

[CocoaPods](http://cocoapods.org) is a dependency manager for Objective-C, which automates and simplifies the process of using 3rd-party libraries like `SNMPLite` in your projects. First, add the following line to your [Podfile](http://guides.cocoapods.org/using/using-cocoapods.html):

```ruby
pod 'SNMPLite'
```

# Usage
(see sample Xcode project in SNMPLiteDemo)
```objective-c
[[SNMPLiteManager shared] getOid:@"1.3.6.1.2.1.4.24.4.1.16.0.0.0.0.0.0.0.0.0.195.218.254.97" address:@"104.236.166.95" retry:3 timeout:1000 completeHandler:^(NSDictionary *result) {
       
}];
```

# License
`SNMPLite` is distributed under the terms and conditions of the [MIT license](https://github.com/SVProgressHUD/SVProgressHUD/blob/master/LICENSE).