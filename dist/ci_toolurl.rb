#!/usr/bin/ruby

require 'json'
require 'optparse'

version = 'XXXXX'
name = 'xxx'

opt = OptionParser.new
opt.on('-v SET_VERSION', '--version=SET_VERSION') {|o| version = o }
opt.on('-n TOOL_NAME', '--name=TOOL_NAME') {|o| name= o }
argv = opt.parse(ARGV)

open(argv[0]) do |f|
  js = JSON.load(f)
  es = js.select{|e| e["name"] == name }
  e= es[0]
  url = e['systems'][0]['url']
  print url.gsub("${NAME}", e["name"]).gsub("${VERSION}", version).gsub("${ARCH}", "x86_64-pc-linux-gnu")
end

