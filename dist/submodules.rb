#!/usr/bin/ruby

require 'json'

modpathstr=`git submodule status --recursive | cut -d' ' -f 3,3`

modpaths=modpathstr.chomp.split("\n")
modurls=modpaths.collect do |m|
  `git -C #{Dir.pwd}/#{m} config --list | grep remote.origin.url= | sed 's/remote.origin.url=//' | sed 's/.git$//'`.chomp
end

modrevs=modpaths.collect do |m|
  `git -C #{Dir.pwd}/#{m} rev-parse HEAD`.chomp
end

modinfos=modpaths.zip(modurls, modrevs)

MODPATH=0
MODURL=1
MODREV=2

modlines = modinfos.collect do |mod|
    "{ \"path\":\"" + mod[MODPATH] + "\", \"url\":\"" + mod[MODURL] + "\", \"revision\":\"" + mod[MODREV] + "\" }"
end

jsonstr = "[" + modlines.join(",") + "]"

jsobj = JSON.parse(jsonstr)

print JSON.pretty_unparse(jsobj)
