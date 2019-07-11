Pod::Spec.new do |s|
  s.name                  = 'SkyhookContext'
  s.version               = '2.1.5'
  s.summary               = 'Skyhook Context SDK'
  s.homepage              = 'http://www.skyhook.com'
  s.author                = { "Skyhook Wireless, Inc." => "iosdev@skyhook.com" }
  s.platform              = :ios
  s.ios.deployment_target = '9.0'
  s.source                = { :git => 'https://github.com/SkyhookWireless/skyhook-context-ios.git', :tag => "#{s.version}" }
  s.source_files          = 'SDK/SkyhookContext.framework/Headers/*.h'
  s.vendored_frameworks   = 'SDK/SkyhookContext.framework'
  s.frameworks            = "CoreLocation", "MapKit", "Security", "SystemConfiguration", "AddressBook", "AddressBookUI"
  s.libraries             = "sqlite3", "c++"
  s.requires_arc          = true
  s.license               = { :type => 'Skyhook License',
                              :text => 'Copyright (c) 2005 - 2018 Skyhook Wireless, Inc. All rights reserved. https://my.skyhookwireless.com/termsofservice' }
end
