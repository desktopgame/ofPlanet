require 'fileutils'
require 'date'

def publish_dir(src, dst)
    if Dir.exists?(dst)
        FileUtils.rm_r(dst)
    end
    FileUtils.cp_r(src, dst)
end

def publish_file(src, dst)
    if Dir.exists?(dst)
        FileUtils.rm(dst)
    end
    FileUtils.cp(src, dst)
end

def mkdir_ifnex(path)
    if !Dir.exists?(path)
        Dir.mkdir(path)
    end
end
# create release dir
exportDirParent = File.expand_path("..", Dir.pwd)
exportDirParent = File.join(exportDirParent, 'ofPlanet_releases')
mkdir_ifnex(exportDirParent)

dateTime = DateTime.now
exportDir = File.join(exportDirParent, sprintf('release_%d_%d%d_%d%d%d', dateTime.year, dateTime.month, dateTime.day, dateTime.hour, dateTime.minute, dateTime.second))
mkdir_ifnex(exportDir)

exportRuntimeDir = File.join(exportDir, 'Runtime')
mkdir_ifnex(exportRuntimeDir)

puts exportDir

# update enviroment
publish_dir('./appdata/data', './bin/data')
# publish_file('./appdata/blocks.json', './bin/blocks.json')
# publish_file('./appdata/textures.json', './bin/textures.json')
# create release dir
publish_dir('./bin/data', File.join(exportRuntimeDir, 'data'))
publish_file('./bin/ofPlanet.exe', File.join(exportRuntimeDir, 'ofPlanet.exe'))
Dir.open('./bin') do |dir|
    dir.each do |file|
        if file.end_with?(".lib") || file.end_with?(".dll") || file == 'blocks.json' || file == 'textures.json'
            path = File.join('./bin/', file)
            publish_file(path, File.join(exportRuntimeDir, file))
        end
    end
end
Dir.open('.') do |dir|
    dir.each do |file|
        if file.end_with?(".md") || file.end_with?(".png")
            path = File.join('.', file)
            publish_file(path, File.join(exportDir, file))
        end
    end
end