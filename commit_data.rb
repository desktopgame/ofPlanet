require 'fileutils'
im_path = './appdata/data'
ex_path = './bin/data'

if Dir.exists?(ex_path)
    FileUtils.rm_r(ex_path)
end
FileUtils.cp_r(im_path, ex_path)