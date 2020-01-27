# ofPlanetユーザーマニュアル
ofPlanetは地形生成エディターです。  
※画像はofPlanetで生成したモデルをUnityで表示した様子
![Unityで生成したモデルのプレビュー](ss_unity.png)

# 動作環境
このアプリケーションはVisualStudio2017を用いて実装されました。  
[VisualStudio2017再頒布パッケージ](https://support.microsoft.com/ja-jp/help/2977003/the-latest-supported-visual-c-downloads)が必要です。

## 起動
`Runtime/ofPlanet.exe`をダブルクリックして起動します。  
Runtime/dataフォルダには `block.json` `textures.json` が必要です。  
他にもブロックのための画像が必要ですが、これはdata配下の自由なディレクトリに配置できます。  
※ただし、サブディレクトリを作ることはできません。全て直下に配置します。

このアプリには最初から `Runtime/data/blocks.json` `Runtime/data/textures.json` そしてテクスチャが `Runtime/data/block` に格納されているので、  
何もしなくても起動することができるはずです。

## 画面
起動すると、以下のような画面が表示されるはずです。
![最初の画面](ss_default.png)

### Setting
ワールドの基本的な設定を定義します。  
![設定画面](ss_setting.png)

#### Size
生成するワールドのサイズを設定出来ます。
#### Biome
生成するワールドの種類を決めます。  
これは `Runtime/data/script` の中に配置されたLuaスクリプトで実装されており、
ユーザーが自由に追加可能です。
#### CameraSpeed
プレビュー時のカメラ速度です。
#### PlayMode
ONにすると、WASDと矢印キーでワールドを移動出来ます。

### Parameter
これはUnityにおける`SerializedField`と同じような働きをします。  
現在選択されているLuaスクリプトのグローバル変数を全て表示します。  
![設定画面](ss_parameter.png)

例えば、以下のスクリプトでは最初の四行で変数を宣言し、初期値を与えています。  
この変数を編集するための適切なエディターを表示しているのが上記の画像です。  
選択中のスクリプトが切り替われば自動で切り替わります。
````
topBlock = "GrassDirt"
fillBlockDeep = "Stone"
fillBlockShallow = "Dirt"
iBaseline = 0

function start()
    return "default"
-- return "ignore"
end

function onFixHeight(y)
    if(y < iBaseline) then
        return iBaseline;
    end
    return y;
end

function onGenerateTerrain(x, y, z)
    startY = y
    setblock(x, y, z, topBlock);
    while y > 0 do
        y = y - 1
        if y < (startY - 5) then
            setblock(x, y, z, fillBlockDeep)
        else
            setblock(x, y, z, fillBlockShallow)
        end
    end
end

function onGenerateWater(x, y, z)
    setblock(x, y, z, "Water");
end

function onGenerateStructures()
end

function onGenerateCave(x, y, z, noise)
    if(noise > 0.1) then
        setblock(x, y, z, "");
    end
end

function onPostGenerate()
    print("done.")
end
````

## textures.json
textures.jsonは例えば以下のようなファイルです。
````
{
  "baseDirectory": "image/block",
  "textures": [
    {
      "baseFileName": "DirtBlock",
      "mappingRule": {
        "all": "Side"
      },
      "reference": "DirtBlock"
    }
    以下略...
}
````
### baseDirectory
テクスチャが格納されるディレクトリを指定します。
### textures
この配下にテクスチャ定義を並べます
### baseFileName
テクスチャのベースとなるファイル名です。  
ブロックは6個の面から構成されますが、  それぞれのファイルに共通する部分をここに定義します。  
例えば、6個の面を構成するテクスチャが
* `Dirt_Top.png`
* `Dirt_Bottom.png`
* `Dirt_Left.png`
* `Dirt_Right.png`
* `Dirt_Front.png`
* `Dirt_Back.png`

であるとき、共通部分は `Dirt_` です。  
これに後述する `mappingRule` をつけたすことで面ごとのテクスチャのファイル名を解決します。

### mappingRule
マッピングルールはある面に対応するファイルの名前です。  
キーには `"all" "top" "bottom" "left" "right" "front" "back"` が使えます。  
ただし、6個の面全てが埋められていれば全てのキーを使う必要はありません。  
例えば、ベース名が `"baseFileName": "Dirt"` のように定義されていて、  
マッピングルールが `"all": "Side"` のように定義されているなら、
全ての面に `DirtSide.png` が適用されることになります。  
これは6個の面のうちいくつかの面のテクスチャが使い回されるときに便利です。  

また、`"all"`と他の面が同時に定義されている場合、  
一度 `"all"` で全ての面を解決したあとで、定義されている他の面を使ってもう一度テクスチャのファイルを解決します。(上書き)  
なので、部分的にテクスチャを使い回すことも可能です。

### reference
リファレンスは、このテクスチャのセット自体に一意な名前をつけるために必要です。  
`blocks.json`からテクスチャを指定するときはこの名前を使用します。

## blocks.json
blocks.jsonは例えば以下のようなファイルです。
````
{
  "blocks": [
    {
      "reference": "Dirt",
      "texture": "DirtBlock"
    },
    以下略...
}
````

### reference
このブロックを参照するための名前を定義します。  
これはLuaスクリプト内部から使用されます。(後述)

### texture
対応するテクスチャを定義します。
上述の `textures.json` の `reference` に定義した名前を使用します。

### textures.json & blocks.json
この2つのファイルが間接参照を多用したデータ構造になっているのは意図的なものです。  
テクスチャのみ/ブロックのみをあとから簡単に差し替えることが可能です。

### Exporter
生成した地形を任意の他のプログラムで利用するために、  
エクスポートするためのウィンドウです。  
JSON/OBJ/BMP に対応しています。

![エクスポート画面](ss_export.png)

# 開発者マニュアル
## ビルド
このプロジェクトのビルドにはopenframeworksが必要です。  
ディレクトリは以下の場所である必要があります。
````
your_dir/of_v0.10.1_vs2017_release/apps/myApps/ofPlanet
````

また、アドオンとして
* [ofxLua](https://github.com/desktopgame/ofxLua)
* [ofxSOIL](https://github.com/desktopgame/ofxSOIL)

が必要です。現在ではアドオンはWindowsにしか対応していないので
このアプリもWindowsでしか動作しません。

## Luaスクリプト
原則として、luaスクリプトは全て `Runtime/data/script` に配置します。  
※ `ofPlanet.exe` が `Runtime` にあるとする

[LUA Manual](https://www.lua.org/manual/5.3/)

### コールバック関数
内部的には、C++からLuaで定義された各種コールバック関数を呼び出すことで地形を生成します。  
パーリンノイズ生成などの定型的な処理は全てC++側で処理されます。  
LuaでやるべきことはC++から受け取ったノイズ(実数型)を使用して地形を生成することと、  
必要に応じて任意の構造物などを生成することです。  

#### start -> mode
スクリプトの初期化処理をここに記述します。      
最初に一度だけ呼び出されます。

また、戻り値として次のどちらかを返す必要があります。
初期化処理のあと、どのコールバックを呼び出すべきかを指定します。
* "default"
    * 全てのコールバックを呼び出します。
* "ignore"
    * onPostGenerateだけを呼び出します。

#### onFixHeight(y) -> y
y: -1 ~ 1 で表される2Dノイズ  
パーリンノイズ2Dで生成されたy座標を必要ならフィルタできます。

#### onGenerateTerrain(x, y, z) -> void
x, y, z: 生成先の座標  
地表にブロックを配置します。

#### onGenerateWater(x, y, z) -> void
x, y, z: 生成先の座標  
水を生成します。

#### onGenerateStructures() -> void
構造物を生成します。

#### onGenerateCave(x, y, z, noise) -> void
x, y, z: 生成先の座標  
noise: -1 ~ 1 で表される3Dノイズ  
洞窟を生成します。

#### onPostGenerate -> void
一番最後に呼び出されるコールバックです。

### 関数

#### setblock(x, y, z, name)
指定座標のブロックを設定します  
nameが空文字列""のとき、指定位置のブロックを削除します

#### getblock(x, y, z) -> name
指定座標のブロックを返します

#### getxsize() -> xsize
ワールドのX方向のサイズを返します

#### getysize() -> ysize
ワールドのY方向のサイズを返します

#### getzsize() -> zsize
ワールドのZ方向のサイズを返します

#### newstruct(name, format)
指定の形状の構造物を文字列nameに紐づけて登録します。  
以下に例を示します。

````
function start()
    newstruct("AAA",[[
,,
,Wood,
,,

,,
,Wood,
,,

,,
,Wood,
,,

,,
,Wood,
,,

Leaf,Leaf,Leaf
Leaf,Leaf,Leaf
Leaf,Leaf,Leaf

Leaf,Leaf,Leaf
Leaf,Leaf,Leaf
Leaf,Leaf,Leaf

Leaf,Leaf,Leaf
Leaf,Leaf,Leaf
Leaf,Leaf,Leaf
]]) 
    return "default"
-- return "ignore"
end
````

※木を構造物として登録するサンプル  
１つの文字列の中に複数のCSVを格納するようなイメージです。空行がCSV間の区切りとなります。  
下に定義されたものほど実際に構造物を生成したときに上の方(+Y)に生成されます。  
また、指定されるブロック名は `blocks.json` の `reference` と同じ名前である必要があります。  
なにもない場合は `,,` のように隙間をあけずにカンマを打てばOKです。

#### genstruct(name)
まだ実験段階です。  
地上に指定の名前の構造物を生成します。

#### expandstruct(x, y, z, name)
指定の座標に構造物を配置します。

### その他

#### ブロックを指定する方法
`blocks.json`に定義されているブロックの `reference` 属性と同じ名前を使用してください。

#### 数値型
Luaには実数型と整数型の区別がありません。  
しかし、Luaのある変数が整数(integer)として使用されることが想定されているなら、  
`ofPlanet`はその変数に対して小数部分を編集できないエディターを提供するべきです。  
この区別をつけるため、`ofPlanet`は変数名にプレフィックス`i`のついた変数を特別扱いします。  
`i`が付く場合には小数部分を編集できないスライダーUIを提供し、それ以外の場合では小数部分まで編集できてしまうスライダーUIを提供します。

## JSONフォーマット
出力して得られるJSONファイルの形式
````
{
    "cell": [
        {
            "block": "Stone",
            "x": 0,
            "y": 0,
            "z": 0,
        }
        このような定義がブロックの数だけ続く...
    ],
    "worldSize": {
        "x": 128,
        "y": 64,
        "z": 128,
    }
}
````

## OBJフォーマット
* 座標系(上=Z+ 右=X+ 奥=Y+)
* 回転モード: XYZオイラー角

## 外部プログラムとの連携
~~様々なバックエンドで地形を利用可能にするため、~~  
~~`ofPlanet`は特定のプラットフォームやフレームワークに依存した形式の出力を行いません。~~  
~~代わりに、`ofPlanet`を利用するユーザは適切に出力をパースする必要があります。~~  
~~現在のところ、`ofPlanet`に対応したバックエンドの一つとして、`uniPlanet`を提供しています。~~

~~[uniPlanet](https://github.com/desktopgame/uniPlanet)~~

uniPlanetは現状でも使用可能ですが、OBJをエクスポートして  それをUnityで読み込むほうが軽量です。  
そのままだと当たり判定がつきませんが、[SAColliderBuilder](https://assetstore.unity.com/packages/tools/sacolliderbuilder-15058)を使えば当たり判定もつけられます。  
※SAColliderBuilderは私の制作したアセットではありません。

# ライセンス
このアプリケーションは以下のライブラリを使用して実装されました。
* [openframeworks](https://openframeworks.cc/ja/)
* [ofxImGui](https://github.com/jvcleave/ofxImGui)
* [soil](https://www.lonesock.net/soil.html)
* [lua](https://www.lua.org/) 
* [thelink2012/any](https://github.com/thelink2012/any/blob/master/any.hpp)
* [kazuho/picojson](https://github.com/kazuho/picojson)
* [Reputeless/PerlinNoise](https://github.com/Reputeless/PerlinNoise)

# アセット
このアプリケーションには以下のアセットが含まれています。  
(使わなくても動作は可能です。)
* [Stone Texture](https://www.deviantart.com/akinuri/art/Stone-Textures-244325727)
* [Sand Texture](https://www.deviantart.com/akinuri/art/Sand-Textures-244325158)
* [Grass Texture](https://www.deviantart.com/akinuri/art/Grass-Textures-218366439)
* [RPG Maker Tiles](https://www.deviantart.com/ayene-chan/art/RPG-Maker-Tiles-255998841)
* [RPG-Maker-Tiles-II](https://www.deviantart.com/ayene-chan/art/RPG-Maker-Tiles-II-261010059)