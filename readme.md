# ofPlanet
ofPlanetは地形生成エディターです。  
表示にはOpenGL, openframeworks を使用しており、  
地形生成部分はluaで実装されています。

## ビルド
[openframeworks](https://openframeworks.cc/ja/)  

ofPlanetはopenframeworksのライブラリを前提とした設定になっています。  
ただしくライブラリを参照するため、以下のディレクトリにプロジェクトを配置してください
````
C:/your/directory/of_v0.10.1_vs2017_release/apps/myApps/ofPlanet/ofPlanet
````


## 構成ファイル
地形生成するにあたり、ofPlanet.exe以外にもファイルが必要です。

### ./textures.json
````
{
  //テクスチャを読み込む基準となる場所
  "baseDirectory": "./data/assets/image/block",
  "textures": [
    {
      // 基本となるファイル名
      "baseFileName": "DirtBlock",
      // 面ごとの名前を定義します。
      // この場合、DirtBlockSide.png が全ての面に適用されます。
      "mappingRule": {
        "all": "Side"
      },
      // ブロックからこのテクスチャを参照するための名前です。
      "reference": "DirtBlock"
    },
    {
      "baseFileName": "WaterBlock",
      // この場合、
      // 上面 -- WaterBlockTop.png
      // それ以外 -- WaterBlockSide.png として解決されます。
      // 面として使用できるキーワードは top bottom left right front back all です。
      "mappingRule": {
        "all": "Side",
        "top": "Top"
      },
      "reference": "WaterBlock"
    }
    ....
  ]
}
````

### ./blocks.json
ブロックの一覧を定義します。  
````
{
  "blocks": [
    {
      // スクリプトからこのブロックを参照するための名前を指定します。
      "reference": "Dirt",
      // 対応するテクスチャを指定します。
      "texture": "DirtBlock"
    },
    ...
  ]
}
````