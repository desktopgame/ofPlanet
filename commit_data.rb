require 'fileutils'

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

publish_dir('./appdata/data', './bin/data')
publish_file('./appdata/blocks.json', './bin/blocks.json')
publish_file('./appdata/textures.json', './bin/textures.json')