/*
 * Copyright (C) 2013-2015 Skyhook Wireless, Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted subject to the following:
 *
 * * Use and redistribution is subject to the Software License and Development
 * Agreement, available at
 * <a href="http://www.skyhookwireless.com">www.skyhookwireless.com</a>
 *
 * * Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#import "SHXAccelerator.h"

/**
 * Campaign Venue
 *
 * Note that the `customData` property is optional. It will be nil if no data
 * is set on My.Skyhook for this campaign.
 */

typedef NS_ENUM(NSInteger, SHXCampaignVenueEventType) {
    SHXCampaignVenueEventTypeNear,
    SHXCampaignVenueEventTypeInside
};

@interface SHXCampaignVenue : NSObject <NSCoding>

@property (readonly, nonatomic) NSNumber * _Nonnull venueIdent;
@property (readonly, nonatomic) NSString * _Nonnull campaignName;
@property (readonly, nonatomic) NSDate * _Nonnull timestamp;
@property (readonly, nonatomic) NSString * _Nullable customData;
@property (readonly, nonatomic) SHXCampaignVenueEventType eventType;


- (id _Nonnull )initWithVenue:(NSNumber *_Nonnull)venueIdent
           campaign:(NSString *_Nonnull)campaignName
          timestamp:(NSDate *_Nonnull)timestamp
         customData:(NSString *_Nullable)customData
          eventType:(SHXCampaignVenueEventType)eventType;

- (id _Nonnull )initWithCoder:(NSCoder *_Nonnull)coder;
- (void)encodeWithCoder:(NSCoder *_Nonnull)coder;

/**
 * These compare venueIdent and campaignName only
 */
- (BOOL)isEqualToCampaignVenue:(SHXCampaignVenue *_Nullable)other;
- (BOOL)isEqual:(id _Nullable )other;
- (NSUInteger)hash;

- (NSString *_Nonnull)description;

@end
