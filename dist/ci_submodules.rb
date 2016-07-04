#!/usr/bin/ruby

require 'json'
require 'optparse'
require 'rugged'
require 'pp'

comparefile = nil
outfile = nil

opt = OptionParser.new
opt.on('-c FILE', '--compare=FILE') {|o| comparefile = o }
opt.on('-o FILE', '--output=FILE') {|o| outfile = o }
opt.parse!(ARGV)

repo = Rugged::Repository.new('.')

def submodules_paths_recursive(repo, coll=nil, path="")
  coll = [] if coll == nil
  repo.submodules.each do |sm|
    coll.push(
      {
        :path=>path+sm.path,
        :url=>sm.repository.remotes['origin'].url,
        :hash=>sm.head_oid
      }
    )
    submodules_paths_recursive(sm.repository, coll, sm.path + '/')
  end
  coll
end

submods = submodules_paths_recursive(repo)

modlines = submods.collect do |sub|
  "{ \"path\":\"" + sub[:path] + "\", \"url\":\"" + sub[:url] + "\", \"revision\":\"" + sub[:hash] + "\" }"
end

jsonstr = "[" + modlines.join(",") + "]"

jsobj = JSON.parse(jsonstr)

if comparefile != nil
  compare = open(comparefile) {|f| JSON.load(f) }
  if (jsobj == compare)
    STDERR.puts('identical')
    exit 0
  else
    STDERR.puts('differ')
    exit -1
  end
end

if outfile != nil
  open(outfile) do |f|
    f.write(JSON.pretty_unparse(jsobj) )
  end
  exit 0
end

print JSON.pretty_unparse(jsobj)
