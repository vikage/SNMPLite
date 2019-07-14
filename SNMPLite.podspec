Pod::Spec.new do |s|
  s.name     = 'SNMPLite'
  s.version  = '1.0.2'
  s.ios.deployment_target = '9'
  s.license  =  { :type => 'MIT', :file => 'LICENSE' }
  s.summary  = 'SNMP library to get SNMP oid'
  s.homepage = 'https://github.com/Thanhdev2703/SNMPLite'
  s.authors   = { 'Thanh Vu' => 'vuvanthanh.haui.ktpm1@gmail.com'}
  s.source   = { :git => 'https://github.com/Thanhdev2703/SNMPLite.git', :tag => s.version.to_s }
  s.description = 'SNMP library to get SNMP oid'
  s.source_files = 'SNMPLite/*.{h,m,cpp,mm}', 'SNMPLite/public/*.h'
  s.public_header_files = 'SNMPLite/public/*.h'
  s.requires_arc = true
end
