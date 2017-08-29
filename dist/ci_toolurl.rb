#!/usr/bin/ruby

require 'json'
require 'optparse'

version = 'XXXXX'

opt = OptionParser.new
opt.on('-v SET_VERSION', '--version=SET_VERSION') {|o| version = o }
argv = opt.parse(ARGV)

open(argv[0]) do |f|
  js = JSON.load(f)
  es = js.select{|e| e["name"] == "contiki-makehelper" }
  e= es[0]
  url = e['systems'][0]['url']
  print url.gsub("${NAME}", e["name"]).gsub("${VERSION}", version).gsub("${ARCH}", "x86_64-pc-linux-gnu")
end

