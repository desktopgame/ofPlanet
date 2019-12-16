# C++であとからファイルを名前空間に含めたいときに使います。
# 既存のコードを namespace hoge { ... } のように上書きします。
# 必要に応じて planet のところを書き換えてください。

# ソースコードと同じディレクトリで使います。
# ruby ns.rb

def add_namespace(path)
    txt = File.open(path, "r") {|fp| fp.read }
    # 最後の #include を探す
    beginPos = txt.rindex("#include")
    # ないならインクルードガード直後
    if beginPos == nil
        beginPos = txt.rindex("#define")
    end
    # その次の行まで移動
    while beginPos < txt.length
        ch = txt[beginPos]
        if ch == "\n"
            beginPos += 1
            break
        end
        beginPos += 1
    end
    if beginPos != nil
        txt = txt.insert(beginPos, "namespace planet {\n");
    end
    # 最後の #endif を探す
    endPos = txt.rindex("#endif")
    if endPos == nil
        endPos = txt.length
    end
    if endPos != nil
        txt = txt.insert(endPos, "}\n");
    end
    if beginPos == nil || endPos == nil
        puts(sprintf("error: %s", path))
        return
    end
    File.open(path, "w") {|fp| fp.write(txt) }
end

Dir.glob("**/*") do |file|
    path = Dir.pwd + "/" + file
    next if !path.end_with?(".cpp") && !path.end_with?(".hpp")
    beginPos = nil
    endPos = nil
    if path.end_with?(".hpp")
        add_namespace(path)
    else path.end_with?(".cpp")
        add_namespace(path)
    end
    puts path
end