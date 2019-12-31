require 'fileutils'

def publish_dir(src, dst)
    if Dir.exists?(dst)
        FileUtils.rm_r(dst)
    end
    FileUtils.cp_r(src, dst)
end

publish_dir('./appdata/data', './bin/data')