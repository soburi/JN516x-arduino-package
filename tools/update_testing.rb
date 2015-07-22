require 'json'
require 'ostruct'
require 'digest'

TAG='testing'
JSONFILE='package_soburi_jn516x-testing_index.json'

TOOLS=['ba-elf-ba2-gcc', 'jenprog']

GHURL='https://github.com/soburi/JN516x-arduino-package'

tool_vers = {}

def fetch_file(url)
  filename = url
  filename =~ /^.*\/([^[\/|\?]]*).*$/
  filename = $1

  if !File.exists?(filename)
    system("curl -L #{url} > #{filename}")
  end
  if File.exists?(filename)
    return filename
  else
    raise
  end
end


tags = `git tag | grep #{TAG}`.split(/\s/)

tags.each do |tt|
  if !File.exists?("#{tt}.tar.gz")
    system("curl -L #{GHURL}/archive/#{tt}.tar.gz > #{tt}.tar.gz")
  end
end

repos = `git tag | grep repository | sort | tail -1`.chomp
lstree = `git ls-tree --name-only #{repos}`.split(/\s/)

newtools = TOOLS.collect do |tool|
  #detect version by windows archive.
  t4win = lstree.find {|l| l =~ /#{tool}_i686-mingw32_([^_]*)\.tar\.bz2/ }
  unless t4win
    STDERR.puts "tool not found"
    raise
  end
  
  version = $1
  tool_vers[tool] = version

  t4mac = lstree.find {|l| l =~ /#{tool}_x86_64-apple-darwin_#{version}\.tar\.bz2/ }
  unless t4mac
    STDERR.puts "tool not found"
    raise
  end
  t4lnx = lstree.find {|l| l =~ /#{tool}_x86_64-pc-linux-gnu_#{version}\.tar\.bz2/ }
  unless t4lnx
    STDERR.puts "tool not found"
    raise
  end

  toolinfo = { "i686-mingw32"=>t4win, "x86_64-apple-darwin"=>t4mac, "x86_64-pc-linux-gnu"=>t4lnx }

  entry = {}
  entry["name"] = tool
  entry["version"] = version
  entry["systems"] = toolinfo.keys.collect do |host|
    toolent = {}
    toolent["host"] = host
    toolent["archiveFileName"] = toolinfo[host]
    url = "#{GHURL}/blob/#{repos}/#{toolinfo[host]}?raw=true"
    toolent["url"] = url
    
    filename = fetch_file(url)

    toolent["checksum"] = "SHA-256:" + Digest::SHA256.hexdigest(File.binread(filename) )
    toolent["size"] = File.size(filename).to_s
    toolent
  end
  entry
end

newjson = ""

open(JSONFILE) do |f|
  bmdata = JSON.load(f)

  packages = bmdata['packages']
  root = packages[0]

  root["name"] = "jn516x"
	root["maintainer"] = "soburi"
	root["websiteURL"] = GHURL
	root["email"] = "soburi@gmail.com"

  pkgs = root["platforms"]

  tags.each do |tt|
    entry = {}
    entry["name"] = "JN516x Boards"
		entry["architecture"] =  "jn516x"
		entry["category"] =  "Contributed"
		entry["version"] =  tt.gsub(/^#{TAG}-/, '')
    url = "#{GHURL}/archive/#{tt}.tar.gz"
		entry["url"] = url

    filename =fetch_file(url)

		entry["archiveFileName"] = filename
		entry["checksum"] =  "SHA-256:" + Digest::SHA256.hexdigest(filename)
		entry["size"] =  File.size(filename).to_s
    entry["boards"] =
      [ {"name"=>"Generic JN516x board"},
        {"name"=>"TOCOS TWE Lite"},
        {"name"=>"SeeedStudio Mesh Bee"} ]
    entry["toolsDependencies"] = TOOLS.collect do |tool|
      toolent = {}
      toolent["packager"] = "jn516x"
      toolent["name"] = tool
      toolent["version"] = tool_vers[tool].to_s
      toolent
    end

    if pkgs.find {|x| x["version"] == tt} == nil
      pkgs.push(entry)
    end
  end

  tools = root['tools']
  newtools.each do |nt|
    oldtools = tools.find_all {|t| t["name"] == nt["name"]}
    unless oldtools.find{|tt| tt["version"] == nt["version"] }
      tools.push(nt)
    end
  end

  newjson = JSON.pretty_generate(bmdata)

end

File.write(JSONFILE, newjson);
