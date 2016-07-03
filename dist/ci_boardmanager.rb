#!/usr/bin/ruby
require 'json'
require 'digest'
require 'optparse'
require 'open-uri'

template = ''
jsonfile = ''
pkg_url= ''
release = ''
repo_url = ''
force = false

opt = OptionParser.new
opt.on('-t FILE', '--template=FILE') {|o| template = o }
opt.on('-j FILE', '--json=FILE') {|o| jsonfile = o }
opt.on('-u PACKAGE_URL', '--url=PACKAGE_URL') {|o| pkg_url= o }
opt.on('-r RELEASE', '--release=RELEASE') {|o| release = o }
opt.on('-g GH_REPO_URL', '--gh-repo=GH_REPO_URL') {|o| repo_url = o }
opt.on('-f', '--force') {|o| force = o }
opt.parse!(ARGV)

slug = repo_url.sub(/https:\/\/github.com\//,'').sub(/\.git$/,'')
user,repo = slug.split('/')
ghpage_url = "https://#{user}.github.io/#{repo}/#{jsonfile}"

suffix = nil
suffix = ".tar.bz2" if pkg_url.end_with?(".tar.bz2")
suffix = ".tar.gz" if pkg_url.end_with?(".tar.gz")

raise "invalid archive format #{pkg_url}." if suffix == nil

STDERR.puts("ghpage_url: #{ghpage_url}\n")
STDERR.puts("  repo_url: #{repo_url}\n")
STDERR.puts("   pkg_url: #{pkg_url}\n")

entry = open(template) {|j| JSON.load(j) }

bmdata = JSON.load('{ "packages": [ { "platforms": [], "tools": [] }  ] }')
begin
  bmdata = open(ghpage_url) {|f| JSON.load(f) }
rescue => e
  bmdata['packages'][0]['name'] = 'defaultname'
  bmdata['packages'][0]['maintainer'] = 'defaultmaintainer'
  bmdata['packages'][0]['websiteURL'] = 'http://example.com'
  bmdata['packages'][0]['email'] = 'default@example.com'
  STDERR.puts(e)
  raise e if not force
end


pkgs = bmdata['packages'][0]["platforms"]

raise if pkgs.find {|x| x["version"] == release} != nil

open(pkg_url) do |ff|
  entry["url"] = pkg_url
  entry["version"] = release.sub(/^.*-/, '')
  entry["archiveFileName"] = repo + '-' + release + suffix
  entry["checksum"] =  "SHA-256:" + Digest::SHA256.hexdigest(ff.read)
  entry["size"] =  "#{ff.size}"
  pkgs.unshift(entry)
end

newjson = JSON.pretty_generate(bmdata)
STDOUT.puts(newjson)
