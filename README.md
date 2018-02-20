# iOS Skyhook Context SDK

The Skyhook Context SDK for iOS provides a simple way to integrate Skyhook Context services into your iOS applications.

## Installation

Xcode 9.0+ is required for all projects. Projects must target >= iOS9

### CocoaPods

Specify the SkyhookContext in your podfile:

```
# Skyhook Context SDK
target "<Your Target Name>" do
pod 'SkyhookContext'
end
```

Install using the following command:

```
$ pod install
```


# Documentation


Prerequisites
-------------

- Requires iOS 9.0+
- Requires a device with region monitoring support: iPhone 4+, iPad (Wi-Fi only) 3+, iPad (Wi-Fi+Cell) 2+, iPad mini, iPad Air, iPod Touch 5+

Installation
------------

### Using Cocoapods

1. Add the following dependency in your Podfile

````
  pod 'SkyhookContext', '~> 2.1.0'
````

3. Navigate to your project folder and execute the following command in terminal window

````
  $ pod update
````

### Using Framework

1. Clone [Skyhook Context](https://github.com/SkyhookWireless/skyhook-context-ios) cocoapod repo. 
2. Drag and drop the framework (SkyhookContext.framework) into your Xcode project. Make sure you check the "Add to targets" checkbox.
3. Add the following frameworks to your project:
    - CoreLocation
    - libsqlite3.0
    - MapKit
    - Security
    - SystemConfiguration
    - AddressBook
    - AddressBookUI
    - Ad (optional)
5. Add the following elements to Other Linker Flags section in your Build Settings:
    - -ObjC
    - -lc++
    - -all_load
6. In order to use the CoreLocation framework features, it is required to include
   both `NSLocationAlwaysAndWhenInUseUsageDescription` and `NSLocationWhenInUseUsageDescription` keys
   to the Info.plist of your app. The string value of each entry should describe purpose
   of the location access and will be shown to your users for permission. If you do not need
   campaign monitoring in your app, and only want venue info based on location, then
   `NSLocationWhenInUseUsageDescription` is sufficient.
   While it is possible to use `NSLocationWhenInUseUsageDescription` to request personas
   based on location only when the app is open, this setting will significantly reduce the number
   of location samples used to generate personas for the device.  To increase accuracy and reduce
   time to generation it is recommended to use `NSLocationAlwaysAndWhenInUseUsageDescription` whenever possible
   for apps leveraging Skyhook Personas.

   If you want the location access request dialog to be shown at a specific time controlled by you,
   you can call `requestWhenInUseAuthorization` or `requestAlwaysAuthorization` of CoreLocation
   location manager yourself, and pass `NO` to `setLocationPromptAllowed:` `SHXAccelerator` method,
   to disable these requests.

Initializing
------------

### Import the SDK

Import the Accelerator library where needed.

    #import <SkyhookContext/Accelerator.h>

### Instantiate the Accelerator Object

    SHXAccelerator *accelerator = [[SHXAccelerator alloc] initWithKey:@"your_application_key"];

### Handle Delegate Events

Designate your class as an *SHXAcceleratorDelegate* and assign this as a delegate to the Accelerator instance you instantiated earlier.

    // here 'self' is an instance of your class that implements SHXAcceleratorDelegate protocol.
    accelerator.delegate = self;

The Context Accelerator SDK is thread-safe, so its API methods can safely be called from any thread, but note that it will always call the methods of your delegate from the main thread regardless of which thread your app uses to call accelerator.

<a name="location_prompting"></a>Location Prompting
-------------------------------------------------
Since Accelerator scans for significant location changes for its geofencing and demographic functions, it has the potential to trigger the user-facing location permissions dialog box.

If you'd prefer to trigger this yourself during the normal flow of your application, you can prevent Accelerator from starting any location activities until you do so:

    [SHXAccelerator setLocationPromptAllowed:NO];

The default setting is *YES*. Note that if you disallow location prompts, geofencing will not work until you have triggered locations by yourself.


Skyhook Personas
----------------

You can request a refresh of the Skyhook persona of the current user by calling `refreshPersonaWithCompletion:`.

    [accelerator refreshPersonaWithCompletion:^(SHXPersona *persona, NSError *error)
    {
        if (error)
        {
            // handle error
            NSLog(@"error:%@", error);
            return;
        }

        // use persona
    }];

### Latest Persona

At any time you can access the Skyhook persona of the current user as represented by the `persona` property. If no persona is available, this property will be *nil*.

    SHXPersona *persona = accelerator.persona;

Note that for apps using the Ad Support framework the latest persona may be erased after a change in the Advertising settings on the device. You may want to request a persona refresh in this case.

The *SHXPersona* object has the following demographic properties:

- age
- gender
- ethnicity
- income
- education

Each of these properties is an *NSArray* object composed of *SHXDemographic* objects, listed in a decreasing order of probability.

For example, to list all the values for the current ethnicity persona:

    SHXPersona *persona = accelerator.persona;

    for (SHXDemographic *demographic in persona.ethnicity)
    {
        NSLog(@"value:%@ probability:%f variance:%f\n",
            demographic.value,
            demographic.probability,
            demographic.variance);
    }

The result of the above code would print something like this:

    value:white probability:0.65 variance: 0.1
    value:other probability:0.15 variance: 0.1
    value:black probability:0.12 variance: 0.1
    value:asian probability:0.08 variance: 0.1

Note that since the values are ordered by probability, you can simply grab the first element (index 0) from the list to get the most probable value for the current user.


### Behaviors

Similarly to the five demographic objects, *SHXPersona* also includes a *behaviors* property.

    SHXPersona *persona = accelerator.persona;

    for (SHXBehavior *behavior in persona.behaviors)
    {
        NSLog(@"ID:%@ name:%@\n",
            behavior.ident,
            behavior.name);
    }

The result of the above code would print something like this:

    ID:12341318394918 name:auto intenders
    ID:1234131839491234 name:auto enthusiasts


### Notifications

Anytime the persona is updated, *SHXAccelerator* will notify its delegate.

    - (void)accelerator:(SHXAccelerator *)accelerator
       didUpdatePersona:(SHXPersona *)persona;


### When does the Skyhook persona update?

The persona is generally updated whenever the Context Accelerator SDK is notified of a location change. If you're using the [campaign monitoring](#campaign_monitoring) feature of Accelerator, coarse location updates will happen automatically. You can enhance the accuracy by [supplying a location sourced by your app](#location_hinting) anytime.

If you are not using campaign monitoring, **you must** supply the location to Accelerator for demographic updates to work.

<a name="campaign_monitoring"></a>Campaign Monitoring
---------------------------------------------------


Typically you start monitoring for campaigns like this:

    [accelerator startMonitoringForAllCampaigns];

If more control is needed, you can instead start monitoring for individual campaigns:

    [accelerator startMonitoringForCampaign:@"YourCampaignName1"];
    [accelerator startMonitoringForCampaign:@"YourCampaignName2"];
    [accelerator startMonitoringForCampaign:@"YourCampaignName3"];

Once you start monitoring for campaign events, Accelerator will notify you via *SHXAcceleratorDelegate* callbacks for various campaign events you have designated on [My.Skyhook](http://my.skyhookwireless.com/).

The delegate receives the following callbacks when the user enters or exits the specified venues. Each callback contains an *SHXCampaignVenue* object whose properties describe the venue.

    - (void)accelerator:(SHXAccelerator *)accelerator
        didEnterVenue:(SHXCampaignVenue *)venue;

    - (void)accelerator:(SHXAccelerator *)accelerator
        didExitVenue:(SHXCampaignVenue *)venue;

The delegate will also notify you if an error has occurred.

    - (void)accelerator:(SHXAccelerator *)accelerator
    didFailWithError:(NSError *)error;

The *SHXCampaignVenue* object has the following properties:

Property Name | Property Type | Definition
--------------|---------------|-----------
venueIdent    | NSNumber      | venue identifier
campaignName  | NSString      | campaign name
timestamp     | NSDate        | time when event was detected
customData    | NSString      | custom data specified at My.Skyhook (will be nil if nothing set there)
eventType     | enum          | event type - near (SHXCampaignVenueEventTypeNear) or inside (SHXCampaignVenueEventTypeInside)

<br />
When monitoring for individual campaigns, you can stop monitoring ones that are no longer needed:

    [accelerator stopMonitoringForCampaign:@"YourCampaignName2"];

**Note that if all campaigns are being monitored, then calling stopMonitoringForCampaign has no effect. It will not stop the specified campaign from being monitored.**

Or you can stop monitoring for all campaigns, whether individual campaigns or all of them are being monitored:

    [accelerator stopMonitoringForAllCampaigns];

Your app can fetch the list of recent campaign visits by calling the `fetchRecentCampaignVisitsWithLimit:completion:` method.

    [accelerator fetchRecentCampaignVisitsWithLimit:100
                                         completion:^(NSArray *visits, NSError *error)
    {
        if (error)
        {
            // handle error
            NSLog(@"error:%@", error);
            return;
        }

        // use recent campaign visits
    }];

<a name="location_hinting"></a>Location Hinting
---------------------------------------------

If your application includes location, you can assist Accelerator by passing it the current location whenever it is received by your application.

    // this is the CLLocationManagerDelegate callback your app would typically recieve
    - (void)locationManager:(CLLocationManager *)manager didUpdateLocations:(NSArray *)locations
    {
        // extract latest location and pass it to the accelerator object
        CLLocation *latestLocation = [locations lastObject];

        // pass the location to Accelerator
        [accelerator didUpdateToLocation:latestLocation];
    }

IP Location
-----------

The Context Accelerator SDK can also be used to give you on-demand IP location using the requesting remote IP.

    [accelerator requestIPLocationWithCompletion:^(SHXIPLocation *location, NSError *error)
    {
        // use returned location or handle error
        if (!error)
        {
            CLLocationCoordinate2D coordinate = location.coordinate;
            MKPlacemark *placemark = [location createMKPlacemark];
            SHXIPLocationType type = location.locationType;
            SHXAccuracyIndicator accuracy = location.accuracyIndicator;
        }
        else
        {
            NSLog(@"error:%@", error);
        }
    }];

If successful, the block will return an *SHXIPLocation* object. The object is a **direct subclass of CLLocation** and although properties other than *coordinate* are not set, you may still use all built-in CoreLocation functions that deal with *CLLocation* objects. To get an *MKPlacemark* for this location, use the *createMKPlacemark* method.

Additional *SHXIPLocation* properties are as follows:


### SHXIPLocation

Property Name      | Property Type                                            | Definition
-------------------|----------------------------------------------------------|-----------
coordinate         | CLLocationCoordinate2D                                   | latitude,longitude
accuracyIndicator  | [SHXAccuracyIndicator](#shx_location_accuracy_indicator) | horizontal accuracy indicator
locationType       | [SHXIPLocationType](#shx_ip_location_type)               | type of IP


#### SHXIPLocationType

Value                    | Definiton
-------------------------|----------
SHXIPLocationTypeFixed   | fixed IP address
SHXIPLocationTypeMobile  | mobile IP address
SHXIPLocationTypeUnknown | unable to resolve type of IP


Venue Information
-----------------

The Context Accelerator SDK provides a collection of methods for obtaining venue information related to your campaigns and location.


### Nearby Monitored Venues

The `fetchNearbyMonitoredVenuesWithLimit:completion:` method allows the client to obtain the unique identifiers of nearby venues that are part of actively monitored campaigns. This method can be used in conjunction with the `fetchInfoForVenues:completion:` method to obtain more detailed venue information.


### Venue Information by unique identifer

The `fetchInfoForVenues:completion:` method allows the client to obtain more detailed venue information using the unqiue venue identifiers from the `SHXCampaignVenue` and `SHXNearbyCampaignVenue` objects.

    [accelerator fetchNearbyMonitoredVenuesWithLimit:100
                                          completion:^(NSArray *venues, NSError *error)
    {
        if (error)
        {
            // handle error
            NSLog(@"error:%@", error);
            return;
        }

        NSMutableArray *ids = [NSMutableArray arrayWithCapacity:[venues count]];
        for (SHXNearbyCampaignVenue *venue in venues)
        {
            [ids addObject:venue.venueIdent];
        }

        [accelerator fetchInfoForVenues:ids
                             completion:^(NSArray *venueInfo, NSError *error)
        {
            if (error)
            {
                // handle error
                NSLog(@"error:%@", error);
                return;
            }

            // use venue information
        }];
    }];


### Venue Information at the current location

The `fetchVenueInfoAtLocationWithCompletion:` method allows the client to request the venue information at the current user location.

    [accelerator fetchVenueInfoAtLocationWithCompletion:^(NSArray *venues, NSError *error)
    {
        if (error)
        {
            // handle error
            NSLog(@"error:%@", error);
            return;
        }

        // use venue information
    }];


### Campaign list

The `fetchCampaignsWithCompletion:` method allows the client to request the a list of campaigns available on server.

    [accelerator fetchCampaignsWithCompletion:^(NSArray<SHXCampaign *> *campaigns, NSError *error)
    {
        if (error)
        {
            // handle error
            NSLog(@"error:%@", error);
            return;
        }

        // use campaigns information
    }];


Privacy Considerations
----------------------

For apps using the iOS Ad Support framework, the Context Accelerator SDK will collect certain usage information in order to improve the quality of Skyhook's positioning and context products. For a detailed overview of the data that the Skyhook Context Accelerator SDK will collect and use, please read our Privacy Policy, available at http://www.skyhookwireless.com/privacy-policy/skyhook

To leverage user-based persona information, the Accelerator SDK needs an anonymous identifier. By default it will use IDFA. You can override the identifier by assigning custom identifier to Accelerator's userID property.

    SHXAccelerator *accelerator;
    ...
    accelerator.userID = @"some-valid-and-unique-user-id”;

In addition to Skyhook's default privacy protections, developers integrating the Context Accelerator SDK may also allow application users to opt out of usage data collection. Note that the application may receive degraded context information if data collection is disabled. To disable usage data collection:

    SHXAccelerator *accelerator;
    ...
    accelerator.optedIn = NO;

You can check the data collection status anytime with

    BOOL isOptedIn = accelerator.isOptedIn;

or re-enable it with

    accelerator.optedIn = YES;

Note that by default Accelerator SDK follows iOS Ad Support Framework settings: data collection will be enabled only if the Ad Support Framework is present and the user did not enable the "Limit Ad Tracking" option.

If your application is not using the iOS Ad Support Framework but the application and/or content provider has received an EXPLICIT opt in from the user to leverage a unique device identifier for Geofence logging and personification, developers must set the userID and set the opt-in to yes to enable data collection.

    accelerator.userID = @"some-valid-and-unique-user-id”;
    ...
    accelerator.optedIn = YES;

If either of these conditions are not met, the default behavior of the SDK is to treat the user as opted out and disable data collection on the Server.

