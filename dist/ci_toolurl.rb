#!/usr/bin/ruby

require 'json'

open(ARGV[0]) do |f|
  js = JSON.load(f)
  es = js.select{|e| e["name"] == "contiki-makehelper" }
  e= es[0]
  url = e['systems'][0]['url']
  print url.gsub("${NAME}", e["name"]).gsub("${VERSION}", e["version"]).gsub("${ARCH}", "x86_64-pc-linux-gnu")
end

