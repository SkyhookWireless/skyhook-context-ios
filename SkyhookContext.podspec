Pod::Spec.new do |s|
  s.name                  = 'SkyhookContext'
  s.version               = '2.0.7'
  s.summary               = 'Skyhook Context SDK'
  s.homepage              = 'http://www.skyhook.com'
  s.author                = { "Alex Pavlov" => "alexander.pavlov@skyhook.com" }
  s.platform              = :ios
  s.ios.deployment_target = '8.0'
  s.source                = { :git => 'https://github.com/SkyhookWireless/skyhook-context-ios.git', :tag => "#{s.version}" }
  s.source_files          = 'SDK/SkyhookContext.framework/Headers/*.h'
  s.vendored_frameworks   = 'SDK/SkyhookContext.framework'
  s.frameworks            = "CoreLocation", "MapKit", "Security", "SystemConfiguration", "AddressBook", "AddressBookUI"
  s.libraries             = "sqlite3", "c++"
  s.requires_arc          = true
  s.license               = { :type => 'Skyhook License',
                              :text => 'Copyright (c) 2005 - 2017 Skyhook, Inc. All rights reserved. https://my.skyhookwireless.com/termsofservice' }
end
